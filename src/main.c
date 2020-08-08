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

int main(void) {

    // Board init
    spi_init();

    // Init the STAT LED DD register
    LED_STAT_DDR |= (1 << LED_STAT_PIN);

    // Main application
    while(MY_VALUE) {
        // Turn on the STAT LED
        LED_STAT_PORT |= (1 << LED_STAT_PIN);
        // Wait 1000 ms
        _delay_ms(1000);
        // Turn off the STAT LED
        LED_STAT_PORT &= ~(1 << LED_STAT_PIN);
        // Wait 250 ms
        _delay_ms(250);
    }
}
