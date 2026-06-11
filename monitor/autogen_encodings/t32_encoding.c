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
bool match_bx_t1(uint16_t field) {
    return (BIT_EXTRACT(field, 7, 9) == 0x8EU) && \
        (BIT_EXTRACT(field, 0, 3) == 0x0U);
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
    uint32_t s = BIT_EXTRACT(parts->simm21, 20, 1);
    uint32_t j2 = BIT_EXTRACT(parts->simm21, 19, 1);
    uint32_t j1 = BIT_EXTRACT(parts->simm21, 18, 1);
    uint32_t imm6 = BIT_EXTRACT(parts->simm21, 12, 6);
    uint32_t imm11 = BIT_EXTRACT(parts->simm21, 1, 11);
    encoded |= ((j1) << 29);
    encoded |= ((j2) << 27);
    encoded |= ((imm11) << 16);
    encoded |= ((s) << 10);
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
    uint32_t s = BIT_EXTRACT(parts->simm25, 24, 1);
    uint32_t i1 = BIT_EXTRACT(parts->simm25, 23, 1);
    uint32_t i2 = BIT_EXTRACT(parts->simm25, 22, 1);
    uint32_t imm10 = BIT_EXTRACT(parts->simm25, 12, 10);
    uint32_t imm11 = BIT_EXTRACT(parts->simm25, 1, 11);
    encoded |= (((~i1 ^ s) & BIT_MASK(1)) << 29);
    encoded |= (((~i2 ^ s) & BIT_MASK(1)) << 27);
    encoded |= ((imm11) << 16);
    encoded |= ((s) << 10);
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
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts) {
    uint16_t encoded = 0x4700;
    if (!UNSIGNED_CHECK_FIT(parts->Rm, 4)) {
        return 0;
    }
    encoded |= ((parts->Rm) << 3);
    *out = encoded;
    return 16;
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
    if (!UNSIGNED_CHECK_FIT(parts->imm12, 12)) {
        return 0;
    }
    encoded |= ((parts->imm12) << 16);
    if (!UNSIGNED_CHECK_FIT(parts->imm4, 4)) {
        return 0;
    }
    encoded |= ((parts->imm4) << 0);
    *out = encoded;
    return 32;
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
    uint32_t s = BIT_EXTRACT(field, 10, 1);
    result.cond = BIT_EXTRACT(field, 6, 4);
    uint32_t imm6 = BIT_EXTRACT(field, 0, 6);
    uint32_t simm21 = 0;
    simm21 |= s << 19;
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
    uint32_t s = BIT_EXTRACT(field, 10, 1);
    uint32_t imm10 = BIT_EXTRACT(field, 0, 10);
    uint32_t i2 = (~(BIT_EXTRACT(field, 27, 1) ^ s)) & BIT_MASK(1);
    uint32_t i1 = (~(BIT_EXTRACT(field, 29, 1) ^ s)) & BIT_MASK(1);
    uint32_t simm25 = 0;
    simm25 |= s << 23;
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
struct bx_t1_parts decode_bx_t1(uint16_t field) {
    struct bx_t1_parts result = {};
    result.Rm = BIT_EXTRACT(field, 3, 4);
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
    result.imm12 = BIT_EXTRACT(field, 16, 12);
    result.imm4 = BIT_EXTRACT(field, 0, 4);
    return result;
}

