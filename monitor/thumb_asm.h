#ifndef THUMB_ASM_H_INCLUDED
#define THUMB_ASM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

// Maximum number of operands an instruction can have
#define THUMB_MAX_OPERANDS 4

// Thumb instructions are a mixture of 32bit wide and 16bit narrow
typedef union {
    uint32_t wide;
    uint16_t narrow;
    uint16_t as_halfwords[2];
} thumb_t;

enum thumb_mnemonic {
    TM_UNKNOWN,

    TM_B,
    TM_BKPT,
    TM_BL,
    TM_BLX,
    TM_BX,
    TM_LDR,
    TM_NOP,
    TM_MOVW,
    TM_POP,
    TM_PUSH,
    TM_SVC,
    TM_UDF,
};

#ifdef THUMB_ASM_SOURCE
static const char *mnemonic_strs[] = {
    "UNKNOWN",

    "B",
    "BKPT",
    "BL",
    "BLX",
    "BX",
    "LDR",
    "NOP",
    "MOVW",
    "POP",
    "PUSH",
    "SVC",
    "UDF",
};
#endif

enum thumb_operand_addressing_mode {
    AM_NONE, // TODO: Need to check this is set when encoding instructions that don't address
    AM_OFFSET,
    AM_PREINDEX,
    AM_POSTINDEX
};

struct thumb_operand {
    enum thumb_operand_type {
        OT_NONE,
        OT_REG,
        OT_IMMEDIATE,
        OT_SIGNED_IMMEDIATE,
        OT_LSL_SHIFT,
    } type;
    union {
        unsigned reg;
        unsigned imm;
        int simm;
        unsigned shift;
    };
};

enum thumb_width_specifier {
    TWS_AUTO,
    TWS_NARROW,
    TWS_WIDE,
};

struct thumb_instruction_spec {
    enum thumb_mnemonic mnemonic;
    enum thumb_width_specifier width;
    enum thumb_operand_addressing_mode addressing_mode;
    unsigned operand_count; // count of non empty operands
    struct thumb_operand operands[THUMB_MAX_OPERANDS];
};

bool thumb_is_wide_instruction(thumb_t ins);
thumb_t *thumb_ins_ptr_increment(thumb_t *insptr);
void thumb_print_register(unsigned reg);

void thumb_add_operand_reg(struct thumb_instruction_spec *instruction, unsigned reg);
void thumb_add_operand_immediate(struct thumb_instruction_spec *instruction, unsigned imm);
void thumb_add_operand_signed_immediate(struct thumb_instruction_spec *instruction, int simm);
void thumb_set_operand_addressing_mode(struct thumb_instruction_spec *instruction,
                                       enum thumb_operand_addressing_mode addressing_mode);
void thumb_add_operand_lslshift(struct thumb_instruction_spec *instruction, unsigned shift);
struct thumb_instruction_spec thumb_disassemble(const thumb_t *insptr);
void thumb_print_instruction(const struct thumb_instruction_spec *instruction,
                             void *address_of_instruction);

struct thumb_referenced_address_result {
    bool found;
    void *address;
};
struct thumb_referenced_address_result
thumb_get_referenced_address(const struct thumb_instruction_spec *instruction,
                             void *address_of_instruction);

enum thumb_assemble_result {
    AR_FAIL_BAD_MNEMONIC,
    AR_FAIL_INVALID_WIDTH,
    AR_FAIL_INVALID_OPERAND,

    AR_NARROW_SUCCESS = 0x10,
    AR_WIDE_SUCCESS,
};
enum thumb_assemble_result thumb_assemble(thumb_t *into, const struct thumb_instruction_spec *instruction_spec);

#endif
