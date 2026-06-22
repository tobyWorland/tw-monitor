/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .global hardware_init
        .type   hardware_init, %function
hardware_init:
        push    {lr}

        // TODO: Move this
        ldr     r0,     =g_hardware_inited
        movs    r1,     1
        strb    r1,     [r0]

        pop     {pc}

        .bss
        .global g_hardware_inited
g_hardware_inited:        .byte   0
