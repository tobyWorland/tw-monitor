#include "stm32f411xce_timer.h"

#include "../../assert.h"
#include "../../io.h"
#include "../../util.h"
#include "../../vector.h"

// TODO: Update to use peripherals struct

#define TIM10_BASE 0x40014400
#define TIM11_BASE 0x40014800

#define TIM10_IRQ 25
#define TIM11_IRQ 26

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"
static volatile struct tim10x *const tim10 = (void*)TIM10_BASE;
static volatile struct tim10x *const tim11 = (void *)TIM11_BASE;
#pragma GCC diagnostic pop

// TODO: Should be a variable and should be used in hardware.s
#define MHz(x) (x * 1000000UL)
#define INPUT_CLK MHz(16)

static volatile bool in_sleep = false;

static void tim10_isr(void) {
    tim10->status = 0;
    in_sleep = false;
}

// TW: Put somewhere else
static void enable_irq(unsigned irq) {
    assert(irq < 32); // TODO: Support all IRQs
    volatile uint32_t *NVIC_ISER0 = (void*)0xE000E100;
    *NVIC_ISER0 |= (1U << irq);
}

void stm32f411xce_timer_sleep_init() {
    tim10->int_enable = TIM1X_DIER_UE;

    tim10->prescaler = INPUT_CLK / 1000;  // 1 ms per count
    tim10->auto_reload = 1000; // 1 second
    tim10->control = TIM1X_CR1_OPM | TIM1X_CR1_URS;

    tim10->event_gen = TIM1X_EGR_UG;
    tim10->status  = 0;

    irq_tim1_up_tim10 = tim10_isr;
    enable_irq(TIM10_IRQ);
}

// TODO: Adjust the reload value instead
void sleep(unsigned seconds) {
    while (seconds--) {
        in_sleep = true;
        tim10->control |= TIM1X_CR1_CEN;
        do {
            __asm("wfi");
        } while (in_sleep);
    }
}
