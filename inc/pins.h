/****************************************************************************
    tiny-oled.firmware - A project to the limits of my abilities and
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

#ifndef _PINS_H_
#define _PINS_H_

#include <avr/io.h>

// SPI Pin definitions
#define SPI_MOSI_DDR    (DDRA)
#define SPI_MOSI_PIN    (4)

#define SPI_SCK_DDR     (DDRA)
#define SPI_SCK_PIN     (5)

#define SPI_SD_CS_DDR   (DDRA)
#define SPI_SD_CS_PORT  (PORTA)
#define SPI_SD_CS_PIN   (2)

#endif // _PINS_H_