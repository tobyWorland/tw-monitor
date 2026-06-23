#include "monitor_assemble.h"

#include "monitor_call.h"
#include "monitor_disassemble.h"
#include "../arm/arm.h"
#include "../assert.h"
#include "../char.h"
#include "../io.h"
#include "../memory.h"
#include "../menu.h"
#include "../util.h"
#include "../string.h"
#include "../thumb_asm.h"

#include <stddef.h>

static struct thumb_instruction_spec instruction = {};

static thumb_t *previous_thumb_instruction(thumb_t *start, thumb_t *current) {
    thumb_t *previous = start;

    assert(start <= current);

    // Find pointer of last instruction and then set the current address to it
    while (1) {
        thumb_t *next = thumb_ins_ptr_increment(previous);
        if (next == current) {
            break;
        } else {
            previous = next;
        }
    }

    return previous;
}

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
    // We won't ever need an unsigned 32bit, so a positive signed 32bit typecasted is perfectly fine
    int imm;
    do {
        // TODO: Should have an arg on menu_number to only allow positive
        imm = menu_number("Unsigned Immediate? ", 0, 0, NULL, NULL);
    } while (imm < 0);
    thumb_add_operand_immediate(&instruction, imm);
}

static void add_signed_immediate(void) {
    int simm = menu_number("Signed Immediate? ", 0, 0, NULL, NULL);
    thumb_add_operand_signed_immediate(&instruction, simm);
}

static void add_label(const char *prompt, void *addr, bool code) {
    thumb_add_operand_signed_immediate(&instruction,
                                       menu_preset_relative_label(prompt, addr, code));
}

