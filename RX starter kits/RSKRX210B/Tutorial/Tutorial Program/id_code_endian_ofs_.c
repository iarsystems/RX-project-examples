/*******************************************************************************
    ID Code Setup
*******************************************************************************/
#pragma public_equ="__ID_BYTES_1_4",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_5_8",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_9_12",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_13_16",0xFFFFFFFF

/*******************************************************************************
    Set the Endian Select Register (ROM based register)
    MDES register (Single Chip Mode)
*******************************************************************************/
#if __LITTLE_ENDIAN__
#pragma public_equ="__MDES",0xFFFFFFFF
#else
#pragma public_equ="__MDES",0xFFFFFFF8
#endif

/*******************************************************************************
    Option Function Select Register 0 (OFS0)
*******************************************************************************/
#pragma public_equ="__OFS0",0xFFFFFFFF

/*******************************************************************************
    Option Function Select Register 1 (OFS1)
*******************************************************************************/
#pragma public_equ="__OFS1",0xFFFFFFFF


