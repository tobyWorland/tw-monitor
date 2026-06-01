#pragma once

#include <stdbool.h>
#include <stdint.h>

struct bkpt_t1_parts {
    uint8_t imm8;
};

struct bx_t1_parts {
    uint8_t Rm;
};

bool match_bkpt_t1(uint16_t field);
bool match_bx_t1(uint16_t field);
bool match_nop_t1(uint16_t field);
bool match_nop_t2(uint32_t field);

unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts *parts);
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts);
unsigned encode_nop_t1(uint16_t *out);
unsigned encode_nop_t2(uint32_t *out);

struct bkpt_t1_parts decode_bkpt_t1(uint16_t field);
struct bx_t1_parts decode_bx_t1(uint16_t field);

