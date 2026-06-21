#ifndef HARDWARE_HARDWARE_H_INCLUDED
#define HARDWARE_HARDWARE_H_INCLUDED

#include <stdint.h>

// hardware_init

extern uint8_t g_hardware_inited;

void usart2_putchar(char c);
void usart2_putstring(const char *string);
void usart2_putnewline(void);
char usart2_getchar(void);

void usart1_putchar(char c);
char usart1_getchar(void);

#endif
