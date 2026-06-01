#include "demo_support.h"

#include "../monitor/char.h"
#include "../monitor/menu.h"
#include "../monitor/util.h"

#include <stdio.h>

int main() {
    demo_make_term_raw();

    struct menu_option options[] = {
        {'a',       "Option A"        },
        {CTRL('A'), "Option CONTROL A"},
        {'b',       "Option B"        },
        {'c',       "Option C"        },
        {'s',       "Submenu"         },
    };
    char picked =
        menu("Demo menu? ", sizeof(options) / sizeof(options[0]), options, "s");

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
