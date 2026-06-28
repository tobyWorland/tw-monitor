#include "../board.h"

#include "stm32f411xce_gpio.h"
#include "stm32f411xce_peripherals.h"
#include "stm32f411xce_rcc.h"
#include "stm32f411xce_timer.h"
#include "stm32f411xce_usart.h"
#include "../../arm/nvic.h"
#include "../../memory.h"
#include "../../util.h"

bool g_board_inited = false;

void board_init(void) {
    nvic_disable_all_irqs();
    rcc_disable_all_clocks();

    // Some drivers may dynamically allocate
    memory_init();

    rcc_enable_clock(&g_periph_gpio_a, true);
    rcc_enable_clock(&g_periph_gpio_b, true);
    rcc_enable_clock(&g_periph_usart1, true);
    rcc_enable_clock(&g_periph_usart2, true);

    // Set PA2 & PA3 to alternate function mode 7 (for USART2)
    gpio_set_pin_alt_func(&g_periph_gpio_a, 2, 7);
    gpio_set_pin_alt_func(&g_periph_gpio_a, 3, 7);

    // Set PB6 & PB7 to alternate function mode 7 (for USART1)
    gpio_set_pin_alt_func(&g_periph_gpio_b, 6, 7);
    gpio_set_pin_alt_func(&g_periph_gpio_b, 7, 7);

    usart_enable(&g_periph_usart1, true);
    usart_enable(&g_periph_usart2, true);

    stm32f411xce_timer_sleep_init(&g_periph_tim10);

    g_board_inited = true;
}

unsigned board_get_sysclock_MHz(void) {
    // TODO: Should be pulled from RCC and be based on whatever is driving the sysclk

    // Default sysclk is RC oscillator @ 16MHz
    return MHz(16);
}

void board_enable_debug_user_break(bool enable) {
    usart_enable_debug_break(&g_periph_usart2, enable);
}
