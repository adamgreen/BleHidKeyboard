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
// This program uses the responses I saw for each key press to generate a keyboard mapping header file.
// To find candidates for rows, I looked for pins which responded the least often. As I selected these pins to be rows,
// the pins to which they were connected were added as columns. By the 5th or so row, I had selected all of the highly 
// recurring common column pins (0, 1, 2, 3, 16, 17, 24, 25) needed by every other row pin.
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef struct KeyStruct
{
    const char* pName;
    uint32_t    from;
    uint32_t    to;
} KeyStruct;

// I generated this table by writing code which pulled every pin low, looked for responses, and then dumped them to
// the UART. I pressed each key on the keyboard and looked at its response.
static KeyStruct g_keys[] = {
    {"HID_KEY_LEFT_CONTROL",    0x00001000, 0x01000000},
    {"HID_KEY_FN",              0x00001000, 0x00020000},
    {"HID_KEY_LEFT_GUI",        0x00000001, 0x00400000},
    {"HID_KEY_LEFT_ALT",        0x00000002, 0x00004000},
    {"HID_KEY_SPACEBAR",        0x00000008, 0x00200000},
    {"HID_KEY_RIGHT_ALT",       0x00000008, 0x00004000},
    {"HID_KEY_APP",             0x00020000, 0x00400000},
    {"HID_KEY_RIGHT_CONTROL",   0x00000004, 0x00001000},
    {"HID_KEY_LEFT_ARROW",      0x00000008, 0x00008000},
    {"HID_KEY_DOWN_ARROW",      0x00000008, 0x00080000},
    {"HID_KEY_RIGHT_ARROW",     0x00000008, 0x00100000},
    {"HID_KEY_LEFT_SHIFT",      0x00002000, 0x00010000},
    {"HID_KEY_Z",               0x00000004, 0x00000800},
    {"HID_KEY_X",               0x00000004, 0x00000400},
    {"HID_KEY_C",               0x00000004, 0x00000200},
    {"HID_KEY_V",               0x00000004, 0x00000100},
    {"HID_KEY_B",               0x00000008, 0x00000100},
    {"HID_KEY_N",               0x00000008, 0x00000080},
    {"HID_KEY_M",               0x00000004, 0x00000080},
    {"HID_KEY_COMMA",           0x00000004, 0x00000040},
    {"HID_KEY_PERIOD",          0x00000004, 0x00000020},
    {"HID_KEY_SLASH",           0x00000008, 0x00000010},
    {"HID_KEY_RIGHT_SHIFT",     0x00000004, 0x00002000},
    {"HID_KEY_UP_ARROW",        0x00000002, 0x00008000},
    {"HID_KEY_END",             0x00008000, 0x02000000},
    {"HID_KEY_CAPS_LOCK",       0x00000400, 0x00010000},
    {"HID_KEY_A",               0x00000001, 0x00000800},
    {"HID_KEY_S",               0x00000001, 0x00000400},
    {"HID_KEY_D",               0x00000001, 0x00000200},
    {"HID_KEY_F",               0x00000001, 0x00000100},
    {"HID_KEY_G",               0x00000002, 0x00000100},
    {"HID_KEY_H",               0x00000002, 0x00000080},
    {"HID_KEY_J",               0x00000001, 0x00000080},
    {"HID_KEY_K",               0x00000001, 0x00000040},
    {"HID_KEY_L",               0x00000001, 0x00000020},
    {"HID_KEY_SEMICOLON",       0x00000001, 0x00000010},
    {"HID_KEY_APOSTROPHE",      0x00000002, 0x00000010},
    {"HID_KEY_ENTER",           0x00000004, 0x00200000},
    {"HID_KEY_PAGE_DOWN",       0x00040000, 0x02000000},
    {"HID_KEY_TAB",             0x00000800, 0x00010000},
    {"HID_KEY_Q",               0x00000800, 0x00020000},
    {"HID_KEY_W",               0x00000400, 0x00020000},
    {"HID_KEY_E",               0x00000200, 0x00020000},
    {"HID_KEY_R",               0x00000100, 0x00020000},
    {"HID_KEY_T",               0x00000100, 0x00010000},
    {"HID_KEY_Y",               0x00000080, 0x00010000},
    {"HID_KEY_U",               0x00000080, 0x00020000},
    {"HID_KEY_I",               0x00000040, 0x00020000},
    {"HID_KEY_O",               0x00000020, 0x00020000},
    {"HID_KEY_P",               0x00000010, 0x00020000},
    {"HID_KEY_LEFT_BRACKET",    0x00000010, 0x00010000},
    {"HID_KEY_RIGHT_BRACKET",   0x00000040, 0x00010000},
    {"HID_KEY_BACKSLASH",       0x00000001, 0x00200000},
    {"HID_KEY_PAGE_UP",         0x00040000, 0x01000000},
    {"HID_KEY_GRAVE_ACCENT",    0x00000800, 0x01000000},
    {"HID_KEY_1",               0x00000800, 0x02000000},
    {"HID_KEY_2",               0x00000400, 0x02000000},
    {"HID_KEY_3",               0x00000200, 0x02000000},
    {"HID_KEY_4",               0x00000100, 0x02000000},
    {"HID_KEY_5",               0x00000100, 0x01000000},
    {"HID_KEY_6",               0x00000080, 0x01000000},
    {"HID_KEY_7",               0x00000080, 0x02000000},
    {"HID_KEY_8",               0x00000040, 0x02000000},
    {"HID_KEY_9",               0x00000020, 0x02000000},
    {"HID_KEY_0",               0x00000010, 0x02000000},
    {"HID_KEY_MINUS",           0x00000010, 0x01000000},
    {"HID_KEY_EQUAL",           0x00000040, 0x01000000},
    {"HID_KEY_BACKSPACE",       0x00010000, 0x00200000},
    {"HID_KEY_HOME",            0x00008000, 0x01000000},
    {"HID_KEY_ESCAPE",          0x00000002, 0x00000800},
    {"HID_KEY_F1",              0x00000400, 0x01000000},
    {"HID_KEY_F2",              0x00000200, 0x01000000},
    {"HID_KEY_F3",              0x00000200, 0x00010000},
    {"HID_KEY_F4",              0x00000002, 0x00000200},
    {"HID_KEY_F5",              0x00000002, 0x00200000},
    {"HID_KEY_F6",              0x00000002, 0x00000040},
    {"HID_KEY_F7",              0x00000020, 0x00010000},
    {"HID_KEY_F8",              0x00000020, 0x01000000},
    {"HID_KEY_F9",              0x00200000, 0x01000000},
    {"HID_KEY_F10",             0x00200000, 0x02000000},
    {"HID_KEY_F11",             0x00080000, 0x02000000},
    {"HID_KEY_F12",             0x00100000, 0x02000000},
    {"HID_KEY_PRINT_SCREEN",    0x00004000, 0x02000000},
    {"HID_KEY_INSERT",          0x00100000, 0x01000000},
    {"HID_KEY_DELETE",          0x00080000, 0x01000000}
};


