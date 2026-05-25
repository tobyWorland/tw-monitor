#include "hardware.h"

#include "vt.h"

#include <stdbool.h>
#include <stdint.h>

static bool isprint(char c) {
    return !((c <= ' ') || (c > '~'));
}

void assert(bool flag); // TODO: Put into header
#ifndef HOST
void assert(bool flag) {
    if (!flag) {
        if (g_hardware_inited) {
            putstring("ASSERT\r\n");
        }
        while (1) __asm("bkpt 99");
    }
}
#endif

char digit_to_char(uint8_t digit) {
    if (digit < 10) {
        return digit + '0';
    } else if (digit <= 0xF) {
        return digit - 10 + 'A';
    } else {
        return '\0';
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
        return 0xFF;
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

    assert(digit_to_char(0xff) == '\0');
    assert(char_to_digit('Z') == 0xFF);
}

void print_hexword(uint32_t address) {
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

uint32_t gethexword(void) {
    uint32_t result = 0xDEADBEEF;
    // FIXME:

    while (1) {
        vt_clearline(); // TODO: Don't clear the whole line just the part printed
        putchar('\r');
        print_hexword(result);
        if (getchar() == 'q') {
            break;
        }
    }
    putnewline();

    return result;
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