// WARNING: Assumes already set to AM_OFFSET
static void set_addressing_mode() {
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

static void add_optional_lsl_shift(void) {
    static const struct menu_option lsl_shift_opts[] = {
        {' ', "No Shift"},
        {'l', "LSL"     },
    };

    char shift_opt = menu("Shift? ", ARR_LEN(lsl_shift_opts), lsl_shift_opts, "l");

    if (shift_opt == 'l') {
        thumb_add_operand_lslshift(&instruction, gethexword(0));
    }
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
        {'i', "ADD  Immediate"            },
        {'I', "ADDS Immediate"            },
        {'w', "ADD  Immediate Wide (ADDW)"},
        {'r', "ADD  Register"             },
        {'R', "ADDS Register"             },
        {'q', "Quit Menu"                 },
    };

    char opt = menu("ASM A> ", ARR_LEN(a_options), a_options, NULL);
    switch (opt) {
    case 'i': // ADD  (Immediate)
    case 'I': // ADDS (Immediate)
    case 'w': // ADDW
        if (opt == 'w') {
            instruction.mnemonic = TM_ADDW;
        } else { // 'i' or 'I'
            instruction.mnemonic = opt == 'I' ? TM_ADDS : TM_ADD;
        }

        add_reg_rd();
        add_reg_rn();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'r': // ADD  (Register)
    case 'R': // ADDS (Register)
        instruction.mnemonic = opt == 'R' ? TM_ADDS : TM_ADD;
        add_reg_rd();
        add_reg_rn();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_b(thumb_t **paddr) {
    static const struct menu_option b_options[] = {
        {'b', "B"        },
        {'l', "BL"       },
        {'L', "BLX"      },
        {'x', "BX"       },
        {'q', "Quit Menu"},
    };

    char opt = menu("ASM B> ", ARR_LEN(b_options), b_options, NULL);
    switch (opt) {
    case 'b':
        instruction.mnemonic = TM_B;
        thumb_set_condition(&instruction, menu_preset_instruction_set_condition_menu());
        add_label("Branch to? ", *paddr, true);
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'l':
        instruction.mnemonic = TM_BL;
        add_label("Branch to? ", *paddr, true);
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'L':
        instruction.mnemonic = TM_BLX;
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'x':
        instruction.mnemonic = TM_BX;
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_m(thumb_t **paddr) {
    static const struct menu_option m_options[] = {
        {'i', "MOV  Immediate"            },
        {'I', "MOVS Immediate"            },
        {'w', "MOV  Immediate Wide (MOVW)"},
        {'r', "MOV  Register"             },
        {'R', "MOVS Register"             },
        {'q', "Quit Menu"                 },
    };

    char opt = menu("ASM M> ", ARR_LEN(m_options), m_options, NULL);
    switch (opt) {
    case 'i': // MOV  (Immediate)
    case 'I': // MOVS (Immediate)
    case 'w': // MOVW
        if (opt == 'w') {
            instruction.mnemonic = TM_MOVW;
        } else { // 'i' or 'I'
            instruction.mnemonic = opt == 'I' ? TM_MOVS : TM_MOV;
        }

        add_reg_rd();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'r': // MOV  (Register)
    case 'R': // MOVS (Register)
        instruction.mnemonic = opt == 'R' ? TM_MOVS : TM_MOV;
        add_reg_rd();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
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
    case 'u': // PUSH
        instruction.mnemonic = TM_PUSH;
        thumb_add_operand_reglist(&instruction, menu_preset_register_list("To push? "));
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'o': // POP
        instruction.mnemonic = TM_POP;
        thumb_add_operand_reglist(&instruction, menu_preset_register_list("To pop? "));
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_s(thumb_t **paddr) {
    static const struct menu_option s_options[] = {
        {'i', "SUB  Immediate"            },
        {'I', "SUBS Immediate"            },
        {'w', "SUB  Immediate Wide (SUBW)"},
        {'r', "SUB  Register"             },
        {'R', "SUBS Register"             },
        {'v', "SVC"                       },
        {'q', "Quit Menu"                 },
    };

    char opt = menu("ASM S> ", ARR_LEN(s_options), s_options, NULL);
    switch (opt) {
    case 'i': // SUB  (Immediate)
    case 'I': // SUBS (Immediate)
    case 'w': // SUBW
        if (opt == 'w') {
            instruction.mnemonic = TM_SUBW;
        } else { // 'i' or 'I'
            instruction.mnemonic = opt == 'I' ? TM_SUBS : TM_SUB;
        }

        add_reg_rd();
        add_reg_rn();
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'r': // SUB  (Register)
    case 'R': // SUBS (Register)
        instruction.mnemonic = opt == 'R' ? TM_SUBS : TM_SUB;
        add_reg_rd();
        add_reg_rn();
        add_reg_rm();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'v': // SVC
        // TODO: Should have a way of getting a byte or arbitrary width number
        instruction.mnemonic = TM_SVC;
        add_immediate();
        assemble_and_show_result(paddr, &instruction);
        break;
    case 'q':
        break;
    default:
        menu_print_missing_action_message();
        break;
    }
}

static void assemble_ldr(thumb_t **paddr) {
    instruction.mnemonic = TM_LDR;

    // Default to regular offset
    thumb_set_operand_addressing_mode(&instruction, AM_OFFSET);

    add_reg_rt();

    static const struct menu_option ldr_base_opts[] = {
        {'i', "PC Relative Immediate"},
        {'l', "PC Relative Label"    },
        {'r', "Register Base"        },
        // TODO: '=' Literal pool
    };

    char opt = menu("LDR Base? ", ARR_LEN(ldr_base_opts), ldr_base_opts, NULL);
    switch (opt) {
    case 'i': // PC Relative Immediate
        thumb_add_operand_reg(&instruction, 15); // PC
        add_signed_immediate();
        break;
    case 'l': // PC Relative Label
        thumb_add_operand_reg(&instruction, 15); // PC
        add_label("Label? ", *paddr, true);
        break;
    case 'r': // Register Base
        add_reg_rn();

        static const struct menu_option ldr_offset_opts[] = {
            {'0', "Zero Offset"     },
            {'i', "Immediate Offset"},
            {'r', "Register Offset" },
        };

        char opt = menu("Offset? ", ARR_LEN(ldr_offset_opts), ldr_offset_opts, NULL);
        switch (opt) {
        case '0':
            thumb_add_operand_signed_immediate(&instruction, 0);
            break;
        case 'i':
            add_signed_immediate();
            set_addressing_mode();
            break;
        case 'r':
            add_reg_rm();
            add_optional_lsl_shift();
            break;
        default:
            menu_print_missing_action_message();
            break;
        }

        break;
    default:
        menu_print_missing_action_message();
        break;
    }

    assemble_and_show_result(paddr, &instruction);
}

void define_new_label(thumb_t **paddr) {
    static const struct menu_option define_label_options[] = {
        {'c', "New Code Label"},
        {'d', "New Data Label"},
        {'q', "Quit"          },
    };
    const char *name = NULL;
    bool created = false;

    while (!created) {
        char opt = menu("Label? ",
                        ARR_LEN(define_label_options),
                        define_label_options,
                        NULL);
        switch (opt) {
        case 'c':
        case 'd': {
            putstring("New name? ");
            name = io_getline();
            created = memory_create_label(name, strlen(name), *paddr, opt == 'c');
            if (!created) {
                putstring("Error: Failed to define new label.\r\n");
            }
            break;
        }
        case 'q':
            return;
        }
    }
}

void monitor_assemble(thumb_t *addr, struct memory_entry *memory_section) {
    static const struct menu_option assemble_options[] = {
        {'a',       "A.."               },
        {'b',       "B..."              },
        {'c',       "CMP"               },
        {'l',       "LDR"               },
        {'m',       "M..."              },
        {'n',       "NOP"               },
        {'p',       "P..."              },
        {'s',       "S..."              },
        {'u',       "UDF"               },
        {'x',       "BKPT"              },
        {':',       "Define Label"      },
        {'.',       "Specify Width"     },
        {CTRL('c'), "Call Assembly"     },
        {CTRL('p'), "Print Assembly"    },
        {'0',       "Up to start"       },
        {'$',       "Down to end"       },
        {CTRL('u'), "Up"                },
        {CTRL('d'), "Down"              },
        {CTRL('r'), "Rewind Instruction"},
        {CTRL('q'), "Quit"              },
    };
    bool quit = false;
    thumb_t *starting_addr = addr;
    thumb_t *end_addr = addr;
    enum thumb_width_specifier width_specifier = TWS_AUTO;

    // If we have a section use it to set the starting_addr and end_addr
    if (memory_section) {
        void *section_address = memory_get_section_address(memory_section);
        starting_addr = section_address;
        end_addr = section_address + memory_get_section_size(memory_section);

        // Clamp address to section
        if (addr < starting_addr) {
            addr = starting_addr;
        } else if (addr > end_addr) {
            addr = end_addr;
        }
    }

    while (!quit) {
        // Check if addr has moved past end_addr
        if (addr > end_addr) {
            end_addr = addr;

            // If we have a section bump the size
            if (memory_section) {
                memory_update_section_size(memory_section, (void*)end_addr - (void*)starting_addr);
            }
        }

        // Reset instruction spec
        instruction = (struct thumb_instruction_spec){};
        instruction.width = width_specifier;

        // Display menu to user
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case 'a': // A...
            assemble_a(&addr);
            break;
        case 'b': // B...
            assemble_b(&addr);
            break;
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
        case 'l': // LDR
            assemble_ldr(&addr);
            break;
        case 'm': // M...
            assemble_m(&addr);
            break;
        case 'n': // NOP
            instruction.mnemonic = TM_NOP;
            assemble_and_show_result(&addr, &instruction);
            break;
        case 'p': // P...
            assemble_p(&addr);
            break;
        case 's': // S...
            assemble_s(&addr);
            break;
        case 'u': // UDF
            // TODO: Should have a way of getting a byte or arbitrary width number
            instruction.mnemonic = TM_UDF;
            add_immediate();
            assemble_and_show_result(&addr, &instruction);
            break;
        case 'x': // BKPT
            // TODO: Should have a way of getting a byte or arbitrary width number
            instruction.mnemonic = TM_BKPT;
            add_immediate();
            assemble_and_show_result(&addr, &instruction);
            break;
        case ':': { // Define Label
            define_new_label(&addr);
            break;
        }
        case '.': // Specify Width
            width_specifier = menu_preset_instruction_width_menu(width_specifier);
            break;
        case CTRL('c'): { // Call Assembly
            void *fn_address = arm_address_set_thumb_intwrk_bit(starting_addr, true);
            monitor_call_function(fn_address, true); // Call with step
            break;
        }
        case CTRL('p'): // Print Assembly
            for (thumb_t *p = starting_addr; p < end_addr; p = thumb_ins_ptr_increment(p)) {

                // Print label
                monitor_disassemble_print_label_at(p);

                // Print indicator of current position before instruction
                putchar(p == addr ? '*' : ' ');

                // Print instruction out
                monitor_disassemble_print_instruction_at(p);
            }
            if (addr == end_addr) {
                putstring("*\r\n");
            }
            break;
        case '0': // Up to start
            addr = starting_addr;
            break;
        case '$': // Down to end
            addr = end_addr;
            break;
        case CTRL('u'): // "Up"
            if (addr > starting_addr) {
                addr = previous_thumb_instruction(starting_addr, addr);
            }
            break;
        case CTRL('d'): // "Down"
            if (addr < end_addr) {
                addr = thumb_ins_ptr_increment(addr);
            }
            break;
        case CTRL('r'): // "Rewind Instruction"
            if (end_addr > starting_addr) {
                end_addr = previous_thumb_instruction(starting_addr, end_addr);
                if (addr > end_addr) {
                    addr = end_addr;
                }
            }
            break;
        case CTRL('q'): // Quit
            quit = true;
            break;
        default:
            menu_print_missing_action_message();
            break;
        }
    }
}
