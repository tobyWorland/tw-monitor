#include "demo_support.h"

#include <stdint.h>

void gethexword(); // TODO:
void putbyte(uint8_t b); // TODO:

int main() {
    demo_make_term_raw();
    gethexword();
    putbyte(0xCA);
    putbyte(0xFE);
    return 0;
}
