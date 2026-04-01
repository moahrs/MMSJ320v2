.global monitor_func_table
|********************************************************************************************************
|                                               uC/OS-II
|                                         The Real-Time Kernel
|
|                            (c) Copyright 1999, Jean J. Labrosse, Weston, FL
|                                          All Rights Reserved
|
|
|                                        IDE68K Specific boot code
|
|
| File         : OS_BOOT.ASM
| By           : Peter J. Fondse
|********************************************************************************************************


|********************************************************************************************************
|                                            REVISION HISTORY
|
| $Log$
|
|********************************************************************************************************


|********************************************************************************************************
|                                          PUBLIC DECLARATIONS
|********************************************************************************************************

           .global        _exit
           .global        __exit
           .global        __putch
           .global        __getch
           .global        __ungetbuf
           .global        __kbhit
           .global        os_boot_entry

|********************************************************************************************************

           .section .text

           .org 0x0000

           .long        0x0063FFFE       | Vector #  0, 0x00000000: Initial SP - Original: __stack
           .long        0x400            | Vector #  1, 0x00000004: Initial PC - Original: _main
           .long        _errorBusAddr   | Vector #  2, 0x00000008: Bus Error
           .long        _errorBusAddr   | Vector #  3, 0x0000000C: Address Error
           .long        _errorIllegalInst   | Vector #  4, 0x00000010: Illegal Instruction
           .long        _errorZeroDivide  | Vector #  5, 0x00000014: Zero Division
           .long        _errorChkInst             | Vector #  6, 0x00000018: CHK, CHK2 Instruction
           .long        _errorTrapV             | Vector #  7, 0x0000001C: TRAPxx, TRAPV Instructions
           .long        _errorPrivV             | Vector #  8, 0x00000020: Privilege Violation
           .long        _traceDebug             | Vector #  9, 0x00000024: Trace
           .long        0x24             | Vector # 10, 0x00000028: Line 1010 Emulator
           .long        0x28             | Vector # 11, 0x0000002C: Line 1111 Emulator
           .long        0x2C             | Vector # 12, 0x00000030: Hardware Breakpoint
           .long        0x2C             | Vector # 13, 0x00000034: Coprocessor Protocol Violation
           .long        0x2C             | Vector # 14, 0x00000038: Format error
           .long        0x2C             | Vector # 15, 0x0000003C: Format error
           .long        0x2C             | Vector # 16, 0x00000040: Timer Tick ISR - _OSTickISR
           .long        0x2C             | Vector # 17, 0x00000044: Reserved
           .long        0x2C             | Vector # 18, 0x00000048: Reserved
           .long        0x2C             | Vector # 19, 0x0000004C: Reserved
           .long        0x4C             | Vector # 20, 0x00000050: Reserved
           .long        0x2C             | Vector # 21, 0x00000054: Reserved
           .long        0x2C             | Vector # 22, 0x00000058: Reserved
           .long        0x2C             | Vector # 23, 0x0000005C: Reserved
           .long        0x30             | Vector # 24, 0x00000060: Spurious interrupt
           .long        0x34             | Vector # 25, 0x00000064: Level 1 Interrupt Autovector
           .long        0x38             | Vector # 26, 0x00000068: Level 2 Interrupt Autovector
           .long        0x3C             | Vector # 27, 0x0000006C: Level 3 Interrupt Autovector
           .long        0x40             | Vector # 28, 0x00000070: Level 4 Interrupt Autovector
           .long        0x44             | Vector # 29, 0x00000074: Level 5 Interrupt Autovector
           .long        0x48             | Vector # 30, 0x00000078: Level 6 Interrupt Autovector
           .long        0x4C             | Vector # 31, 0x0000007C: Level 7 Interrupt Autovector
           .long        0x008000C4             | Vector # 32, 0x00000080: Trap # 0 Context switch - _OSCtxSw
           .long        0x54             | Vector # 33, 0x00000084: Trap # 1
           .long        0x58             | Vector # 34, 0x00000088: Trap # 2
           .long        0x5C             | Vector # 35, 0x0000008C: Trap # 3
           .long        0x60             | Vector # 36, 0x00000090: Trap # 4
           .long        0x64             | Vector # 37, 0x00000094: Trap # 5
           .long        0x68             | Vector # 38, 0x00000098: Trap # 6
           .long        0x6C             | Vector # 39, 0x0000009C: Trap # 7
           .long        0x70             | Vector # 40, 0x000000A0: Trap # 8
           .long        0x74             | Vector # 41, 0x000000A4: Trap # 9
           .long        0x78             | Vector # 42, 0x000000A8: Trap #10
           .long        0x7C             | Vector # 43, 0x000000AC: Trap #11
           .long        0x80             | Vector # 44, 0x000000B0: Trap #12
           .long        0x84             | Vector # 45, 0x000000B4: Trap #13
           .long        0x88             | Vector # 46, 0x000000B8: Trap #14
           .long        0x8C             | Vector # 47, 0x000000BC: Trap #15 System call
           .long        0x90             | Vector # 48, 0x000000C0: Reserved
           .long        0x94             | Vector # 49, 0x000000C4: Reserved
           .long        0x98             | Vector # 50, 0x000000C8: Reserved
           .long        0x9C             | Vector # 51, 0x000000CC: Reserved
           .long        0xA0             | Vector # 52, 0x000000D0: Reserved
           .long        0xA4             | Vector # 53, 0x000000D4: Reserved
           .long        0xA8             | Vector # 54, 0x000000D8: Reserved
           .long        0xAC             | Vector # 55, 0x000000DC: Reserved
           .long        0xB0             | Vector # 56, 0x000000E0: Reserved
           .long        0xB4             | Vector # 57, 0x000000E4: Reserved
           .long        0xB8             | Vector # 58, 0x000000E8: Reserved
           .long        0xBC             | Vector # 59, 0x000000EC: Reserved
           .long        0xC0             | Vector # 60, 0x000000F0: Reserved
           .long        0xC4             | Vector # 61, 0x000000F4: Reserved
           .long        0xC8             | Vector # 62, 0x000000F8: Reserved
           .long        0xCC             | Vector # 63, 0x000000FC: Reserved
           .long        0               | Vector # 64, 0x00000100: User Defined Vector
           .long        0               | Vector # 65, 0x00000104: User Defined Vector
           .long        0               | Vector # 66, 0x00000108: User Defined Vector
           .long        0               | Vector # 67, 0x0000010C: User Defined Vector
           .long        0               | Vector # 68, 0x00000110: User Defined Vector
           .long        0               | Vector # 69, 0x00000114: User Defined Vector
           .long        0               | Vector # 70, 0x00000118: User Defined Vector
           .long        0               | Vector # 71, 0x0000011C: User Defined Vector
           .long        0               | Vector # 72, 0x00000120: User Defined Vector
           .long        0               | Vector # 73, 0x00000124: User Defined Vector
           .long        0               | Vector # 74, 0x00000128: User Defined Vector
           .long        0               | Vector # 75, 0x0000012C: User Defined Vector
           .long        0               | Vector # 76, 0x00000130: User Defined Vector
           .long        0               | Vector # 77, 0x00000134: User Defined Vector
           .long        0               | Vector # 78, 0x00000138: User Defined Vector
           .long        0               | Vector # 79, 0x0000013C: User Defined Vector
           .long        0               | Vector # 80, 0x00000140: User Defined Vector
           .long        0               | Vector # 81, 0x00000144: User Defined Vector
           .long        0               | Vector # 82, 0x00000148: User Defined Vector
           .long        0               | Vector # 83, 0x0000014C: User Defined Vector
           .long        0               | Vector # 84, 0x00000150: User Defined Vector
           .long        0               | Vector # 85, 0x00000154: User Defined Vector
           .long        0               | Vector # 86, 0x00000158: User Defined Vector
           .long        0               | Vector # 87, 0x0000015C: User Defined Vector
           .long        0               | Vector # 88, 0x00000160: User Defined Vector
           .long        0               | Vector # 89, 0x00000164: User Defined Vector
           .long        0               | Vector # 90, 0x00000168: User Defined Vector
           .long        0               | Vector # 91, 0x0000016C: User Defined Vector
           .long        0               | Vector # 92, 0x00000170: User Defined Vector
           .long        0               | Vector # 93, 0x00000174: User Defined Vector
           .long        0               | Vector # 94, 0x00000178: User Defined Vector
           .long        0               | Vector # 95, 0x0000017C: User Defined Vector
           .long        0               | Vector # 96, 0x00000180: User Defined Vector
           .long        0               | Vector # 97, 0x00000184: User Defined Vector
           .long        0               | Vector # 98, 0x00000188: User Defined Vector
           .long        0               | Vector # 99, 0x0000018C: User Defined Vector
           .long        0               | Vector #100, 0x00000190: User Defined Vector
           .long        0               | Vector #101, 0x00000194: User Defined Vector
           .long        0               | Vector #102, 0x00000198: User Defined Vector
           .long        0               | Vector #103, 0x0000019C: User Defined Vector
           .long        0               | Vector #104, 0x000001A0: User Defined Vector
           .long        0               | Vector #105, 0x000001A4: User Defined Vector
           .long        0               | Vector #106, 0x000001A8: User Defined Vector
           .long        0               | Vector #107, 0x000001AC: User Defined Vector
           .long        0               | Vector #108, 0x000001B0: User Defined Vector
           .long        0               | Vector #109, 0x000001B4: User Defined Vector
           .long        0               | Vector #110, 0x000001B8: User Defined Vector
           .long        0               | Vector #111, 0x000001BC: User Defined Vector
           .long        0               | Vector #112, 0x000001C0: User Defined Vector
           .long        0               | Vector #113, 0x000001C4: User Defined Vector
           .long        0               | Vector #114, 0x000001C8: User Defined Vector
           .long        0               | Vector #115, 0x000001CC: User Defined Vector
           .long        0               | Vector #116, 0x000001D0: User Defined Vector
           .long        0               | Vector #117, 0x000001D4: User Defined Vector
           .long        0               | Vector #118, 0x000001D8: User Defined Vector
           .long        0               | Vector #119, 0x000001DC: User Defined Vector
           .long        0               | Vector #120, 0x000001E0: User Defined Vector
           .long        0               | Vector #121, 0x000001E4: User Defined Vector
           .long        0               | Vector #122, 0x000001E8: User Defined Vector
           .long        0               | Vector #123, 0x000001EC: User Defined Vector
           .long        0               | Vector #124, 0x000001F0: User Defined Vector
           .long        0               | Vector #125, 0x000001F4: User Defined Vector
           .long        0               | Vector #126, 0x000001F8: User Defined Vector
           .long        0               | Vector #127, 0x000001FC: User Defined Vector
           .long        0               | Vector #128, 0x00000200: User Defined Vector
           .long        0               | Vector #129, 0x00000204: User Defined Vector
           .long        0               | Vector #130, 0x00000208: User Defined Vector
           .long        0               | Vector #131, 0x0000020C: User Defined Vector
           .long        0               | Vector #132, 0x00000210: User Defined Vector
           .long        0               | Vector #133, 0x00000214: User Defined Vector
           .long        0               | Vector #134, 0x00000218: User Defined Vector
           .long        0               | Vector #135, 0x0000021C: User Defined Vector
           .long        0               | Vector #136, 0x00000220: User Defined Vector
           .long        0               | Vector #137, 0x00000224: User Defined Vector
           .long        0               | Vector #138, 0x00000228: User Defined Vector
           .long        0               | Vector #139, 0x0000022C: User Defined Vector
           .long        0               | Vector #140, 0x00000230: User Defined Vector
           .long        0               | Vector #141, 0x00000234: User Defined Vector
           .long        0               | Vector #142, 0x00000238: User Defined Vector
           .long        0               | Vector #143, 0x0000023C: User Defined Vector
           .long        0               | Vector #144, 0x00000240: User Defined Vector
           .long        0               | Vector #145, 0x00000244: User Defined Vector
           .long        0               | Vector #146, 0x00000248: User Defined Vector
           .long        0               | Vector #147, 0x0000024C: User Defined Vector
           .long        0               | Vector #148, 0x00000250: User Defined Vector
           .long        0               | Vector #149, 0x00000254: User Defined Vector
           .long        0               | Vector #150, 0x00000258: User Defined Vector
           .long        0               | Vector #151, 0x0000025C: User Defined Vector
           .long        0               | Vector #152, 0x00000260: User Defined Vector
           .long        0               | Vector #153, 0x00000264: User Defined Vector
           .long        0               | Vector #154, 0x00000268: User Defined Vector
           .long        0               | Vector #155, 0x0000026C: User Defined Vector
           .long        0               | Vector #156, 0x00000270: User Defined Vector
           .long        0               | Vector #157, 0x00000274: User Defined Vector
           .long        0               | Vector #158, 0x00000278: User Defined Vector
           .long        0               | Vector #159, 0x0000027C: User Defined Vector

           .org         0x280
           .long        _intMfpGpi0        | Vector 0xA0: General Purpose Interrupt 0 (I0)
           .long        _intMfpGpi1        | Vector 0xA1: General Purpose Interrupt 1 (I1)
           .long        _intMfpGpi2        | Vector 0xA2: General Purpose Interrupt 2 (I2)
           .long        _intMfpGpi3        | Vector 0xA3: General Purpose Interrupt 3 (I3)
           .long        _intMfpTmrD        | Vector 0xA4: Timer D
           .long        _intMfpTmrC        | Vector 0xA5: Timer C
           .long        _intMfpGpi4        | Vector 0xA6: General Purpose Interrupt 4 (I4)
           .long        _intMfpGpi5        | Vector 0xA7: General Purpose Interrupt 5 (I5)
           .long        0x008001D0         | Vector 0xA8: Timer B / OSTick ISR entry
           .long        _intMfpXmitErr     | Vector 0xA9: Transmit Error
           .long        _intMfpXmitBufEmpty | Vector 0xAA: Transmit Buffer Empty
           .long        _intMfpRecErr      | Vector 0xAB: Receive Error
           .long        _intMfpRecBufFull  | Vector 0xAC: Receiver Buffer Full
           .long        _intMfpTmrA        | Vector 0xAD: Timer A
           .long        _intMfpGpi6        | Vector 0xAE: General Purpose Interrupt 6 (I6)
           .long        _intMfpGpi7        | Vector 0xAF: General Purpose Interrupt 7 (I7)

