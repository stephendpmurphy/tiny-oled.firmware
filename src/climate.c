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

/*! @file climate.c
 * @brief Module used to interact with the bme280 temp and humidity sensor
 */

#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "spi.h"
#include "bme280.h"
#include "climate.h"
#include "pins.h"

/*!
 * @brief Callback for AVR specific SPI writes driven by the BME280 driver
 *
 * @param[in] reg_addr : Address to write to via SPI
 * @param[in] *data : Pointer to a buffer of data to be written via SPI
 * @param[in] len : Length of data to be written from the data buffer
 * @param[in] *intf_ptr : Pointer to our interface - Only used if more than one BME280 is connected
 *
 * @return Result of the SPI write
 */
static int8_t user_spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 * @brief Callback for AVR specific SPI reads driven by the BME280 driver
 *
 * @param[in] reg_addr : Address to read from via SPI
 * @param[out] *data : Pointer to a buffer where the read data should be placed
 * @param[in] len : Length of data to be read from the device and written to our data buffer
 * @param[in] *intf_ptr : Pointer to our interface - Only used if more than one BME280 is connected
 *
 * @return Result of the SPI read
 */
static int8_t user_spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 * @brief Callback for our AVR specific delay
 *
 * @param[in] period : Duration we should delay - in us
 * @param[in] *intf_ptr : Pointer to a buffer to be used with the msg/event
 *
 * @return Result of GPIO/Delay msg/event
 */
static void user_delay_us(uint32_t period, void *intf_ptr);

/*! @brief bme280 captured climate data */
struct bme280_data climate_data;

/*! @brief bme280 device instance */
struct bme280_dev dev;

/*! @brief bme280 device address/instance */
uint8_t dev_addr;

/*! @brief bme280 min req delay between samples */
uint32_t req_delay;

/*!
 * @brief Callback for AVR specific SPI writes driven by the BME280 driver
 */
static int8_t user_spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    // Check the input parameters
    if( data == NULL )
    {
        return BME280_E_NULL_PTR;
    }

    // Assert CS
    spi_assertCS(&SPI_BME280_CS_PORT, SPI_BME280_CS_PIN, 0);

    // Transmit the address
    spi_write(&reg_addr, 0x01);
    // Transmit the data buffer
    spi_write((uint8_t *)data, len);

    // De-assert CS
    spi_assertCS(&SPI_BME280_CS_PORT, SPI_BME280_CS_PIN, 1);

    return BME280_OK;
}

/*!
 * @brief Callback for AVR specific SPI reads driven by the BME280 driver
 */
static int8_t user_spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    // Check the input parameters
    if( data == NULL )
    {
        return BME280_E_NULL_PTR;
    }

    // Assert CS
    spi_assertCS(&SPI_BME280_CS_PORT, SPI_BME280_CS_PIN, 0);

    // Transmit the address
    spi_write(&reg_addr, 0x01);
    // Read the data back
    spi_read(data, len);

    // De-assert CS
    spi_assertCS(&SPI_BME280_CS_PORT, SPI_BME280_CS_PIN, 1);

    return BME280_OK;
}

/*!
 * @brief Callback for our AVR specific delay
 */
static void user_delay_us(uint32_t period, void *intf_ptr) {
    while( period > 0) {
        _delay_us(1);
        period--;
    }
}

/*!
 * @brief This API initializes the climate module and sets up the BME280 register
 */
int8_t climate_init(void) {
    int8_t rslt = BME280_OK;
    uint8_t settings_sel;

    // Set our dev SPI & Delay functions
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_SPI_INTF;
    dev.read = user_spi_read;
    dev.write = user_spi_write;
    dev.delay_us = user_delay_us;

    // Init thhe BME280 driver
    rslt = bme280_init(&dev);

    if( rslt == BME280_OK ) {
        // Updating the sampling settings
        dev.settings.osr_h = BME280_OVERSAMPLING_1X;
        dev.settings.osr_p = BME280_OVERSAMPLING_16X;
        dev.settings.osr_t = BME280_OVERSAMPLING_2X;
        dev.settings.filter = BME280_FILTER_COEFF_16;
        dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

        settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL | BME280_STANDBY_SEL;

        // Apply the sensor settings
        rslt = bme280_set_sensor_settings(settings_sel, &dev);
    }

    if( rslt == BME280_OK ) {
        // Set the sensor mode to normal
        rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);
    }

    if( rslt == BME280_OK ) {
        // Determine the minimum delay needed with our current cal and sample settings
        req_delay = bme280_cal_meas_delay(&dev.settings);
        // Delay the above determined amount of time
        dev.delay_us(req_delay, dev.intf_ptr);
        // Retrieve the sensor data for the first time
        rslt = bme280_get_sensor_data(BME280_ALL, &climate_data, &dev);
    }

    return rslt;
}

/*!
 * @brief This API retrieves temp, pressure, and humidity data from the BME280
 */
int8_t climate_getData(void) {
    int8_t rslt = BME280_OK;
    // Retrieve the sensor data
    rslt = bme280_get_sensor_data(BME280_ALL, &climate_data, &dev);

    return rslt;
}
