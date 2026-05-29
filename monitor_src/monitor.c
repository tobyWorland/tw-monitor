#include "assert.h"
#include "arm.h"
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
extern uint32_t user_start; // Defined in linker script

void hidden() { // TOOD: Remove
    putstring("Hidden function called!\r\n");
}

__attribute__((naked))
void breakpoint() { // TOOD: Remove
    __asm("nop\n"
          "nop\n"
          "bkpt 1\n"
          "nop\n"
          "nop.w\n"
          "nop\n"
          "bx lr");
}

void monitor_main(bool surpress_init) {
    static const struct menu_option options[] = {
        {'d',       "Memory Dump"           },
        {'c',       "Call Address"          },
        {'e',       "Enter"                 },
        {'u',       "Un/Disassemble"        },
        {'s',       "Call Address with Step"},
        {CTRL('l'), "Clear Screen"          },
    };
    static uint32_t addr = 0;

    if (!surpress_init) {
        vt_clearscreen();

        putstring("** Monitor ready **\r\n");
        putstring("hidden address: ");
        puthexword((uint32_t)hidden | 1); // | 1 for the interwork bit
        putnewline();
        putstring("breakpoint address: ");
        puthexword((uint32_t)breakpoint | 1); // | 1 for the interwork bit
        putnewline();
        putstring("User: ");
        puthexword((uint32_t)&user_start);
        putnewline();
        putstring("Halting debug? ");
        putstring(arm_halting_debug_active() ? "YES" : "NO");
        putnewline();
        if (arm_halting_debug_active()) {
            putstring("Monitor will not work as hardware debugger will take "
                      "priority!");
            putnewline();
        }
        arm_enable_debug_monitor();
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
            monitor_call_function((void *)addr, false);
            break;
        case 's':
            addr = gethexword(addr);
            monitor_call_function((void *)addr, true);
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
