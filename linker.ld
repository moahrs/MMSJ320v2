
ENTRY(_start)

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
}
