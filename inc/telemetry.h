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

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

#include <stdint.h>
#include "icm20948_api.h"

/*!
 * @brief This API initializes the telemetry module
 *
 * @param[in] void
 *
 * @return Returns the telemetry init status
 */
int8_t telemetry_init(void);

/*!
 * @brief This API retrieves a sample of data from the telemetry module
 *
 * @param[in] void
 *
 * @return Returns the telemetry status from retrieving a data sample
 */
int8_t telemetry_getData(void);

/*! @brief ICM20948 captured gyro data */
extern icm20948_gyro_t gyro_data;
/*! @brief ICM20948 captured accel data */
extern icm20948_accel_t accel_data;

#endif // _TELEMETRY_H_