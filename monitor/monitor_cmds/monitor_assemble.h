#ifndef MONITOR_ASSEMBLE_H_INCLUDED
#define MONITOR_ASSEMBLE_H_INCLUDED

#include "../memory.h"
#include "../thumb_asm.h"

// memory entry is optional and can be NULL
void monitor_assemble(thumb_t *addr, struct memory_entry *memory_entry);

#endif
