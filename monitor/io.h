#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#ifndef HOST
/**
 * @brief Put character onto screen
 *
 * @param[in] c character to put on screen
 */
void putchar(char c);
/**
 * @brief Get character from user
 *
 * @returns character from user
 */
char getchar(void);
#else
#include <stdio.h>
#endif
/**
 * @brief Put \b string onto screen
 *
 * @param[in] string to put on screen
 */
void putstring(const char *string);
/**
 * @brief Put \b n chars from \b string onto screen
 *
 * @param[in] string to put on screen
 * @param[in] n number of characters to put
 */
void putnstring(const char *string, unsigned n);
/**
 * @brief Put newline onto screen
 */
void putnewline(void);
/**
 * @brief Put hex number out onto the screen
 *
 * Has a minimum number of digits to print
 * @param[in] digit_min minimum number of digits to print
 * @param[in] number to print as a hex number
 */
void puthexnumber(unsigned digit_min, uint32_t number);
/**
 * @brief Put half-word out in hex onto the screen
 *
 * @param[in] hword halfword to print as hex number
 */
void puthexhalfword(uint16_t hword);
/**
 * @brief Put word out in hex onto the screen
 *
 * @param[in] address word to print as hex number
 */
void puthexword(uint32_t address);
/**
 * @brief Put byte out in hex onto the screen
 *
 * @param[in] byte to print as hex number
 */
void putbyte(uint8_t byte);
/**
 * @brief Get a hex word from the user
 *
 * @returns Number received from user
 */
uint32_t gethexword(uint32_t default_value);
/**
 * @brief Basic printf implementatino
 *
 * @note Width, precision and fill not implemented (nor taking them as args with *)
 *
 * @param[in] fmt format string
 * @param[in] ... args for format string
 */
void io_printf(const char *fmt, ...);
/**
 * @brief Get a token from the user until a certain delimiter
 *
 * @param[in] delimiter that ends the token
 * @param[in] echo if user input should be echoed back (printed so the user can see)
 * @returns Token from user
 */
const char *io_gettill(char delimiter, bool echo);
/**
 * @brief Get a line from the user
 *
 * @returns Line entered by the user
 */
const char *io_getline(void);

#endif
