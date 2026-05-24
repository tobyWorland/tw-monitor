/* vim: set tabstop=8:shiftwidth=8:expandtab */
	.syntax	unified
	.cpu	cortex-m4
	.thumb

	.global	startup_entry
	.type	startup_entry, %function
startup_entry:
	// Setup stack // TODO: No need to do if started from flash due to vector table
	ldr	r0,	=stack_initial_top
	mov	sp,	r0

	// TODO: Set VTOR to vector table in ram

	bl	hardware_init

	ldr	r0,	=str_monitor
	bl	putstring

1:
	bl	getchar

	// Check char is in printable range
	cmp	r0,	' '
	ble	1b
	cmp	r0,	'~'
	bgt	1b

	mov	r4,	r0
	ldr	r0,	=str_char
	bl	putstring
	mov	r0,	r4
	bl	putchar
	bl	putnewline
	b	1b

	.global	hang
	.type	hang, %function
hang:
	bkpt	90
	b	hang

	.data
str_monitor:	.asciz "Monitor\r\n"
str_char:	.asciz "CHAR: "
