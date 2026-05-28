#include "monitor_enter.h"

#include "../assert.h"
#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../vt.h"

#include <stdint.h>

#ifndef HOST
static void enter_load_address(const void *addr, void *dest, enum enter_type ent_type) {
    switch (ent_type) {
    case ET_BYTE:
        *(uint8_t*)dest = *(uint8_t*)addr;
        break;
    case ET_HWORD:
        *(uint16_t*)dest = *(uint16_t*)addr;
        break;
    case ET_WORD:
        *(uint32_t*)dest = *(uint32_t*)addr;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
};

static void enter_store_address(void *addr, const void *src, enum enter_type ent_type) {
    switch (ent_type) {
    case ET_BYTE:
        *(uint8_t*)addr = *(uint8_t*)src;
        break;
    case ET_HWORD:
        *(uint16_t*)addr = *(uint16_t*)src;
        break;
    case ET_WORD:
        *(uint32_t*)addr = *(uint32_t*)src;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
};

static unsigned enter_get_digits_from_type(enum enter_type ent_type) {
    unsigned width_in_bytes = (unsigned)ent_type;
    return width_in_bytes * 2;
}

enum enter_type enter_ent_type_submenu(void) {
    static const struct menu_option ent_options[] = {
        {'b', "Bytes"       },
        {'h', "Half Words"  },
        {'w', "Words"       },
        {'i', "Instructions"},
    };
    char opt = submenu("Type? ", ARR_LEN(ent_options), ent_options);
    switch (opt) {
    case 'b':
        return ET_BYTE;
    case 'h':
        return ET_HWORD;
    case 'w':
        return ET_WORD;
    case 'i':
        return ET_INSTRUCTION;
    default:
        ASSERT_NOT_REACHED();
    }
}

void monitor_enter(void *addr, enum enter_type ent_type) {
    uint32_t current;
    unsigned digit_idx = 0;
    unsigned col = 0;

    const unsigned digit_width = enter_get_digits_from_type(ent_type);
    const bool extra_space_in_number = digit_width > 4;

    assert(ent_type != ET_INSTRUCTION); // TODO: Not supported yet

    enter_load_address(addr, &current, ent_type);

    puthexword((uint32_t)addr);
    putchar(' ');

    while (1) {
        char c;
        uint8_t digit;

        puthexnumber(digit_width, current);

        if (digit_idx >= digit_width) {
            enter_store_address(addr, &current, ent_type);
            addr += (unsigned)ent_type;
            enter_load_address(addr, &current, ent_type);
            digit_idx = 0;

            if (++col == 16) {
                putnewline();
                puthexword((uint32_t)addr);
                col = 0;
            }

            putchar(' ');
            puthexnumber(digit_width, current);
        }

        c = getchar();

        if (c == '\r') {
            break;
        } else if (c == '\b') {
            if (digit_idx == 0) {
                // Go back a byte
                if (col > 0) {
                    col--;
                    addr -= (unsigned)ent_type;

                    // + 1 for the space between entries
                    vt_blank_last_n_chars(digit_width + 1 + !!extra_space_in_number);

                    enter_load_address(addr, &current, ent_type);
                    digit_idx = 0;
                } else {
                    // TODO: handle going back a row
                }
            } else {
                // Reload byte from memory
                enter_load_address(addr, &current, ent_type);
                digit_idx = 0;
            }
        } else if (c == ' ') { // Leave entry as is and enter the next one
            digit_idx = digit_width; // Trigger the next entry logic on the next iteration of the loop
        } else if ((digit = char_to_digit(c)) != 0xFF) {
            current <<= 4;
            current |= digit;
            digit_idx++;
        }

        vt_blank_last_n_chars(digit_width + !!extra_space_in_number);
    }

    enter_store_address(addr, &current, ent_type);
    putnewline();
}
#endif
