#include "menu.h"

#include "arm.h"
#include "assert.h"
#include "char.h"
#include "io.h"
#include "memory.h"
#include "thumb_asm.h"
#include "util.h"
#include "terminal.h"
#include "string.h"

#include <limits.h>
#include <stddef.h>

static const char *last_prompt;
static const char *last_option;

static void print_menu_help(unsigned option_count,
                            const struct menu_option *options,
                            bool print_help_question_mark) {
    if (print_help_question_mark) {
        putstring("?\r\n");
    }
    for (unsigned i = 0; i < option_count; i++) {
        const struct menu_option *opt = &options[i];
        putstring(" * ");
        switch (opt->key) {  // TODO: TEST
        case '\r':
            putstring("RET");
            break;
        case '\b':
            putstring("BAK");
            break;
        case ' ':
            putstring("SPC");
            break;
        default:
            if (IS_CTRL(opt->key)) {
                putstring("c-");
                putchar(opt->key + 'A' - 1); // -1 as C-A is 1 not 0
            } else {
                assert(char_isprint(opt->key));
                putchar('\'');
                putchar(opt->key);
                putchar('\'');
            }
        }
        putstring(" - ");
        putstring(opt->name);
        putnewline();
    }
}

static const struct menu_option *find_menu_option(char c, unsigned option_count,
                                                  const struct menu_option *options) {
    for (unsigned i = 0; i < option_count; i++) {
        const struct menu_option *opt = &options[i];
        if (opt->key == c) {
            return opt;
        }
    }
    return NULL;
}

char menu(const char *prompt, unsigned option_count,
          const struct menu_option *options, const char *submenu_keys) {
    assert(option_count > 0);
    putstring(prompt);

    while (1) {
        char c = getchar();

        // Help
        if (c == '?') {
            print_menu_help(option_count, options, true);
            putstring(prompt);
            continue;
        }

        // Check options
        const struct menu_option *opt = find_menu_option(c, option_count, options);
        if (opt != NULL) {
            putstring(opt->name);

            last_prompt = prompt;
            last_option = opt->name;

            // If c is in submenu_keys don't print a newline
            if (submenu_keys != NULL) {
                for (const char *s = submenu_keys; *s; s++) {
                    if (*s == c) {
                        return c;
                    }
                }
            }

            putnewline();
            return c;
        }
    }
}

// WARNING: Ensure the prompt and options struct of the parent menu is still alive
char submenu(const char *prompt, unsigned option_count,
             const struct menu_option *options) {
    assert(option_count > 0);
    putchar(' ');
    putstring(prompt);

    while (1) {
        char c = getchar();

        // Help
        if (c == '?') {
            print_menu_help(option_count, options, true);
            putstring(last_prompt);
            putstring(last_option);
            putchar(' ');
            putstring(prompt);
            continue;
        }

        // Check options
        const struct menu_option *opt = find_menu_option(c, option_count, options);
        if (opt != NULL) {
            putstring(opt->name);
            putnewline();
            return c;
        }
    }
}

