#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include <uart.h>

void cmd_help(void) {
    printf_P(PSTR(
        "bOSduino - arduino/avr operating system\n"
        "Commands list:\n"
        " - help:    display this message\n"
        " - cls:     clear screen/display\n"
        " - sp:      Stack Pointer\n"
        " - reboot:  reboot system/device\n"
        " - exit:    quit and halt system\n"
    ));
}

void cmd_cls(void) {
    for (uint8_t i = 0; i < 50; i++)
        putchar('\n');
}

void cmd_reboot(void) {
    printf_P(PSTR("Reboot in  "));
    for (char i = 3; i > 0; i--) {
        printf_P(PSTR("\b%d"), i);
        _delay_ms(1000);
    }
    printf_P(PSTR("\b%d\n"), 0);
    _delay_ms(100);
    uart_end();
    asm volatile("jmp 0"::);
}
