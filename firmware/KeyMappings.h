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
// Auto generated from /keycodes/main.c

typedef struct RowMapping
{
    uint32_t               bitmask;
    HidKeyboardUsageValues usage;
} RowMapping;

typedef struct ColumnMappings
{
    const RowMapping*      pMappings;
    uint32_t               bitmask;
    uint8_t                count;
} ColumnMappings;


static const RowMapping g_col0RowMappings[] =
{
    {0x00400000, HID_KEY_LEFT_GUI},
    {0x00000800, HID_KEY_A},
    {0x00000400, HID_KEY_S},
    {0x00000200, HID_KEY_D},
    {0x00000100, HID_KEY_F},
    {0x00000080, HID_KEY_J},
    {0x00000040, HID_KEY_K},
    {0x00000020, HID_KEY_L},
    {0x00000010, HID_KEY_SEMICOLON},
    {0x00200000, HID_KEY_BACKSLASH},
};
static const RowMapping g_col1RowMappings[] =
{
    {0x00004000, HID_KEY_LEFT_ALT},
    {0x00008000, HID_KEY_UP_ARROW},
    {0x00000100, HID_KEY_G},
    {0x00000080, HID_KEY_H},
    {0x00000010, HID_KEY_APOSTROPHE},
    {0x00000800, HID_KEY_ESCAPE},
    {0x00000200, HID_KEY_F4},
    {0x00200000, HID_KEY_F5},
    {0x00000040, HID_KEY_F6},
};
static const RowMapping g_col2RowMappings[] =
{
    {0x00001000, HID_KEY_RIGHT_CONTROL},
    {0x00000800, HID_KEY_Z},
    {0x00000400, HID_KEY_X},
    {0x00000200, HID_KEY_C},
    {0x00000100, HID_KEY_V},
    {0x00000080, HID_KEY_M},
    {0x00000040, HID_KEY_COMMA},
    {0x00000020, HID_KEY_PERIOD},
    {0x00002000, HID_KEY_RIGHT_SHIFT},
    {0x00200000, HID_KEY_ENTER},
};
static const RowMapping g_col3RowMappings[] =
{
    {0x00200000, HID_KEY_SPACEBAR},
    {0x00004000, HID_KEY_RIGHT_ALT},
    {0x00008000, HID_KEY_LEFT_ARROW},
    {0x00080000, HID_KEY_DOWN_ARROW},
    {0x00100000, HID_KEY_RIGHT_ARROW},
    {0x00000100, HID_KEY_B},
    {0x00000080, HID_KEY_N},
    {0x00000010, HID_KEY_SLASH},
};
static const RowMapping g_col16RowMappings[] =
{
    {0x00002000, HID_KEY_LEFT_SHIFT},
    {0x00000400, HID_KEY_CAPS_LOCK},
    {0x00000800, HID_KEY_TAB},
    {0x00000100, HID_KEY_T},
    {0x00000080, HID_KEY_Y},
    {0x00000010, HID_KEY_LEFT_BRACKET},
    {0x00000040, HID_KEY_RIGHT_BRACKET},
    {0x00200000, HID_KEY_BACKSPACE},
    {0x00000200, HID_KEY_F3},
    {0x00000020, HID_KEY_F7},
};
static const RowMapping g_col17RowMappings[] =
{
    {0x00001000, HID_KEY_FN},
    {0x00400000, HID_KEY_APP},
    {0x00000800, HID_KEY_Q},
    {0x00000400, HID_KEY_W},
    {0x00000200, HID_KEY_E},
    {0x00000100, HID_KEY_R},
    {0x00000080, HID_KEY_U},
    {0x00000040, HID_KEY_I},
    {0x00000020, HID_KEY_O},
    {0x00000010, HID_KEY_P},
};
static const RowMapping g_col24RowMappings[] =
{
    {0x00001000, HID_KEY_LEFT_CONTROL},
    {0x00040000, HID_KEY_PAGE_UP},
    {0x00000800, HID_KEY_GRAVE_ACCENT},
    {0x00000100, HID_KEY_5},
    {0x00000080, HID_KEY_6},
    {0x00000010, HID_KEY_MINUS},
    {0x00000040, HID_KEY_EQUAL},
    {0x00008000, HID_KEY_HOME},
    {0x00000400, HID_KEY_F1},
    {0x00000200, HID_KEY_F2},
    {0x00000020, HID_KEY_F8},
    {0x00200000, HID_KEY_F9},
    {0x00100000, HID_KEY_INSERT},
    {0x00080000, HID_KEY_DELETE},
};
static const RowMapping g_col25RowMappings[] =
{
    {0x00008000, HID_KEY_END},
    {0x00040000, HID_KEY_PAGE_DOWN},
    {0x00000800, HID_KEY_1},
    {0x00000400, HID_KEY_2},
    {0x00000200, HID_KEY_3},
    {0x00000100, HID_KEY_4},
    {0x00000080, HID_KEY_7},
    {0x00000040, HID_KEY_8},
    {0x00000020, HID_KEY_9},
    {0x00000010, HID_KEY_0},
    {0x00200000, HID_KEY_F10},
    {0x00080000, HID_KEY_F11},
    {0x00100000, HID_KEY_F12},
    {0x00004000, HID_KEY_PRINT_SCREEN},
};

static const ColumnMappings g_columnMappings[] =
{
    {
        g_col0RowMappings,
        0xFFFFFFFE,
        sizeof(g_col0RowMappings)/sizeof(g_col0RowMappings[0])
    },
    {
        g_col1RowMappings,
        0xFFFFFFFD,
        sizeof(g_col1RowMappings)/sizeof(g_col1RowMappings[0])
    },
    {
        g_col2RowMappings,
        0xFFFFFFFB,
        sizeof(g_col2RowMappings)/sizeof(g_col2RowMappings[0])
    },
    {
        g_col3RowMappings,
        0xFFFFFFF7,
        sizeof(g_col3RowMappings)/sizeof(g_col3RowMappings[0])
    },
    {
        g_col16RowMappings,
        0xFFFEFFFF,
        sizeof(g_col16RowMappings)/sizeof(g_col16RowMappings[0])
    },
    {
        g_col17RowMappings,
        0xFFFDFFFF,
        sizeof(g_col17RowMappings)/sizeof(g_col17RowMappings[0])
    },
    {
        g_col24RowMappings,
        0xFEFFFFFF,
        sizeof(g_col24RowMappings)/sizeof(g_col24RowMappings[0])
    },
    {
        g_col25RowMappings,
        0xFDFFFFFF,
        sizeof(g_col25RowMappings)/sizeof(g_col25RowMappings[0])
    },
};
