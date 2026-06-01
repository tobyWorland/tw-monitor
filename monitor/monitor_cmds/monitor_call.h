#ifndef MONITOR_CALL_H_INCLUDED
#define MONITOR_CALL_H_INCLUDED

#include <stdbool.h>

void monitor_call_function(void (*fn)(), bool step);

#endif
