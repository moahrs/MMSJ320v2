.text
.global _runSO
.global runSO
.global _runCmd
.global runCmd
.global _runBas
.global runBas
.global _runOSCmd
.global runOSCmd
.global _trapFunc0
.global _trapFuncs
.global _intSpuriousInt
.global _intPIC
.global _intUsbSerial
.global _intVideo
.global _intMouse
.global _intKeyboard
.global _intMfpGpi0
.global _intMfpGpi1
.global _intMfpGpi2
.global _intMfpGpi3
.global _intMfpTmrD
.global _intMfpTmrC
.global _intMfpGpi4
.global _intMfpGpi5
.global _intMfpTmrB
.global _intMfpXmitErr
.global _intMfpXmitBufEmpty
.global _intMfpRecErr
.global _intMfpRecBufFull
.global _intMfpTmrA
.global _intMfpGpi6
.global _intMfpGpi7
.global _errorBusAddr
.global _errorIllegalInst
.global _errorZeroDivide
.global _errorChkInst
.global _errorTrapV
.global _errorPrivV
.global _errorProc
.global _traceDebug
| Executar SO pre-carregado no endere???o de memoria 0x00800000
|
_runSO:
runSO:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)
       jsr       0x00800000       | Jump to position ram memory
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5
       rts

_runCmd:
runCmd:
       .extern runMemory
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)
       move.l    runMemory,%a0
       jsr       (%a0)       | Jump to position ram memory
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5
       rts

_runBas:
runBas:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)
       jsr       0x00008000       | Jump to position Basic ram memory
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5
       rts

_runOSCmd:
runOSCmd:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)
       jsr       0x00800170                          | lembrar de ajustar esse endere???o se ele mudar
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5
       rts

| Executar Funcoes Trap #15
|
_trapFunc0:
       rte
_trapFuncs:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       basicFuncBios
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

| Interrupcoes
|
_intSpuriousInt:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcSpuriousInt
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

_intPIC:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntPIC
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

_intUsbSerial:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntUsbSerial
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

_intVideo:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntVideo
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

_intMouse:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMouse
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

_intKeyboard:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntKeyboard
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte

|_intMultiTask:
|       move.l    _intpos,%a1
|       move.l    (%a1),%a0
|       move.l    (%a1),%d0               | compara se (_intpos) = 0
|       cmp.l     #0,%d0
|       beq.s     _intMultiTask_1       | se for = 0, pula para o RTE
|       jmp       (%a0)                  | se nao, chama subrotina de multitask no OS
|_intMultiTask_1:
|       rte

_intMfpGpi0:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi0
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5      | movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi1:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi1
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi2:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi2
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi3:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi3
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpTmrD:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpTmrD
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpTmrC:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpTmrC
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi4:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi4
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi5:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi5
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpTmrB:
       jmp 0x00800022           | Without Push or Pop Registers. uC/OS-II RTOS will do this.
       rte
_intMfpXmitErr:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpXmitErr
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpXmitBufEmpty:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpXmitBufEmpty
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpRecErr:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpRecErr
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpRecBufFull:
       movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpRecBufFull
       movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpTmrA:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpTmrA
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5      | movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi6:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi6
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5      | movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_intMfpGpi7:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       jsr       funcIntMfpGpi7
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5      | movem.l (%a7)+,%a1/%d0       | restore registers
       rte
_errorBusAddr:
       move.w #0x0000,0x0060E208
       jmp _errorProc
_errorIllegalInst:
       move.w #0x0001,0x0060E208
       jmp _errorProc
_errorZeroDivide:
       move.w #0x0002,0x0060E208
       jmp _errorProc
_errorChkInst:
       move.w #0x0003,0x0060E208
       jmp _errorProc
_errorTrapV:
       move.w #0x0004,0x0060E208
       jmp _errorProc
_errorPrivV:
       move.w #0x0005,0x0060E208
       jmp _errorProc
       nop
       nop
_errorProc:
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5/%a6,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       MOVE #65,%d0               | 30 bytes to read (15 words)
       MOVE.l #0x0060E20A,%a0     | error buffer (*errorBufferAddrBus)
ERRORLOOP:
       MOVE.w (%a7)+,(%a0)+        | SP to error buffer
       DBRA %d0,ERRORLOOP         | until all bytes copied to buffer error (%a0)
       move.l  #_fontesMatrix,%d1   | colocar endereco da variavel que vai ser de ponteiro para a posicao das fontes apos a compilacao
       move.l  %d1,0x00611816              | colocar endereco apos a compilacao de onde estao as fontes para VGA
       jmp      funcErrorBusAddr
       rte
_traceDebug:
       MOVE.l %a7,0x0060E692      | SP to %a7 temp buffer
       movem.l   %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)      | movem.l %a1/%d0,-(%a7)       | save registers
       MOVE.l 0x0060E692,%a1      | trace %a7 temp buffer
       MOVE.l 0x0060E68C,%a0      | trace buffer (*traceData)
       MOVE #7,%d0                 | 8 words
TRACELOOP:
       move.w (%a1)+,(%a0)+
       DBRA %d0,TRACELOOP          | until all bytes copied to buffer trace (%a0)
       MOVE.l %a0,0x0060E68C      | trace buffer (*traceData)
       movem.l   (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5      | movem.l (%a7)+,%a1/%d0       | restore registers
       rte
       
