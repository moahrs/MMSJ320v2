.text
.global _start
.global __heap
.global __stack
.global __ungetbuf
.global _uCOSII_TICK
.global _exit
.global __exit

.extern mmsjos_main
.extern mousePointer
.extern mouseHourGlass
.extern iconesMenuSys
.extern fsGetDirAtuData
.extern fsSetClusterDir
.extern fsGetClusterDir
.extern fsSectorWrite
.extern fsSectorRead
.extern fsFindDirPath
.extern fsOsCommand
.extern fsCreateFile
.extern fsOpenFile
.extern fsCloseFile
.extern fsInfoFile
.extern fsFree
.extern fsReadFile
.extern fsWriteFile
.extern fsDelFile
.extern fsRenameFile
.extern loadFile
.extern fsMakeDir
.extern fsChangeDir
.extern fsRemoveDir
.extern OSTimeDlyHMSM
.extern fsFindInDir
.extern fsMalloc
.extern fsFindNextCluster
.extern fsFindClusterFree
.extern fsSetMfp
.extern fsGetMfp

/* IDE68K used absolute ORG directives; with ELF+linker this would pad the binary. */
#ifdef IDE68K
.org 0x00800000
#endif
_start:
    or.w #0x0700,%sr
    move.l #spritePointer,%d1
    move.l %d1,mousePointer
    move.l #spriteHourGlass,%d1
    move.l %d1,mouseHourGlass
    move.l #iconesMenu,%d1
    move.l %d1,iconesMenuSys
    jmp mmsjos_main

mmsjos_func_table:
    .long fsGetDirAtuData
    .long fsSetClusterDir
    .long fsGetClusterDir
    .long fsSectorWrite
    .long fsSectorRead
    .long fsFindDirPath
    .long fsOsCommand
    .long fsCreateFile
    .long fsOpenFile
    .long fsCloseFile
    .long fsInfoFile
    .long fsFree
    .long fsReadFile
    .long fsWriteFile
    .long fsDelFile
    .long fsRenameFile
    .long loadFile
    .long fsMakeDir
    .long fsChangeDir
    .long fsRemoveDir
    .long OSTimeDlyHMSM
    .long fsFindInDir
    .long fsMalloc
    .long fsFindNextCluster
    .long fsFindClusterFree
    .long fsSetMfp
    .long fsGetMfp

__exit:
_exit:
    lea __stack,%a7
    trap #15
    .word 0
    jmp main

_uCOSII_TICK:
    .long OSTickISR

.section .data
.balign 2
__heap:
    .long heap

#ifdef IDE68K
.section .bss
.balign 2
.org 0x602000
.space 1024
__stack:
__ungetbuf:
.space 4

.section .heap
.balign 2
.org 0x818000
heap:
#endif
