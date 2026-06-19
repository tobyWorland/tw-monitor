#ifndef SYSCALL_H_INCLUDED
#define SYSCALL_H_INCLUDED

#include <stdint.h>

struct syscall_frame {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
};

void syscall_handler(uint8_t syscall_number, struct syscall_frame *frame);

#endif
