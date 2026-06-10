#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

#ifdef HOST
void pad_init();
void pad_hold(char c);
unsigned pad_len(void);
#endif

const char *utoa_pad_w(unsigned number, unsigned base, unsigned minwidth, bool fillwithzero);
const char *itoa_pad_w(int number, unsigned base, unsigned minwidth, bool fillwidthzero);

const char *utoa_pad(unsigned number, unsigned base);
const char *itoa_pad(int number, unsigned base);

unsigned strtou(const char *s, char **endptr, unsigned base);
int strtoi(const char *s, char **endptr, unsigned base);

#ifndef HOST
void *memset(void *s, char c, size_t n);
void *memcpy(void *__restrict__ dest, const void *__restrict__ src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
#else
// FIXME: Need to handle this better
// Host should be able to build these implementations and then rename them instead
#include <string.h>
#endif

#endif
