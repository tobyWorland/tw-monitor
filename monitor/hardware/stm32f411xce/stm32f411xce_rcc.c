#include "stm32f411xce_rcc.h"

#include "../../assert.h"
#include "../../util.h"

#include <stddef.h>

#define RCC_BASE     0x40023800
#define RCC_AHB1_ENR RCC_BASE + 0x30
#define RCC_AHB2_ENR RCC_BASE + 0x34
#define RCC_APB1_ENR RCC_BASE + 0x40
#define RCC_APB2_ENR RCC_BASE + 0x44

static volatile uint32_t *rcc_get_bus_enable_reg(enum periph_bus bus) {
    void *ptr = NULL;

    switch (bus) {
    case PERIPH_BUS_AHB1:
        ptr = (void *)RCC_AHB1_ENR;
        break;
    case PERIPH_BUS_AHB2:
        ptr = (void *)RCC_AHB2_ENR;
        break;
    case PERIPH_BUS_APB1:
        ptr = (void *)RCC_APB1_ENR;
        break;
    case PERIPH_BUS_APB2:
        ptr = (void *)RCC_APB2_ENR;
        break;
    }

    assert(ptr);

    return ptr;
}

void rcc_enable_clock(struct peripheral *periph, bool enable) {
    volatile uint32_t *enable_register = rcc_get_bus_enable_reg(periph->rcc.bus);

    if (enable) {
        *enable_register |= BIT(periph->rcc.rcc_index);
    } else {
        *enable_register &= ~BIT(periph->rcc.rcc_index);
    }
}
