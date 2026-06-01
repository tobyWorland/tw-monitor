#include "monitor_memdump.h"

#include "../io.h"

#include <stdint.h>

#ifndef HOST
void monitor_memdump(void *addr) {
    uint8_t *addr_as_byte_ptr = (void *)addr;

    for (int row = 0; row < 4; row++) {
        puthexword((uint32_t)addr_as_byte_ptr);
        for (int col = 0; col < 16; col++) {
            putchar(' ');
            putbyte(*addr_as_byte_ptr++);
        }
        putnewline();
    }
}
#endif
