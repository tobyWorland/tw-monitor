#include "assert.h"
#include "arm.h"
#include "char.h"
#include "hardware.h"
#include "io.h"
#include "menu.h"
#include "thumb_asm.h"
#include "util.h"
#include "vt.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef HOST

void monitor_memdump(void *addr) {
    uint8_t *addr_as_byte_ptr = (void *)addr;

    for (int row = 0; row < 4; row++) {
        puthexword((uint32_t)addr_as_byte_ptr);
        for (int col = 0; col < 16; col++) {
            putchar(' ');
            putbyte(*addr_as_byte_ptr++);
        }
        putnewline();
    }
}

// TODO: Print mnemonics and operands
void monitor_disassemble(void *addr) {
    // Check thumb interwork bit isn't set
    if (arm_address_has_thumb_intwrk_bit(addr)) {
        enum menu_warning_fix_result opt =
            menu_preset_warning_fix("WARN Thumb bit set: ");
        if (opt == MENU_WF_FIX) {
            addr = arm_address_set_thumb_intwrk_bit(addr, false);
        } else if (opt == MENU_WF_QUIT) {
            return;
        } else {
            assert(opt == MENU_WF_PROCEED);
        }
    }

    uint16_t *addr_as_hword_ptr = (void *)addr;
    bool cont;

    do {
        for (int i = 0; i < 8; i++) { // Disassemble the next 8 instructions
            puthexword((uint32_t)addr_as_hword_ptr);
            putstring(": ");

            uint16_t first_hword = *addr_as_hword_ptr++;
            puthexhalfword(first_hword);

            // If wide instruction include the 2nd half word
            if (thumb_is_wide_instruction(first_hword)) {
                putchar(' ');
                puthexhalfword(*addr_as_hword_ptr++);
            }

            putnewline();
        }

        cont = menu_preset_continue("More? ");
    } while (cont);
}

void monitor_call_function(void (*fn)()) {
    // Check for thumb interwork bit in function address
    if (!arm_address_has_thumb_intwrk_bit(fn)) {
        enum menu_warning_fix_result opt =
            menu_preset_warning_fix("WARN No thumb bit: ");
        if (opt == MENU_WF_FIX) {
            fn = arm_address_set_thumb_intwrk_bit(fn, true);
        } else if (opt == MENU_WF_QUIT) {
            return;
        } else {
            assert(opt == MENU_WF_PROCEED);
        }
    }

    // Do call
    fn();
}

void hidden() { // TOOD: Remove
    putstring("Hidden function called!\r\n");
}

enum enter_type {
    ET_BYTE = 1,
    ET_HWORD = 2,
    ET_WORD = 4,
    ET_INSTRUCTION
};

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

static enum enter_type enter_ent_type_submenu() {
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

void monitor_main(bool surpress_init) {
    static const struct menu_option options[] = {
        {'d',       "Memory Dump"   },
        {'c',       "Call Address"  },
        {'e',       "Enter"         },
        {'u',       "Un/Disassemble"},
        {CTRL('l'), "Clear Screen"  },
    };
    static uint32_t addr = 0;

    if (!surpress_init) {
        vt_clearscreen();

        putstring("** Monitor ready **\r\n");
        putstring("hidden address: ");
        puthexword((uint32_t)hidden | 1); // | 1 for the interwork bit
        putnewline();
    }

    while (1) {
        char opt = menu("> ", ARR_LEN(options), options, "e");
        switch (opt) {
        case 'd':
            addr = gethexword(addr);
            monitor_memdump((void *)addr);
            break;
        case 'c':
            addr = gethexword(addr);
            monitor_call_function((void *)addr);
            break;
        case 'e': {
            enum enter_type ent_type = enter_ent_type_submenu();
            addr = gethexword(addr);
            monitor_enter((void *)addr, ent_type);
        }
            break;
        case 'u':
            addr = gethexword(addr);
            monitor_disassemble((void *)addr);
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
