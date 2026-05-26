#include "vt.h"

#include "io.h"

void vt_clearline(void) {
    putstring("\e[2K");
}

void vt_blank_last_n_chars(unsigned n) {
    for (int i = 0; i < n; i++) putchar('\b');
    for (int i = 0; i < n; i++) putchar(' ');
    for (int i = 0; i < n; i++) putchar('\b');
}
