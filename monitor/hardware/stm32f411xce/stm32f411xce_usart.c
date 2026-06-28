#include "stm32f411xce_usart.h"

#include "../../assert.h"
#include "../../char.h"
#include "../../memory.h"
#include "../../arm/debug.h"
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

#define BUFFER_SIZE 24

struct usart_driver_state {
    struct usart_intqueue {
        char *buffer;
        uint8_t len;
        uint8_t front;
        uint8_t back;
    } intqueue;
    bool enable_break;
    bool poll;
};
#define USART_DRIVER_STATE_INIT (struct usart_driver_state){ .poll = true }

static struct usart_driver_state s_usart_states[USART_COUNT];

static struct usart_driver_state *get_driver_state(struct peripheral *usart_periph) {
    assert(usart_periph);
    assert(usart_periph->driver_idx);

    // Index starts from 1, not 0.
    return &s_usart_states[usart_periph->driver_idx-1];
}

static void intqueue_init(struct usart_driver_state *driver_state) {
    char *old_buffer = driver_state->intqueue.buffer;

    driver_state->intqueue = (struct usart_intqueue){};

    if (old_buffer == NULL) {
        // Old buffer does not exist, allocate one
        old_buffer = memory_sys_alloc(BUFFER_SIZE);
    }

    // Reuse old buffer
    driver_state->intqueue.buffer = old_buffer;
}

static void intqueue_free(struct usart_driver_state *driver_state) {
    char *buffer = driver_state->intqueue.buffer;
    if (buffer) {
        memory_sys_free(buffer);
    }
}

static bool intqueue_is_empty(struct usart_driver_state *driver_state) {
    return driver_state->intqueue.len == 0;
}

static bool intqueue_is_full(struct usart_driver_state *driver_state) {
    return driver_state->intqueue.len == BUFFER_SIZE;
}

static uint8_t intqueue_get(struct usart_driver_state *driver_state) {
    if (intqueue_is_empty(driver_state)) {
        return 0;
    }

    util_enter_critical();
    uint8_t b = driver_state->intqueue.buffer[driver_state->intqueue.front++];
    driver_state->intqueue.front %= BUFFER_SIZE;
    driver_state->intqueue.len--;
    util_leave_critical();

    return b;
}

static void intqueue_put_from_isr(struct usart_driver_state *driver_state, uint8_t b) {
    if (intqueue_is_full(driver_state)) {
        // Silently drop b
        return;
    }

    driver_state->intqueue.buffer[driver_state->intqueue.back++] = b;
    driver_state->intqueue.back %= BUFFER_SIZE;
    driver_state->intqueue.len++;
}

static void _usart_isr(void) {
    struct peripheral *usart_periph = peripheral_usart_periph_from_irq(get_active_irq());
    assert(usart_periph);

    struct usart *usart = usart_periph->base;
    struct usart_driver_state *driver_state = get_driver_state(usart_periph);

    if (usart->status & USART_SR_RXNE) {
        char b = usart->data_reg;

        if (driver_state->enable_break) {
            if (b == CTRL('c')) {
                usart2_putstring("***USER DEBUG BREAK***\r\n");
                arm_pend_debug_monitor();
                return;
            }
        }

        intqueue_put_from_isr(driver_state, b);
    }
}

void usart_enable(struct peripheral *usart_periph, bool enable) {
    volatile struct usart *usart = usart_periph->base;

    if (!enable) { // Disable
        usart->control1 = 0;

        // Free the queue if it has been allocated
        intqueue_free(get_driver_state(usart_periph));

        return;
    }

    // Initialise driver state
    *get_driver_state(usart_periph) = USART_DRIVER_STATE_INIT;

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

    // Set polling off, to enable interrupts
    usart_set_polling(usart_periph, false);

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
    struct usart_driver_state *driver_state = get_driver_state(usart_periph);

    if (driver_state->poll) {
        // Use polling to get a byte

        // Wait for usart to have a byte for us
        while (!(usart->status & USART_SR_RXNE));

        // Read byte from data register
        return usart->data_reg;
    } else {
        // Use interrupt to get a byte

        while (1) {
            if (!intqueue_is_empty(driver_state)) {
                return intqueue_get(driver_state);
            }
            __WFI();
        }
    }
}

void usart_enable_debug_break(struct peripheral *usart_periph, bool enable_break) {
    get_driver_state(usart_periph)->enable_break = enable_break;
}

bool usart_get_polling(struct peripheral *usart_periph) {
    return get_driver_state(usart_periph)->poll;
}

void usart_set_polling(struct peripheral *usart_periph, bool should_poll) {
    struct usart_driver_state *driver_state = get_driver_state(usart_periph);

    struct usart *usart = usart_periph->base;

    driver_state->poll = should_poll;

    if (driver_state->poll) {
        // Disable to prevent IRQ from becoming active
        nvic_disable_irq(usart_periph->irqs[0]);

        // Disable interrupt for RX not empty
        usart->control1 &= ~USART_CR1_RXNEIE;

        // Clear pending
        nvic_clear_irq(usart_periph->irqs[0]);
    } else {
        // Init queue before enabling IRQ
        intqueue_init(driver_state);

        // Enable IRQ
        nvic_enable_irq(usart_periph->irqs[0]);

        // Enable interrupt for RX not empty
        usart->control1 |= USART_CR1_RXNEIE;
    }
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
