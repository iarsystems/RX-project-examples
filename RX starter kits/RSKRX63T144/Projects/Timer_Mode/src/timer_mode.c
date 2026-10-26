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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : timer_mode.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
*******************************************************************************/
/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define for number of 24MHz PCLKA/4 timer ticks in a ms */
#define TCLK_TICKS_PER_MS 24000

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Declares function prototypes defined in this file */
#include "timer_mode.h"

/*******************************************************************************
* Function Name : Init_TimerMode
* Description   : Function configures the MTU3's channel 3 output pin MTIOC3B
*                 using compare match method to output a 1KHz square-wave. The
*                 MTIOC3B pin is automatically toggled after each compare match.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TimerMode (void)
{
    /* Configure pin 1 of port 7 as an output */
    PORT7.PDR.BIT.B1 = 0x1;

    /* Configure pin 1 of port 7 for peripheral function */
    PORT7.PMR.BIT.B1 = 0x1;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;

    /* Configure pin 1 of port 7 for MTIOC3B outputs */
    MPC.P71PFS.BIT.PSEL = 0x1;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the MTU3 module clock stop mode */
    MSTP_MTU3 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Clock PCLKA/4=24MHz, count at falling edge,
       TCNT cleared by TGRB compare match */
    MTU3.TCR.BIT.TPSC = 0x1;
    MTU3.TCR.BIT.CKEG = 0x1;
    MTU3.TCR.BIT.CCLR = 0x2;

    /* Operate timers in normal mode */
    MTU3.TMDR1.BIT.MD = 0x0;

    /* Initial output is 0 and toggled at TRGB compare match  */
    MTU3.TIORH.BYTE = 0x30;

    /* Set a toggle period of 500 us */
    MTU3.TGRB = TCLK_TICKS_PER_MS / 2;

    /* Enable MTIOC3B output pin */
    MTU.TOERA.BIT.OE3B = 0x1;

    /* Start MTU3 TCNT counter */
    MTU.TSTRA.BIT.CST3 = 0x1;
}
/*******************************************************************************
* End of function Init_TimerMode
*******************************************************************************/

