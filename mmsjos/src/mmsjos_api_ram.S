.text
.global _ltoa
.global _itoa
.global _strcpy
.global _strcat
.global _toupper
.global _memset

_ltoa:    jmp 0x0080000C
_itoa:    jmp 0x00800012
_strcpy:  jmp 0x00800018
_strcat:  jmp 0x0080001E
_toupper: jmp 0x00800012
_memset:  jmp 0x00800018

.extern OSPrioHighRdy
.extern OSRdyGrp
.extern OSRdyTbl
.extern OSRunning
.extern OSTaskSwHook
.extern OSTCBCur
.extern OSTCBHighRdy
.extern OSTCBPrioTbl
.extern OSTimeTick
.extern OSUnMapTbl

_OSStartHighRdy = OSStartHighRdy
_OSCtxSw = OSCtxSw
_OSIntCtxSw = OSIntCtxSw
_OSTickISR = OSTickISR
_OSIntExit68K = OSIntExit68K

OSStartHighRdy:
    jsr OSTaskSwHook
    addq.b #1,OSRunning
    move.l OSTCBHighRdy,%a0
    move.l (%a0),%a7
    movem.l (%a7)+,%a0-%a6/%d0-%d7
    rte

OSCtxSw:
    movem.l %a0-%a6/%d0-%d7,-(%a7)
    move.l OSTCBCur,%a0
    move.l %a7,(%a0)
    jsr OSTaskSwHook
    move.l OSTCBHighRdy,%a0
    move.l %a0,OSTCBCur
    move.l (%a0),%a7
    move.b OSPrioHighRdy,OSPrioCur
    movem.l (%a7)+,%a0-%a6/%d0-%d7
    rte

OSIntCtxSw:
    adda.l #10,%a7
    move.l OSTCBCur,%a1
    move.l %a7,(%a1)
    jsr OSTaskSwHook
    move.l OSTCBHighRdy,%a1
    move.l %a1,OSTCBCur
    move.l (%a1),%a7
    move.b OSPrioHighRdy,OSPrioCur
    movem.l (%a7)+,%a0-%a6/%d0-%d7
    rte

OSIntExit68K:
    subq.b #1,OSIntNesting
    bne OSIntExit68K_1
    tst.b OSLockNesting
    bne OSIntExit68K_1
    move.w 60(%a7),%d0
    and.w #0x0700,%d0
    bne OSIntExit68K_1

    lea OSUnMapTbl,%a0
    clr.l %d0
    move.b OSRdyGrp,%d0
    move.b 0(%a0,%d0.l),%d1

    lea OSRdyTbl,%a0
    clr.l %d0
    move.b %d1,%d0
    lea 0(%a0,%d0.l),%a0
    clr.l %d0
    move.b (%a0),%d0

    lea OSUnMapTbl,%a0
    lea 0(%a0,%d0.l),%a0
    move.b %d1,%d0
    lsl.b #3,%d0
    add.b (%a0),%d0
    move.b %d0,OSPrioHighRdy

    cmp.b OSPrioCur,%d0
    beq.s OSIntExit68K_1

    lea OSTCBPrioTbl,%a0
    clr.l %d1
    move.b %d0,%d1
    lsl.l #2,%d1
    lea 0(%a0,%d1.l),%a0
    move.l (%a0),OSTCBHighRdy

    addq.l #1,OSCtxSwCtr
    move.l OSTCBCur,%a0
    move.l %a7,(%a0)
    jsr OSTaskSwHook
    move.l OSTCBHighRdy,%a0
    move.l %a0,OSTCBCur
    move.b OSPrioHighRdy,OSPrioCur
    move.l (%a0),%a7

OSIntExit68K_1:
    movem.l (%a7)+,%a0-%a6/%d0-%d7
    rte

OSTickISR:
    or.w #0x0700,%sr
    addq.b #1,OSIntNesting
    movem.l %a0-%a6/%d0-%d7,-(%a7)
    jsr OSTimeTick
    bra OSIntExit68K
