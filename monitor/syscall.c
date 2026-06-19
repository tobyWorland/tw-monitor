#include "syscall.h"

#include "assert.h"
#include "io.h"

#define SYSCALL_TRACE 0

// Check docs/syscalls.md
void syscall_handler(uint8_t syscall_number, struct syscall_frame *frame) {
#if SYSCALL_TRACE
    io_printf("SYSCALL: #%u - R0: %x R1: %x R2: %x R3: %x\r\n", syscall_number,
              frame->r0, frame->r1, frame->r2, frame->r3);
#endif
    switch (syscall_number) {
    case 0: // Exit
        io_printf("Exit %u\r\n", frame->r0);
        ASSERT_NOT_REACHED(); // TODO: Implement a proper exit
    case 1: // Output decimal, new line
        io_printf("%d\r\n", frame->r0);
        break;
    default:
        io_printf("Invalid system call function #%u!\r\n", syscall_number);
        break;
    }
}
