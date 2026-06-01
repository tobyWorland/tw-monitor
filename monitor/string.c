#include "string.h"

#include "assert.h"
#include "char.h"
#include "util.h"

#define MIN_BASE 2
#define MAX_BASE 16
#define PAD_SIZE 32 + 1 // space for a 32bit unsigned number + null term

static char pad[PAD_SIZE];
static char *pad_ptr;

#ifdef HOST
#define PAD_STATIC
#else
#define PAD_STATIC static
#endif

PAD_STATIC void pad_init() {
    pad_ptr = pad + PAD_SIZE;
    *--pad_ptr = '\0';
}

PAD_STATIC void pad_hold(char c) {
    assert(pad_ptr > pad);
    *--pad_ptr = c;
}

PAD_STATIC unsigned pad_len(void) {
    return &pad[PAD_SIZE] - pad_ptr - 1;
}

const char *utoa_pad_w(unsigned number, unsigned base, unsigned minwidth,
                       bool fillwithzero) {
    assert((base >= MIN_BASE) && (base <= MAX_BASE));
    char fillchar = fillwithzero ? '0' : ' ';

    pad_init();

    if (number == 0) {
        pad_hold('0');
    } else {
        unsigned i = 0;
        while (number > 0) {
            if (!(++i % 5)) {
                pad_hold('_');
            }

            const uint8_t digit = number % base;
            pad_hold(digit_to_char(digit));
            number /= base;
        }
    }

    if (fillwithzero) {
        while (pad_len() < minwidth) {
            if (!(pad_len() % 4)) {
                pad_hold('_');
            }
            pad_hold(fillchar);
        }
    } else {
        while (pad_len() < minwidth) {
            pad_hold(fillchar);
        }
    }

    return pad_ptr;
}

const char *itoa_pad_w(int number, unsigned base, unsigned minwidth,
                     bool fillwidthzero) {
    bool neg = number < 0;
    unsigned abs = neg ? -number : number;

    if (minwidth > 0) {
        minwidth -= 1 * neg;
    }

    utoa_pad_w(abs, base, minwidth, fillwidthzero);

    if (neg) {
        pad_hold('-');
    }

    return pad_ptr;
}

const char *utoa_pad(unsigned number, unsigned base) {
    return utoa_pad_w(number, base, 0, false);
}

const char *itoa_pad(int number, unsigned base) {
    return itoa_pad_w(number, base, 0, false);
}

// Needed by GCC
#ifndef HOST
void *memset(void *s, char c, size_t n) {
    char *ptr = s;
    while (n--) {
        *ptr++ = c;
    }
    return s;
}
#endif
