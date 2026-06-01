#include "demo_support.h"

#include "../monitor/io.h"

#include <stdint.h>

int main() {
    demo_make_term_raw();
    gethexword(0);
    putbyte(0xCA);
    putbyte(0xFE);
    return 0;
}
