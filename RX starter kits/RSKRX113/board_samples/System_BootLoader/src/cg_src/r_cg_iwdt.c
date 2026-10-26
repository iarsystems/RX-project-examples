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
* File Name    : r_cg_iwdt.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for IWDT module.
* Creation Date: 2015-01-21
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
#include "r_cg_iwdt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_IWDT_Create
* Description  : This function initializes IWDT module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IWDT_Create(void)
{
    /* Set control registers */
    IWDT.IWDTCR.WORD = _0000_IWDT_TIMEOUT_128 | _0040_IWDT_CLOCK_DIV64 | _0300_IWDT_WINDOW_END_0 | 
                       _3000_IWDT_WINDOW_START_100;
    IWDT.IWDTRCR.BYTE = _00_IWDT_NMI_INTERRUPT;
    IWDT.IWDTCSTPR.BYTE = _80_IWDT_COUNT_STOP_ENABLE;

    /* Enable NMI pin interrupt */
    ICU.NMIER.BIT.IWDTEN = 1U;

}

/***********************************************************************************************************************
* Function Name: R_IWDT_Restart
* Description  : This function restarts IWDT module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IWDT_Restart(void)
{
    /* Refreshed by writing 00h and then writing FFh */
    IWDT.IWDTRR = 0x00U;
    IWDT.IWDTRR = 0xFFU;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
