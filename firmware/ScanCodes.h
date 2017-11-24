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
/* Tables which map AT & PS/2 scan codes (set 2) to ASCII text which represents the key's name. */
#ifndef SCAN_CODES_H_
#define SCAN_CODES_H_

#include "HidKeyboard.h"


// This is the single byte scan code table used for a normal QWERTY keyboard.
static const char* const    g_scanCodesQWERTY[256] =
{
    "<unknown>",    // 00
    "<F9>",         // 01
    "<unknown>",    // 02
    "<F5>",         // 03
    "<F3>",         // 04
    "<F1>",         // 05
    "<F2>",         // 06
    "<F12>",        // 07
    "<unknown>",    // 08
    "<F10>",        // 09
    "<F8>",         // 0A
    "<F6>",         // 0B
    "<F4>",         // 0C
    "<TAB>",        // 0D
    "`",            // 0E
    "<unknown>",    // 0F

    "<unknown>",    // 10
    "<Left ALT>",   // 11
    "<Left Shift>", // 12
    "<unknown>",    // 13
    "<Left CTRL>",  // 14
    "Q",            // 15
    "1",            // 16
    "<unknown>",    // 17
    "<unknown>",    // 18
    "<unknown>",    // 19
    "Z",            // 1A
    "S",            // 1B
    "A",            // 1C
    "W",            // 1D
    "2",            // 1E
    "<unknown>",    // 1F

    "<unknown>",    // 20
    "C",            // 21
    "X",            // 22
    "D",            // 23
    "E",            // 24
    "4",            // 25
    "3",            // 26
    "<unknown>",    // 27
    "<unknown>",    // 28
    " ",            // 29
    "V",            // 2A
    "F",            // 2B
    "T",            // 2C
    "R",            // 2D
    "5",            // 2E
    "<unknown>",    // 2F

    "<unknown>",    // 30
    "N",            // 31
    "B",            // 32
    "H",            // 33
    "G",            // 34
    "Y",            // 35
    "6",            // 36
    "<unknown>",    // 37
    "<unknown>",    // 38
    "<unknown>",    // 39
    "M",            // 3A
    "J",            // 3B
    "U",            // 3C
    "7",            // 3D
    "8",            // 3E
    "<unknown>",    // 3F

    "<unknown>",    // 40
    ",",            // 41
    "K",            // 42
    "I",            // 43
    "O",            // 44
    "0",            // 45
    "9",            // 46
    "<unknown>",    // 47
    "<unknown>",    // 48
    ".",            // 49
    "/",            // 4A
    "L",            // 4B
    ";",            // 4C
    "P",            // 4D
    "-",            // 4E
    "<unknown>",    // 4F

    "<unknown>",    // 50
    "<unknown>",    // 51
    "'",            // 52
    "<unknown>",    // 53
    "[",            // 54
    "=",            // 55
    "<unknown>",    // 56
    "<unknown>",    // 57
    "<Caps Lock>",  // 58
    "<Right Shift>",// 59
    "\n",         // 5A
    "]",            // 5B
    "<unknown>",    // 5C
    "\\",           // 5D
    "<unknown>",    // 5E
    "<unknown>",    // 5F

    "<unknown>",    // 60
    "<unknown>",    // 61
    "<unknown>",    // 62
    "<unknown>",    // 63
    "<unknown>",    // 64
    "<unknown>",    // 65
    "\b",           // 66
    "<unknown>",    // 67
    "<unknown>",    // 68
    "1",            // 69
    "<unknown>",    // 6A
    "4",            // 6B
    "7",            // 6C
    "<unknown>",    // 6D
    "<unknown>",    // 6E
    "<unknown>",    // 6F

    "0",            // 70
    ".",            // 71
    "2",            // 72
    "5",            // 73
    "6",            // 74
    "8",            // 75
    "<esc>",        // 76
    "<Num Lock>",   // 77
    "<F11>",        // 78
    "+",            // 79
    "3",            // 7A
    "-",            // 7B
    "*",            // 7C
    "9",            // 7D
    "<Scroll Lock>",// 7E
    "<unknown>",    // 7F

    "<unknown>",    // 80
    "<unknown>",    // 81
    "<unknown>",    // 82
    "<F7>",         // 83
    "<unknown>",    // 84
    "<unknown>",    // 85
    "<unknown>",    // 86
    "<unknown>",    // 87
    "<unknown>",    // 88
    "<unknown>",    // 89
    "<unknown>",    // 8A
    "<unknown>",    // 8B
    "<unknown>",    // 8C
    "<unknown>",    // 8D
    "<unknown>",    // 8E
    "<unknown>",    // 8F

    "<unknown>",    // 90
    "<unknown>",    // 91
    "<unknown>",    // 92
    "<unknown>",    // 93
    "<unknown>",    // 94
    "<unknown>",    // 95
    "<unknown>",    // 96
    "<unknown>",    // 97
    "<unknown>",    // 98
    "<unknown>",    // 99
    "<unknown>",    // 9A
    "<unknown>",    // 9B
    "<unknown>",    // 9C
    "<unknown>",    // 9D
    "<unknown>",    // 9E
    "<unknown>",    // 9F

    "<unknown>",    // A0
    "<unknown>",    // A1
    "<unknown>",    // A2
    "<unknown>",    // A3
    "<unknown>",    // A4
    "<unknown>",    // A5
    "<unknown>",    // A6
    "<unknown>",    // A7
    "<unknown>",    // A8
    "<unknown>",    // A9
    "<unknown>",    // AA
    "<unknown>",    // AB
    "<unknown>",    // AC
    "<unknown>",    // AD
    "<unknown>",    // AE
    "<unknown>",    // AF

    "<unknown>",    // B0
    "<unknown>",    // B1
    "<unknown>",    // B2
    "<unknown>",    // B3
    "<unknown>",    // B4
    "<unknown>",    // B5
    "<unknown>",    // B6
    "<unknown>",    // B7
    "<unknown>",    // B8
    "<unknown>",    // B9
    "<unknown>",    // BA
    "<unknown>",    // BB
    "<unknown>",    // BC
    "<unknown>",    // BD
    "<unknown>",    // BE
    "<unknown>",    // BF

    "<unknown>",    // C0
    "<unknown>",    // C1
    "<unknown>",    // C2
    "<unknown>",    // C3
    "<unknown>",    // C4
    "<unknown>",    // C5
    "<unknown>",    // C6
    "<unknown>",    // C7
    "<unknown>",    // C8
    "<unknown>",    // C9
    "<unknown>",    // CA
    "<unknown>",    // CB
    "<unknown>",    // CC
    "<unknown>",    // CD
    "<unknown>",    // CE
    "<unknown>",    // CF

    "<unknown>",    // D0
    "<unknown>",    // D1
    "<unknown>",    // D2
    "<unknown>",    // D3
    "<unknown>",    // D4
    "<unknown>",    // D5
    "<unknown>",    // D6
    "<unknown>",    // D7
    "<unknown>",    // D8
    "<unknown>",    // D9
    "<unknown>",    // DA
    "<unknown>",    // DB
    "<unknown>",    // DC
    "<unknown>",    // DD
    "<unknown>",    // DE
    "<unknown>",    // DF

    "<unknown>",    // E0
    "<unknown>",    // E1
    "<unknown>",    // E2
    "<unknown>",    // E3
    "<unknown>",    // E4
    "<unknown>",    // E5
    "<unknown>",    // E6
    "<unknown>",    // E7
    "<unknown>",    // E8
    "<unknown>",    // E9
    "<unknown>",    // EA
    "<unknown>",    // EB
    "<unknown>",    // EC
    "<unknown>",    // ED
    "<unknown>",    // EE
    "<unknown>",    // EF

    "<unknown>",    // F0
    "<unknown>",    // F1
    "<unknown>",    // F2
    "<unknown>",    // F3
    "<unknown>",    // F4
    "<unknown>",    // F5
    "<unknown>",    // F6
    "<unknown>",    // F7
    "<unknown>",    // F8
    "<unknown>",    // F9
    "<unknown>",    // FA
    "<unknown>",    // FB
    "<unknown>",    // FC
    "<unknown>",    // FD
    "<unknown>",    // FE
    "<unknown>"     // FF
};



