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
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Contains ADC module initialisation. No interrupts used.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes
*******************************************************************************/

/* Provides a structure to access on-chip I/O registers. */
#include "iorx63n.h"
/* Defines adc functions and variables */
#include "adc.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Outline       : Init_ADC
* Description   : Initialises the 12-bit ADC module.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADC(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;
    
    /* Use the AN000 (Potentiometer) pin 
       as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;
    
    /* ADC clock = PCLK/8, single scan mode */
    S12AD.ADCSR.BYTE = 0x00u;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001u;      
}
/******************************************************************************
* End of function Init_ADC
******************************************************************************/

/******************************************************************************
* Outline       : Start_ADC
* Description   : Starts the 12-bit ADC module.
* Argument      : none
* Return value  : none
******************************************************************************/
void Start_ADC(void)
{
    /* Start a conversion */
    S12AD.ADCSR.BIT.ADST = 1u;
}
/******************************************************************************
* End of function Start_ADC
******************************************************************************/

/******************************************************************************
* Outline       : Read_ADC
* Description   : Returns the 12-bit ADC result.
* Argument      : none
* Return value  : uint16_t 
******************************************************************************/
uint16_t Read_ADC(void)
{
    /* Wait for the conversion to end */
    while(1u == S12AD.ADCSR.BIT.ADST);
    
    /* Return the ADC value */
    return(S12AD.ADDR0 & ADC_MASK);
}
/******************************************************************************
* End of function Read_ADC
******************************************************************************/
