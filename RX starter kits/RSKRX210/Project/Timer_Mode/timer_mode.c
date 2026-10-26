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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : timer_mode.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines MTU2 configuration functions
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* MTU2 RPDL function definitions */
#include "r_pdl_mtu2.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares function prototypes defined in this file */
#include "timer_mode.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* MTU2 timer callback function  */
void CB_Timer_TimerMode(void);

/*******************************************************************************
* Outline     : Init_TimerMode
* Description   : Function configures the MTU's output pin MTIOC4A. It then 
*          initialises the MTU's channel 4 to output a 1KHz square-wave.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_TimerMode(void)
{
  /* Declare error flag */
  bool err = true;

  /* Allocate a copy of the structure for channel */
  R_MTU2_Create_structure ch4_create_parameters;
  
  /* Allocate a copy of the control structure for channel 4 */
  R_MTU2_ControlChannel_structure ch4_control_parameters;
  
  /* Load the defaults */
  R_MTU2_Create_load_defaults(&ch4_create_parameters);
    
  /* Configure the MTU pin MTIOC4A for pulse output */
  err &= R_MTU2_Set
        (
          PDL_NO_DATA,
          PDL_NO_DATA,
          PDL_NO_DATA,
          PDL_NO_DATA,
          PDL_MTU2_PIN_4A_P24,
          PDL_NO_DATA,
          PDL_NO_DATA
        );
  
  /* Set the non-default options for channel 4 */
  /* Select normal operation */
  ch4_create_parameters.channel_mode = PDL_MTU2_MODE_NORMAL;

  /* Set the clock */
  ch4_create_parameters.counter_operation = \
    PDL_MTU2_CLK_PCLK_DIV_4 | PDL_MTU2_CLEAR_TGRA;

  /* Disable buffer operations */
  ch4_create_parameters.buffer_operation = PDL_MTU2_BUFFER_AC_DISABLE;

  /* Set the initial pin state to high and toggle state at compare matches */
  ch4_create_parameters.TGR_A_B_operation = PDL_MTU2_A_OC_HIGH_CM_INV;

  /* Set initial TCNT value to 0 */
  ch4_create_parameters.TCNT_TCNTU_value = 0;

  /* Set the output frequency to 1KHz
    ((25 MHz / 4 )/1000)/2 = 3125 (i.e. 0xC35)    */
  ch4_create_parameters.TGRA_TCNTV_value = 0xC35u;

  /* Set timer callback function */  
  ch4_create_parameters.func1 = CB_Timer_TimerMode;

  /* Set interrupt priority for callback function */
  ch4_create_parameters.interrupt_priority_1 = 7;

  /* Load the non-default parameters */
  err &= R_MTU2_Create
        (
          4,
          &ch4_create_parameters
        );
          
  /* Set the control options for channel 4 */
  ch4_control_parameters.TCNT_TCNTU_value = 0;
  ch4_control_parameters.TGRA_TCNTV_value = 0xC35u;
  ch4_control_parameters.control_setting = PDL_MTU2_START;
    
  /* Start the operation of channel 4 */
  err &=   R_MTU2_ControlChannel
      (
        4,
        &ch4_control_parameters
      );
        
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function Init_TimerMode
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Timer_TimerMode
* Description  : Timer interrupt callback function. This function toggles LED0
*         every time the pulse output changes at MTIOC4A. 
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Timer_TimerMode(void)
{
  /* Declare error flag */
  bool err = true;

  /* Toggle LED0 on every pulse output change */
  err &=   R_IO_PORT_Modify
      (
        PDL_IO_PORT_1_4,
        PDL_IO_PORT_XOR,
        1
      );
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_Timer_TimerMode
*******************************************************************************/  
