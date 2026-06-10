
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

    . = ALIGN(4);
    data_lma_start = .;

    . = sram_start;
    .data : AT(data_lma_start) {
        data_vma_start = .;
        *(.data);
        . = ALIGN(4); /* Ensure the size of .data is in words for copy code */
        data_vma_end = .;
    }
    /* This must be outside the section */
    data_size_words = (data_vma_end - data_vma_start) / 4;

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
    stack_initial_top = sram_end;
    stack_ceiling = stack_initial_top - stack_size;
}
