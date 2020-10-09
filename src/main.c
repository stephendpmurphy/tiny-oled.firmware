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

/*! @file main.c
 * @brief Main source for the tiny-oled firmware
 */

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "main.h"
#include "pins.h"
#include "spi.h"
#include "avr_ws2812.h"
#include "pff.h"
#include "diskio.h"
#include "display.h"
#include "climate.h"
#include "telemetry.h"
#include "tick.h"
#include "usb.h"
#include "diskio.h"
#include "pff.h"

/*! @brief Enum for the different states our device coule be in */
typedef enum {
    DEV_STATE_SPLASH = 0x00,
    DEV_STATE_CLIMATE,
    DEV_STATE_TELEM
} eState_t;

/*! @brief Structure holding our Device state and ref times */
typedef struct {
    eState_t state;
    uint32_t state_refTime;
    uint32_t telem_data_refTime;
    uint32_t disp_refTime;
} strDevice_t;

#define TELEM_DATA_TIME     (30)    // ms
#define SPLASH_DISP_TIME    (1500)  // ms
#define STAT_LED_FLASH_RATE (1000)  // ms
#define DISP_UPDATE_RATE    (30)    // ms

/*! @brief Structure holding our Device state and ref times */
static strDevice_t Device;

/*!
 * @brief This function updates the display based on the current device state
 *
 * @param[in] void
 *
 * @returns Returns void
 */
static void updateDisplay(void) {
    // Check if we are due for refreshing the display
    if( (Device.disp_refTime + DISP_UPDATE_RATE) > tick_getTick())
        return;

    // Based on which state we are, display the appropriate screen
    switch( Device.state ) {
        case DEV_STATE_SPLASH:
            display_splash();
            break;

        case DEV_STATE_CLIMATE:
            display_climate(climate_data.temperature, climate_data.humidity, climate_data.pressure);
            break;

        case DEV_STATE_TELEM:
            display_telem(accel_data.x, accel_data.y, accel_data.z);
            break;

        default:
            break;
    }
}

/*!
 * @brief This functions runs state specific code based on the current
 * device state.
 *
 * @param[in] void
 *
 * @returns Returns void
 */
static void dev_sm(void) {
    char dataString[64] = {0};

    switch( Device.state ) {
        case DEV_STATE_SPLASH:
            // Check if we have been in the splash long enough. If so, transition to climate
            if( tick_timeSince(Device.state_refTime) > SPLASH_DISP_TIME ) {
                Device.state = DEV_STATE_TELEM;
                Device.state_refTime = tick_getTick();
            }
            break;

        case DEV_STATE_CLIMATE:
            // Retrieve device data
            climate_getData();
            break;

        case DEV_STATE_TELEM:
            // Retrieve our telemetry data
            telemetry_getData();

            // If we are due for it, print the data out over USB
            if( tick_timeSince(Device.telem_data_refTime) > TELEM_DATA_TIME ) {
                memset(dataString, 0x00, sizeof(dataString));
                sprintf(dataString, "\33[2Kaccel x:%d y:%d z:%d\r",
                    accel_data.x, accel_data.y, accel_data.z);

                usb_sendString((const uint8_t *)dataString, sizeof(dataString));
                Device.telem_data_refTime = tick_getTick();
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function and entry point for the firmware
 *
 * @param[in] void
 *
 * @return Returns void
 */
int main(void) {
    const char* textString = "this is a test!\r\n";
    uint8_t res;
    FATFS fs;			/* File system object */

    // Board init
    tick_init();
    spi_init();

    // Driver init
    display_init();
    climate_init();
    telemetry_init();
    usb_init();

    res = disk_initialize();
    if( res == RES_OK ) {
        res = pf_mount(&fs);
    }

    if( res == FR_OK) {
        res |= pf_open("HELLO.TXT");
    }

    if( res == FR_OK ) {
        res |= pf_write(textString, strlen("this is a test!\r\n"), NULL);
        res |= pf_write(0, 0, NULL);
    }

    while(1) {
        display_status(res);
        _delay_ms(500);
    }


    Device.state = DEV_STATE_SPLASH;
    Device.state_refTime = tick_getTick();

    while(1) {
        // Run the USB task
        usb_update();

        // Update the LED UI

        // Handle button events

        // Run the device state machine
        dev_sm();

        // Handle the oled display
        updateDisplay();

        // Throttle the application a bit
        _delay_ms(10);
    }
}
