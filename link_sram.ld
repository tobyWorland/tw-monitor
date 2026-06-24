
stack_size = 4k;

MEMORY {
    sram  (wx) : ORIGIN = 0x20000000, LENGTH = 128k
}

ENTRY(startup_entry);

SECTIONS {
    sram_start = ORIGIN(sram);

    . = sram_start;
    .vect : ALIGN(128) {
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
        . = ALIGN(4); /* Ensure the size of .bss is in words for init code */
        bss_vma_end = .;
    }
    /* This must be outside the section */
    bss_size_words = (bss_vma_end - bss_vma_start) / 4;

    . = ALIGN(4);
    user_start = .;

    /* Stack */
    stack_initial_top = ORIGIN(sram) + LENGTH(sram);
    . = stack_initial_top - stack_size;
    stack_ceiling = ALIGN(8);
    /* Align to 8 so it can be used to place the memory_entry below
     * which needs 8 byte alignment due to ldrd/strd instructions when -Os */
}
