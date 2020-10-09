#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>

FILE uart_stream;

void uart_init(uint32_t baudrate);

void uart_set_stdio(void);
void uart_set_stdin(void);
void uart_set_stdout(void);

void uart_end(void);

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif  /* !UART_H */
