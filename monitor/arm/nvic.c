#include "nvic.h"

#include "../assert.h"

#include <stdint.h>

// 240 is the max according the Cortex-M4 Devices Generic User Guide
#define NVIC_MAX_IRQ 240

#define NVIC_ISER0 (void *)0xE000E100
#define NVIC_IRQ_PER_ISER 32
#define NVIC_ISERX_COUNT 8

#define NVIC_ICPR0 (void *)0xE000E280
#define NVIC_IRQ_PER_ICPR 32
#define NVIC_ICPRX_COUNT 8

void nvic_disable_all_irqs(void) {
    volatile uint32_t *iser = NVIC_ISER0;

    for (int i = 0; i < NVIC_ISERX_COUNT; i++, iser++) {
        *iser = 0;
    }
}

void nvic_set_enable_irq(unsigned irq, bool enable) {
    assert(irq < NVIC_MAX_IRQ);
    volatile uint32_t *iser = NVIC_ISER0;

    // Find the correct iser register and bit value
    // 32 per ISER, so IRQ 33 would be ISER1, BIT 1
    iser += irq / NVIC_IRQ_PER_ISER;
    irq %= NVIC_IRQ_PER_ISER;

    if (enable) {
        *iser |= (1U << irq);
    } else {
        *iser &= ~(1U << irq);
    }
}

void nvic_enable_irq(unsigned irq) {
    nvic_set_enable_irq(irq, true);
}

void nvic_disable_irq(unsigned irq) {
    nvic_set_enable_irq(irq, false);
}

void nvic_clear_irq(unsigned irq) {
    assert(irq < NVIC_MAX_IRQ);
    volatile uint32_t *icpr = NVIC_ICPR0;

    // Find the correct icpr register and bit value
    // 32 per ICPR, so IRQ 33 would be ICPR1, BIT 1
    icpr += irq / NVIC_IRQ_PER_ICPR;
    irq %= NVIC_IRQ_PER_ICPR;

    *icpr |= (1U << irq);
}
