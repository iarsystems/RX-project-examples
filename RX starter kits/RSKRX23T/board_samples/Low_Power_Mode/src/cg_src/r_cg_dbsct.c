/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_dbsct.c
* Version      : Code Generator for RX23T V1.00.00.03 [28 May 2015]
* Device(s)    : R5F523T3AxFM
* Tool-Chain   : CCRX
* Description  : Setting of B.
* Creation Date: 2015/07/31
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/*
*CONVERTER REPLACED|REMOVED:
*#pragma unpack
*/
#pragma pack()

/*
*CONVERTER REPLACED|REMOVED:
*#pragma section C C$DSEC
*/
#pragma error "place this data/code segment in a location refer to C++/C Dev guide, 
EWRX_MigratingFromRenesas.pdf found under $TOOLKIT$/rx/docs
and Renesas own #pragma section documentation"

extern const struct {
    uint8_t *rom_s;       /* Start address of the initialized data section in ROM */
    uint8_t *rom_e;       /* End address of the initialized data section in ROM   */
    uint8_t *ram_s;       /* Start address of the initialized data section in RAM */
}   _DTBL[] = {
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("D"), __secend
*/
    { __section_begin("D"), __section_end("D"), __sectop("R") },
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("D_2"), __secend
*/
    { __section_begin("D_2"), __section_end("D_2"), __sectop("R_2") },
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("D_1"), __secend
*/
    { __section_begin("D_1"), __section_end("D_1"), __sectop("R_1") }
};
/*
*CONVERTER REPLACED|REMOVED:
*#pragma section C C$BSEC
*/
#pragma error "place this data/code segment in a location refer to C++/C Dev guide, 
EWRX_MigratingFromRenesas.pdf found under $TOOLKIT$/rx/docs
and Renesas own #pragma section documentation"

extern const struct {
    uint8_t *b_s;         /* Start address of non-initialized data section */
    uint8_t *b_e;         /* End address of non-initialized data section */
}   _BTBL[] = {
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("B"), __secend
*/
    { __section_begin("B"), __section_end("B") },
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("B_2"), __secend
*/
    { __section_begin("B_2"), __section_end("B_2") },
/*
*CONVERTER REPLACED|REMOVED:
*    { __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    { __section_begin("B_1"), __secend
*/
    { __section_begin("B_1"), __section_end("B_1") }
};

/*
*CONVERTER REPLACED|REMOVED:
*#pragma section

*/

/*
** CTBL prevents excessive output of L1100 messages when linking.
** Even if CTBL is deleted, the operation of the program does not change.
*/
uint8_t * const _CTBL[] = {
/*
*CONVERTER REPLACED|REMOVED:
*    __sectop
*/
    __section_begin("C_1"), __sectop("C_2"), __sectop("C"),
/*
*CONVERTER REPLACED|REMOVED:
*    __sectop
*/
    __section_begin("W_1"), __sectop("W_2"), __sectop("W"),
/*
*CONVERTER REPLACED|REMOVED:
*    __sectop
*/
    __section_begin("L"), __sectop("SU"),
/*
*CONVERTER REPLACED|REMOVED:
*    __sectop
*/
    __section_begin("C$DSEC"), __sectop("C$BSEC"),
/*
*CONVERTER REPLACED|REMOVED:
*    __sectop
*/
/*
*CONVERTER REPLACED|REMOVED:
*    __section_begin("C$INIT"), __sectop("C$VTBL"), __sectop("C$VECT")
*/

};

/*
*CONVERTER REPLACED|REMOVED:
*#pragma packoption
*/


/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
