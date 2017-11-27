/*  Copyright (C) 2017  Adam Green (https://github.com/adamgreen)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/* MCP23018 - I2C based 16-Bit I/O Expander with Open-Drain Outputs. */
#include "MCP23018.h"
#include <app_util_platform.h>
#include <nrf_assert.h>
#include <string.h>
// UNDONE: #include <nrf_delay.h>


// MCP23018 Registers when using default BANK (BANK0).
#define MCP23018_IODIRA_REGISTER        0x00
#define MCP23018_IODIRB_REGISTER        0x01
#define MCP23018_IPOLA_REGISTER         0x02
#define MCP23018_IPOLB_REGISTER         0x03
#define MCP23018_GPINTENA_REGISTER      0x04
#define MCP23018_GPINTENB_REGISTER      0x05
#define MCP23018_DEFVALA_REGISTER       0x06
#define MCP23018_DEFVALB_REGISTER       0x07
#define MCP23018_INTCONA_REGISTER       0x08
#define MCP23018_INTCONB_REGISTER       0x09
#define MCP23018_IOCON_REGISTER         0x0A // 0x0B is also valid for this register.
#define MCP23018_GPPUA_REGISTER         0x0C
#define MCP23018_GPPUB_REGISTER         0x0D
#define MCP23018_INTFA_REGISTER         0x0E
#define MCP23018_INTFB_REGISTER         0x0F
#define MCP23018_INTCAPA_REGISTER       0x10
#define MCP23018_INTCAPB_REGISTER       0x11
#define MCP23018_GPIOA_REGISTER         0x12
#define MCP23018_GPIOB_REGISTER         0x13
#define MCP23018_OLATA_REGISTER         0x14
#define MCP23018_OLATB_REGISTER         0x15

// MCP23018_IOCON_REGISTER bits
// Interrupt Clearing Control bit (0: Reading GPIO register clears interrupt. 1: Reading INTCAP clears interrupt.)
#define MCP23018_IOCON_INTCC            (1 << 0)
// Interrupt output Polarity bit (0: Active Low. 1: Active High.)
#define MCP23018_IOCON_INTPOL           (1 << 1)
// Open Drain bit (0: Active driver output. 1: Open-Drain output.)
#define MCP23018_IOCON_ODR              (1 << 2)
// Sequential Operation Mode bit (0: Sequential operation enabled. 1: Sequential operation disabled.)
#define MCP23018_IOCON_SEQOP            (1 << 5)
// Interrupt Mirror bit (0: The INTA/B output pins are not wired together. 
//                       1: The INTA/B output pins are wired OR together.)
#define MCP23018_IOCON_MIRROR           (1 << 6)
// Bank bit (0: The port A and port B registers are sequentially addressed: 1: They aren't sequential.)
#define MCP23018_IOCON_BANK             (1 << 7)

// This module only expects to have two I2C transaction in flight at a time.
#define MCP23018_TWI_MAX_PENDING_TRANSACTIONS 2


static app_twi_t g_twi = APP_TWI_INSTANCE(0);
static  bool     g_hasInitCompleted = false;


// Forward Declarations
static void gpioWriteCompletedCallback(ret_code_t result, void* pvContext);
static void gpioReadCompletedCallback(ret_code_t result, void* pvContext);


uint32_t i2cInit(uint32_t sclPin, uint32_t sdaPin)
{
    nrf_drv_twi_config_t config =
    {
       .scl                = sclPin,
       .sda                = sdaPin,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };
    uint32_t result;

    if (g_hasInitCompleted)
        return NRF_SUCCESS;
        
    APP_TWI_INIT(&g_twi, &config, MCP23018_TWI_MAX_PENDING_TRANSACTIONS, result);
    if (result != NRF_SUCCESS)
        return result;
    g_hasInitCompleted = true;

    return NRF_SUCCESS;
}

void i2cUninit(void)
{
    if (g_hasInitCompleted)
        app_twi_uninit(&g_twi);
}

uint32_t mcp23018Init(MCP23018* pThis, uint8_t i2cAddress)
{
    ASSERT ( pThis );

    memset(pThis, 0, sizeof(*pThis));
    pThis->i2cAddress = i2cAddress;
    // UNDONE: g_isUpdateInProgress = false;

    return NRF_SUCCESS;
}

