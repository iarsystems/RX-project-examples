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
* File Name     : adc12repeat.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines ADC repeat functions used for 12bit ADC unit.
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
/* Defines port registers */
#include "iorx63n.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "adc12repeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable gADC_Result is used to contain the value of the 12 bit ADC */
volatile uint16_t gADC_Result;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Init_Timer function prototype */
static void InitTimer_ADC12Repeat(void);

/*******************************************************************************
* Outline       : Init_ADC12Repeat
* Description   : ADC initialisation function. This function configures the ADC
*                  unit for continuous scan operation, then configures a timer  
*                  for a periodic interrupt every 300 milliseconds.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADC12Repeat(void)
{
    /* Initialise CMT2 */
    InitTimer_ADC12Repeat();
    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;
    
    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;
    
    /* ADC clock = PCLK/8, continuous scan mode */
    S12AD.ADCSR.BYTE |= 0x40u;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001u;    
    
    /* Set the sampling cycle to 255 states (approximately 5 us) */
    S12AD.ADSSTR23.WORD = 0x14FFu;
        
    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 1;
}
/*******************************************************************************
* End of function Init_ADC12Repeat
*******************************************************************************/

/*******************************************************************************
* Outline      : InitTimer_ADC12Repeat
* Description  : This function initialises the CMT timer to generate a interval
*                interrupt, which is used to refresh the debug LCD with the 
*                latest ADC value.
* Argument     : none
* Return value : none
*******************************************************************************/
static void InitTimer_ADC12Repeat(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;    
    
    /* Set CMT2 interrupt priority level to 5 */    
    IPR(CMT2,CMI2) = 0x5u;
    
    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1u;
    CMT2.CMCR.BIT.CMIE = 0x1u;
    
    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0u;
    
    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2u;
        
    /* Set compare match value to generate 200ms interval (750 cycles @ 375KHz) */
    CMT2.CMCOR = 750u;   
    
    /* Start timer running */
    CMT.CMSTR1.BIT.STR2 = 0x1u;
}
/*******************************************************************************
* End of function InitTimer_ADC12Repeat
*******************************************************************************/

/******************************************************************************
* Outline       : Excep_CMTU2_CMT2
* Description   : Compare match timer channel 2 interrupt handler. The function
*                 is trigger every compare match interval, and updates the 
*                 debug LCD with the latest ADC value.
* Argument          : none    
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMT2_CMI2(void)
{    
    /* Declare temporary character string */
    uint8_t lcd_buffer[9] = "=H\'WXYZ ";
    
    /* Fetch ADC value */
    gADC_Result = (uint16_t)(S12AD.ADDR0 & 0x0FFF);
        
    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */    
    u16ToString(lcd_buffer, 3, gADC_Result);
    
    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);    
}
/******************************************************************************
* End of function Excep_CMTU2_CMT2
******************************************************************************/
