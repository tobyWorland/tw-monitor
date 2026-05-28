#include "menu.h"

#include "assert.h"
#include "char.h"
#include "io.h"
#include "util.h"

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

bool menu_preset_continue(const char *prompt) {
    static const struct menu_option continue_options[] = {
        {'c', "Continue"},
        {'q', "Quit"    },
    };
    return menu(prompt, ARR_LEN(continue_options), continue_options, NULL) == 'c';
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
