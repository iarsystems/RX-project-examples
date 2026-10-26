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
* File Name    : pwm.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : File provides PWM timer functions       
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/******************************************************************************
User Includes (Project level includes)
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
/* Declares functions defined in this file */
#include "pwm.h"

/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Duty cycle varaible definition */
unsigned short duty_cycle = 0xA;


/*””FUNC COMMENT””*************************************************************
* Outline     : Init_PWM
* Description   : Initializes both timer channels, to create the PWM output 
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Init_PWM(void)
{
  /* Configure waveform output timer, channel 0 */
  R_TMR_CreatePeriodic(PDL_TMR_TMR0, PDL_TMR_FREQUENCY|PDL_TMR_OUTPUT_HIGH,
             1E3, 10, PDL_NO_FUNC, PDL_NO_FUNC, 0);
             
  /* Configure duty cycle update timer, channel 1 */
  R_TMR_CreatePeriodic(PDL_TMR_TMR1, PDL_TMR_FREQUENCY, 30, 20, TMR_callback,
             PDL_NO_FUNC, 3);           
}
/******************************************************************************
End of function Init_PWM
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : TMR_callback
* Description   : Function executed at every pulse cycle of timer channel 1.
*          Function increments the duty cycle of waveform from 10% to 
*          90%, and then resets back to 10% to start again.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void TMR_callback(void)
{
  /* Increment duty cycle */
  duty_cycle++;
  
  /* If duty cycle is greater than 90%, reset to 10% */
  if(duty_cycle > 0x5A)
  {
    duty_cycle = 0xA;
  }
  
  /* Update waveform dutycycle */
  R_TMR_ControlPeriodic(PDL_TMR_TMR0, PDL_TMR_FREQUENCY, 1E3, duty_cycle);
}
/******************************************************************************
End of function TMR_Callback
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SW3_callback
* Description   : Function is called after switch 3 key press has been
*          debounced. Function stops the duty cycle update timer, thereby
*          freezing the output waveform duty cycle.
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SW3_callback(void)
{
  /* Stop the duty cycle update timer */  
  R_TMR_ControlChannel(1, PDL_TMR_STOP, 0, 0, 0);
}
/******************************************************************************
End of function SW3_callback
******************************************************************************/
