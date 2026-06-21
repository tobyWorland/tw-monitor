#ifndef HARDWARE_STM32F411XCE_GPIO_H_INCLUDED
#define HARDWARE_STM32F411XCE_GPIO_H_INCLUDED

#include "../peripheral.h"

#include <stdbool.h>

void gpio_set_pin_alt_func(struct peripheral *gpio_periph, unsigned pin, unsigned altfunc_code);

#endif
