/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_cmt_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for CMT module.
* Creation Date: 2015-01-20
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
#include "iorx113.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* DEBOUNCE_TICKS - Number of 100ms counter increments for debounce delay */
#define DEBOUNCE_TICKS (2)

/* Global counter variable - incremented within CMT0 ISR */
volatile uint16_t g_1ms_counter = 0x00;

/* Global counter variable - incremented within CMT3 ISR */
volatile uint8_t g_100ms_counter = 0x00;

/* Global flag - shows a single 100ms tick*/
volatile uint8_t g_100ms_expired = FALSE;

volatile uint8_t g_expired = FALSE;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_cmt_cmi0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT0_CMI0
#if FAST_INTERRUPT_VECTOR == VECT_CMT0_CMI0
__fast_interrupt static void r_cmt_cmi0_interrupt(void)
#else
__interrupt static void r_cmt_cmi0_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

	g_1ms_counter++;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_cmt_cmi3_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_CMT3_CMI3
#if FAST_INTERRUPT_VECTOR == VECT_CMT3_CMI3
__fast_interrupt static void r_cmt_cmi3_interrupt(void)
#else
__interrupt static void r_cmt_cmi3_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */

	g_100ms_counter++;

	g_100ms_expired = TRUE;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/*******************************************************************************
* Function Name: R_CMT_I2cTimeout
* Description  : This function informs if the channel 3 timer has overflowed
* Argument     : none
* Return value : uint8_t FALSE = not overflowed, TRUE = overflowed
*******************************************************************************/
uint8_t R_CMT_I2cTimeout (void)
{
	return (g_100ms_expired);
}

/*******************************************************************************
* End of function R_CMT_I2cTimeout
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CMT_I2cTimerReset
* Description  : This function resets the channel 3 Timer function, with a 1s
* 			   : dtimeout value;
* Argument     : none
* Return value : none
*******************************************************************************/
void R_CMT_I2cTimerReset (void)
{
    R_CMT3_Stop();

    CMT3.CMCNT = 0x00;		/* Reset the count in progress */
    g_100ms_counter = 0;
    g_100ms_expired = FALSE;
}

/*******************************************************************************
* End of function R_CMT_I2cTimerReset
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CMT_I2cTimerStart
* Description  : This function starts the channel 3  Timer from a reset state
* Argument     : none
* Return value : none
*******************************************************************************/
void R_CMT_I2cTimerStart (void)
{
    R_CMT_I2cTimerReset();
    R_CMT3_Start();
}

/*******************************************************************************
* End of function R_CMT_I2cTimerStart
*******************************************************************************/

/********************************************************************************
* Function Name: R_CMT_MsDelay
* Description  : Uses CMT0 to wait for a specified number of milliseconds
* Arguments    : uint16_t millisecs, number of milliseconds to wait
* Return Value : None
*******************************************************************************/
void R_CMT_MsDelay (const uint16_t millisec)
{
    /* Reset the global counter*/
    g_1ms_counter = 0;

	/* Start the 1ms CMT */
	R_CMT0_Start();

	while (g_1ms_counter <= millisec)
	{
		/* Wait */
	}

	/* Stop the 1ms CMT */
	R_CMT0_Stop();
}

/*******************************************************************************
End of function R_CMT_MsDelay
*******************************************************************************/

/********************************************************************************
* Function Name: R_CMT_DebounceDelay
* Description  : Waits for a pre-determined number of timer ticks
* 			   : before returning. Used to provide a delay when the user
* 			   : switches have been pressed to ensure switch bounce does not
* 			   : appear as multiple presses,
* Arguments    : None
* Return Value : None
**********************************************************************************/
void R_CMT_DebounceDelay(void)
{

	R_CMT_MsDelay(DEBOUNCE_TICKS * 100);
}

/*****************************************************************************
End of Function R_CMT_DebounceDelay
******************************************************************************/


/* End user code. Do not edit comment generated here */
