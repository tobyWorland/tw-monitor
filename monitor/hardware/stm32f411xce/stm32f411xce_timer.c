#include "stm32f411xce_timer.h"

#include "stm32f411xce_rcc.h"
#include "../board.h"
#include "../../assert.h"
#include "../../arm/nvic.h"
#include "../../io.h"
#include "../../util.h"
#include "../../vector.h"

#include <stddef.h>

struct tim10x {
    uint32_t control;            // CR1
    uint32_t reserved1;          // RESERVED1
    uint32_t reserved2;          // RESERVED2
    uint32_t int_enable;         // DIER
    uint32_t status;             // SR
    uint32_t event_gen;          // EGR
    uint32_t capt_comp_mode1;    // CCMR1
    uint32_t reserved3;          // RESERVED3
    uint32_t capt_comp_enable1;  // CCER
    uint32_t count;              // CNT
    uint32_t prescaler;          // PSC
    uint32_t auto_reload;        // ARR
    uint32_t reserved4;          // RESERVED4
    uint32_t capt_comp1;         // CCR1
    uint32_t reserved5[6];       // reserved5[6]
    uint32_t option;             // OR
};

#define TIM1X_CR1_OPM   BIT(3)
#define TIM1X_CR1_URS   BIT(2)
#define TIM1X_CR1_CEN   BIT(0)

#define TIM1X_DIER_UE   BIT(0)

#define TIM1X_EGR_UG    BIT(0)

static volatile bool s_in_sleep = false;
static volatile struct tim10x *s_sleep_timer = NULL;

static void tim10_isr(void) {
    s_sleep_timer->status = 0;
    s_in_sleep = false;
}

void stm32f411xce_timer_sleep_init(struct peripheral *timer_periph) {
    if (s_sleep_timer) {
        // Disable old timer if set
        s_sleep_timer->control = 0;
        // TODO: Disable old timer clock in RCC
    }

    // Replace sleep timer with the one passed
    rcc_enable_clock(timer_periph, true);
    s_sleep_timer = timer_periph->base;

    s_sleep_timer->int_enable = TIM1X_DIER_UE;

    s_sleep_timer->prescaler = board_get_sysclock_MHz() / 1000;  // 1 ms per count
    s_sleep_timer->auto_reload = 1000; // 1 second
    s_sleep_timer->control = TIM1X_CR1_OPM | TIM1X_CR1_URS;

    s_sleep_timer->event_gen = TIM1X_EGR_UG;
    s_sleep_timer->status  = 0;

    vector_set_isr_for(timer_periph->irqs[0], tim10_isr);
    nvic_enable_irq(timer_periph->irqs[0]);
}

// TODO: Adjust the reload value instead
void sleep(unsigned seconds) {
    while (seconds--) {
        s_in_sleep = true;
        s_sleep_timer->control |= TIM1X_CR1_CEN;
        do {
            __asm("wfi");
        } while (s_in_sleep);
    }
}