__exit:                                | exit() functions
_exit:
           lea         __stack,%a7      | reset stack
           trap        #15
           .word        0               | IDE68K system call 0 -> EXIT (no return)
           jsr         main           | when re-started after exit() call
           rts

__putch:                               | Basic character output routine
           move.l      4(%a7),%d0
           trap        #15
           .word        1               | IDE68K system call 1 -> PUTCH
__putch_rts:
           rts

__getch:                               | Basic character input routine
           trap        #15
           .word        3               | IDE68K system call 3 -> GETCHE
           ext.w       %d0
           ext.l       %d0              | %d0.L is char sign extended to 32 bits
           rts

__kbhit:
           trap        #15
           .word        4               | IDE68K system call 4 -> KBHIT
           sne         %d0              | %d0.B = 0xFF if char in buffer, 0x00 if not
           rts

           .extern __allocp
           .extern videoFontes

           .org 0x400
os_boot_entry:
            and.w   #0xF8FF,%sr            | mascara de interrup????o nao setada, 0
            move.l  #_fontesMatrix,%d1   | colocar endereco da variavel que vai ser de ponteiro para a posicao das fontes apos a compilacao
            move.l  %d1,videoFontes              | colocar endereco apos a compilacao de onde estao as fontes para VGA
            clr.l __allocp
            bra main

