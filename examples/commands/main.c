#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <defines.h>
#include <uart.h>

extern void cmd_help();
extern void cmd_cls();
extern void cmd_reboot();

/* getting string from std input */
int get_string(char *str, int size) {
    char ch;
    int status = 0;
    int8_t i = 0;

    while (1) {
        status = scanf_P(PSTR("%c"), &ch);
        switch (ch) {
            case '\0':
                break;
            
            case '\n':
                putchar('\n');
                return status;
            
            case '\t':
                ch = ' ';
                str[i] = ch;
                i++;
                putchar(ch);
                break;
            
            case '\b':
            case '\x7F':
                if (i > 0) {
                    i--;
                    str[i] = '\0';
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                }
                break;
            
            default:
                if (i < (size - 1)) {
                    str[i] = ch;
                    i++;
                    putchar(ch);
                }
                break;
        }
    }
    return status;
}

void cmd() {
    char buf[21];   /* buffer for input */

    while (1) {
        memset(buf, 0, sizeof buf);
        putchar('>');

        if (get_string(buf, sizeof buf) == EOF) {
            printf_P(PSTR("EOF Error!\n"));
            continue;
        }

        if (buf[0] == 0)
            continue;
        else if (!strcmp_P(buf, PSTR("help"))) {
            cmd_help();
        }
        else if (!strcmp_P(buf, PSTR("cls"))) {
            cmd_cls();
        }
        else if (!strcmp_P(buf, PSTR("sp"))) {
            /* print Stack Pointer */
            printf_P(PSTR("SP: 0x%04X\n"), SP);
        }
        else if (!strcmp_P(buf, PSTR("reboot"))) {
            cmd_reboot();
        }
        else if (!strcmp_P(buf, PSTR("exit"))) {
            printf_P(PSTR("Exiting...\n"));
            break;
        }
        else
            printf_P(PSTR("Unknown command: %s\n"), buf);
    }
}

int main(void) {
    uart_init(9600);    /* init UART with baudrate 9600 */
    uart_set_stdio();   /* set uart as std io */

    printf_P(PSTR("\nHello there! Type \"help\" to get help =)\n"));
    cmd();  /* command line */
    printf_P(PSTR("Now you can disconnect.\n"));
    uart_end(); /* UART turn off */
}
