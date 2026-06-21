#include "../board.h"

#include "stm32f411xce_peripherals.h"
#include "stm32f411xce_rcc.h"

void board_init(void) {
    rcc_enable_clock(&g_periph_gpio_a, true);
    rcc_enable_clock(&g_periph_gpio_b, true);
    rcc_enable_clock(&g_periph_usart1, true);
    rcc_enable_clock(&g_periph_usart2, true);
    rcc_enable_clock(&g_periph_tim10, true);
}
