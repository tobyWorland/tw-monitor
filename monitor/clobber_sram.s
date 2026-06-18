/* vim: set tabstop=8 shiftwidth=8 expandtab : */
        .syntax unified
        .cpu    cortex-m4
        .thumb

        .set    USART1_BASE,    0x40011000
        .set    USART_OFF_SR,   0
        .set    USART_OFF_DR,   4
        .set    ACK,            6

        .set    USART_SR_TXE,           (1 << 7)
        .set    USART_SR_RXNE,          (1 << 5)

        .global clobber_sram_from_usart1
        .type clobber_sram_from_usart1, %function
clobber_sram_from_usart1:
        // Reset stack
        ldr     r0,     =stack_initial_top
        mov     sp,     r0

        // Size of fragment
        movs    r0,     (end_of_fragment - start_of_fragment)

        // Allocate space on stack for fragment
        subs    sp,     r0

        // Copy fragment onto stack
        asr     r0,     2 // R0 / 4 to get size in words
        mov     r1,     sp
        adr     r2,     start_of_fragment
1:
        ldr     r3,     [r2],   4
        str     r3,     [r1],   4

        subs    r0,     1
        bne     1b
2:

        // Jump to fragment in stack
        mov     pc,     sp

start_of_fragment:
        .balign 4
        // Load constants
        ldr     r4,     =USART1_BASE
        ldr     r5,     =sram_start

        // Get length of bytes and store into r6
        subs    sp,     4
        mov     r1,     sp
        bl      rx
        bl      rx
        ldrh    r6,     [sp],   4

        mov     r1,     r5 // Set destination to start of sram

        // Receive bytes into sram
1:
        cbz     r6,     2f

        bl      rx

        subs    r6,     1
        b       1b
2:

        // Get entry point from vector table and jump
        ldr     r0,     [r5, 4]
        bx      r0

// Inputs:
// R4 Base of USART
// R1 Pointer to destination
// Returns:
// R1++
rx:
        // Wait for byte
        ldr     r0,     [r4, USART_OFF_SR]
        tst     r0,     USART_SR_RXNE
        beq     rx

        // Get byte from USART and write into *R1++
        ldrb    r0,     [r4, USART_OFF_DR]
        strb    r0,     [r1],   1

        // Assume there is enough space to send ACK as there is nothing else to wait for
        // Send ACK byte over USART
        movs    r0,     ACK
        strb    r0,     [r4, USART_OFF_DR]
        bx      lr

        // Ensure literals get copied with the fragment
        .pool

        .balign 4
end_of_fragment:
