#ifndef HARDWARE_STM32F411XCE_USART_H_INCLUDED
#define HARDWARE_STM32F411XCE_USART_H_INCLUDED

#include "../peripheral.h"

#include <stdbool.h>

void usart_enable(struct peripheral *usart_periph, bool enable);
void usart_putbyte(struct peripheral *usart_periph, uint8_t b);
uint8_t usart_getbyte(struct peripheral *usart_periph);

#endif
