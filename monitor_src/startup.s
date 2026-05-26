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
        bl      monitor_main

        .global hang
        .type   hang, %function
hang:
        bkpt    90
        b       hang

        .global hardfault_handler
        .type   hardfault_handler, %function
hardfault_handler:
        b       .
