/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2008. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : switch.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for switch interrupts
*         
******************************************************************************/
/******************************************************************************
* History     : 30.10.2008 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/***********************************************************************************
User Includes
***********************************************************************************/
/* The following 'r_pdl' header files define the API functions used in this file */
#include "r_pdl_io_port.h"
#include "r_pdl_adc_10.h"
#include "r_pdl_sci.h"
#include "r_pdl_intc.h"
#include "r_pdl_tmr.h"
#include "r_pdl_cgc.h"
#include "r_pdl_definitions.h"
/* Defines functions used in the switch interrupts */
#include "switch.h"


/**********************************************************************************
System Definitions & Global Variables
***********************************************************************************/
/* The global variable KeyPress is used as a flag whenever a switch is pressed */
unsigned short KeyPress = 0;
/* Variable used to store the IRQ status of each switch interrupt */
uint8_t irq_status;


/*””FUNC COMMENT””*************************************************************
* Outline     : SW1_handler
* Description   : Defines program routines used on SW1 key press
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW1_handler(void)
{
  /* Initialise IRQ status flag variable */
  irq_status = 0x0;
  
  /* Fetch IRQ status, and write to variable irq_status */
    R_INTC_GetExtInterruptStatus(PDL_INTC_IRQ8, &irq_status);
   
    /* Falling edge detected? */
    if ((irq_status & 0x0C) == 0x04)
    {
    /* Disable interrupts, to prevent switch bounce */
       R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_RISING | PDL_INTC_DISABLE);
    }
  
    else
    {
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW1_debounce after timer ends */
    R_TMR_CreateOneShot(PDL_TMR_TMR2, 0, 20E-3, SW1_debounce, 7);
  
  /* Set KeyPress Varaible */
  KeyPress = 0x1;
}
/***********************************************************************************
End SW1_handler
***********************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW2_handler
* Description   : Defines program routines used on SW2 key press
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW2_handler(void)
{
  /* Initialise IRQ status flag variable */
  irq_status = 0x0;
  
  /* Fetch IRQ srtatus, and write variable to irq_status */
    R_INTC_GetExtInterruptStatus(PDL_INTC_IRQ9, &irq_status);
  
    /* Falling edge detected? */
    if ((irq_status & 0x0C) == 0x04)
    {
    /* Disable interrupts, to prevent switch bounce */
      R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_RISING | PDL_INTC_DISABLE);
    }
  
    else
    {
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW2_debounce after timer ends */
    R_TMR_CreateOneShot(PDL_TMR_TMR2, 0, 20E-3, SW2_debounce, 7);
  
  /* Set KeyPress Varaible */
  KeyPress = 0x2;
}
/***********************************************************************************
End SW2_handler
***********************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW3_handler
* Description   : Defines program routines used on SW3 key press
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW3_handler(void)
{
  /* Initialise IRQ status flag variable */
  irq_status = 0x0;
  
  /* Fetch IRQ srtatus, and write variable to irq_status */
    R_INTC_GetExtInterruptStatus(PDL_INTC_IRQ3, &irq_status);
  
    /* Falling edge detected? */
    if ((irq_status & 0x0C) == 0x04)
    {
    /* Disable interrupts, to prevent switch bounce */
      R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_RISING | PDL_INTC_DISABLE);
    }
    else
    {
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW3_debounce after timer ends */
     R_TMR_CreateOneShot(PDL_TMR_TMR2, PDL_TMR_OUTPUT_OFF, 20E-3, SW3_debounce, 7);
  
  /* Set KeyPress Varaible */
  KeyPress = 0x3;
}
/***********************************************************************************
End SW3_handler
***********************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW1_debounce
* Description   : Defines switch debounce routines
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW1_debounce(void)
{
  /* Renable interrupts after debounce timer */
    R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_ENABLE);
}
/***********************************************************************************
End SW1_debounce
***********************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW2_debounce
* Description   : Defines switch debounce routines
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW2_debounce(void)
{
  /* Renable interrupts after debounce timer */
    R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_ENABLE);
}
/***********************************************************************************
End SW2_debounce
***********************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW3_debounce
* Description   : Defines switch debounce routines
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW3_debounce(void)
{
  /* Renable interrupts after debounce timer */
    R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_ENABLE);
}
/***********************************************************************************
End SW3_debounce
***********************************************************************************/
