#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

// Control characters are mapped from above null to below escape e.g. C-a is 1 and C-z is 26
#define CTRL(c)    (c & 0x1f)
#define IS_CTRL(c) (c < '\e')

bool char_isprint(char c);
char char_to_upper(char c);
char digit_to_char(uint8_t digit);
uint8_t char_to_digit(char c);

#endif
