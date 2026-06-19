#include "arm.h"

#include "thumb_asm.h"
#include "util.h"
#include "io.h"
#include "menu.h"
#include "monitor_cmds/monitor_disassemble.h"
#include "startup.h"

#include <stddef.h>
#include <stdint.h>

// TODO: Test
// TODO: Make address conversion portable to run on host

bool arm_address_has_thumb_intwrk_bit(void *address) {
    uint32_t addr = (uint32_t)address;
    return addr & BIT(0);
}

void *arm_address_set_thumb_intwrk_bit(void *old_address, bool set) {
    uint32_t addr = (uint32_t)old_address;

    if (set) {
        addr |= BIT(0);
    } else { // Clear
        addr &= ~BIT(0);
    }

    return (void*)addr;
}

volatile uint32_t *DHCSR = (void *)0xE000EDF0;
volatile uint32_t *DEMCR = (void *)0xE000EDFC;

#define DHCSR_C_DEBUGEN BIT(0)

#define DEMCR_MON_STEP  BIT(18)
#define DEMCR_MON_PEND  BIT(17)
#define DEMCR_MON_EN    BIT(16)

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
        break;
    case 'c':
        step = false;
        break;
    case 'u':
        monitor_disassemble(pc);
        break;
    case 'm':
        pc = exit_to_monitor;
        step = false;
        exit_to_monitor();
        break;
    default:
        putstring("HANG");
        while(1);
    }

    arm_enable_debug_stepping(step);

#if 0
    putstring("New PC: ");
    puthexword((uint32_t)pc);
    putnewline();
#endif

    return pc;
}
