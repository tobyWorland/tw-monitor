#include "demo_support.h"

#include "../monitor_src/char.h"
#include "../monitor_src/menu.h"

#include <stdio.h>

int main() {
    demo_make_term_raw();

    struct menu_option options[] = {
        {'a',       "Option A"          },
        {CTRL('A'), "Optional CONTROL A"},
        {'b',       "Option B"          },
        {'c',       "Option C"          }
    };
    char picked =
        menu("Demo menu? ", sizeof(options) / sizeof(options[0]), options);
    printf("User picked '%c'\r\n", picked);

    return 0;
}
