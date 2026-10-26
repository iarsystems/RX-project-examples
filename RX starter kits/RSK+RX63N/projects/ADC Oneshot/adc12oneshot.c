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
* File Name     : adc12oneshot.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes 
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "adc12oneshot.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable gADC_Result is used to contain the value of the 12 bit ADC */
volatile uint16_t gADC_Result;
/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Switch press callback function prototype */
static void SwitchCB_ADC12Oneshot(void);

/*******************************************************************************
* Outline       : Init_ADC12Oneshot
* Description   : ADC initialisation function. This function configures the 
*                  S12AD unit for single scan operation.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADC12Oneshot(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
    
    /* Use the AN000 (Potentiometer) pin 
       as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01;
    
    /* ADC clock = PCLK/8, single scan mode */
    S12AD.ADCSR.BYTE = 0x00;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001;      
    
    /* Configure a switch press callback function */
    SetSwitchPressCallback(SwitchCB_ADC12Oneshot);
}
/*******************************************************************************
* End of function Init_ADC12Oneshot
*******************************************************************************/

/*******************************************************************************
* Outline       : FetchResult_ADC12Oneshot
* Description   : This function triggers an AD conversion, waits for it to 
*                 complete then returns the result.
* Argument      : none
* Return value  : Returns the 12bit ADC value as a 16bit unsigned integer.
*******************************************************************************/
uint16_t FetchResult_ADC12Oneshot(void)
{
    /* Start a conversion */
    S12AD.ADCSR.BIT.ADST = 1;
    
    /* Wait for the conversion to end */
    while(1 == S12AD.ADCSR.BIT.ADST);
    
    /* Fetch ADC value */
    uint16_t adc_value = (uint16_t)(S12AD.ADDR0 & 0x0FFF);
    
    /* Return the ADC value */
    return adc_value;
}
/*******************************************************************************
* End of function FetchResult_ADC12Oneshot
*******************************************************************************/

/*******************************************************************************
* Outline       : SwitchCB_ADC12Oneshot
* Description   : ADC initialisation function. This function configures the 
*                  S12AD unit for single scan operation.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void SwitchCB_ADC12Oneshot(void)
{
    /* Check if switch SW3 was pressed */
    if(gSwitchFlag & SWITCHHOLD_3)
    {
        /* Declare temporary local lcd buffer string */
        uint8_t lcd_buffer[9u] = "=H\'WXYZ ";
    
        /* Fetch ADC value, and update global variable */
        gADC_Result = FetchResult_ADC12Oneshot();
    
        /* Convert ADC value into string and insert into lcd buffer */
        u16ToString(lcd_buffer, 3u, gADC_Result);
    
        /* Update the debug LCD screen with latest value */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }
}
/*******************************************************************************
* End of function SwitchCB_ADC12Oneshot
*******************************************************************************/
