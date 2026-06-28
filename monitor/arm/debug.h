#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdbool.h>

bool arm_halting_debug_active(void);
void arm_enable_debug_monitor(void);
void arm_enable_debug_stepping(bool on);
void arm_pend_debug_monitor(void);
void arm_begin_stepping_at(void *address);

#endif
