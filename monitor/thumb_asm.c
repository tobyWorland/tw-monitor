#include "memory.h"
#define THUMB_ASM_SOURCE
#include "thumb_asm.h"

#include "autogen_encodings/t32_encoding.h"
#include "assert.h"
#include "io.h"
#include "util.h"
#include "string.h"

bool thumb_is_wide_instruction(thumb_t ins) {
    // ARMv7-M Architecture Reference Manual
    // A5.3 32bit Thumb instruction Encoding
    return ((ins.narrow >> 13) == 7) &&
        (((ins.narrow >> 11) & 3) != 0);
}

thumb_t *thumb_ins_ptr_increment(thumb_t *insptr) {
    void *rawp = insptr;
    if (thumb_is_wide_instruction(*insptr)) {
        rawp += sizeof(insptr->wide);
    } else {
        rawp += sizeof(insptr->narrow);
    }
    return rawp;
}

void thumb_print_register(unsigned reg) {
    assert(reg <= 15);
    if (reg < 13) {
        putchar('R');
        putstring(utoa_pad_w(reg, 10, 2, true));
    } else {
        const char *name;
        switch (reg) {
        case 13:
            name = " SP";
            break;
        case 14:
            name = " LR";
            break;
        case 15:
            name = " PC";
            break;
        default:
            ASSERT_NOT_REACHED();
        }
        putstring(name);
    }
}

static struct thumb_operand *new_operand(struct thumb_instruction_spec *instruction) {
    assert(instruction->operand_count < THUMB_MAX_OPERANDS);
    return &instruction->operands[instruction->operand_count++];
}

void thumb_add_operand_reg(struct thumb_instruction_spec *instruction, unsigned reg) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_REG,
        .reg = reg,
    };
}

void thumb_add_operand_immediate(struct thumb_instruction_spec *instruction, unsigned imm) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_IMMEDIATE,
        .imm = imm,
    };
}

struct thumb_instruction_spec thumb_disassemble(const thumb_t *insptr) {
    struct thumb_instruction_spec instruction = {};

    if (thumb_is_wide_instruction(*insptr)) {
        uint32_t wide_ins = insptr->wide;
        instruction.width = TWS_WIDE;

        if (match_nop_t2(wide_ins)) {
            instruction.mnemonic = TM_NOP;
        } else if (match_movw_i_t3(wide_ins)) {
            const struct movw_i_t3_parts parts = decode_movw_i_t3(wide_ins);

            instruction.mnemonic = TM_MOVW;
            thumb_add_operand_reg(&instruction, parts.Rd);
            thumb_add_operand_immediate(&instruction, parts.imm16);
        } else if (match_b_t4(wide_ins)) {
            const struct b_t4_parts parts = decode_b_t4(wide_ins);

            instruction.mnemonic = TM_B;
            // +4 as immediate is relative to PC - check assembling comment
            thumb_add_operand_immediate(&instruction, parts.simm25 + 4);
        }
    } else {
        uint16_t ins = insptr->narrow;
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
        } else if (match_svc_t1(ins)) {
            const struct svc_t1_parts parts = decode_svc_t1(ins);

            instruction.mnemonic = TM_SVC;
            thumb_add_operand_immediate(&instruction, parts.imm8);
        } else if (match_b_t2(ins)) {
            const struct b_t2_parts parts = decode_b_t2(ins);

            instruction.mnemonic = TM_B;
            // +4 as immediate is relative to PC - check assembling comment
            thumb_add_operand_immediate(&instruction, parts.simm11 + 4);
        }
    }

    return instruction;
}

void thumb_print_instruction(const struct thumb_instruction_spec *instruction,
                             void *address_of_instruction) {
    putstring(mnemonic_strs[instruction->mnemonic]);
    if (instruction->width == TWS_WIDE) {
        putstring(".W");
    }
    for (unsigned i = 0; i < instruction->operand_count; i++) {
        putchar(' ');
        switch (instruction->operands[i].type) {
        case OT_REG:
            thumb_print_register(instruction->operands[i].reg);
            break;
        case OT_IMMEDIATE: // TODO: signed?
            putstring(utoa_pad(instruction->operands[i].reg, 16));
            break;
        default:
            ASSERT_NOT_REACHED();
        }
    }
    struct thumb_referenced_address_result refd_address_result =
        thumb_get_referenced_address(instruction, address_of_instruction);
    if (refd_address_result.found) {
        io_printf(" ; References %x", refd_address_result.address);

        struct memory_entry *label =
            memory_rlookup_label(refd_address_result.address);
        if (label) {
            putstring(" <");
            memory_print_name_from_label(label);
            putchar('>');
        }
    }
}

