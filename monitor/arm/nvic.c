#include "nvic.h"

#include "../assert.h"

#include <stdint.h>

// 240 is the max according the Cortex-M4 Devices Generic User Guide
#define NVIC_MAX_IRQ 240

#define NVIC_ISER0 (void *)0xE000E100
#define NVIC_IRQ_PER_ISER 32
#define NVIC_ISERX_COUNT 8

void nvic_disable_all_irqs(void) {
    volatile uint32_t *iser = NVIC_ISER0;

    for (int i = 0; i < NVIC_ISERX_COUNT; i++, iser++) {
        *iser = 0;
    }
}

void nvic_enable_irq(unsigned irq) {
    assert(irq < NVIC_MAX_IRQ);
    volatile uint32_t *iser = NVIC_ISER0;

    // Find the correct iser register and bit value
    // 32 per ISER, so IRQ 33 would be ISER1, BIT 1
    iser += irq / NVIC_IRQ_PER_ISER;
    irq %= NVIC_IRQ_PER_ISER;

    *iser |= (1U << irq);
}
