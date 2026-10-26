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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : switch.c
* Version     : 1.00
* Device     : R5F5630EDDFP
* Tool-Chain   : RX Family C Compiler, v1.01
* H/W Platform  : RSKRX630
* Description   : Defines interrupt service routine functions for the switch
*          interrupts. De-bounces switch and sets a key press flag. 
*******************************************************************************/
/*******************************************************************************
* History     : 23.01.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Defines RX630 port registers */
#include "iorx630_usb.h"
/* Switch handler function definitions */
#include "switch.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Switch flag global variable */
volatile uint8_t gSwitchFlag = 0x00;
/* Switch standby ready global variable */
volatile bool gSwitchStandbyReady = true;
/* Switch press callback pointer declaration */
void (*gSwitchPressCallbackFunc)(void) = NULL;
/* Switch release callback pointer declaration */
void (*gSwitchReleaseCallbackFunc)(void) = NULL;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Switch de-bounce callback function prototype */
void SwitchDebounce(void);

/*******************************************************************************
* Outline     : InitialiseSwitchInterrupts
* Description   : Configures the IRQ pins connected to switches SW1-SW3 to
*          detect switch presses, and invoke an ISR.
* Argument      : none
* Return value  : none
*******************************************************************************/
void InitialiseSwitchInterrupts(void)
{  
  /* Reset the switch flag */
  gSwitchFlag = 0x00;
  
  /* Reset the switch standby ready flag */
  gSwitchStandbyReady = 0x1;
  
  /* Enable write to PFSWE bit */
  MPC.PWPR.BYTE = 0x00;
  /* Disable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x40;
  
  /* Set IRQ2 on Port3 as an input pin */
  MPC.P32PFS.BIT.ISEL = 0x1;
  /* Select the pin function for the lower 5 bits */
  MPC.P32PFS.BIT.PSEL = 0x0;
  
  /* Set IRQ12 on Port4 as an input pin */
  MPC.P44PFS.BIT.ISEL = 0x1;
  
  /* Set IRQ15 on Port0 as an input pin */
  MPC.P07PFS.BIT.ISEL = 0x1;
  /* Select the pin function for the lower 5 bits */
  MPC.P07PFS.BIT.PSEL = 0x0;
  
  /* Enable write protection to PFS registers */
  MPC.PWPR.BYTE = 0x80;
  
  /* Configure switch SW1 (IRQ2) */
  /* Clear SW1 interrupt requests */
  ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x0;  
  /* Set P3_2 (SW2) as an input */
  PORT3.PDR.BIT.B2 = 0;
  /* Set detection direction as falling edge */
  ICU.IRQCR[2].BIT.IRQMD = 0x1;  
  /* Clear IR flag */
  ICU.IR[VECT_ICU_IRQ2].BIT.IR = 0x0;            
  /* Set interrupt priority level to 7 */
  ICU.IPR[IPR_ICU_IRQ2].BIT.IPR = 0x7;  
  /* Enable SW1 interrupt requests */
  ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x1;      
                
  /* Configure IRQ12 interrupt (connected to SW2) */
  /* Clear SW2 interrupt requests */
  ICU.IER[IER_ICU_IRQ2].BIT.IEN4 = 0x0;  
  /* Set P3_2 (SW2) as an input */
  PORT4.PDR.BIT.B4 = 0;
  /* Set detection direction as falling edge */
  ICU.IRQCR[12].BIT.IRQMD = 0x1;  
  /* Clear IR flag */
  ICU.IR[VECT_ICU_IRQ12].BIT.IR = 0x0;            
  /* Set interrupt priority level to 7 */
  ICU.IPR[IPR_ICU_IRQ12].BIT.IPR = 0x7;  
  /* Enable SW2 interrupt requests */
  ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x1;              
              
  /* Configure IRQ15 interrupt (connected to SW3) */
  /* Clear SW3 interrupt requests */
  ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x0;  
  /* Set P3_2 (SW3) as an input */
  PORT0.PDR.BIT.B7 = 0;
  /* Set detection direction as falling edge */
  ICU.IRQCR[15].BIT.IRQMD = 0x1;  
  /* Clear IR flag */
  ICU.IR[VECT_ICU_IRQ15].BIT.IR = 0x0;            
  /* Set interrupt priority level to 7 */
  ICU.IPR[IPR_ICU_IRQ15].BIT.IPR = 0x7;  
  /* Enable SW3 interrupt requests */
  ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x1;
}
/******************************************************************************
* End of InitialiseSwitchInterrupts
******************************************************************************/

