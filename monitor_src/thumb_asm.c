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

static void set_operand_reg(struct thumb_operand *operand, unsigned reg) {
    *operand = (struct thumb_operand){
        .type = OT_REG,
        .imm = reg,
    };
}

static void set_operand_immediate(struct thumb_operand *operand, unsigned imm) {
    *operand = (struct thumb_operand){
        .type = OT_IMMEDIATE,
        .imm = imm,
    };
}

// NOTE: Temporary
void thumb_print_disassembled_instruction(const thumb_t *insptr) {
    struct thumb_instruction instruction = {};

    if (thumb_is_wide_instruction(*insptr)) {
        uint32_t wide_ins = *(uint32_t*)insptr;
        instruction.wide = true;

        if (match_nop_t2(wide_ins)) {
            instruction.mnemonic = TM_NOP;
        }
    } else {
        uint16_t ins = *insptr;

        if (match_bkpt_t1(ins)) {
            const struct bkpt_t1_parts parts = decode_bkpt_t1(ins);

            instruction.mnemonic = TM_BKPT;
            set_operand_immediate(&instruction.operands[0], parts.imm8);
        } else if (match_nop_t1(ins)) {
            instruction.mnemonic = TM_NOP;
        } else if (match_bx_t1(ins)) {
            const struct bx_t1_parts parts = decode_bx_t1(ins);

            instruction.mnemonic = TM_BX;
            set_operand_reg(&instruction.operands[0], parts.Rm);
        }
    }

    putchar(' ');
    putstring(mnemonic_strs[instruction.mnemonic]);
    if (instruction.wide) {
        putstring(".W");
    }
    for (unsigned i = 0; i < ARR_LEN(instruction.operands); i++) {
        putchar(' ');
        if (instruction.operands[i].type != OT_NONE) {
            switch (instruction.operands[i].type) {
            case OT_REG:
                thumb_print_register(instruction.operands[i].reg);
                break;
            case OT_IMMEDIATE:
                // TODO: Print without padding
                puthexnumber(8, instruction.operands[i].reg);
                break;
            default:
                ASSERT_NOT_REACHED();
            }
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

#define ENSURE_NARROW() if (instruction_spec->wide) return AR_FAIL_INVALID_WIDTH
enum thumb_assemble_result thumb_assemble(thumb_t *into, const struct thumb_instruction *instruction_spec) {
    switch (instruction_spec->mnemonic) {
    case TM_BKPT: {
        struct bkpt_t1_parts parts;

        ENSURE_NARROW();

        // TODO: Check other operands are empty - should have a operands given count in the spec?
        if (instruction_spec->operands[0].type == OT_IMMEDIATE) {
            parts.imm8 = instruction_spec->operands[0].imm;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        // TODO: Should be a pointer to parts
        return encoder_to_asm_result(encode_bkpt_t1(into, parts));
    }
        // TODO: BX
        // TODO: NOP
    default:
        break; // Go on to fail
    }

    return AR_FAIL_BAD_MNEMONIC;
}
#undef ENSURE_NARROW
