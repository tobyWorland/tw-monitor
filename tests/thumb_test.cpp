#include <gtest/gtest.h>

extern "C" {
#include "../monitor_src/thumb_asm.h"
}

const uint16_t nop_w[2] = {0xF3AF, 0x8000};
const uint16_t bx_lr = 0x4770;

TEST(thumb_asm, is_wide) {
    EXPECT_EQ(thumb_is_wide_instruction(nop_w[0]), true);
    EXPECT_EQ(thumb_is_wide_instruction(bx_lr), false);
}

// TODO: Test thumb_print_disassembled_instruction
