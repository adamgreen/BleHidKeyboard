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
#ifndef KEYBOARD_MATRIX_H__
#define KEYBOARD_MATRIX_H__

#include <nrf_assert.h>
#include <stdint.h>
#include "HidKeyboard.h"
#include "MCP23018.h"


// Type of function pointer called when keystate has changed (key pressed or released).
typedef void (*keyStateCallback) (HidKeyboardInputReport* pReport, void* pvContext, bool sleepPressed);


typedef struct KeyboardMatrix
{
    keyStateCallback        pCallback;
    void*                   pvContext;
    app_timer_id_t          timerId;
    uint32_t                scanStep;
    uint32_t                scanInterval;
    uint32_t                intA1Pin;
    uint32_t                intB1Pin;
    uint32_t                intA2Pin;
    uint32_t                intB2Pin;
    uint32_t                spacebarInterruptPin;
    uint32_t                spacebarColumnBitmask;
    uint32_t                spacebarRowBitmask;
    uint8_t                 rowsRead[4];
    uint8_t                 flags;
    bool                    scanningStarted;
    // Use two of the 16-bit I/O expanders for interfacing to the keyboard matrix.
    MCP23018                mcp23018_1;
    MCP23018                mcp23018_2;
    HidKeyboardInputReport  reportPrev;
    HidKeyboardInputReport  reportCurr;
    app_timer_t             timerData;
} KeyboardMatrix;


uint32_t kbmatrixInit(KeyboardMatrix* pThis,
                      uint8_t i2cAddress1, uint8_t i2cAddress2, 
                      uint32_t sclPin, uint32_t sdaPin, 
                      uint32_t intA1Pin, uint32_t intB1Pin, uint32_t intA2Pin, uint32_t intB2Pin,
                      uint32_t timerPrescaler,
                      keyStateCallback pCallback, void* pvContext);
void     kbmatrixUninit(KeyboardMatrix* pThis);

uint32_t kbmatrixConfigureForWakeupOnSpacebar(KeyboardMatrix* pThis);

#endif // KEYBOARD_MATRIX_H__
