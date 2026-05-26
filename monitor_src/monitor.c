#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "io.h"

#include <stdbool.h>
#include <stdint.h>

void test() {
    for (int i = 0; i <= 0xF; i++) {
        putstring("Test: I => ");
        putchar(digit_to_char(i));
        putnewline();
        assert(char_to_digit(digit_to_char(i)) == i);
    }

    putstring("lowercase test\r\n");
    for (char c = 'a'; c <= 'f'; c++) {
        putstring("Test: ");
        putchar(c);
        putstring(" => ");
        putchar(digit_to_char(char_to_digit(c)));
        putnewline();
    }

    assert(digit_to_char(0xff) == '\0');
    assert(char_to_digit('Z') == 0xFF);
}

#ifndef HOST

void monitor_memdump(void *addr) {
    puthexword((uint32_t)addr);

    uint8_t *addr_as_byte_ptr = (void*)addr;
    for (int i = 0; i < 16; i++) {
        putchar(' ');
        putbyte(*addr_as_byte_ptr++);
    }
    putnewline();
}

void monitor_call_function(void (*fn)()) {
    fn();
}

void hidden() { // TOOD: Remove
    putstring("Hidden function called!\r\n");
}

void monitor_main() {
    char c;

    test();

#if 0
    while (1) {
        c = getchar();

        if (isprint(c)) {
            putstring("CHAR: ");
            putchar(c);
            putnewline();
        }
    }
#endif

    while (1) {
#if 0
        uint32_t addr = gethexword();
        monitor_memdump((void*)addr);
#else
        volatile uint32_t addr;
        volatile char opt;

        // set addr=&sram_start
        // set addr=hidden then set addr|=1
        // For dump: set opt='d'
        // For call: set opt='c'
        __asm("bkpt 1");

        switch (opt) {
        case 'd':
            monitor_memdump((void*)addr);
            break;
        case 'c':
            monitor_call_function((void*)addr);
            break;
        default: __asm("bkpt 9"); // Bad option
        }
#endif
    }
}
#endif
