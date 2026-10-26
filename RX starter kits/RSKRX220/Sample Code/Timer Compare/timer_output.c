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
* File Name     : timer_output.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines timer functions used to configure the CMT unit to
*                 toggle the user LEDs on every compare match.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Provides declarations of functions defined in this file */
#include "timer_output.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Name: init_compare_match_timer
* Description  : This function initialises a CMT timer to generate a compare
*                match interrupt every 100ms.
* Arguments    : none
* Return Value : none
******************************************************************************/
void init_compare_match_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set CMT2 interrupt priority level to 7*/
    IPR(CMT2, CMI2) = 0x7u;

    /* Enable CMT2 interrupts */
    IEN(CMT2, CMI2) = 0x1u;

    /* Clear CMT2 interrupt flag */
    IR(CMT2, CMI2) = 0x0;

    /* Select the PCLK clock division as PCLK/512 = 39.06KHz */
    CMT2.CMCR.BIT.CKS = 0x3u;

    /* Specify the timer period */
    CMT2.CMCOR = 3906u;

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x1u;
}
/******************************************************************************
* End of function init_compare_match_timer
******************************************************************************/

/*******************************************************************************
* Function Name: Excep_CMTU1_CMT2_isr
* Description  : The CMT2 interrupt handler is called after every CMT 100ms
*                period. The function toggles the user LEDs.
* Arguments    : none
* Return Value : none
********************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMTU1_CMT2_isr(void)
{
    /* Toggle LEDs */
    LED0 = (uint8_t) ~LED0;
    LED1 = (uint8_t) ~LED1;
    LED2 = (uint8_t) ~LED2;
    LED3 = (uint8_t) ~LED3;
}
/********************************************************************************
* End of function Excep_CMTU1_CMT2_isr
********************************************************************************/

