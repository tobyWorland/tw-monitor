#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdint.h>

#ifndef HOST
void putchar(char c);
char getchar(void);
#else
#include <stdio.h>
#endif
void putstring(const char *string);
void putnewline(void);
void puthexword(uint32_t address);
void putbyte(uint8_t byte);
uint32_t gethexword(void);

#endif
