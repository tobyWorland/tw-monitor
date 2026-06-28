#include "stm32f411xce_peripherals.h"

#include <stddef.h>

struct peripheral g_periph_gpio_a = {
    .name = "GPIO_A",
    .base = (void*)0x40020000,
    .rcc = {
        .bus = PERIPH_BUS_AHB1,
        .rcc_index = 0,
    }
};

struct peripheral g_periph_gpio_b = {
    .name = "GPIO_B",
    .base = (void*)0x40020400,
    .rcc = {
        .bus = PERIPH_BUS_AHB1,
        .rcc_index = 1,
    }
};

struct peripheral g_periph_usart1 = {
    .name = "USART1",
    .base = (void*)0x40011000,
    .rcc = {
        .bus = PERIPH_BUS_APB2,
        .rcc_index = 4,
    },
    .irqs = {37},
    .driver_idx = 1,
};

struct peripheral g_periph_usart2 = {
    .name = "USART2",
    .base = (void*)0x40004400,
    .rcc = {
        .bus = PERIPH_BUS_APB1,
        .rcc_index = 17,
    },
    .irqs = {38},
    .driver_idx = 2,
};

struct peripheral g_periph_tim10 = {
    .name = "TIM10",
    .base = (void*)0x40014400,
    .rcc = {
        .bus = PERIPH_BUS_APB2,
        .rcc_index = 17,
    },
    .irqs = {25}
};

struct peripheral g_periph_tim11 = {
    .name = "TIM11",
    .base = (void*)0x40014800,
    .rcc = {
        .bus = PERIPH_BUS_APB2,
        .rcc_index = 18,
    },
    .irqs = {26}
};

struct peripheral *peripheral_usart_periph_from_irq(unsigned irq) {
    // USART2 is first as it's more likely to be used
    if (g_periph_usart2.irqs[0] == irq) {
        return &g_periph_usart2;
    }

    if (g_periph_usart1.irqs[0] == irq) {
        return &g_periph_usart1;
    }

    return NULL;
}
