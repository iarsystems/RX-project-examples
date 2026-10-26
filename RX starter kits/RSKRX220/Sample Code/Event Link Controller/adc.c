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
* File Name     : adc.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Provides declarations of functions defined in this file */
#include "adc.h"

/*******************************************************************************
* Function Name: init_adc
* Description  : ADC initialisation function. This function configures the 
*                S12AD unit for single scan operation.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_adc (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;
    
    /* Use the AN000 (Potentiometer) pin 
       as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;
    
    /* ADC single scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x0;
    
    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u; 
}
/*******************************************************************************
* End of function init_adc12Oneshot
*******************************************************************************/

/******************************************************************************
* Function Name: run_adc_single
* Description  : Runs an A-D conversion and returns the result.
* Arguments    : none  
* Return Value : uint16_t - Result of ADC conversion
******************************************************************************/
uint16_t run_adc_single (void)
{
    /* Start a conversion */
    S12AD.ADCSR.BIT.ADST = 0x1u;
    
    /* Wait for the conversion to end */
    while (0x1 == S12AD.ADCSR.BIT.ADST)
    {
        /* Do nothing until ADC conversion completes */
    }

    /* Return the S12AD value */
    return (S12AD.ADDR0);  
}
/******************************************************************************
* End of function run_adc_single
******************************************************************************/

