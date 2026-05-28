
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
	.bss : {
		start_bss = .;
		*(.bss);
		. = ALIGN(4);
		end_bss = .;
	}

	/* Stack */
	stack_initial_top = sram_end;
	stack_ceiling = stack_initial_top - stack_size;
}
