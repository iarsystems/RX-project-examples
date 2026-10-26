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
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for switch interrupts
*         
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/***********************************************************************************
User Includes
***********************************************************************************/
/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"
/* ADC RPDL function defintions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function defintions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function defintions */
#include "r_pdl_intc.h"
/* Timer RPDL function defintions */
#include "r_pdl_tmr.h"
/* CGC RPDL function defintions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Switch handler function defintions */
#include "switch.h"
/* DTC function definitions */
#include "dtc.h"


/**********************************************************************************
System Definitions & Global Variables
***********************************************************************************/
/* The global variable KeyPress is used as a flag whenever a switch is pressed */
unsigned short KeyPress = 0;
/* Variable used to store the IRQ status of each switch interrupt */
uint8_t switch_rise = 0x0;


/*””FUNC COMMENT””*************************************************************
* Outline     : SW1_handler
* Description   : Defines program routines used on SW1 key press
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW1_handler(void)
{
    /* Falling edge detected? */
    if (!switch_rise)
    {
    /* Prepare for rising edge to follow */
    switch_rise = 0x1;
    
    /* Disable interrupts, to prevent switch bounce */
      R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_RISING | PDL_INTC_DISABLE);
    
    /* Start the AD conversion */
    ADC_Run();
    }
    else
  {
    /* Prepare for falling edge to follow */
    switch_rise = 0x0;
    
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW3_debounce after timer ends */
     R_TMR_CreateOneShot(PDL_TMR_TMR1, PDL_TMR_OUTPUT_OFF, 20E-3, SW1_debounce, 7);  
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
    /* Falling edge detected? */
    if (!switch_rise)
    {
    /* Prepare for rising edge to follow */
    switch_rise = 0x1;
    
    /* Disable interrupts, to prevent switch bounce */
      R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_RISING | PDL_INTC_DISABLE);
    }
    else
  {
    /* Prepare for falling edge to follow */
    switch_rise = 0x0;
    
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW3_debounce after timer ends */
     R_TMR_CreateOneShot(PDL_TMR_TMR1, PDL_TMR_OUTPUT_OFF, 20E-3, SW2_debounce, 7);  
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

    /* Falling edge detected? */
    if (!switch_rise)
    {
    /* Prepare for rising edge to follow */
    switch_rise = 0x1;
    
    /* Disable interrupts, to prevent switch bounce */
      R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_RISING | PDL_INTC_DISABLE);
    }
    else
  {
    /* Prepare for falling edge to follow */
    switch_rise = 0x0;
    
    /* Disable interrupts, to prevent switch bounce */
        R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_FALLING | PDL_INTC_DISABLE);
    }
  
    /* Start the debounce timer, and call function SW3_debounce after timer ends */
     R_TMR_CreateOneShot(PDL_TMR_TMR1, PDL_TMR_OUTPUT_OFF, 20E-3, SW3_debounce, 7);
    
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