/******************************************************************************
* Outline     : SetSwitchPressCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*          function for the switch interrupts. The passed function is
*          executed every time any switch is pressed down.
* Argument    : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchPressCallback(void(*callback)(void))
{
  /* Store the callback function pointer into the global variable */
  gSwitchPressCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchPressCallback
******************************************************************************/

/******************************************************************************
* Outline     : SetSwitchReleaseCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*          function for the switch interrupts. The passed function is
*          executed every time any switch is released.
* Argument    : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchReleaseCallback(void(*callback)(void))
{
  /* Store the callback function pointer into the global variable */
  gSwitchReleaseCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchReleaseCallback
******************************************************************************/

/******************************************************************************
* Outline     : ControlSwitchInterrupts
* Description   : Enables or disables the switch IRQ interrupts, based on the
*          input variable, control.
* Argument    : uint8_t - 0x1  : Enables switch interrupts.
*                0x0  : Disables switch interrupts.
* Return value  : none
******************************************************************************/
void ControlSwitchInterrupts(uint8_t control)
{  
  /* Check if control input is 0x1 */
  if(control)
  {
    /* Enable SW1 interrupt requests */
    ICU.IER[IER_ICU_IRQ2].BIT.IEN7 = 0x1;
  
    /* Enable SW2 interrupt requests */
    ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x1;
  
    /* Enable SW3 interrupt requests */
    ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x1;
  }
  /* Control input is 0x0 */
  else
  {  
    /* Disable SW1 interrupts */
    ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x0;
    
    /* Disable SW2 interrupts */
    ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x0;
  
    /* Disable SW3 interrupts */
    ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x0;
  }
}  
/******************************************************************************
* End of function ControlSwitchInterrupts
******************************************************************************/  
  
