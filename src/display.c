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

/*! @file display.c
 * @brief Module for driving the tiny-oled display.
 * This module displays information using the u8g2 lib connected to an SSD1306 OLED.
 */

#include <stdio.h>
#include <util/delay.h>
#include "display.h"
#include "spi.h"
#include "pins.h"
#include "u8g2.h"

/*!
 * @brief API for setting/resetting the SSD1306 Reset pin
 *
 * @param[in] val : Value to write to the RESET pin
 *
 * @return void
 */
static void _set_res_pin(uint8_t val);

/*!
 * @brief API for setting/resetting the SSD1306 DC pin
 *
 * @param[in] val : Value to write to the DC pin
 *
 * @return void
 */
static void _set_dc_pin(uint8_t val);

/*!
 * @brief Callback for calling AVR specific GPIO control and delay functions.
 *
 * @param[in] u8x8 : Pointer to the u8g2 dev instance
 * @param[in] msg : Msg/event to be executed using AVR specific HW calls
 * @param[in] arg_int : Param to be used with the msg/event sent
 * @param[in] *arg_ptr : Pointer to a buffer to be used with the msg/event
 *
 * @return Result of GPIO/Delay msg/event
 */
static uint8_t u8g2_gpio_and_delay_avr(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);

/*!
 * @brief Callback for calling AVR specific SPI functions.
 *
 * @param[in] u8x8 : Pointer to the u8g2 dev instance
 * @param[in] msg : Msg/event to be executed using AVR specific SPI functions
 * @param[in] arg_int : Param to be used with the msg/event sent
 * @param[in] *arg_ptr : Pointer to a buffer to be used with the msg/event
 *
 * @return Result of SPI msg/event
 */
static uint8_t u8x8_byte_4wire_sw_spi_avr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/*!
 * @brief Instance of our u8g2 lib used to drive and interface to the OLED
 */
static u8g2_t u8g2;

/*!
 * @brief API for setting/resetting the SSD1306 Reset pin
 */
static void _set_res_pin(uint8_t val) {
    if( val )
    {
        DISP_RES_PORT |= (0x01 << DISP_RES_PIN);
    }
    else
    {
        DISP_RES_PORT &= ~(0x01 << DISP_RES_PIN);
    }
}

/*!
 * @brief API for setting/resetting the SSD1306 DC pin
 */
static void _set_dc_pin(uint8_t val) {
    if( val )
    {
        DISP_DC_PORT |= (0x01 << DISP_DC_PIN);
    }
    else
    {
        DISP_DC_PORT &= ~(0x01 << DISP_DC_PIN);
    }
}

/*!
 * @brief Callback for calling AVR specific GPIO control and delay functions.
 */
static uint8_t u8g2_gpio_and_delay_avr(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            _delay_ms(1);
            break;
        case U8X8_MSG_DELAY_MILLI:
            while( arg_int--) _delay_ms(1);
            break;
        case U8X8_MSG_GPIO_DC:
            _set_dc_pin(arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
            _set_res_pin(arg_int);
            break;
    }
    return 1;
}

/*!
 * @brief Callback for calling AVR specific SPI functions.
 */
static uint8_t u8x8_byte_4wire_sw_spi_avr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            spi_write(arg_ptr, arg_int);
            break;

        case U8X8_MSG_BYTE_SET_DC:
            _set_dc_pin(arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            spi_assertCS(&SPI_DISP_CS_PORT, SPI_DISP_CS_PIN, 0);
            asm("NOP");
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            spi_assertCS(&SPI_DISP_CS_PORT, SPI_DISP_CS_PIN, 1);
            asm("NOP");
        default:
            return 0;
    }
    return 1;
}

/*!
 * @brief This API initializes the u8g2 instance and writes the tiny-oled splash screen
 * onto the display.
 */
void display_init(void) {
    // Init the RESET and DC pins for the display
    DISP_RES_DDR |= (0x01 << DISP_RES_PIN);
    DISP_DC_DDR |= (0x01 << DISP_DC_PIN);

    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, U8G2_R0, (u8x8_msg_cb)u8x8_byte_4wire_sw_spi_avr, (u8x8_msg_cb)u8g2_gpio_and_delay_avr);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_FirstPage(&u8g2);
    do
    {
        u8g2_SetFontMode(&u8g2, 1);
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawBox(&u8g2, 30, 0, 74, 15);
        u8g2_SetDrawColor(&u8g2, 2);

        u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
        u8g2_DrawStr(&u8g2, 36, 11, "tiny-OLED");
        u8g2_DrawStr(&u8g2, 15, 29, "stephendpmurphy");
    } while (u8g2_NextPage(&u8g2));
}

/*!
 * @brief This API displays the climate screen with our temp, humidity and pressure
 * values.
 */
void display_climate(long int temp, long int humidity, long int pressure) {
    char str[64] = {0x00};

    u8g2_FirstPage(&u8g2);
    do
    {
        u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);

        sprintf(str, "T: %ld", temp);
        u8g2_DrawStr(&u8g2, 0, 14, str);

        sprintf(str, "P: %ld", pressure);
        u8g2_DrawStr(&u8g2, 64, 14, str);

        sprintf(str, "H: %ld", humidity);
        u8g2_DrawStr(&u8g2, 0, 30, str);
    } while (u8g2_NextPage(&u8g2));
}