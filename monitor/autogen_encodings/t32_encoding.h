// AUTOGEN - DO NOT EDIT

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct adds_i_t1_parts {
    uint32_t imm3;
    uint32_t Rn;
    uint32_t Rd;
};

struct add_i_t2_parts {
    uint32_t Rdn;
    uint32_t imm8;
};

struct addw_i_t4_parts {
    uint32_t Rd;
    uint32_t Rn;
    uint32_t imm12;
};

struct adds_r_t1_parts {
    uint32_t Rm;
    uint32_t Rn;
    uint32_t Rd;
};

struct add_r_t2_parts {
    uint32_t Rdn;
    uint32_t Rm;
};

struct b_cond_t1_noit_parts {
    uint32_t cond;
    int32_t simm8;
};

struct b_t2_parts {
    int32_t simm11;
};

struct b_cond_t3_noit_parts {
    uint32_t cond;
    int32_t simm21;
};

struct b_t4_parts {
    int32_t simm25;
};

struct bkpt_t1_parts {
    uint32_t imm8;
};

struct bl_t1_parts {
    int32_t simm25;
};

struct blx_t1_parts {
    uint32_t Rm;
};

struct bx_t1_parts {
    uint32_t Rm;
};

struct cmp_i_t1_parts {
    uint32_t Rn;
    uint32_t imm8;
};

struct cmp_r_t1_parts {
    uint32_t Rm;
    uint32_t Rn;
};

struct cmp_r_t2_parts {
    uint32_t Rn;
    uint32_t Rm;
};

struct ldr_i_t1_parts {
    uint32_t Rt;
    uint32_t Rn;
    uint32_t imm7;
};

struct ldr_i_t2_sponly_parts {
    uint32_t Rt;
    uint32_t imm10;
};

struct ldr_i_t3_parts {
    uint32_t Rt;
    uint32_t imm12;
    uint32_t Rn;
};

struct ldr_i_t4_parts {
    uint32_t Rt;
    uint32_t index;
    uint32_t add;
    uint32_t writeback;
    uint32_t imm8;
    uint32_t Rn;
};

struct ldr_lit_t1_parts {
    uint32_t Rt;
    uint32_t imm10;
};

struct ldr_lit_t2_parts {
    uint32_t Rt;
    uint32_t imm12;
    uint32_t add;
};

struct ldr_r_t1_parts {
    uint32_t Rm;
    uint32_t Rn;
    uint32_t Rt;
};

struct ldr_r_t2_lsl_parts {
    uint32_t Rt;
    uint32_t lsl_shift_imm2;
    uint32_t Rm;
    uint32_t Rn;
};

struct movs_i_t1_parts {
    uint32_t Rd;
    uint32_t imm8;
};

struct movw_i_t3_parts {
    uint32_t Rd;
    uint32_t imm16;
};

struct mov_r_t1_parts {
    uint32_t Rm;
    uint32_t Rd;
};

struct movs_r_t2_noit_parts {
    uint32_t Rm;
    uint32_t Rd;
};

struct mov_r_t3_opt_s_parts {
    uint32_t Rd;
    uint32_t Rm;
    uint32_t setflags;
};

struct pop_t1_parts {
    uint32_t pc;
    uint32_t regs;
};

struct pop_t2_parts {
    uint32_t pc;
    uint32_t lr;
    uint32_t regs;
};

struct pop_t3_parts {
    uint32_t Rt;
};

struct push_t1_parts {
    uint32_t lr;
    uint32_t regs;
};

struct push_t2_parts {
    uint32_t lr;
    uint32_t regs;
};

struct push_t3_parts {
    uint32_t Rt;
};

struct subs_i_t1_parts {
    uint32_t imm3;
    uint32_t Rn;
    uint32_t Rd;
};

struct sub_i_t2_parts {
    uint32_t Rdn;
    uint32_t imm8;
};

struct subw_i_t4_parts {
    uint32_t Rd;
    uint32_t Rn;
    uint32_t imm12;
};

struct subs_r_t1_parts {
    uint32_t Rm;
    uint32_t Rn;
    uint32_t Rd;
};

struct svc_t1_parts {
    uint32_t imm8;
};

struct udf_t1_parts {
    uint32_t imm8;
};

struct udf_t2_parts {
    uint32_t imm16;
};

