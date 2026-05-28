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
void puthexhalfword(uint16_t hword);
void puthexword(uint32_t address);
void putbyte(uint8_t byte);
uint32_t gethexword(uint32_t default_value);

#endif
