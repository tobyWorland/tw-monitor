#include "hardware.h"

#include <stdbool.h>
#include <stdint.h>

static bool isprint(char c) {
    return !((c <= ' ') || (c > '~'));
}

void assert(bool flag) {
    if (!flag) {
        if (g_hardware_inited) {
            putstring("ASSERT\r\n");
        }
        while (1) __asm("bkpt 99");
    }
}

char digit_to_char(uint8_t digit) {
    assert(digit <= 0xF);
    if (digit < 10) {
        return digit + '0';
    } else {
        return digit - 10 + 'A';
    }
}

char char_to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        c -= 'a' - 'A';
    }
    return c;
}

uint8_t char_to_digit(char c) {
    c = char_to_upper(c);
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        assert(false);
        __builtin_unreachable();
    }
}

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
}

void monitor_main() {
    char c;

    test();

    while (1) {
        c = getchar();

        if (isprint(c)) {
            putstring("CHAR: ");
            putchar(c);
            putnewline();
        }
    }
}
