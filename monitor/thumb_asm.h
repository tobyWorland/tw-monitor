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

enum thumb_condition {
    TC_NONE,

    // NOTE: Conditions are 1 off the actual values
    TC_EQ, // = 0
    TC_NE, // = 1
    TC_CS, // = 2
    TC_CC, // = 3
    TC_MI, // = 4
    TC_PL, // = 5
    TC_VS, // = 6
    TC_VC, // = 7
    TC_HI, // = 8
    TC_LS, // = 9
    TC_GE, // = 10
    TC_LT, // = 11
    TC_GT, // = 12
    TC_LE, // = 13
    TC_AL, // = 14
};

#ifdef THUMB_ASM_SOURCE
static const char *condition_strs[] = {
    "NONE", // TODO: Need to check this is set when encoding instructions that don't have a condition

    "EQ",
    "NE",
    "CS",
    "CC",
    "MI",
    "PL",
    "VS",
    "VC",
    "HI",
    "LS",
    "GE",
    "LT",
    "GT",
    "LE",
    "AL",
};
#endif

enum thumb_mnemonic {
    TM_UNKNOWN,

    TM_ADD,
    TM_ADDS,
    TM_ADDW,
    TM_B,
    TM_BKPT,
    TM_BL,
    TM_BLX,
    TM_BX,
    TM_CMP,
    TM_LDR,
    TM_NOP,
    TM_MOV,
    TM_MOVS,
    TM_MOVW,
    TM_POP,
    TM_PUSH,
    TM_SUB,
    TM_SUBS,
    TM_SUBW,
    TM_SVC,
    TM_UDF,
};

#ifdef THUMB_ASM_SOURCE
static const char *mnemonic_strs[] = {
    "UNKNOWN",

    "ADD",
    "ADDS",
    "ADDW",
    "B",
    "BKPT",
    "BL",
    "BLX",
    "BX",
    "CMP",
    "LDR",
    "NOP",
    "MOV",
    "MOVS",
    "MOVW",
    "POP",
    "PUSH",
    "SUB",
    "SUBS",
    "SUBW",
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
        OT_REGLIST,
        OT_IMMEDIATE,
        OT_SIGNED_IMMEDIATE,
        OT_LSL_SHIFT,
    } type;
    union {
        unsigned reg;
        uint16_t reglist;
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
    enum thumb_condition condition;
    enum thumb_width_specifier width;
    enum thumb_operand_addressing_mode addressing_mode;
    unsigned operand_count; // count of non empty operands
    struct thumb_operand operands[THUMB_MAX_OPERANDS];
};

bool thumb_is_wide_instruction(thumb_t ins);
thumb_t *thumb_ins_ptr_increment(thumb_t *insptr);
void thumb_print_register(unsigned reg, bool pad_named_registers);
void thumb_print_register_list(uint16_t reglist);

unsigned thumb_count_register_list(uint16_t reglist);

void thumb_add_operand_reg(struct thumb_instruction_spec *instruction, unsigned reg);
void thumb_add_operand_reglist(struct thumb_instruction_spec *instruction, uint16_t reglist);
void thumb_add_operand_immediate(struct thumb_instruction_spec *instruction, unsigned imm);
void thumb_add_operand_signed_immediate(struct thumb_instruction_spec *instruction, int simm);
void thumb_set_condition(struct thumb_instruction_spec *instruction, enum thumb_condition condition);
// TODO: Drop the "operand" from the name for addressing mode
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
