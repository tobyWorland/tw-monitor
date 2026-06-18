#include "monitor_dev.h"

#include "../clobber_sram.h"
#include "../io.h"
#include "../menu.h"
#include "../util.h"

#include <stddef.h>

extern uint32_t vector_table[];

void monitor_dev(void) {
    static const struct menu_option dev_options[] = {
        {'e', "Exec from USART1"},
        {'s', "Corrupt SVCall"},
        {'q', "Quit"          },
    };
    char opt = menu("DEV> ", ARR_LEN(dev_options), dev_options, NULL);
    switch (opt) {
    case 'e': {
        clobber_sram_from_usart1();
        break;
    }
    case 's': {
        vector_table[11] = 0; // Zero the vector SVCall
        break;
    }
    case 'q':
        break;
    default:
        putstring("Error: Missing action\r\n");
        break;
    }
}
