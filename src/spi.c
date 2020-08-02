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

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "spi.h"
#include "pins.h"

void spi_init(void) {

    // MOSI and SCK pin init
    SPI_MOSI_DDR    |= (0x01 << SPI_MOSI_PIN);
    SPI_SCK_DDR     |= (0x01 << SPI_MOSI_PIN);

    // SD Chip Select
    SPI_SD_CS_DDR   |= (0x01 << SPI_SD_CS_PIN);

    // SPI Register Init
    SPCR |= (1 << SPE) | (0x01 << MSTR); // SPI Enable | Master Mode
}

uint8_t spi_write(uint8_t *buf, uint8_t len) {

    // Make sure the length is non zero, and we weren't
    // given a NULL ptr buffer
    if( (len <= 0x00) || (buf == NULL) ) {
        return EXIT_FAILURE;
    }

    // While we still have bytes to write
    while(len > 0) {
        // Load the data for transmit
        SPDR = buf[len];
        // Wait for it to finish transmitting
        while( !(SPSR & (0x01 << SPIF)) );
        // Decrement the len count
        len--;
    }

    return EXIT_SUCCESS;
}

uint8_t spi_read(uint8_t *buf, uint8_t len) {

    // Make sure the length is non zero, and we weren't
    // given a NULL ptr buffer
    if( (len <= 0x00) || (buf == NULL) ) {
        return 1;
    }

    // While we still have bytes to read
    while(len > 0) {
        // Grab a byte from the input buffer
        memcpy(&buf[len], (void *)&SPDR, 0x01);
        // Decrement the len count
        len--;
    }

    return EXIT_SUCCESS;
}

void spi_assert_cs(uint8_t port, uint8_t pin) {
    port |= (1 << pin);
}

void spi_deassert_cs(uint8_t port, uint8_t pin) {
    port &= ~(1 << pin);
}
