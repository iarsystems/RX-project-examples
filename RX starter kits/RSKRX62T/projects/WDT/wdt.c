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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : wdt.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Defines WDT functions       
******************************************************************************/
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* Compare match timer RPDL function definitions */
#include "r_pdl_cmt.h"
/* WDT RPDL function definitions */
#include "r_pdl_wdt.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "wdt.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Flash/WDT reset interrupt period global variable */
float gCMT_Period = 1E-3;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* AD conversion complete callback function prototype declaration */
void CB_ADConversion(void);
/* Watchdog timer overflow interrupt callback function prototype declaration */
void CB_WDTOverflow(void);
/* Timer period interrupt callback function prototype delcaration */
void CB_CMTPeriod(void);

/*******************************************************************************
* Outline      : Init_WDT
* Description  : This function configures the ADC unit for singleshot operation,
*         the watchdog for a 699ms timeout period, and the TMR unit to
*         create a periodic interrupt every 100ms.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_WDT(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure the ADC unit for one shot operation. After succesful AD
     conversion, the callback function CB_ADConversion is executed. */
  err &=  R_ADC_10_Create
      (
        0,
        PDL_ADC_10_MODE_SINGLE|PDL_ADC_10_CHANNELS_OPTION_0,
        PDL_ADC_10_TRIGGER_SOFTWARE,
        50E6,
        6E-7,
        CB_ADConversion,
        3
      );  
  
  /* Configure the WDT unit to timeout after 699ms, and call the function
     the callback function CB_WDTOverflow  when the timer overflows */
  err &=  R_WDT_Create
      (
        PDL_WDT_PCLK_DIV_32768,  
        CB_WDTOverflow,
        7
      );  
      
  /* Configure CMT unit to create a periodic interrupt every 100ms,
     and execute the callback function CB_CMTPeriod every period */          
  err &= R_CMT_Create(
             1,
             PDL_CMT_PERIOD,
             100E-3,    
             CB_CMTPeriod,
             5    
              );  
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_WDT
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_ADConversion
* Description  : This callback function is executed after every succesful AD
*         conversion. The function fetches the ADC result, and uses it to
*         calculate a new timer period for the LED flash/WDT reset ISR.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_ADConversion(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare temporary ADC result container variable */
  uint16_t adc_result = 0x0000;
  
  /* Fetch the result from the ADC conversion */
  err &=  R_ADC_10_Read
      (
        0,
        &adc_result
      );
  
  /* Set the timer period global variable value relative to the ADC result.
     A Timer period is the equivalence of the ADC value multiplied 
     by a factor of 0.18, added to a 10us offset */
  gCMT_Period = (float) ((adc_result * 180E-6) + 10E-6);
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);

}
/*******************************************************************************
* End of function CB_ADConversion
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTPeriod
* Description  : Timer period interrupt callback function. This function resets
*         the watchdog timer to prevent it overflowing, and toggles the 
*         user LEDs. The function also changes the time period between
*         timer interrupts, by changing the timer period to the value 
*         stored in the variable, gCMT_Period.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMTPeriod(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Reset the watchdog timer */
  err &=  R_WDT_Control
      (
        PDL_WDT_RESET_COUNTER
      );

  /* Toggle user LEDs */
  err &=  R_IO_PORT_Modify
    (
      PDL_IO_PORT_7_1|PDL_IO_PORT_7_2,
      PDL_IO_PORT_XOR,
      1
    );
  
  /* Toggle user LEDs */
  err &=  R_IO_PORT_Modify
    (
      PDL_IO_PORT_7_3,
      PDL_IO_PORT_XOR,
      1
    );
      
  /* Toggle user LEDs */    
  err &=  R_IO_PORT_Modify
    (
      PDL_IO_PORT_3_3,
      PDL_IO_PORT_XOR,
      1
    );
      
  /* Trigger AD conversion to start */
  err &=  R_ADC_10_Control
      (
        PDL_ADC_10_0_ON
      );
    
  /* Reconfigure the timer interrupt period with the value of the
     globabl variable gCMT_Period */
  err &=  R_CMT_Control
      (
        1,
        PDL_CMT_PERIOD|PDL_CMT_START,
        gCMT_Period
      );  
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_CMTPeriod
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_WDTOverflow
* Description  : This callback function is executed when the watchdog timer
*         overflows. The function sets the user LEDs ON, displays
*          "Watchdog Overflow" on the debug LCD, and waits in an infinite
*         while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_WDTOverflow(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Turn user LEDs on */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_7_1|PDL_IO_PORT_7_2,
        0
      );
  
  /* Turn user LEDs on */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_7_3,
        0
      );
      
  /* Turn user LEDs on */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_3_3,
        0
      );
  
  /* Display 'Watchdog Overflow' message on the debug LCD */
  DisplayLCD(LCD_LINE1, "Watchdog");
  DisplayLCD(LCD_LINE2, "Overflow");

  /* Halt in while loop when RPDL errors detected */  
  while(!err);    

  /* Infinite while loop */  
  while(1);
}
/*******************************************************************************
* End of function CB_WDTOverflow
*******************************************************************************/
