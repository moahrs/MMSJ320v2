;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                            (c) Copyright 1999, Jean J. Labrosse, Weston, FL
;                                          All Rights Reserved
;
;
;                                      68000 specific assembly code
;                                        with 68881 FPU support
;
; File         : OS_FCPU_A.ASM
; By           : Jean J. Labrosse, Peter J. Fondse
;********************************************************************************************************


;********************************************************************************************************
;                                            REVISION HISTORY
;
; $Log$
;
;********************************************************************************************************


;********************************************************************************************************
;                                          PUBLIC DECLARATIONS
;********************************************************************************************************

        xdef   _OSIntCtxSw                     ; Satisfy OSIntExit() in OS_CORE.C
        xdef   _OSStartHighRdy
        xdef   _OSCtxSw
        xdef   _OSTickISR
        xdef   OSIntExit68K

;********************************************************************************************************
;                                         EXTERNAL DECLARATIONS
;********************************************************************************************************

        xref   _OSCtxSwCtr
        xref   _OSIntExit
        xref   _OSIntNesting
        xref   _OSLockNesting
        xref   _OSPrioCur
        xref   _OSPrioHighRdy
        xref   _OSRdyGrp
        xref   _OSRdyTbl
        xref   _OSRunning
        xref   _OSTaskSwHook
        xref   _OSTCBCur
        xref   _OSTCBHighRdy
        xref   _OSTCBPrioTbl
        xref   _OSTimeTick
        xref   _OSUnMapTbl

;********************************************************************************************************
;                               START HIGHEST PRIORITY TASK READY-TO-RUN
;
; Description : This function is called by OSStart() to start the highest priority task that was created
;               by your application before calling OSStart().
;
; Arguments   : none
;
; Note(s)     : 1) The stack frame is assumed to look as follows:
;
;                  OSTCBHighRdy->OSTCBStkPtr +  0  ---->  D0    (H)        Low Memory
;                                            +  2         D0    (L)
;                                            +  4         D1    (H)
;                                            +  6         D1    (L)
;                                            +  8         D2    (H)
;                                            + 10         D2    (L)
;                                            + 12         D3    (H)
;                                            + 14         D3    (L)
;                                            + 16         D4    (H)
;                                            + 18         D4    (L)
;                                            + 20         D5    (H)
;                                            + 22         D5    (L)
;                                            + 24         D6    (H)
;                                            + 26         D6    (L)
;                                            + 28         D7    (H)
;                                            + 30         D7    (L)
;                                            + 32         A0    (H)
;                                            + 34         A0    (L)
;                                            + 36         A1    (H)
;                                            + 38         A1    (L)
;                                            + 40         A2    (H)
;                                            + 42         A2    (L)
;                                            + 44         A3    (H)
;                                            + 46         A3    (L)
;                                            + 48         A4    (H)
;                                            + 50         A4    (L)
;                                            + 52         A5    (H)
;                                            + 54         A5    (L)
;                                            + 56         A6    (H)
;                                            + 58         A6    (L)
;                                            + 60         OS_INITIAL_SR
;                                            + 62         task  (H)
;                                            + 64         task  (L)
;                                            + 66         task  (H)
;                                            + 68         task  (L)
;                                            + 70         pdata (H)
;                                            + 72         pdata (L)        High Memory
;
;               2) OSStartHighRdy() MUST:
;                      a) Call OSTaskSwHook() then,
;                      b) Set OSRunning to TRUE,
;                      c) Switch to the highest priority task.
;********************************************************************************************************

        SECTION   code

_OSStartHighRdy:
        JSR       _OSTaskSwHook            ; Invoke user defined context switch hook
        ADDQ.B    #1,_OSRunning            ; Indicate that we are multitasking
        MOVE.L    _OSTCBHighRdy,A0         ; Point to TCB of highest priority task ready to run
        MOVE.L    (A0),A7                  ; Get the stack pointer of the task to resume
        MOVEM.L   (A7)+,A0-A6/D0-D7        ; Restore the CPU registers
        RTE                                ; Run task

