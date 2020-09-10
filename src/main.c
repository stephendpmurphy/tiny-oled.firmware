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
#include "display.h"
#include "bme280.h"
#include "climate.h"

#define	PIXEL_NUM   (8)

int main(void) {
    int8_t rslt = BME280_OK;

    // Board init
    spi_init();
    // Init the OLED Display
    display_init();
    // Init the BME280 Climate sensor
    rslt = climate_init();

    // Init the STAT LED DD register
    LED_STAT_DDR |= (1 << LED_STAT_PIN);

    _delay_ms(1500);

    // Main application
    while(1) {

        rslt = climate_getData();

        if( rslt == BME280_OK )
        {
            display_climate(climate_data.temperature, climate_data.humidity, climate_data.pressure);
        }

        _delay_ms(100);
    }
}
