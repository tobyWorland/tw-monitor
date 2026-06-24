#ifndef HARDWARE_STM32F411XCE_TIMER_H_INCLUDED
#define HARDWARE_STM32F411XCE_TIMER_H_INCLUDED

#include "../peripheral.h"

#include <stdint.h>

void stm32f411xce_timer_sleep_init(struct peripheral *timer_periph);
void sleep(unsigned seconds);

#endif
