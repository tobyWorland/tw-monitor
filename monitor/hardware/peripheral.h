#ifndef HARDWARE_PERIPHERAL_H_INCLUDED
#define HARDWARE_PERIPHERAL_H_INCLUDED

#include <stdint.h>

#define MAX_IRQ_PER_PERIPH 4

enum periph_bus {
    PERIPH_BUS_AHB1,
    PERIPH_BUS_AHB2,
    PERIPH_BUS_APB1,
    PERIPH_BUS_APB2,
};

struct peripheral {
    const char name[8];
    void *base;
    struct {
        enum periph_bus bus : 8;

        // Corresponds to the bit in RCC enable and reset registers
        int8_t rcc_index : 8;
    } rcc;
    uint8_t irqs[MAX_IRQ_PER_PERIPH];

    // Index of the peripheral for the driver
    // Starts from 1. 0 is N/A. e.g. USART2 is 2, USART1 is 1, etc
    // Meant for driver use and has no relevance to the hardware itself.
    uint8_t driver_idx;
};

#endif
