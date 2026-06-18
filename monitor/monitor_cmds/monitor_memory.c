#include "monitor_memory.h"

#include "../io.h"
#include "../memory.h"
#include "../menu.h"
#include "../string.h"
#include "../util.h"

#include <stdbool.h>
#include <stddef.h>

void monitor_memory() {
    static const struct menu_option memory_options[] = {
        {'c', "Set Code Label"},
        {'d', "Set Data Label"},
        {'q', "Quit"          },
    };
    bool quit = false;

    while (!quit) {
        memory_print_entries();

        char opt =
            menu("Memory? ", ARR_LEN(memory_options), memory_options, NULL);
        switch (opt) {
        case 'c':
        case 'd': {
            putstring("Address? ");
            void *ptr = (void *)gethexword(0);
            putstring("Name? ");
            const char *name = io_getline();
            bool created =
                memory_create_label(name, strlen(name), ptr, opt == 'c');
            io_printf("%s\r\n", created ? "Created label!" : "Error: Label already exists or name is invalid.");
            break;
        }
        case 'q':
            quit = true;
            break;
        }
    }
}
