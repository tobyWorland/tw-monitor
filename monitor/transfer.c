#include "transfer.h"

#include "hardware.h"
#include "io.h"
#include "string.h"

#define ACK 6

static void receive_into(void *dest, uint16_t len) {
    uint8_t *p = dest;

    while (len--) {
        *p++ = usart1_getchar();
        usart1_putchar(ACK);
    }
}

bool transfer_receive(void *destination) {
    uint16_t len;

    putstring("Waiting for transfer...\r\n");

    receive_into(&len, 2);

    putstring("Getting ");
    putstring(itoa_pad(len, 10));
    putstring(" bytes\r\n");

    receive_into(destination, len);
    putstring("Transfer complete\r\n");
}
