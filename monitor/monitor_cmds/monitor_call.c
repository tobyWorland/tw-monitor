#include "monitor_call.h"

#include "../arm.h"
#include "../assert.h"
#include "../menu.h"

#ifndef HOST
void monitor_call_function(void (*fn)(), bool step) {
    // Check for thumb interwork bit in function address
    if (!arm_address_has_thumb_intwrk_bit(fn)) {
        switch (menu_preset_warning_fix("WARN No thumb bit: ")) {
        case MENU_WF_FIX:
            fn = arm_address_set_thumb_intwrk_bit(fn, true);
        case MENU_WF_PROCEED:
            break;
        case MENU_WF_QUIT:
            return;
        default:
            assert(false);
        }
    }

    // Do call
    if (step) {
        arm_enable_debug_stepping(true);

        // If using hardware, use a breakpoint instruction to trigger debugger
        if (arm_halting_debug_active()) {
            __BKPT(1);
        }
    }
    fn();
}
#endif
