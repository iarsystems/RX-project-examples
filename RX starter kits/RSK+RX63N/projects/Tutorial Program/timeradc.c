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
* File Name    : timeradc.c
* Version     : 1.00
* Device     : R5F563NB
* Tool-Chain   : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description    : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History     : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes 
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>
/* Defines RX630 port registers */
#include "iorx63n.h"
/* Defines macros relating to the user LEDs and switches */
#include "rskrx63ndef.h"
/* LCD controlling function prototypes & macro definitions */
#include "timeradc.h"
/* Flashing LED function definitions */
#include "flashLED.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* StartTimer function declaration */
static void StartTimer(void);
/* StartADC function declaration */
static void StartADC(void);

/*******************************************************************************
* Outline     : Timer_ADC
* Description   : Function initialises the Timer and ADC peripherals needed to 
*          vary the LED flash rate based on the setting of potentiometer.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Timer_ADC(void)
{
    /* Call the ADC start function */
  StartADC();  
         
  /* Call the Timer start function */
  StartTimer();
}  
/*******************************************************************************
* End of function Timer_ADC
*******************************************************************************/  
  
/*******************************************************************************
* Outline     : StartTimer
* Description   : Configures CMT channel 1 to call the 'CB_TimerADC' callback 
*          function which starts the AD conversion
* Argument    : none
* Return value  : none
*******************************************************************************/
static void StartTimer(void)
{  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the CMT1 module clock stop mode */
  MSTP_CMT1 = 0;
  /* Cancel the CMT2 module clock stop mode */
  MSTP_CMT2 = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
  
  /* Set CMT1 interrupt priority level to 10 */  
  IPR(CMT1,CMI1) = 0xA;
  /* Enable CMT1 interrupts */
  IEN(CMT1,CMI1) = 0x1;
  /* Clear CMT1 interrupt flag */
  IR(CMT1,CMI1) = 0x0;
  
  /* Set CMT2 interrupt priority level to 5 */
  IPR(CMT2,CMI2) = 0x5;
  /* Enable CMT2 interrupts */
  IEN(CMT2,CMI2) = 0x1;  
  /* Clear CMT2 interrupt flag */
  IR(CMT2,CMI2) = 0x0;
    
  /* Select the PCLK clock division as PCLK/512 = 93.75KHz */ 
  CMT1.CMCR.BIT.CKS = 0x3;
  
  /* Specify the timer period */
  CMT1.CMCOR = (uint16_t)((S12AD.ADDR0 * 16) + 1);    
  
  /* Enable the compare match interrupt */
  CMT1.CMCR.BIT.CMIE = 0x1;

  /* Start CMT1 count */
  CMT.CMSTR0.BIT.STR1 = 0x1;
}
/*******************************************************************************
* End of function StartTimer
*******************************************************************************/

/*******************************************************************************
* Outline     : StartADC
* Description   : Initialises the S12AD ADC module's channel AN000 for  
*          continuous scan operations, then starts the conversions.
* Argument    : none
* Return value  : none
*******************************************************************************/
static void StartADC(void)
{  
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
* End of function StartADC
*******************************************************************************/

/******************************************************************************
* Outline    : Excep_CMTU0_CMT1
* Description  : CMT1 interrupt handler. This function calls the function 
*          Toggle_LED to toggle the user LEDs and updates the timer's 
*          period using the A/D conversion value.
* Argument      : none  
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT1_CMI1
__interrupt void Excep_CMTU0_CMT1(void)
{  
  /* Toggle LEDs */
  Toggle_LED();
    
    /* Fetch latest ADC value */
    uint16_t adc_value = S12AD.ADDR0;
  
  /* Update the timer */
  CMT1.CMCOR =(uint16_t)(((adc_value/50) * 200) + 3000);
}
/******************************************************************************
* End of function Excep_CMTU0_CMT1
******************************************************************************/
