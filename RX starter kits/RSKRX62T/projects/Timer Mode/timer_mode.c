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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : timer_mode.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Defines timer mode functions prototypes used.    
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* MTU RPDL function defintions */
#include "r_pdl_mtu3.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares function prototypes defined in this file */
#include "timer_mode.h"


/*******************************************************************************
* Outline     : Init_TimerMode
* Description   : Function configures the MTU's output pin MTIOC2A. It then 
*          initialises the MTU's channel 2 to output a 1KHz square-wave.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_TimerMode(void)
{
  /* Declare error flag */
  bool err = true;

  /* Allocate a copy of the structure for channel */
  R_MTU3_Create_structure ch2_create_parameters;
  
  /* Allocate a copy of the control structure for channel 2 */
  R_MTU3_ControlChannel_structure ch2_control_parameters;
  
  /* Load the defaults */
  R_MTU3_Create_load_defaults(&ch2_create_parameters);
    
  /* Configure the MTU pin */
  err &= R_MTU3_Set
        (
          PDL_MTU3_PIN_0A_A
        );
  
  /* Set the non-default options for channel 4 */
  /* Select normal operation */
  ch2_create_parameters.data2 = PDL_MTU3_MODE_NORMAL;
  /* Set the clock */
  ch2_create_parameters.data3 = PDL_MTU3_CLK_ICLK_DIV_64 | PDL_MTU3_CLEAR_TGRA;
  /* Disable buffer operations */
  ch2_create_parameters.data5 = PDL_MTU3_BUFFER_AC_DISABLE;
  /* Set the initial pin state to high and toggle state at compare matches */
  ch2_create_parameters.data6 = PDL_MTU3_A_OC_HIGH_CM_INV;
  /* Set initial TCNT value to 0 */
  ch2_create_parameters.data9 = 0;
  /* Set period to 1KHz */
  ch2_create_parameters.data10 = 770;
  /* Set unused parameters to 0 */
  ch2_create_parameters.data11 = 0;
  ch2_create_parameters.data12 = 0;
  ch2_create_parameters.data13 = 0;
  ch2_create_parameters.data14 = 0;
  ch2_create_parameters.data15 = 0;
  ch2_create_parameters.func1 = PDL_NO_FUNC;

  /* Load the non-default parameters */
  err &= R_MTU3_Create
        (
          2,
          &ch2_create_parameters
        );
          
  /* Set the control options for channel 2 */
  ch2_control_parameters.data2 = PDL_MTU3_START;

  /* Start the operation of channel 2 */
  err &= R_MTU3_ControlChannel
  (
    2,
    &ch2_control_parameters
  );
        
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/******************************************************************************
* End of function Init_TimerMode
******************************************************************************/
