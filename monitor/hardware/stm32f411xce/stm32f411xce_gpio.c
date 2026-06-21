#include "stm32f411xce_gpio.h"

#define GPIO_OFF_MODER 0
#define GPIO_OFF_AFRL 0x20

struct gpio {
    uint32_t mode;                // MODER
    uint32_t output_type;         // OTYPER
    uint32_t output_speed;        // OSPEEDR
    uint32_t pull_up_pull_down;   // PUPDR
    uint32_t input_data;          // IDR
    uint32_t output_data;         // ODR
    uint32_t bit_set_reset;       // BSRR
    uint32_t port_config_lock;    // LCKR
    uint32_t alt_func_low;        // AFRL
    uint32_t alt_func_high;       // AFRH
};

#define MODER_CNF_INPUT  0
#define MODER_CNF_OUTPUT 1
#define MODER_CNF_ALTFUN 2
#define MODER_CNF_ANALOG 3

void gpio_set_pin_alt_func(struct peripheral *gpio_periph, unsigned pin, unsigned altfunc_code) {
    volatile struct gpio *gpio = gpio_periph->base;

    // 2 bits per pin in MODER
    gpio->mode &= ~(3 << 2 * pin);
    gpio->mode |= (MODER_CNF_ALTFUN << 2 * pin);

    // 4 bits per pin in AFRL & AFRH
    if (pin < 8) {
        gpio->alt_func_low &= ~(15 << 4 * pin);
        gpio->alt_func_low |= (altfunc_code << 4 * pin);
    } else {
        gpio->alt_func_low &= ~(15 << 4 * (pin - 8));
        gpio->alt_func_low |= (altfunc_code << 4 * (pin - 8));
    }
}
