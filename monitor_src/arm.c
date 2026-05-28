#include "arm.h"

#include "util.h"

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