/******************************************************************************
* Outline     : Excep_IRQ2
* Description   : Switch 1 callback ISR function. The function disables switch
*          interrupts, then uses a one-shot timer to re-enable them after
*          a 10ms de-bounce timer period.
* Argument    : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ2
__interrupt void Excep_IRQ2(void)
{          
  /* Set detection direction to rising edge */
  if(ICU.IRQCR[2].BIT.IRQMD == 0x1)
  {
    /* Disable switch 1 interrupts */
    ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x0;

    /* Set detection direction as rising edge */
    ICU.IRQCR[2].BIT.IRQMD = 0x0;
    
    /* Set global switch flag to indicate SW1 is held down */
    gSwitchFlag |= 0x08;
    
    /* Check if switch press callback function is not NULL */
    if(gSwitchPressCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchPressCallbackFunc();
    }
  }
  else
  {
    /* Disable switch 1 interrupts */
    ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x0;
    
    /* Set detection direction to falling edge */
    ICU.IRQCR[2].BIT.IRQMD = 0x1;
    
    /* Clear SW1 held-down flag bit in switch flag */
    gSwitchFlag &= 0xF7;
    
    /* Set global switch flag to indicate SW1 press complete */
    gSwitchFlag |= 0x80;
  
    /* Check if switch release callback function is not NULL */
    if(gSwitchReleaseCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchReleaseCallbackFunc();
    }
  }

  /* Set standby ready flag as false */
  gSwitchStandbyReady = false;

  /* Clear IRQ2 interrupt flag */
  ICU.IR[VECT_ICU_IRQ2].BIT.IR = 0;
  
  /* Create 10ms one-shot timer interrupt to de-bounce the switch input */
  SwitchDebounce();
}
/******************************************************************************
* End of function SW1_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline     : Excep_IRQ12
* Description   : Switch 2 callback ISR function. The function disables switch
*          interrupts, then uses a oneshot timer to re-enable them after
*          a 10ms de-bounce timer period.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ12
__interrupt void Excep_IRQ12(void)
{  
  /* Set detection direction to rising edge */
  if(ICU.IRQCR[12].BIT.IRQMD == 0x1)
  {
    /* Disable switch 2 interrupts */
    ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x0;

    /* Set detection direction as rising edge */
    ICU.IRQCR[12].BIT.IRQMD = 0x0;
    
    /* Set global switch flag to indicate SW2 is held down */
    gSwitchFlag |= 0x04;
    
    /* Check if switch press callback function is not NULL */
    if(gSwitchPressCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchPressCallbackFunc();
    }
  }
  else
  {
    /* Disable switch 1 interrupts */
    ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x0;
    
    /* Set detection direction to falling edge */
    ICU.IRQCR[12].BIT.IRQMD = 0x1;
    
    /* Clear SW2 held-down flag bit in switch flag */
    gSwitchFlag &= 0xFB;
    
    /* Set global switch flag to indicate SW2 press complete */
    gSwitchFlag |= 0x40;
  
    /* Check if switch release callback function is not NULL */
    if(gSwitchReleaseCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchReleaseCallbackFunc();
    }
  }

  /* Set standby ready flag as false */
  gSwitchStandbyReady = false;

  /* Clear IRQ12 interrupt flag */
  ICU.IR[VECT_ICU_IRQ12].BIT.IR = 0;
  
  /* Create 10ms one-shot timer interrupt to de-bounce the switch input */
  SwitchDebounce();
}
/******************************************************************************
* End of function SW2_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline     : Excep_IRQ15
* Description   : Switch 3 callback ISR function. The function disables switch
*          interrupts, then uses a oneshot timer to re-enable them after
*          a 10ms de-bounce timer period.
* Argument    : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ15
__interrupt void Excep_IRQ15(void)
{      
  /* Set detection direction to rising edge */
  if(ICU.IRQCR[15].BIT.IRQMD == 0x1)
  {
    /* Disable switch 3 interrupts */
    ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x0;

    /* Set detection direction as rising edge */
    ICU.IRQCR[15].BIT.IRQMD = 0x0;
    
    /* Set global switch flag to indicate SW3 is held down */
    gSwitchFlag |= 0x02;
    
    /* Check if switch press callback function is not NULL */
    if(gSwitchPressCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchPressCallbackFunc();
    }
  }
  else
  {
    /* Disable switch 3 interrupts */
    ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x0;
    
    /* Set detection direction to falling edge */
    ICU.IRQCR[15].BIT.IRQMD = 0x1;
    
    /* Clear SW1 held-down flag bit in switch flag */
    gSwitchFlag &= 0xFD;
    
    /* Set global switch flag to indicate SW1 press complete */
    gSwitchFlag |= 0x20;
  
    /* Check if switch release callback function is not NULL */
    if(gSwitchReleaseCallbackFunc)
    {
      /* Execute user callback function */
      gSwitchReleaseCallbackFunc();
    }
  }

  /* Set standby ready flag as false */
  gSwitchStandbyReady = false;

  /* Clear IRQ15 interrupt flag */
  ICU.IR[VECT_ICU_IRQ15].BIT.IR = 0;
  
  /* Create 10ms one-shot timer interrupt to de-bounce the switch input */
  SwitchDebounce();
}
/******************************************************************************
* End of function SW3_InterruptCB
******************************************************************************/  

/******************************************************************************
* Outline     : SwitchDebounceCB
* Description   : One-shot timer callback function. This function is triggered
*          by the timer interrupt, and re-enables switch interrupts.
* Argument      : none
* Return value  : none
******************************************************************************/
void SwitchDebounce(void)
{  
  /* Declare and initialise a count variable */
  uint16_t debounce_delay = 0x1FFF;
  
  /* Add a delay */
  while(--debounce_delay != 0x0)
  {
    /* Do nothing */  
  }
  
  /* Re-enable switch 1 interrupts */
  ICU.IER[IER_ICU_IRQ2].BIT.IEN2 = 0x1;

  /* Re-enable switch 2 interrupts */
  ICU.IER[IER_ICU_IRQ12].BIT.IEN4 = 0x1;

  /* Re-enable switch 3 interrupts */
  ICU.IER[IER_ICU_IRQ15].BIT.IEN7 = 0x1;
}
/******************************************************************************
* End of function SwitchDebounceCB
******************************************************************************/
