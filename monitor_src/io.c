#include "io.h"

#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "util.h"
#include "vt.h"

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

void putnewline(void) {
    usart2_putnewline();
}
#endif

void puthexnumber(unsigned digit_min, uint32_t number) {
    char pad[10];
    char *ptr = pad + sizeof(pad);
    *--ptr = '\0';

    assert((digit_min <= 8) && (digit_min > 0)); // Support 32bit maximum

    for (int i = 0; i < MIN(digit_min, 4); i++) {
        *--ptr = digit_to_char(number & 0xF);
        number >>= 4;
    }

    if (digit_min > 4) {
        digit_min -= 4;
        *--ptr = ' ';

        for (int i = 0; i < MIN(digit_min, 4); i++) {
            *--ptr = digit_to_char(number & 0xF);
            number >>= 4;
        }
    }

    putstring(ptr);
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
        vt_clearline(); // TODO: Don't clear the whole line just the part
                        // printed
        putchar('\r');
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
    }
    putnewline();

    return result;
}
