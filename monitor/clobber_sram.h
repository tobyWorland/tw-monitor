#ifndef CLOBBER_SRAM_H_INCLUDED
#define CLOBBER_SRAM_H_INCLUDED

/**
 * @fn clobber_sram_from_usart1
 * @brief Replace contents of SRAM with binary data sent over USART1
 *
 * Used to replace running program without needing to connect the debugger.
 * Protocol used is the same as transfer.c and binary data is sent from binsend.c
 *
 * Implemented in monitor/clobber_sram.s
 *
 * @see transfer_receive
 */
__attribute__((noreturn))
void clobber_sram_from_usart1(void);

#endif
