#ifndef THUMB_ASM_H_INCLUDED
#define THUMB_ASM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

bool thumb_is_wide_instruction(uint16_t ins);

#endif
