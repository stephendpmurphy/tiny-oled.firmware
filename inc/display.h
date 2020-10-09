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

/*! @file display.h
 * @brief Module for driving the tiny-oled display.
 * This module displays information using the u8g2 lib connected to an SSD1306 OLED.
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*!
 * @brief This API initializes the u8g2 instance and writes the tiny-oled splash screen
 * onto the display.
 *
 * @param[in] void
 *
 * @return Returns void
 */
void display_init(void);

/*!
 * @brief This API displays the splash screen with our project name and username.
 *
 * @param[in] void
 *
 * @return Returns void
 */
void display_splash(void);

/*!
 * @brief This API displays the climate screen with our temp, humidity and pressure
 * values.
 *
 * @param[in] temp : Temperature reading to be displayed
 * @param[in] humidity : Humidity reading to be displayed
 * @param[in] pressure : Pressure reading to be displayed
 *
 * @return Returns void
 */
void display_climate(const long int temp, const long int humidity, const long int pressure);

/*!
 * @brief This API displays the telemetry screen with our x, y, and z values
 * values.
 *
 * @param[in] x_val : X value for the telemetry data
 * @param[in] y_val : Y value for the telemetry data
 * @param[in] z_val : Z value for the telemetry data
 *
 * @return Returns void
 */
void display_telem(const int16_t x_val, const int16_t y_val, const int16_t z_val);

void display_status(const uint8_t val);

#endif // _DISPLAY_H_