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
* File Name     : timer_mode.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Declares function prototypes defined in this file */
#include "timer_mode.h"

/*******************************************************************************
* Outline         : Init_TimerMode
* Description     : Function configures the MTU2a's channel 3 output pin MTIOC3B 
*                  using compare match method to output a 1KHz square-wave. The 
*                  MTIOC3B pin is automatically toggled after each compare match.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TimerMode(void)
{    
    /* Configure pin 5 of port C as an output */
    PORTC.PDR.BIT.B5 = 1;
    
    /* Configure pin 5 of port C for peripheral function */
    PORTC.PMR.BIT.B5 = 1;
    
    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;
    
    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;
    
    /* Configure pin 5 of port C for MTIOC3B outputs */
    MPC.PC5PFS.BIT.PSEL = 0x1;
    
    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;
    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the MTU0 module clock stop mode */
    MSTP_MTU0 = 0;
        
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
    
    /* Set MTU3 TGIB3 interrupt priority level to 7 */    
    IPR(MTU3,TGIB3) = 0x7;
    /* Enable MTU3 TGIB3 interrupts */
    IEN(MTU3,TGIB3) = 0x1;
    /* Clear MTU3 TGIB3 interrupt flag */
    IR(MTU3,TGIB3) = 0x0;
                              
    /* Clock PCLK/64, count at falling edge, TCNT cleared by TRGB compare match */
    MTU3.TCR.BYTE = 0x4B;    
    
    /* Operate timers in normal mode */
    MTU3.TMDR.BYTE = 0;
    
    /* Initial output is 0 and toggled at TRGB compare match  */
    MTU3.TIORH.BYTE = 0x30;    
    
    /* Enable TGIEA interrupts */
    MTU3.TIER.BYTE = 0x02;
    
    /* Enable access to protected MTU registers */
    MTU.TRWER.BIT.RWE = 0x1;
     
    /* Set a period of 500us */
    MTU3.TGRB = 0x176;
    
    /* Enable MTIOC3B output pin */
    MTU.TOER.BIT.OE3B = 0x1;
    
    /* Start MTU3 TCNT counter */
    MTU.TSTR.BIT.CST3 = 0x1;
}
/*******************************************************************************
* End of function Init_TimerMode
*******************************************************************************/


/*******************************************************************************
* Outline         : Excep_TPU9_TGIB9
* Description     : MTU3 timer compare match input routine. Toggles LED0 on match
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector = VECT_TPU9_TGI9B
__interrupt void Excep_TPU9_TGIB9(void)
{
    LED0 = ~LED0;
}
/*******************************************************************************
* End of function Excep_TPU9_TGIA9
*******************************************************************************/
