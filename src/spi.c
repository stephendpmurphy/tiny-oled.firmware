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

#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "spi.h"
#include "pins.h"

void spi_init(void) {

    // Set MOSI and SCK as outputs
    SPI_MOSI_DDR    |= (0x01 << SPI_MOSI_PIN);
    SPI_SCK_DDR     |= (0x01 << SPI_SCK_PIN);

    // When in MASTER mode, we need to ensure the !SS pin
    // is an output and driven high. Otherwise the SPI module
    // will switch to a slave mode
    SPI_SS_DDR      |= (0x01 << SPI_SS_PIN);
    SPI_SS_PORT     |= (0x01 << SPI_SS_PIN);

    // SD Chip Select
    SPI_SD_CS_DDR   |= (0x01 << SPI_SD_CS_PIN);
    SPI_SD_CS_PORT  |= (0x01 << SPI_SD_CS_PIN);

    // SPI Register Init
    SPCR |= (1 << SPE) | (0x01 << MSTR); // SPI Enable | Master Mode
}

uint8_t spi_write(uint8_t *buf, uint8_t len) {

    uint8_t tx_count = 0;

    // Make sure the length is non zero, and we weren't
    // given a NULL ptr buffer
    if( (len <= 0x00) || (buf == NULL) ) {
        return EXIT_FAILURE;
    }

    // While we still have bytes to write
    while(tx_count < len) {
        // Load the data for transmit
        SPDR = buf[tx_count];
        // Wait for it to finish transmitting
        while(!(SPSR & (1<<SPIF)));
        // Decrement the len count
        tx_count++;
    }

    return EXIT_SUCCESS;
}

uint8_t spi_read(uint8_t *buf, uint8_t len) {

    // Make sure the length is non zero, and we weren't
    // given a NULL ptr buffer
    if( (len <= 0x00) || (buf == NULL) ) {
        return EXIT_FAILURE;
    }

    // While we still have bytes to read
    while(len > 0) {
        SPDR = 0xFF; // Load a dummy byte
        while(!(SPSR & (1<<SPIF)));
        // Grab a byte from the input buffer
        memcpy(&buf[len], (void *)&SPDR, 0x01);
        // Decrement the len count
        len--;
    }

    return EXIT_SUCCESS;
}