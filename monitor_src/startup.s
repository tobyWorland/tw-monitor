/* vim: set tabstop=8:shiftwidth=8:expandtab */
	.syntax	unified
	.cpu	cortex-m4	
	.thumb

	.global	startup_entry
	.type 	startup_entry, %function
startup_entry:
	// Setup stack // TODO: No need to do if started from flash due to vector table
	ldr	r0,	=initial_initial_top
	mov	sp,	r0

	// TODO: Set VTOR to vector table in ram

	nop
	nop
	nop

	.global	hang
	.type 	hang, %function
hang:
	bkpt	90
	b	hang
