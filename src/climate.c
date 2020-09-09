#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "spi.h"
#include "bme280.h"
#include "climate.h"
#include "pins.h"

static int8_t user_spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
static int8_t user_spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
static void user_delay_us(uint32_t period, void *intf_ptr);

str_climate_t climate_info = {0};

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

int climate_init(void) {

    int8_t rslt = BME280_OK;
    uint8_t settings_sel;

    climate_info.dev.intf_ptr = &climate_info.dev_addr;
    climate_info.dev.intf = BME280_SPI_INTF;
    climate_info.dev.read = user_spi_read;
    climate_info.dev.write = user_spi_write;
    climate_info.dev.delay_us = user_delay_us;

    rslt = bme280_init(&climate_info.dev);

    if( rslt == BME280_OK ) {
        climate_info.dev.settings.osr_h = BME280_OVERSAMPLING_1X;
        climate_info.dev.settings.osr_p = BME280_OVERSAMPLING_16X;
        climate_info.dev.settings.osr_t = BME280_OVERSAMPLING_2X;
        climate_info.dev.settings.filter = BME280_FILTER_COEFF_16;

        settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

        rslt = bme280_set_sensor_settings(settings_sel, &climate_info.dev);

        climate_info.req_delay = bme280_cal_meas_delay(&climate_info.dev.settings);

        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &climate_info.dev);

        climate_info.dev.delay_us(climate_info.req_delay, climate_info.dev.intf_ptr);

        rslt = bme280_get_sensor_data(BME280_ALL, &climate_info.comp_data, &climate_info.dev);

        climate_info.dev.delay_us(climate_info.req_delay, climate_info.dev.intf_ptr);
    }

    return rslt;
}

int climate_getData(void) {
    int8_t rslt = BME280_OK;

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &climate_info.dev);
    climate_info.dev.delay_us(climate_info.req_delay, climate_info.dev.intf_ptr);
    rslt = bme280_get_sensor_data(BME280_ALL, &climate_info.comp_data, &climate_info.dev);

    return rslt;
}
