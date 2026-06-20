#ifndef MONITOR_DISASSEMBLE_H_INCLUDED
#define MONITOR_DISASSEMBLE_H_INCLUDED

#include "../thumb_asm.h"

void monitor_disassemble_print_instruction_at(thumb_t *addr);
void monitor_disassemble(void *addr);

#endif
