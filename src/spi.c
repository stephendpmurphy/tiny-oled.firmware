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

/*! @file spi.c
 * @brief Module to init, read, and write data via the AVR SPI module
 */

#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "spi.h"
#include "pins.h"

/*!
 * @brief This API initiliazes the AVR SPI module.
 */
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
    // Set it high
    SPI_SD_CS_PORT  |= (0x01 << SPI_SD_CS_PIN);

    // Display Chip Select
    SPI_DISP_CS_DDR |= (0x01 << SPI_DISP_CS_PIN);
    // Set it high
    SPI_DISP_CS_PORT |= (0x01 << SPI_DISP_CS_PIN);

    // BME280 Chip Select
    SPI_BME280_CS_DDR |= (0x01 << SPI_BME280_CS_PIN);
    // Set it high
    SPI_BME280_CS_PORT |= (0x01 << SPI_BME280_CS_PIN);

    // ICM20948 Chip Select
    SPI_ICM20948_CS_DDR |= (0x01 << SPI_ICM20948_CS_PIN);
    // Set it high
    SPI_ICM20948_CS_PORT |= (0x01 << SPI_ICM20948_CS_PIN);

    // SPI Register Init
    SPCR |= (1 << SPE) | (0x01 << MSTR); // SPI Enable | Master Mode
}

/*!
 * @brief This API writes data via the AVR SPI module.
 */
uint8_t spi_write(const uint8_t *buf, const uint8_t len) {

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
        // Increment the tx count
        tx_count++;
    }

    return EXIT_SUCCESS;
}

/*!
 * @brief This API reads data via the AVR SPI module.
 */
uint8_t spi_read(uint8_t *buf, const uint8_t len) {

    uint8_t rx_count = 0;

    // Make sure the length is non zero, and we weren't
    // given a NULL ptr buffer
    if( (len <= 0x00) || (buf == NULL) ) {
        return EXIT_FAILURE;
    }

    // While we still have bytes to read
    while(rx_count < len) {
        // Load a dummy byte to transmit
        SPDR = 0xFF;
        // Wait for the transmission to finish
        while(!(SPSR & (1 << SPIF)));
        // Grab a byte from the input buffer
        memcpy(&buf[rx_count], (void *)&SPDR, 0x01);
        // Increment the RX count
        rx_count++;
    }

    return EXIT_SUCCESS;
}

/*!
 * @brief This API asserts the CS line for our desired device.
 */
void spi_assertCS(volatile uint8_t *port, const uint8_t pin, const uint8_t val) {

    if( val ) {
        *port |= (0x01 << pin);
    }
    else {
        *port &= ~(0x01 << pin);
    }
}