#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <defines.h>
#include "uart.h"

static FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/*!
 * @brief Perform UART startup initialization.
 * Initialize the UART, tx/rx, 8N1.
 * @param baudrate Baudrate
 */
void uart_init(uint32_t baudrate) {
    UCSR0B = 0x00;  // disable while setting baud rate
    // UCSR0A = 0x02;  // set U2x
    // UCSR0C = 0x06;  // Asynchronous, 8 bits data & 2(1?) stop bits

    /* calculate UBRR */
    uint16_t ubrr = (F_CPU / (16UL * baudrate)) - 1;   // for async, normal mode (U2X = 0)
    // uint16_t ubrr = (F_CPU / (8UL * baudrate)) - 1;    // for async, 2x speed (U2X = 1)
    // uint16_t ubrr = (F_CPU / (2UL * baudrate)) - 1;    // for sync

    /* Set baud rate */
    UBRR0L = (uint8_t)ubrr;
    UBRR0H = (uint8_t)(ubrr >> 8);
    
    /* Set frame format: Asynchronous, 8data, 1 stop bit */
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    
    /* Enable receiver and transmitter */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

/*!
 * @brief UART turn off
 */
void uart_end(void) {
    bit_clear(UCSR0B, RXEN0);
    bit_clear(UCSR0B, TXEN0);
    bit_clear(UCSR0B, RXCIE0);
    bit_clear(UCSR0B, UDRIE0);
    
    fdev_close();
}

/*!
 * @brief Send one character to the UART.
 * @param c Sending char
 * @param stream Stream to sending
 */
int uart_putchar(char c, FILE *stream) {
    /* Wait for empty transmit buffer */
    loop_until_bit_is_set(UCSR0A, UDRE0);

    /* Put data into buffer, sends the data */
    UDR0 = c;
    return 0;
}

/*!
 * @brief Receive one character from the UART. The actual reception is
 * line-buffered, and one character is returned from the buffer at
 * each invokation.
 * @param stream Stream from getting
 * @return Getting char; or EOF/another error
 */
int	uart_getchar(FILE *stream) {
    char c;
    
    loop_until_bit_is_set(UCSR0A, RXC0);
    if (UCSR0A & _BV(FE0))
        return _FDEV_EOF;
    if (UCSR0A & _BV(DOR0))
        return _FDEV_ERR;
    c = UDR0;

    if (c == '\r')
        c = '\0';

    return c;
}

/*!
 * @brief Set UART as stdio
 */
void uart_set_stdio(void) {
    stdout = stdin = &uart_stream;
}

/*!
 * @brief Set UART as std in
 */
void uart_set_stdin(void) {
    stdin = &uart_stream;
}

/*!
 * @brief Set UART as std out
 */
void uart_set_stdout(void) {
    stdout = &uart_stream;
}

