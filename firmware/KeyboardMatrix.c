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
/* KeyboardMatrix module which makes use of the I2C based MCP23018 I/O Expander to interface with the column inputs
   and row outputs of a PC matrix keyboard. */
#include <app_error.h>
#include <app_timer.h>
#include <string.h>
#include "KeyboardMatrix.h"


static void scanKeyboardMatrixCallback(void* pContext);


uint32_t kbmatrixInit(KeyboardMatrix* pThis,
                      uint8_t i2cAddress1, uint8_t i2cAddress2, 
                      uint32_t sclPin, uint32_t sdaPin,
                      uint32_t timerPrescaler,
                      keyStateCallback* pCallback, void* pvContext)
{
    ASSERT ( pThis );
// UNDONE:    ASSERT ( pCallback );

    memset(pThis, 0, sizeof(*pThis));
    pThis->pCallback = pCallback;
    pThis->pvContext = pvContext;

    uint32_t errorCode = i2cInit(sclPin, sdaPin);
    APP_ERROR_CHECK(errorCode);

    // UNDONE: Need to initialize two MCP23018 devices.
    errorCode = mcp23018Init(&pThis->mcp23018_1, i2cAddress1);
    APP_ERROR_CHECK(errorCode);

    bool interruptPinMirroringDisabled = false;
    bool interruptDrainOutputDisabled = false;
    bool interruptActiveLow = false;
    bool interruptClearOnGPIORead = false;
    errorCode = mcp23018SetIoConfiguration(&pThis->mcp23018_1,
                                           interruptPinMirroringDisabled,
                                           interruptDrainOutputDisabled,
                                           interruptActiveLow,
                                           interruptClearOnGPIORead);
    APP_ERROR_CHECK(errorCode);

    // UNDONE: I think I will need a total of 6 row inputs in port B of the second MCP23018.
    // Set all outputs to 1 before setting direction register.
    errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, ~0x00, ~0x03);
    APP_ERROR_CHECK(errorCode);
    while (!mcp23018HasAsyncSetCompleted(&pThis->mcp23018_1))
    {
    }

    // UNDONE: I think I will need a total of 6 row inputs in port B of the second MCP23018.
    // PortA-0 pin is output and PortB-0 to PortB-1 are inputs. 
    // Default all other pins to be outputs to conserve power.
    errorCode =  mcp23018SetIoDirections(&pThis->mcp23018_1, 0x00, 0x03);
    APP_ERROR_CHECK(errorCode);

    // Enable pull-up on the PortB input pins.
    errorCode = mcp23018SetGpioPullUps(&pThis->mcp23018_1, 0x00, 0x03);
    APP_ERROR_CHECK(errorCode);

    // Start scanning timer.
    pThis->timerId = &pThis->timerData;
    errorCode = app_timer_create(&pThis->timerId,
                                APP_TIMER_MODE_REPEATED,
                                scanKeyboardMatrixCallback);
    APP_ERROR_CHECK(errorCode);

    // NOTE: APP_TIMER_TICKS results in 64-bit math.
    uint32_t scanInterval = APP_TIMER_TICKS(1000, timerPrescaler);
    errorCode = app_timer_start(pThis->timerId, scanInterval, pThis);
    APP_ERROR_CHECK(errorCode);

    return errorCode;
}

static void scanKeyboardMatrixCallback(void* pContext)
{
    KeyboardMatrix* pThis = (KeyboardMatrix*)pContext;

    // UNDONE: Just hacking in here for now.
    static bool     firstScan = true;
    static uint32_t scanStep = 0;
    static uint8_t  portA = 0xFF;
    static uint8_t  portB = 0xFF;
    uint32_t        errorCode = 0;

    if (!firstScan)
    {
        // Make sure that previous async command completed.
        ASSERT ( mcp23018HasAsyncSetCompleted(&pThis->mcp23018_1) );

        // On even steps, the previous step's read should have completed so print it out.
        if ((scanStep & 1) == 0)
        {
            printf("%u-%u\n", (scanStep == 2) ? 0 : 1, portB ^ 0x03);
        }
    }
    firstScan = false;
    
    switch (scanStep)
    {
    case 0:
        // Pull first column low.
        errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, 0xFE, 0x00);
        APP_ERROR_CHECK(errorCode);
        break;
    case 1:
    case 3:
        // Read the rows.
        errorCode = mcp23018AsyncGetGpio(&pThis->mcp23018_1, &portA, &portB);
        APP_ERROR_CHECK(errorCode);
        break;
    case 2:
        // Pull second column low.
        errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, 0xFD, 0x00);
        APP_ERROR_CHECK(errorCode);
        break;
    default:
        ASSERT ( false );
    }
    scanStep = (scanStep + 1) % 4;
}

void kbmatrixUninit(KeyboardMatrix* pThis)
{
    ASSERT ( pThis );

    i2cUninit();
}
