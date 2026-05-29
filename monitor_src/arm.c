#include "arm.h"

#include "util.h"
#include "io.h"

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
#define DEMCR_MON_EN    BIT(16)

bool arm_halting_debug_active(void) {
    return *DHCSR & DHCSR_C_DEBUGEN;
}

void arm_enable_debug_monitor(void) {
    *DEMCR |= DEMCR_MON_EN;
}

void debug_monitor() {
    putstring("**DEBUG**\r\n");
    return;
}
