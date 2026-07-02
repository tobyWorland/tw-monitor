#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED

#include <stdbool.h>

/**
 * @brief Assert if flag is false
 *
 * If \b flag is true carry on, else raise error
 * @param[in] flag Condition to check
 */
void assert(bool flag);

/**
 * @brief Assert if program reaches here
 */
#define ASSERT_NOT_REACHED()                    \
    assert(false);                              \
    __builtin_unreachable();

#endif
