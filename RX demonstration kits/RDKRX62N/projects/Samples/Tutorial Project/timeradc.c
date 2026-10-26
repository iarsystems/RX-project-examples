/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : timerADC.c 
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines timer and ADC functions used in this tutorial.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* CMT RPDL function defintions */
#include "r_pdl_tmr.h"
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and global variables used in this file */
#include "switch.h"
/* Defines LED flash functions used in this file */
#include "flashled.h"
/* Provides declarations of functions defined in this file */
#include "timeradc.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* AD conversion complete callback function prototype delcaration */
void CB_ADConversion(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Timer period global variable - time between LED flashes */
volatile float gTimerPeriod = 100E-3;

/*******************************************************************************
* Outline      : TimerADC
* Description  : The function configures a ADC unit and a timer unit, so that an
*                AD conversion is triggered every period of the timer. After each
*                AD conversion, the callback function, CB_ADConversion, is 
*                executed.
* Argument     : none
* Return value : none
*******************************************************************************/
void TimerADC(void)
{
  /* Declare error flag */
  bool err = true;

#if 0            
  /* Initialise ADC */
  err &=    R_ADC_10_Create
    (
    0,
    PDL_ADC_10_MODE_SINGLE
    | PDL_ADC_10_CHANNELS_OPTION_1
    | PDL_ADC_10_TRIGGER_TMR0_CM_A,
    48E6,
    4E-6,
    CB_ADConversion,
    6
    );
#endif

  /* Configure the ADC unit */
  err &=    R_ADC_12_Create
    (
    0,
    PDL_ADC_12_SCAN_SINGLE
    | PDL_ADC_12_CHANNEL_4
    | PDL_ADC_12_DATA_ALIGNMENT_RIGHT
    | PDL_ADC_12_DIV_1,
    PDL_ADC_12_TRIGGER_TMR0,
    PDL_NO_DATA,
    CB_ADConversion,
    6
    );


  /* Configure TMR unit to trigger AD conversion every timer period */
  err &=    R_TMR_CreatePeriodic
    (
    PDL_TMR_UNIT0,
    PDL_TMR_PERIOD | PDL_TMR_ADC_TRIGGER_ON,
    gTimerPeriod,
    5E-3,
    PDL_NO_FUNC,
    PDL_NO_FUNC,
    0
    );

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function TimerADC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_ADConversion
* Description  : This callback function is executed after every successful AD
*                conversion. The function fetches the ADC result, and uses it to
*                calculate a new timer period, and then updates the timer with
*                this value. The function also toggles the LEDs with each
*                execution.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_ADConversion(void)
{
  /* Declare error flag */
  bool err = true;

  /* Declare temporary ADC results container variable */
  uint16_t adc_results[8];

#if 0    
  /* Fetch the result of the AD conversion, and store in the array local array
  adc_results */
  err &=    R_ADC_10_Read
    (
    0,
    adc_results
    );
#endif            

  /* Fetch the result of the AD conversion, and store in the array local array
  adc_results */
  err &=    R_ADC_12_Read
    (
    0,
    adc_results
    );

  /* Recalculate the new timer period to make the flash period visible,
  using the following equation:

  adc_value * 2
  ------------- + 0.0035  =>  min = 35ms, max = 208.3ms           
  10000                                                       */
  gTimerPeriod = ((adc_results[4] >> 2) * 2E-4) + 35E-3;

  /* Toggle the user LEDs */    
  ToggleLED();

  /* Update the timer with the new timer period, from the global variable
  gTimerPeriod */
  err &=    R_TMR_ControlPeriodic
    (
    PDL_TMR_UNIT0,
    PDL_TMR_PERIOD,
    gTimerPeriod,
    5E-3
    );

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_ADConversion
*******************************************************************************/