int menu_number(const char *prompt, int init_number, unsigned option_count,
                const struct menu_option *options,
                enum menu_number_callback_result (*on_option)(int *, char c)) {

    // If no options are given,
    // then there must be a NULL for options & on_option,
    // and option_count must be 0
    if (!options || !option_count || !on_option) {
        assert(!options && !option_count && !on_option);
    }

    int number = init_number;
    int base = 10;
    bool sign = false;

    static const struct menu_option number_options[] = {
        {CTRL('b'), "Toggle Base 16 <==> 10"},
        {CTRL('i'), "Initial Number"        },
        {CTRL('z'), "Zero Current"          },
        {'-',       "Toggle Sign"           },
        {'\r',      "Submit"                },
        {'\b',      "Remove Last Digit"     },
    };

    while (1) {
        terminal_clearline();
        putstring(prompt);
        if (number == 0 && sign) {
            putchar('-');
        }
        putstring(itoa_pad(number, base));

        char c = getchar();

        // Handle digits
        uint8_t digit = char_to_digit(c);
        if (digit < base) {
            if (sign) {
                if ((INT_MIN + digit) / base < number) {
                    number *= base;
                    number -= digit;
                }
            } else {
                if ((INT_MAX - digit) / base > number) {
                    number *= base;
                    number += digit;
                }
            }
            continue;
        }

        // Help
        if (c == '?') {
            print_menu_help(option_count, options, true);
            print_menu_help(ARR_LEN(number_options), number_options, false);
            putstring(prompt);
            continue;
        }

        // Check number options (built in)
        const struct menu_option *num_opt = find_menu_option(c, ARR_LEN(number_options), number_options);
        if (num_opt != NULL) {
            switch (c) {
            case CTRL('b'): // Toggle Base
                base = base == 10 ? 16 : 10;
                break;
            case CTRL('i'): // Initial Number
                sign = init_number < 0;
                number = init_number;
                continue;
            case CTRL('z'): // Zero Current
                sign = false;
                number = 0;
                continue;
            case '-': // Toggle Sign
                if (number == 0) {
                    // Allow flipping sign on zero for easily entering negative numbers
                    sign = !sign;
                } else {
                number *= -1;
                sign = number < 0; // Don't just NOT the sign as INT_MIN can never be positive
                }
                continue;
            case '\r': // Submit
                putnewline();
                return number;
            case '\b': // Remove Last Digit
                number /= base;
                continue;
            default:
                ASSERT_NOT_REACHED();
                break;
            }

            putchar(' ');
            putstring(num_opt->name);
            putnewline();
            continue;
        }

        // Check options
        const struct menu_option *opt = find_menu_option(c, option_count, options);
        if (opt != NULL) {
            enum menu_number_callback_result callresult = on_option(&number, c);
            bool need_newline = false;

            if (callresult & MNCR_CLR_REPROMPT) {
                terminal_clearline();
                putstring(prompt);
                if (number == 0 && sign) {
                    putchar('-');
                }
                putstring(itoa_pad(number, base));
                need_newline = true;
            }
            if (callresult & MNCR_PRINT_OPT) {
                putchar(' ');
                putstring(opt->name);
                need_newline = true;
            }

            if (need_newline) {
                putnewline();
            }

            if (callresult & MNCR_QUIT_MENU) {
                return number;
            }
        }
    }
}

bool menu_preset_continue(const char *prompt, bool erase_on_continue) {
    static const struct menu_option continue_options[] = {
        {'c', "Continue"},
        {'q', "Quit"    },
    };
    bool cont = menu(prompt, ARR_LEN(continue_options), continue_options,
                     erase_on_continue ? "c" : NULL) == 'c';

    if (cont) {
        terminal_clearline();
    }

    return cont;
}

enum menu_warning_fix_result menu_preset_warning_fix(const char *prompt) {
    static const struct menu_option warning_options[] = {
        {'p', "Proceed"},
        {'f', "Fix"},
        {'q', "Quit"    },
    };
    char opt = menu(prompt, ARR_LEN(warning_options), warning_options, NULL);
    switch (opt) {
    case 'p': return MENU_WF_PROCEED;
    case 'f': return MENU_WF_FIX;
    case 'q': return MENU_WF_QUIT;
    }
    ASSERT_NOT_REACHED();
}

static enum menu_number_callback_result menu_preset_register_callback(int *reg, char c) {
    switch (c) {
    case 's':
        *reg = 13;
        break;
    case 'l':
        *reg = 14;
        break;
    case 'p':
        *reg = 15;
        break;
    }
    return MNCR_CLR_REPROMPT | MNCR_PRINT_OPT | MNCR_QUIT_MENU;
}

unsigned menu_preset_register(const char *prompt) {
    static const struct menu_option register_options[] = {
        {'s', "SP"},
        {'l', "LR"},
        {'p', "PC"},
    };

    int reg = menu_number(prompt, 0, ARR_LEN(register_options),
                          register_options, menu_preset_register_callback);
    return reg;
}

