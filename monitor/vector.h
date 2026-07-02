#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/**
 * @brief An Interrupt Service Routine (ISR)
 */
typedef void (*isr_t)(void);

/**
 * @brief Set interrupt handler for IRQ
 *
 * Updates the vector table stored in SRAM allowing interrupt handlers
 * to be changed at runtime.
 * @param[in] irq IRQ to update
 * @param[in] isr ISR to call when IRQ is triggered
 */
void vector_set_isr_for(unsigned irq, isr_t isr);

#endif