monitor_func_table:
           .long processCmd
           .long clearScr
           .long printText
           .long printChar
           .long delayms
           .long loadSerialToMem
           .long readChar
           .long hideCursor
           .long showCursor
           .long inputLine
           .long modeVideo
           .long vdp_init
           .long vdp_colorize
           .long vdp_plot_hires
           .long vdp_plot_color
           .long vdp_set_bdcolor
           .long vdp_set_pattern_color
           .long vdp_set_cursor
           .long vdp_set_cursor_pos
           .long vdp_textcolor
           .long vdp_write
           .long vdp_set_sprite_pattern
           .long vdp_sprite_color
           .long vdp_sprite_get_attributes
           .long vdp_sprite_get_position
           .long vdp_sprite_init
           .long vdp_sprite_set_position
           .long writeLongSerial
           .long writeSerial
           .long vdp_read_color_pixel
           .long vdp_get_cursor
           .long vdp_get_color
           .long lstmGetSize
           .long vdp_get_cfg
           .long setRegister
           .long read_status_reg
           .long setWriteAddress
           .long setReadAddress
           .long getVideoFontes
           .long readMouse
            
           .section .rodata
           .balign 2
const:                                  | constant data & string data

           .section .data
           .balign 2
data:                                   | initialized data
           .global      __heap
__heap:
           .long      heap              | begin of free memory           

           .section .bss
           .balign 2
           .org 0x600000
bss:                                    | un-initialized data
           .space 1024             | stack for main() function
__stack:
__ungetbuf:
           .space 4               | ungetbuffer for stdio functions
           .section .heapram
           .balign 2
heap:                                   | start of free memory (placed at 0x810000 by linker)

