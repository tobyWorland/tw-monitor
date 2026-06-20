#include "assert.h"
#include "arm/arm.h"
#include "arm/scb.h"
#include "char.h"
#include "io.h"
#include "menu.h"
#include "memory.h"
#include "monitor_cmds/monitor_assemble.h"
#include "monitor_cmds/monitor_call.h"
#include "monitor_cmds/monitor_dev.h"
#include "monitor_cmds/monitor_disassemble.h"
#include "monitor_cmds/monitor_enter.h"
#include "monitor_cmds/monitor_memdump.h"
#include "monitor_cmds/monitor_memory.h"
#include "util.h"
#include "string.h"
#include "terminal.h"
#include "transfer.h"

#include "stm32f411xce_timer.h"

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
        {'a',       "Assemble"              },
        {'c',       "Call Address"          },
        {'d',       "Memory Dump"           },
        {'e',       "Enter"                 },
        {'m',       "Memory State"          },
        {'u',       "Un/Disassemble"        },
        {'r',       "Receive File"          },
        {'s',       "Call Address with Step"},
        {';',       "Development Menu"      },
        {CTRL('l'), "Clear Screen"          },
        {CTRL('r'), "Reset"                 },
    };
    static uint32_t addr = 0;

    if (!surpress_init) {
        terminal_clearscreen();

        memory_init();

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

    stm32f411xce_timer_sleep_init();

#if 0
    while (1) {
        putstring("Waiting for 1...\r\n");
        sleep(1);
        putstring("Waiting for 3...\r\n");
        sleep(3);
        putstring("Waiting for 10...\r\n");
        sleep(10);
    }
#endif

#if 0
    io_printf("Hello!\r\n");
    io_printf("Hello %s!\r\nX = %x, Y = %x\r\nC = '%c'\r\n",
              "world", 4, 7, 'V');
#endif

    while (1) {
        char opt = menu("> ", ARR_LEN(options), options, "e");
        switch (opt) {
        case 'a': {
            struct absolute_address_result absolute_address =
                menu_preset_absolute_address("Assemble to? ", (void *)addr,
                                             true, true);

            if (absolute_address.address) {
                addr = (uint32_t)absolute_address.address;
            }

            monitor_assemble(absolute_address.address, absolute_address.section);
            break;
        }
        case 'c':
            addr = gethexword(addr);
            monitor_call_function((void *)addr, false);
            break;
        case 'd':
            addr = gethexword(addr);
            monitor_memdump((void *)addr);
            break;
        case 'e': {
            enum enter_type ent_type = enter_ent_type_submenu();
            addr = gethexword(addr);
            monitor_enter((void *)addr, ent_type);
            break;
        }
        case 'm':
            monitor_memory();
            break;
        case 'u':
            addr = gethexword(addr);
            monitor_disassemble((void *)addr);
            break;
        case 'r':
            addr = gethexword(addr);
            transfer_receive((void*)addr);
            break;
        case 's':
            addr = gethexword(addr);
            monitor_call_function((void *)addr, true);
            break;
        case ';':
            monitor_dev();
            break;
        case CTRL('l'):
            terminal_clearscreen();
            break;
        case CTRL('r'):
            scb_sys_reset_request();
            break;
        default:
            menu_print_missing_action_message();
            break;
        }
    }
}
#endif