bool match_adds_i_t1(uint16_t field);
bool match_add_i_t2(uint16_t field);
bool match_addw_i_t4(uint32_t field);
bool match_adds_r_t1(uint16_t field);
bool match_add_r_t2(uint16_t field);
bool match_b_cond_t1_noit(uint16_t field);
bool match_b_t2(uint16_t field);
bool match_b_cond_t3_noit(uint32_t field);
bool match_b_t4(uint32_t field);
bool match_bkpt_t1(uint16_t field);
bool match_bl_t1(uint32_t field);
bool match_blx_t1(uint16_t field);
bool match_bx_t1(uint16_t field);
bool match_cmp_i_t1(uint16_t field);
bool match_cmp_r_t1(uint16_t field);
bool match_cmp_r_t2(uint16_t field);
bool match_ldr_i_t1(uint16_t field);
bool match_ldr_i_t2_sponly(uint16_t field);
bool match_ldr_i_t3(uint32_t field);
bool match_ldr_i_t4(uint32_t field);
bool match_ldr_lit_t1(uint16_t field);
bool match_ldr_lit_t2(uint32_t field);
bool match_ldr_r_t1(uint16_t field);
bool match_ldr_r_t2_lsl(uint32_t field);
bool match_nop_t1(uint16_t field);
bool match_nop_t2(uint32_t field);
bool match_movs_i_t1(uint16_t field);
bool match_movw_i_t3(uint32_t field);
bool match_mov_r_t1(uint16_t field);
bool match_movs_r_t2_noit(uint16_t field);
bool match_mov_r_t3_opt_s(uint32_t field);
bool match_pop_t1(uint16_t field);
bool match_pop_t2(uint32_t field);
bool match_pop_t3(uint32_t field);
bool match_push_t1(uint16_t field);
bool match_push_t2(uint32_t field);
bool match_push_t3(uint32_t field);
bool match_subs_i_t1(uint16_t field);
bool match_sub_i_t2(uint16_t field);
bool match_subw_i_t4(uint32_t field);
bool match_subs_r_t1(uint16_t field);
bool match_svc_t1(uint16_t field);
bool match_udf_t1(uint16_t field);
bool match_udf_t2(uint32_t field);

unsigned encode_adds_i_t1(uint16_t *out, struct adds_i_t1_parts *parts);
unsigned encode_add_i_t2(uint16_t *out, struct add_i_t2_parts *parts);
unsigned encode_addw_i_t4(uint32_t *out, struct addw_i_t4_parts *parts);
unsigned encode_adds_r_t1(uint16_t *out, struct adds_r_t1_parts *parts);
unsigned encode_add_r_t2(uint16_t *out, struct add_r_t2_parts *parts);
unsigned encode_b_cond_t1_noit(uint16_t *out, struct b_cond_t1_noit_parts *parts);
unsigned encode_b_t2(uint16_t *out, struct b_t2_parts *parts);
unsigned encode_b_cond_t3_noit(uint32_t *out, struct b_cond_t3_noit_parts *parts);
unsigned encode_b_t4(uint32_t *out, struct b_t4_parts *parts);
unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts *parts);
unsigned encode_bl_t1(uint32_t *out, struct bl_t1_parts *parts);
unsigned encode_blx_t1(uint16_t *out, struct blx_t1_parts *parts);
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts);
unsigned encode_cmp_i_t1(uint16_t *out, struct cmp_i_t1_parts *parts);
unsigned encode_cmp_r_t1(uint16_t *out, struct cmp_r_t1_parts *parts);
unsigned encode_cmp_r_t2(uint16_t *out, struct cmp_r_t2_parts *parts);
unsigned encode_ldr_i_t1(uint16_t *out, struct ldr_i_t1_parts *parts);
unsigned encode_ldr_i_t2_sponly(uint16_t *out, struct ldr_i_t2_sponly_parts *parts);
unsigned encode_ldr_i_t3(uint32_t *out, struct ldr_i_t3_parts *parts);
unsigned encode_ldr_i_t4(uint32_t *out, struct ldr_i_t4_parts *parts);
unsigned encode_ldr_lit_t1(uint16_t *out, struct ldr_lit_t1_parts *parts);
unsigned encode_ldr_lit_t2(uint32_t *out, struct ldr_lit_t2_parts *parts);
unsigned encode_ldr_r_t1(uint16_t *out, struct ldr_r_t1_parts *parts);
unsigned encode_ldr_r_t2_lsl(uint32_t *out, struct ldr_r_t2_lsl_parts *parts);
unsigned encode_nop_t1(uint16_t *out);
unsigned encode_nop_t2(uint32_t *out);
unsigned encode_movs_i_t1(uint16_t *out, struct movs_i_t1_parts *parts);
unsigned encode_movw_i_t3(uint32_t *out, struct movw_i_t3_parts *parts);
unsigned encode_mov_r_t1(uint16_t *out, struct mov_r_t1_parts *parts);
unsigned encode_movs_r_t2_noit(uint16_t *out, struct movs_r_t2_noit_parts *parts);
unsigned encode_mov_r_t3_opt_s(uint32_t *out, struct mov_r_t3_opt_s_parts *parts);
unsigned encode_pop_t1(uint16_t *out, struct pop_t1_parts *parts);
unsigned encode_pop_t2(uint32_t *out, struct pop_t2_parts *parts);
unsigned encode_pop_t3(uint32_t *out, struct pop_t3_parts *parts);
unsigned encode_push_t1(uint16_t *out, struct push_t1_parts *parts);
unsigned encode_push_t2(uint32_t *out, struct push_t2_parts *parts);
unsigned encode_push_t3(uint32_t *out, struct push_t3_parts *parts);
unsigned encode_subs_i_t1(uint16_t *out, struct subs_i_t1_parts *parts);
unsigned encode_sub_i_t2(uint16_t *out, struct sub_i_t2_parts *parts);
unsigned encode_subw_i_t4(uint32_t *out, struct subw_i_t4_parts *parts);
unsigned encode_subs_r_t1(uint16_t *out, struct subs_r_t1_parts *parts);
unsigned encode_svc_t1(uint16_t *out, struct svc_t1_parts *parts);
unsigned encode_udf_t1(uint16_t *out, struct udf_t1_parts *parts);
unsigned encode_udf_t2(uint32_t *out, struct udf_t2_parts *parts);

