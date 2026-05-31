#include "monitor_assemble.h"

#include "../char.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"

#include <stddef.h>

void monitor_assemble(void *addr) {
    static const struct menu_option assemble_options[] = {
        {'b', "BX"},
        {'n', "NOP"},
        {'N', "NOP.W"},
        {'x', "BKPT"},
        {CTRL('p'), "Print Assembly"},
        {CTRL('q'), "Quit"},
    };
    bool quit = false;
    void *starting_addr = addr;

    while (!quit) {
        char opt = menu("ASM> ", ARR_LEN(assemble_options), assemble_options, NULL);
        switch (opt) {
        case CTRL('q'):
            quit = true;
            break;
        default:
            putstring("Error: Missing action\r\n");
            break;
        }
    }
}
