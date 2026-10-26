/*---------------------------------------------------------------------------*/
/*                            - cstartup.s -                                 */
/*                                                                           */
/*       This module contains the code executed before the C "main"          */
/*       function is called.                                                 */
/*       The code can be tailored to suit special hardware needs.            */
/*                                                                           */
/*       Copyright 2009-2018 IAR Systems AB.                                 */
/*                                                                           */
/*       Entries:                                                            */
/*         __iar_program_start       C entry point                           */
/*                                                                           */
/*                                                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef __ROPI__
#error __ROPI__ not defined
#endif
#include "cfi.m"

;------------------------------------------------------------------------------
; Reset vector.
;------------------------------------------------------------------------------
; The C startup sequence.
;
; Calls __low_level_init to perform early user initialization, after which the
; static data areas are initialized if necessary. Then 'main' and finally 'exit'
; are called.
;------------------------------------------------------------------------------
        module    _CSTARTUP
        section   ISTACK_S:DATA:NOROOT
        section   ISTACK_TR:DATA:NOROOT
        section   USTACK:DATA:NOROOT
        section   .inttable:CONST:NOROOT
#if __CORE__ > __CORE_V1__
        section   .exceptvect:CONST:NOROOT
#endif
        section   .text:CODE:NOROOT(2)
        extern    _main
        extern    _exit
        extern    ___iar_data_init2
        public    __iar_program_start
        require   __iar_main_call
#if __ROPI__
        extern    __SC_BASE
#endif
        require   __cinit
        require   __iar_init_vectors
        CALL_GRAPH_ROOT __iar_program_start, "task"
        cfi Block cfiBlock0 Using cfiCommonUndef
        cfi Function __iar_program_start
__iar_program_start:
;------------------------------------------------------------------------------
; After a reset, execution will start here.
;------------------------------------------------------------------------------
        code

;------------------------------------------------------------------------------
; If the application swithes to user mode, the user stack pointer will
; need initialization.
;------------------------------------------------------------------------------
        mvtc      #(SFE USTACK), USP  ; stack pointer
        cfi EndBlock cfiBlock0
       
       
        section   ISTACK_TR:DATA:NOROOT
        section   .text:CODE:NOROOT
        public    __iar_init_sp_tr
        cfi Block cfiBlock0aa Using cfiCommonUndef
        cfi Function __iar_program_start

__iar_init_sp_tr:
        code
        mvtc      #(SFE ISTACK_TR), ISP  ; istack pointer
        cfi EndBlock cfiBlock0aa


        section   .text:CODE:NOROOT
        cfi Block cfiBlock0aaa Using cfiCommonUndef
        cfi Function __iar_program_start


__iar_init_vectors:
;------------------------------------------------------------------------------
; If the application moves the exception vector, for example when in ropi mode,
; initialize EXTB here (Core RXv2 or RXv3).
;------------------------------------------------------------------------------
#if __CORE__ > __CORE_V1__
  #if __ROPI__
        ; set up EXTB
  #else
        mvtc      #(SFB .exceptvect),EXTB  ; exception vector base pointer
  #endif
#endif

;------------------------------------------------------------------------------
; If the application does not enable interrupts in low_level_init, then the
; initialization of INTB can wait until just before the call to main. This
; allows interrupt handlers and the vector to be placed in RAM.
;------------------------------------------------------------------------------
#if __ROPI__
        ; set up INTB
#else
        mvtc      #(SFB .inttable), INTB ; vector address
#endif
        cfi EndBlock cfiBlock0aaa

        section   .text:CODE:NOROOT
        public    __iar_init_tfu
        extern    __?TFUREGS
        cfi Block cfiBlock0a Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__iar_init_tfu:
        mov.l     #__?TFUREGS,R13
        mov.b     #0x07, [R13]
        mov.b     #0x07, 0x01[R13]
        cfi EndBlock cfiBlock0a

        section   .text:CODE:NOROOT
        public    __iar_init_rwpi
        extern    __rwpi_base
        cfi Block cfiBlock0b Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__iar_init_rwpi:
        mov.l     #__rwpi_base,R13
        cfi EndBlock cfiBlock0b

        section   .text:CODE:NOROOT
        public    __iar_init_FINTV
        extern    ___fast_interrupt
        cfi Block cfiBlock1 Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__iar_init_FINTV:
        mvtc      #___fast_interrupt:32,fintv  ; setup the fast interrupt
        cfi EndBlock cfiBlock1

        section   .text:CODE:NOROOT
        public    __iar_subn_handling
        cfi Block cfiBlock2 Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__iar_subn_handling:
#if __FPU__
        mvtc      #0, fpsw            ; enable subnormal handling
#endif
        cfi EndBlock cfiBlock2
        section   .text:CODE:NOROOT
        public    __iar_call_low_level_init
        extweak   ___low_level_init
        cfi Block cfiBlock3 Using cfiCommonUndef
        cfi Function __iar_program_start
        code

;------------------------------------------------------------------------------
; If hardware must be initiated from assembly or if interrupts
; should be on when reaching main, this is the place to insert
; such code.
;------------------------------------------------------------------------------

__iar_call_low_level_init:
__iar_init$$needed:
        CFI FunCall ___low_level_init
        bsr.a   ___low_level_init
        cmp     #0, r1
        beq     __iar_main_call
        cfi EndBlock cfiBlock3

;------------------------------------------------------------------------------
; Data initialization.
;
; Initialize the static data areas.
;------------------------------------------------------------------------------
        section .text:CODE:NOROOT
        cfi Block cfiBlock4 Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__cinit:
        CFI FunCall ___iar_data_init2
        bsr     ___iar_data_init2
        cfi EndBlock cfiBlock4

;------------------------------------------------------------------------------
; Call C++ static constructors.
;------------------------------------------------------------------------------

        section .text:CODE:NOROOT
        public  __iar_cstart_call_ctors2
        extweak ___iar_dynamic_initialization
        cfi Block cfiBlock5 Using cfiCommonUndef
        cfi Function __iar_program_start
        code
__iar_cstart_call_ctors2:
        bsr.a   ___iar_dynamic_initialization
        cfi FunCall ___iar_dynamic_initialization
        cfi EndBlock cfiBlock5

;------------------------------------------------------------------------------
; Call 'main' C function.
;------------------------------------------------------------------------------

        section .text:CODE:NOROOT
        extern PID$$Base
        extern PID$$Limit
        public  __iar_cstart_end
        public  __iar_main_call

;------------------------------------------------------------------------------
; Switch to user mode here if desired.
;------------------------------------------------------------------------------

__iar_init$$done2:
__iar_main_call:
        cfi Block cfiBlock6 Using cfiCommonUndef
        cfi Function __iar_program_start
        code
#if __ROPI__
#else
        extern    __iar_reset_vector
        extern    _vector_table
        require   _vector_table
        require   __iar_reset_vector
#endif
;------------------------------------------------------------------------------
; Init INTB here if interrupt handlers should be placed in RAM.
;------------------------------------------------------------------------------
        ;mvtc      #(SFB .inttable), INTB
        mov.l     #0,r1     ; set argc = 0
        CFI FunCall _main
        bsr.a     _main
        CFI FunCall _exit
        bra.a     _exit
        cfi EndBlock cfiBlock6
__iar_cstart_end:

        end
