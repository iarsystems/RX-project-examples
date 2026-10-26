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
        SECTION .nmivec:CONST:ROOT
        EXTERN  ___privileged_handler
        EXTERN  ___violation_handler
        EXTERN  ___undefined_handler
        EXTERN  ___NMI_handler
        EXTERN  __float_placeholder
        EXTERN  __iar_program_start
        EXTWEAK __MDES
        EXTWEAK __OFS1 
        EXTWEAK __OFS0        
        EXTERN  __ID_BYTES_1_4
        EXTERN  __ID_BYTES_5_8
        EXTERN  __ID_BYTES_9_12
        EXTERN  __ID_BYTES_13_16
        PUBLIC  __iar_reset_vector

        DATA
        DC32    __MDES   // MDES register for RX630 group
        DS32    1 // reserved
        DC32    __OFS1   // OFS1 register for RX630 group
        DC32    __OFS0   // OFS0 register for RX630 group
        DS32    4 // reserved
        DC32    __ID_BYTES_1_4
        DC32    __ID_BYTES_5_8
        DC32    __ID_BYTES_9_12
        DC32    __ID_BYTES_13_16
        DS32    8 // reserved
        DC32    ___privileged_handler
        DC32    ___violation_handler
        DS32    1 // reserved
        DC32    ___undefined_handler
        DS32    1 // reserved
        DC32    __float_placeholder
        DS32    1 // reserved
        DS32    1 // reserved
        DS32    1 // reserved
        DS32    1 // reserved
        DC32    ___NMI_handler
__iar_reset_vector:
        DC32    __iar_program_start
        END
