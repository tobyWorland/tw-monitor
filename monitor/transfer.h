#ifndef TRANSFER_H_INCLUDED
#define TRANSFER_H_INCLUDED

#include <stdbool.h>

/**
 * @brief Receive binary data sent over USART1 into memory at destination
 *
 * Protocol used is the same as clobber_sram.h and binary data is sent from binsend.c
 *
 * @see clobber_sram_from_usart1
 */
bool transfer_receive(void *destination);

#endif