struct thumb_referenced_address_result
thumb_get_referenced_address(const struct thumb_instruction_spec *instruction,
                             void *address_of_instruction) {
    intptr_t address = 0;
    bool found = false;

    switch (instruction->mnemonic) {
    case TM_B:
        if (instruction->operand_count == 1 && instruction->operands[0].type == OT_IMMEDIATE) {
            found = true;
            address = (int)address_of_instruction + (int)instruction->operands[0].imm;
        }
        break;
    default:
        break;
    }

    return (struct thumb_referenced_address_result){found, (void*)address};
}

static enum thumb_assemble_result
encoder_to_asm_result(unsigned encoder_result) {
    switch (encoder_result) {
    case 32: return AR_WIDE_SUCCESS;
    case 16: return AR_NARROW_SUCCESS;
    default: return AR_FAIL_INVALID_OPERAND;
    }
}

#define ENSURE_NARROW() if (instruction_spec->width == TWS_WIDE)   return AR_FAIL_INVALID_WIDTH
#define ENSURE_WIDE()   if (instruction_spec->width == TWS_NARROW) return AR_FAIL_INVALID_WIDTH
enum thumb_assemble_result thumb_assemble(thumb_t *into, const struct thumb_instruction_spec *instruction_spec) {
    switch (instruction_spec->mnemonic) {
    case TM_B: {
        // TODO: Support conditionals

        if (instruction_spec->operand_count != 1 || instruction_spec->operands[0].type != OT_IMMEDIATE) {
            return AR_FAIL_INVALID_OPERAND;
        }

        // -4 as immediate is relative to PC
        // And regardless of the width of instruction, the hardware behaves
        // as if the PC is 4 bytes ahead
        int32_t label = (int32_t)instruction_spec->operands[0].imm - 4;

        if (instruction_spec->width == TWS_AUTO || instruction_spec->width == TWS_NARROW) {
            struct b_t2_parts parts = {
                .simm11 = label,
            };
            unsigned result = encode_b_t2(&into->narrow, &parts);
            if (result != 0) {
                return encoder_to_asm_result(result);
            }
        }

        if (instruction_spec->width == TWS_AUTO || instruction_spec->width == TWS_WIDE) {
            struct b_t4_parts parts = {
                .simm25 = label,
            };
            return encoder_to_asm_result(encode_b_t4(&into->wide, &parts));
        }
        break;
    }
    case TM_BKPT: {
        struct bkpt_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_IMMEDIATE) {
            parts.imm8 = instruction_spec->operands[0].imm;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_bkpt_t1(&into->narrow, &parts));
    }
    case TM_BX: {
        struct bx_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_REG) {
            parts.Rm = instruction_spec->operands[0].reg;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_bx_t1(&into->narrow, &parts));
    }
    case TM_NOP: {
        if (instruction_spec->operand_count != 0) {
            return AR_FAIL_INVALID_OPERAND;
        }

        unsigned enc_result;

        if (instruction_spec->width == TWS_WIDE) {
            enc_result = encode_nop_t2((void*)into);
        } else {
            enc_result = encode_nop_t1(&into->narrow);
        }

        return encoder_to_asm_result(enc_result);
    }
        // TODO: Support MOV
    case TM_MOVW: {
        struct movw_i_t3_parts parts;

        ENSURE_WIDE();

        if (instruction_spec->operand_count != 2 ||
            instruction_spec->operands[0].type != OT_REG ||
            instruction_spec->operands[1].type != OT_IMMEDIATE) {
            return AR_FAIL_INVALID_OPERAND;
        }

        parts.Rd = instruction_spec->operands[0].reg;
        parts.imm16 = instruction_spec->operands[1].imm;

        return encoder_to_asm_result(encode_movw_i_t3(&into->wide, &parts));
    }
    case TM_SVC: {
        struct svc_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_IMMEDIATE) {
            parts.imm8 = instruction_spec->operands[0].imm;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_svc_t1(&into->narrow, &parts));
    }
    default:
        break; // Go on to fail
    }

    return AR_FAIL_BAD_MNEMONIC;
}
#undef ENSURE_NARROW
#undef ENSURE_WIDE
