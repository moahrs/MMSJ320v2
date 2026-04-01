
ENTRY(os_boot_entry)

PHDRS
{
    text PT_LOAD FLAGS(5); /* R-X */
    data PT_LOAD FLAGS(6); /* RW- */
}

SECTIONS
{
    . = 0x000000;

    .text : { *(.text*) } :text
    .rodata : { *(.rodata*) } :text
    .data : { *(.data*) } :data
    .bss (NOLOAD) : { *(.bss*) *(COMMON) } :data

    /* heap: in User RAM at 0x810000, separate from C .bss variables */
    . = 0x810000;
    .heapram (NOLOAD) : { *(.heapram) }
}
