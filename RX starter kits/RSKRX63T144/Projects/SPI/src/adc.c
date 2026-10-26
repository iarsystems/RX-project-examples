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
* File Name     : adc.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Provides declarations of functions defined in this file */
#include "adc.h"

/*******************************************************************************
* Function Name: Init_ADC
* Description  : ADC initialisation function. This function configures the
*                S12AD unit for single scan operation.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void Init_ADC (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;

    /* Use the AN000 (Potentiometer) pin
       as an I/O for peripheral functions */

    /* ADC single scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x00;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u;
}
/*******************************************************************************
* End of function Init_ADC
*******************************************************************************/

/******************************************************************************
* Function Name: Run_ADC_Single
* Description  : Runs an A-D conversion and returns the result.
* Arguments    : none
* Return Value : uint16_t result
******************************************************************************/
uint16_t Run_ADC_Single (void)
{
    /* Start a conversion */
    S12AD.ADCSR.BIT.ADST = 0x1u;

    /* Wait for the conversion to end */
    while (0x1 == S12AD.ADCSR.BIT.ADST)
    {
    }

    /* Return the S12AD value */
    return (S12AD.ADDR0);
}
/******************************************************************************
* End of function Run_ADC_Single
******************************************************************************/