// This is the single byte scan code table used for the special ABC (alphabetical order) keyboard.
static const char* const    g_scanCodesABC[256] =
{
    "<unknown>",    // 00
    "<F9>",         // 01
    "<unknown>",    // 02
    "<F5>",         // 03
    "<F3>",         // 04
    "<F1>",         // 05
    "<F2>",         // 06
    "<F12>",        // 07
    "<unknown>",    // 08
    "<F10>",        // 09
    "<F8>",         // 0A
    "<F6>",         // 0B
    "<F4>",         // 0C
    "<TAB>",        // 0D
    "`",            // 0E
    "<unknown>",    // 0F

    "<unknown>",    // 10
    "<Left ALT>",   // 11
    "<Left Shift>", // 12
    "<unknown>",    // 13
    "<Left CTRL>",  // 14
    "A",            // 15
    "1",            // 16
    "<unknown>",    // 17
    "<unknown>",    // 18
    "<unknown>",    // 19
    "T",            // 1A
    "L",            // 1B
    "K",            // 1C
    "B",            // 1D
    "2",            // 1E
    "<unknown>",    // 1F

    "<unknown>",    // 20
    "V",            // 21
    "U",            // 22
    "M",            // 23
    "C",            // 24
    "4",            // 25
    "3",            // 26
    "<unknown>",    // 27
    "<unknown>",    // 28
    " ",            // 29
    "W",            // 2A
    "N",            // 2B
    "E",            // 2C
    "D",            // 2D
    "5",            // 2E
    "<unknown>",    // 2F

    "<unknown>",    // 30
    "Y",            // 31
    "X",            // 32
    "P",            // 33
    "O",            // 34
    "F",            // 35
    "6",            // 36
    "<unknown>",    // 37
    "<unknown>",    // 38
    "<unknown>",    // 39
    "Z",            // 3A
    "Q",            // 3B
    "G",            // 3C
    "7",            // 3D
    "8",            // 3E
    "<unknown>",    // 3F

    "<unknown>",    // 40
    ",",            // 41
    "R",            // 42
    "H",            // 43
    "I",            // 44
    "0",            // 45
    "9",            // 46
    "<unknown>",    // 47
    "<unknown>",    // 48
    ".",            // 49
    "/",            // 4A
    "S",            // 4B
    ";",            // 4C
    "J",            // 4D
    "-",            // 4E
    "<unknown>",    // 4F

    "<unknown>",    // 50
    "<unknown>",    // 51
    "'",            // 52
    "<unknown>",    // 53
    "[",            // 54
    "=",            // 55
    "<unknown>",    // 56
    "<unknown>",    // 57
    "<Caps Lock>",  // 58
    "<Right Shift>",// 59
    "\n",         // 5A
    "]",            // 5B
    "<unknown>",    // 5C
    "\\",           // 5D
    "<unknown>",    // 5E
    "<unknown>",    // 5F

    "<unknown>",    // 60
    "<unknown>",    // 61
    "<unknown>",    // 62
    "<unknown>",    // 63
    "<unknown>",    // 64
    "<unknown>",    // 65
    "\b",           // 66
    "<unknown>",    // 67
    "<unknown>",    // 68
    "1",            // 69
    "<unknown>",    // 6A
    "4",            // 6B
    "7",            // 6C
    "<unknown>",    // 6D
    "<unknown>",    // 6E
    "<unknown>",    // 6F

    "0",            // 70
    ".",            // 71
    "2",            // 72
    "5",            // 73
    "6",            // 74
    "8",            // 75
    "<esc>",        // 76
    "<Num Lock>",   // 77
    "<F11>",        // 78
    "+",            // 79
    "3",            // 7A
    "-",            // 7B
    "*",            // 7C
    "9",            // 7D
    "<Scroll Lock>",// 7E
    "<unknown>",    // 7F

    "<unknown>",    // 80
    "<unknown>",    // 81
    "<unknown>",    // 82
    "<F7>",         // 83
    "<unknown>",    // 84
    "<unknown>",    // 85
    "<unknown>",    // 86
    "<unknown>",    // 87
    "<unknown>",    // 88
    "<unknown>",    // 89
    "<unknown>",    // 8A
    "<unknown>",    // 8B
    "<unknown>",    // 8C
    "<unknown>",    // 8D
    "<unknown>",    // 8E
    "<unknown>",    // 8F

    "<unknown>",    // 90
    "<unknown>",    // 91
    "<unknown>",    // 92
    "<unknown>",    // 93
    "<unknown>",    // 94
    "<unknown>",    // 95
    "<unknown>",    // 96
    "<unknown>",    // 97
    "<unknown>",    // 98
    "<unknown>",    // 99
    "<unknown>",    // 9A
    "<unknown>",    // 9B
    "<unknown>",    // 9C
    "<unknown>",    // 9D
    "<unknown>",    // 9E
    "<unknown>",    // 9F

    "<unknown>",    // A0
    "<unknown>",    // A1
    "<unknown>",    // A2
    "<unknown>",    // A3
    "<unknown>",    // A4
    "<unknown>",    // A5
    "<unknown>",    // A6
    "<unknown>",    // A7
    "<unknown>",    // A8
    "<unknown>",    // A9
    "<unknown>",    // AA
    "<unknown>",    // AB
    "<unknown>",    // AC
    "<unknown>",    // AD
    "<unknown>",    // AE
    "<unknown>",    // AF

    "<unknown>",    // B0
    "<unknown>",    // B1
    "<unknown>",    // B2
    "<unknown>",    // B3
    "<unknown>",    // B4
    "<unknown>",    // B5
    "<unknown>",    // B6
    "<unknown>",    // B7
    "<unknown>",    // B8
    "<unknown>",    // B9
    "<unknown>",    // BA
    "<unknown>",    // BB
    "<unknown>",    // BC
    "<unknown>",    // BD
    "<unknown>",    // BE
    "<unknown>",    // BF

    "<unknown>",    // C0
    "<unknown>",    // C1
    "<unknown>",    // C2
    "<unknown>",    // C3
    "<unknown>",    // C4
    "<unknown>",    // C5
    "<unknown>",    // C6
    "<unknown>",    // C7
    "<unknown>",    // C8
    "<unknown>",    // C9
    "<unknown>",    // CA
    "<unknown>",    // CB
    "<unknown>",    // CC
    "<unknown>",    // CD
    "<unknown>",    // CE
    "<unknown>",    // CF

    "<unknown>",    // D0
    "<unknown>",    // D1
    "<unknown>",    // D2
    "<unknown>",    // D3
    "<unknown>",    // D4
    "<unknown>",    // D5
    "<unknown>",    // D6
    "<unknown>",    // D7
    "<unknown>",    // D8
    "<unknown>",    // D9
    "<unknown>",    // DA
    "<unknown>",    // DB
    "<unknown>",    // DC
    "<unknown>",    // DD
    "<unknown>",    // DE
    "<unknown>",    // DF

    "<unknown>",    // E0
    "<unknown>",    // E1
    "<unknown>",    // E2
    "<unknown>",    // E3
    "<unknown>",    // E4
    "<unknown>",    // E5
    "<unknown>",    // E6
    "<unknown>",    // E7
    "<unknown>",    // E8
    "<unknown>",    // E9
    "<unknown>",    // EA
    "<unknown>",    // EB
    "<unknown>",    // EC
    "<unknown>",    // ED
    "<unknown>",    // EE
    "<unknown>",    // EF

    "<unknown>",    // F0
    "<unknown>",    // F1
    "<unknown>",    // F2
    "<unknown>",    // F3
    "<unknown>",    // F4
    "<unknown>",    // F5
    "<unknown>",    // F6
    "<unknown>",    // F7
    "<unknown>",    // F8
    "<unknown>",    // F9
    "<unknown>",    // FA
    "<unknown>",    // FB
    "<unknown>",    // FC
    "<unknown>",    // FD
    "<unknown>",    // FE
    "<unknown>"     // FF
};



