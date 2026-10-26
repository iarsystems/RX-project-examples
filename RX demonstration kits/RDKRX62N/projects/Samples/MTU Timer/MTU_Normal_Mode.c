/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
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
* File Name     : MTU_Normal_mode.c
* Version       : 1.00
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines MTU functions used to generate a squarewave output
*                 waveform.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* MTU RPDL function definitions */
#include "r_pdl_mtu2.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "MTU_Normal_Mode.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
void CB_MTU_Timer(void);

/*******************************************************************************
* Outline      : Init_MTU_Normal_Mode
* Description  : This function initialises an MTU to generate a 1000 Hz square
*                wave output on pin P21 (MTIOC1B).
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_MTU_Normal_Mode(void)
{
  /* Allocate a copy of the structure for the selected channel */
  R_MTU2_Create_structure         ch1_create_parms;
  R_MTU2_ControlChannel_structure ch1_control_parms;

  /* Declare error flag */
  bool err = true;

  /* First, the math  --  To get a 1KHz wave, we need 2000 signal inversions/sec.
  The peripheral clock frequency is 48MHz.  Dividing this by 64 with the PCLK 
  Divider in the MTU, the TCNT source frequency can be reduced to 750,000Hz.
  Setting the TGRB to 375 for the compare and clearing of the TCNT gets us to 
  the 2000 inversions per second that we are looking for. */


  /* Configure the MTU pins */
  err = err && R_MTU2_Set
    (
    PDL_MTU2_PIN_CLKABCD_B /* Select the -A or -B pins for MTCLKA, MTCLKB, MTCLKD and MTCLKD. */
    );    


  /* Load the defaults */
  R_MTU2_Create_load_defaults( &ch1_create_parms );   

  /* Set non-default options for channel 1 */
  ch1_create_parms.data2  = PDL_MTU2_MODE_NORMAL     | PDL_MTU2_SYNC_DISABLE;
  ch1_create_parms.data3  = PDL_MTU2_CLK_PCLK_DIV_64 | PDL_MTU2_CLEAR_TGRB;
  ch1_create_parms.data6  = PDL_MTU2_B_OC_HIGH_CM_INV;
  ch1_create_parms.data11 = 375;                   /* TGRB */
  ch1_create_parms.func2  = CB_MTU_Timer;          /* TGRB event func */
  ch1_create_parms.data19 = 5;                     /* interrupt priority */

  err = err && R_MTU2_Create
    (
    1,                // channel selection
    &ch1_create_parms // pointer to the structure
    );    


  /* Start MTU channel 1 */
  ch1_control_parms.data2 = PDL_MTU2_START;

  /* Modify the operation of channel 1 */
  err = err && R_MTU2_ControlChannel
    (
    1,                 // channel selection
    &ch1_control_parms // pointer to the structure
    );                


  /* Halt in while loop when RPDL errors detected */    
  while (!err)
  {
  }
}
/*******************************************************************************
* End of function Init_MTU_Normal_Mode
*******************************************************************************/

unsigned long counter = 0;
/*******************************************************************************
* Outline      : CB_MTU_Timer
* Description  : This callback functions is executed every "TGRB match"
*                interrupt. The function toggles the output state of LED4 
*                every 500 calls (1/2 second interval).
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_MTU_Timer(void)
{
  /* Declare error flag */
  bool err = true;

  counter++;

  if ( counter == 1000 )
  {

    /* Toggle the output state of LED4 */
    err = err && R_IO_PORT_Modify
      (
      PDL_IO_PORT_D_5,
      PDL_IO_PORT_XOR,
      1
      );
    counter = 0;
  }

  /* Halt in while loop when RPDL errors detected */
  while (!err)
  {
  }
}
/*******************************************************************************
* End of function CB_MTU_Timer
*******************************************************************************/

