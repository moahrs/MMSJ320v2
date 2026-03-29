
    .global _start

_start:
    move.l #0x00080000, %sp   /* stack top (adjust later) */
    jsr main
loop:
    bra loop
