#ifndef HARDWARE_BOARD_H_INCLUDED
#define HARDWARE_BOARD_H_INCLUDED

#include <stdbool.h>

extern bool g_board_inited; ///< Flag used to signify if hardware has been initialised

/**
 * @brief Initialise the board's hardware
 */
void board_init(void);

/**
 * @brief Get current system clock in MHz
 *
 * @returns System clock in MHz
 */
unsigned board_get_sysclock_MHz(void);

/**
 * @brief Enable the use of the user break key to enter the debug monitor handler
 *
 * User break key by default is C-c
 *
 * It is desirable to only enable when executing code from the user to
 * allow the escaping from infinite loops, or examining the value of
 * the program counter
 *
 * @param[in] If true enable user break key, else disable the user break key
 */
void board_enable_debug_user_break(bool enable);

#endif
