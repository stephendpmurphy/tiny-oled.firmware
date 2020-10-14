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

/*! @file uart.c
 * @brief Module to init, read, and write data via the AVR UART module
 */

#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

static void uart_putchar(char n);
int uart_putchar_printf(char var, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

/*!
 * @brief This API sends a single character out the uart
 *
 * @param[in] n: Character to send
 */
static void uart_putchar(char n) {
    // Wait for empty transmit buffer
    while ( !(UCSR1A & (_BV(UDRE1))) );
    // Start transmission
    UDR1 = n;
}

/*!
 * @brief This API is a callback used by printf
 * to send a character out the UART
 *
 * @param[in] var: Character to send
 * @param[in] *stream: stdout stream which is sending the data
 *
 * @return Status of sending a character
 */
int uart_putchar_printf(char var, FILE *stream) {
    uart_putchar(var);
    return 0;
}

/*!
 * @brief This API initializes the UART
 */
void uart_init(void) {
    // setup our stdio stream
    stdout = &mystdout;
    /* Set baudrate to 9600 Table 18-6 of ATmega32u4 datasheet */
    UBRR1L = 51;
    /* Enable receiver and transmitter */
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    /* Set frame format: 8data, 1stop bit */
    UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
}
