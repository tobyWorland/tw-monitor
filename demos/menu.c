#include "demo_support.h"

#include "../monitor/char.h"
#include "../monitor/menu.h"
#include "../monitor/util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool number_callback(int *px, char c) {
    switch (c) {
    case 'i':
        *px += 3;
        break;
    case 'o':
        *px -= 5;
        break;
    case 'p':
        *px = 172;
        return false; // Suppress option
    case CTRL('q'):
        puts(" Quit!\r");
        exit(0);
        break;
    }
    return true; // Allow option to be printed
}

void number_menu() {
    struct menu_option number_opts[] = {
        {'i',       "Add 3 (shown)"         },
        {'o',       "Sub 5 (shown)"         },
        {'p',       "Set to 172 (not shown)"},
        {CTRL('q'), "Quit"                  },
    };

    const int def = 5;

    while (1) {
        int x = menu_number("NUM> ", def, ARR_LEN(number_opts),
                        number_opts, number_callback);
        printf("Got: %d (%x)\r\n", x, x);
    }
}

int main() {
    demo_make_term_raw();

    struct menu_option options[] = {
        {'a',       "Option A"        },
        {CTRL('A'), "Option CONTROL A"},
        {'b',       "Option B"        },
        {'c',       "Option C"        },
        {'n',       "Number Menu"     },
        {'s',       "Submenu"         },
    };
    char picked =
        menu("Demo menu? ", sizeof(options) / sizeof(options[0]), options, "s");

    if (picked == 'n') {
        number_menu();
        return 0;
    }

    if (picked == 's') {
        struct menu_option suboptions[] = {
            {'1', "Number 1"},
            {'2', "Number 2"},
            {'3', "Number 3"},
        };
        char subpicked = submenu("Number? ", ARR_LEN(suboptions), suboptions);
        printf("User sub picked '%c'\r\n", subpicked);
    }

    printf("User picked '%c'\r\n", picked);

    return 0;
}
