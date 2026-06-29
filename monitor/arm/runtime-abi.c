#include <stdint.h>

// See "Run-time ABI for the Arm Architecture"

// Return 64bit values into registers by putting this last
// Can be used for signed or unsigned it doesn't matter
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__((naked))
static void return_2_64s_into_regs(uint64_t a, uint64_t b) {
    __asm("bx lr");
}
#pragma GCC diagnostic pop

// NOTE: Is not actually void
void __aeabi_ldivmod(int64_t numerator, int64_t denominator) {
    int64_t quotient = 0;

    for (; numerator >= denominator; numerator -= denominator) {
        quotient++;
    }

    int64_t remainder = numerator;

    return_2_64s_into_regs(quotient, remainder);
}

// NOTE: Is not actually void
void __aeabi_uldivmod(uint64_t numerator, uint64_t denominator) {
    uint64_t quotient = 0;

    for (; numerator >= denominator; numerator -= denominator) {
        quotient++;
    }

    uint64_t remainder = numerator;

    return_2_64s_into_regs(quotient, remainder);
}
