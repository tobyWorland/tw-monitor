#include "io.h"

#include "char.h"
#include "hardware.h"
#include "vt.h"

void puthexword(uint32_t address) {
    char pad[10];
    char *ptr = pad + sizeof(pad);
    *--ptr = '\0';

    for (int i = 0; i < 4; i++) {
        *--ptr = digit_to_char(address & 0xF);
        address >>= 4;
    }

    *--ptr = ' ';

    for (int i = 0; i < 4; i++) {
        *--ptr = digit_to_char(address & 0xF);
        address >>= 4;
    }

    putstring(ptr);
}

void putbyte(uint8_t byte) {
    putchar(digit_to_char(byte >> 4));
    putchar(digit_to_char(byte & 0x0F));
}

uint32_t gethexword(void) {
    uint32_t result = 0;

    while (1) {
        vt_clearline(); // TODO: Don't clear the whole line just the part printed
        putchar('\r');
        puthexword(result);

        char c = getchar();

        if (c == 127) { // backspace will send delete which is 127
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
