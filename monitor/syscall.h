#ifndef SYSCALL_H_INCLUDED
#define SYSCALL_H_INCLUDED

#include <stdint.h>

void syscall_handler(uint8_t syscall_number);

#endif
