#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stdbool.h>

#ifdef HOST
void pad_init();
void pad_hold(char c);
unsigned pad_len(void);
#endif

const char *utoa_pad_w(unsigned number, unsigned base, unsigned minwidth, bool fillwithzero);
const char *itoa_pad_w(int number, unsigned base, unsigned minwidth, bool fillwidthzero);

const char *utoa_pad(unsigned number, unsigned base);
const char *itoa_pad(int number, unsigned base);

#endif
