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
#include <nrf_gpio.h>
#include <app_timer.h>
#include <string.h>
#include "KeyboardMatrix.h"
#include "KeyMappings.h"


static void scanKeyboardMatrixCallback(void* pContext);
static void updateKeyboardInputReport(KeyboardMatrix* pThis, HidKeyboardUsageValues key);


uint32_t kbmatrixInit(KeyboardMatrix* pThis,
                      uint8_t i2cAddress1, uint8_t i2cAddress2, 
                      uint32_t sclPin, uint32_t sdaPin, uint32_t interruptPin,
                      uint32_t timerPrescaler,
                      keyStateCallback pCallback, void* pvContext)
{
    ASSERT ( pThis );
    ASSERT ( pCallback );

    memset(pThis, 0, sizeof(*pThis));
    pThis->pCallback = pCallback;
    pThis->pvContext = pvContext;
    pThis->interruptPin = interruptPin;

    uint32_t errorCode = i2cInit(sclPin, sdaPin);
    APP_ERROR_CHECK(errorCode);

    // Initialize both MCP23018 port extender devices.
    errorCode = mcp23018Init(&pThis->mcp23018_1, i2cAddress1);
    APP_ERROR_CHECK(errorCode);
    errorCode = mcp23018Init(&pThis->mcp23018_2, i2cAddress2);
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
    errorCode = mcp23018SetIoConfiguration(&pThis->mcp23018_2,
                                           interruptPinMirroringDisabled,
                                           interruptDrainOutputDisabled,
                                           interruptActiveLow,
                                           interruptClearOnGPIORead);
    APP_ERROR_CHECK(errorCode);

    // Enable pull-up on all pins.
    errorCode = mcp23018SetGpioPullUps(&pThis->mcp23018_1, 0xFF, 0xFF);
    APP_ERROR_CHECK(errorCode);
    errorCode = mcp23018SetGpioPullUps(&pThis->mcp23018_2, 0xFF, 0xFF);
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
#define COLUMN_COUNT (sizeof(g_columnMappings)/sizeof(g_columnMappings[0]))
#define SCAN_STEPS   ((COLUMN_COUNT) * 2)

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
    if (pThis->scanningStarted && (!mcp23018HasAsyncSetCompleted(&pThis->mcp23018_1) || 
                                   !mcp23018HasAsyncSetCompleted(&pThis->mcp23018_2)))
    {
        return;
    }

    uint32_t column = pThis->scanStep >> 1;
    uint32_t readStep = pThis->scanStep & 1;
    if (readStep)
    {
        // Issue the async read for the row outputs in this step.
        errorCode = mcp23018AsyncGetGpio(&pThis->mcp23018_1, &pThis->rowsRead[0], &pThis->rowsRead[1]);
        APP_ERROR_CHECK(errorCode);
        errorCode = mcp23018AsyncGetGpio(&pThis->mcp23018_2, &pThis->rowsRead[2], &pThis->rowsRead[3]);
        APP_ERROR_CHECK(errorCode);
    }
    else
    {
        uint32_t lastColumnIndex;

        // These rows were read when the previous column was pulled low and not the current row.
        if (column == 0)
            lastColumnIndex = COLUMN_COUNT - 1;
        else
            lastColumnIndex = column - 1;
        uint32_t lastColumnMask = g_columnMappings[lastColumnIndex].bitmask;
        
        // The previous step's read has completed so process it.
        uint32_t rowsPressed = ((uint32_t)(pThis->rowsRead[0] ^ 0xFF) << 0)|
                               ((uint32_t)(pThis->rowsRead[1] ^ 0xFF) << 8) |
                               ((uint32_t)(pThis->rowsRead[2] ^ 0xFF) << 16) |
                               ((uint32_t)(pThis->rowsRead[3] ^ 0xFF) << 24);
        // Clear the column just asserted low.
        rowsPressed &= lastColumnMask;
        if (rowsPressed && pThis->scanningStarted)
        {
            uint8_t rowCount = g_columnMappings[lastColumnIndex].count;
            const RowMapping* pMappings = g_columnMappings[lastColumnIndex].pMappings;
            for (int row = 0 ; row <  rowCount ; row++)
            {
                if (rowsPressed & pMappings[row].bitmask)
                {
                    updateKeyboardInputReport(pThis, pMappings[row].usage);
                }
            }
        }

        // Pull one of the column outputs low in this step.
        uint32_t bitmask = g_columnMappings[column].bitmask;

        // UNDONE: This should be done async as well. I could add a method to MCP23018 that does both in one I2C transaction.
        // Only the current column should be output (direction bit == 0) and the rest should be inputs.
        errorCode =  mcp23018SetIoDirections(&pThis->mcp23018_1, bitmask & 0xFF, (bitmask >> 8) & 0xFF);
        APP_ERROR_CHECK(errorCode);
        errorCode =  mcp23018SetIoDirections(&pThis->mcp23018_2, (bitmask >> 16) & 0xFF, (bitmask >> 24) & 0xFF);
        APP_ERROR_CHECK(errorCode);

        // Set the current column to 0 and leave all of the inputs as 1.
        errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, bitmask & 0xFF, (bitmask >> 8) & 0xFF);
        APP_ERROR_CHECK(errorCode);
        errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_2, (bitmask >> 16) & 0xFF, (bitmask >> 24) & 0xFF);
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

    // UNDONE: Add special handling for FN key as necessary.
    if (key == 0x00 || key == 0xFF)
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

// UNDONE: Customize this for my keyboard.
uint32_t kbmatrixConfigureForWakeupOnSpacebar(KeyboardMatrix* pThis)
{
    // Configure INTB to go low when the spacebar's row goes low.
    uint32_t errorCode = mcp23018SetDefaultValues(&pThis->mcp23018_1, 0x00, 0x02);
    APP_ERROR_CHECK(errorCode);
    errorCode = mcp23018SetInterruptControlValues(&pThis->mcp23018_1, 0x00, 0x02);
    APP_ERROR_CHECK(errorCode);
    errorCode = mcp23018SetGpioInterruptEnables(&pThis->mcp23018_1, 0x00, 0x02);
    APP_ERROR_CHECK(errorCode);

    // Pull the spacebar's column low so that its row will go low when the spacebar is pressed, triggering interrupt.
    errorCode = mcp23018AsyncSetGpio(&pThis->mcp23018_1, ~1, 0x00);
    APP_ERROR_CHECK(errorCode);

    // Configure the INTB pin to be a sense/wakeup source.
    nrf_gpio_cfg_sense_input(pThis->interruptPin, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
    
    return errorCode;
}
