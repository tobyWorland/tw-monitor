
#include "demo_support.h"

#include "../monitor/io.h"

#include <stdint.h>

int main() {
    demo_make_term_raw();

    uint32_t number = 0xDEAD1337;

    for (unsigned i = 1; i <= 8; i++) {
        puthexnumber(i, number);
        putnewline();
    }

    return 0;
}
