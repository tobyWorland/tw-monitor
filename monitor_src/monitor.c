#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "io.h"

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
