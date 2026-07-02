#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

/**
 *  @brief Get control code of character
 *
 *  Control characters are mapped from above null to below escape e.g. C-a is 1
 *  and C-z is 26
 *  @param[in] C Character to convert into control code
 *  @return Control + \b C code
 */
#define CTRL(C) (C & 0x1f)
/**
 *  @brief Check if char represent a control code
 *  @param[in] C Character code
 *  @return True if \b C is a control code, else False
 */
#define IS_CTRL(C) (C < '\e')

/**
 *  @brief Check if char is printable (e.g. not a control code like backspace or return)
 *
 *  Equivalent to standard library \a isprint()
 *  @param[in] c Character code
 *  @return True if \b c is in printable range, else False
 */
bool char_isprint(char c);
/**
 *  @brief Convert character to upper-case
 *
 *  Only applies to lower-case alphabetical characters (a-z), other characters are passed through as is.
 *  @param[in] c Character to make upper-case
 *  @return Upper-case character or \b c if not applicable
 */
char char_to_upper(char c);
/**
 *  @brief Convert digit to character
 *
 *  Supports up to base 16
 *  Reverse of @see char_to_digit
 *  Example: 2 -> '2'
 *  @param[in] digit to create character from
 *  @return Character matching the digit, or '\0' if invalid digit
 */
char digit_to_char(uint8_t digit);
/**
 *  @brief Interpret character as representing a digit
 *
 *  Supports up to base 16
 *  Reverse of @see digit_to_char
 *  Example: '2' -> 2
 *  @param[in] c character to make digit from
 *  @return Digit matching the character, or 0xFF if invalid character
 */
uint8_t char_to_digit(char c);

#endif
