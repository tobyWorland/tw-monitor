#include "../board.h"

#include "stm32f411xce_gpio.h"
#include "stm32f411xce_peripherals.h"
#include "stm32f411xce_rcc.h"
#include "stm32f411xce_usart.h"

void board_init(void) {
    rcc_enable_clock(&g_periph_gpio_a, true);
    rcc_enable_clock(&g_periph_gpio_b, true);
    rcc_enable_clock(&g_periph_usart1, true);
    rcc_enable_clock(&g_periph_usart2, true);
    rcc_enable_clock(&g_periph_tim10, true);

    // Set PA2 & PA3 to alternate function mode 7 (for USART2)
    gpio_set_pin_alt_func(&g_periph_gpio_a, 2, 7);
    gpio_set_pin_alt_func(&g_periph_gpio_a, 3, 7);

    // Set PB6 & PB7 to alternate function mode 7 (for USART1)
    gpio_set_pin_alt_func(&g_periph_gpio_b, 6, 7);
    gpio_set_pin_alt_func(&g_periph_gpio_b, 7, 7);

    usart_enable(&g_periph_usart1, true);
    usart_enable(&g_periph_usart2, true);
}
