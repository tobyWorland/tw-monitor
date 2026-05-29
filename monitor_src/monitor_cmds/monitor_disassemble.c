#include "monitor_disassemble.h"

#include "../arm.h"
#include "../assert.h"
#include "../io.h"
#include "../menu.h"
#include "../thumb_asm.h"

#ifndef HOST
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

#if 1
            const uint16_t *insptr = addr_as_hword_ptr;
#endif

            uint16_t first_hword = *addr_as_hword_ptr++;
            puthexhalfword(first_hword);

            // If wide instruction include the 2nd half word
            if (thumb_is_wide_instruction(first_hword)) {
                putchar(' ');
                puthexhalfword(*addr_as_hword_ptr++);
            } else {
                putstring("     ");
            }

            // TODO: Should have CMake option to include disassemble code
#if 1
            thumb_print_disassembled_instruction(insptr);
#endif

            putnewline();
        }

        cont = menu_preset_continue("More? ", true);
    } while (cont);
}
#endif
