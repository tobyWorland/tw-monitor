
sram_start = 0x20000000;
sram_size  = 128k;
sram_end   = sram_start + sram_size;

ENTRY(startup_entry);

SECTIONS {
	. = sram_start;

	.text : {
		*(.text);
	}
}
