#include "debug.h"

#include "arm.h"
#include "../hardware/stm32f411xce/stm32f411xce_peripherals.h"
#include "../hardware/stm32f411xce/stm32f411xce_usart.h"
#include "../terminal.h"
#include "../thumb_asm.h"
#include "../util.h"
#include "../io.h"
#include "../menu.h"
#include "../monitor_cmds/monitor_disassemble.h"
#include "../startup.h"

#include <stddef.h>
#include <stdint.h>

volatile uint32_t *DHCSR = (void *)0xE000EDF0;
volatile uint32_t *DEMCR = (void *)0xE000EDFC;

#define DHCSR_C_DEBUGEN BIT(0)

#define DEMCR_MON_STEP  BIT(18)
#define DEMCR_MON_PEND  BIT(17)
#define DEMCR_MON_EN    BIT(16)

static void *s_start_stepping_address = NULL;

// When using gdb to trigger the monitor:
// set *DEMCR |= (1 << 17)

bool arm_halting_debug_active(void) {
    return *DHCSR & DHCSR_C_DEBUGEN;
}

void arm_enable_debug_monitor(void) {
    *DEMCR |= DEMCR_MON_EN;
}

void arm_enable_debug_stepping(bool on) {
    if (on) {
        *DEMCR |= DEMCR_MON_STEP;
    } else {
        *DEMCR &= ~DEMCR_MON_STEP;
    }
}

void arm_pend_debug_monitor(void) {
    *DEMCR |= DEMCR_MON_PEND;
}

const struct menu_option debug_options[] = {
    {'s', "Step"                    },
    {'z', "Skip Instruction"        },
    {'c', "Continue (Stop stepping)"},
    {'u', "Un-assemble from PC"     },
    {'m', "Exit to monitor"         },
};

static void increment_pc(void **pc) {
    *pc = thumb_ins_ptr_increment(*pc);
}

void *debug_monitor(void *pc) {
    bool step = true;
    bool exit = false;

    // Check if we're meant to be automatically stepping until a
    // certain address before showing the debugger to the user
    // See arm_begin_stepping_at() for more details
    if (s_start_stepping_address != NULL) {
        static void *last_pc = NULL;
        bool is_stuck = pc == last_pc;

        // Check we don't get stuck while automatically stepping
        // E.g. breakpoint instructions, or branches to self
        // Note: This won't stop infinite loops of more than one instruction however
        if (is_stuck) {
            terminal_set_video_reverse(true);
            io_printf("Error: Debugger got stuck while trying to step to: 0x%x\r\n", pc);
            terminal_set_video_reverse(false);
        }

        if (pc == s_start_stepping_address || is_stuck) {
            last_pc = NULL;
            s_start_stepping_address = NULL;
        } else {
            last_pc = pc;
            return pc;
        }
    }

    // TODO: Make more generic (not specific to USART2)
    bool usart_poll = usart_get_polling(&g_periph_usart2);
    if (!usart_poll) {
        // Interrupts for USART won't work in debug monitor - temporary disable
        usart_set_polling(&g_periph_usart2, true);
    }

    while (!exit) {
        exit = true;
        putstring("**DEBUG**\r\n");
        print_registers();

        // Same as R15 from print_registers()
        putstring("PC: ");
        puthexword((uint32_t)pc);
        putchar(' ');
        struct thumb_instruction_spec ins_spec = thumb_disassemble(pc);
        thumb_print_instruction(&ins_spec, pc);
        putnewline();

        char opt = menu("DEBUG> ", ARR_LEN(debug_options), debug_options, NULL);
        switch (opt) {
        case 's':
            break;
        case 'z':
            increment_pc(&pc);
            exit = false;
            break;
        case 'c':
            step = false;
            break;
        case 'u':
            monitor_disassemble(pc);
            exit = false;
            break;
        case 'm':
            pc = exit_to_monitor;
            step = false;
            break;
        default:
            putstring("HANG");
            while(1);
        }
    }

    arm_enable_debug_stepping(step);

    // TODO: Make more generic (not specific to USART2)
    if (!usart_poll) {
        // Re-enable USART2 interrupt
        usart_set_polling(&g_periph_usart2, false);
    }

#if 0
    putstring("New PC: ");
    puthexword((uint32_t)pc);
    putnewline();
#endif

    return pc;
}

// Set an address we want to show the debugger at
//
// This approach avoids having to use a breakpoint instruction, or the
// limited FPB (flash patch and Breakpoint unit) as the board I'm
// targeting only has the v1 which will only work on the code area of
// memory, instead of across the whole 32bit address space. Although
// support for this unit will be implemented in the future.
void arm_begin_stepping_at(void *address) {
    // Program counter doesn't have the interwork bit
    void *pc_address = arm_address_set_thumb_intwrk_bit(address, false);

    s_start_stepping_address = pc_address;
    arm_enable_debug_stepping(true);
}
