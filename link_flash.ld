
sram_start = 0x20000000;
sram_size  = 128k;
sram_end   = sram_start + sram_size;

/* ST RM0383 3.3 Embedded Flash memory in STM32F411xC/E */
flash_start = 0x08000000;
flash_size  = 512k; /* Sector 0 - Sector 7 */
flash_end   = flash_start + flash_size;

stack_size = 4k;

ENTRY(startup_entry);

SECTIONS {
	. = flash_start;

	.vect : {
		*(.vect);
	}

	. = ALIGN(4);
	.text : {
		*(.text);
		*(.rodata*);
	}

	. = sram_start;

	. = ALIGN(4);
	data_lma_start = .;
	.data : AT(ADDR(.text) + SIZEOF(.text)) {
		data_vma_start = .;
		*(.data);
		. = ALIGN(4);
		data_vma_end = .;
	}

	. = ALIGN(4);
	.bss (NOLOAD) : {
		start_bss = .;
		*(.bss);
		. = ALIGN(4);
		end_bss = .;
	}

	/* Stack */
	stack_initial_top = sram_end;
	stack_ceiling = stack_initial_top - stack_size;
}
