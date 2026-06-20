#include "monitor_disassemble.h"

#include "../arm/arm.h"
#include "../assert.h"
#include "../io.h"
#include "../menu.h"
#include "../memory.h"

void monitor_disassemble_print_label_at(thumb_t *addr) {
    // Check if there is a label at this address
    struct memory_entry *label = memory_rlookup_label(addr);
    if (label) {
        // Found a label, print the name out
        memory_print_name_from_label(label);
        putstring(":\r\n");
    }
}

void monitor_disassemble_print_instruction_at(thumb_t *addr) {
    puthexword((uint32_t)addr);
    putstring(": ");

    puthexhalfword(addr->as_halfwords[0]);

    // If wide instruction, include the 2nd half word otherwise fill with spaces
    if (thumb_is_wide_instruction(*addr)) {
        putchar(' ');
        puthexhalfword(addr->as_halfwords[1]);
    } else {
        putstring("     ");
    }

    // TODO: Should have CMake option to include disassemble code
#if 1
    struct thumb_instruction_spec ins_spec = thumb_disassemble(addr);
    putchar(' ');
    thumb_print_instruction(&ins_spec, addr);
#endif

    putnewline();
}

#ifndef HOST
void monitor_disassemble(void *addr) {
    // Check thumb interwork bit isn't set
    if (arm_address_has_thumb_intwrk_bit(addr)) {
        switch (menu_preset_warning_fix("WARN Thumb bit set: ")) {
        case MENU_WF_FIX:
            addr = arm_address_set_thumb_intwrk_bit(addr, false);
        case MENU_WF_PROCEED:
            break;
        case MENU_WF_QUIT:
            return;
        default:
            assert(false);
        }
    }

    thumb_t *addr_as_thumb_ptr = addr;
    do {
        for (int i = 0; i < 8; i++) { // Disassemble the next 8 instructions
            monitor_disassemble_print_label_at(addr_as_thumb_ptr);
            monitor_disassemble_print_instruction_at(addr_as_thumb_ptr);
            addr_as_thumb_ptr = thumb_ins_ptr_increment(addr_as_thumb_ptr);
        }

    } while (menu_preset_continue("More? ", true));
}
#endif
