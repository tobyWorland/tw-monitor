#ifndef NVIC_H_INCLUDED
#define NVIC_H_INCLUDED

#include <stdbool.h>

void nvic_disable_all_irqs(void);
void nvic_set_enable_irq(unsigned irq, bool enable);
void nvic_enable_irq(unsigned irq);
void nvic_disable_irq(unsigned irq);
void nvic_clear_irq(unsigned irq);

#endif
