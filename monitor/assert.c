#include "assert.h"
#include "hardware/board.h"
#include "io.h"

#ifndef HOST
void assert(bool flag) {
    if (!flag) {
        if (g_board_inited) {
            putstring("ASSERT\r\n");
        }
        while (1)
            __asm("bkpt 99");
    }
}
#endif
