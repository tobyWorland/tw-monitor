#ifndef HARDWARE_BOARD_H_INCLUDED
#define HARDWARE_BOARD_H_INCLUDED

#include <stdbool.h>

extern bool g_board_inited;

void board_init(void);

unsigned board_get_sysclock_MHz(void);

#endif
