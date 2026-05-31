#ifndef THUMB_ASM_H_INCLUDED
#define THUMB_ASM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

enum thumb_mnemonic {
    TM_UNKNOWN,

    TM_BKPT,
    TM_BX,
    TM_NOP,
};

struct thumb_operand {
    enum thumb_operand_type {
        OT_NONE,
        OT_REG,
        OT_IMMEDIATE
    } type;
    union {
        unsigned reg;
        unsigned imm;
    };
};

struct thumb_instruction {
    enum thumb_mnemonic mnemonic;
    bool wide;
    struct thumb_operand operands[3];
};

bool thumb_is_wide_instruction(uint16_t ins);
void thumb_print_disassembled_instruction(const uint16_t *insptr);

#endif