// This is the two byte scan code table used for a normal QWERTY keyboard.
// These extended scan codes will be preceded by a 0xE0 byte.
static const char* const    g_scanCodesExtended[256] =
{
    "<unknown>",    // 00
    "<unknown>",    // 01
    "<unknown>",    // 02
    "<unknown>",    // 03
    "<unknown>",    // 04
    "<unknown>",    // 05
    "<unknown>",    // 06
    "<unknown>",    // 07
    "<unknown>",    // 08
    "<unknown>",    // 09
    "<unknown>",    // 0A
    "<unknown>",    // 0B
    "<unknown>",    // 0C
    "<unknown>",    // 0D
    "<unknown>",    // 0E
    "<unknown>",    // 0F
    
    "<unknown>",    // 10
    "<Right ALT>",  // 11
    "<Modifier>",   // 12
    "<unknown>",    // 13
    "<Right CTRL>",  // 14
    "<unknown>",    // 15
    "<unknown>",    // 16
    "<unknown>",    // 17
    "<unknown>",    // 18
    "<unknown>",    // 19
    "<unknown>",    // 1A
    "<unknown>",    // 1B
    "<unknown>",    // 1C
    "<unknown>",    // 1D
    "<unknown>",    // 1E
    "<Left GUI>",   // 1F

    "<unknown>",    // 20
    "<unknown>",    // 21
    "<unknown>",    // 22
    "<unknown>",    // 23
    "<unknown>",    // 24
    "<unknown>",    // 25
    "<unknown>",    // 26
    "<Right GUI>",  // 27
    "<unknown>",    // 28
    "<unknown>",    // 29
    "<unknown>",    // 2A
    "<unknown>",    // 2B
    "<unknown>",    // 2C
    "<unknown>",    // 2D
    "<unknown>",    // 2E
    "<APPS>",       // 2F

    "<unknown>",    // 30
    "<unknown>",    // 31
    "<unknown>",    // 32
    "<unknown>",    // 33
    "<unknown>",    // 34
    "<unknown>",    // 35
    "<unknown>",    // 36
    "<unknown>",    // 37
    "<unknown>",    // 38
    "<unknown>",    // 39
    "<unknown>",    // 3A
    "<unknown>",    // 3B
    "<unknown>",    // 3C
    "<unknown>",    // 3D
    "<unknown>",    // 3E
    "<unknown>",    // 3F
    
    "<unknown>",    // 40
    "<unknown>",    // 41
    "<unknown>",    // 42
    "<unknown>",    // 43
    "<unknown>",    // 44
    "<unknown>",    // 45
    "<unknown>",    // 46
    "<unknown>",    // 47
    "<unknown>",    // 48
    "<unknown>",    // 49
    "/",            // 4A
    "<unknown>",    // 4B
    "<unknown>",    // 4C
    "<unknown>",    // 4D
    "<unknown>",    // 4E
    "<unknown>",    // 4F

    "<unknown>",    // 50
    "<unknown>",    // 51
    "<unknown>",    // 52
    "<unknown>",    // 53
    "<unknown>",    // 54
    "<unknown>",    // 55
    "<unknown>",    // 56
    "<unknown>",    // 57
    "<unknown>",    // 58
    "<unknown>",    // 59
    "\n",         // 5A
    "<unknown>",    // 5B
    "<unknown>",    // 5C
    "<unknown>",    // 5D
    "<unknown>",    // 5E
    "<unknown>",    // 5F

    "<unknown>",    // 60
    "<unknown>",    // 61
    "<unknown>",    // 62
    "<unknown>",    // 63
    "<unknown>",    // 64
    "<unknown>",    // 65
    "<unknown>",    // 66
    "<unknown>",    // 67
    "<unknown>",    // 68
    "<End>",        // 69
    "<unknown>",    // 6A
    "<Left Arrow>", // 6B
    "<Home>",       // 6C
    "<unknown>",    // 6D
    "<unknown>",    // 6E
    "<unknown>",    // 6F

    "<Insert>",     // 70
    "<Delete>",     // 71
    "<Down Arrow>", // 72
    "<unknown>",    // 73
    "<Right Arrow>",// 74
    "<Up Arrow>",   // 75
    "<unknown>",    // 76
    "<unknown>",    // 77
    "<unknown>",    // 78
    "<unknown>",    // 79
    "<Page Down>",  // 7A
    "<unknown>",    // 7B
    "<Print Screen>",// 7C
    "<Page Up>",    // 7D
    "<unknown>",    // 7E
    "<unknown>",    // 7F

    "<unknown>",    // 80
    "<unknown>",    // 81
    "<unknown>",    // 82
    "<unknown>",    // 83
    "<unknown>",    // 84
    "<unknown>",    // 85
    "<unknown>",    // 86
    "<unknown>",    // 87
    "<unknown>",    // 88
    "<unknown>",    // 89
    "<unknown>",    // 8A
    "<unknown>",    // 8B
    "<unknown>",    // 8C
    "<unknown>",    // 8D
    "<unknown>",    // 8E
    "<unknown>",    // 8F

    "<unknown>",    // 90
    "<unknown>",    // 91
    "<unknown>",    // 92
    "<unknown>",    // 93
    "<unknown>",    // 94
    "<unknown>",    // 95
    "<unknown>",    // 96
    "<unknown>",    // 97
    "<unknown>",    // 98
    "<unknown>",    // 99
    "<unknown>",    // 9A
    "<unknown>",    // 9B
    "<unknown>",    // 9C
    "<unknown>",    // 9D
    "<unknown>",    // 9E
    "<unknown>",    // 9F

    "<unknown>",    // A0
    "<unknown>",    // A1
    "<unknown>",    // A2
    "<unknown>",    // A3
    "<unknown>",    // A4
    "<unknown>",    // A5
    "<unknown>",    // A6
    "<unknown>",    // A7
    "<unknown>",    // A8
    "<unknown>",    // A9
    "<unknown>",    // AA
    "<unknown>",    // AB
    "<unknown>",    // AC
    "<unknown>",    // AD
    "<unknown>",    // AE
    "<unknown>",    // AF

    "<unknown>",    // B0
    "<unknown>",    // B1
    "<unknown>",    // B2
    "<unknown>",    // B3
    "<unknown>",    // B4
    "<unknown>",    // B5
    "<unknown>",    // B6
    "<unknown>",    // B7
    "<unknown>",    // B8
    "<unknown>",    // B9
    "<unknown>",    // BA
    "<unknown>",    // BB
    "<unknown>",    // BC
    "<unknown>",    // BD
    "<unknown>",    // BE
    "<unknown>",    // BF

    "<unknown>",    // C0
    "<unknown>",    // C1
    "<unknown>",    // C2
    "<unknown>",    // C3
    "<unknown>",    // C4
    "<unknown>",    // C5
    "<unknown>",    // C6
    "<unknown>",    // C7
    "<unknown>",    // C8
    "<unknown>",    // C9
    "<unknown>",    // CA
    "<unknown>",    // CB
    "<unknown>",    // CC
    "<unknown>",    // CD
    "<unknown>",    // CE
    "<unknown>",    // CF

    "<unknown>",    // D0
    "<unknown>",    // D1
    "<unknown>",    // D2
    "<unknown>",    // D3
    "<unknown>",    // D4
    "<unknown>",    // D5
    "<unknown>",    // D6
    "<unknown>",    // D7
    "<unknown>",    // D8
    "<unknown>",    // D9
    "<unknown>",    // DA
    "<unknown>",    // DB
    "<unknown>",    // DC
    "<unknown>",    // DD
    "<unknown>",    // DE
    "<unknown>",    // DF

    "<unknown>",    // E0
    "<unknown>",    // E1
    "<unknown>",    // E2
    "<unknown>",    // E3
    "<unknown>",    // E4
    "<unknown>",    // E5
    "<unknown>",    // E6
    "<unknown>",    // E7
    "<unknown>",    // E8
    "<unknown>",    // E9
    "<unknown>",    // EA
    "<unknown>",    // EB
    "<unknown>",    // EC
    "<unknown>",    // ED
    "<unknown>",    // EE
    "<unknown>",    // EF

    "<unknown>",    // F0
    "<unknown>",    // F1
    "<unknown>",    // F2
    "<unknown>",    // F3
    "<unknown>",    // F4
    "<unknown>",    // F5
    "<unknown>",    // F6
    "<unknown>",    // F7
    "<unknown>",    // F8
    "<unknown>",    // F9
    "<unknown>",    // FA
    "<unknown>",    // FB
    "<unknown>",    // FC
    "<unknown>",    // FD
    "<unknown>",    // FE
    "<unknown>"     // FF
};



