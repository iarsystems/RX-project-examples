; Copyright 2009-2018 IAR Systems AB

#ifndef CFI_M54
#define CFI_M54

; ----------------------------------------------
; Common names block.
;

        CFI Names cfiNames0
        CFI Resource SP:32
        CFI Resource R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32, R8:32
        CFI Resource R9:32, R10:32, R11:32, R12:32, R13:32, R14:32, R15:32
        CFI VirtualResource ?RET:32
        CFI StackFrame CFA SP DATA
        CFI EndNames cfiNames0

XCFI_NAMES MACRO
        CFI Names cfiNames
        CFI Resource SP:32
        CFI Resource R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32, R8:32
        CFI Resource R9:32, R10:32, R11:32, R12:32, R13:32, R14:32, R15:32
        CFI VirtualResource ?RET:32
        CFI StackFrame CFA SP DATA
        CFI EndNames cfiNames
        ENDM

; ----------------------------------------------
; Common block for functions using the normal 
; calling convention.
;

        CFI Common cfiCommonCC Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 Undefined
        CFI R4 Undefined
        CFI R5 Undefined
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 Undefined
        CFI R15 Undefined
        CFI EndCommon cfiCommonCC

XCFI_COMMON MACRO
        CFI Common cfiCommon Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 Undefined
        CFI R4 Undefined
        CFI R5 Undefined
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 Undefined
        CFI R15 Undefined
        CFI EndCommon cfiCommon
        ENDM

; ----------------------------------------------
; Common block for functions with normal scratch
; register R1 only. 
;
        CFI Common cfiCommonR1 Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 SameValue
        CFI R3 SameValue
        CFI R4 SameValue
        CFI R5 SameValue
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 SameValue
        CFI R15 SameValue
        CFI EndCommon cfiCommonR1

; ----------------------------------------------
; Common block for functions taking an additional
; parameter in R2.
;
        CFI Common cfiCommonR2R1 Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 SameValue
        CFI R4 SameValue
        CFI R5 SameValue
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 SameValue
        CFI R15 SameValue
        CFI EndCommon cfiCommonR2R1

; ----------------------------------------------
; Common block for functions taking parameters
; in R3 and R2:R1.
;
        CFI Common cfiCommonR3R1 Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 Undefined
        CFI R4 SameValue
        CFI R5 SameValue
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 SameValue
        CFI R15 SameValue
        CFI EndCommon cfiCommonR3R1

; ----------------------------------------------
; Common block for functions taking parameters
; in R4:R3 and R2:R1.
;
        CFI Common cfiCommonR4R1 Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 Undefined
        CFI R4 Undefined
        CFI R5 SameValue
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 SameValue
        CFI R15 SameValue
        CFI EndCommon cfiCommonR4R1

; ----------------------------------------------
; Common block for functions with all resources
; undefined.
;
        CFI Common cfiCommonUndef Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+4
        CFI ?RET Frame(CFA, -4)
        CFI R1 Undefined
        CFI R2 Undefined
        CFI R3 Undefined
        CFI R4 Undefined
        CFI R5 Undefined
        CFI R6 Undefined
        CFI R7 Undefined
        CFI R8 Undefined
        CFI R9 Undefined
        CFI R10 Undefined
        CFI R11 Undefined
        CFI R12 Undefined
        CFI R13 Undefined
        CFI R14 Undefined
        CFI R15 Undefined
        CFI EndCommon cfiCommonUndef
        
        
; ----------------------------------------------
; Common block for functions with interrupt.
;
        CFI Common cfiCommonInt Using cfiNames0
        CFI CodeAlign 1
        CFI DataAlign 1
        CFI ReturnAddress ?RET CODE
        CFI CFA SP+8
        CFI ?RET Frame(CFA, -8)
        CFI R1 SameValue
        CFI R2 SameValue
        CFI R3 SameValue
        CFI R4 SameValue
        CFI R5 SameValue
        CFI R6 SameValue
        CFI R7 SameValue
        CFI R8 SameValue
        CFI R9 SameValue
        CFI R10 SameValue
        CFI R11 SameValue
        CFI R12 SameValue
        CFI R13 SameValue
        CFI R14 SameValue
        CFI R15 SameValue
        CFI EndCommon cfiCommonInt
        

#endif