struct adds_i_t1_parts decode_adds_i_t1(uint16_t field);
struct add_i_t2_parts decode_add_i_t2(uint16_t field);
struct addw_i_t4_parts decode_addw_i_t4(uint32_t field);
struct adds_r_t1_parts decode_adds_r_t1(uint16_t field);
struct add_r_t2_parts decode_add_r_t2(uint16_t field);
struct b_cond_t1_noit_parts decode_b_cond_t1_noit(uint16_t field);
struct b_t2_parts decode_b_t2(uint16_t field);
struct b_cond_t3_noit_parts decode_b_cond_t3_noit(uint32_t field);
struct b_t4_parts decode_b_t4(uint32_t field);
struct bkpt_t1_parts decode_bkpt_t1(uint16_t field);
struct bl_t1_parts decode_bl_t1(uint32_t field);
struct blx_t1_parts decode_blx_t1(uint16_t field);
struct bx_t1_parts decode_bx_t1(uint16_t field);
struct cmp_i_t1_parts decode_cmp_i_t1(uint16_t field);
struct cmp_r_t1_parts decode_cmp_r_t1(uint16_t field);
struct cmp_r_t2_parts decode_cmp_r_t2(uint16_t field);
struct ldr_i_t1_parts decode_ldr_i_t1(uint16_t field);
struct ldr_i_t2_sponly_parts decode_ldr_i_t2_sponly(uint16_t field);
struct ldr_i_t3_parts decode_ldr_i_t3(uint32_t field);
struct ldr_i_t4_parts decode_ldr_i_t4(uint32_t field);
struct ldr_lit_t1_parts decode_ldr_lit_t1(uint16_t field);
struct ldr_lit_t2_parts decode_ldr_lit_t2(uint32_t field);
struct ldr_r_t1_parts decode_ldr_r_t1(uint16_t field);
struct ldr_r_t2_lsl_parts decode_ldr_r_t2_lsl(uint32_t field);
struct movs_i_t1_parts decode_movs_i_t1(uint16_t field);
struct movw_i_t3_parts decode_movw_i_t3(uint32_t field);
struct mov_r_t1_parts decode_mov_r_t1(uint16_t field);
struct movs_r_t2_noit_parts decode_movs_r_t2_noit(uint16_t field);
struct mov_r_t3_opt_s_parts decode_mov_r_t3_opt_s(uint32_t field);
struct pop_t1_parts decode_pop_t1(uint16_t field);
struct pop_t2_parts decode_pop_t2(uint32_t field);
struct pop_t3_parts decode_pop_t3(uint32_t field);
struct push_t1_parts decode_push_t1(uint16_t field);
struct push_t2_parts decode_push_t2(uint32_t field);
struct push_t3_parts decode_push_t3(uint32_t field);
struct subs_i_t1_parts decode_subs_i_t1(uint16_t field);
struct sub_i_t2_parts decode_sub_i_t2(uint16_t field);
struct subw_i_t4_parts decode_subw_i_t4(uint32_t field);
struct subs_r_t1_parts decode_subs_r_t1(uint16_t field);
struct svc_t1_parts decode_svc_t1(uint16_t field);
struct udf_t1_parts decode_udf_t1(uint16_t field);
struct udf_t2_parts decode_udf_t2(uint32_t field);

