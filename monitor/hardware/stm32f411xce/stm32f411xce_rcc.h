#ifndef HARDWARE_STM32F411XCE_RCC_H_INCLUDED
#define HARDWARE_STM32F411XCE_RCC_H_INCLUDED

#include "../peripheral.h"

#include <stdbool.h>

void rcc_disable_all_clocks(void);
void rcc_enable_clock(struct peripheral *periph, bool enable);

#endif
