/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .section ".vect", "awx"
        .global vector_table
vector_table:
        .word   stack_initial_top // Stack (flash only)
        .word   startup_entry     // Reset (flash only)
        .word   0                 // NMI
        .word   hardfault_handler // Hardfault
        .word   0                 // MemManage
        .word   0                 // BusFault
        .word   0                 // UsageFault
        .space  4*4               // Reserved
        .word   0                 // SVCall
        .word   _debug_monitor    // DebugMonitor
        .space  4                 // Reserved
        .word   0                 // PendSV
        .word   0                 // Systick
        // IRQs
        .space 4 * 85

        // TIM1 update interrupt and TIM10 global interrupt
        .global irq_tim1_up_tim10
        .set    irq_tim1_up_tim10,  (vector_table + 0xA4)
