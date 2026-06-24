
stack_size = 4k;

MEMORY {
    /* ST RM0383 3.3 Embedded Flash memory in STM32F411xC/E */
    flash (rx) : ORIGIN = 0x08000000, LENGTH = 512k /* Sector 0 - Sector 7 */
    sram  (wx) : ORIGIN = 0x20000000, LENGTH = 128k
    boot  (rx) : ORIGIN = 0x00000000, LENGTH = 0x7FFFF
}

ENTRY(startup_entry);

SECTIONS {
    sram_start = ORIGIN(sram);

    .vect : ALIGN(128) {
        vect_vma_start = .;
        *(.vect);
        . = ALIGN(4); /* Ensure the size of .vect is in words for copy code */
        vect_vma_end = .;
    } >sram AT>flash
    /* This must be outside the section */
    vect_size_words = (vect_vma_end - vect_vma_start) / 4;
    vect_lma_start = LOADADDR(.vect);

    . = ORIGIN(BOOT) + SIZEOF(.vect);

    .text . : {
        *(.text);
        *(.rodata*);
    } >boot AT>flash

    .data : {
        data_vma_start = .;
        *(.data);
        . = ALIGN(4); /* Ensure the size of .data is in words for copy code */
        data_vma_end = .;
    } >sram AT>flash
    /* This must be outside the section */
    data_size_words = (data_vma_end - data_vma_start) / 4;
    data_lma_start = LOADADDR(.data);

    .bss (NOLOAD) : {
        bss_vma_start = .;
        *(.bss);
        . = ALIGN(4); /* Ensure the size of .bss is in words for init code */
        bss_vma_end = .;
    } >sram
    /* This must be outside the section */
    bss_size_words = (bss_vma_end - bss_vma_start) / 4;

    /* User area starts at the next free area of sram (after .bss) */
    . = bss_vma_end;
    . = ALIGN(4);
    user_start = .;

    /* Stack */
    stack_initial_top = ORIGIN(sram) + LENGTH(sram);
    . = stack_initial_top - stack_size;
    stack_ceiling = ALIGN(8);
    /* Align to 8 so it can be used to place the memory_entry below
     * which needs 8 byte alignment due to ldrd/strd instructions when -Os */
}
