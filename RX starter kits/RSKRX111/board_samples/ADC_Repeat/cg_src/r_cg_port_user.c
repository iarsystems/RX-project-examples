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
* File Name    : r_cg_port_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for Port module.
* Creation Date: 2013-08-15
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
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */


/* Start user code for adding. Do not edit comment generated here */
void LED0_On(void)
{
    /* LED0 is located on port B7 */
    PORTB.PODR.BYTE &= ~(_80_Pm7_OUTPUT_1);
}

void LED0_Off(void)
{
    /* LED0 is located on port B7 */
    PORTB.PODR.BYTE |= _80_Pm7_OUTPUT_1;
}

void LED0_Toggle(void)
{
    /* LED0 is located on port B7 */
    PORTB.PODR.BYTE ^= _80_Pm7_OUTPUT_1;
}

void LED1_On(void)
{
    /* LED1 is located on port A0 */
    PORTA.PODR.BYTE &= ~(_01_Pm0_OUTPUT_1);
}

void LED1_Off(void)
{
    /* LED1 is located on port A0 */
    PORTA.PODR.BYTE |= _01_Pm0_OUTPUT_1;
}

void LED1_Toggle(void)
{
    /* LED1 is located on port A0 */
    PORTA.PODR.BYTE ^= _01_Pm0_OUTPUT_1;
}

void LED2_On(void)
{
    /* LED0 is located on port 54 */
    PORT5.PODR.BYTE &= ~(_10_Pm4_OUTPUT_1);
}

void LED2_Off(void)
{
    /* LED0 is located on port 54 */
    PORT5.PODR.BYTE |= _10_Pm4_OUTPUT_1;
}

void LED2_Toggle(void)
{
    /* LED0 is located on port 54 */
    PORT5.PODR.BYTE ^= _10_Pm4_OUTPUT_1;
}

void LED3_On(void)
{
    /* LED3 is located on port B6 */
    PORTB.PODR.BYTE &= ~(_40_Pm6_OUTPUT_1);
}

void LED3_Off(void)
{
    /* LED3 is located on port B6 */
    PORTB.PODR.BYTE |= _40_Pm6_OUTPUT_1;
}

void LED3_Toggle(void)
{
    /* LED3 is located on port B6 */
    PORTB.PODR.BYTE ^= _40_Pm6_OUTPUT_1;
}


/* End user code. Do not edit comment generated here */
