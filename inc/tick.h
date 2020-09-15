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

/*! @file tick.h
 * @brief Header file for the tick module used for event timing
 */

#ifndef _TICK_H_
#define _TICK_H_

/*!
 * @brief This API initiliazes the tick module and timer
 *
 * @param void
 *
 * @returns void
 */
void tick_init(void);

/*!
 * @brief This API returns the current tick value
 *
 * @param void
 *
 * @returns Returns the current tick value
 */
uint32_t tick_getTick(void);

/*!
 * @brief This API returns time since a passed in ref time
 *
 * @param[in] ref : The reference in which you would like to
 * know how much time as elapsed since.
 *
 * @returns Returns the time since the passed in ref value.
 */
uint32_t tick_timeSince(uint32_t ref);

#endif // _TICK_H_