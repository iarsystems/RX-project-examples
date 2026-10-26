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
* File Name     : pmdemo_switches.c
* Version       : 1.0
* Description   : Pushbuttons related functions for the power modes demo.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 25.01.2012     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>
#include "pmdemo_switches.h"

/******************************************************************************
Global data
******************************************************************************/
uint32_t g_pressed_switch ;     /* Set to 1, 2, or 3 by ISRs for IRQ pins */


/******************************************************************************
* Function name: switches_enable
* Description  : Turns on interrupts for all switches 
* Arguments    : mask -
*                   which switch mask to use
* Return value : none
******************************************************************************/
void switches_enable (uint32_t mask)
{
  /* Enable the interrupts */
  if (mask & SW1_MASK)
  {
    IEN(ICU, IRQ8) = 1; 
  }
  if (mask & SW2_MASK)
  {
    IEN(ICU, IRQ9) = 1; 
  }
  if (mask & SW3_MASK)
  {
    IEN(ICU, IRQ12) = 1; 
  }
} /* End of function switches_enable() */
 

/******************************************************************************
* Function name: switches_disable
* Description  : Turns off interrupts for all switches 
* Arguments    : mask -
*                   which switch mask to use
* Return value : none
******************************************************************************/
void switches_disable (uint32_t mask)
{
  /* Disable the interrupts */
  /* Enable the interrupts */
  if (mask & SW1_MASK)
  {
    IEN(ICU, IRQ8) = 0; 
  }
  if (mask & SW2_MASK)
  {
    IEN(ICU, IRQ9) = 0; 
  }
  if (mask & SW3_MASK)
  {
    IEN(ICU, IRQ12) = 0; 
  }
} /* End of function switches_disable() */ 


/******************************************************************************
* Function name: switches_interrupts_clear
* Description  : Clears pending interrupts for all switches 
* Arguments    : none
* Return value : none
******************************************************************************/
void switches_interrupts_clear (void)
{
  /* Clears pending interrupts */
  IR(ICU,IRQ8 ) = 0; 
  IR(ICU,IRQ9 ) = 0; 
  IR(ICU,IRQ12) = 0; 
} /* End of function switches_interrupts_clear() */ 


/******************************************************************************
* Function name: switches_released
* Description  : Tells if all the switches are released.
* Arguments    : none
* Return value : Non-zero -
*                    if the swtiches are all released.
*                zero -
*                    if any are pressed.
******************************************************************************/
uint32_t switches_released (void)
{
  uint8_t switch_state;
  
  switch_state = PORT4.PIDR.BYTE & 0x03;
  return (switch_state == 0x03) ? 1 : 0;
} /* End of function switches_released() */
 

/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{
  /* Set global flag that SW1 was pressed */
  g_pressed_switch = 1;
  switches_disable(SW1_MASK | SW2_MASK | SW3_MASK);
} /* End of function sw1_callback() */
     

/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
  /* Set global flag that SW2 was pressed */
  g_pressed_switch = 2;
  switches_disable(SW1_MASK | SW2_MASK | SW3_MASK);
} /* End of function sw2_callback() */ 
    

/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback (void) 
{
  /* Set global flag that SW3 was pressed */
  g_pressed_switch = 3;
  switches_disable(SW1_MASK | SW2_MASK | SW3_MASK);
} /* End of function sw3_callback() */    
