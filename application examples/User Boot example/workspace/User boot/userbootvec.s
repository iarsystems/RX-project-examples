;-----------------------------------------------------------------------------
; userbootvec table.
;
;             
;

        // This segment part is marked as ROOT, since it must
        // be preserved by the linker.
        MODULE  NMIVEC
        SECTION .userbootvec:CONST:ROOT
        EXTERN  __iar_program_start_ub
        PUBLIC  __iar_reset_vector

        DATA
        DC32    0x55736572           // UB_CODE_A_UPPER   
        DC32    0x426F6F74           // UB_CODE_A_LOWER
        DC32    0xFFFFFF07           // UB_CODE_B_UPPER
        DC32    0x0008C04C           // UB_CODE_B_LOWER
        DC32    0xFFFFFFFF           // MDEB register for RX63N group
__iar_reset_vector:
        DC32    __iar_program_start_ub
        END
