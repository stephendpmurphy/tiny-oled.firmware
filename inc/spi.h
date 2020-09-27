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

/*! @file spi.h
 * @brief Header file for the AVR SPI module API
 */

#ifndef _SPI_H_
#define _SPI_H

/*!
 * @brief This API initiliazes the AVR SPI module.
 *
 * @param void
 *
 * @return void
 */
void spi_init(void);

/*!
 * @brief This API writes data via the AVR SPI module.
 *
 * @param[in] *buf : Pointer to a buffer containing data to be written
 * @param[in] len : Length of data to be written from our buffer via SPI
 *
 * @return Returns the state of the SPI write
 */
uint8_t spi_write(const uint8_t *buf, const uint8_t len);

/*!
 * @brief This API reads data via the AVR SPI module.
 *
 * @param[out] *buf : Pointer to a buffer where our read data should be placed
 * @param[in] len : Length of data to be read to our buffer via SPI
 *
 * @return Returns the state of the SPI read
 */
uint8_t spi_read(uint8_t *buf,  const uint8_t len);

/*!
 * @brief This API asserts the CS line for our desired device.
 *
 * @param[in] port : Which PORT we want to update
 * @param[in] pin : Which PIN we want to update in the provided PORT
 * @param[in] val : Value to be written to our PIN in the specified PORT
 *
 * @return void
 */
void spi_assertCS(volatile uint8_t *port, const uint8_t pin, const uint8_t val);

#endif // _SPI_H_