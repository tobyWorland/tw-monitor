#include "demo_support.h"

#include "../monitor/char.h"
#include "../monitor/menu.h"
#include "../monitor/util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool quit_number_menu = false;
enum menu_number_callback_result number_callback(int *px, char c) {
    enum menu_number_callback_result result = MNCR_PRINT_OPT;

    switch (c) {
    case 'i':
        *px += 3;
        break;
    case 'o':
        *px -= 5;
        result |= MNCR_CLR_REPROMPT;
        break;
    case 'p':
        *px = 172;
        result &= ~MNCR_PRINT_OPT; // Suppress printing the option
        break;
    case CTRL('q'):
        quit_number_menu = true;
        result |= MNCR_QUIT_MENU;
        break;
    }
    return result;
}

void number_menu() {
    struct menu_option number_opts[] = {
        {'i',       "Add 3 (shown)"              },
        {'o',       "Sub 5 (shown and re-prompt)"},
        {'p',       "Set to 172 (not shown)"     },
        {CTRL('q'), "Quit"                       },
    };

    const int def = 5;

    while (!quit_number_menu) {
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
