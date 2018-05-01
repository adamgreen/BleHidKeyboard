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
// HID Keyboard Definitions
#ifndef HID_KEYBOARD_H_
#define HID_KEYBOARD_H_

// Keyboard/Keypad HID Usage Values.
typedef enum HidKeyboardUsageValues
{
    HID_KEY_NOEVENT = 0x00,
    HID_KEY_ERROR_ROLLOVER = 0x01,
    HID_KEY_POST_FAIL = 0x02,
    HID_KEY_ERROR_UNDEFIEND = 0x03,
    HID_KEY_A = 0x04,
    HID_KEY_B,
    HID_KEY_C,
    HID_KEY_D,
    HID_KEY_E,
    HID_KEY_F,
    HID_KEY_G,
    HID_KEY_H,
    HID_KEY_I,
    HID_KEY_J,
    HID_KEY_K,
    HID_KEY_L,
    HID_KEY_M,
    HID_KEY_N,
    HID_KEY_O,
    HID_KEY_P,
    HID_KEY_Q,
    HID_KEY_R,
    HID_KEY_S,
    HID_KEY_T,
    HID_KEY_U,
    HID_KEY_V,
    HID_KEY_W,
    HID_KEY_X,
    HID_KEY_Y,
    HID_KEY_Z,
    HID_KEY_1 = 0x1E,
    HID_KEY_2,
    HID_KEY_3,
    HID_KEY_4,
    HID_KEY_5,
    HID_KEY_6,
    HID_KEY_7,
    HID_KEY_8,
    HID_KEY_9,
    HID_KEY_0,
    HID_KEY_ENTER = 0x28,
    HID_KEY_ESCAPE = 0x29,
    HID_KEY_BACKSPACE = 0x2A,
    HID_KEY_TAB = 0x2B,
    HID_KEY_SPACEBAR = 0x2C,
    HID_KEY_MINUS = 0x2D,
    HID_KEY_EQUAL = 0x2E,
    HID_KEY_LEFT_BRACKET = 0x2F,
    HID_KEY_RIGHT_BRACKET = 0x30,
    HID_KEY_BACKSLASH = 0x31,
    HID_KEY_SEMICOLON = 0x33,
    HID_KEY_APOSTROPHE = 0x34,
    HID_KEY_GRAVE_ACCENT = 0x35,
    HID_KEY_COMMA = 0x36,
    HID_KEY_PERIOD = 0x37,
    HID_KEY_SLASH = 0x38,
    HID_KEY_CAPS_LOCK = 0x39,
    HID_KEY_F1 = 0x3A,
    HID_KEY_F2,
    HID_KEY_F3,
    HID_KEY_F4,
    HID_KEY_F5,
    HID_KEY_F6,
    HID_KEY_F7,
    HID_KEY_F8,
    HID_KEY_F9,
    HID_KEY_F10,
    HID_KEY_F11,
    HID_KEY_F12,
    HID_KEY_PRINT_SCREEN = 0x46,
    HID_KEY_SCROLL_LOCK = 0x47,
    HID_KEY_PAUSE = 0x48,
    HID_KEY_INSERT = 0x49,
    HID_KEY_HOME = 0x4A,
    HID_KEY_PAGE_UP = 0x4B,
    HID_KEY_DELETE = 0x4C,
    HID_KEY_END = 0x4D,
    HID_KEY_PAGE_DOWN = 0x4E,
    HID_KEY_RIGHT_ARROW = 0x4F,
    HID_KEY_LEFT_ARROW = 0x50,
    HID_KEY_DOWN_ARROW = 0x51,
    HID_KEY_UP_ARROW = 0x52,
    HID_KEYPAD_NUM_LOCK = 0x53,
    HID_KEYPAD_SLASH = 0x54,
    HID_KEYPAD_ASTERISK = 0x55,
    HID_KEYPAD_MINUS = 0x56,
    HID_KEYPAD_PLUS = 0x57,
    HID_KEYPAD_ENTER = 0x58,
    HID_KEYPAD_1 = 0x59,
    HID_KEYPAD_2,
    HID_KEYPAD_3,
    HID_KEYPAD_4,
    HID_KEYPAD_5,
    HID_KEYPAD_6,
    HID_KEYPAD_7,
    HID_KEYPAD_8,
    HID_KEYPAD_9,
    HID_KEYPAD_0,
    HID_KEYPAD_PERIOD = 0x63,
    HID_KEY_APP = 0x65,
    HID_KEY_LEFT_CONTROL = 0xE0,
    HID_KEY_LEFT_SHIFT = 0xE1,
    HID_KEY_LEFT_ALT = 0xE2,
    HID_KEY_LEFT_GUI = 0xE3,
    HID_KEY_RIGHT_CONTROL = 0xE4,
    HID_KEY_RIGHT_SHIFT = 0xE5,
    HID_KEY_RIGHT_ALT = 0xE6,
    HID_KEY_RIGHT_GUI = 0xE7,
    HID_KEY_FN = 0xFF // Special usage for the FN key on my particular keyboard.
} HidKeyboardUsageValues;

// Keyboard LED HID Usage Values.
typedef enum HidKeyboardLedUsageValues
{
    HID_LED_UNDEFINED = 0,
    HID_LED_NUM_LOCK = 1,
    HID_LED_CAPS_LOCK = 2,
    HID_LED_SCROLL_LOCK = 3,
    HID_LED_COMPOSE = 4,
    HID_LED_KANA = 5
} HidKeyboardLedUsageValues;

// Keyboard modifier bitmask values used in input report.
#define HID_INPUT_REPORT_KEY_LEFT_CONTROL   (1 << 0)
#define HID_INPUT_REPORT_KEY_LEFT_SHIFT     (1 << 1)
#define HID_INPUT_REPORT_KEY_LEFT_ALT       (1 << 2)
#define HID_INPUT_REPORT_KEY_LEFT_GUI       (1 << 3)
#define HID_INPUT_REPORT_KEY_RIGHT_CONTROL  (1 << 4)
#define HID_INPUT_REPORT_KEY_RIGHT_SHIFT    (1 << 5)
#define HID_INPUT_REPORT_KEY_RIGHT_ALT      (1 << 6)
#define HID_INPUT_REPORT_KEY_RIGHT_GUI      (1 << 7)

// Keyboard LED bitmask values used in output report.
#define HID_OUTPUT_REPORT_LED_NUM_LOCK      (1 << 0)
#define HID_OUTPUT_REPORT_LED_CAPS_LOCK     (1 << 1)
#define HID_OUTPUT_REPORT_LED_SCROLL_LOCK   (1 << 2)
#define HID_OUTPUT_REPORT_LED_COMPOSE       (1 << 3)
#define HID_OUTPUT_REPORT_LED_KANA          (1 << 4)

// Keyboard input report structure.
typedef struct HidKeyboardInputReport
{
    uint8_t modifierBitmask;
    uint8_t reserved;
    uint8_t keyArray[6];
} HidKeyboardInputReport;

// Keyboard output report structure.
typedef struct HidKeyboardOutputReport
{
    uint8_t ledBitmask;
} HidKeyboardOutputReport;

#endif // HID_KEYBOARD_H_
