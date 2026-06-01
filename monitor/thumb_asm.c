#include "thumb_asm.h"

#include "autogen_encodings/t32_encoding.h"
#include "assert.h"
#include "io.h"
#include "util.h"
#include "string.h"

bool thumb_is_wide_instruction(thumb_t ins) {
    // ARMv7-M Architecture Reference Manual
    // A5.3 32bit Thumb instruction Encoding
    return ((ins >> 13) == 7) && ((ins >> 11) != 0);
}

void thumb_print_register(unsigned reg) {
    assert(reg <= 15);
    if (reg < 13) {
        putchar('R');
        putstring(utoa_pad(reg, 10));
    } else {
        const char *name;
        switch (reg) {
        case 13:
            name = "SP";
            break;
        case 14:
            name = "LR";
            break;
        case 15:
            name = "PC";
            break;
        default:
            ASSERT_NOT_REACHED();
        }
        putstring(name);
    }
}

static const char *mnemonic_strs[] = {
    "UNKNOWN",

    "BKPT",
    "BX",
    "NOP",
};

static struct thumb_operand *new_operand(struct thumb_instruction *instruction) {
    assert(instruction->operand_count < THUMB_MAX_OPERANDS);
    return &instruction->operands[instruction->operand_count++];
}

void thumb_add_operand_reg(struct thumb_instruction *instruction, unsigned reg) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_REG,
        .imm = reg,
    };
}

void thumb_add_operand_immediate(struct thumb_instruction *instruction, unsigned imm) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_IMMEDIATE,
        .imm = imm,
    };
}

void thumb_print_disassembled_instruction(const thumb_t *insptr) {
    struct thumb_instruction instruction = {};

    if (thumb_is_wide_instruction(*insptr)) {
        uint32_t wide_ins = *(uint32_t*)insptr;
        instruction.width = TWS_WIDE;

        if (match_nop_t2(wide_ins)) {
            instruction.mnemonic = TM_NOP;
        }
    } else {
        uint16_t ins = *insptr;
        instruction.width = TWS_NARROW;

        if (match_bkpt_t1(ins)) {
            const struct bkpt_t1_parts parts = decode_bkpt_t1(ins);

            instruction.mnemonic = TM_BKPT;
            thumb_add_operand_immediate(&instruction, parts.imm8);
        } else if (match_nop_t1(ins)) {
            instruction.mnemonic = TM_NOP;
        } else if (match_bx_t1(ins)) {
            const struct bx_t1_parts parts = decode_bx_t1(ins);

            instruction.mnemonic = TM_BX;
            thumb_add_operand_reg(&instruction, parts.Rm);
        }
    }

    putchar(' ');
    putstring(mnemonic_strs[instruction.mnemonic]);
    if (instruction.width == TWS_WIDE) {
        putstring(".W");
    }
    for (unsigned i = 0; i < instruction.operand_count; i++) {
        putchar(' ');
        switch (instruction.operands[i].type) {
        case OT_REG:
            thumb_print_register(instruction.operands[i].reg);
            break;
        case OT_IMMEDIATE: // TODO: signed?
            putstring(utoa_pad(instruction.operands[i].reg, 16));
            break;
        default:
            ASSERT_NOT_REACHED();
        }
    }
}

static enum thumb_assemble_result
encoder_to_asm_result(unsigned encoder_result) {
    switch (encoder_result) {
    case 32: return AR_WIDE_SUCCESS;
    case 16: return AR_NARROW_SUCCESS;
    default: return AR_FAIL_INVALID_OPERAND;
    }
}

#define ENSURE_NARROW() if (instruction_spec->width == TWS_WIDE) return AR_FAIL_INVALID_WIDTH
enum thumb_assemble_result thumb_assemble(thumb_t *into, const struct thumb_instruction *instruction_spec) {
    switch (instruction_spec->mnemonic) {
    case TM_BKPT: {
        struct bkpt_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_IMMEDIATE) {
            parts.imm8 = instruction_spec->operands[0].imm;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_bkpt_t1(into, &parts));
    }
    case TM_BX: {
        struct bx_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_REG) {
            parts.Rm = instruction_spec->operands[0].reg;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_bx_t1(into, &parts));
    }
    case TM_NOP: {
        if (instruction_spec->operand_count != 0) {
            return AR_FAIL_INVALID_OPERAND;
        }

        unsigned enc_result;

        if (instruction_spec->width == TWS_WIDE) {
            enc_result = encode_nop_t2((void*)into);
        } else {
            enc_result = encode_nop_t1(into);
        }

        return encoder_to_asm_result(enc_result);
    }
    default:
        break; // Go on to fail
    }

    return AR_FAIL_BAD_MNEMONIC;
}
#undef ENSURE_NARROW
