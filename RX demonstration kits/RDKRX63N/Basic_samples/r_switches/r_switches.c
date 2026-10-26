/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/*******************************************************************************
* File Name    : r_switches.h
* Version      : 1.00
* Description  : Functions for using switches with callback functions. 
********************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 17.01.2012 1.00    First Release            
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <iorx63n.h>
/* Switches prototypes. */
#include "r_switches.h"
#include "yrdkrx63n.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SW1_IRQ_NUMBER  8
#define SW2_IRQ_NUMBER  9
#define SW3_IRQ_NUMBER 12

/*******************************************************************************
* Function Name: R_SWITCHES_Init
* Description  : Initializes pins to be input and interrupt on switch presses.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_SWITCHES_Init(void)
{      
  /* Unlock protection register */
  MPC.PWPR.BIT.B0WI = 0 ;         
  /* Unlock MPC registers */    
  MPC.PWPR.BIT.PFSWE = 1 ;        
  
  /* Make switch pins inputs. */
  SW1_PDR = 0;
  SW2_PDR = 0;
  SW3_PDR = 0;
  
  /* Set port mode registers for switches. */
  SW1_PMR = 0;
  SW2_PMR = 0;
  SW3_PMR = 0;
  
  /* The switches on the RDKRX63N are connected to the following pins/IRQ's    
  Switch  Port    IRQ
  ------  ----    ----
  SW1     P4.0    IRQ8
  SW2     P4.1    IRQ9
  SW3     P4.4    IRQ12
  */
  
  MPC.P40PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  MPC.P41PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  MPC.P44PFS.BYTE = 0x40;    /* P40 is used as IRQ pin */
  
  /* Set IRQ type (falling edge) */
  ICU.IRQCR[SW1_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
  ICU.IRQCR[SW2_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
  ICU.IRQCR[SW3_IRQ_NUMBER].BIT.IRQMD  = 0x01; 
  
  /* Set interrupt priority to 3 */
  IPR(ICU, IRQ8)  = 3;
  IPR(ICU, IRQ9)  = 3;
  IPR(ICU, IRQ12) = 3;
  
  /* Clear any pending interrupts */
  IR(ICU, IRQ8)  = 0;
  IR(ICU, IRQ9)  = 0;
  IR(ICU, IRQ12) = 0;
  
  /* Enable the interrupts */
  IEN(ICU, IRQ8)  = 1;
  IEN(ICU, IRQ9)  = 1;
  IEN(ICU, IRQ12) = 1;
} 

#if defined(SW1_CALLBACK_FUNCTION)
/*******************************************************************************
* Function name: sw1_isr
* Description  : Sample ISR for switch 1 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
*******************************************************************************/
#pragma vector = VECT_ICU_IRQ8
__interrupt void sw1_isr (void) 
{
  /* TODO: Add some debouncing! */
  
  /* Call callback function. */
  SW1_CALLBACK_FUNCTION();    
}    
#endif /* SW1_CALLBACK_FUNCTION */

#if defined(SW2_CALLBACK_FUNCTION)
/*******************************************************************************
* Function name: sw2_isr
* Description  : Sample ISR for switch 2 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
*******************************************************************************/
#pragma vector = VECT_ICU_IRQ9
__interrupt void sw2_isr (void) 
{
  /* TODO: Add some debouncing! */
  
  /* Call callback function. */
  SW2_CALLBACK_FUNCTION();    
}    
#endif /* SW2_CALLBACK_FUNCTION */

#if defined(SW3_CALLBACK_FUNCTION)
/*******************************************************************************
* Function name: sw3_isr
* Description  : Sample ISR for switch 3 input (must do hardware setup first!)
* Arguments    : none
* Return value : none
*******************************************************************************/
#pragma vector = VECT_ICU_IRQ12
__interrupt void sw3_isr (void) 
{
  /* TODO: Add some debouncing! */
  
  /* Call callback function. */
  SW3_CALLBACK_FUNCTION();    
}
#endif /* SW3_CALLBACK_FUNCTION */


