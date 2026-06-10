#include "string.h"

#include "assert.h"
#include "char.h"
#include "util.h"

#include <stdint.h>

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
        while (pad_len() < minwidth + (minwidth / 5)) {
            if (!((pad_len() + 1) % 5)) {
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

unsigned strtou(const char *s, char **endptr, unsigned base) {
    unsigned result = 0;

    assert((base >= MIN_BASE) && (base <= MAX_BASE));

    if (endptr == NULL) {
        endptr = (char**)&s;
    }

    for (; *s; s++) {
        uint8_t digit = char_to_digit(*s);
        if (digit == 0xFF || digit >= base) {
            break;
        }

        result *= base;
        result += digit;
    }

    *endptr = (char *)s;
    return result;
}

int strtoi(const char *s, char **endptr, unsigned base) {
    bool neg = false;
    int result;

    if (*s == '-') {
        neg = true;
        s++;
    }

    result = strtou(s, endptr, base);
    if (neg) {
        result *= -1;
    }

    return result;
}

#ifndef HOST
// Needed by GCC
void *memset(void *s, char c, size_t n) {
    char *ptr = s;
    while (n--) {
        *ptr++ = c;
    }
    return s;
}

void *memcpy(void *__restrict__ dest, const void *__restrict__ src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = s1;
    const uint8_t *p2 = s2;
    while (n--) {
        unsigned diff = *p2++ - *p1++;
        if (diff != 0) {
            return diff;
        }
    }
    return 0;
}
#endif
