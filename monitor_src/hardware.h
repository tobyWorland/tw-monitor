#ifndef HARDWARE_H_INCLUDED
#define HARDWARE_H_INCLUDED

#include <stdint.h>

// hardware_init

extern uint8_t g_hardware_inited;

void putchar(char c);
void putstring(const char *string);
void putnewline(void);
char getchar(void);

#endif
