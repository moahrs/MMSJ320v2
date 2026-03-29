/* basic_inic.S - Startup code for BASIC interpreter
 * Converted from basic.a68 (IDE68K) to GAS syntax
 * Sets up stack/heap layout and jumps to main
 */

            .section .text
            .org    0x00008000          /* Start address of BASIC program */

            .global start
start:
            jmp     main               /* jump directly to C main() - absolute long */

__exit:                                 /* exit() functions */
_exit:
            lea     __stack,%a7         /* reset stack */
            trap    #15
            .word   0                   /* system call 0 -> EXIT (no return) */
            jmp     main               /* when re-started after exit() call */

            .section .text              /* constant data & string data */
            .section .data
            .balign 2

            .global __heap
__heap:
            .long   heap                /* begin of free memory */

            .section .bss
            .balign 2
            .org    0x610000
            .space  1024                /* stack for main() function (ds.w 512) */
__stack:
__ungetbuf:
            .space  4                   /* ungetbuffer for stdio (ds.l 1) */

            .section .heap
            .balign 2
            .org    0x8F0000
heap:                                   /* start of free memory */