uint32_t mcp23018SetIoConfiguration(MCP23018* pThis,
                                    bool mirrorInterruptOutputs,
                                    bool openDrainInterruptOutputs,
                                    bool interruptActiveHigh,
                                    bool intcapReadClearsInterrupt)
{
    // Default all bits to 0.
    // BANK and SEQOP bits will only be set to 0 because other routines expect them in that state.
    uint8_t configurationValue = 0;

    ASSERT ( pThis );
    
    if (mirrorInterruptOutputs)
        configurationValue |= MCP23018_IOCON_MIRROR;
    if (openDrainInterruptOutputs)
        configurationValue |= MCP23018_IOCON_ODR;
    if (interruptActiveHigh)
        configurationValue |= MCP23018_IOCON_INTPOL;
    if (intcapReadClearsInterrupt)
        configurationValue |= MCP23018_IOCON_INTCC;

    uint8_t setConfiguration[] = { MCP23018_IOCON_REGISTER, configurationValue };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setConfiguration, sizeof(setConfiguration), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018SetIoDirections(MCP23018* pThis, uint8_t portAInputBits, uint8_t portBInputBits)
{
    ASSERT ( pThis );

    // Uses sequential writes to BANK0 to write both ports at once.
    uint8_t setIoDirection[] = { MCP23018_IODIRA_REGISTER, portAInputBits, portBInputBits };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setIoDirection, sizeof(setIoDirection), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018SetGpioPullUps(MCP23018* pThis, uint8_t portAPullUpBits, uint8_t portBPullUpBits)
{
    ASSERT ( pThis );

    // Uses sequential writes to BANK0 to write both ports at once.
    uint8_t setPullUps[] = { MCP23018_GPPUA_REGISTER, portAPullUpBits, portBPullUpBits };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setPullUps, sizeof(setPullUps), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018SetDefaultValues(MCP23018* pThis, uint8_t portADefaultValueBits, uint8_t portBDefaultValueBits)
{
    ASSERT ( pThis );

    // Uses sequential writes to BANK0 to write both ports at once.
    uint8_t setDefaultValues[] = { MCP23018_DEFVALA_REGISTER, portADefaultValueBits, portBDefaultValueBits };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setDefaultValues, sizeof(setDefaultValues), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018SetInterruptControlValues(MCP23018* pThis,
                                           uint8_t portACompareToDefaultValueBits, 
                                           uint8_t portBCompareToDefaultValueBits)
{
    ASSERT ( pThis );

    // Uses sequential writes to BANK0 to write both ports at once.
    uint8_t setConfigValues[] = 
    { 
        MCP23018_INTCONA_REGISTER,
        portACompareToDefaultValueBits, 
        portBCompareToDefaultValueBits 
    };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setConfigValues, sizeof(setConfigValues), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018SetGpioInterruptEnables(MCP23018* pThis, 
                                         uint8_t portAInterruptEnableBits, 
                                         uint8_t portBInterruptEnableBits)
{
    ASSERT ( pThis );

    // Uses sequential writes to BANK0 to write both ports at once.
    uint8_t setGpioInterruptEnables[] =
    { 
        MCP23018_GPINTENA_REGISTER,
        portAInterruptEnableBits, 
        portBInterruptEnableBits 
    };
    app_twi_transfer_t transfer[] =
    {
        APP_TWI_WRITE(pThis->i2cAddress, setGpioInterruptEnables, sizeof(setGpioInterruptEnables), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t mcp23018AsyncSetGpio(MCP23018* pThis, uint8_t portA, uint8_t portB)
{
    ASSERT ( pThis );

    if (pThis->writeInProgress)
        return NRF_ERROR_BUSY;
        
    // Uses sequential writes to BANK0 to write both ports at once.
    pThis->gpioWrite[0] = MCP23018_GPIOA_REGISTER;
    pThis->gpioWrite[1] = portA;
    pThis->gpioWrite[2] = portB;

    pThis->writeTransfer.p_data    = pThis->gpioWrite;
    pThis->writeTransfer.length    = sizeof(pThis->gpioWrite);
    pThis->writeTransfer.operation = APP_TWI_WRITE_OP(pThis->i2cAddress);
    pThis->writeTransfer.flags     = 0;

    pThis->writeTransaction.callback            = gpioWriteCompletedCallback;
    pThis->writeTransaction.p_user_data         = pThis;
    pThis->writeTransaction.p_transfers         = &pThis->writeTransfer;
    pThis->writeTransaction.number_of_transfers = 1;

    pThis->writeInProgress = true;
    return app_twi_schedule(&g_twi, &pThis->writeTransaction);
}

static void gpioWriteCompletedCallback(ret_code_t result, void* pvContext)
{
    MCP23018* pThis = (MCP23018*)pvContext;

    ASSERT ( pThis );
    pThis->writeInProgress = false;
}

uint32_t mcp23018AsyncGetGpio(MCP23018* pThis, uint8_t* pPortA, uint8_t* pPortB)
{
    static const uint8_t gpioRegisterAddress[] = { MCP23018_GPIOA_REGISTER };

    ASSERT ( pThis );

    if (pThis->readInProgress)
        return NRF_ERROR_BUSY;

    // Remember the output buffers so that they can be filled in when the read completes.
    pThis->pPortARead = pPortA;
    pThis->pPortBRead = pPortB;

    // Uses sequential writes to BANK0 to write both ports at once.
    // First transfer is a write with the register address.
    pThis->readTransfers[0].p_data    = (uint8_t*)&gpioRegisterAddress;
    pThis->readTransfers[0].length    = sizeof(gpioRegisterAddress);
    pThis->readTransfers[0].operation = APP_TWI_WRITE_OP(pThis->i2cAddress);
    pThis->readTransfers[0].flags     = APP_TWI_NO_STOP;

    // Second transfer supplies the buffer to read the 2 GPIO ports into.
    pThis->readTransfers[1].p_data    = pThis->gpioRead;
    pThis->readTransfers[1].length    = sizeof(pThis->gpioRead);
    pThis->readTransfers[1].operation = APP_TWI_READ_OP(pThis->i2cAddress);
    pThis->readTransfers[1].flags     = 0;

    pThis->readTransaction.callback            = gpioReadCompletedCallback;
    pThis->readTransaction.p_user_data         = pThis;
    pThis->readTransaction.p_transfers         = pThis->readTransfers;
    pThis->readTransaction.number_of_transfers = sizeof(pThis->readTransfers) / sizeof(pThis->readTransfers[0]);

    pThis->readInProgress = true;
    return app_twi_schedule(&g_twi, &pThis->readTransaction);
}

static void gpioReadCompletedCallback(ret_code_t result, void* pvContext)
{
    MCP23018* pThis = (MCP23018*)pvContext;

    ASSERT ( pThis );

    *pThis->pPortARead = pThis->gpioRead[0];
    *pThis->pPortBRead = pThis->gpioRead[1];
    pThis->readInProgress = false;
}

bool mcp23018HasAsyncSetCompleted(MCP23018* pThis)
{
    ASSERT ( pThis );

    return !pThis->writeInProgress;
}

bool mcp23018HasAsyncGetCompleted(MCP23018* pThis)
{
    return !pThis->readInProgress;
}



#ifdef UNDONE
// PCA9685 Registers
#define PCA9685_MODE1_REGISTER          0x00
#define PCA9685_MODE2_REGISTER          0x01
#define PCA9685_SUBADDR1_REGISTER       0x02
#define PCA9685_SUBADDR2_REGISTER       0x03
#define PCA9685_SUBADDR3_REGISTER       0x04
#define PCA9685_ALLCALLADR_REGISTER     0x05
#define PCA9685_LED0_ON_L_REGISTER      0x06
#define PCA9685_LED0_ON_H_REGISTER      0x07
#define PCA9685_LED0_OFF_L_REGISTER     0x08
#define PCA9685_LED0_OFF_H_REGISTER     0x09
#define PCA9685_LED1_ON_L_REGISTER      0x0A
#define PCA9685_LED1_ON_H_REGISTER      0x0B
#define PCA9685_LED1_OFF_L_REGISTER     0x0C
#define PCA9685_LED1_OFF_H_REGISTER     0x0D
#define PCA9685_LED2_ON_L_REGISTER      0x0E
#define PCA9685_LED2_ON_H_REGISTER      0x0F
#define PCA9685_LED2_OFF_L_REGISTER     0x10
#define PCA9685_LED2_OFF_H_REGISTER     0x11
#define PCA9685_LED3_ON_L_REGISTER      0x12
#define PCA9685_LED3_ON_H_REGISTER      0x13
#define PCA9685_LED3_OFF_L_REGISTER     0x14
#define PCA9685_LED3_OFF_H_REGISTER     0x15
#define PCA9685_LED4_ON_L_REGISTER      0x16
#define PCA9685_LED4_ON_H_REGISTER      0x17
#define PCA9685_LED4_OFF_L_REGISTER     0x18
#define PCA9685_LED4_OFF_H_REGISTER     0x19
#define PCA9685_LED5_ON_L_REGISTER      0x1A
#define PCA9685_LED5_ON_H_REGISTER      0x1B
#define PCA9685_LED5_OFF_L_REGISTER     0x1C
#define PCA9685_LED5_OFF_H_REGISTER     0x1D
#define PCA9685_LED6_ON_L_REGISTER      0x1E
#define PCA9685_LED6_ON_H_REGISTER      0x1F
#define PCA9685_LED6_OFF_L_REGISTER     0x20
#define PCA9685_LED6_OFF_H_REGISTER     0x21
#define PCA9685_LED7_ON_L_REGISTER      0x22
#define PCA9685_LED7_ON_H_REGISTER      0x23
#define PCA9685_LED7_OFF_L_REGISTER     0x24
#define PCA9685_LED7_OFF_H_REGISTER     0x25
#define PCA9685_LED8_ON_L_REGISTER      0x26
#define PCA9685_LED8_ON_H_REGISTER      0x27
#define PCA9685_LED8_OFF_L_REGISTER     0x28
#define PCA9685_LED8_OFF_H_REGISTER     0x29
#define PCA9685_LED9_ON_L_REGISTER      0x2A
#define PCA9685_LED9_ON_H_REGISTER      0x2B
#define PCA9685_LED9_OFF_L_REGISTER     0x2C
#define PCA9685_LED9_OFF_H_REGISTER     0x2D
#define PCA9685_LED10_ON_L_REGISTER     0x2E
#define PCA9685_LED10_ON_H_REGISTER     0x2F
#define PCA9685_LED10_OFF_L_REGISTER    0x30
#define PCA9685_LED10_OFF_H_REGISTER    0x31
#define PCA9685_LED11_ON_L_REGISTER     0x32
#define PCA9685_LED11_ON_H_REGISTER     0x33
#define PCA9685_LED11_OFF_L_REGISTER    0x34
#define PCA9685_LED11_OFF_H_REGISTER    0x35
#define PCA9685_LED12_ON_L_REGISTER     0x36
#define PCA9685_LED12_ON_H_REGISTER     0x37
#define PCA9685_LED12_OFF_L_REGISTER    0x38
#define PCA9685_LED12_OFF_H_REGISTER    0x39
#define PCA9685_LED13_ON_L_REGISTER     0x3A
#define PCA9685_LED13_ON_H_REGISTER     0x3B
#define PCA9685_LED13_OFF_L_REGISTER    0x3C
#define PCA9685_LED13_OFF_H_REGISTER    0x3D
#define PCA9685_LED14_ON_L_REGISTER     0x3D
#define PCA9685_LED14_ON_H_REGISTER     0x3F
#define PCA9685_LED14_OFF_L_REGISTER    0x40
#define PCA9685_LED14_OFF_H_REGISTER    0x41
#define PCA9685_LED15_ON_L_REGISTER     0x42
#define PCA9685_LED15_ON_H_REGISTER     0x43
#define PCA9685_LED15_OFF_L_REGISTER    0x44
#define PCA9685_LED15_OFF_H_REGISTER    0x45
#define PCA9685_ALL_LED_ON_L_REGISTER   0xFA
#define PCA9685_ALL_LED_ON_H_REGISTER   0xFB
#define PCA9685_ALL_LED_OFF_L_REGISTER  0xFC
#define PCA9685_ALL_LED_OFF_H_REGISTER  0xFD
#define PCA9685_PRE_SCALE_REGISTER      0xFE
#define PCA9685_TEST_MODE_REGISTER      0xFF

// PCA9685_MODE1_REGISTER bits
#define PCA9685_MODE1_ALLCALL           (1 << 0)
#define PCA9685_MODE1_SUB3              (1 << 1)
#define PCA9685_MODE1_SUB2              (1 << 2)
#define PCA9685_MODE1_SUB1              (1 << 3)
#define PCA9685_MODE1_SLEEP             (1 << 4)
#define PCA9685_MODE1_AI                (1 << 5)
#define PCA9685_MODE1_EXTCLK            (1 << 6)
#define PCA9685_MODE1_RESTART           (1 << 7)

// PCA9685_MODE_2_REGISTER bits
#define PCA9685_MODE2_OUTNE_OFFSET      0
#define PCA9685_MODE2_OUTNE_MASK        (0x3 << PCA9685_MODE2_OUTNE_OFFSET)
#define PCA9685_MODE2_OUTDRV_BIT        (1 << 2)
#define PCA9685_MODE2_OCH_BIT           (1 << 3)
#define PCA9685_MODE2_INVRT_BIT         (1 << 4)

// PCA9685_LEDx_ON_H_REGISTER bit to turn channel full on
#define PCA9685_LEDx_ON_H_FULL_ON       (1 << 4)

// PCA9685_LEDx_OFF_H_REGISTER bit to turn channel full off
#define PCA9685_LEDx_OFF_H_FULL_OFF     (1 << 4)

// This module only expects to have one I2C transaction in flight at a time.
#define SERVO_TWI_MAX_PENDING_TRANSACTIONS 1


static app_twi_t g_twi = APP_TWI_INSTANCE(0);
static uint8_t   g_i2cAddress;
static bool      g_isUpdateInProgress;


static uint32_t enableAutoIncrementRegisterAddressing();
static uint32_t setPrescaler(uint8_t pwmClockPrescaler);
static void     channelWriteCompletedCallback(ret_code_t result, void* pvContext);


uint32_t pca9685Init(uint8_t i2cAddress, uint32_t sdaPin, uint32_t sclPin, uint8_t pwmClockPrescaler)
{
    bool                 hasInitCompleted = false;
    nrf_drv_twi_config_t config =
    {
       .scl                = sclPin,
       .sda                = sdaPin,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };
    uint32_t             result;

    g_i2cAddress = i2cAddress;
    g_isUpdateInProgress = false;

    APP_TWI_INIT(&g_twi, &config, SERVO_TWI_MAX_PENDING_TRANSACTIONS, result);
    if (result != NRF_SUCCESS)
        goto handleError;
    hasInitCompleted = true;

    result = setPrescaler(pwmClockPrescaler);
    if (result != NRF_SUCCESS)
        goto handleError;

    result = enableAutoIncrementRegisterAddressing();
    if (result != NRF_SUCCESS)
        goto handleError;

    nrf_delay_us(500);

    return NRF_SUCCESS;

handleError:
    if (hasInitCompleted)
        app_twi_uninit(&g_twi);
    return result;
}

static uint32_t enableAutoIncrementRegisterAddressing()
{
    static const uint8_t enableAutoIncrement[] = { PCA9685_MODE1_REGISTER, PCA9685_MODE1_AI };
    app_twi_transfer_t const transfer[] =
    {
        APP_TWI_WRITE(g_i2cAddress, enableAutoIncrement, sizeof(enableAutoIncrement), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}


static uint32_t setPrescaler(uint8_t pwmClockPrescaler)
{
    uint8_t setPrescaler[] = { PCA9685_PRE_SCALE_REGISTER, pwmClockPrescaler };
    app_twi_transfer_t const transfer[] =
    {
        APP_TWI_WRITE(g_i2cAddress, setPrescaler, sizeof(setPrescaler), 0)
    };

    return app_twi_perform(&g_twi, transfer, sizeof(transfer)/sizeof(transfer[0]), NULL);
}

uint32_t pca9685AsyncSetChannelOnOffTimes(const uint16_t*    pOnTimes,
                                          const uint16_t*    pOffTimes,
                                          size_t             channelCount)
{
    // 1 byte for register address + channelCount * 12-bit on times + channelCount * 12-bit off times.
    size_t                       commandSize = 1 + channelCount * sizeof(uint16_t) + channelCount * sizeof(uint16_t);
    static uint8_t               commandBuffer[1 + PCA9685_CHANNEL_COUNT * 2 * sizeof(uint16_t)];
    static app_twi_transfer_t    transfer[1];
    static app_twi_transaction_t transaction;
    uint8_t*                     pCurr = commandBuffer;

    if (g_isUpdateInProgress)
        return NRF_ERROR_BUSY;

    *pCurr++ = PCA9685_LED0_ON_L_REGISTER;
    for (size_t i = 0 ; i < channelCount ; i++)
    {
        memcpy(pCurr, pOnTimes++, sizeof(uint16_t));
        pCurr += sizeof(uint16_t);
        memcpy(pCurr, pOffTimes++, sizeof(uint16_t));
        pCurr += sizeof(uint16_t);
    }
    assert ( (pCurr - commandBuffer) == commandSize );

    transfer[0].p_data    = commandBuffer;
    transfer[0].length    = commandSize;
    transfer[0].operation = APP_TWI_WRITE_OP(g_i2cAddress);
    transfer[0].flags     = 0;

    transaction.callback            = channelWriteCompletedCallback;
    transaction.p_user_data         = NULL;
    transaction.p_transfers         = transfer;
    transaction.number_of_transfers = sizeof(transfer) / sizeof(transfer[0]);

    g_isUpdateInProgress = true;
    return app_twi_schedule(&g_twi, &transaction);
}

static void channelWriteCompletedCallback(ret_code_t result, void* pvContext)
{
    g_isUpdateInProgress = false;
}
#endif // UNDONE
