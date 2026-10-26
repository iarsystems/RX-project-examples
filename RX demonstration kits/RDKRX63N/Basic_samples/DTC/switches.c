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
* File Name     : switches.c
* Version       : 1.0
* H/W Platform  : YRDKRX62N
* Description   : Support for the pushbuttons on the YRDK RX62N
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 06.09.2011     1.00        First release
*           23.01.2012                 CS revisions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <iorx63n.h>
#include <intrinsics.h>

#include "switches.h"

/******************************************************************************
* Exported global variables
******************************************************************************/
volatile bool g_sw1_pressed = false; /* Used as a flag that can be polled. */

/******************************************************************************
* Function name: switches_initialize
* Description  : Sets up the three pushbuttons on the RDK for interrupts
* Arguments    : none
* Return value : none
******************************************************************************/
void switches_initialize (void)
{
  /* The switches on the YRDK RX63N are connected to the following pins/IRQ's
  
  Switch  Port    IRQ
  ------  ----    ----
  SW1     P4.0    IRQ8
  SW2     P4.1    IRQ9
  SW3     P4.4    IRQ12
  */
  
  /* Make the pins inputs */
  PORT4.PDR.BIT.B0 = 0; 
  PORT4.PDR.BIT.B1 = 0; 
  PORT4.PDR.BIT.B2 = 0; 
  
  /* Set P40, P41, and P44 for IRQx function */
  PORT4.PMR.BYTE  = 0x00;    /* All GPIO for now */
  MPC.P40PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  MPC.P41PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  MPC.P44PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  
  /* Set IRQ type (falling edge) */
  ICU.IRQCR[ 8].BIT.IRQMD = 0x01; 
  ICU.IRQCR[ 9].BIT.IRQMD = 0x01; 
  ICU.IRQCR[12].BIT.IRQMD = 0x01; 
  
  /* Set interrupt priority to 3 */
  IPR(ICU, IRQ8 ) = 0x03; 
  IPR(ICU, IRQ9 ) = 0x03; 
  IPR(ICU, IRQ12) = 0x03; 
  
  /* Clear any pending interrupts */
  IR (ICU, IRQ8 ) = 0;
  IR (ICU, IRQ9 ) = 0;
  IR (ICU, IRQ12) = 0;
  
  /* Enable the interrupts */
  IEN(ICU, IRQ8 ) = 1; 
  IEN(ICU, IRQ9 ) = 1; 
  IEN(ICU, IRQ12) = 1; 
} /* End of function switches_initialize() */


/******************************************************************************
* Function name: sw1_interrupt_enable
* Description  : 
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_interrupt_enable(void)
{  
  do
  {
    IR(ICU, IRQ8) = 0;   /* Clear any pending interrupts. */
  } while(IR(ICU, IRQ8));
  
  IEN(ICU, IRQ8) = 1;      /* Enable this interrupt. */
} /* End of function sw1_interrupt_enable() */


/******************************************************************************
* Function name: sw1_isr
* Description  : Sample ISR for switch 1 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_ICU_IRQ8
__interrupt void sw1_isr (void) 
{   
  IEN(ICU, IRQ8) = 0;    /* Disable this interrupt. */
  
  IR (ICU, IRQ8) = 0;    /* Clear the Interrupt flag. */
  
  g_sw1_pressed  = true; /* Set a flag to indicate switch state. */
} /* End of sw1_isr */
    

/******************************************************************************
* Function name: sw2_isr
* Description  : Sample ISR for switch 2 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_ICU_IRQ9
__interrupt void sw2_isr (void) 
{
  __no_operation();
  /* TODO: Add some debouncing! */
} /* End of sw2_isr */
    

/******************************************************************************
* Function name: sw3_isr
* Description  : Sample ISR for switch 3 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_ICU_IRQ12
__interrupt void sw3_isr (void) 
{
  __no_operation();
  /* TODO: Add some debouncing! */
} /* End of sw3_isr */


/*******************************************************************************
End of file: switches.c
*******************************************************************************/ 