// This is the single byte scan code table to HID usage code used for a normal QWERTY keyboard.
static const HidKeyboardUsageValues g_scanCodesToHidQWERTY[256] =
{
    0x00,                   // 00
    HID_KEY_F9,             // 01
    0x00,                   // 02
    HID_KEY_F5,             // 03
    HID_KEY_F3,             // 04
    HID_KEY_F1,             // 05
    HID_KEY_F2,             // 06
    HID_KEY_F12,            // 07
    0x00,                   // 08
    HID_KEY_F10,            // 09
    HID_KEY_F8,             // 0A
    HID_KEY_F6,             // 0B
    HID_KEY_F4,             // 0C
    HID_KEY_TAB,            // 0D
    HID_KEY_GRAVE_ACCENT,   // 0E
    0x00,                   // 0F

    0x00,                   // 10
    HID_KEY_LEFT_ALT,       // 11
    HID_KEY_LEFT_SHIFT,     // 12
    0x00,                   // 13
    HID_KEY_LEFT_CONTROL,   // 14
    HID_KEY_Q,              // 15
    HID_KEY_1,              // 16
    0x00,                   // 17
    0x00,                   // 18
    0x00,                   // 19
    HID_KEY_Z,              // 1A
    HID_KEY_S,              // 1B
    HID_KEY_A,              // 1C
    HID_KEY_W,              // 1D
    HID_KEY_2,              // 1E
    0x00,                   // 1F

    0x00,                   // 20
    HID_KEY_C,              // 21
    HID_KEY_X,              // 22
    HID_KEY_D,              // 23
    HID_KEY_E,              // 24
    HID_KEY_4,              // 25
    HID_KEY_3,              // 26
    0x00,                   // 27
    0x00,                   // 28
    HID_KEY_SPACEBAR,       // 29
    HID_KEY_V,              // 2A
    HID_KEY_F,              // 2B
    HID_KEY_T,              // 2C
    HID_KEY_R,              // 2D
    HID_KEY_5,              // 2E
    0x00,                   // 2F

    0x00,                   // 30
    HID_KEY_N,              // 31
    HID_KEY_B,              // 32
    HID_KEY_H,              // 33
    HID_KEY_G,              // 34
    HID_KEY_Y,              // 35
    HID_KEY_6,              // 36
    0x00,                   // 37
    0x00,                   // 38
    0x00,                   // 39
    HID_KEY_M,              // 3A
    HID_KEY_J,              // 3B
    HID_KEY_U,              // 3C
    HID_KEY_7,              // 3D
    HID_KEY_8,              // 3E
    0x00,                   // 3F

    0x00,                   // 40
    HID_KEY_COMMA,          // 41
    HID_KEY_K,              // 42
    HID_KEY_I,              // 43
    HID_KEY_O,              // 44
    HID_KEY_0,              // 45
    HID_KEY_9,              // 46
    0x00,                   // 47
    0x00,                   // 48
    HID_KEY_PERIOD,         // 49
    HID_KEY_SLASH,          // 4A
    HID_KEY_L,              // 4B
    HID_KEY_SEMICOLON,      // 4C
    HID_KEY_P,              // 4D
    HID_KEY_MINUS,          // 4E
    0x00,                   // 4F

    0x00,                   // 50
    0x00,                   // 51
    HID_KEY_APOSTROPHE,     // 52
    0x00,                   // 53
    HID_KEY_LEFT_BRACKET,   // 54
    HID_KEY_EQUAL,          // 55
    0x00,                   // 56
    0x00,                   // 57
    HID_KEY_CAPS_LOCK,      // 58
    HID_KEY_RIGHT_SHIFT,    // 59
    HID_KEY_ENTER,          // 5A
    HID_KEY_RIGHT_BRACKET,  // 5B
    0x00,                   // 5C
    HID_KEY_BACKSLASH,      // 5D
    0x00,                   // 5E
    0x00,                   // 5F

    0x00,                   // 60
    0x00,                   // 61
    0x00,                   // 62
    0x00,                   // 63
    0x00,                   // 64
    0x00,                   // 65
    HID_KEY_BACKSPACE,      // 66
    0x00,                   // 67
    0x00,                   // 68
    HID_KEYPAD_1,           // 69
    0x00,                   // 6A
    HID_KEYPAD_4,           // 6B
    HID_KEYPAD_7,           // 6C
    0x00,                   // 6D
    0x00,                   // 6E
    0x00,                   // 6F

    HID_KEYPAD_0,           // 70
    HID_KEYPAD_PERIOD,      // 71
    HID_KEYPAD_2,           // 72
    HID_KEYPAD_5,           // 73
    HID_KEYPAD_6,           // 74
    HID_KEYPAD_8,           // 75
    HID_KEY_ESCAPE,         // 76
    HID_KEYPAD_NUM_LOCK,    // 77
    HID_KEY_F11,            // 78
    HID_KEYPAD_PLUS,        // 79
    HID_KEYPAD_3,           // 7A
    HID_KEYPAD_MINUS,       // 7B
    HID_KEYPAD_ASTERISK,    // 7C
    HID_KEYPAD_9,           // 7D
    HID_KEY_SCROLL_LOCK,    // 7E
    0x00,                   // 7F

    0x00,                   // 80
    0x00,                   // 81
    0x00,                   // 82
    HID_KEY_F7,             // 83
    0x00,                   // 84
    0x00,                   // 85
    0x00,                   // 86
    0x00,                   // 87
    0x00,                   // 88
    0x00,                   // 89
    0x00,                   // 8A
    0x00,                   // 8B
    0x00,                   // 8C
    0x00,                   // 8D
    0x00,                   // 8E
    0x00,                   // 8F

    0x00,                   // 90
    0x00,                   // 91
    0x00,                   // 92
    0x00,                   // 93
    0x00,                   // 94
    0x00,                   // 95
    0x00,                   // 96
    0x00,                   // 97
    0x00,                   // 98
    0x00,                   // 99
    0x00,                   // 9A
    0x00,                   // 9B
    0x00,                   // 9C
    0x00,                   // 9D
    0x00,                   // 9E
    0x00,                   // 9F

    0x00,                   // A0
    0x00,                   // A1
    0x00,                   // A2
    0x00,                   // A3
    0x00,                   // A4
    0x00,                   // A5
    0x00,                   // A6
    0x00,                   // A7
    0x00,                   // A8
    0x00,                   // A9
    0x00,                   // AA
    0x00,                   // AB
    0x00,                   // AC
    0x00,                   // AD
    0x00,                   // AE
    0x00,                   // AF

    0x00,                   // B0
    0x00,                   // B1
    0x00,                   // B2
    0x00,                   // B3
    0x00,                   // B4
    0x00,                   // B5
    0x00,                   // B6
    0x00,                   // B7
    0x00,                   // B8
    0x00,                   // B9
    0x00,                   // BA
    0x00,                   // BB
    0x00,                   // BC
    0x00,                   // BD
    0x00,                   // BE
    0x00,                   // BF

    0x00,                   // C0
    0x00,                   // C1
    0x00,                   // C2
    0x00,                   // C3
    0x00,                   // C4
    0x00,                   // C5
    0x00,                   // C6
    0x00,                   // C7
    0x00,                   // C8
    0x00,                   // C9
    0x00,                   // CA
    0x00,                   // CB
    0x00,                   // CC
    0x00,                   // CD
    0x00,                   // CE
    0x00,                   // CF

    0x00,                   // D0
    0x00,                   // D1
    0x00,                   // D2
    0x00,                   // D3
    0x00,                   // D4
    0x00,                   // D5
    0x00,                   // D6
    0x00,                   // D7
    0x00,                   // D8
    0x00,                   // D9
    0x00,                   // DA
    0x00,                   // DB
    0x00,                   // DC
    0x00,                   // DD
    0x00,                   // DE
    0x00,                   // DF

    0x00,                   // E0
    0x00,                   // E1
    0x00,                   // E2
    0x00,                   // E3
    0x00,                   // E4
    0x00,                   // E5
    0x00,                   // E6
    0x00,                   // E7
    0x00,                   // E8
    0x00,                   // E9
    0x00,                   // EA
    0x00,                   // EB
    0x00,                   // EC
    0x00,                   // ED
    0x00,                   // EE
    0x00,                   // EF

    0x00,                   // F0
    0x00,                   // F1
    0x00,                   // F2
    0x00,                   // F3
    0x00,                   // F4
    0x00,                   // F5
    0x00,                   // F6
    0x00,                   // F7
    0x00,                   // F8
    0x00,                   // F9
    0x00,                   // FA
    0x00,                   // FB
    0x00,                   // FC
    0x00,                   // FD
    0x00,                   // FE
    0x00                    // FF
};

