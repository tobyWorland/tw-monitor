#ifndef THUMB_ASM_H_INCLUDED
#define THUMB_ASM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

// Instructions are made of 16bit
// Even though thumb is a mixture of 32bit wide and 16bit narrow
// instructions, wide is 2 x 16bit half words as oppose to a single
// 32bit word. (There is a difference in little endian)
typedef uint16_t thumb_t;

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

bool thumb_is_wide_instruction(thumb_t ins);
void thumb_print_disassembled_instruction(const uint16_t *insptr);

enum thumb_assemble_result {
    AR_FAIL_BAD_MNEMONIC,
    AR_FAIL_INVALID_WIDTH,
    AR_FAIL_INVALID_OPERAND,

    AR_NARROW_SUCCESS = 0x10,
    AR_WIDE_SUCCESS,
};
enum thumb_assemble_result thumb_assemble(thumb_t *into, const struct thumb_instruction *instruction_spec);

#endif
