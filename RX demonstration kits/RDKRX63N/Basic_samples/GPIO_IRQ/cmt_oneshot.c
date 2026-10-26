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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : cmt_oneshot.c
* Version       : 1.0
* Description   : Functions to provide a one-shot timer using a CMT channel
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 16.09.2011     1.00        First release
*           23.01.2012                 CS revisions
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>

#include "cmt_oneshot.h"


/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
void (*cmt_callback)(void) = 0;
void cmt_stop (void);


/******************************************************************************
* Function name : cmt_callback_set
* Description   : Receives a pointer to a function and sets it as the callback
*          function for the CMT interrupt. The callback is
*          executed when the CMT interrupt ISR is invoked on a compare match.
* Caution: this callback is executing within an ISR holding off interrupts; Be aware 
*          of the timing impact of this on the rest of the system and keep it short!
* Arguments    : func -
*                     The pointer to the callback function.
* Return Value  : none
******************************************************************************/
void cmt_callback_set(void(*func)(void))
{
  cmt_callback = func;
} /* End of function cmt_callback_set() */


/******************************************************************************
* Function name : cmt_init
* Description   : Create a one-second one-shot timer based on PCLK (48 MHz)
* Arguments     : None
* Return Value  : None
******************************************************************************/
void cmt_init(void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  /* Cancel CMT module stop state. */
  MSTP(CMT0) = 0;  
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* Compare Match Timer Control Register (CMCR) 
  b15:b8  reserved:   Read/Write value always 0
  b7      reserved:   Read undef. Write value always 1
  b6      CMIE:       Compare Match Interrupt Enable                
  b5:b2   reserved:   Read/Write value always 0
  b1:b0   CKS:        clock select   3 = count on PCLK / 512  
  */
  CMT0.CMCR.WORD = 0x0003; // Just set clock divisor for now. 
  
  /* Compare Match Timer Counter (CMCNT)
  b15:b0  16-bit readable/writable up-counter to generate interrupt requests.
  */
  CMT0.CMCNT = 0x00;      // Clear the count (default value).
  
  /* Compare Match Timer Constant Register (CMCOR))
  b15:b0  16-bit register sets the value for a compare match with CMCNT.
  */ 
  CMT0.CMCOR = 0xFFFF;    // Set to max (default value).
  
  IR (CMT0, CMI0);        // Interrupt reset.  
  IPR(CMT0, CMI0) = 4;    // Interrupt priority set.
  IEN(CMT0, CMI0) = 1;    // Interrupt enable.
} /* End of function cmt_init() */


/******************************************************************************
* Function name : cmt_start
* Description   : Sets the compare match value then start the timer.
* Arguments     : match_constant - 
*                     The counter compare match value.                                 .
* Return Value  : None
******************************************************************************/
void cmt_start(uint16_t match_constant)
{
  CMT0.CMCNT          = 0x00;           /* Clear counter register. */
  CMT0.CMCOR          = match_constant; /* Set the value for count match interrupt. */
  CMT0.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
  CMT.CMSTR0.BIT.STR0 = 1;              /* Start counting. */
} /* End of function cmt_start() */


/******************************************************************************
* Function name : cmt_stop
* Description   : Stops the Compare match timer and disables its interrupt.
* Arguments     : None
* Return Value  : None
******************************************************************************/
void cmt_stop (void)
{
  CMT0.CMCR.BIT.CMIE  = 0;      // Disable count match interrupt.
  CMT.CMSTR0.BIT.STR0 = 0;      // Stop counting. 
} /* End of function cmt_stop() */


/******************************************************************************
* Function name: CMT_isr
* Description  : Sample ISR for CMT interrupt.
* Arguments    : None
* Return value : None
******************************************************************************/
#pragma vector = VECT_CMT0_CMI0
__interrupt void CMT_isr (void) 
{
  cmt_stop();
  if (cmt_callback)
  {
    cmt_callback();
  }
} /* End of CMT_isr */

/*******************************************************************************
End of file: cmt_oneshot.c
*******************************************************************************/
