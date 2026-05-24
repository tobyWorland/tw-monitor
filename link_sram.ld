
sram_start = 0x20000000;
sram_size  = 128k;
sram_end   = sram_start + sram_size;

stack_size = 4k;

ENTRY(startup_entry);

SECTIONS {
	. = sram_start;

	.text : {
		*(.text);
	}

	/* Stack */
	stack_initial_top = sram_end;
	stack_ceiling = initial_sp - stack_size;
}
