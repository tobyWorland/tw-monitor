#include "assert.h"
#include "hardware.h"

#ifndef HOST
void assert(bool flag) {
    if (!flag) {
        if (g_hardware_inited) {
            putstring("ASSERT\r\n");
        }
        while (1) __asm("bkpt 99");
    }
}
#endif
