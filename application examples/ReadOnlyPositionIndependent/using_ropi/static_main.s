; Absolute module.
; To execute a ropi application, there must be a static part, 
; a startup program, as the reset vector must be static.
; Here, the PID base register is initialized followed by a jump 
; to the ropi application.

        section .text:CODE:NOROOT(2)
        public _main
        extern    _printf
        require   _printf  ; make static
        extern    ___DebugBreak  ; low-level generic debugger break point
        require   ___DebugBreak
        code
_main:
        mov.l   #0xFFF50000,R13 ; start address and ROPI data base address
        mov.l   #0x8000,R12     ; set up base address for RWPI
        mov.l   #0x8800,SP      ; set up stack pointer for RWPI module
        jmp     R13             ; start the ropi application

        end
