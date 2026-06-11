#include "menu.h"

#include "assert.h"
#include "char.h"
#include "io.h"
#include "util.h"
#include "terminal.h"

#include <stddef.h>

static const char *last_prompt;
static const char *last_option;

static void print_menu_help(unsigned option_count,
                            const struct menu_option *options) {
    putstring("?\r\n");
    for (unsigned i = 0; i < option_count; i++) {
        const struct menu_option *opt = &options[i];
        putstring(" * ");
        if (IS_CTRL(opt->key)) {
            putstring("c-");
            putchar(opt->key + 'A' - 1); // -1 as C-A is 1 not 0
        } else {
            assert(char_isprint(opt->key));
            putchar('\'');
            putchar(opt->key);
            putchar('\'');
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
            print_menu_help(option_count, options);
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
            print_menu_help(option_count, options);
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

unsigned menu_preset_register(const char *prompt) {
    // TODO: Needs a nicer user experience
    //       /w Numbers input with the extra keys to pick sp, lr and pc
    static const struct menu_option register_options[] = {
        {'r', "Register "}, // Extra space is intended
        {'s', "SP"       },
        {'l', "LR"       },
        {'p', "PC"       },
    };

    char opt = menu(prompt, ARR_LEN(register_options), register_options, "r");
    switch (opt) {
    case 'r': {
        // TODO: Should be in decimal
        // TODO: Should be able to cancel and return back to this menu
        return gethexword(0);
    }
    case 's': return 13;
    case 'l': return 14;
    case 'p': return 15;
    }
    ASSERT_NOT_REACHED();
}

int32_t menu_preset_label(const char *prompt, void *relative_from) {
    static const struct menu_option label_options[] = {
        {'.', "Here"  },
        {'+', "+Bytes"},
        {'-', "-Bytes"},
        //{'l', "Label"}, // TODO: Implement
    };

    (void)relative_from; // Silence warnings - FIXME: Needed for "Label" option

    char opt = menu(prompt, ARR_LEN(label_options), label_options, NULL);
    switch (opt) {
    case '+': {
        // TODO: Should be able to cancel and return back to this menu
        return gethexword(0);
    }
    case '-': {
        // TODO: Should be able to cancel and return back to this menu
        return -(int32_t)gethexword(0);
    }
    case '.': return 0;
    }
    ASSERT_NOT_REACHED();
}
