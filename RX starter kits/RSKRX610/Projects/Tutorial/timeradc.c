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
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : timeradc.c
* Version     : 1.00
* Device     : RX610
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines ADC and timer functions     
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/*****************************************************************************
User Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Flashing LED function definitions */
#include "flashLED.h"
/* Declares functions defined in this file */
#include "timeradc.h"


/*****************************************************************************
System Definitions & Variables
*****************************************************************************/
/* The variable ad_value is used to contain the value of the ADC */
unsigned short ad_value;


/*””FUNC COMMENT””************************************************************
* Outline     : TimerADC
* Description   : Function intialises the Timer and ADC peripherals needed to 
*          vary the LED flash rate based on the AD pot.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TimerADC(void)
{     
  /* Call the Timer start function */
  StartTimer();
  
  /* Call the ADC start function */
  StartADC();
  
  /* Disable switch interrupts */
  R_INTC_ControlExtInterrupt(PDL_INTC_IRQ3, PDL_INTC_RISING|PDL_INTC_DISABLE);
  R_INTC_ControlExtInterrupt(PDL_INTC_IRQ8, PDL_INTC_RISING|PDL_INTC_DISABLE);
  R_INTC_ControlExtInterrupt(PDL_INTC_IRQ9, PDL_INTC_RISING|PDL_INTC_DISABLE);
}
/******************************************************************************
End TimerADC function
*******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : StartTimer
* Description   : Initialises the Timer peripheral
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void StartTimer(void)
{
  /* Configure timer */
  R_TMR_CreateUnit(1, PDL_TMR_CLK_PCLK_DIV_8192|PDL_TMR_CLEAR_CM_A, 0, 0,
           0x3FF, 0, PDL_NO_FUNC, TimerADC_callback, PDL_NO_FUNC, 3);
}
/*****************************************************************************
End StartTimer function
*****************************************************************************/


/*””FUNC COMMENT””************************************************************
* Outline     : StartADC
* Description   : Initialises ADC peripheral
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””********************************************************/
void StartADC(void)
{
  /* Configure the ADC channel that is connected to the potentiometer */
  R_ADC_10_Create(0, PDL_ADC_10_MODE_SINGLE|PDL_ADC_10_CHANNELS_OPTION_1,
          50E6, 6E-7, ADC_callback, 5);
}
/*****************************************************************************
End StartADC function
*****************************************************************************/


/*””FUNC COMMENT””************************************************************
* Outline     : ADC_callback
* Description   : Callback function that is exectuted after a succesful A-D 
*          conversion. Function toggles the LEDs, reads the ADC value 
*          and sets the compare match value with the it - thereby 
*          varying the LED blink rate.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void ADC_callback(void)
{
  /* Fetch ADC value */
  R_ADC_10_Read(0, &ad_value);
  
  /* Toggle LEDs */
  ToggleLEDs();
    
  /* Add minimium value ad_value to ensure blink rate is visible */
  ad_value += 0xF;
  
  /* Start a new timer delay */
  R_TMR_ControlUnit(1, PDL_TMR_TIME_CONSTANT_A, 0, ad_value, 0);
}
/*****************************************************************************
End ADC_callback function
*****************************************************************************/


/*””FUNC COMMENT””************************************************************
* Outline     : TimerADC_callback
* Description   : Callback function that is exectuted after a compare match
*          occurs in the Timer peripheral. Function initiates an AD
*          conversion.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””********************************************************/
void TimerADC_callback(void)
{
  /* Start A/D conversion */
  R_ADC_10_Control(PDL_ADC_10_0_ON);
}
/*****************************************************************************
End TimerADC_callback function
*****************************************************************************/
