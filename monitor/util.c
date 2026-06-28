#include "util.h"

#include "assert.h"

#include <stdint.h>

static int8_t s_critical_count = 0;

void util_enter_critical(void) {
#ifndef HOST
    __asm("CPSID F"); // Disable interrupts by setting FAULTMASK
    s_critical_count++;
#endif
}

void util_leave_critical(void) {
#ifndef HOST
    assert(--s_critical_count >= 0);
    if (s_critical_count == 0) {
        __asm("CPSIE F"); // Enable interrupts by setting FAULTMASK
    }
#endif
}
