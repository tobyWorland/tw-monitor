/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .set    RCC_BASE,               0x40023800
        .set    RCC_AHB1_ENR,           RCC_BASE + 0x30
        .set    RCC_APB1_ENR,           RCC_BASE + 0x40

        .set    RCC_AHB1_OFF_GPIOA,     0

        .set    RCC_APB1_OFF_USART2,    17

        .set    GPIOA_BASE,             0x40020000
        .set    GPIOA_MODER,            GPIOA_BASE + 0x00
        .set    GPIOA_AFRL,             GPIOA_BASE + 0x20

        .set    GPIO_MODER_CNF_INPUT,   0
        .set    GPIO_MODER_CNF_OUTPUT,  1
        .set    GPIO_MODER_CNF_ALTFUN,  2
        .set    GPIO_MODER_CNF_ANALOG,  3

        .set    USART2_BASE,            0x40004400
        .set    USART2_SR,              USART2_BASE + 0x00
        .set    USART2_DR,              USART2_BASE + 0x04
        .set    USART2_BRR,             USART2_BASE + 0x08
        .set    USART2_CR1,             USART2_BASE + 0x0C

        .set    USART_CR1_UE,           (1 << 13)
        .set    USART_CR1_TE,           (1 << 3)
        .set    USART_CR1_RE,           (1 << 2)

        .set    USART_SR_TXE,           (1 << 7)
        .set    USART_SR_RXNE,          (1 << 5)

        .global hardware_init
        .type   hardware_init, %function
hardware_init:
        push    {lr}

        // Enable peripheral clock for GPIO A
        ldr     r0,     =RCC_AHB1_ENR
        movs    r1,     (1 << RCC_AHB1_OFF_GPIOA)
        str     r1,     [r0]

        // Change PA2 & PA3 to alternate function mode (for USART2)
        //   NOTE: Need to OR mode settings as debug pins are also on GPIO A
        ldr     r0,     =GPIOA_MODER
        ldr     r1,     [r0]
        //   (MODE << (pin * 2)) - as 2 bits per pin mode in MODER register
        ldr     r2,     =((GPIO_MODER_CNF_ALTFUN << (2*2)) | (GPIO_MODER_CNF_ALTFUN << (3*2)))
        orr     r1,     r2
        str     r1,     [r0]

        // Set alternate function for PA2 & PA3 to 7 (for USART2)
        //   NOTE: Need to OR mode settings as debug pins are also on GPIO A
        ldr     r0,     =GPIOA_AFRL
        ldr     r1,     [r0]
        //   (MODE << (pin * 4)) - as 4 bits per pin mode in AFRL register
        ldr     r2,     =((7 << (2*4)) | (7 << (3*4)))
        orr     r1,     r2
        str     r1,     [r0]

        // Enable peripheral clock for USART2
        ldr     r0,     =RCC_APB1_ENR
        movs    r1,     (1 << RCC_APB1_OFF_USART2)
        str     r1,     [r0]

        // Enable USART2
        ldr     r0,     =USART2_CR1
        ldr     r1,     =USART_CR1_UE
        str     r1,     [r0]

        // Set USART2 baud rate
        // Reference: ST RM0383 - 19.3.4 Fractional baud rate generation
        // In short: clock freq / baud rate / 16 = USARTDIV
        // Only the lower 16bit of the BRR register are used
        // 15-4 Mantissa
        //  3-0 Fractional
        // For the baud rate 115200 using the starting RC oscillator @ 16MHz:
        //   16e6 / 115200 / 16 = 8.681 approx
        //   thus mantissa = 8, fractional = 0.681 * (2**4) = 10 as integer
        ldr     r0,     =USART2_BRR
        ldr     r1,     =(8 << 4 | 10)
        str     r1,     [r0]

        // Enable USART2 transmitter & receiver
        ldr     r0,     =USART2_CR1
        ldr     r1,     [r0]
        ldr     r2,     =(USART_CR1_TE | USART_CR1_RE)
        orr     r1,     r2
        str     r1,     [r0]

        pop     {pc}

        .global putchar
        .type   putchar, %function
        // Inputs: R0 = byte to transmit over USART2
putchar:
        ldr     r1,     =USART2_SR
        movs    r2,     USART_SR_TXE
putchar.check_tx_register_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     putchar.check_tx_register_empty

        // Transmit character
        ldr     r1,     =USART2_DR
        strb    r0,     [r1]
        bx      lr

        .global putstring
        .type   putstring, %function
        // Inputs: R0 = zero terminated string to transmit over USART2
putstring:
        push    {r4, lr}
        mov     r4,     r0

1:
        ldrb    r0,     [r4],   1
        cbz     r0,     2f

        bl      putchar

        b       1b
2:
        pop     {r4, pc}

        .global putnewline
        .type   putnewline, %function
putnewline:
        push    {lr}
        movs    r0,     '\r'
        bl      putchar
        movs    r0,     '\n'
        bl      putchar
        pop     {pc}

        .global getchar
        .type   getchar, %function
        // Returns: R0 = byte received from USART2
getchar:
        ldr     r1,     =USART2_SR
        movs    r2,     USART_SR_RXNE
getchar.check_rx_register_not_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     getchar.check_rx_register_not_empty

        // Get the received byte
        ldr     r1,     =USART2_DR
        ldrb    r0,     [r1]
        bx      lr
