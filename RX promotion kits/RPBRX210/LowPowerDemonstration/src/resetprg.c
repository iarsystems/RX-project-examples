/*******************************************************************************
* File Name    : resetprg.c
* Version      : 1.10
* Description  : Defines post-reset routines that are used to configure the
*				 MCU prior to the main program starting. This is where the 
*				 program counter starts on power-up or reset. 
* Notes        : Refer to 'Function Definitions' Sect. for interface definitions
* Disclaimer   : See end of file for usage restrictions & copyright information
*******************************************************************************/
/*******************************************************************************
* History      : 1.00  (2010-12-17)
*              : 1.10  (2011-02-21)
*******************************************************************************/

/******************************************************************************/
/*! @file */
/******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines system level functions used in this file */
#include <machine.h>
#include <intrinsics.h>
/* Provides standard variable type defines used in this file */
#include "typedefine.h"

#ifdef __cplusplus
extern "C" {
#endif
  int __low_level_init ( void );
#ifdef __cplusplus
}
#endif

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define PSW_init  0x00010000	/*!< PSW bit pattern */
#define FPSW_init 0x00000000	/*!< FPSW bit base pattern */

/*******************************************************************************
* Function definitions
*******************************************************************************/

/*******************************************************************************
* Function Name :  PowerON_Reset
*******************************************************************************/
/*!
* @brief   Power On Reset handler
* @details Executes upon power on
*/
int __low_level_init ( void )
{ 
  nop();
  set_psw(PSW_init);			// Set Ubit & Ibit for PSW
  return 1;
}
/*******************************************************************************
* End of function PowerON_Reset
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
********************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. 
* This software is owned by Renesas Electronics Corporation and is protected 
* under all applicable laws, including copyright laws.
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
/* End of file */