#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdint.h>

void putchar(char c);
void putstring(const char *string);
void putnewline(void);
char getchar(void);
void puthexword(uint32_t address);
void putbyte(uint8_t byte);
uint32_t gethexword(void);

#endif
