#include "stm32f411xce_peripherals.h"

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

// TODO: IRQ
struct peripheral g_periph_usart1 = {
    .name = "USART1",
    .base = (void*)0x40011000,
    .rcc = {
        .bus = PERIPH_BUS_APB2,
        .rcc_index = 4,
    }
};

// TODO: IRQ
struct peripheral g_periph_usart2 = {
    .name = "USART2",
    .base = (void*)0x40004400,
    .rcc = {
        .bus = PERIPH_BUS_APB1,
        .rcc_index = 17,
    }
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
