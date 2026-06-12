#include "monitor_assemble.h"

#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../thumb_asm.h"

#include <stddef.h>

static enum thumb_width_specifier width_specifier;

static void assemble_and_show_result(thumb_t **paddr, const struct thumb_instruction_spec *instruction) {
    enum thumb_assemble_result result = thumb_assemble(*paddr, instruction);
    switch (result) {
    case AR_NARROW_SUCCESS:
    case AR_WIDE_SUCCESS:
        *paddr = thumb_ins_ptr_increment(*paddr);
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

static void print_missing_action_message(void) {
    putstring("Error: Missing action\r\n");
}

static void assemble_b(thumb_t **paddr) {
    static const struct menu_option b_options[] = {
        {'b', "B"},
        {'x', "BX"},
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM B> ", ARR_LEN(b_options), b_options, NULL);
    switch (opt) {
    case 'b': {
        struct thumb_instruction_spec instruction = {};
        instruction.mnemonic = TM_B;
        instruction.width = width_specifier;
        thumb_add_operand_immediate(&instruction,
                                    menu_preset_relative_label("Branch to? ", *paddr, true));
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'x': {
        struct thumb_instruction_spec instruction = {};
        instruction.mnemonic = TM_BX;
        instruction.width = width_specifier;
        thumb_add_operand_reg(&instruction, menu_preset_register("Rm? "));
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        print_missing_action_message();
        break;
    }
}

void monitor_assemble(thumb_t *addr) {
    static const struct menu_option assemble_options[] = {
        {'b', "B..." },
        {'m', "MOVW" },
        {'n', "NOP"  },
        {'s', "SVC"  },
        {'x', "BKPT" },
        {'.', "Specify Width"},
        {CTRL('p'), "Print Assembly"},
        {CTRL('q'), "Quit"},
    };
    bool quit = false;
    thumb_t *starting_addr = addr;
    width_specifier = TWS_AUTO;

    while (!quit) {
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case 'b': { // B...
            assemble_b(&addr);
            break;
        }
        case 'm': { // MOVW
            struct thumb_instruction_spec instruction = {};
            instruction.mnemonic = TM_MOVW;
            instruction.width = width_specifier;
            thumb_add_operand_reg(&instruction, menu_preset_register("Rd? "));
            thumb_add_operand_immediate(&instruction, gethexword(0));
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'n': { // NOP
            struct thumb_instruction_spec instruction = {};
            instruction.mnemonic = TM_NOP;
            instruction.width = width_specifier;
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 's': { // SVC
            // TODO: Should have a way of getting a byte or arbitrary width number
            uint32_t immediate = gethexword(0);
            struct thumb_instruction_spec instruction = {};
            instruction.mnemonic = TM_SVC;
            instruction.width = width_specifier;
            thumb_add_operand_immediate(&instruction, immediate);
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'x': { // BKPT
            // TODO: Should have a way of getting a byte or arbitrary width number
            uint32_t immediate = gethexword(0);
            struct thumb_instruction_spec instruction = {};
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
                 p = thumb_ins_ptr_increment(p)) {
                puthexword((uint32_t)p);
                putchar(' ');
                struct thumb_instruction_spec ins_spec = thumb_disassemble(p);
                thumb_print_instruction(&ins_spec);
                putnewline();
            }
            break;
        }
        case CTRL('q'):
            quit = true;
            break;
        default:
            print_missing_action_message();
            break;
        }
    }
}
