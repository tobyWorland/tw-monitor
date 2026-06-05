#include "syscall.h"

#include "io.h"

void syscall_handler(uint8_t syscall_number) {
    io_printf("SYSCALL %x\r\n", syscall_number);
}
