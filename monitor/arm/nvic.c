#include "nvic.h"

#include "../assert.h"

#include <stdint.h>

void nvic_disable_all_irqs(void) {
    // TODO: Support all IRQs
    volatile uint32_t *NVIC_ISER0 = (void*)0xE000E100;
    *NVIC_ISER0 = 0;
}

void nvic_enable_irq(unsigned irq) {
    assert(irq < 32); // TODO: Support all IRQs
    volatile uint32_t *NVIC_ISER0 = (void*)0xE000E100;
    *NVIC_ISER0 |= (1U << irq);
}
