#include "stm32f411xce_usart.h"

#include "../../assert.h"
#include "../../arm/nvic.h"
#include "../../vector.h"
#include "../../util.h"
#include "../../startup.h"
#include "stm32f411xce_peripherals.h"

#include <stddef.h>

struct usart {
    uint32_t status;        // SR
    uint32_t data_reg;      // DR
    uint32_t baud_rate_div; // BRR
    uint32_t control1;      // CR1
    // TODO: Finish
};

#define USART_SR_TXE     BIT(7)
#define USART_SR_RXNE    BIT(5)

#define USART_CR1_UE     BIT(13)
#define USART_CR1_RXNEIE BIT(5)
#define USART_CR1_TE     BIT(3)
#define USART_CR1_RE     BIT(2)

// TODO: Rework
static char usart2_byte = 0;

static struct usart *usart_from_irq(unsigned irq) {
    struct peripheral *usart_periph = peripheral_usart_periph_from_irq(irq);
    if (!usart_periph) {
        return NULL;
    }
    return usart_periph->base;
}

static void _usart_isr() {
    struct usart *usart = usart_from_irq(get_active_irq());
    assert(usart);

    if (usart == g_periph_usart2.base && usart->status & USART_SR_RXNE) {
        char b = usart->data_reg;
        if (!usart2_byte) {
            usart2_byte = b;
        }
    }
}

void usart_enable(struct peripheral *usart_periph, bool enable) {
    volatile struct usart *usart = usart_periph->base;

    if (!enable) { // Disable
        usart->control1 = 0;
        return;
    }

    // Enable USART
    usart->control1 = USART_CR1_UE;

    // TODO: Need to calculate this in C
    // Set USART2 baud rate
    // Reference: ST RM0383 - 19.3.4 Fractional baud rate generation
    // In short: clock freq / baud rate / 16 = USARTDIV
    // Only the lower 16bit of the BRR register are used
    // 15-4 Mantissa
    //  3-0 Fractional
    // For the baud rate 115200 using the starting RC oscillator @ 16MHz:
    //   16e6 / 115200 / 16 = 8.681 approx
    //   thus mantissa = 8, fractional = 0.681 * (2**4) = 10 as integer
    usart->baud_rate_div = 8 << 4 | 10;

    while ((usart->status & USART_SR_RXNE)) {
        (void)usart->data_reg;
    }

    // Set ISR
    vector_set_isr_for(usart_periph->irqs[0], _usart_isr);
    nvic_enable_irq(usart_periph->irqs[0]);

    // Enable interrupt for RX not empty
    usart->control1 |= USART_CR1_RXNEIE;

    // Enable TX and RX
    usart->control1 |= USART_CR1_TE | USART_CR1_RE;
}

void usart_putbyte(struct peripheral *usart_periph, uint8_t b) {
    volatile struct usart *usart = usart_periph->base;

    // Wait for usart to have space for our byte
    while (!(usart->status & USART_SR_TXE));

    // Write byte to data register
    usart->data_reg = b;
}

uint8_t usart_getbyte(struct peripheral *usart_periph) {
    volatile struct usart *usart = usart_periph->base;

    if (usart_periph == &g_periph_usart2) {
        do {
            if (usart2_byte) {
                uint8_t b = usart2_byte;
                usart2_byte = 0;
                return b;
            }
            __WFI();
        } while(1);
    }

    // Wait for usart to have a byte for us
    while (!(usart->status & USART_SR_RXNE));

    // Read byte from data register
    return usart->data_reg;
}

// FIXME: Temporary
#include "stm32f411xce_peripherals.h"

void usart2_putchar(char c) {
    usart_putbyte(&g_periph_usart2, c);
}
void usart2_putstring(const char *string) {
    for (; *string; string++) {
        usart2_putchar(*string);
    }
}
void usart2_putnewline(void) {
    usart2_putchar('\r');
    usart2_putchar('\n');
}
char usart2_getchar(void) {
    return usart_getbyte(&g_periph_usart2);
}

void usart1_putchar(char c) {
    usart_putbyte(&g_periph_usart1, c);
}
char usart1_getchar(void) {
    return usart_getbyte(&g_periph_usart1);
}
