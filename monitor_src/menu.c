#include "menu.h"

#include "assert.h"
#include "char.h"
#include "io.h"
#include "util.h"
#include "vt.h"

static const char *last_prompt;
static const char *last_option;

char menu(const char *prompt, unsigned option_count,
          const struct menu_option *options) {
    assert(option_count > 0);
    putstring(prompt);

    while (1) {
        char c = getchar();

        // Help
        if (c == '?') {
            putstring("?\r\n");
            for (unsigned i = 0; i < option_count; i++) {
                const struct menu_option *opt = &options[i];
                putstring(" * ");
                if (IS_CTRL(opt->key)) {
                    putstring("c-");
                    putchar(opt->key + 'A' - 1); // -1 as C-A is 1 not 0
                } else {
                    assert(char_isprint(c));
                    putchar('\'');
                    putchar(opt->key);
                    putchar('\'');
                }
                putstring(" - ");
                putstring(opt->name);
                putnewline();
            }
            putstring(prompt);
            continue;
        }

        // Check options
        for (unsigned i = 0; i < option_count; i++) {
            const struct menu_option *opt = &options[i];
            if (opt->key == c) {
                putstring(opt->name);
                putnewline();

                last_prompt = prompt;
                last_option = opt->name;

                return c;
            }
        }
    }
}

// WARNING: Ensure the prompt and options struct of the parent menu is still alive
char submenu(const char *prompt, unsigned option_count,
             const struct menu_option *options) {
    assert(option_count > 0);

    vt_upline();
    vt_clearline();

    putstring(last_prompt);
    putstring(last_option);
    putchar(' ');
    putstring(prompt);

    while (1) {
        char c = getchar();

        // Help
        if (c == '?') {
            putstring("?\r\n");
            for (unsigned i = 0; i < option_count; i++) {
                const struct menu_option *opt = &options[i];
                putstring(" * ");
                if (IS_CTRL(opt->key)) {
                    putstring("c-");
                    putchar(opt->key + 'A' - 1); // -1 as C-A is 1 not 0
                } else {
                    assert(char_isprint(c));
                    putchar('\'');
                    putchar(opt->key);
                    putchar('\'');
                }
                putstring(" - ");
                putstring(opt->name);
                putnewline();
            }
            putstring(last_prompt);
            putstring(last_option);
            putchar(' ');
            putstring(prompt);
            continue;
        }

        // Check options
        for (unsigned i = 0; i < option_count; i++) {
            const struct menu_option *opt = &options[i];
            if (opt->key == c) {
                putstring(opt->name);
                putnewline();
                return c;
            }
        }
    }
}

bool menu_preset_continue(const char *prompt) {
    static const struct menu_option continue_options[] = {
        {'c', "Continue"},
        {'q', "Quit"    },
    };
    return menu(prompt, ARR_LEN(continue_options), continue_options) == 'c';
}

enum menu_warning_fix_result menu_preset_warning_fix(const char *prompt) {
    static const struct menu_option warning_options[] = {
        {'p', "Proceed"},
        {'f', "Fix"},
        {'q', "Quit"    },
    };
    char opt = menu(prompt, ARR_LEN(warning_options), warning_options);
    switch (opt) {
    case 'p': return MENU_WF_PROCEED;
    case 'f': return MENU_WF_FIX;
    case 'q': return MENU_WF_QUIT;
    }
    ASSERT_NOT_REACHED();
}
