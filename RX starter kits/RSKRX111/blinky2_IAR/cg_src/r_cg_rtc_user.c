/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc_user.c
* Version      : Applilet4 for RX111 V1.00.00.02 [27 Mar 2013]
* Device(s)    : R5F51115AxLF
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for RTC module.
* Creation Date: 4/4/2013
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
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern unsigned char LED_NO;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_rtc_prd_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RTC_PRD
#if FAST_INTERRUPT_VECTOR == VECT_RTC_PRD
__fast_interrupt static void r_rtc_prd_interrupt(void)
#else
__interrupt static void r_rtc_prd_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
  switch (LED_NO) 
  {
    case 0:
          LED0 = LED_On;	                // turn on LED
          LED1 = LED2 = LED3 = LED_Off;         // turn off LED
          LED_NO++;
          break;
    case 1:
          LED1 = LED_On;	                // turn on LED
          LED0 = LED2 = LED3 = LED_Off;         // turn off LEDs
          LED_NO++;
          break;
    case 2:
          LED2 = LED_On;	                // turn on LED
          LED0 = LED1 = LED3 = LED_Off;         // turn off LEDs
           LED_NO++;
         break;
     case 3:
          LED3 = LED_On;	                // turn on LED
          LED0 = LED1 = LED2 = LED_Off;         // turn off LEDs
          LED_NO++;
          break;
     case 4:
          LED0 = LED1 = LED2 = LED3 = LED_On;  // turn on LEDs
          LED_NO++;
          break;
    default:
          LED0 = LED1 = LED2 = LED3 = LED_Off;  // turn off LEDs
          LED_NO = 0 ;
          break;
  }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