#ifndef HOST
intptr_t menu_preset_relative_label(const char *prompt, void *relative_from, bool is_code) {
    static const struct menu_option label_options[] = {
        {'.', "Here"  },
        {'+', "+Bytes"},
        {'-', "-Bytes"},
        {'l', "Label"},
    };

    char opt = menu(prompt, ARR_LEN(label_options), label_options, NULL);
    switch (opt) {
    case '+': {
        // TODO: Should be able to cancel and return back to this menu
        return gethexword(0);
    }
    case '-': {
        // TODO: Should be able to cancel and return back to this menu
        return -(intptr_t)gethexword(0);
    }
    case 'l': {
        const char *name;
        while (1) {
            putstring("Label Name? ");
            name = io_getline();
            struct memory_entry *label = memory_lookup_label(name, strlen(name));
            if (label) {
                void *label_addr = memory_addr_from_entry(label);
                if (is_code) {
                    // Relative addresses to code should not have the interwork bit set
                    // Should only use interwork for absolute addresses
                    label_addr = arm_address_set_thumb_intwrk_bit(label_addr, false);
                }
                return (intptr_t)label_addr - (intptr_t)relative_from;
            } else {
                putstring("Error: Label does not exist.\r\n");
            }
        }
    }
    case '.': return 0;
    }
    ASSERT_NOT_REACHED();
}
#endif

static bool mp_register_list_valid_reg(unsigned reg) {
    if (reg > 15) {
        io_printf("Invalid Register %u\r\n", reg);
        return false;
    }
    return true;
}

uint16_t menu_preset_register_list(const char *prompt) {
    static const struct menu_option list_options[] = {
        {',',  "Toggle Register"},
        {'-',  "Toggle Range"   },
        {'\r', "Submit"         },
    };

    uint16_t reg_list = 0;
    bool quit = false;

    while (!quit) {
        terminal_clearline();
        thumb_print_register_list(reg_list);
        putchar(' ');

        char opt = menu(prompt, ARR_LEN(list_options), list_options, NULL);

        switch (opt) {
        case ',': {
            const unsigned reg = menu_preset_register("REG ");
            if (mp_register_list_valid_reg(reg)) {
                reg_list ^= (1U << reg);
            }
            break;
        }
        case '-': {
            const unsigned start = menu_preset_register("FROM ");
            const unsigned end = menu_preset_register("TO ");
            if (mp_register_list_valid_reg(start) &&
                mp_register_list_valid_reg(end)) {
                for (unsigned r = start; r <= end; r++) {
                    reg_list ^= (1U << r);
                }
            }
            break;
        }
        case '\r':
            quit = true;
            break;
        }
    }

    return reg_list;
}

enum thumb_width_specifier menu_preset_instruction_width_menu(enum thumb_width_specifier current) {
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

bool menu_preset_instruction_set_flags_menu(void) {
    static const struct menu_option set_flags_options[] = {
        {'s', "Set Flags"      },
        {' ', "Don't Set Flags"},
    };

    return menu("Set Flags? ", ARR_LEN(set_flags_options), set_flags_options,
                NULL) == 's';
}

enum thumb_condition menu_preset_instruction_set_condition_menu(void) {
    static const struct menu_option set_condition_options[] = {
        {' ', "No condition"},

        // TODO: Should reuse the strings "condition_strs" from thumb_asm
        {'e', "EQ"          },
        {'n', "NE"          },
        {'c', "CS"          },
        {'C', "CC"          },
        {'m', "MI"          },
        {'p', "PL"          },
        {'v', "VS"          },
        {'V', "VC"          },
        {'h', "HI"          },
        {'l', "LS"          },
        {'g', "GE"          },
        {'L', "LT"          },
        {'G', "GT"          },
        {'E', "LE"          },
        {'a', "AL"          },
    };

    char opt = menu("Condition? ",
                    ARR_LEN(set_condition_options),
                    set_condition_options,
                    NULL);
    enum thumb_condition condition_result = TC_NONE;

    for (unsigned i = 0; i < ARR_LEN(set_condition_options); i++) {
        if (set_condition_options[i].key == opt) {
            return i;
        }
    }
    return condition_result;
}

void menu_print_missing_action_message(void) {
    putstring("Error: Missing action\r\n");
}
