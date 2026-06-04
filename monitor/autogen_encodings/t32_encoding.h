#pragma once

#include <stdbool.h>
#include <stdint.h>

struct bkpt_t1_parts {
    uint8_t imm8;
};

struct bx_t1_parts {
    uint8_t Rm;
};

struct mov_t1_parts {
    uint8_t Rd;
    uint8_t imm8;
};

struct movw_t3_parts {
    uint8_t Rd;
    uint32_t imm16;
};

struct svc_t1_parts {
    uint8_t imm8;
};

bool match_bkpt_t1(uint16_t field);
bool match_bx_t1(uint16_t field);
bool match_nop_t1(uint16_t field);
bool match_nop_t2(uint32_t field);
bool match_mov_t1(uint16_t field);
bool match_movw_t3(uint32_t field);
bool match_svc_t1(uint16_t field);

unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts *parts);
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts);
unsigned encode_nop_t1(uint16_t *out);
unsigned encode_nop_t2(uint32_t *out);
unsigned encode_mov_t1(uint16_t *out, struct mov_t1_parts *parts);
unsigned encode_movw_t3(uint32_t *out, struct movw_t3_parts *parts);
unsigned encode_svc_t1(uint16_t *out, struct svc_t1_parts *parts);

struct bkpt_t1_parts decode_bkpt_t1(uint16_t field);
struct bx_t1_parts decode_bx_t1(uint16_t field);
struct mov_t1_parts decode_mov_t1(uint16_t field);
struct movw_t3_parts decode_movw_t3(uint32_t field);
struct svc_t1_parts decode_svc_t1(uint16_t field);

