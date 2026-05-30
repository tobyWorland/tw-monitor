#include "string.h"

#include "assert.h"
#include "char.h"

#define MIN_BASE 2
#define MAX_BASE 16
#define PAD_SIZE 32 + 1 // space for a 32bit unsigned number + null term

char pad[PAD_SIZE];
char *pad_ptr;

static void pad_init() {
    pad_ptr = pad + PAD_SIZE;
    *--pad_ptr = '\0';
}

static void pad_hold(char c) {
    assert(pad_ptr > pad);
    *--pad_ptr = c;
}

const char *utoa_pad(unsigned number, unsigned base) {
    assert((base >= MIN_BASE) && (base <= MAX_BASE));

    pad_init();

    if (number == 0) {
        pad_hold('0');
        return pad_ptr;
    }

    while (number > 0) {
        const uint8_t digit = number % base;
        pad_hold(digit_to_char(digit));
        number /= base;
    }

    return pad_ptr;
}

const char *itoa_pad(int number, unsigned base) {
    bool neg = number < 0;
    unsigned abs = neg ? -number : number;

    utoa_pad(abs, base);

    if (neg) {
        pad_hold('-');
    }

    return pad_ptr;
}
