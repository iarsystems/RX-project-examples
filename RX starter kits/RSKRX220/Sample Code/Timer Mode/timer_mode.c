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
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines MTU2 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Declares function prototypes defined in this file */
#include "timer_mode.h"


/*******************************************************************************
* Function Name: init_timer_mode
* Description  : Function configures the MTU2a's channel 3 output pin MTIOC3A 
*                using compare match method to output a 1KHz square-wave. The 
*                MTIOC3A pin is automatically toggled after each compare match.
* Argument     : none
* Return value : none
*******************************************************************************/
void init_timer_mode (void)
{
    /* Configure pin 1 of port C as an output */
    PORTC.PDR.BIT.B1 = 0x1u;
    
    /* Configure pin 1 of port C for peripheral function */
    PORTC.PMR.BIT.B1 = 0x1u;
    
    /* Configure pin 4 of port 1 for peripheral function */
    PORT1.PMR.BIT.B4 = 0x1u;
    
    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;
    
    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;
    
    /* Configure pin 1 of port C for MTIOC3A outputs */
    MPC.PC1PFS.BIT.PSEL = 0x1u;
    
    /* Configure pin 4 of port 1 (LED0) for MTIOC3A outputs */
    MPC.P14PFS.BIT.PSEL = 0x1u;
    
    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;
    
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;
    
    /* Cancel the MTU0 module clock stop mode */
    MSTP_MTU0 = 0x0;
        
    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;
                            
    /* Clock PCLK/64=312.5kHz, count at falling edge, TCNT cleared by TRGA
       compare match */
    MTU3.TCR.BYTE = 0x2Bu;   
    
    /* Operate timers in normal mode */
    MTU3.TMDR.BYTE = 0x0;
    
    /* Initial output is 0 and toggled at TRGA compare match  */
    MTU3.TIORH.BYTE = 0x03u; 
    
    /* Enable access to protected MTU registers */
    MTU.TRWER.BIT.RWE = 0x1u;
     
    /* Set a period of 500us */
    MTU3.TGRA = 0x09Cu;
    
    /* Start MTU3 TCNT counter */
    MTU.TSTR.BIT.CST3 = 0x1u;
}
/*******************************************************************************
* End of function init_timer_mode
*******************************************************************************/

