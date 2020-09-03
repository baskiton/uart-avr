#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>

/* Size of internal line buffer used by uart_getchar(). */
#define RX_BUFSIZE 80

// FILE uart_stdio;

extern void uart_init(uint32_t cpu_freq, uint32_t baudrate);
extern void uart_stdio_set(void);
extern void uart_end(void);

extern int uart_putchar(char c, FILE *stream);
extern int uart_getchar(FILE *stream);

#endif  /* !UART_H */
