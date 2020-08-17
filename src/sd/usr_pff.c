#include <stdint.h>
#include <util/delay.h>
#include "pins.h"
#include "spi.h"

void usr_assert_cs(void) {
    SPI_SD_CS_DDR &= ~(1 << SPI_SD_CS_BIT);
}

void usr_deassert_cs(void) {
    SPI_SD_CS_DDR |= (1 << SPI_SD_CS_PIN);
}

uint8_t usr_is_cs_low(void) {
    return(!(SPI_SD_CS_PIN & (1 << SPI_SD_CS_BIT)));
}

void usr_spi_tx_byte(uint8_t byte) {
    spi_write(&byte, 0x01);
}

uint8_t usr_spi_rx_byte(void) {
    uint8_t br = 0x00;

    spi_read(&br, 0x01);

    return br;
}

void usr_delay100us(void) {
    _delay_us(100);
}
