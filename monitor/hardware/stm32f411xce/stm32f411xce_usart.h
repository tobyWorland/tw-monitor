#ifndef HARDWARE_STM32F411XCE_USART_H_INCLUDED
#define HARDWARE_STM32F411XCE_USART_H_INCLUDED

#include "../peripheral.h"

#include <stdbool.h>

void usart_enable(struct peripheral *usart_periph, bool enable);
void usart_putbyte(struct peripheral *usart_periph, uint8_t b);
uint8_t usart_getbyte(struct peripheral *usart_periph);
void usart_enable_debug_break(struct peripheral *usart_periph, bool enable_break);
bool usart_get_polling(struct peripheral *usart_periph);
void usart_set_polling(struct peripheral *usart_periph, bool should_poll);

// TODO: Temporary
void usart2_putchar(char c);
void usart2_putstring(const char *string);
void usart2_putnewline(void);
char usart2_getchar(void);

void usart1_putchar(char c);
char usart1_getchar(void);
// END OF TEMPORARY

#endif
