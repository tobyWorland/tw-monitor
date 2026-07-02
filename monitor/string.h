#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

// Expose pad_*() to the host for testing
#ifdef HOST
void pad_init();
void pad_hold(char c);
unsigned pad_len(void);
#endif

/**
 * @brief Convert unsigned number to string with minimum width
 *
 * @param[in] number to print
 * @param[in] base to print the number out in
 * @param[in] minwidth the minimum string length
 * @param[in] fillwithzero if \a true then fill the remaining width with zeros, \a else use spaces
 * @returns String containing the representation of the number
 * @warning Not reentrant
 */
const char *utoa_pad_w(unsigned number, unsigned base, unsigned minwidth, bool fillwithzero);
/**
 * @brief Convert signed number to string with minimum width
 *
 * @param[in] number to print
 * @param[in] base to print the number out in
 * @param[in] minwidth the minimum string length
 * @param[in] fillwithzero if \a true then fill the remaining width with zeros, \a else use spaces
 * @returns String containing the representation of the number
 * @warning Not reentrant
 */
const char *itoa_pad_w(int number, unsigned base, unsigned minwidth, bool fillwidthzero);

/**
 * @brief Convert unsigned number to string
 *
 * @param[in] number to print
 * @param[in] base to print the number out in
 * @returns String containing the representation of the number
 * @warning Not reentrant
 */
const char *utoa_pad(unsigned number, unsigned base);
/**
 * @brief Convert signed number to string
 *
 * @param[in] number to print
 * @param[in] base to print the number out in
 * @returns String containing the representation of the number
 * @warning Not reentrant
 */
const char *itoa_pad(int number, unsigned base);

/**
 * @brief Convert string to unsigned number
 *
 * Converts as much of the string as possible allowing for parsing
 * number from a string separated with non-digit delimiters
 * @param[in] s string to parse
 * @param[in out] endptr address of character pointer to store where passing failed.
 *                If the string passed completely, then this will point to the null terminator '\0'
 *                Is optional, and can be set to NULL if not used
 * @param[in] base numerical base to use
 * @returns Unsigned number represented in string
 */
unsigned strtou(const char *s, char **endptr, unsigned base);
/**
 * @brief Convert string to signed number
 *
 * Converts as much of the string as possible allowing for parsing
 * number from a string separated with non-digit delimiters
 * @param[in] s string to parse
 * @param[in out] endptr address of character pointer to store where passing failed.
 *                If the string passed completely, then this will point to the null terminator '\0'
 *                Is optional, and can be set to NULL if not used
 * @param[in] base numerical base to use
 * @returns Signed number represented in string
 */
int strtoi(const char *s, char **endptr, unsigned base);

#ifndef HOST_HIDE_PROTOTYPES
void *memset(void *s, char c, size_t n);
void *memcpy(void *__restrict__ dest, const void *__restrict__ src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
unsigned strlen(const char *s);
#endif

#endif