;********************************************************************************************************
;                                       TASK LEVEL CONTEXT SWITCH
;
; Description : This function is called when a task makes a higher priority task ready-to-run.
;               Called through TRAP #0
;
; Arguments   : none
;
; Note(s)     : 1) Upon entry,
;                  OSTCBCur     points to the OS_TCB of the task to suspend
;                  OSTCBHighRdy points to the OS_TCB of the task to resume
;
;               2) The stack frame of the task to suspend looks as follows (the registers for
;                  task to suspend need to be saved):
;
;                                         SP +  0  ---->  SR                   Low Memory
;                                            +  2         PC of task  (H)
;                                            +  4         PC of task  (L)      High Memory
;
;               3) The stack frame of the task to resume looks as follows:
;
;                  OSTCBHighRdy->OSTCBStkPtr +  0  ---->  D0    (H)           Low Memory
;                                            +  2         D0    (L)
;                                            +  4         D1    (H)
;                                            +  6         D1    (L)
;                                            +  8         D2    (H)
;                                            + 10         D2    (L)
;                                            + 12         D3    (H)
;                                            + 14         D3    (L)
;                                            + 16         D4    (H)
;                                            + 18         D4    (L)
;                                            + 20         D5    (H)
;                                            + 22         D5    (L)
;                                            + 24         D6    (H)
;                                            + 26         D6    (L)
;                                            + 28         D7    (H)
;                                            + 30         D7    (L)
;                                            + 32         A0    (H)
;                                            + 34         A0    (L)
;                                            + 36         A1    (H)
;                                            + 38         A1    (L)
;                                            + 40         A2    (H)
;                                            + 42         A2    (L)
;                                            + 44         A3    (H)
;                                            + 46         A3    (L)
;                                            + 48         A4    (H)
;                                            + 50         A4    (L)
;                                            + 52         A5    (H)
;                                            + 54         A5    (L)
;                                            + 56         A6    (H)
;                                            + 58         A6    (L)
;                                            + 60         OS_INITIAL_SR       (See OS_CPU.H)
;                                            + 62         PC of task  (H)
;                                            + 64         PC of task  (L)     High Memory
;********************************************************************************************************

_OSCtxSw:
        MOVEM.L   A0-A6/D0-D7,-(A7)              ; Save the registers of the current task
        MOVE.L    _OSTCBCur,A0                   ; Save the stack pointer in the suspended task TCB
        MOVE.L    A7,(A0)
        JSR       _OSTaskSwHook                  ; Invoke user defined context switch hook
        MOVE.L    _OSTCBHighRdy,A0               ; OSTCBCur  = OSTCBHighRdy
        MOVE.L    A0,_OSTCBCur
        MOVE.L    (A0),A7                        ; Get the stack pointer of the task to resume
        MOVE.B    _OSPrioHighRdy,_OSPrioCur      ; OSPrioCur = OSPrioHighRdy
        MOVEM.L   (A7)+,A0-A6/D0-D7              ; Restore the CPU registers
        RTE                                      ; Run task

;********************************************************************************************************
;                                      INTERRUPT LEVEL CONTEXT SWITCH
;
; Description : This function is called from OSIntExit() in OS_CORE.C
;               Provided for backward compatibility. The ISR MUST NOT call OSIntExit(), but should jump
;               to OSIntExit68K().
;********************************************************************************************************

_OSIntCtxSw:
        ADDA.L    #10,A7                         ; Adjust the stack
        MOVE.L    _OSTCBCur,A1                   ; Save the stack pointer in the suspended task TCB
        MOVE.L    A7,(A1)
        JSR       _OSTaskSwHook                  ; Invoke user defined context switch hook
        MOVE.L    _OSTCBHighRdy,A1               ; OSTCBCur  = OSTCBHighRdy
        MOVE.L    A1,_OSTCBCur
        MOVE.L    (A1),A7                        ; Get the stack pointer of the task to resume
        MOVE.B    _OSPrioHighRdy,_OSPrioCur      ; OSPrioCur = OSPrioHighRdy
        MOVEM.L   (A7)+,A0-A6/D0-D7              ; Restore the CPU registers
        RTE                                      ; Run task

;********************************************************************************************************
;                           INTERRUPT EXIT FUNCTION (68K specific)
;
; Description : ISR's (written in Assembly) must directly JUMP to OSIntExit68K
;
; Notes       : You must NOT use OSIntExit() to exit an ISR with the 68K, but OSIntExit68K().
;
; Stack frame upon entry:
;
;                  SP +  0  ---->  D0    (H)
;                     +  2         D0    (L)
;                     +  4         D1    (H)
;                     +  6         D1    (L)
;                     +  8         D2    (H)
;                     + 10         D2    (L)
;                     + 12         D3    (H)
;                     + 14         D3    (L)
;                     + 16         D4    (H)
;                     + 18         D4    (L)
;                     + 20         D5    (H)
;                     + 22         D5    (L)
;                     + 24         D6    (H)
;                     + 26         D6    (L)
;                     + 28         D7    (H)
;                     + 30         D7    (L)
;                     + 32         A0    (H)
;                     + 34         A0    (L)
;                     + 36         A1    (H)
;                     + 38         A1    (L)
;                     + 40         A2    (H)
;                     + 42         A2    (L)
;                     + 44         A3    (H)
;                     + 46         A3    (L)
;                     + 48         A4    (H)
;                     + 50         A4    (L)
;                     + 52         A5    (H)
;                     + 54         A5    (L)
;                     + 56         A6    (H)
;                     + 58         A6    (L)
;                     + 60         Task or ISR's SR
;                     + 62         PC of task  (H)
;                     + 64         PC of task  (L)                   High Memory
;********************************************************************************************************

