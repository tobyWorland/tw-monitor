#ifndef HARDWARE_STM32F411XCE_PERIPHERAL_H_INCLUDED
#define HARDWARE_STM32F411XCE_PERIPHERAL_H_INCLUDED

#include "../peripheral.h"

extern struct peripheral g_periph_gpio_a;
extern struct peripheral g_periph_gpio_b;
extern struct peripheral g_periph_usart1;
extern struct peripheral g_periph_usart2;
extern struct peripheral g_periph_tim10;
// TODO: g_periph_tim11

struct peripheral *peripheral_usart_periph_from_irq(unsigned irq);

#endif
