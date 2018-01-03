/*  Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

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
static void updateKeyboardInputReport(KeyboardMatrix* pThis, HidKeyboardUsageValues key);


uint32_t kbmatrixInit(KeyboardMatrix* pThis,
                      uint8_t i2cAddress1, uint8_t i2cAddress2, 
                      uint32_t sclPin, uint32_t sdaPin,
                      uint32_t timerPrescaler,
                      keyStateCallback pCallback, void* pvContext)
{
    ASSERT ( pThis );
    ASSERT ( pCallback );

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
                                APP_TIMER_MODE_SINGLE_SHOT,
                                scanKeyboardMatrixCallback);
    APP_ERROR_CHECK(errorCode);

    // NOTE: APP_TIMER_TICKS results in 64-bit math.
    pThis->scanInterval = APP_TIMER_TICKS(2, timerPrescaler);
    errorCode = app_timer_start(pThis->timerId, pThis->scanInterval, pThis);
    APP_ERROR_CHECK(errorCode);

    return errorCode;
}

// UNDONE: Move these constants
#define COLUMN_COUNT 2
#define ROW_COUNT    2
#define SCAN_STEPS   ((COLUMN_COUNT) * 2)
#define ROW_BITMASK  ((1 << (ROW_COUNT)) - 1)

static const HidKeyboardUsageValues g_keymap[COLUMN_COUNT][ROW_COUNT] =
{
    { HID_KEY_A,        HID_KEY_SPACEBAR },
    { HID_KEY_PERIOD,   HID_KEY_PERIOD }
};

static void scanKeyboardMatrixCallback(void* pContext)
{
    static const uint8_t overflowKeyArray[6] = {HID_KEY_ERROR_ROLLOVER,
                                                HID_KEY_ERROR_ROLLOVER,
                                                HID_KEY_ERROR_ROLLOVER,
                                                HID_KEY_ERROR_ROLLOVER,
                                                HID_KEY_ERROR_ROLLOVER,
                                                HID_KEY_ERROR_ROLLOVER};
    KeyboardMatrix* pThis = (KeyboardMatrix*)pContext;
    uint32_t        errorCode = 0;

    // First queue up the next timer tick.
    // Don't use a repeating timer since it can result in the application queue filling up when some BLE events run
    // too long in main().
    errorCode = app_timer_start(pThis->timerId, pThis->scanInterval, pThis);
    APP_ERROR_CHECK(errorCode);

    // Just skip this timer tick if the previous I/O hasn't completed yet and try again on the next one.
    if (pThis->scanningStarted && !mcp23018HasAsyncSetCompleted(&pThis->mcp23018_1))
    {
        return;
    }

    uint32_t column = pThis->scanStep >> 1;
    uint32_t readStep = pThis->scanStep & 1;
    if (readStep)
    {
        // Read in row outputs from port B in this step.
        errorCode = mcp23018AsyncGetGpio(&pThis->mcp23018_1, NULL, &pThis->rowsRead);
        APP_ERROR_CHECK(errorCode);
    }
    else
    {
        // The previous step's read should have completed so print it out.
        uint8_t rowsPressed = (pThis->rowsRead ^ ROW_BITMASK) & ROW_BITMASK;
        if (rowsPressed && pThis->scanningStarted)
        {
            uint8_t  rowMask = 1;
            uint32_t lastColumnRead;

            // These rows were read when the previous column was pulled low and not the current row.
            if (column == 0)
                lastColumnRead = COLUMN_COUNT - 1;
            else
                lastColumnRead = column - 1;
            for (int row = 0 ; row < ROW_COUNT ; row++)
            {
                if (rowMask & rowsPressed)
                {
                    updateKeyboardInputReport(pThis, g_keymap[lastColumnRead][row]);

                    // UNDONE: Need a way to turn this off and on.
                    //printf("%lu-%u ", lastColumnRead, row);
                }
                rowMask <<= 1;
            }
            //printf("\n");
        }

        // Pull one of the column outputs low on port A in this step.
        uint32_t bitmask = 0xFFFFFFFF & ~(1 << column);
        errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, bitmask & 0xFF, 0x00);
        APP_ERROR_CHECK(errorCode);
    }
    
    // At the beginning of a pass, finalize the previous scan results and prepare to fill in the report for this scan.
    if (pThis->scanStep == 0 && pThis->scanningStarted)
    {
        if (pThis->overflowDetected)
        {
            // Set all keys to overflow error if more than 6 keys were pressed at once.
            memcpy(pThis->reportCurr.keyArray, overflowKeyArray, sizeof(pThis->reportCurr.keyArray));
        }

        if (0 != memcmp(&pThis->reportPrev, &pThis->reportCurr, sizeof(pThis->reportPrev)))
        {
            // Key state has changed since the last scan so it should be sent to host via callback.
            pThis->pCallback(&pThis->reportCurr, pThis->pvContext);
        }

        // Remember the current report in prevPrev.
        pThis->reportPrev = pThis->reportCurr;
        
        // Initialize reportCurr to be filled in with this scan's results.
        memset(&pThis->reportCurr, 0, sizeof(pThis->reportCurr));
        pThis->overflowDetected = false;
    }

    pThis->scanningStarted = true;
    pThis->scanStep++;
    if (pThis->scanStep >= SCAN_STEPS)
        pThis->scanStep = 0;
}

static void updateKeyboardInputReport(KeyboardMatrix* pThis, HidKeyboardUsageValues key)
{
    const size_t length = sizeof(pThis->reportCurr.keyArray);
    size_t i;

    if (key == 0x00)
    {
        // Key doesn't have recognized HID code so just return.
        return;
    }

    // Some usage values are stored in keyArray of input report and other are bits in modifierBitmask.
    if (key >= HID_KEY_LEFT_CONTROL && key <= HID_KEY_RIGHT_GUI)
    {
        // Needs to set bit in modifierBitmask field of input report.
        uint8_t modifierBit = (1 << (key - HID_KEY_LEFT_CONTROL));
        pThis->reportCurr.modifierBitmask |= modifierBit;
    }
    else
    {
        // Needs to set value in keyArray field of input report.
        for (i = 0 ; i < length ; i++)
        {
            if (pThis->reportCurr.keyArray[i] == key || pThis->reportCurr.keyArray[i] == 0x00)
            {
                pThis->reportCurr.keyArray[i] = key;
                break;
            }
        }

        // Detect and flag if overflow of keyArray has occurred. Overflow error will be sent to host at end of scan
        // cycle if this flag is set.
        if (i == length)
            pThis->overflowDetected = true;
    }
}

void kbmatrixUninit(KeyboardMatrix* pThis)
{
    ASSERT ( pThis );

    i2cUninit();
}
