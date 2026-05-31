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

unsigned encode_bkpt_t1(uint16_t *out, struct bkpt_t1_parts parts) {
    uint16_t encoded = 0xbe00;
    if (parts.imm8 & ~((1ULL << 8) - 1)) {
        return 0;
    }
    encoded |= (parts.imm8 << 0);
    *out = encoded;
    return 16;
}
unsigned encode_bx_t1(uint16_t *out, struct bx_t1_parts parts) {
    uint16_t encoded = 0x4700;
    if (parts.Rm & ~((1ULL << 4) - 1)) {
        return 0;
    }
    encoded |= (parts.Rm << 3);
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

