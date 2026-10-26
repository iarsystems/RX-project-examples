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
* File Name    : switch.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : Defines interrupt service routine functions for the switch
*          interrupts.
* Edited by phemery
* 10/09/2010  :  Changed a lot of the code from the examples given by Renesas.
*          Removed most of the CB routines since I only use 1 switch
*          to be used as Left Click. The rest is done with the timer CB.
******************************************************************************/
/******************************************************************************
* History     : 20.07.2010 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* CGC RPDL function defintions */
#include "r_pdl_intc.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* CMT RPDL function (RX62N specific) definitions */
#include "r_pdl_cmt_RX62Nxx.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function definitions */
#include "switch.h"
/**/
#include "rskrx62ndef.h"
/**/
#include "clock-arch.h"

/******************************************************************************
* Global Variables
******************************************************************************/

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* Switch 1 interrupt callback function prototype */
void SW1_InterruptCB(void);
/* Setup the interrupt timer for while switch is pressed */
void InitialiseTMRRepeatSwi();

/******************************************************************************
* Outline     : InitialiseSwitchInterrupts
* Description   : Configures the IRQ pins connect to switches SW1-SW3 to detect
*          switch presses, and invoke an ISR.
* Argument      : none
* Return value  : none
******************************************************************************/
void InitialiseSwitchInterrupts(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Configure SW1 interrupt */
  err &=  R_INTC_CreateExtInterrupt
      (
        PDL_INTC_IRQ14,
        PDL_INTC_BOTH|PDL_INTC_A,
        SW1_InterruptCB,
        7
      );              

  /* Setup the repeatable timer that runs while switches are pressed */    
  InitialiseTMRRepeatSwi();  
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of InitialiseSwitchInterrupts
******************************************************************************/


/******************************************************************************
* Outline     : InitialiseTMRRepeatSwi
* Description   : Takes a pointer to a function, and sets it as the callback
*          function for the timer interrupt. The passed function is
*          executed everytime any switch is pressed down.
*          Compare Match Timer Unit 2 is used generate
*          100 ms (10 Hz) system tick.  
* Argument    : none
* Return value  : none
******************************************************************************/
void InitialiseTMRRepeatSwi()
{
  /**
   * Assigns a pointer to a function to global vector table entry.
   * So when CMT2 interrupt goes off it will call this function.
   */
  rpdl_CMT_callback_func[2] = (VoidCallBackFunc)gReadJoystick;

  /* CMT2 Enabled */
  SYSTEM.MSTPCRA.BIT.MSTPA14 = 0;             
    /* Count stop */
    CMT.CMSTR1.BIT.STR2 = 0x0;           
    /* Pclock/512 */
    CMT2.CMCR.WORD = 0x0043;             
    /* Timer counter clear */
    CMT2.CMCNT = 0x0000;         
    /* 100ms - phemery 10/09/2010 divided by 3 to get faster count */
    CMT2.CMCOR = (PCLK/CMT0_CLK_SELECT) / (CMT2_CLOCK_CONF_SECOND );            
    /* Count start */
  CMT.CMSTR1.BIT.STR2 = 0x1;    

    /* Interrupt priority level = 1 */  
    /* Enable ICU for CMT2 interrupts */
    IPR(CMT2, CMI2) = 1;
    IEN(CMT2, CMI2) = 1;   
}
/******************************************************************************
* End of InitialiseTMRRepeatSwi
******************************************************************************/

/******************************************************************************
* Outline     : ControlSwitchInterrupts
* Description   : Enables or disables the switch IRQ interrupts, based on the
*          input variable, control.
* Argument    : uint8_t - non-zero: Enables switch ISR.
*              zero  : Disables switch ISR.
* Return value  : none
******************************************************************************/
void ControlSwitchInterrupts(bool control)
{
  /* Declare error flag */
  bool err = true;
  
  /* Check if control input is non-zero */
  if(control)
  {
    /* Enable SW1 interrupts */
    err &=   R_INTC_ControlExtInterrupt
        (
          PDL_INTC_IRQ8,
          PDL_INTC_ENABLE|PDL_INTC_FALLING
        );
  }
  /* Control input is zero */
  else
  {  
    /* Disable SW1 interrupts */
    err &=  R_INTC_ControlExtInterrupt
        (
          PDL_INTC_IRQ8,
          PDL_INTC_DISABLE
        );
  }
  /* Halt in while loop when RPDL errors detected */  
  while(!err);  
}  
/******************************************************************************
* End of function ControlSwitchInterrupts
******************************************************************************/  

/******************************************************************************
* Outline     : SW1_InterruptCB
* Description   : Switch 1 callback ISR function. Sets global flag.
* Argument    : none
* Return value  : none
******************************************************************************/
void SW1_InterruptCB(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare status container variable */
  uint8_t status = 0x00;
  
  /* Fetch input edge status */
  err &=  R_INTC_GetExtInterruptStatus
      (
        PDL_INTC_IRQ14,
        &status
      );
  
  /* Check if interrupt was triggered from falling edge */
  if((status & 0x06) == 0x04)
  {    
    /* Set global switch flag to indicate SW1 is held down */
    jSW1 = 0x00;
  }
  /* Interrupt was triggered from a rising edge */
  else
  {
    /* Set global switch flag to indicate SW1 press complete */
    jSW1 = 0x01;
  }  
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}

/******************************************************************************
* End of function SW1_InterruptCB
******************************************************************************/  
