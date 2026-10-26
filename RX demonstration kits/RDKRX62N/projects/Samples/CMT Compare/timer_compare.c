/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : timer_compare.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines timer functions used to configure the CMT unit to 
*                 toggle the user LEDs on every compare match.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Description
*         : 08.Oct.2010     First release
*         : 02.Dec.2010     Second YRDK release
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
* Outline       : Init_CompareMatchTimer
* Description   : This function initialises a CMT timer that executes a callback
*                 function everytime a compare match occurs.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_CompareMatchTimer(void)
{
  /* Declare error flag */
  bool err = true;

  /* CMT is configured for a 100ms interval, and executes the callback 
  function CB_CompareMatch on every compare match */
  err &=  R_CMT_Create
    (
    3,               // timer channel selection
    PDL_CMT_PERIOD,  // configuration selection
    100E-3,          // period, frequency, or register data
    CB_CompareMatch, // callback function
    3                // interrupt priority level
    );

  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function Init_CompareMatchTimer
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_CompareMatch
* Description   : This callback function is called as an ISR whenever a compare
*                 match on CMT channel 3 occurs. The function toggles the state
*                 of the user LEDs.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_CompareMatch(void)
{
  /* Declare error flag */
  bool err = true;

  /* Toggle the output state of the user LEDs */
  err &=  R_IO_PORT_Modify
    (
    PDL_IO_PORT_D_3 | PDL_IO_PORT_D_4 | PDL_IO_PORT_D_5,
    PDL_IO_PORT_XOR,
    1
    );

  /* Toggle the output state of the user LEDs */
  err &=  R_IO_PORT_Modify
    (
    PDL_IO_PORT_E_1 | PDL_IO_PORT_E_2 | PDL_IO_PORT_E_3,
    PDL_IO_PORT_XOR,
    1
    );

  /* Halt in while loop when RPDL errors detected */  
  while (!err);          
}
/*******************************************************************************
* End of function CB_CompareMatch
*******************************************************************************/
