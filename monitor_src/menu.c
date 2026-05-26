#include "menu.h"

#include "assert.h"
#include "io.h"

char menu(const char *prompt, unsigned option_count, struct menu_option *options) {
    assert(option_count > 0);
    putstring(prompt);

    while (1) {
        char c = getchar();

        // Help
        if (c == '?') {
            putstring("?\r\n");
            for (unsigned i = 0; i < option_count; i++) {
                const struct menu_option *opt = &options[i];
                putstring(" * '");
                putchar(opt->key);
                putstring("' - ");
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
