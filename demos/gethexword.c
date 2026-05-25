#include "demo_support.h"

#include <stdint.h>

void gethexword(); // TODO:
void print_byte(uint8_t b); // TODO:

int main() {
    demo_make_term_raw();
    gethexword();
    print_byte(0xCA);
    print_byte(0xFE);
    return 0;
}
