/****************************************************************************
    tiny-oled.firmware - A project to push the limits of my abilities and
    understanding of embedded firmware development.
    Copyright (C) 2020 Stephen Murphy - github.com/stephendpmurphy

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
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
