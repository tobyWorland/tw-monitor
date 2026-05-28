
sram_start = 0x20000000;
sram_size  = 128k;
sram_end   = sram_start + sram_size;

stack_size = 4k;

ENTRY(startup_entry);

SECTIONS {
	. = sram_start;

	.vect : {
		*(.vect);
	}

	. = ALIGN(4);
	.text : {
		*(.text);
		*(.rodata*);
	}

	. = ALIGN(4);
	.data : {
		*(.data);
	}

	. = ALIGN(4);
	.bss (NOLOAD) : {
		bss_vma_start = .;
		*(.bss);
		. = ALIGN(4);
		bss_vma_end = .;
	}

	/* Stack */
	stack_initial_top = sram_end;
	stack_ceiling = stack_initial_top - stack_size;
}
