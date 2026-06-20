#ifndef SCB_H_INCLUDED
#define SCB_H_INCLUDED

void print_fault_state(void);
void clear_fault_state(void);

__attribute__((noreturn))
void scb_sys_reset_request(void);

#endif
