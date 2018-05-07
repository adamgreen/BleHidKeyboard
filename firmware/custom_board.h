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
#ifndef CUSTOM_BOARD_H
#define CUSTOM_BOARD_H

// LEDs definitions for our custom board
#define LEDS_NUMBER    1

#define LED_START      30
#define LED_1          30
#define LED_STOP       30

#define LEDS_LIST { LED_1 }

#define BSP_LED_0      LED_1

#define BSP_LED_0_MASK (1<<BSP_LED_0)
#define BSP_LED_1_MASK 0

#define LEDS_MASK      (BSP_LED_0_MASK | BSP_LED_1_MASK)

// All LEDs are lit when GPIO is high.
#define LEDS_INV_MASK  0

// There are no buttons on this board. Will use keyboard matrix instead.
#define BUTTONS_NUMBER 0

#endif // CUSTOM_BOARD_H