OSIntExit68K:
        SUBQ.B    #1,_OSIntNesting              ; if (--OSIntNesting == 0)
        BNE       OSIntExit68K_1
        TST.B     _OSLockNesting                ; if (OSLockNesting == 0)
        BNE       OSIntExit68K_1
        MOVE.W    (60,A7),D0                    ;  if (LAST nested ISR)
        AND.W     #$0700,D0
        BNE       OSIntExit68K_1
        LEA       _OSUnMapTbl,A0                ;  y = OSUnMapTbl[OSRdyGrp];
        CLR.L     D0
        MOVE.B    _OSRdyGrp,D0
        MOVE.B    0(A0,D0.L),D1                 ;  y in D1
        LEA       _OSRdyTbl,A0                  ;  OSPrioHighRdy = (INT8U)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
        CLR.L     D0
        MOVE.B    D1,D0
        LEA       0(A0,D0.L),A0
        CLR.L     D0
        MOVE.B    (A0),D0                       ;  OSRdyTbl[y] in D0
        LEA       _OSUnMapTbl,A0
        LEA       0(A0,D0.L),A0                 ;  &OSUnMapTbl[OSRdyTbl[y]] in A0
        MOVE.B    D1,D0
        LSL.B     #3,D0                         ;  (y << 3) in D0
        ADD.B     (A0),D0
        MOVE.B    D0,_OSPrioHighRdy
        CMP.B     _OSPrioCur,D0                 ;  if (OSPrioCur != OSPrioHighRdy) {
        BEQ.S     OSIntExit68K_1
        LEA       _OSTCBPrioTbl,A0              ;    OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy];
        CLR.L     D1
        MOVE.B    D0,D1
        LSL.L     #2,D1
        LEA       0(A0,D1.L),A0
        MOVE.L    (A0),_OSTCBHighRdy
        ADDQ.L    #1,_OSCtxSwCtr                ;    OSCtxSwCtr++;
        MOVE.L    _OSTCBCur,A0                  ;    Save the stack pointer in the suspended task TCB
        MOVE.L    A7,(A0)
        JSR       _OSTaskSwHook                 ;    Invoke user defined context switch hook
        MOVE.L    _OSTCBHighRdy,A0              ;    OSTCBCur  = OSTCBHighRdy
        MOVE.L    A0,_OSTCBCur
        MOVE.B    _OSPrioHighRdy,_OSPrioCur     ;    OSPrioCur = OSPrioHighRdy
        MOVE.L    (A0),A7                       ;    Get the stack pointer of the task to resume
OSIntExit68K_1:
        MOVEM.L   (A7)+,A0-A6/D0-D7             ;  Restore the CPU registers
        RTE                                     ;  Return to task or nested ISR

;********************************************************************************************************
;                                           SYSTEM TICK ISR
;
; Description : This function is the ISR used to notify uC/OS-II that a system tick has occurred.
;
; Arguments   : none
;
; Notes       : 1) You MUST increment 'OSIntNesting' and NOT call OSIntEnter()
;               2) You MUST save ALL the CPU registers as shown below
;               3) You MUST JUMP to OSIntExit68K() instead of call the function.
;********************************************************************************************************

_OSTickISR:
        OR.W      #$0700,SR                     ; Disable ALL interrupts
        ADDQ.B    #1,_OSIntNesting              ; OSIntNesting++;
        MOVEM.L   A0-A6/D0-D7,-(A7)             ; Save the registers of the current task
        JSR       _OSTimeTick                   ; Call uC/OS-II's tick updating function
        BRA       OSIntExit68K                  ; Exit ISR

;*********************************************************************************************************
;                                           SAVE FPU REGISTERS
;                                        void OSFPSave(void *pblk)
;
; Description : This function is called to save the contents of the FPU registers during a context
;               switch.  It is assumed that a pointer to a storage area for the FPU registers is placed
;               in the task's TCB (i.e. .OSTCBExtPtr).
; Arguments   : pblk is passed to this function when called.
; Note(s)     : 1) The stack frame upon entry looks as follows:
;
;                      SP + 0 -> Return address(H) of function (Low memory)
;                         + 2    Return address(L) of function
;                         + 4    pblk(H)
;                         + 6    pblk(L)                       (High memory)
;*********************************************************************************************************

_OSFPSave:
        MOVE.L    4(A7),A0
        FMOVEM.X  FP0-FP7,(A0)
        RTS

;*********************************************************************************************************
;                                           RESTORE FPU REGISTERS
;                                       void OSFPRestore(void *pblk)
;
; Description : This function is called to restore the contents of the FPU registers during a context
;               switch.  It is assumed that a pointer to a storage area for the FPU registers is placed
;               in the task's TCB (i.e. .OSTCBExtPtr).
; Arguments   : pblk is passed to this function when called.
; Note(s)     : 1) The stack frame upon entry looks as follows:
;
;                      SP + 0 -> Return address(H) of function (Low memory)
;                         + 2    Return address(L) of function
;                         + 4    pblk(H)
;                         + 6    pblk(L)                       (High memory)
;*********************************************************************************************************

_OSFPRestore:
        MOVE.L    4(A7),A0
        FMOVEM.X  (A0),FP0-FP7
        RTS