#define ARRAY_SIZE(X) (sizeof(X)/sizeof(X[0]))

int compareCounts(const void* pv1, const void* pv2)
{
    uint32_t* p1 = (uint32_t*)pv1;
    uint32_t* p2 = (uint32_t*)pv2;
    uint32_t count1 = p1[1];
    uint32_t count2 = p2[1];

    if (count1 == count2)
        return 0;
    else if (count1 < count2)
        return 1;
    else
        return -1;
}

int main(void)
{
    static const char filename[] = "../firmware/KeyMappings.h";
    FILE* pFile = fopen(filename, "w");
    if (!pFile)
    {
        printf("error: Failed to open %s for writing\n", filename);
        return 1;
    }
#ifdef UNDONE
    // I used the following code to count the times a particular pin was used. The ones with the lower counts were
    // assumed to be row and as each row was added to my list, I pulled the corresponding columns into the other list.
    // After about 5 rows were added to my list, almost all of the columns had been pulled.
    uint32_t counts[26][2];

    memset(counts, 0, sizeof(counts));

    for (size_t i = 0 ; i < ARRAY_SIZE(g_keys) ; i++)
    {
        uint32_t pinFrom = log2(g_keys[i].from);
        uint32_t pinTo = log2(g_keys[i].to);

        assert ( pinFrom < 26 && pinTo < 26 );
        counts[pinFrom][1]++;
        counts[pinTo][1]++;
    }

    printf("Counts (unsorted)\n");
    for (size_t i = 0 ; i < 26 ; i++)
    {
        counts[i][0] = i;
        printf("%u - %u\n", counts[i][0], counts[i][1]);
    }

    qsort(counts, 26, sizeof(uint32_t) * 2, compareCounts);

    printf("Counts (sorted)\n");
    for (size_t i = 0 ; i < 26 ; i++)
    {
        uint32_t pin = counts[i][0];
        printf("%u - %u\n", counts[i][0], counts[i][1]);
        for (size_t j = 0 ; j < ARRAY_SIZE(g_keys) ; j++)
        {
            uint32_t pin1 = log2(g_keys[j].from);
            uint32_t pin2 = log2(g_keys[j].to);

            if (pin == pin1)
            {
                printf("  %u to %u (%s)\n", pin, pin2, g_keys[j].pName);
            }
            else if (pin == pin2)
            {
                printf("  %u to %u (%s)\n", pin, pin1, g_keys[j].pName);
            }
        }
    }
#endif // UNDONE

    fprintf(pFile,  "/*  Copyright (C) 2018  Adam Green (https://github.com/adamgreen)\n"
                    "\n"
                    "    This program is free software; you can redistribute it and/or\n"
                    "    modify it under the terms of the GNU General Public License\n"
                    "    as published by the Free Software Foundation; either version 2\n"
                    "    of the License, or (at your option) any later version.\n"
                    "\n"
                    "    This program is distributed in the hope that it will be useful,\n"
                    "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                    "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                    "    GNU General Public License for more details.\n"
                    "*/\n"
                    "// Auto generated from /keycodes/main.c\n\n");

    fprintf(pFile, "typedef struct RowMapping\n"
                   "{\n"
                   "    uint32_t               bitmask;\n"
                   "    HidKeyboardUsageValues usage;\n"
                   "} RowMapping;\n\n");
    fprintf(pFile, "typedef struct ColumnMappings\n"
                   "{\n"
                   "    const RowMapping*      pMappings;\n"
                   "    uint32_t               bitmask;\n"
                   "    uint8_t                count;\n"
                   "} ColumnMappings;\n\n\n");

    uint8_t columns[] = {0, 1, 2, 3, 16, 17, 24, 25};
    for (size_t i = 0 ; i < ARRAY_SIZE(columns) ; i++)
    {
        uint32_t column = columns[i];
        uint32_t pin = 1 << column;

        fprintf(pFile, "static const RowMapping g_col%uRowMappings[] =\n"
                       "{\n", column);
        for (size_t j = 0 ; j < ARRAY_SIZE(g_keys) ; j++)
        {
            uint32_t pin1 = g_keys[j].from;
            uint32_t pin2 = g_keys[j].to;

            if (pin == pin1)
            {
                fprintf(pFile, "    {0x%08X, %s},\n", pin2, g_keys[j].pName);
            }
            else if (pin == pin2)
            {
                fprintf(pFile, "    {0x%08X, %s},\n", pin1, g_keys[j].pName);
            }

        }
        fprintf(pFile, "};\n");
    }

    fprintf(pFile, "\nstatic const ColumnMappings g_columnMappings[] =\n"
                   "{\n");
    for (size_t i = 0 ; i < ARRAY_SIZE(columns) ; i++)
    {
        uint32_t column = columns[i];
        uint32_t pin = 1 << column;

        fprintf(pFile, "    {\n"
                       "        g_col%uRowMappings,\n"
                       "        0x%08X,\n"
                       "        sizeof(g_col%uRowMappings)/sizeof(g_col%uRowMappings[0])\n"
                       "    },\n", column, ~pin, column, column);
    }
    fprintf(pFile, "};\n");

    fclose(pFile);
    pFile = NULL;
    return 0;
}
