#include "hardware.h"

#include <stdbool.h>

static bool isprint(char c) {
    return !((c <= ' ') || (c > '~'));
}

void monitor_main() {
    char c;
    while (1) {
        c = getchar();

        if (isprint(c)) {
            putstring("CHAR: ");
            putchar(c);
            putnewline();
        }
    }
}
