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

#ifndef _PINS_H_
#define _PINS_H_

#include <avr/io.h>

// SPI Pin definitions
#define SPI_MOSI_DDR        (DDRB)
#define SPI_MOSI_PIN        (2)

#define SPI_SCK_DDR         (DDRB)
#define SPI_SCK_PIN         (1)

// AVR SPI SS pin
#define SPI_SS_DDR          (DDRB)
#define SPI_SS_PORT         (PORTB)
#define SPI_SS_PIN          (0)

// SD CS
#define SPI_SD_CS_DDR       (DDRB)
#define SPI_SD_CS_PORT      (PORTB)
#define SPI_SD_CS_PIN       (3)

// Display CS
#define SPI_DISP_CS_DDR     (DDRD)
#define SPI_DISP_CS_PORT    (PORTD)
#define SPI_DISP_CS_PIN     (6)

// BME280 CS
#define SPI_BME280_CS_DDR   (DDRB)
#define SPI_BME280_CS_PORT  (PORTB)
#define SPI_BME280_CS_PIN   (5)

// SSD1306 Pin definitions
#define DISP_RES_DDR        (DDRB)
#define DISP_RES_PORT       (PORTB)
#define DISP_RES_PIN        (4)

#define DISP_DC_DDR         (DDRD)
#define DISP_DC_PORT        (PORTD)
#define DISP_DC_PIN         (7)

// LED Status Pin definitions
#define LED_STAT_DDR        (DDRD)
#define LED_STAT_PORT       (PORTD)
#define LED_STAT_PIN        (4)

#endif // _PINS_H_