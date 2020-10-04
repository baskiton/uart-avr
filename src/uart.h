#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>

FILE uart_stream;

extern void uart_init(uint32_t baudrate);

extern void uart_set_stdio(void);
extern void uart_set_stdin(void);
extern void uart_set_stdout(void);

extern void uart_end(void);

extern int uart_putchar(char c, FILE *stream);
extern int uart_getchar(FILE *stream);

#endif  /* !UART_H */
