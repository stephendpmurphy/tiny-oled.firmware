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

/*! @file tick.c
 * @brief Source for the tick module which provides a tick used in scheduling
 * and handling device events
 */

#include <stdio.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define TICK_PERIOD (2) // ms

/*! @brief Current tick val in 2ms increments */
static uint32_t tick_val = 0x0000;

/*!
 * @brief This API initiliazes the tick module and timer
 */
void tick_init(void) {
    // Disable all interrupts
    cli();

    // Configure TIMER0 for a CLK/64 pre-scaler
    TCCR0A &= 0x00;
    TCCR0B |= (0x01 << CS01) | (0x01 << CS00);
    // Enable the overflow interrupt
    TIMSK0 |= (0x01 << TOIE0);

    // Enable all interrupts
    sei();
}

/*!
 * @brief This API returns the current tick value
 */
uint32_t tick_getTick(void) {
    return tick_val;
}

/*!
 * @brief This API returns time since a passed in ref time
 */
uint32_t tick_timeSince(uint32_t ref) {
    // If the ref time given is a time
    // greater than our current tick val
    // return 0.
    if( ref > tick_val )
        return 0;

    // Otherwise, return the difference of the ref
    // and the current tick.
    return (tick_val - ref);
}

/*!
 * @brief ISR for the Timer0 overflow interrupt
 */
ISR(TIMER0_OVF_vect)
{
    if( tick_val  >= (UINT32_MAX - 2) ) {
        tick_val = 0x0000;
    }

    tick_val += TICK_PERIOD;
}
