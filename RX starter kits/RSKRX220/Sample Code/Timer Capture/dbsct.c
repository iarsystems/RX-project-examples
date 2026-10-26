/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : dbsct.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines the structure of ROM and RAM areas.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines type structures used in this file */
#include "typedefine.h"

/* Preprocessor directive to specify the boundary alignment value for the 
   members of structures & unions */
#pragma unpack

/* Specify a start of the section C$DSEC */
#pragma section C C$DSEC

/* MCU ROM and RAM structure definition */
extern const struct {
    _UBYTE * rom_s;    /* Start address of the initialized data section in ROM */
    _UBYTE * rom_e;    /* End address of the initialized data section in ROM   */
    _UBYTE * ram_s;    /* Start address of the initialized data section in RAM */
}   _DTBL[] = {
    { __section_begin("D"), __section_end("D"), __section_begin("R") },
    { __section_begin("D_2"), __section_end("D_2"), __section_begin("R_2") },
    { __section_begin("D_1"), __section_end("D_1"), __section_begin("R_1") },
    { __section_begin("L"), __section_end("L"), __section_begin("L") }
};

/* Specify a start of the section C$BSEC */
#pragma section C C$BSEC

/* MCU ROM and RAM structure definition */
extern const struct {
    _UBYTE * b_s;         /* Start address of non-initialized data section */
    _UBYTE * b_e;         /* End address of non-initialized data section   */
}   _BTBL[] = {
    { __section_begin("B"), __section_end("B") },
    { __section_begin("B_2"), __section_end("B_2") },
    { __section_begin("B_1"), __section_end("B_1") }
};

/* End of section C$BSEC */
#pragma section

/* CTBL prevents excessive output of L1100 messages when linking.
   Even if CTBL is deleted, the operation of the program does not change. */
_UBYTE * const _CTBL[] = {
    __section_begin("C_1"), __section_begin("C_2"), __section_begin("C"),
    __section_begin("W_1"), __section_begin("W_2"), __section_begin("W")
};

/* Preprocessor directive to specify the boundary alignment value for the 
   members of structures & unions */
#pragma pack()option

