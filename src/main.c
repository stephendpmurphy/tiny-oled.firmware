/****************************************************************************
    tiny-oled.firmware - A project to push the limits of my abilities and
    understanding of embedded firmware development.
    Copyright (C) 2020 Stephen Murphy - github.com/stephendpmurphy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "main.h"
#include "pins.h"
#include "spi.h"
#include "avr_ws2812.h"

#define	PIXEL_NUM   (8)

int main(void) {
    uint8_t x = 0;
    uint8_t i;
    ws2812_RGB_t pixels[PIXEL_NUM] = {0};
	ws2812_RGB_t p = {0, 100, 0};
    ws2812_RGB_t empty = {0,0,0};

    // Board init
    spi_init();

    // Init the STAT LED DD register
    LED_STAT_DDR |= (1 << LED_STAT_PIN);

    // Main application
    while(MY_VALUE) {
        x++;

        if(x > 7)
            x = 0;

        for (i = 0; i < PIXEL_NUM; ++i) {
            if(i == x) {
                pixels[i] = p;
            }
            else {
                pixels[i] = empty;
            }
        }
        ws2812_setleds(pixels, PIXEL_NUM);

        // Turn on the STAT LED
        LED_STAT_PORT |= (1 << LED_STAT_PIN);
        _delay_ms(250);
        // Turn off the STAT LED
        LED_STAT_PORT &= ~(1 << LED_STAT_PIN);
        // Wait 250 ms
        _delay_ms(250);
    }
}
