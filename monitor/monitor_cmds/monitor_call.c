#include "monitor_call.h"

#include "../arm/arm.h"
#include "../arm/debug.h"
#include "../assert.h"
#include "../hardware/board.h"
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

    board_enable_debug_user_break(true);

    // Do call
    if (step) {
        if (!arm_halting_debug_active()) {
            arm_begin_stepping_at(fn);
        } else {
            // If using hardware, use a breakpoint instruction to trigger debugger instead
            __BKPT(1);
        }
    }
    fn();

    board_enable_debug_user_break(false);
}
#endif
