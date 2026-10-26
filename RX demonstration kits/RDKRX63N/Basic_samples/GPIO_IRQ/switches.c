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
* H/W Platform  : YRDKRX63N
* Description   : Support for the pushbuttons on the YRDK RX62N
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
#include <stdbool.h>
#include <intrinsics.h>
#include <iorx63n.h>

#include "switches.h"
#include "cmt_oneshot.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define SETTLE_TIME       94 /* About 10 milliseconds with 48MHz pclk/512 */
#define SETTLE_CHECK_TIME 94 /* About 10 milliseconds with 48MHz pclk/512 */

/******************************************************************************
Local function declarations 
******************************************************************************/
static void switch_debounce_init (void);
static void sw1_debounce_callback(void);

/******************************************************************************
Local global variables
******************************************************************************/
volatile uint8_t g_sw3_pressed     = 0;
volatile uint8_t g_sw1_state       = 1;     /* Start in switch released state. */
volatile bool g_sw1_interrupted    = false; /*  Debounce state control variable. */
volatile bool g_sw1_settled        = true;  /*  "" */

volatile uint32_t g_sw1_up_count   = 0;     /*  Not essential. Used for demo. */ 
volatile uint32_t g_sw1_down_count = 0;     /*  "" */


/******************************************************************************
* Function name: switch_debounce_init
* Description  : Initialises the method that will be used to debounce the switch.
*                In this case, a CMT timer is used which calls a callback. 
* Arguments    : none                 
* Return value : none
******************************************************************************/
void switch_debounce_init(void)
{
  cmt_init();
  cmt_callback_set(&sw1_debounce_callback);
} /* End of function switch_debounce_init() */


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
  
  switch_debounce_init();
} /* End of function switches_initialize() */

/******************************************************************************
* Function name: sw1_debounce_callback
* Description  : Is called on timeout compare of debounce timer. Switch interrupt
*              : has been disabled during debounce period. This function enables 
*              : the interrupt and starts the counter with another time period
*              : during which no additional switch interrupt should occur if the 
*              : switch state has stabilized. If the switch is still bouncing 
*              : then it will invoke another debounce cycle. Otherwise, the 
*              : successful completion of the check count will indicate that the
*              : switch has settled and it will then set up for the next event. 
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_debounce_callback(void)
{
  if (g_sw1_interrupted)              /*  Do settle check. */
  { 
    g_sw1_interrupted      = false; /*  Clear flag to detect next interrupt. */
    ICU.IRQCR[8].BIT.IRQMD = 3;     /*  Interrupt on either edge. */
    IEN(ICU, IRQ8)         = 1;     /*  Enable sw1 interrupt. */
    cmt_start(SETTLE_CHECK_TIME);   /*  Restart switch debounce timer.  */        
  }
  else /*  Sw1 passed settle check, so ready to set switch state. */
  {
    g_sw1_settled = true;
    
    if(1 == PORT4.PIDR.BIT.B0)
    {
      /* Switch is currently high (switch released). 
      Change to interrupt on falling edge to detect next press. */
      ICU.IRQCR[8].BIT.IRQMD = 1;
      
      g_sw1_state = 1;    /*  Switch is in open state (up). */
      if (g_sw1_up_count < 99)
      {
        g_sw1_up_count++;   /*  Debug */
      }
    }     
    else
    {
      /* Switch is currently low (switch pressed)                        */
      /* so change to interrupt on rising edge to detect switch release. */
      ICU.IRQCR[8].BIT.IRQMD = 2;   
      
      g_sw1_state = 0;    /*  Save switch state (down). */
      if (g_sw1_down_count < 99)
      {
        g_sw1_down_count++; /*  Debug   */            
      }
    }
    IEN(ICU, IRQ8) = 1;     /*  Enable sw1 interrupt.    */                
  } 
} /* End of function sw1_debounce_callback() */


/******************************************************************************
* Function name: sw1_isr
* Description  : Sample ISR for switch 1 input. Employs timed debounce and 
*              : and can call different routines for switch pressed and switch
*              : released events.
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_ICU_IRQ8
__interrupt void sw1_isr (void) 
{
  IEN(ICU, IRQ8) = 0;         /* Disable this interrupt. */
  
  g_sw1_interrupted = true;
  
  cmt_start(SETTLE_TIME);     /* Start switch debounce timer. */
  
  if (g_sw1_settled)          /* If the switch was previously declared settled, */
  {
    g_sw1_settled = false;  /* set it as currently unsettled. Debounce timer callback will change this. */
    
    /* Find out which edge triggered this interrupt. */
    if      (1 == ICU.IRQCR[8].BIT.IRQMD) /* If the switch was just pressed  (falling edge) */
    {
      PORTD.PODR.BIT.B5 = 0;            /* turn on LED 4. */
    }
    else if (2 == ICU.IRQCR[8].BIT.IRQMD) /* If the switch was just released (rising  edge) */
    {
      PORTD.PODR.BIT.B5 = 1;            /* turn off LED 4. */
    }
    else                                  /* If something unexpected happened, do nothing. */
    {
    }        
  }
} /* End of sw1_isr */
   

/******************************************************************************
* Function name: sw2_isr
* Description  : Sample ISR for switch 2 input.
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_ICU_IRQ9
__interrupt void sw2_isr (void) 
{
  g_sw1_down_count = 0; /* Clear the SW1 up and down counters. */
  g_sw1_up_count   = 0; /* It does not matter if there is key bounce here. */
}   /* End of sw2_isr */

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
* End of file: switches.c
*******************************************************************************/
