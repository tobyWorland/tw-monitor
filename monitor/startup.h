#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

/**
 * @brief Print register state recorded by \b save_registers
 *
 * save_registers is internal to monitor/startup.s and can only be
 * called from an exception handler. This function is intended to only
 * be called when an exception is being handled.
 *
 * @warning Requires save_registers to be called first
 */
void print_registers(void);

/**
 * @brief Get the current active exception
 *
 * Returns 0 if no exception is active (thread mode), otherwise see docs/exceptions.md
 *
 * @returns Current exception number
 */
int get_active_exception(void);
/**
 * @brief Get the current active IRQ
 *
 * Returns -1 if no IRQ is active, otherwise return IRQ number
 *
 * @warning If in handler mode but not an IRQ: -1 is still returned
 *
 * @returns Current active IRQ
 */
int get_active_irq(void);

// TODO: Remove and implement a proper exit
/**
 * @brief Reset stack and jump to monitor menu (no return)
 */
__attribute__((noreturn))
void exit_to_monitor(void);

#endif
