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

#ifndef _SPI_H_
#define _SPI_H

uint8_t spi_init(void);
uint8_t spi_write(uint8_t dev, uint8_t *buf, uint8_t len);
uint8_t spi_read(uint8_t dev, uint8_t *buf, uint8_t len);

#endif // _SPI_H_

