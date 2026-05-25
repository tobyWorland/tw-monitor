#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

bool char_isprint(char c);
char char_to_upper(char c);
char digit_to_char(uint8_t digit);
uint8_t char_to_digit(char c);

#endif