// This is the single byte scan code table to HID usage code used for the special ABC (alphabetical order) keyboard.
static const HidKeyboardUsageValues g_scanCodesToHidABC[256] =
{
    0x00,                   // 00
    HID_KEY_F9,             // 01
    0x00,                   // 02
    HID_KEY_F5,             // 03
    HID_KEY_F3,             // 04
    HID_KEY_F1,             // 05
    HID_KEY_F2,             // 06
    HID_KEY_F12,            // 07
    0x00,                   // 08
    HID_KEY_F10,            // 09
    HID_KEY_F8,             // 0A
    HID_KEY_F6,             // 0B
    HID_KEY_F4,             // 0C
    HID_KEY_TAB,            // 0D
    HID_KEY_GRAVE_ACCENT,   // 0E
    0x00,                   // 0F

    0x00,                   // 10
    HID_KEY_LEFT_ALT,       // 11
    HID_KEY_LEFT_SHIFT,     // 12
    0x00,                   // 13
    HID_KEY_LEFT_CONTROL,   // 14
    HID_KEY_A,              // 15
    HID_KEY_1,              // 16
    0x00,                   // 17
    0x00,                   // 18
    0x00,                   // 19
    HID_KEY_T,              // 1A
    HID_KEY_L,              // 1B
    HID_KEY_K,              // 1C
    HID_KEY_B,              // 1D
    HID_KEY_2,              // 1E
    0x00,                   // 1F

    0x00,                   // 20
    HID_KEY_V,              // 21
    HID_KEY_U,              // 22
    HID_KEY_M,              // 23
    HID_KEY_C,              // 24
    HID_KEY_4,              // 25
    HID_KEY_3,              // 26
    0x00,                   // 27
    0x00,                   // 28
    HID_KEY_SPACEBAR,       // 29
    HID_KEY_W,              // 2A
    HID_KEY_N,              // 2B
    HID_KEY_E,              // 2C
    HID_KEY_D,              // 2D
    HID_KEY_5,              // 2E
    0x00,                   // 2F

    0x00,                   // 30
    HID_KEY_Y,              // 31
    HID_KEY_X,              // 32
    HID_KEY_P,              // 33
    HID_KEY_O,              // 34
    HID_KEY_F,              // 35
    HID_KEY_6,              // 36
    0x00,                   // 37
    0x00,                   // 38
    0x00,                   // 39
    HID_KEY_Z,              // 3A
    HID_KEY_Q,              // 3B
    HID_KEY_G,              // 3C
    HID_KEY_7,              // 3D
    HID_KEY_8,              // 3E
    0x00,                   // 3F

    0x00,                   // 40
    HID_KEY_COMMA,          // 41
    HID_KEY_R,              // 42
    HID_KEY_H,              // 43
    HID_KEY_I,              // 44
    HID_KEY_0,              // 45
    HID_KEY_9,              // 46
    0x00,                   // 47
    0x00,                   // 48
    HID_KEY_PERIOD,         // 49
    HID_KEY_SLASH,          // 4A
    HID_KEY_S,              // 4B
    HID_KEY_SEMICOLON,      // 4C
    HID_KEY_J,              // 4D
    HID_KEY_MINUS,          // 4E
    0x00,                   // 4F

    0x00,                   // 50
    0x00,                   // 51
    HID_KEY_APOSTROPHE,     // 52
    0x00,                   // 53
    HID_KEY_LEFT_BRACKET,   // 54
    HID_KEY_EQUAL,          // 55
    0x00,                   // 56
    0x00,                   // 57
    HID_KEY_CAPS_LOCK,      // 58
    HID_KEY_RIGHT_SHIFT,    // 59
    HID_KEY_ENTER,          // 5A
    HID_KEY_RIGHT_BRACKET,  // 5B
    0x00,                   // 5C
    HID_KEY_BACKSLASH,      // 5D
    0x00,                   // 5E
    0x00,                   // 5F

    0x00,                   // 60
    0x00,                   // 61
    0x00,                   // 62
    0x00,                   // 63
    0x00,                   // 64
    0x00,                   // 65
    HID_KEY_BACKSPACE,      // 66
    0x00,                   // 67
    0x00,                   // 68
    HID_KEYPAD_1,           // 69
    0x00,                   // 6A
    HID_KEYPAD_4,           // 6B
    HID_KEYPAD_7,           // 6C
    0x00,                   // 6D
    0x00,                   // 6E
    0x00,                   // 6F

    HID_KEYPAD_0,           // 70
    HID_KEYPAD_PERIOD,      // 71
    HID_KEYPAD_2,           // 72
    HID_KEYPAD_5,           // 73
    HID_KEYPAD_6,           // 74
    HID_KEYPAD_8,           // 75
    HID_KEY_ESCAPE,         // 76
    HID_KEYPAD_NUM_LOCK,    // 77
    HID_KEY_F11,            // 78
    HID_KEYPAD_PLUS,        // 79
    HID_KEYPAD_3,           // 7A
    HID_KEYPAD_MINUS,       // 7B
    HID_KEYPAD_ASTERISK,    // 7C
    HID_KEYPAD_9,           // 7D
    HID_KEY_SCROLL_LOCK,    // 7E
    0x00,                   // 7F

    0x00,                   // 80
    0x00,                   // 81
    0x00,                   // 82
    HID_KEY_F7,             // 83
    0x00,                   // 84
    0x00,                   // 85
    0x00,                   // 86
    0x00,                   // 87
    0x00,                   // 88
    0x00,                   // 89
    0x00,                   // 8A
    0x00,                   // 8B
    0x00,                   // 8C
    0x00,                   // 8D
    0x00,                   // 8E
    0x00,                   // 8F

    0x00,                   // 90
    0x00,                   // 91
    0x00,                   // 92
    0x00,                   // 93
    0x00,                   // 94
    0x00,                   // 95
    0x00,                   // 96
    0x00,                   // 97
    0x00,                   // 98
    0x00,                   // 99
    0x00,                   // 9A
    0x00,                   // 9B
    0x00,                   // 9C
    0x00,                   // 9D
    0x00,                   // 9E
    0x00,                   // 9F

    0x00,                   // A0
    0x00,                   // A1
    0x00,                   // A2
    0x00,                   // A3
    0x00,                   // A4
    0x00,                   // A5
    0x00,                   // A6
    0x00,                   // A7
    0x00,                   // A8
    0x00,                   // A9
    0x00,                   // AA
    0x00,                   // AB
    0x00,                   // AC
    0x00,                   // AD
    0x00,                   // AE
    0x00,                   // AF

    0x00,                   // B0
    0x00,                   // B1
    0x00,                   // B2
    0x00,                   // B3
    0x00,                   // B4
    0x00,                   // B5
    0x00,                   // B6
    0x00,                   // B7
    0x00,                   // B8
    0x00,                   // B9
    0x00,                   // BA
    0x00,                   // BB
    0x00,                   // BC
    0x00,                   // BD
    0x00,                   // BE
    0x00,                   // BF

    0x00,                   // C0
    0x00,                   // C1
    0x00,                   // C2
    0x00,                   // C3
    0x00,                   // C4
    0x00,                   // C5
    0x00,                   // C6
    0x00,                   // C7
    0x00,                   // C8
    0x00,                   // C9
    0x00,                   // CA
    0x00,                   // CB
    0x00,                   // CC
    0x00,                   // CD
    0x00,                   // CE
    0x00,                   // CF

    0x00,                   // D0
    0x00,                   // D1
    0x00,                   // D2
    0x00,                   // D3
    0x00,                   // D4
    0x00,                   // D5
    0x00,                   // D6
    0x00,                   // D7
    0x00,                   // D8
    0x00,                   // D9
    0x00,                   // DA
    0x00,                   // DB
    0x00,                   // DC
    0x00,                   // DD
    0x00,                   // DE
    0x00,                   // DF

    0x00,                   // E0
    0x00,                   // E1
    0x00,                   // E2
    0x00,                   // E3
    0x00,                   // E4
    0x00,                   // E5
    0x00,                   // E6
    0x00,                   // E7
    0x00,                   // E8
    0x00,                   // E9
    0x00,                   // EA
    0x00,                   // EB
    0x00,                   // EC
    0x00,                   // ED
    0x00,                   // EE
    0x00,                   // EF

    0x00,                   // F0
    0x00,                   // F1
    0x00,                   // F2
    0x00,                   // F3
    0x00,                   // F4
    0x00,                   // F5
    0x00,                   // F6
    0x00,                   // F7
    0x00,                   // F8
    0x00,                   // F9
    0x00,                   // FA
    0x00,                   // FB
    0x00,                   // FC
    0x00,                   // FD
    0x00,                   // FE
    0x00                    // FF
};

