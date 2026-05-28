#include "menu.h"

#include "assert.h"
#include "char.h"
#include "io.h"

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
    return menu(prompt,
                sizeof(continue_options) / sizeof(continue_options[0]),
                continue_options) == 'c';
}
