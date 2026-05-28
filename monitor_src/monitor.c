#include "assert.h"
#include "char.h"
#include "io.h"
#include "menu.h"
#include "monitor_cmds/monitor_call.h"
#include "monitor_cmds/monitor_disassemble.h"
#include "monitor_cmds/monitor_enter.h"
#include "monitor_cmds/monitor_memdump.h"
#include "util.h"
#include "vt.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef HOST
void hidden() { // TOOD: Remove
    putstring("Hidden function called!\r\n");
}

void monitor_main(bool surpress_init) {
    static const struct menu_option options[] = {
        {'d',       "Memory Dump"   },
        {'c',       "Call Address"  },
        {'e',       "Enter"         },
        {'u',       "Un/Disassemble"},
        {CTRL('l'), "Clear Screen"  },
    };
    static uint32_t addr = 0;

    if (!surpress_init) {
        vt_clearscreen();

        putstring("** Monitor ready **\r\n");
        putstring("hidden address: ");
        puthexword((uint32_t)hidden | 1); // | 1 for the interwork bit
        putnewline();
    }

    while (1) {
        char opt = menu("> ", ARR_LEN(options), options, "e");
        switch (opt) {
        case 'd':
            addr = gethexword(addr);
            monitor_memdump((void *)addr);
            break;
        case 'c':
            addr = gethexword(addr);
            monitor_call_function((void *)addr);
            break;
        case 'e': {
            enum enter_type ent_type = enter_ent_type_submenu();
            addr = gethexword(addr);
            monitor_enter((void *)addr, ent_type);
        }
            break;
        case 'u':
            addr = gethexword(addr);
            monitor_disassemble((void *)addr);
            break;
        case CTRL('l'):
            vt_clearscreen();
            break;
        default:
            putstring("Error: Missing action\r\n");
            break;
        }
    }
}
#endif