// This is the two byte scan code to HID usage code table used for a normal QWERTY keyboard.
// These extended scan codes will be preceded by a 0xE0 byte.
static const HidKeyboardUsageValues g_scanCodesToHidExtended[256] =
{
    0x00,                   // 00
    0x00,                   // 01
    0x00,                   // 02
    0x00,                   // 03
    0x00,                   // 04
    0x00,                   // 05
    0x00,                   // 06
    0x00,                   // 07
    0x00,                   // 08
    0x00,                   // 09
    0x00,                   // 0A
    0x00,                   // 0B
    0x00,                   // 0C
    0x00,                   // 0D
    0x00,                   // 0E
    0x00,                   // 0F
    
    0x00,                   // 10
    HID_KEY_RIGHT_ALT,      // 11
    0x00,                   // 12
    0x00,                   // 13
    HID_KEY_RIGHT_CONTROL,  // 14
    0x00,                   // 15
    0x00,                   // 16
    0x00,                   // 17
    0x00,                   // 18
    0x00,                   // 19
    0x00,                   // 1A
    0x00,                   // 1B
    0x00,                   // 1C
    0x00,                   // 1D
    0x00,                   // 1E
    HID_KEY_LEFT_GUI,       // 1F

    0x00,                   // 20
    0x00,                   // 21
    0x00,                   // 22
    0x00,                   // 23
    0x00,                   // 24
    0x00,                   // 25
    0x00,                   // 26
    HID_KEY_RIGHT_GUI,      // 27
    0x00,                   // 28
    0x00,                   // 29
    0x00,                   // 2A
    0x00,                   // 2B
    0x00,                   // 2C
    0x00,                   // 2D
    0x00,                   // 2E
    HID_KEY_APP,            // 2F

    0x00,                   // 30
    0x00,                   // 31
    0x00,                   // 32
    0x00,                   // 33
    0x00,                   // 34
    0x00,                   // 35
    0x00,                   // 36
    0x00,                   // 37
    0x00,                   // 38
    0x00,                   // 39
    0x00,                   // 3A
    0x00,                   // 3B
    0x00,                   // 3C
    0x00,                   // 3D
    0x00,                   // 3E
    0x00,                   // 3F
    
    0x00,                   // 40
    0x00,                   // 41
    0x00,                   // 42
    0x00,                   // 43
    0x00,                   // 44
    0x00,                   // 45
    0x00,                   // 46
    0x00,                   // 47
    0x00,                   // 48
    0x00,                   // 49
    HID_KEYPAD_SLASH,       // 4A
    0x00,                   // 4B
    0x00,                   // 4C
    0x00,                   // 4D
    0x00,                   // 4E
    0x00,                   // 4F

    0x00,                   // 50
    0x00,                   // 51
    0x00,                   // 52
    0x00,                   // 53
    0x00,                   // 54
    0x00,                   // 55
    0x00,                   // 56
    0x00,                   // 57
    0x00,                   // 58
    0x00,                   // 59
    HID_KEYPAD_ENTER,       // 5A
    0x00,                   // 5B
    0x00,                   // 5C
    0x00,                   // 5D
    0x00,                   // 5E
    0x00,                   // 5F

    0x00,                   // 60
    0x00,                   // 61
    0x00,                   // 62
    0x00,                   // 63
    0x00,                   // 64
    0x00,                   // 65
    0x00,                   // 66
    0x00,                   // 67
    0x00,                   // 68
    HID_KEY_END,            // 69
    0x00,                   // 6A
    HID_KEY_LEFT_ARROW,     // 6B
    HID_KEY_HOME,           // 6C
    0x00,                   // 6D
    0x00,                   // 6E
    0x00,                   // 6F

    HID_KEY_INSERT,         // 70
    HID_KEY_DELETE,         // 71
    HID_KEY_DOWN_ARROW,     // 72
    0x00,                   // 73
    HID_KEY_RIGHT_ARROW,    // 74
    HID_KEY_UP_ARROW,       // 75
    0x00,                   // 76
    0x00,                   // 77
    0x00,                   // 78
    0x00,                   // 79
    HID_KEY_PAGE_DOWN,      // 7A
    0x00,                   // 7B
    HID_KEY_PRINT_SCREEN,   // 7C
    HID_KEY_PAGE_UP,        // 7D
    0x00,                   // 7E
    0x00,                   // 7F

    0x00,                   // 80
    0x00,                   // 81
    0x00,                   // 82
    0x00,                   // 83
    0x00,                   // 84
    0x00,                   // 85
    0x00,                   // 86
    0x00,                   // 87
    0x00,                   // 88
    0x00,                   // 89
    0x00,                   // 8A
    0x00,                   // 8B
    0x00,                   // 8C
    0x00,                   // 8D
    0x00,                   // 8E
    0x00,                   // 8F

    0x00,                   // 90
    0x00,                   // 91
    0x00,                   // 92
    0x00,                   // 93
    0x00,                   // 94
    0x00,                   // 95
    0x00,                   // 96
    0x00,                   // 97
    0x00,                   // 98
    0x00,                   // 99
    0x00,                   // 9A
    0x00,                   // 9B
    0x00,                   // 9C
    0x00,                   // 9D
    0x00,                   // 9E
    0x00,                   // 9F

    0x00,                   // A0
    0x00,                   // A1
    0x00,                   // A2
    0x00,                   // A3
    0x00,                   // A4
    0x00,                   // A5
    0x00,                   // A6
    0x00,                   // A7
    0x00,                   // A8
    0x00,                   // A9
    0x00,                   // AA
    0x00,                   // AB
    0x00,                   // AC
    0x00,                   // AD
    0x00,                   // AE
    0x00,                   // AF

    0x00,                   // B0
    0x00,                   // B1
    0x00,                   // B2
    0x00,                   // B3
    0x00,                   // B4
    0x00,                   // B5
    0x00,                   // B6
    0x00,                   // B7
    0x00,                   // B8
    0x00,                   // B9
    0x00,                   // BA
    0x00,                   // BB
    0x00,                   // BC
    0x00,                   // BD
    0x00,                   // BE
    0x00,                   // BF

    0x00,                   // C0
    0x00,                   // C1
    0x00,                   // C2
    0x00,                   // C3
    0x00,                   // C4
    0x00,                   // C5
    0x00,                   // C6
    0x00,                   // C7
    0x00,                   // C8
    0x00,                   // C9
    0x00,                   // CA
    0x00,                   // CB
    0x00,                   // CC
    0x00,                   // CD
    0x00,                   // CE
    0x00,                   // CF

    0x00,                   // D0
    0x00,                   // D1
    0x00,                   // D2
    0x00,                   // D3
    0x00,                   // D4
    0x00,                   // D5
    0x00,                   // D6
    0x00,                   // D7
    0x00,                   // D8
    0x00,                   // D9
    0x00,                   // DA
    0x00,                   // DB
    0x00,                   // DC
    0x00,                   // DD
    0x00,                   // DE
    0x00,                   // DF

    0x00,                   // E0
    0x00,                   // E1
    0x00,                   // E2
    0x00,                   // E3
    0x00,                   // E4
    0x00,                   // E5
    0x00,                   // E6
    0x00,                   // E7
    0x00,                   // E8
    0x00,                   // E9
    0x00,                   // EA
    0x00,                   // EB
    0x00,                   // EC
    0x00,                   // ED
    0x00,                   // EE
    0x00,                   // EF

    0x00,                   // F0
    0x00,                   // F1
    0x00,                   // F2
    0x00,                   // F3
    0x00,                   // F4
    0x00,                   // F5
    0x00,                   // F6
    0x00,                   // F7
    0x00,                   // F8
    0x00,                   // F9
    0x00,                   // FA
    0x00,                   // FB
    0x00,                   // FC
    0x00,                   // FD
    0x00,                   // FE
    0x00                    // FF
};

#endif // SCAN_CODES_H_
