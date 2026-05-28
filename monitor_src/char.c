#include "char.h"

bool char_isprint(char c) {
    return !((c < ' ') || (c > '~'));
}

char char_to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        c -= 'a' - 'A';
    }
    return c;
}

char digit_to_char(uint8_t digit) {
    if (digit < 10) {
        return digit + '0';
    } else if (digit <= 0xF) {
        return digit - 10 + 'A';
    } else {
        return '\0';
    }
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
