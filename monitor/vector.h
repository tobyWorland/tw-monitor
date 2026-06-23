#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef void (*isr_t)(void);

void vector_set_isr_for(unsigned irq, isr_t isr);

#endif
