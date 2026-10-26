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
* File Name    : timer_mode.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines timer functions    
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
/* Declares function prototypes defined in this file */
#include "timer_mode.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : Timer_Init
* Description   : Function initialises the timer channel 3
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Timer_Init(void)
{
  /* Initialise 1khz Timer output */
  R_TMR_CreatePeriodic(PDL_TMR_TMR3, PDL_TMR_FREQUENCY|PDL_TMR_OUTPUT_HIGH, 
             2E3, 40, Timer_callback, PDL_NO_FUNC, 2);
}
/******************************************************************************
End of function Timer_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : Timer_callback
* Description   : Callback function executes every period cycle of timer channel
*          3. Function toggles pin 8_3 (J2-20)
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Timer_callback(void)
{
  /* Toggle pin 8_3 (J2-20) */
  R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_XOR, 1);
}
/******************************************************************************
End of function Timer_callback
******************************************************************************/
