/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .set    SCB_VTOR,    0xE000ED08 // Vector Table Offset Register

reset_stack:
        ldr     r0,     =stack_initial_top
        mov     sp,     r0
        bx      lr

        .global startup_entry
        .type   startup_entry, %function
startup_entry:
.ifdef STARTUP_FROM_SRAM
        // Setup stack
        bl      reset_stack

        // Set VTOR to vector table in ram
        ldr     r0,     =vector_table
        ldr     r1,     =SCB_VTOR
        str     r0,     [r1]
.else
        // Copy .data if started from flash
        ldr     r0,     =data_lma_start
        ldr     r1,     =data_vma_start
        ldr     r2,     =data_size_words
1:
        cbz     r2,     2f
        ldr     r3,     [r0],   4
        str     r3,     [r1],   4
        subs    r2,     1
        b       1b
2:
.endif

        // Clear .bss
        ldr     r0,     =bss_vma_start
        ldr     r1,     =bss_size_words
        movs    r2,     0
1:
        cbz     r1,     2f
        str     r2,     [r0],   4
        subs    r1,     1
        b       1b
2:

        bl      hardware_init

        movs    r0,     0
        bl      monitor_main

        .global hang
        .type   hang, %function
hang:
        bkpt    90
        b       hang

        .type   fault_exit, %function
fault_exit:
        bl      reset_stack
        movs    r0,     1 // Supress init - Don't reset addr or clear screen
        bl      monitor_main
        b       hang

        .set    EXCEPTION_FRAME_OFF_R0,   0
        .set    EXCEPTION_FRAME_OFF_R1,   4
        .set    EXCEPTION_FRAME_OFF_R2,   8
        .set    EXCEPTION_FRAME_OFF_R3,   12
        .set    EXCEPTION_FRAME_OFF_R12,  16
        .set    EXCEPTION_FRAME_OFF_LR,   20
        .set    EXCEPTION_FRAME_OFF_PC,   24
        .set    EXCEPTION_FRAME_OFF_xPSR, 28

        .set    xPSR_THUMB_BIT,           (1 << 24)
        .set    xPSR_IPSR_BIT_MASK,       0x1F

        .global _syscall
        .type _syscall, %function
_syscall:
        // Get address of SVC instruction using the PC
        ldr     r0,     [sp, EXCEPTION_FRAME_OFF_PC]
        ldrh    r0,     [r0, -2] // Go back 2 (size of SVC instruction) as the program counter has already moved on
        ands    r0,     0xFF // Pull the immediate from the SVC (least significant 8bits)
        b       syscall_handler

        .type   fault_exit_unwind_exceptions, %function
fault_exit_unwind_exceptions:
// TODO: If exception is an IRQ the associated periperhal should be disabled or reset?
        push    {lr}
        ldr     r0,     =str_exc_unwind
        bl      putstring
        mrs     r0,     xPSR
        ands    r0,     xPSR_IPSR_BIT_MASK
        mov     r1,     10
        bl      utoa_pad
        bl      putstring
        bl      putnewline
        pop     {lr}
        b       hardfault_handler.exit_fault
       
        .global hardfault_handler
        .type   hardfault_handler, %function
hardfault_handler:
        push    {lr}
        ldr     r0,     =g_hardware_inited
        ldr     r0,     [r0]

        cmp     r0,     0
        beq     . // Hang if usart isn't ready

        ldr     r0,     =str_hardfault
        bl      putstring

        // Print fault information
        bl      print_fault_state
        bl      clear_fault_state

        // Print EXC_RETURN
        ldr     r0,     =str_exc_return
        bl      putstring
        ldr     r0,     [sp]
        bl      puthexword
        bl      putnewline

        // Check exception frame is on the MSP
        pop     {lr}
        ands    r0,     lr,     0x0F
        cmp     r0,     0x0D
        beq     . // Hang if PSP is used // TODO: Handle PSP

        push    {lr}
        // Print xPSR
        ldr     r0,     =str_xPSR
        bl      putstring
        ldr     r0,     [sp, EXCEPTION_FRAME_OFF_xPSR + 4] // +4 to skip over EXC_RETURN on stack
        bl      puthexword
        bl      putnewline

        // Print registers
        bl      save_registers
        bl      print_registers
        pop     {lr}
        // Fallthrough

