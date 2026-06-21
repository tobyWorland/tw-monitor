/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .set    GPIO_MODER_CNF_INPUT,   0
        .set    GPIO_MODER_CNF_OUTPUT,  1
        .set    GPIO_MODER_CNF_ALTFUN,  2
        .set    GPIO_MODER_CNF_ANALOG,  3

        .set    USART1_BASE,            0x40011000
        .set    USART1_SR,              USART1_BASE + 0x00
        .set    USART1_DR,              USART1_BASE + 0x04
        .set    USART1_BRR,             USART1_BASE + 0x08
        .set    USART1_CR1,             USART1_BASE + 0x0C

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

        // Enable USART1
        ldr     r0,     =USART1_CR1
        ldr     r1,     =USART_CR1_UE
        str     r1,     [r0]

        // Set USART1 baud rate
        // Reference: ST RM0383 - 19.3.4 Fractional baud rate generation
        // In short: clock freq / baud rate / 16 = USARTDIV
        // Only the lower 16bit of the BRR register are used
        // 15-4 Mantissa
        //  3-0 Fractional
        // For the baud rate 115200 using the starting RC oscillator @ 16MHz:
        //   16e6 / 115200 / 16 = 8.681 approx
        //   thus mantissa = 8, fractional = 0.681 * (2**4) = 10 as integer
        ldr     r0,     =USART1_BRR
        ldr     r1,     =(8 << 4 | 10)
        str     r1,     [r0]

        // Enable USART1 transmitter & receiver
        ldr     r0,     =USART1_CR1
        ldr     r1,     [r0]
        ldr     r2,     =(USART_CR1_TE | USART_CR1_RE)
        orr     r1,     r2
        str     r1,     [r0]

        ldr     r0,     =g_hardware_inited
        movs    r1,     1
        strb    r1,     [r0]

        pop     {pc}

        .global usart2_putchar
        .type   usart2_putchar, %function
        // Inputs: R0 = byte to transmit over USART2
usart2_putchar:
        ldr     r1,     =USART2_SR
        movs    r2,     USART_SR_TXE
usart2_putchar.check_tx_register_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     usart2_putchar.check_tx_register_empty

        // Transmit character
        ldr     r1,     =USART2_DR
        strb    r0,     [r1]
        bx      lr

        .global usart2_putstring
        .type   usart2_putstring, %function
        // Inputs: R0 = zero terminated string to transmit over USART2
usart2_putstring:
        push    {r4, lr}
        mov     r4,     r0

1:
        ldrb    r0,     [r4],   1
        cbz     r0,     2f

        bl      putchar

        b       1b
2:
        pop     {r4, pc}

        .global usart2_putnewline
        .type   usart2_putnewline, %function
usart2_putnewline:
        push    {lr}
        movs    r0,     '\r'
        bl      putchar
        movs    r0,     '\n'
        bl      putchar
        pop     {pc}

        .global usart2_getchar
        .type   usart2_getchar, %function
        // Returns: R0 = byte received from USART2
usart2_getchar:
        ldr     r1,     =USART2_SR
        movs    r2,     USART_SR_RXNE
usart2_getchar.check_rx_register_not_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     usart2_getchar.check_rx_register_not_empty

        // Get the received byte
        ldr     r1,     =USART2_DR
        ldrb    r0,     [r1]
        bx      lr

        .global usart1_putchar
        .type   usart1_putchar, %function
        // Inputs: R0 = byte to transmit over USART1
usart1_putchar:
        ldr     r1,     =USART1_SR
        movs    r2,     USART_SR_TXE
usart1_putchar.check_tx_register_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     usart1_putchar.check_tx_register_empty

        // Transmit character
        ldr     r1,     =USART1_DR
        strb    r0,     [r1]
        bx      lr

        .global usart1_getchar
        .type   usart1_getchar, %function
        // Returns: R0 = byte received from USART1
usart1_getchar:
        ldr     r1,     =USART1_SR
        movs    r2,     USART_SR_RXNE
usart1_getchar.check_rx_register_not_empty:
        ldr     r3,     [r1]
        tst     r3,     r2
        beq     usart1_getchar.check_rx_register_not_empty

        // Get the received byte
        ldr     r1,     =USART1_DR
        ldrb    r0,     [r1]
        bx      lr

        .bss
        .global g_hardware_inited
g_hardware_inited:        .byte   0
