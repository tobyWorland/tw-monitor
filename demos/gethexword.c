#include "demo_support.h"

#include "../monitor_src/io.h"

#include <stdint.h>

int main() {
    demo_make_term_raw();
    gethexword();
    putbyte(0xCA);
    putbyte(0xFE);
    return 0;
}
