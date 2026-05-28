#include "monitor_call.h"

#include "../arm.h"
#include "../assert.h"
#include "../menu.h"

#ifndef HOST
void monitor_call_function(void (*fn)()) {
    // Check for thumb interwork bit in function address
    if (!arm_address_has_thumb_intwrk_bit(fn)) {
        enum menu_warning_fix_result opt =
            menu_preset_warning_fix("WARN No thumb bit: ");
        if (opt == MENU_WF_FIX) {
            fn = arm_address_set_thumb_intwrk_bit(fn, true);
        } else if (opt == MENU_WF_QUIT) {
            return;
        } else {
            assert(opt == MENU_WF_PROCEED);
        }
    }

    // Do call
    fn();
}
#endif
