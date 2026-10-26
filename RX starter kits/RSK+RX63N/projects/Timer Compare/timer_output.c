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
* File Name     : timer_output.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines timer functions used to configure the CMT unit to 
*                  toggle the user LEDs on every compare match.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RSK board LEDs and switches */
#include "rskrx63ndef.h"
/* Defines MCU port registers */
#include "iorx63n.h"
/* Provides declarations of functions defined in this file */
#include "timer_output.h"

/******************************************************************************
* Outline      : Init_CompareMatchTimer
* Description  : This function initialises a CMT timer to generate a compare 
*                 match interrupt every 100ms.
* Argument     : none
* Return value : none
******************************************************************************/
void Init_CompareMatchTimer(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;
        
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
    
    /* Set CMT2 interrupt priority level to 7*/
    IPR(CMT2,CMI2) = 0x7;
    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;    
    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;
    
    /* Select the PCLK clock division as PCLK/512 = 93.75KHz */ 
    CMT2.CMCR.BIT.CKS = 0x3;
    
    /* Specify the timer period */
    CMT2.CMCOR = 9375;
        
    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;
}
/******************************************************************************
* End of function Init_CompareMatchTimer
******************************************************************************/

/*******************************************************************************
* Outline       : Excep_CMTU1_CMT2
* Description   : The CMT2 interrupt handler is called after every CMT 100ms 
*                 period. The function toggles the user LEDs.
* Argument      : none    
* Return value  : none
********************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{
    /* Toggle LEDs */    
    LED0 = (uint8_t)~LED0;
    LED1 = (uint8_t)~LED1;
    LED2 = (uint8_t)~LED2;
    LED3 = (uint8_t)~LED3;
}
/********************************************************************************
* End of function Excep_CMTU1_CMT2
********************************************************************************/
