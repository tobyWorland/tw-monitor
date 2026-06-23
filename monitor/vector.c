#include "vector.h"

#include "assert.h"

// Defined in vector.s
extern isr_t vector_table_irq_start[];

#define MAX_IRQ 85

void vector_set_isr_for(unsigned irq, isr_t isr) {
    assert(irq < MAX_IRQ);
    vector_table_irq_start[irq] = isr;
}
