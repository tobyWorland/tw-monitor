#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED

#include <stdbool.h>

void assert(bool flag);

#define ASSERT_NOT_REACHED()                    \
    assert(false);                              \
    __builtin_unreachable();

#endif
