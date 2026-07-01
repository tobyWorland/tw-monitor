#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

void print_registers(void);

int get_active_exception(void);
int get_active_irq(void);

// TODO: Remove and implement a proper exit
__attribute__((noreturn))
void exit_to_monitor(void);

#endif
