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
* File Name    : temp_sensor.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines temperature sensor & ADC controlling functions.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Provides access to the standard I/O function */
#include <stdio.h>
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* RWP RPDL function definitions */
#include "r_pdl_rwp.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "temp_sensor.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* ADC interrupt callback function prototype declaration */
void CB_Timer_TempSensor(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global ADC result container variable */
uint16_t usADC_Result = 0;
/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Outline     : Init_TempSensor
* Description   : Temp. sensor & ADC initialisation function. This function 
*          configures the temperature sensor & the ADC unit in single 
*          scan mode. Then, it configures a CMT timer for a periodic 
*          interrupt every 500ms.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_TempSensor(void)
{
  /* Declare error flag */
  bool err = true;

  /* Disable the write protection for module stop control register */
  err &=  R_RWP_Control
      (
        PDL_RWP_ENABLE_MODE_RESET_WRITE
      );

  /* Cancel the module stop state for temperature sensor */
  SYSTEM.MSTPCRB.BIT.MSTPB8 = 0;

  /* Enable the write protection for module stop control register */  
  err &=  R_RWP_Control
      (
        PDL_RWP_DISABLE_MODE_RESET_WRITE
      );
    
  /* Enable the temperature sensor  */
  TEMPS.TSCR.BIT.TSEN = 1;
  
  /* Set the PGA gain  */
  TEMPS.TSCR.BIT.PGAGAIN = 3;

  /* Configure the ADC to measure the temperature sensor output */
  err &=  R_ADC_12_CreateUnit
      (
        0,
        PDL_ADC_12_INPUT_TS|PDL_ADC_12_SCAN_SINGLE|\
         PDL_ADC_12_ADSSTR_CALCULATE | PDL_ADC_12_ADSHCR_CALCULATE,
        PDL_NO_DATA,
        PDL_NO_DATA,
        5E-6,
        3E-6,
        0,
        PDL_NO_FUNC,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        PDL_NO_DATA
      );            

  /* Configure CMT timer for temperature measurement intervals */
  err &=  R_CMT_Create
      (
        0,
        PDL_CMT_PERIOD,
        500E-3,
        CB_Timer_TempSensor,
        8
      );

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function Init_TempSensor
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Timer_TempSensor
* Description  : This callback function is executed when a compare match occurs
*         in the CMT timer (every 500ms). The function re-starts the 
*         temperature measurement.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Timer_TempSensor(void)
{
  /* Declare error flag */
  bool err = true;

  /* Used to skip first 2 readings to allow the temperature reading to
     stabilize  */
  static uint8_t skip_read=2; 

  /* Declare temporary character string */
  uint8_t lcd_buffer[] = "00.00 oC";

  /* Variable to hold the temperature value */
  float measured_temp;

  /* Disable the PGA  */
  TEMPS.TSCR.BIT.PGAEN = 0;

  /* Poll ADC unit until conversion is complete and store result in the
     global variable usADC_Result */  
  err &=  R_ADC_12_Read
      (
        0,
        PDL_NO_PTR,
        &usADC_Result
      );

  /* Formula for the temperature characteristic -
    T = (Vs – V1)/Slope + T1   */
  measured_temp = ((((float)usADC_Result)/4096.0F)*SUPPLY_V);
  measured_temp = ((measured_temp - TEMP_V1)/TEMP_SLOPE) + TEMP_T1;  

  /* Convert the measured temperature into string  */
  if((sprintf((char *)lcd_buffer, "%2.2f oC", measured_temp)) < 0)
  {
    /* Error converting the value  */
    while(1);
  }

  /* Display the temperature reading on the LCD. */
  if (skip_read == 0)
  {
    /* Display application name  */
    Display_LCD(LCD_LINE1, "Temp    ");
    
    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);
  }
  else
  {
    /* Skip first 2 readings */
    skip_read--;  
  }

  /* Re-start the ADC conversion  */
  err &=   R_ADC_12_Control
      (
        PDL_ADC_12_0_ON
      );

  /* Enable the PGA  */
  TEMPS.TSCR.BIT.PGAEN = 1;
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_Timer_TempSensor
*******************************************************************************/
