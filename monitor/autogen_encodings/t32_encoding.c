#include "t32_encoding.h"

bool match_bkpt_t1(uint16_t field) {
    return (((field >> 8) & ((1ULL << 8) - 1)) == 190);
}
bool match_bx_t1(uint16_t field) {
    return (((field >> 7) & ((1ULL << 9) - 1)) == 142) && \
        (((field >> 0) & ((1ULL << 3) - 1)) == 0);
}
bool match_nop_t1(uint16_t field) {
    return (((field >> 0) & ((1ULL << 16) - 1)) == 48896);
}
bool match_nop_t2(uint32_t field) {
    return (((field >> 16) & ((1ULL << 16) - 1)) == 32768) && \
        (((field >> 0) & ((1ULL << 16) - 1)) == 62383);
}
bool match_mov_t1(uint16_t field) {
    return (((field >> 11) & ((1ULL << 5) - 1)) == 4);
}
bool match_movw_t3(uint32_t field) {
    return (((field >> 31) & ((1ULL << 1) - 1)) == 0) && \
        (((field >> 11) & ((1ULL << 5) - 1)) == 30) && \
        (((field >> 4) & ((1ULL << 6) - 1)) == 36);
}
bool match_svc_t1(uint16_t field) {
    return (((field >> 8) & ((1ULL << 8) - 1)) == 223);
}

unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts *parts) {
    uint16_t encoded = 0xbe00;
    if (parts->imm8 & ~((1ULL << 8) - 1)) {
        return 0;
    }
    encoded |= (parts->imm8 << 0);
    *out = encoded;
    return 16;
}
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts *parts) {
    uint16_t encoded = 0x4700;
    if (parts->Rm & ~((1ULL << 4) - 1)) {
        return 0;
    }
    encoded |= (parts->Rm << 3);
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
unsigned encode_mov_t1(uint16_t *out, struct mov_t1_parts *parts) {
    uint16_t encoded = 0x2000;
    if (parts->Rd & ~((1ULL << 3) - 1)) {
        return 0;
    }
    encoded |= (parts->Rd << 8);
    if (parts->imm8 & ~((1ULL << 8) - 1)) {
        return 0;
    }
    encoded |= (parts->imm8 << 0);
    *out = encoded;
    return 16;
}
unsigned encode_movw_t3(uint32_t *out, struct movw_t3_parts *parts) {
    uint32_t encoded = 0xf240;
    if (parts->imm16 & ~((1ULL << 16) - 1)) {
        return 0;
    }
    uint32_t imm4 = ((parts->imm16 >> 12) & ((1 << 4) - 1));
    uint32_t i = ((parts->imm16 >> 11) & ((1 << 1) - 1));
    uint32_t imm3 = ((parts->imm16 >> 8) & ((1 << 3) - 1));
    uint32_t imm8 = ((parts->imm16 >> 0) & ((1 << 8) - 1));
    encoded |= (imm3 << 28);
    if (parts->Rd & ~((1ULL << 4) - 1)) {
        return 0;
    }
    encoded |= (parts->Rd << 24);
    encoded |= (imm8 << 16);
    encoded |= (i << 10);
    encoded |= (imm4 << 0);
    *out = encoded;
    return 32;
}
unsigned encode_svc_t1(uint16_t *out, struct svc_t1_parts *parts) {
    uint16_t encoded = 0xdf00;
    if (parts->imm8 & ~((1ULL << 8) - 1)) {
        return 0;
    }
    encoded |= (parts->imm8 << 0);
    *out = encoded;
    return 16;
}

struct bkpt_t1_parts decode_bkpt_t1(uint16_t field) {
    struct bkpt_t1_parts result = {};
    result.imm8 = ((field >> 0) & ((1ULL << 8) - 1));
    return result;
}
struct bx_t1_parts decode_bx_t1(uint16_t field) {
    struct bx_t1_parts result = {};
    result.Rm = ((field >> 3) & ((1ULL << 4) - 1));
    return result;
}
struct mov_t1_parts decode_mov_t1(uint16_t field) {
    struct mov_t1_parts result = {};
    result.Rd = ((field >> 8) & ((1ULL << 3) - 1));
    result.imm8 = ((field >> 0) & ((1ULL << 8) - 1));
    return result;
}
struct movw_t3_parts decode_movw_t3(uint32_t field) {
    struct movw_t3_parts result = {};
    uint32_t imm3 = ((field >> 28) & ((1ULL << 3) - 1));
    result.Rd = ((field >> 24) & ((1ULL << 4) - 1));
    uint32_t imm8 = ((field >> 16) & ((1ULL << 8) - 1));
    uint32_t i = ((field >> 10) & ((1ULL << 1) - 1));
    uint32_t imm4 = ((field >> 0) & ((1ULL << 4) - 1));
    uint32_t imm16 = 0;
    imm16 |= imm4 << 12;
    imm16 |= i << 11;
    imm16 |= imm3 << 8;
    imm16 |= imm8 << 0;
    result.imm16 = imm16;
    return result;
}
struct svc_t1_parts decode_svc_t1(uint16_t field) {
    struct svc_t1_parts result = {};
    result.imm8 = ((field >> 0) & ((1ULL << 8) - 1));
    return result;
}

