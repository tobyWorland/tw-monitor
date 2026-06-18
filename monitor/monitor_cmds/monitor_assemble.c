#include "monitor_assemble.h"

#include "../assert.h"
#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"
#include "../thumb_asm.h"

#include <stddef.h>

static struct thumb_instruction_spec instruction = {};

static void add_reg_rt(void) {
    thumb_add_operand_reg(&instruction, menu_preset_register("Rt? "));
}

static void add_reg_rd(void) {
    thumb_add_operand_reg(&instruction, menu_preset_register("Rd? "));
}

static void add_reg_rn(void) {
    thumb_add_operand_reg(&instruction, menu_preset_register("Rn? "));
}

static void add_reg_rm(void) {
    thumb_add_operand_reg(&instruction, menu_preset_register("Rm? "));
}

static void add_immediate(void) {
    thumb_add_operand_immediate(&instruction, gethexword(0));
}

static void add_signed_immediate(void) {
    // TODO: Actually accept a signed number
    thumb_add_operand_signed_immediate(&instruction, gethexword(0));
}

static void add_label(const char *prompt, void *addr, bool code) {
    thumb_add_operand_signed_immediate(&instruction,
                                       menu_preset_relative_label(prompt, addr, code));
}

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

static void assemble_a(thumb_t **paddr) {
    static const struct menu_option a_options[] = {
        {'i', "ADD{S} Immediate"},
        {'w', "ADD Immediate Wide (ADDW)"},
        {'r', "ADD{S} Register"},
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM A> ", ARR_LEN(a_options), a_options, NULL);
    switch (opt) {
    case 'i': // ADD{S} (Immediate)
    case 'w': { // ADDW
        if (opt == 'w') {
            instruction.mnemonic = TM_ADDW;
        } else { // 'i'
            instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_ADDS : TM_ADD;
        }

        add_reg_rd();
        add_reg_rn();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'r': { // ADD{S} (Register)
        instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_ADDS : TM_ADD;
        add_reg_rd();
        add_reg_rn();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_b(thumb_t **paddr) {
    static const struct menu_option b_options[] = {
        {'b', "B"},
        {'l', "BL"},
        {'L', "BLX"},
        {'x', "BX"},
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM B> ", ARR_LEN(b_options), b_options, NULL);
    switch (opt) {
    case 'b': {
        instruction.mnemonic = TM_B;
        thumb_set_condition(&instruction, menu_preset_instruction_set_condition_menu());
        add_label("Branch to? ", *paddr, true);
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'l': {
        instruction.mnemonic = TM_BL;
        add_label("Branch to? ", *paddr, true);
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'L': {
        instruction.mnemonic = TM_BLX;
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'x': {
        instruction.mnemonic = TM_BX;
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_m(thumb_t **paddr) {
    static const struct menu_option m_options[] = {
        {'i', "MOV{S} Immediate"},
        {'w', "MOV Immediate Wide (MOVW)"},
        {'r', "MOV{S} Register"},
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM M> ", ARR_LEN(m_options), m_options, NULL);
    switch (opt) {
    case 'i': // MOV{S} (Immediate)
    case 'w': { // MOVW
        if (opt == 'w') {
            instruction.mnemonic = TM_MOVW;
        } else { // 'i'
            instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_MOVS : TM_MOV;
        }

        add_reg_rd();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'r': { // MOV{S} (Register)
        instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_MOVS : TM_MOV;
        add_reg_rd();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_p(thumb_t **paddr) {
    static const struct menu_option p_options[] = {
        {'u', "PUSH"     },
        {'o', "POP"      },
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM P> ", ARR_LEN(p_options), p_options, NULL);
    switch (opt) {
    case 'u': { // PUSH
        instruction.mnemonic = TM_PUSH;
        thumb_add_operand_reglist(&instruction, menu_preset_register_list("To push? "));
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'o': { // POP
        instruction.mnemonic = TM_POP;
        thumb_add_operand_reglist(&instruction, menu_preset_register_list("To pop? "));
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_s(thumb_t **paddr) {
    static const struct menu_option s_options[] = {
        {'i', "SUB{S} Immediate"         },
        {'w', "SUB Immediate Wide (SUBW)"},
        {'r', "SUB{S} Register"          },
        {'v', "SVC"},
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM S> ", ARR_LEN(s_options), s_options, NULL);
    switch (opt) {
    case 'i': // SUB{S} (Immediate)
    case 'w': { // SUBW
        if (opt == 'w') {
            instruction.mnemonic = TM_SUBW;
        } else { // 'i'
            instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_SUBS : TM_SUB;
        }

        add_reg_rd();
        add_reg_rn();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'r': { // SUB{S} (Register)
        instruction.mnemonic = menu_preset_instruction_set_flags_menu() ? TM_SUBS : TM_SUB;
        add_reg_rd();
        add_reg_rn();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'v': { // SVC
        // TODO: Should have a way of getting a byte or arbitrary width number
        instruction.mnemonic = TM_SVC;
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    }
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void _ldr_register_base_immediate_offset() {
    // Get immediate offset

    // TODO: Make signed immediate (gethexword - use menu instead?)
    add_signed_immediate();

    // Set addressing mode
    static const struct menu_option addressing_mode_opts[] = {
        {' ', "Base Offset"         },
        {'!', "Pre-Indexing Offset" },
        {',', "Post-Indexing Offset"},
    };

    char addressing_opt = menu(
        "Addressing mode? ", ARR_LEN(addressing_mode_opts), addressing_mode_opts, NULL);
    switch (addressing_opt) {
    case ' ':
        // Already default
        break;
    case '!':
        thumb_set_operand_addressing_mode(&instruction, AM_PREINDEX);
        break;
    case ',':
        thumb_set_operand_addressing_mode(&instruction, AM_POSTINDEX);
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void _ldr_register_base_register_offset() {
    // Get register offset (Rm)
    add_reg_rm();

    // Get optional LSL shift
    static const struct menu_option lsl_shift_opts[] = {
        {' ', "No Shift"},
        {'l', "LSL"     },
    };

    char shift_opt = menu("Shift? ", ARR_LEN(lsl_shift_opts), lsl_shift_opts, "l");

    if (shift_opt == 'l') {
        thumb_add_operand_lslshift(&instruction, gethexword(0));
    }
}

static void assemble_ldr(thumb_t **paddr) {
    instruction.mnemonic = TM_LDR;

    // Default to regular offset
    thumb_set_operand_addressing_mode(&instruction, AM_OFFSET);

    add_reg_rt();

    static const struct menu_option ldr1_opts[] = {
        {'i', "PC Relative Immediate"},
        {'l', "PC Relative Label"    },
        {'r', "Register Base"        },
        // TODO: '=' Literal pool
    };

    char opt = menu("LDR> ", ARR_LEN(ldr1_opts), ldr1_opts, NULL);
    switch (opt) {
    case 'i': // PC Relative Immediate
        thumb_add_operand_reg(&instruction, 15); // PC
        add_signed_immediate();
        break;
    case 'l': // PC Relative Label
        thumb_add_operand_reg(&instruction, 15); // PC
        add_label("Label? ", *paddr, true);
        break;
    case 'r': { // Register Base
        add_reg_rn();

        static const struct menu_option ldr2_opts[] = {
            {'0', "Zero Offset"     },
            {'i', "Immediate Offset"},
            {'r', "Register Offset" },
        };

        char opt =
            menu("Offset? ", ARR_LEN(ldr2_opts), ldr2_opts, NULL);
        switch (opt) {
        case '0':
            thumb_add_operand_signed_immediate(&instruction, 0);
            break;
        case 'i':
            _ldr_register_base_immediate_offset();
            break;
        case 'r':
            _ldr_register_base_register_offset();
            break;
        default:
            menu_print_missing_action_message();
            break;
        }

        break;
    }
    default:
        menu_print_missing_action_message();
        break;
    }

    assemble_and_show_result(paddr, &instruction);
}

void monitor_assemble(thumb_t *addr) {
    static const struct menu_option assemble_options[] = {
        {'a', "A.."},
        {'b', "B..."},
        {'c', "CMP"},
        {'l', "LDR" },
        {'m', "M..."},
        {'n', "NOP" },
        {'p', "P..."},
        {'s', "S..." },
        {'u', "UDF"},
        {'x', "BKPT" },
        {'.', "Specify Width"},
        {CTRL('p'), "Print Assembly"},
        {CTRL('q'), "Quit"},
    };
    bool quit = false;
    thumb_t *starting_addr = addr;
    enum thumb_width_specifier width_specifier = TWS_AUTO;

    while (!quit) {
        // Reset instruction spec
        instruction = (struct thumb_instruction_spec){};
        instruction.width = width_specifier;

        // Display menu to user
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case 'a': { // A...
            assemble_a(&addr);
            break;
        }
        case 'b': { // B...
            assemble_b(&addr);
            break;
        }
        case 'c': { // CMP
            instruction.mnemonic = TM_CMP;
            add_reg_rn();

            static const struct menu_option cmp_opts[] = {
                {'i', "Immediate"},
                {'r', "Register" },
            };
            char opt = menu("I/R? ", ARR_LEN(cmp_opts), cmp_opts, NULL);
            switch (opt) {
            case 'i':
                add_immediate();
                break;
            case 'r':
                add_reg_rm();
                break;
            default:
                menu_print_missing_action_message();
                break;
            }

            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'l': { // LDR
            assemble_ldr(&addr);
            break;
        }
        case 'm': { // M...
            assemble_m(&addr);
            break;
        }
        case 'n': { // NOP
            instruction.mnemonic = TM_NOP;
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'p': { // P...
            assemble_p(&addr);
            break;
        }
        case 's': { // S...
            assemble_s(&addr);
            break;
        }
        case 'u': { // UDF
            // TODO: Should have a way of getting a byte or arbitrary width number
            instruction.mnemonic = TM_UDF;
            add_immediate();
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case 'x': { // BKPT
            // TODO: Should have a way of getting a byte or arbitrary width number
            instruction.mnemonic = TM_BKPT;
            add_immediate();
            assemble_and_show_result(&addr, &instruction);
            break;
        }
        case '.': {
            width_specifier = menu_preset_instruction_width_menu(width_specifier);
            break;
        }
        case CTRL('p'): {
            for (thumb_t *p = starting_addr; p < addr;
                 p = thumb_ins_ptr_increment(p)) {
                puthexword((uint32_t)p);
                putchar(' ');
                struct thumb_instruction_spec ins_spec = thumb_disassemble(p);
                thumb_print_instruction(&ins_spec, p);
                putnewline();
            }
            break;
        }
        case CTRL('q'):
            quit = true;
            break;
        default:
            menu_print_missing_action_message();
            break;
        }
    }
}
