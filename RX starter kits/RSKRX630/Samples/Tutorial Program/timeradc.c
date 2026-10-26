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
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description    : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
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
#include "iorx630.h"
/* Defines macros relating to the RX630 user LEDs and switches */
#include "rskrx630def.h"
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
* Global Variables
*******************************************************************************/
/* The variable gADC_Result is used to contain the value of the 12 bit ADC */
volatile uint16_t gADC_Result;
/* Declare a variable to selec*/
volatile uint32_t gPeriodic_Delay;
/* Declare a variable to store the global count value */
volatile uint32_t gDelay_Counter = 0;

/*******************************************************************************
* Outline     : Timer_ADC
* Description   : Function initialises the Timer and ADC peripherals needed to 
*          vary the LED flash rate based on the setting of potentiometer.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Timer_ADC(void)
{       
  /* Call the Timer start function */
  StartTimer();
  
  /* Call the ADC start function */
  StartADC();  
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
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the S12AD module clock stop mode */
  MSTP_S12AD = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;
    
  /* Clear the S12AD interrupt flag */
  IR(S12AD, S12ADI0) = 0x0;
  
  /* Set the S12AD interrupt level to 5 */
  IPR(S12AD, S12ADI0) = 0x5;
  
  /* Enable S12AD interrupt requests */
  IEN(S12AD, S12ADI0) = 0x1;

  /* Use the AN000 (Potentiometer) pin 
     as an I/O for peripheral functions */
  PORT4.PMR.BYTE = 0x01;
  
  /* ADC clock = PCLK/8, continuous scan mode */
  S12AD.ADCSR.BYTE |= 0x40;
  
  /* Selects AN000 */
  S12AD.ADANS0.WORD = 0x0001;  
  
  /* Enable scan end interrupts */
  S12AD.ADCSR.BIT.ADIE = 1;
  
  /* start ADC */
  S12AD.ADCSR.BIT.ADST = 1;
}
/*******************************************************************************
* End of function StartADC
*******************************************************************************/

/******************************************************************************
* Outline    : Timer_Delay
* Description  : Function used to create delays in milliseconds or 
*          microseconds depending on the user selection for controlling 
*          the debug LCD function's calls. 
* Argument      : uint32_t   -   _Delay_Period
*          int8     -   Unit  
* Return value  : none
******************************************************************************/
void Timer_Delay(uint32_t User_Delay, int8_t Unit)
{
  /* Clear the timer's count */
  gDelay_Counter = 0;
  
  /* Stor a copy of the user delay value to gPeriodic_Delay */
  gPeriodic_Delay = User_Delay;
    
  /* Check if microseconds delay is required */
  if(Unit == 'u')
  {  
    /* Select the PCLK clock division as PCLK/8 = 6MHz */ 
    CMT2.CMCR.BIT.CKS = 0x0;
  
    /* Specify the timer period */
    CMT2.CMCOR = (uint16_t)(User_Delay * 6);
  }
  
  /* Check if milliseconds delay is required */
  if(Unit == 'm')
  {
    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2;
  
    /* Specify the timer period */
    CMT2.CMCOR = (uint16_t)(User_Delay * 375);    
  }
  
  /* Enable the compare match interrupt */
  CMT2.CMCR.BIT.CMIE = 1;

  /* Start CMT2 count */
  CMT.CMSTR1.BIT.STR2 = 1;

  /* Wait for the timer to timeout */
  while(gDelay_Counter != User_Delay)
  {
    /* Wait */
  }
  
  /* Stop CMT2 count */
  CMT.CMSTR1.BIT.STR2 = 0;
  
  /* Clear counter */
  gDelay_Counter = 0;
}
/******************************************************************************
* End of function Timer_Delay
******************************************************************************/

/******************************************************************************
* Outline    : Excep_S12AD0_S12ADI0
* Description  : S12AD interrupt handler. This function stores the A/D value 
*          into variable gADC_Result.
* Argument      : none  
* Return value  : none
******************************************************************************/
#pragma vector=VECT_S12AD_S12ADI0
__interrupt void Excep_S12AD_S12ADI0(void)
{
  /* Fetch ADC value */
  gADC_Result = S12AD.ADDR0;
}
/******************************************************************************
* End of function Excep_S12AD0_S12ADI0
******************************************************************************/

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
  
  /* Update the timer */
  CMT1.CMCOR =(uint16_t)(((gADC_Result/50) * 200) + 3000);
}
/******************************************************************************
* End of function Excep_CMTU0_CMT1
******************************************************************************/

/******************************************************************************
* Outline    : Excep_CMTU1_CMT2
* Description  : CMT2 interrupt handler. This function increments the 
*          gDelay_Counter variable.
* Argument      : none  
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{
  /* Increment counter */
  gDelay_Counter++;
}
/******************************************************************************
* End of function Excep_CMTU1_CMT2
******************************************************************************/
