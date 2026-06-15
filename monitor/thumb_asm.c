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

void thumb_print_register(unsigned reg, bool pad_named_registers) {
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
        if (!pad_named_registers) {
            name++; // Skip string to after padding space
        }
        putstring(name);
    }
}

void thumb_print_register_list(uint16_t reglist) {
    bool first = true;
    putchar('{');
    // TODO: Should be able to print ranges e.g. {r0-r3} instead of {r0, r1, r2, r3}

    for (unsigned i = 0, mask = 1; i < 16; i++, mask <<= 1) {
        if (reglist & mask) {
            if (first) {
                first = false;
            } else {
                putstring(", ");
            }
            thumb_print_register(i, false);
        }
    }

    putchar('}');
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

void thumb_add_operand_signed_immediate(struct thumb_instruction_spec *instruction, int simm) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_SIGNED_IMMEDIATE,
        .simm = simm,
    };
}

void thumb_set_operand_addressing_mode(struct thumb_instruction_spec *instruction,
                                       enum thumb_operand_addressing_mode addressing_mode) {
    instruction->addressing_mode = addressing_mode;
}

void thumb_add_operand_lslshift(struct thumb_instruction_spec *instruction, unsigned shift) {
    *new_operand(instruction) = (struct thumb_operand){
        .type = OT_LSL_SHIFT,
        .shift = shift,
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
            thumb_add_operand_signed_immediate(&instruction, parts.simm25 + 4);
        } else if (match_ldr_i_t3(wide_ins)) {
            const struct ldr_i_t3_parts parts = decode_ldr_i_t3(wide_ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, parts.Rn);
            thumb_add_operand_immediate(&instruction, parts.imm12);
        } else if (match_ldr_i_t4(wide_ins)) {
            const struct ldr_i_t4_parts parts = decode_ldr_i_t4(wide_ins);

            instruction.mnemonic = TM_LDR;
            if (parts.index && parts.writeback) {
                instruction.addressing_mode = AM_PREINDEX;
            } else if (!parts.index && parts.writeback) {
                instruction.addressing_mode = AM_POSTINDEX;
            } else if (parts.index && !parts.writeback) {
                instruction.addressing_mode = AM_OFFSET;
            } else {
                // TODO: Handle undefined case when !index && !writeback
                ASSERT_NOT_REACHED();
            }
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, parts.Rn);
            thumb_add_operand_signed_immediate(&instruction, parts.imm8 * (parts.add ? 1 : -1));
        } else if (match_ldr_lit_t2(wide_ins)) {
            const struct ldr_lit_t2_parts parts = decode_ldr_lit_t2(wide_ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, 15); // PC
            thumb_add_operand_signed_immediate(&instruction, parts.imm12 * (parts.add ? 1 : -1));
        } else if (match_ldr_r_t2_lsl(wide_ins)) {
            const struct ldr_r_t2_lsl_parts parts = decode_ldr_r_t2_lsl(wide_ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, parts.Rn);
            thumb_add_operand_reg(&instruction, parts.Rm);
            thumb_add_operand_lslshift(&instruction, parts.lsl_shift_imm2);
        } else if (match_udf_t2(wide_ins)) {
            const struct udf_t2_parts parts = decode_udf_t2(wide_ins);

            instruction.mnemonic = TM_UDF;
            thumb_add_operand_immediate(&instruction, parts.imm16);
        } else if (match_bl_t1(wide_ins)) {
            const struct bl_t1_parts parts = decode_bl_t1(wide_ins);

            instruction.mnemonic = TM_BL;
            // +4 as immediate is relative to PC - check assembling comment
            thumb_add_operand_signed_immediate(&instruction, parts.simm25 + 4);
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
            thumb_add_operand_signed_immediate(&instruction, parts.simm11 + 4);
        } else if (match_ldr_i_t1(ins)) {
            const struct ldr_i_t1_parts parts = decode_ldr_i_t1(ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, parts.Rn);
            thumb_add_operand_immediate(&instruction, parts.imm7);
        } else if (match_ldr_i_t2_sponly(ins)) {
            const struct ldr_i_t2_sponly_parts parts = decode_ldr_i_t2_sponly(ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, 13); // SP
            thumb_add_operand_immediate(&instruction, parts.imm10);
        } else if (match_ldr_lit_t1(ins)) {
            const struct ldr_lit_t1_parts parts = decode_ldr_lit_t1(ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, 15); // PC
            thumb_add_operand_immediate(&instruction, parts.imm10);
        } else if (match_ldr_r_t1(ins)) {
            const struct ldr_r_t1_parts parts = decode_ldr_r_t1(ins);

            instruction.mnemonic = TM_LDR;
            instruction.addressing_mode = AM_OFFSET;
            thumb_add_operand_reg(&instruction, parts.Rt);
            thumb_add_operand_reg(&instruction, parts.Rn);
            thumb_add_operand_reg(&instruction, parts.Rm);
        } else if (match_udf_t1(ins)) {
            const struct udf_t1_parts parts = decode_udf_t1(ins);

            instruction.mnemonic = TM_UDF;
            thumb_add_operand_immediate(&instruction, parts.imm8);
        } else if (match_blx_t1(ins)) {
            const struct blx_t1_parts parts = decode_blx_t1(ins);

            instruction.mnemonic = TM_BLX;
            thumb_add_operand_reg(&instruction, parts.Rm);
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
        if (i == 1 && instruction->addressing_mode != AM_NONE) {
            putstring(" [");
        } else if (instruction->addressing_mode == AM_POSTINDEX &&
            i == instruction->operand_count - 1) {
            putstring("], ");
        } else {
            putchar(' ');
        }

        switch (instruction->operands[i].type) {
        case OT_REG:
            thumb_print_register(instruction->operands[i].reg, false);
            break;
        case OT_IMMEDIATE:
            putstring(utoa_pad(instruction->operands[i].imm, 16));
            break;
        case OT_SIGNED_IMMEDIATE:
            putstring(itoa_pad(instruction->operands[i].simm, 16));
            break;
        case OT_LSL_SHIFT:
            io_printf("LSL %u", instruction->operands[i].shift);
            break;
        default:
            ASSERT_NOT_REACHED();
        }
    }

    if (instruction->addressing_mode == AM_OFFSET) {
        putchar(']');
    } else if (instruction->addressing_mode == AM_PREINDEX) {
        putstring("]!");
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
        // TODO: Add BL
        // TODO: Add LDR
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
    switch (instruction_spec->mnemonic) { // TODO: None of these cases should break, instead they should return AR_FAIL_INVALID_WIDTH
    case TM_B: {
        // TODO: Support conditionals

        if (instruction_spec->operand_count != 1 || instruction_spec->operands[0].type != OT_IMMEDIATE) {
            return AR_FAIL_INVALID_OPERAND;
        }

        // -4 as immediate is relative to PC
        // And regardless of the width of instruction, the hardware behaves
        // as if the PC is 4 bytes ahead
        int32_t label = (int32_t)instruction_spec->operands[0].imm - 4; // TODO: Use signed immediate

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
    case TM_BL: {
        struct bl_t1_parts parts;

        ENSURE_WIDE();

        if (instruction_spec->operand_count == 1 &&
            instruction_spec->operands[0].type == OT_IMMEDIATE) {

            // -4 as immediate is relative to PC
            // And regardless of the width of instruction, the hardware behaves
            // as if the PC is 4 bytes ahead
            int32_t label = (int32_t)instruction_spec->operands[0].imm - 4; // TODO: Use signed immediate

            parts.simm25 = label;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_bl_t1(&into->wide, &parts));
    }
    case TM_BLX: {
        struct blx_t1_parts parts;

        ENSURE_NARROW();

        if (instruction_spec->operand_count == 1 && instruction_spec->operands[0].type == OT_REG) {
            parts.Rm = instruction_spec->operands[0].reg;
        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        return encoder_to_asm_result(encode_blx_t1(&into->narrow, &parts));
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
    case TM_LDR: {
        if (instruction_spec->operand_count < 3 || // 3 or 4 operands are valid
            instruction_spec->operand_count > 4 ||
            instruction_spec->addressing_mode == AM_NONE ||
            instruction_spec->operands[0].type != OT_REG ||
            instruction_spec->operands[1].type != OT_REG) {
            return AR_FAIL_INVALID_OPERAND;
        }

        uint8_t Rt = instruction_spec->operands[0].reg;
        uint8_t Rn = instruction_spec->operands[1].reg;
        bool can_be_narrow = (instruction_spec->width == TWS_AUTO ||
                       instruction_spec->width == TWS_NARROW);
        bool can_be_wide = (instruction_spec->width == TWS_AUTO ||
                            instruction_spec->width == TWS_WIDE);
        unsigned result;

        if (instruction_spec->operands[2].type == OT_REG) {
            unsigned lsl_shift = 0;

            if (instruction_spec->operand_count == 4) {
                if (instruction_spec->operands[3].type == OT_LSL_SHIFT) {
                    lsl_shift = instruction_spec->operands[3].shift;
                } else {
                    return AR_FAIL_INVALID_OPERAND;
                }
            }

            if (instruction_spec->operands[2].type != OT_REG ||
                instruction_spec->addressing_mode != AM_OFFSET) {
                return AR_FAIL_INVALID_OPERAND;
            }

            uint8_t Rm = instruction_spec->operands[2].reg;

            if (can_be_narrow && !lsl_shift) {
                struct ldr_r_t1_parts parts = {
                    .Rt = Rt,
                    .Rn = Rn,
                    .Rm = Rm
                };
                result = encode_ldr_r_t1(&into->narrow, &parts);
                if (result != 0) {
                    return encoder_to_asm_result(result);
                }
            }

            if (can_be_wide) {
                struct ldr_r_t2_lsl_parts parts = {
                    .Rt = Rt,
                    .Rn = Rn,
                    .Rm = Rm,
                    .lsl_shift_imm2 = lsl_shift,
                };
                return encoder_to_asm_result(encode_ldr_r_t2_lsl(&into->wide, &parts));
            }
        } else if (instruction_spec->operands[2].type == OT_SIGNED_IMMEDIATE) { // TODO: Support unsigned immediate?
            if (instruction_spec->operand_count != 3) {
                return AR_FAIL_INVALID_OPERAND;
            }

            int imm_offset = instruction_spec->operands[2].simm;
            bool neg_offset = imm_offset < 0;
            enum thumb_operand_addressing_mode addressing_mode = instruction_spec->addressing_mode;

            // These encodings only support base + unsigned immediate offset
            if (addressing_mode == AM_OFFSET && !neg_offset) {
                if (can_be_narrow) {
                    if (Rn == 13) { // SP specific encoding
                        struct ldr_i_t2_sponly_parts parts = {
                            .Rt = Rt,
                            .imm10 = imm_offset,
                        };
                        result = encode_ldr_i_t2_sponly(&into->narrow, &parts);
                        if (result != 0) {
                            return encoder_to_asm_result(result);
                        }
                    } else if (Rn == 15) { // PC specific encoding (for loading literals)
                        struct ldr_lit_t1_parts parts = {
                            .Rt = Rt,
                            .imm10 = imm_offset,
                        };
                        result = encode_ldr_lit_t1(&into->narrow, &parts);
                        if (result != 0) {
                            return encoder_to_asm_result(result);
                        }
                    }

                    struct ldr_i_t1_parts parts = {
                        .Rt = Rt,
                        .Rn = Rn,
                        .imm7 = imm_offset,
                    };
                    result = encode_ldr_i_t1(&into->narrow, &parts);
                    if (result != 0) {
                        return encoder_to_asm_result(result);
                    }
                }

                if (can_be_wide) {
                    struct ldr_i_t3_parts parts = {
                        .Rt = Rt,
                        .Rn = Rn,
                        .imm12 = imm_offset,
                    };
                    result = encode_ldr_i_t3(&into->wide, &parts);
                    if (result != 0) {
                        return encoder_to_asm_result(result);
                    }
                }
            }

            if (can_be_wide) {
                if (Rn == 15 && addressing_mode == AM_OFFSET) { // PC specific encoding (for loading literals)
                    struct ldr_lit_t2_parts parts = {
                        .Rt = Rt,

                        // Offset is unsigned, and made negative by setting `.add` to false
                        .imm12 = neg_offset ? -imm_offset : imm_offset,
                        .add = !neg_offset
                    };
                    result = encode_ldr_lit_t2(&into->wide, &parts);
                    if (result != 0) {
                        return encoder_to_asm_result(result);
                    }
                }

                struct ldr_i_t4_parts parts = {
                    .Rt = Rt,
                    .Rn = Rn,

                     // Offset is unsigned, and made negative by setting `.add` to false
                    .imm8 = neg_offset ? -imm_offset : imm_offset,
                    .add = !neg_offset,
                };

                if (addressing_mode == AM_PREINDEX) { // LDR Rt, [Rn, offset]!
                    parts.index = true;
                    parts.writeback = true;
                } else if (addressing_mode == AM_POSTINDEX) { // LDR Rt, [Rn], offset
                    parts.index = false;
                    parts.writeback = true;
                } else { // AM_OFFSET // LDR Rt, [Rn, offset]
                    parts.index = true;
                    parts.writeback = false;
                }

                return encoder_to_asm_result(encode_ldr_i_t4(&into->wide, &parts));
            }

        } else {
            return AR_FAIL_INVALID_OPERAND;
        }

        break;
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
        // TODO: POP
        // TODO: PUSH
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
    case TM_UDF: {
        if (instruction_spec->operand_count != 1 || instruction_spec->operands[0].type != OT_IMMEDIATE) {
            return AR_FAIL_INVALID_OPERAND;
        }

        if (instruction_spec->width == TWS_AUTO || instruction_spec->width == TWS_NARROW) {
            struct udf_t1_parts parts = {
                .imm8 = instruction_spec->operands[0].imm,
            };
            unsigned result = encode_udf_t1(&into->narrow, &parts);
            if (result != 0) {
                return encoder_to_asm_result(result);
            }
        }

        if (instruction_spec->width == TWS_AUTO || instruction_spec->width == TWS_WIDE) {
            struct udf_t2_parts parts = {
                .imm16 = instruction_spec->operands[0].imm,
            };
            return encoder_to_asm_result(encode_udf_t2(&into->wide, &parts));
        }
        break;
    }
    default:
        break; // Go on to fail
    }

    return AR_FAIL_BAD_MNEMONIC;
}
#undef ENSURE_NARROW
#undef ENSURE_WIDE
