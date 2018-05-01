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
#ifndef MCP23018_H__
#define MCP23018_H__

#include <app_twi.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct MCP23018
{
    uint8_t*              pPortARead;
    uint8_t*              pPortBRead;
    app_twi_transfer_t    writeTransfers[2];
    app_twi_transfer_t    readTransfers[2];
    app_twi_transaction_t writeTransaction;
    app_twi_transaction_t readTransaction;
    uint8_t               i2cAddress;
    uint8_t               setIoDirection[1+2];
    uint8_t               gpioWrite[1+2];
    uint8_t               gpioRead[2];
    bool                  writeInProgress;
    bool                  readInProgress;
} MCP23018;


uint32_t i2cInit(uint32_t sclPin, uint32_t sdaPin, uint8_t transactionQueueSize);
void     i2cUninit(void);

uint32_t mcp23018Init(MCP23018* pThis, uint8_t i2cAddress);

uint32_t mcp23018SetIoConfiguration(MCP23018* pThis,
                                    bool mirrorInterruptOutputs,
                                    bool openDrainInterruptOutputs,
                                    bool interruptActiveHigh,
                                    bool intcapReadClearsInterrupt);
uint32_t mcp23018SetIoDirections(MCP23018* pThis, uint8_t portAInputBits, uint8_t portBInputBits);
uint32_t mcp23018SetGpioPullUps(MCP23018* pThis, uint8_t portAPullUpBits, uint8_t portBPullUpBits);
uint32_t mcp23018SetDefaultValues(MCP23018* pThis, uint8_t portADefaultValueBits, uint8_t portBDefaultValueBits);
uint32_t mcp23018SetInterruptControlValues(MCP23018* pThis,
                                           uint8_t portACompareToDefaultValueBits, 
                                           uint8_t portBCompareToDefaultValueBits);
uint32_t mcp23018SetGpioInterruptEnables(MCP23018* pThis, 
                                         uint8_t portAInterruptEnableBits, 
                                         uint8_t portBInterruptEnableBits);
uint32_t mcp23018AsyncSetGpio(MCP23018* pThis, uint8_t portA, uint8_t portB);
uint32_t mcp23018AsyncSetOutputAndPullLow(MCP23018* pThis, uint8_t portAMask, uint8_t portBMask);
uint32_t mcp23018AsyncGetGpio(MCP23018* pThis, uint8_t* pPortA, uint8_t* pPortB);
bool     mcp23018HasAsyncSetCompleted(MCP23018* pThis);
bool     mcp23018HasAsyncGetCompleted(MCP23018* pThis);


#endif // MCP23018_H__