hardfault_handler.exit_fault:
        // Modify the exception frame so we return from the hardfault:
        // ...into fault exit if returning to thread mode
        // OR into fault_exit_unwind_exception if returning to handler mode
        adds    r0,     sp,     EXCEPTION_FRAME_OFF_PC
        ands    r1,     lr,     0xF
        cmp     r1,     1
        ite     eq
        ldreq   r2,     =fault_exit_unwind_exceptions
        ldrne   r2,     =fault_exit
        str     r2,     [r0]

        // Reset program status register
        //   Mostly for if the thumb bit gets cleared
        adds    r0,     sp,     EXCEPTION_FRAME_OFF_xPSR
        ldr     r2,     [r0]
        ands    r2,     xPSR_IPSR_BIT_MASK
        ldr     r1,     =xPSR_THUMB_BIT
        orr     r1,     r2
        str     r1,     [r0]

        bx      lr

        .global _debug_monitor
        .type   _debug_monitor, %function
_debug_monitor:
        // Check exception frame is on the MSP
        ands    r0,     lr,     0x0F
        cmp     r0,     0x0D
        beq     . // Hang if PSP is used // TODO: Handle PSP

        // Print registers
        push    {lr}
        bl      save_registers

        // Get program counter to pass to debug_monitor
        adds    r12,    sp,     4 // Skip over LR to exception frame
        ldr     r0,     [r12, EXCEPTION_FRAME_OFF_PC]
        bl      debug_monitor
        
        // Use the program counter from debug_monitor
        adds    r12,    sp,     4 // Skip over LR to exception frame
        str     r0,     [r12, EXCEPTION_FRAME_OFF_PC]

        pop     {pc}

        // INPUTS: SP = exception frame with the LR added on top
save_registers:
        ldr     r12,    =registers
        adds    r3,     sp,     4 // Skip over the LR put on the stack by the caller
        ldmia   r3,     {r0-r3}
        stm     r12!,   {r0-r3}
        stm     r12!,   {r4-r11}

        adds    r3,     sp,     4 // Skip over the LR put on the stack by the caller
        ldr     r0,     [r3, EXCEPTION_FRAME_OFF_R12]
        str     r0,     [r12],  4
        mov     r0,     r3
        str     r0,     [r12],  4
        ldr     r0,     [r3, EXCEPTION_FRAME_OFF_LR]
        str     r0,     [r12],  4
        ldr     r0,     [r3, EXCEPTION_FRAME_OFF_PC]
        str     r0,     [r12],  4
        bx      lr

        .global print_registers
        .type   print_registers, %function
print_registers:
        push    {r4, r5, lr}
        ldr     r4,     =registers
        mov     r5,     0

1:
        cmp     r5,     16
        beq     2f

        mov     r0,     r5
        movs    r1,     1
        bl      thumb_print_register
        mov     r0,     ' '
        bl      putchar
        ldr     r0,     [r4],   4
        bl      puthexword

        movs    r0,     ' '
        bl      putchar

        adds    r5,     1

        tst     r5,     3
        it      eq
        bleq    putnewline

        b       1b
2:
        pop     {r4, r5, pc}

        .section ".rodata", "a"
str_hardfault:  .asciz "\r\n\r\n***HARDFAULT***\r\n"
str_exc_return: .asciz "EXC_RET "
str_exc_unwind: .asciz "EXPCETION UNWIND "
str_xPSR:       .asciz "xPSR    "

        .data
        .balign 4
registers:
        .space  4*16 // R0-R15
