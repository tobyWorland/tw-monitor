#include <gtest/gtest.h>

extern "C" {
#include "../monitor/thumb_asm.h"
}

thumb_t a = (thumb_t) {
    .as_halfwords = {0x1, 0x2}
};

const thumb_t nop_w = {.as_halfwords = {0xF3AF, 0x8000}};
const thumb_t bx_lr = {.narrow = 0x4770};
const thumb_t b_0   = {.narrow = 0xE7FE};

TEST(thumb_asm, is_wide) {
    EXPECT_EQ(thumb_is_wide_instruction(nop_w), true);
    EXPECT_EQ(thumb_is_wide_instruction(bx_lr), false);
    EXPECT_EQ(thumb_is_wide_instruction(b_0), false);
}

// TODO: Test thumb_print_disassembled_instruction
