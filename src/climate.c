#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "spi.h"
#include "bme280.h"
#include "climate.h"
#include "pins.h"

struct bme280_data climate_data;

static int8_t user_spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
static int8_t user_spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
static void user_delay_us(uint32_t period, void *intf_ptr);

struct bme280_dev dev;
uint8_t dev_addr;
uint32_t req_delay;

int8_t user_spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    // Check the input parameters
    if( data == NULL )
    {
        return BME280_E_NULL_PTR;
    }

    // Assert CS
    SPI_BME280_CS_PORT &= ~(0x01 << SPI_BME280_CS_PIN);

    // Transmit the address
    spi_write(&reg_addr, 0x01);
    // Transmit the data buffer
    spi_write((uint8_t *)data, len);

    // De-assert CS
    SPI_BME280_CS_PORT |= (0x01 << SPI_BME280_CS_PIN);

    return BME280_OK;
}

int8_t user_spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    // Check the input parameters
    if( data == NULL )
    {
        return BME280_E_NULL_PTR;
    }

    // Assert CS
    SPI_BME280_CS_PORT &= ~(0x01 << SPI_BME280_CS_PIN);

    // Transmit the address
    spi_write(&reg_addr, 0x01);
    // Read the data back
    spi_read(data, len);

    // De-assert CS
    SPI_BME280_CS_PORT |= (0x01 << SPI_BME280_CS_PIN);

    return BME280_OK;
}

void user_delay_us(uint32_t period, void *intf_ptr) {
    while( period > 0) {
        _delay_us(1);
        period--;
    }
}

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

int8_t climate_getData(void) {
    int8_t rslt = BME280_OK;
    // Retrieve the sensor data
    rslt = bme280_get_sensor_data(BME280_ALL, &climate_data, &dev);

    return rslt;
}
