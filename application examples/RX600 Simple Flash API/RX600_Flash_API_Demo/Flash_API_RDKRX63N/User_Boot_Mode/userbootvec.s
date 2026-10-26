;-----------------------------------------------------------------------------
; Fixed interrupt table.  We install all fixed interrupts in
; a section called NMIVEC.  All fixed interrupts have a
; hard coded name that is default handled in this file.  
; See fixedint.c for information how to replace them with handlers written in C.
;
;             
;

        // This segment part is marked as ROOT, since it must
        // be preserved by the linker.
        MODULE  NMIVEC
        SECTION .userbootvec:CONST:ROOT
        EXTERN  __iar_program_start
        EXTERN  __UB_CODE_A_UPPER
        EXTERN  __UB_CODE_A_LOWER
        EXTERN  __UB_CODE_B_UPPER
        EXTERN  __UB_CODE_B_LOWER
        EXTWEAK __MDEB
        PUBLIC  __iar_reset_vector

        DATA
        DC32    __UB_CODE_A_UPPER
        DC32    __UB_CODE_A_LOWER
        DC32    __UB_CODE_B_UPPER
        DC32    __UB_CODE_B_LOWER
        DC32    __MDEB           // MDEB register for RX63N group
__iar_reset_vector:
        DC32    __iar_program_start
        END
