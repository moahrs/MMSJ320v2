ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5); /* R-X */
    data PT_LOAD FLAGS(6); /* RW- */
}

SECTIONS
{
    /* MMSJOS executes from 0x00800000 in RAM */
    . = 0x00800000;

    /* Keep legacy runtime addresses without forcing sparse binary padding. */
    __stack = 0x00602400;
    __ungetbuf = __stack;
    heap = 0x00818000;

    .text : { *(.text*) } :text
    .rodata : { *(.rodata*) } :text
    .data : { *(.data*) } :data
    .bss (NOLOAD) : { *(.bss*) *(COMMON) } :data
}
