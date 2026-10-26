/* Adapted for use with IAR Embedded Workbench */
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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : timer_compare.c
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines timer functions used to configure the CMT unit to 
*         toggle the user LEDs on every compare match.
*******************************************************************************/
/*******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "timer_compare.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Compare match callback function declaration */
void CB_CompareMatch(void);

/*******************************************************************************
* Outline      : Init_CompareMatchTimer
* Description  : This function initialises a CMT timer that executes a callback
*         function everytime a compare match occurs.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_CompareMatchTimer(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* CMT is configured for a 100ms interval, and executes the callback 
     function CB_CompareMatch on every compare match */
  err &=  R_CMT_Create
      (
        3,
        PDL_CMT_PERIOD,
        100E-3,
        CB_CompareMatch,
        3
      );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_CompareMatchTimer
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CompareMatch
* Description  : This callback function is called as an ISR whenever a compare
*         match on CMT channel 3 occurs. The function toggles the state
*         of the user LEDs.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CompareMatch(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Toggle the output state of the user LEDs */
  err &=  R_IO_PORT_Modify
      (
        PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
        PDL_IO_PORT_XOR,
        1
      );
  
  /* Toggle the output state of the user LEDs */
  err &=  R_IO_PORT_Modify
      (
        PDL_IO_PORT_3_4,
        PDL_IO_PORT_XOR,
        1
      );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);          
}
/*******************************************************************************
* End of function CB_CompareMatch
*******************************************************************************/
