/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .set    SCB_VTOR,    0xE000ED08 // Vector Table Offset Register

        .global startup_entry
        .type   startup_entry, %function
startup_entry:
        // Setup stack // TODO: No need to do if started from flash due to vector table
        ldr     r0,     =stack_initial_top
        mov     sp,     r0

        // Set VTOR to vector table in ram
        ldr     r0,     =vector_table
        ldr     r1,     =SCB_VTOR
        str     r0,     [r1]

        // TODO: Copy .data if started from flash

        // Clear .bss
        ldr     r0,     =start_bss
        ldr     r1,     =end_bss // R1 holds the number of words to zero out
                                 // (linker script ensures end-start will be in whole worlds)
        sub     r1,     r0
        asr     r1,     2 // div 4
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

        .set    EXCEPTION_FRAME_OFF_R0,   0
        .set    EXCEPTION_FRAME_OFF_R1,   4
        .set    EXCEPTION_FRAME_OFF_R2,   8
        .set    EXCEPTION_FRAME_OFF_R3,   12
        .set    EXCEPTION_FRAME_OFF_R12,  16
        .set    EXCEPTION_FRAME_OFF_LR,   20
        .set    EXCEPTION_FRAME_OFF_PC,   24
        .set    EXCEPTION_FRAME_OFF_xPSR, 28

        .set    xPSR_THUMB_BIT,           (1 << 24)

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

        // Print registers
        pop     {lr}
        ldr     r12,    =registers
        ldmia   sp,     {r0-r3}
        stm     r12!,   {r0-r3}
        stm     r12!,   {r4-r11}
        ldr     r0,     [sp, EXCEPTION_FRAME_OFF_R12]
        str     r0,     [r12],  4
        mov     r0,     sp
        str     r0,     [r12],  4
        ldr     r0,     [sp, EXCEPTION_FRAME_OFF_LR]
        str     r0,     [r12],  4
        ldr     r0,     [sp, EXCEPTION_FRAME_OFF_PC]
        str     r0,     [r12],  4
        push    {lr}
        bl      print_registers

        pop     {lr}
        ands    r0,     lr,     0x0F
        cmp     r0,     0x0D
        beq     . // Hang if PSP is used // TODO: Handle PSP

        // Modify the exception frame so we return from the hardfault into monitor_main(1)
        adds    r0,     sp,     EXCEPTION_FRAME_OFF_PC
        ldr     r1,     =monitor_main
        str     r1,     [r0]
        adds    r0,     sp,     EXCEPTION_FRAME_OFF_R0
        movs    r1,     1 // Supress init - Don't reset addr, clear screen
        str     r1,     [r0]

        // Reset program status register
        //   Mostly for if the thumb bit gets cleared
        adds    r0,     sp,     EXCEPTION_FRAME_OFF_xPSR
        ldr     r1,     =xPSR_THUMB_BIT
        str     r1,     [r0]

        bx      lr

print_registers:
        push    {r4, r5, lr}
        ldr     r4,     =registers
        mov     r5,     0

1:
        cmp     r5,     16
        beq     2f

        mov     r0,     'R'
        bl      putchar
        mov     r0,     r5
        bl      putbyte
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

        .data
str_hardfault:  .asciz "\r\n\r\n***HARDFAULT***\r\n"

        .balign 4
registers:
        .space  4*16 // R0-R15
