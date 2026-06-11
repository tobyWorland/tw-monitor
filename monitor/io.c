#include "io.h"

#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "util.h"
#include "string.h"
#include "terminal.h"

#include <stdarg.h>

// TODO: Support using something (like another display) other than USART2
#ifndef HOST
void putchar(char c) {
    usart2_putchar(c);
}

char getchar(void) {
    return usart2_getchar();
}

void putstring(const char *string) {
    usart2_putstring(string);
}

// TODO: Should really swap this and putstring around, and have putstring call putnstring with max n
void putnstring(const char *string, unsigned n) {
    for (; *string && n; string++, n--) {
        usart2_putchar(*string);
    }
}

void putnewline(void) {
    usart2_putnewline();
}
#endif

void puthexnumber(unsigned digit_min, uint32_t number) {
    putstring(utoa_pad_w(number, 16, digit_min, true));
}

void puthexhalfword(uint16_t hword) {
    puthexnumber(4, hword);
}

void puthexword(uint32_t word) {
    puthexnumber(8, word);
}

void putbyte(uint8_t byte) {
    putchar(digit_to_char(byte >> 4));
    putchar(digit_to_char(byte & 0x0F));
}

uint32_t gethexword(uint32_t default_value) {
    uint32_t result = default_value;

    while (1) {
        puthexword(result);

        char c = getchar();

        if (c == '\b') {
            result >>= 4;
        } else if (c == '\r') {
            break;
        }

        uint8_t digit = char_to_digit(c);
        if (digit != 0xFF) {
            result <<= 4;
            result |= digit;
        }

        terminal_erase_last_chars(9);
    }
    putnewline();

    return result;
}

void io_printf(const char *fmt, ...) {
    va_list ap;
    bool in_format = false;

    va_start(ap, fmt);

    for (; *fmt; fmt++) {
        if (!in_format) {
            if (*fmt == '%') {
                in_format = true;
            } else {
                putchar(*fmt);
            }
        } else {
            switch (*fmt) {
            case '%': putchar('%'); break;
            case 's':
                putstring(va_arg(ap, char*));
                break;
            case 'c':
                putchar(va_arg(ap, int));
                break;
            case 'x':
            case 'X':
                puthexword(va_arg(ap, uint32_t));
                break;
            case 'd':
                putstring(itoa_pad(va_arg(ap, int), 10));
                break;
            case 'u':
                putstring(utoa_pad(va_arg(ap, int), 10));
                break;
            }
            in_format = false;
        }
    }

    va_end(ap);
}

const char *io_gettill(char delimiter, bool echo) {
    #define MAX_BUFFER_LEN 20
    static char buffer[MAX_BUFFER_LEN + 1]; // +1 for NULL terminator
    unsigned count = 0;

    while (1) {
        char c = getchar();
        if (c == '\b') {
            if (count > 0) {
                terminal_erase_last_chars(1);
                count--;
            }
        } else if (c == delimiter) {
            break;
        } else {
            if (count < MAX_BUFFER_LEN) {
                buffer[count++] = c;
                if (echo) {
                    putchar(c);
                }
            }
        }
    }

    buffer[count] = '\0';

    if (echo) {
        putnewline();
    }

    return buffer;
    #undef MAX_BUFFER_LEN
}

const char *io_getline(void) {
    return io_gettill('\r', true);
}
