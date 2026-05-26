#include "assert.h"
#include "char.h"
#include "hardware.h"
#include "io.h"
#include "menu.h"
#include "vt.h"

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

void monitor_enter(void *addr) {
    uint8_t *addr_as_byte_ptr = (void*)addr;
    uint8_t current_byte = *addr_as_byte_ptr;
    unsigned digit_idx = 0;
    unsigned col = 0;

    puthexword((uint32_t)addr_as_byte_ptr);
    putchar(' ');

    while (1) {
        char c;
        uint8_t digit;

        putbyte(current_byte);

        if (digit_idx >= 2) {
            *addr_as_byte_ptr = current_byte;
            current_byte = *++addr_as_byte_ptr;
            digit_idx = 0;

            if (++col == 16) {
                putnewline();
                puthexword((uint32_t)addr_as_byte_ptr);
                col = 0;
            }

            putchar(' ');
            putbyte(current_byte);
        }

        c = getchar();

        if (c == '\r') {
            break;
        } else if (c == '\b') {
            if (digit_idx == 0) {
                // Go back a byte
                if (col > 0) {
                    col--;
                    addr_as_byte_ptr--;
                    vt_blank_last_n_chars(3);

                    current_byte = *addr_as_byte_ptr;
                    digit_idx = 0;
                } else {
                    // TODO: handle going back a row
                }
            } else {
                // Reload byte from memory
                current_byte = *addr_as_byte_ptr;
                digit_idx = 0;
            }
        } else if (c == ' ') { // Leave byte as is and enter the next one
            digit_idx = 2;
        } else if ((digit = char_to_digit(c)) != 0xFF) {
            current_byte <<= 4;
            current_byte |= digit;
            digit_idx++;
        }

        vt_blank_last_n_chars(2);
    }

    *addr_as_byte_ptr = current_byte;
    putnewline();
}

void monitor_main(bool surpress_init) {
    const struct menu_option options[] = {
        { 'd',       "Memory Dump"  },
        { 'c',       "Call Address" },
        { 'e',       "Enter Bytes"  },
        { CTRL('l'), "Clear Screen" },
    };
    static uint32_t addr = 0;

    if (!surpress_init) {
        vt_clearscreen();

        test();

        putstring("** Monitor ready **\r\n");
        putstring("hidden address: ");
        puthexword((uint32_t)hidden | 1); // | 1 for the interwork bit
        putnewline();
    }

    while (1) {
        char opt = menu("> ", sizeof(options)/sizeof(options[0]), options);
        switch (opt) {
        case 'd':
            addr = gethexword(addr);
            monitor_memdump((void*)addr);
            break;
        case 'c':
            addr = gethexword(addr);
            monitor_call_function((void*)addr);
            break;
        case 'e':
            addr = gethexword(addr);
            monitor_enter((void*)addr);
            break;
        case CTRL('l'):
            vt_clearscreen();
            break;
        default:
            putstring("Error: Missing action\r\n");
            break;
        }
    }
}
#endif
