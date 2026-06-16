// AUTOGEN - DO NOT EDIT

#include "t32_encoding.h"

#define BIT(N) (1UL << (N))
#define BIT_MASK(WIDTH) (BIT(WIDTH)-1)
#define BIT_EXTRACT(VALUE, POSITION, WIDTH) (((VALUE) >> (POSITION)) & BIT_MASK(WIDTH))

#define UNSIGNED_CHECK_FIT(VALUE, WIDTH) !((VALUE) & ~BIT_MASK(WIDTH))
#define SIGNED_CHECK_FIT(VALUE, WIDTH) UNSIGNED_CHECK_FIT((((VALUE) < 0) ? (-(VALUE) - 1) : (VALUE)), WIDTH-1)

#define UNSIGNED_SHRINK_TO(VALUE, WIDTH) BIT_EXTRACT(VALUE, 0, WIDTH)
#define SIGNED_SHRINK_TO(VALUE, WIDTH) (BIT_EXTRACT(VALUE, 0, WIDTH-1) | ((VALUE < 0) * BIT((WIDTH)-1)))

#define SIGN_BIT_SET(VALUE, WIDTH) !!((VALUE) & BIT(WIDTH-1))
#define SIGNED_EXTEND(VALUE, OLD_WIDTH) (SIGN_BIT_SET(VALUE, OLD_WIDTH) ? (VALUE) | ~BIT_MASK(OLD_WIDTH) : (VALUE))

bool match_adds_i_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0xEU);
}
bool match_add_i_t2(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x6U);
}
bool match_addw_i_t4(uint32_t field) {
    return (BIT_EXTRACT(field, 31, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU) && \
        (BIT_EXTRACT(field, 4, 6) == 0x20U);
}
bool match_adds_r_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0xCU);
}
bool match_add_r_t2(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0x44U);
}
bool match_b_cond_t1_noit(uint16_t field) {
    return (BIT_EXTRACT(field, 12, 4) == 0xDU);
}
bool match_b_t2(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x1CU);
}
bool match_b_cond_t3_noit(uint32_t field) {
    return (BIT_EXTRACT(field, 30, 2) == 0x2U) && \
        (BIT_EXTRACT(field, 28, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU);
}
bool match_b_t4(uint32_t field) {
    return (BIT_EXTRACT(field, 30, 2) == 0x2U) && \
        (BIT_EXTRACT(field, 28, 1) == 0x1U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU);
}
bool match_bkpt_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0xBEU);
}
bool match_bl_t1(uint32_t field) {
    return (BIT_EXTRACT(field, 30, 2) == 0x3U) && \
        (BIT_EXTRACT(field, 28, 1) == 0x1U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU);
}
bool match_blx_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 7, 9) == 0x8FU) && \
        (BIT_EXTRACT(field, 0, 3) == 0x0U);
}
bool match_bx_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 7, 9) == 0x8EU) && \
        (BIT_EXTRACT(field, 0, 3) == 0x0U);
}
bool match_cmp_i_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x5U);
}
bool match_cmp_r_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 6, 10) == 0x10AU);
}
bool match_cmp_r_t2(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0x45U);
}
bool match_ldr_i_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0xDU);
}
bool match_ldr_i_t2_sponly(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x13U);
}
bool match_ldr_i_t3(uint32_t field) {
    return (BIT_EXTRACT(field, 4, 12) == 0xF8DU);
}
bool match_ldr_i_t4(uint32_t field) {
    return (BIT_EXTRACT(field, 27, 1) == 0x1U) && \
        (BIT_EXTRACT(field, 4, 12) == 0xF85U);
}
bool match_ldr_lit_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x9U);
}
bool match_ldr_lit_t2(uint32_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0xF8U) && \
        (BIT_EXTRACT(field, 0, 7) == 0x5FU);
}
bool match_ldr_r_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0x2CU);
}
bool match_ldr_r_t2_lsl(uint32_t field) {
    return (BIT_EXTRACT(field, 22, 6) == 0x0U) && \
        (BIT_EXTRACT(field, 4, 12) == 0xF85U);
}
bool match_nop_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 0, 16) == 0xBF00U);
}
bool match_nop_t2(uint32_t field) {
    return (BIT_EXTRACT(field, 16, 16) == 0x8000U) && \
        (BIT_EXTRACT(field, 0, 16) == 0xF3AFU);
}
bool match_movs_i_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x4U);
}
bool match_movw_i_t3(uint32_t field) {
    return (BIT_EXTRACT(field, 31, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU) && \
        (BIT_EXTRACT(field, 4, 6) == 0x24U);
}
bool match_mov_r_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0x46U);
}
bool match_movs_r_t2_noit(uint16_t field) {
    return (BIT_EXTRACT(field, 6, 10) == 0x0U);
}
bool match_mov_r_t3_opt_s(uint32_t field) {
    return (BIT_EXTRACT(field, 28, 4) == 0x0U) && \
        (BIT_EXTRACT(field, 20, 4) == 0x0U) && \
        (BIT_EXTRACT(field, 5, 11) == 0x752U) && \
        (BIT_EXTRACT(field, 0, 4) == 0xFU);
}
bool match_pop_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0x5EU);
}
bool match_pop_t2(uint32_t field) {
    return (BIT_EXTRACT(field, 29, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 0, 16) == 0xE8BDU);
}
bool match_pop_t3(uint32_t field) {
    return (BIT_EXTRACT(field, 16, 12) == 0xB04U) && \
        (BIT_EXTRACT(field, 0, 16) == 0xF85DU);
}
bool match_push_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0x5AU);
}
bool match_push_t2(uint32_t field) {
    return (BIT_EXTRACT(field, 31, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 29, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 0, 16) == 0xE92DU);
}
bool match_push_t3(uint32_t field) {
    return (BIT_EXTRACT(field, 16, 12) == 0xD04U) && \
        (BIT_EXTRACT(field, 0, 16) == 0xF84DU);
}
bool match_subs_i_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0xFU);
}
bool match_sub_i_t2(uint16_t field) {
    return (BIT_EXTRACT(field, 11, 5) == 0x7U);
}
bool match_subw_i_t4(uint32_t field) {
    return (BIT_EXTRACT(field, 31, 1) == 0x0U) && \
        (BIT_EXTRACT(field, 11, 5) == 0x1EU) && \
        (BIT_EXTRACT(field, 4, 6) == 0x2AU);
}
bool match_subs_r_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 9, 7) == 0xDU);
}
bool match_svc_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0xDFU);
}
bool match_udf_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 8, 8) == 0xDEU);
}
bool match_udf_t2(uint32_t field) {
    return (BIT_EXTRACT(field, 28, 4) == 0xAU) && \
        (BIT_EXTRACT(field, 4, 12) == 0xF7FU);
}

