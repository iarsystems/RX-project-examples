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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : timeradc.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares functions defined in this file */
#include "timeradc.h"
/* Flashing LED function definitions */
#include "flashLED.h"

/*******************************************************************************
Function Prototypes
*******************************************************************************/
/* StartTimer function declaration */
static void StartTimer(void);
/* StartADC function declaration */
static void StartADC(void);
/* ADC interrupt callback function declaration */
static void CB_ADConversion(void);
/* Timer callback function declaration */
static void CB_Timer(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable ad_value is used to contain the value of the ADC */
uint16_t ad_value[16];

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
* Description   : Configures CMT channel 0 to call the 'CB_TimerADC' callback 
*          function which starts the AD conversion
* Argument    : none
*******************************************************************************/
static void StartTimer(void)
{
  /* Declare error flag */
  bool err = true;
             
  /* Configure compare match timer */
  err &=  R_CMT_Create
      (
        0, 
        PDL_CMT_PCLK_DIV_512, 
        0x03FF, 
        CB_Timer,
        3
      );  

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function StartTimer
*******************************************************************************/

/*******************************************************************************
* Outline     : StartADC
* Description   : Initialises the ADC's channel 0 for one-shot operations and  
*          set the callback function.
* Argument    : none
* Return value  : none
*******************************************************************************/
static void StartADC(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Set analog channel AN000 */
  err &=  R_ADC_12_Set
      (
        PDL_ADC_12_PIN_AN000_P40 
      );
  
  /* Configure the ADC in single mode, software triggered */
  err &=  R_ADC_12_CreateUnit
      (
        0,
        PDL_ADC_12_SCAN_SINGLE,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        CB_ADConversion,
        6,
        PDL_NO_FUNC,
        PDL_NO_DATA
      );            

  /* Configure the ADC channel AN000 */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        0,
        PDL_ADC_12_CH_GROUP_A|\
        PDL_ADC_12_CH_SAMPLE_AND_HOLD_ENABLE,
        5E-6
      );

  /* Start the AD conversion  */
  err &=  R_ADC_12_Control
      (
        PDL_ADC_12_0_ON
      );  
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function StartADC
*******************************************************************************/

/*******************************************************************************
* Outline     : CB_ADConversion
* Description   : Callback function that is executed after a successful A/D 
*          conversion. Function toggles the LEDs, reads the ADC value 
*          and sets the compare match value with the it - thereby 
*          varying the LED blink rate.
* Argument    : none
* Return value   : none
*******************************************************************************/
static void CB_ADConversion(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Initialises a temporary copy of 'ad_value' storage variable */
  uint16_t ad_value_x16 = 0x0;
  
  /* Fetch ADC value */
  err &=  R_ADC_12_Read
      (
        0, 
        ad_value,
        PDL_NO_PTR
      );

  /* Toggle LEDs */
  Toggle_LED();
    
  /* Increase clock delay by a factor of 16 */
  ad_value_x16 = (uint16_t)((ad_value[0] * 0x10));
  
  /* Adjust the minimum value to ensure blink rate is visible */
  if(ad_value_x16 < 1800)
  {
    ad_value_x16 = 1800;
  }
       
  /* Start a new timer delay */
  err &=  R_CMT_Control
      (
        0, 
        PDL_CMT_CONSTANT, 
        (double)ad_value_x16
      );

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_ADConversion
*******************************************************************************/

/*******************************************************************************
* Outline     : CB_Timer
* Description   : Callback function that is executed after a compare match
*          occurs in the Timer peripheral. Function starts an AD
*          conversion.
* Argument    : none
* Return value  : none
*******************************************************************************/
static void CB_Timer(void)
{
  /* Declare error flag */
  bool err = true;

  /* Start A/D conversion */
  err &=  R_ADC_12_Control
      (
        PDL_ADC_12_0_ON
      );

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_Timer
*******************************************************************************/
