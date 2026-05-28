#include "thumb_asm.h"

bool thumb_is_wide_instruction(uint16_t ins) {
    // ARMv7-M Architecture Reference Manual
    // A5.3 32bit Thumb instruction Encoding
    return ((ins >> 13) == 7) && ((ins >> 11) != 0);
}
