#ifndef NVIC_H_INCLUDED
#define NVIC_H_INCLUDED

void nvic_disable_all_irqs(void);
void nvic_enable_irq(unsigned irq);

#endif
