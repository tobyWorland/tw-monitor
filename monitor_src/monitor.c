#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "vt.h"

#include <stdbool.h>
#include <stdint.h>

void test() {
    for (int i = 0; i <= 0xF; i++) {
        putstring("Test: I => ");
        putchar(digit_to_char(i));
        putnewline();
        assert(char_to_digit(digit_to_char(i)) == i);
    }

    putstring("lowercase test\r\n");
    for (char c = 'a'; c <= 'f'; c++) {
        putstring("Test: ");
        putchar(c);
        putstring(" => ");
        putchar(digit_to_char(char_to_digit(c)));
        putnewline();
    }

    assert(digit_to_char(0xff) == '\0');
    assert(char_to_digit('Z') == 0xFF);
}

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

#ifndef HOST
void monitor_main() {
    char c;

    test();

#if 0
    while (1) {
        c = getchar();

        if (isprint(c)) {
            putstring("CHAR: ");
            putchar(c);
            putnewline();
        }
    }
#endif

    while (1) {
        uint32_t addr = gethexword();
        puthexword(addr);

        uint8_t *addr_as_byte_ptr = (void*)addr;
        for (int i = 0; i < 16; i++) {
            putchar(' ');
            putbyte(*addr_as_byte_ptr++);
        }
        putnewline();
    }
}
#endif
