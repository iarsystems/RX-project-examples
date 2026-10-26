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
        PUBLIC  __iar_reset_vector

        DATA
__iar_reset_vector:
        DC32    __iar_program_start
        END
