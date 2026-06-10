#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

typedef void (*isr_t)(void);

// Defined in vector.s
extern volatile isr_t irq_tim1_up_tim10;

#endif