unsigned encode_adds_i_t1(uint16_t *out, struct adds_i_t1_parts *parts) {
    uint16_t encoded = 0x1c00;
    if (!UNSIGNED_CHECK_FIT(parts->imm3, 3)) {
        return 0;
    }
    encoded |= ((parts->imm3) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_add_i_t2(uint16_t *out, struct add_i_t2_parts *parts) {
    uint16_t encoded = 0x3000;
    if (!UNSIGNED_CHECK_FIT(parts->Rdn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rdn) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_addw_i_t4(uint32_t *out, struct addw_i_t4_parts *parts) {
    uint32_t encoded = 0xf200;
    if (!UNSIGNED_CHECK_FIT(parts->imm12 >> 0, 12)) {
        return 0;
    }
    uint32_t i = BIT_EXTRACT(parts->imm12, 11, 1);
    uint32_t imm3 = BIT_EXTRACT(parts->imm12, 8, 3);
    uint32_t imm8 = BIT_EXTRACT(parts->imm12, 0, 8);
    encoded |= ((imm3) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 4)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 24);
    encoded |= ((imm8) << 16);
    encoded |= ((i) << 10);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 4)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_adds_r_t1(uint16_t *out, struct adds_r_t1_parts *parts) {
    uint16_t encoded = 0x1800;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 3)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_add_r_t2(uint16_t *out, struct add_r_t2_parts *parts) {
    uint16_t encoded = 0x4400;
    if (!UNSIGNED_CHECK_FIT(parts->Rdn >> 0, 4)) {
        return 0;
    }
    uint32_t Rdn_3 = BIT_EXTRACT(parts->Rdn, 3, 1);
    uint32_t Rdn_2_0 = BIT_EXTRACT(parts->Rdn, 0, 3);
    encoded |= ((Rdn_3) << 7);
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    encoded |= ((Rdn_2_0) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_b_cond_t1_noit(uint16_t *out, struct b_cond_t1_noit_parts *parts) {
    uint16_t encoded = 0xd000;
    if (BIT_EXTRACT(parts->simm8, 0, 1)) {
        return 0;
    }
    if (!SIGNED_CHECK_FIT(parts->simm8 >> 1, 9)) {
        return 0;
    }
    uint32_t imm8 = BIT_EXTRACT(parts->simm8, 1, 8);
    if (!UNSIGNED_CHECK_FIT(parts->cond, 4)) {
        return 0;
    }
    encoded |= ((parts->cond) << 8);
    encoded |= ((imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_b_t2(uint16_t *out, struct b_t2_parts *parts) {
    uint16_t encoded = 0xe000;
    if (BIT_EXTRACT(parts->simm11, 0, 1)) {
        return 0;
    }
    if (!SIGNED_CHECK_FIT(parts->simm11 >> 1, 12)) {
        return 0;
    }
    uint32_t imm11 = BIT_EXTRACT(parts->simm11, 1, 11);
    encoded |= ((imm11) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_b_cond_t3_noit(uint32_t *out, struct b_cond_t3_noit_parts *parts) {
    uint32_t encoded = 0x8000f000;
    if (BIT_EXTRACT(parts->simm21, 0, 1)) {
        return 0;
    }
    if (!SIGNED_CHECK_FIT(parts->simm21 >> 1, 21)) {
        return 0;
    }
    uint32_t sign = BIT_EXTRACT(parts->simm21, 20, 1);
    uint32_t j2 = BIT_EXTRACT(parts->simm21, 19, 1);
    uint32_t j1 = BIT_EXTRACT(parts->simm21, 18, 1);
    uint32_t imm6 = BIT_EXTRACT(parts->simm21, 12, 6);
    uint32_t imm11 = BIT_EXTRACT(parts->simm21, 1, 11);
    encoded |= ((j1) << 29);
    encoded |= ((j2) << 27);
    encoded |= ((imm11) << 16);
    encoded |= ((sign) << 10);
    if (!UNSIGNED_CHECK_FIT(parts->cond, 4)) {
        return 0;
    }
    encoded |= ((parts->cond) << 6);
    encoded |= ((imm6) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_b_t4(uint32_t *out, struct b_t4_parts *parts) {
    uint32_t encoded = 0x9000f000;
    if (BIT_EXTRACT(parts->simm25, 0, 1)) {
        return 0;
    }
    if (!SIGNED_CHECK_FIT(parts->simm25 >> 1, 25)) {
        return 0;
    }
    uint32_t sign = BIT_EXTRACT(parts->simm25, 24, 1);
    uint32_t i1 = BIT_EXTRACT(parts->simm25, 23, 1);
    uint32_t i2 = BIT_EXTRACT(parts->simm25, 22, 1);
    uint32_t imm10 = BIT_EXTRACT(parts->simm25, 12, 10);
    uint32_t imm11 = BIT_EXTRACT(parts->simm25, 1, 11);
    encoded |= (((~i1 ^ sign) & BIT_MASK(1)) << 29);
    encoded |= (((~i2 ^ sign) & BIT_MASK(1)) << 27);
    encoded |= ((imm11) << 16);
    encoded |= ((sign) << 10);
    encoded |= ((imm10) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts *parts) {
    uint16_t encoded = 0xbe00;
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_bl_t1(uint32_t *out, struct bl_t1_parts *parts) {
    uint32_t encoded = 0xd000f000;
    if (BIT_EXTRACT(parts->simm25, 0, 1)) {
        return 0;
    }
    if (!SIGNED_CHECK_FIT(parts->simm25 >> 1, 25)) {
        return 0;
    }
    uint32_t sign = BIT_EXTRACT(parts->simm25, 24, 1);
    uint32_t i1 = BIT_EXTRACT(parts->simm25, 23, 1);
    uint32_t i2 = BIT_EXTRACT(parts->simm25, 22, 1);
    uint32_t imm10 = BIT_EXTRACT(parts->simm25, 12, 10);
    uint32_t imm11 = BIT_EXTRACT(parts->simm25, 1, 11);
    encoded |= (((~i1 ^ sign) & BIT_MASK(1)) << 29);
    encoded |= (((~i2 ^ sign) & BIT_MASK(1)) << 27);
    encoded |= ((imm11) << 16);
    encoded |= ((sign) << 10);
    encoded |= ((imm10) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_blx_t1(uint16_t *out, struct blx_t1_parts *parts) {
    uint16_t encoded = 0x4780;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    *out = encoded;
    return 16;
}
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts) {
    uint16_t encoded = 0x4700;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    *out = encoded;
    return 16;
}
unsigned encode_cmp_i_t1(uint16_t *out, struct cmp_i_t1_parts *parts) {
    uint16_t encoded = 0x2800;
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_cmp_r_t1(uint16_t *out, struct cmp_r_t1_parts *parts) {
    uint16_t encoded = 0x4280;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 3)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_cmp_r_t2(uint16_t *out, struct cmp_r_t2_parts *parts) {
    uint16_t encoded = 0x4500;
    if (!UNSIGNED_CHECK_FIT(parts->Rn >> 0, 4)) {
        return 0;
    }
    uint32_t Rn_3 = BIT_EXTRACT(parts->Rn, 3, 1);
    uint32_t Rn_2_0 = BIT_EXTRACT(parts->Rn, 0, 3);
    encoded |= ((Rn_3) << 7);
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    encoded |= ((Rn_2_0) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_ldr_i_t1(uint16_t *out, struct ldr_i_t1_parts *parts) {
    uint16_t encoded = 0x6800;
    if (BIT_EXTRACT(parts->imm7, 0, 2)) {
        return 0;
    }
    if (!UNSIGNED_CHECK_FIT(parts->imm7 >> 2, 7)) {
        return 0;
    }
    uint32_t _imm5 = BIT_EXTRACT(parts->imm7, 2, 5);
    encoded |= ((_imm5) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 3)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_ldr_i_t2_sponly(uint16_t *out, struct ldr_i_t2_sponly_parts *parts) {
    uint16_t encoded = 0x9800;
    if (BIT_EXTRACT(parts->imm10, 0, 2)) {
        return 0;
    }
    if (!UNSIGNED_CHECK_FIT(parts->imm10 >> 2, 10)) {
        return 0;
    }
    uint32_t _imm8 = BIT_EXTRACT(parts->imm10, 2, 8);
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 3)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 8);
    encoded |= ((_imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_ldr_i_t3(uint32_t *out, struct ldr_i_t3_parts *parts) {
    uint32_t encoded = 0xf8d0;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->imm12, 12)) {
        return 0;
    }
    encoded |= ((parts->imm12) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 4)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_ldr_i_t4(uint32_t *out, struct ldr_i_t4_parts *parts) {
    uint32_t encoded = 0x800f850;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->index, 1)) {
        return 0;
    }
    encoded |= ((parts->index) << 26);
    if (!UNSIGNED_CHECK_FIT(parts->add, 1)) {
        return 0;
    }
    encoded |= ((parts->add) << 25);
    if (!UNSIGNED_CHECK_FIT(parts->writeback, 1)) {
        return 0;
    }
    encoded |= ((parts->writeback) << 24);
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 4)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_ldr_lit_t1(uint16_t *out, struct ldr_lit_t1_parts *parts) {
    uint16_t encoded = 0x4800;
    if (BIT_EXTRACT(parts->imm10, 0, 2)) {
        return 0;
    }
    if (!UNSIGNED_CHECK_FIT(parts->imm10 >> 2, 10)) {
        return 0;
    }
    uint32_t _imm8 = BIT_EXTRACT(parts->imm10, 2, 8);
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 3)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 8);
    encoded |= ((_imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_ldr_lit_t2(uint32_t *out, struct ldr_lit_t2_parts *parts) {
    uint32_t encoded = 0xf85f;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->imm12, 12)) {
        return 0;
    }
    encoded |= ((parts->imm12) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->add, 1)) {
        return 0;
    }
    encoded |= ((parts->add) << 7);
    *out = encoded;
    return 32;
}
unsigned encode_ldr_r_t1(uint16_t *out, struct ldr_r_t1_parts *parts) {
    uint16_t encoded = 0x5800;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 3)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 3)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_ldr_r_t2_lsl(uint32_t *out, struct ldr_r_t2_lsl_parts *parts) {
    uint32_t encoded = 0xf850;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->lsl_shift_imm2, 2)) {
        return 0;
    }
    encoded |= ((parts->lsl_shift_imm2) << 20);
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 4)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_nop_t1(uint16_t *out) {
    uint16_t encoded = 0xbf00;
    *out = encoded;
    return 16;
}
unsigned encode_nop_t2(uint32_t *out) {
    uint32_t encoded = 0x8000f3af;
    *out = encoded;
    return 32;
}
unsigned encode_movs_i_t1(uint16_t *out, struct movs_i_t1_parts *parts) {
    uint16_t encoded = 0x2000;
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_movw_i_t3(uint32_t *out, struct movw_i_t3_parts *parts) {
    uint32_t encoded = 0xf240;
    if (!UNSIGNED_CHECK_FIT(parts->imm16 >> 0, 16)) {
        return 0;
    }
    uint32_t imm4 = BIT_EXTRACT(parts->imm16, 12, 4);
    uint32_t i = BIT_EXTRACT(parts->imm16, 11, 1);
    uint32_t imm3 = BIT_EXTRACT(parts->imm16, 8, 3);
    uint32_t imm8 = BIT_EXTRACT(parts->imm16, 0, 8);
    encoded |= ((imm3) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 4)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 24);
    encoded |= ((imm8) << 16);
    encoded |= ((i) << 10);
    encoded |= ((imm4) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_mov_r_t1(uint16_t *out, struct mov_r_t1_parts *parts) {
    uint16_t encoded = 0x4600;
    if (!UNSIGNED_CHECK_FIT(parts->Rd >> 0, 4)) {
        return 0;
    }
    uint32_t Rd_3 = BIT_EXTRACT(parts->Rd, 3, 1);
    uint32_t Rd_2_0 = BIT_EXTRACT(parts->Rd, 0, 3);
    encoded |= ((Rd_3) << 7);
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    encoded |= ((Rd_2_0) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_movs_r_t2_noit(uint16_t *out, struct movs_r_t2_noit_parts *parts) {
    uint16_t encoded = 0x0;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 3)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_mov_r_t3_opt_s(uint32_t *out, struct mov_r_t3_opt_s_parts *parts) {
    uint32_t encoded = 0xea4f;
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 4)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 24);
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->setflags, 1)) {
        return 0;
    }
    encoded |= ((parts->setflags) << 4);
    *out = encoded;
    return 32;
}
unsigned encode_pop_t1(uint16_t *out, struct pop_t1_parts *parts) {
    uint16_t encoded = 0xbc00;
    if (!UNSIGNED_CHECK_FIT(parts->pc, 1)) {
        return 0;
    }
    encoded |= ((parts->pc) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->regs, 8)) {
        return 0;
    }
    encoded |= ((parts->regs) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_pop_t2(uint32_t *out, struct pop_t2_parts *parts) {
    uint32_t encoded = 0xe8bd;
    if (!UNSIGNED_CHECK_FIT(parts->pc, 1)) {
        return 0;
    }
    encoded |= ((parts->pc) << 31);
    if (!UNSIGNED_CHECK_FIT(parts->lr, 1)) {
        return 0;
    }
    encoded |= ((parts->lr) << 30);
    if (!UNSIGNED_CHECK_FIT(parts->regs, 13)) {
        return 0;
    }
    encoded |= ((parts->regs) << 16);
    *out = encoded;
    return 32;
}
unsigned encode_pop_t3(uint32_t *out, struct pop_t3_parts *parts) {
    uint32_t encoded = 0xb04f85d;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    *out = encoded;
    return 32;
}
unsigned encode_push_t1(uint16_t *out, struct push_t1_parts *parts) {
    uint16_t encoded = 0xb400;
    if (!UNSIGNED_CHECK_FIT(parts->lr, 1)) {
        return 0;
    }
    encoded |= ((parts->lr) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->regs, 8)) {
        return 0;
    }
    encoded |= ((parts->regs) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_push_t2(uint32_t *out, struct push_t2_parts *parts) {
    uint32_t encoded = 0xe92d;
    if (!UNSIGNED_CHECK_FIT(parts->lr, 1)) {
        return 0;
    }
    encoded |= ((parts->lr) << 30);
    if (!UNSIGNED_CHECK_FIT(parts->regs, 13)) {
        return 0;
    }
    encoded |= ((parts->regs) << 16);
    *out = encoded;
    return 32;
}
unsigned encode_push_t3(uint32_t *out, struct push_t3_parts *parts) {
    uint32_t encoded = 0xd04f84d;
    if (!UNSIGNED_CHECK_FIT(parts->Rt, 4)) {
        return 0;
    }
    encoded |= ((parts->Rt) << 28);
    *out = encoded;
    return 32;
}
unsigned encode_subs_i_t1(uint16_t *out, struct subs_i_t1_parts *parts) {
    uint16_t encoded = 0x1e00;
    if (!UNSIGNED_CHECK_FIT(parts->imm3, 3)) {
        return 0;
    }
    encoded |= ((parts->imm3) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_sub_i_t2(uint16_t *out, struct sub_i_t2_parts *parts) {
    uint16_t encoded = 0x3800;
    if (!UNSIGNED_CHECK_FIT(parts->Rdn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rdn) << 8);
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_subw_i_t4(uint32_t *out, struct subw_i_t4_parts *parts) {
    uint32_t encoded = 0xf2a0;
    if (!UNSIGNED_CHECK_FIT(parts->imm12 >> 0, 12)) {
        return 0;
    }
    uint32_t i = BIT_EXTRACT(parts->imm12, 11, 1);
    uint32_t imm3 = BIT_EXTRACT(parts->imm12, 8, 3);
    uint32_t imm8 = BIT_EXTRACT(parts->imm12, 0, 8);
    encoded |= ((imm3) << 28);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 4)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 24);
    encoded |= ((imm8) << 16);
    encoded |= ((i) << 10);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 4)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 0);
    *out = encoded;
    return 32;
}
unsigned encode_subs_r_t1(uint16_t *out, struct subs_r_t1_parts *parts) {
    uint16_t encoded = 0x1a00;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 3)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 6);
    if (!UNSIGNED_CHECK_FIT(parts->Rn, 3)) {
        return 0;
    }
    encoded |= ((parts->Rn) << 3);
    if (!UNSIGNED_CHECK_FIT(parts->Rd, 3)) {
        return 0;
    }
    encoded |= ((parts->Rd) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_svc_t1(uint16_t *out, struct svc_t1_parts *parts) {
    uint16_t encoded = 0xdf00;
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_udf_t1(uint16_t *out, struct udf_t1_parts *parts) {
    uint16_t encoded = 0xde00;
    if (!UNSIGNED_CHECK_FIT(parts->imm8, 8)) {
        return 0;
    }
    encoded |= ((parts->imm8) << 0);
    *out = encoded;
    return 16;
}
unsigned encode_udf_t2(uint32_t *out, struct udf_t2_parts *parts) {
    uint32_t encoded = 0xa000f7f0;
    if (!UNSIGNED_CHECK_FIT(parts->imm16 >> 0, 16)) {
        return 0;
    }
    uint32_t imm4 = BIT_EXTRACT(parts->imm16, 12, 4);
    uint32_t imm12 = BIT_EXTRACT(parts->imm16, 0, 12);
    encoded |= ((imm12) << 16);
    encoded |= ((imm4) << 0);
    *out = encoded;
    return 32;
}

struct adds_i_t1_parts decode_adds_i_t1(uint16_t field) {
    struct adds_i_t1_parts result = {};
    result.imm3 = BIT_EXTRACT(field, 6, 3);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rd = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct add_i_t2_parts decode_add_i_t2(uint16_t field) {
    struct add_i_t2_parts result = {};
    result.Rdn = BIT_EXTRACT(field, 8, 3);
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct addw_i_t4_parts decode_addw_i_t4(uint32_t field) {
    struct addw_i_t4_parts result = {};
    uint32_t imm3 = BIT_EXTRACT(field, 28, 3);
    result.Rd = BIT_EXTRACT(field, 24, 4);
    uint32_t imm8 = BIT_EXTRACT(field, 16, 8);
    uint32_t i = BIT_EXTRACT(field, 10, 1);
    result.Rn = BIT_EXTRACT(field, 0, 4);
    uint32_t imm12 = 0;
    imm12 |= i << 11;
    imm12 |= imm3 << 8;
    imm12 |= imm8 << 0;
    result.imm12 = imm12;
    return result;
}
struct adds_r_t1_parts decode_adds_r_t1(uint16_t field) {
    struct adds_r_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 6, 3);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rd = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct add_r_t2_parts decode_add_r_t2(uint16_t field) {
    struct add_r_t2_parts result = {};
    uint32_t Rdn_3 = BIT_EXTRACT(field, 7, 1);
    result.Rm = BIT_EXTRACT(field, 3, 4);
    uint32_t Rdn_2_0 = BIT_EXTRACT(field, 0, 3);
    uint32_t Rdn = 0;
    Rdn |= Rdn_3 << 3;
    Rdn |= Rdn_2_0 << 0;
    result.Rdn = Rdn;
    return result;
}
struct b_cond_t1_noit_parts decode_b_cond_t1_noit(uint16_t field) {
    struct b_cond_t1_noit_parts result = {};
    result.cond = BIT_EXTRACT(field, 8, 4);
    uint32_t imm8 = BIT_EXTRACT(field, 0, 8);
    uint32_t simm8 = 0;
    simm8 |= imm8 << 0;
    result.simm8 = SIGNED_EXTEND(simm8, 8)<< 1;
    return result;
}
struct b_t2_parts decode_b_t2(uint16_t field) {
    struct b_t2_parts result = {};
    uint32_t imm11 = BIT_EXTRACT(field, 0, 11);
    uint32_t simm11 = 0;
    simm11 |= imm11 << 0;
    result.simm11 = SIGNED_EXTEND(simm11, 11)<< 1;
    return result;
}
struct b_cond_t3_noit_parts decode_b_cond_t3_noit(uint32_t field) {
    struct b_cond_t3_noit_parts result = {};
    uint32_t j1 = BIT_EXTRACT(field, 29, 1);
    uint32_t j2 = BIT_EXTRACT(field, 27, 1);
    uint32_t imm11 = BIT_EXTRACT(field, 16, 11);
    uint32_t sign = BIT_EXTRACT(field, 10, 1);
    result.cond = BIT_EXTRACT(field, 6, 4);
    uint32_t imm6 = BIT_EXTRACT(field, 0, 6);
    uint32_t simm21 = 0;
    simm21 |= sign << 19;
    simm21 |= j2 << 18;
    simm21 |= j1 << 17;
    simm21 |= imm6 << 11;
    simm21 |= imm11 << 0;
    result.simm21 = SIGNED_EXTEND(simm21, 20)<< 1;
    return result;
}
struct b_t4_parts decode_b_t4(uint32_t field) {
    struct b_t4_parts result = {};
    uint32_t imm11 = BIT_EXTRACT(field, 16, 11);
    uint32_t sign = BIT_EXTRACT(field, 10, 1);
    uint32_t imm10 = BIT_EXTRACT(field, 0, 10);
    uint32_t i2 = (~(BIT_EXTRACT(field, 27, 1) ^ sign)) & BIT_MASK(1);
    uint32_t i1 = (~(BIT_EXTRACT(field, 29, 1) ^ sign)) & BIT_MASK(1);
    uint32_t simm25 = 0;
    simm25 |= sign << 23;
    simm25 |= i1 << 22;
    simm25 |= i2 << 21;
    simm25 |= imm10 << 11;
    simm25 |= imm11 << 0;
    result.simm25 = SIGNED_EXTEND(simm25, 24)<< 1;
    return result;
}
struct bkpt_t1_parts decode_bkpt_t1(uint16_t field) {
    struct bkpt_t1_parts result = {};
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct bl_t1_parts decode_bl_t1(uint32_t field) {
    struct bl_t1_parts result = {};
    uint32_t imm11 = BIT_EXTRACT(field, 16, 11);
    uint32_t sign = BIT_EXTRACT(field, 10, 1);
    uint32_t imm10 = BIT_EXTRACT(field, 0, 10);
    uint32_t i2 = (~(BIT_EXTRACT(field, 27, 1) ^ sign)) & BIT_MASK(1);
    uint32_t i1 = (~(BIT_EXTRACT(field, 29, 1) ^ sign)) & BIT_MASK(1);
    uint32_t simm25 = 0;
    simm25 |= sign << 23;
    simm25 |= i1 << 22;
    simm25 |= i2 << 21;
    simm25 |= imm10 << 11;
    simm25 |= imm11 << 0;
    result.simm25 = SIGNED_EXTEND(simm25, 24)<< 1;
    return result;
}
struct blx_t1_parts decode_blx_t1(uint16_t field) {
    struct blx_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 3, 4);
    return result;
}
struct bx_t1_parts decode_bx_t1(uint16_t field) {
    struct bx_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 3, 4);
    return result;
}
struct cmp_i_t1_parts decode_cmp_i_t1(uint16_t field) {
    struct cmp_i_t1_parts result = {};
    result.Rn = BIT_EXTRACT(field, 8, 3);
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct cmp_r_t1_parts decode_cmp_r_t1(uint16_t field) {
    struct cmp_r_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 3, 3);
    result.Rn = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct cmp_r_t2_parts decode_cmp_r_t2(uint16_t field) {
    struct cmp_r_t2_parts result = {};
    uint32_t Rn_3 = BIT_EXTRACT(field, 7, 1);
    result.Rm = BIT_EXTRACT(field, 3, 4);
    uint32_t Rn_2_0 = BIT_EXTRACT(field, 0, 3);
    uint32_t Rn = 0;
    Rn |= Rn_3 << 3;
    Rn |= Rn_2_0 << 0;
    result.Rn = Rn;
    return result;
}
struct ldr_i_t1_parts decode_ldr_i_t1(uint16_t field) {
    struct ldr_i_t1_parts result = {};
    uint32_t _imm5 = BIT_EXTRACT(field, 6, 5);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rt = BIT_EXTRACT(field, 0, 3);
    uint32_t imm7 = 0;
    imm7 |= _imm5 << 0;
    result.imm7 = imm7<< 2;
    return result;
}
struct ldr_i_t2_sponly_parts decode_ldr_i_t2_sponly(uint16_t field) {
    struct ldr_i_t2_sponly_parts result = {};
    result.Rt = BIT_EXTRACT(field, 8, 3);
    uint32_t _imm8 = BIT_EXTRACT(field, 0, 8);
    uint32_t imm10 = 0;
    imm10 |= _imm8 << 0;
    result.imm10 = imm10<< 2;
    return result;
}
struct ldr_i_t3_parts decode_ldr_i_t3(uint32_t field) {
    struct ldr_i_t3_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    result.imm12 = BIT_EXTRACT(field, 16, 12);
    result.Rn = BIT_EXTRACT(field, 0, 4);
    return result;
}
struct ldr_i_t4_parts decode_ldr_i_t4(uint32_t field) {
    struct ldr_i_t4_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    result.index = BIT_EXTRACT(field, 26, 1);
    result.add = BIT_EXTRACT(field, 25, 1);
    result.writeback = BIT_EXTRACT(field, 24, 1);
    result.imm8 = BIT_EXTRACT(field, 16, 8);
    result.Rn = BIT_EXTRACT(field, 0, 4);
    return result;
}
struct ldr_lit_t1_parts decode_ldr_lit_t1(uint16_t field) {
    struct ldr_lit_t1_parts result = {};
    result.Rt = BIT_EXTRACT(field, 8, 3);
    uint32_t _imm8 = BIT_EXTRACT(field, 0, 8);
    uint32_t imm10 = 0;
    imm10 |= _imm8 << 0;
    result.imm10 = imm10<< 2;
    return result;
}
struct ldr_lit_t2_parts decode_ldr_lit_t2(uint32_t field) {
    struct ldr_lit_t2_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    result.imm12 = BIT_EXTRACT(field, 16, 12);
    result.add = BIT_EXTRACT(field, 7, 1);
    return result;
}
struct ldr_r_t1_parts decode_ldr_r_t1(uint16_t field) {
    struct ldr_r_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 6, 3);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rt = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct ldr_r_t2_lsl_parts decode_ldr_r_t2_lsl(uint32_t field) {
    struct ldr_r_t2_lsl_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    result.lsl_shift_imm2 = BIT_EXTRACT(field, 20, 2);
    result.Rm = BIT_EXTRACT(field, 16, 4);
    result.Rn = BIT_EXTRACT(field, 0, 4);
    return result;
}
struct movs_i_t1_parts decode_movs_i_t1(uint16_t field) {
    struct movs_i_t1_parts result = {};
    result.Rd = BIT_EXTRACT(field, 8, 3);
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct movw_i_t3_parts decode_movw_i_t3(uint32_t field) {
    struct movw_i_t3_parts result = {};
    uint32_t imm3 = BIT_EXTRACT(field, 28, 3);
    result.Rd = BIT_EXTRACT(field, 24, 4);
    uint32_t imm8 = BIT_EXTRACT(field, 16, 8);
    uint32_t i = BIT_EXTRACT(field, 10, 1);
    uint32_t imm4 = BIT_EXTRACT(field, 0, 4);
    uint32_t imm16 = 0;
    imm16 |= imm4 << 12;
    imm16 |= i << 11;
    imm16 |= imm3 << 8;
    imm16 |= imm8 << 0;
    result.imm16 = imm16;
    return result;
}
struct mov_r_t1_parts decode_mov_r_t1(uint16_t field) {
    struct mov_r_t1_parts result = {};
    uint32_t Rd_3 = BIT_EXTRACT(field, 7, 1);
    result.Rm = BIT_EXTRACT(field, 3, 4);
    uint32_t Rd_2_0 = BIT_EXTRACT(field, 0, 3);
    uint32_t Rd = 0;
    Rd |= Rd_3 << 3;
    Rd |= Rd_2_0 << 0;
    result.Rd = Rd;
    return result;
}
struct movs_r_t2_noit_parts decode_movs_r_t2_noit(uint16_t field) {
    struct movs_r_t2_noit_parts result = {};
    result.Rm = BIT_EXTRACT(field, 3, 3);
    result.Rd = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct mov_r_t3_opt_s_parts decode_mov_r_t3_opt_s(uint32_t field) {
    struct mov_r_t3_opt_s_parts result = {};
    result.Rd = BIT_EXTRACT(field, 24, 4);
    result.Rm = BIT_EXTRACT(field, 16, 4);
    result.setflags = BIT_EXTRACT(field, 4, 1);
    return result;
}
struct pop_t1_parts decode_pop_t1(uint16_t field) {
    struct pop_t1_parts result = {};
    result.pc = BIT_EXTRACT(field, 8, 1);
    result.regs = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct pop_t2_parts decode_pop_t2(uint32_t field) {
    struct pop_t2_parts result = {};
    result.pc = BIT_EXTRACT(field, 31, 1);
    result.lr = BIT_EXTRACT(field, 30, 1);
    result.regs = BIT_EXTRACT(field, 16, 13);
    return result;
}
struct pop_t3_parts decode_pop_t3(uint32_t field) {
    struct pop_t3_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    return result;
}
struct push_t1_parts decode_push_t1(uint16_t field) {
    struct push_t1_parts result = {};
    result.lr = BIT_EXTRACT(field, 8, 1);
    result.regs = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct push_t2_parts decode_push_t2(uint32_t field) {
    struct push_t2_parts result = {};
    result.lr = BIT_EXTRACT(field, 30, 1);
    result.regs = BIT_EXTRACT(field, 16, 13);
    return result;
}
struct push_t3_parts decode_push_t3(uint32_t field) {
    struct push_t3_parts result = {};
    result.Rt = BIT_EXTRACT(field, 28, 4);
    return result;
}
struct subs_i_t1_parts decode_subs_i_t1(uint16_t field) {
    struct subs_i_t1_parts result = {};
    result.imm3 = BIT_EXTRACT(field, 6, 3);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rd = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct sub_i_t2_parts decode_sub_i_t2(uint16_t field) {
    struct sub_i_t2_parts result = {};
    result.Rdn = BIT_EXTRACT(field, 8, 3);
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct subw_i_t4_parts decode_subw_i_t4(uint32_t field) {
    struct subw_i_t4_parts result = {};
    uint32_t imm3 = BIT_EXTRACT(field, 28, 3);
    result.Rd = BIT_EXTRACT(field, 24, 4);
    uint32_t imm8 = BIT_EXTRACT(field, 16, 8);
    uint32_t i = BIT_EXTRACT(field, 10, 1);
    result.Rn = BIT_EXTRACT(field, 0, 4);
    uint32_t imm12 = 0;
    imm12 |= i << 11;
    imm12 |= imm3 << 8;
    imm12 |= imm8 << 0;
    result.imm12 = imm12;
    return result;
}
struct subs_r_t1_parts decode_subs_r_t1(uint16_t field) {
    struct subs_r_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 6, 3);
    result.Rn = BIT_EXTRACT(field, 3, 3);
    result.Rd = BIT_EXTRACT(field, 0, 3);
    return result;
}
struct svc_t1_parts decode_svc_t1(uint16_t field) {
    struct svc_t1_parts result = {};
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct udf_t1_parts decode_udf_t1(uint16_t field) {
    struct udf_t1_parts result = {};
    result.imm8 = BIT_EXTRACT(field, 0, 8);
    return result;
}
struct udf_t2_parts decode_udf_t2(uint32_t field) {
    struct udf_t2_parts result = {};
    uint32_t imm12 = BIT_EXTRACT(field, 16, 12);
    uint32_t imm4 = BIT_EXTRACT(field, 0, 4);
    uint32_t imm16 = 0;
    imm16 |= imm4 << 12;
    imm16 |= imm12 << 0;
    result.imm16 = imm16;
    return result;
}

