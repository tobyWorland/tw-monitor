#include "monitor_assemble.h"

#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../thumb_asm.h"

#include <stddef.h>

static void assemble_and_show_result(thumb_t **paddr, const struct thumb_instruction *instruction) {
    enum thumb_assemble_result result = thumb_assemble(*paddr, instruction);
    switch (result) {
    case AR_NARROW_SUCCESS:
        *paddr += 1; // + 16bit
        break;
    case AR_WIDE_SUCCESS:
        *paddr += 2; // + 32bit
        break;
    default:
        putstring("Failed to assemble ");
        putbyte(result);
        putnewline();
        break;
    }
}

static enum thumb_width_specifier show_width_menu(enum thumb_width_specifier current) {
    // NOTE: Make sure these line up with the order in the enum as I use it to print them
    static const struct menu_option width_menu[] = {
        {'a', "AUTO"  },
        {'n', "NARROW"},
        {'w', "WIDE"  },
    };

    putstring("Current: ");
    putstring(width_menu[current].name);
    putnewline();

    char opt = menu("New width? ", ARR_LEN(width_menu), width_menu, NULL);
    // TODO: feels like there would be a way to reuse the struct instead of a switch
    switch (opt) {
    case 'n': return TWS_NARROW;
    case 'w': return TWS_WIDE;
    default: return TWS_AUTO;
    }
}

void monitor_assemble(thumb_t *addr) {
    static const struct menu_option assemble_options[] = {
        {'b', "BX"   },
        {'n', "NOP"  },
        {'x', "BKPT" },
        {'.', "Specify Width"},
        {CTRL('p'), "Print Assembly"},
        {CTRL('q'), "Quit"},
    };
    bool quit = false;
    thumb_t *starting_addr = addr;
    enum thumb_width_specifier width_specifier = TWS_AUTO;

    while (!quit) {
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case 'b': { // BX
            struct thumb_instruction instruction = {};
            instruction.mnemonic = TM_BX;
            instruction.width = width_specifier;
            thumb_add_operand_reg(&instruction, menu_preset_register("Rm? "));
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'n': { // NOP
            struct thumb_instruction instruction = {};
            instruction.mnemonic = TM_NOP;
            instruction.width = width_specifier;
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'x': { // BKPT
            // TODO: Should have a way of getting a byte or arbitrary width number
            uint32_t immediate = gethexword(0);
            struct thumb_instruction instruction = {};
            instruction.mnemonic = TM_BKPT;
            instruction.width = width_specifier;
            thumb_add_operand_immediate(&instruction, immediate);
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case '.': {
            width_specifier = show_width_menu(width_specifier);
            break;
        }
        case CTRL('p'): {
            for (thumb_t *p = starting_addr; p < addr; // TW: Make addr thumb_t?
                 p += thumb_is_wide_instruction(*p) ? 2 : 1) {
                puthexword((uint32_t)p);
                putchar(' ');
                thumb_print_disassembled_instruction(p);
                putnewline();
            }
            break;
        }
        case CTRL('q'):
            quit = true;
            break;
        default:
            putstring("Error: Missing action\r\n");
            break;
        }
    }
}
