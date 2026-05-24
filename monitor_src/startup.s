	.syntax	unified
	.cpu	cortex-m4	
	.thumb

	.global	startup_entry
	.type 	startup_entry, %function
startup_entry:
	nop
	nop
	nop

	.global	hang
	.type 	hang, %function
hang:
	bkpt	90
	b	hang
