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
* File Name    : rtc.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines RTC controlling functions.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* RTC RPDL function definitions */
#include "r_pdl_rtc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "rtc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Variable used to store the string to be displayed on the LCD */
uint8_t lcd_buffer[9];
/* Variable used to hold current time */
uint32_t time_data = 0x0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Periodic interrupt callback function prototype */
void CB_1HZ_RTC(void);
/* RTC alarm callback function prototype */
void CB_Alarm_RTC(void);
/* Integer to string function prototype declaration */
static void uint32_ToBCDString(uint8_t *, uint8_t, uint32_t);

/*******************************************************************************
* Outline     : Init_RTC
* Description   : The RTC initialisation function. This function presets the 
*          time & date of the RTC. It configures the RTC to generate 
*          an interrupt every second. The alarm is also configured to 
*          trigger at a specified time.  
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_RTC(void)
{  
  /* Declare error flag */
  bool err = true;

  /* Configure the clock as follows - 
    Initial time - 11:59:30 
    Initial date - 01/01/2011
    Interrupt - every 1 second
    Alarm time - 12:00:00
    Alarm date - 01/01/2011
    callback function for RTC Alarm - CB_Alarm_RTC
    callback function for RTC periodic function - CB_Update_LCD  */  
  err &=  R_RTC_Create
      (
        PDL_RTC_SUBCLOCK_DRIVE_LOW|PDL_RTC_ALARM_HOUR_ENABLE |\
        PDL_RTC_ALARM_MINUTE_ENABLE | \
        PDL_RTC_ALARM_SECOND_ENABLE |\
        PDL_RTC_OUTPUT_DISABLE,
        PDL_NO_DATA,
        0xFF115930,  
        0x20110101,  
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,  
        PDL_RTC_PERIODIC_1_HZ, 
        0x00120000,  
        0x20110101,  
        CB_Alarm_RTC, 
        6,         
        CB_1HZ_RTC, 
        7      
      );

  /* Start the clock */
  err &=   R_RTC_Control
      (
        PDL_RTC_ALARM_TIME_ENABLE | PDL_RTC_ALARM_DATE_DISABLE|\
        PDL_RTC_CLOCK_START,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA
      );

  /* Halt in while loop when RPDL errors detected */
  while(!err);  
}
/*******************************************************************************
* End of function Init_RTC
*******************************************************************************/

/*******************************************************************************
* Outline     : CB_Alarm_RTC
* Description   : Callback function to the RTC alarm interrupt, generated when 
*          the time set in the alarm registers matches with the current
*          RTC time.
* Argument    : none
* Return value  : none
*******************************************************************************/
void CB_Alarm_RTC(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Turn on LED1 to indicate that the alarm is triggered  */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_5,
        0
      );

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_Alarm_RTC
*******************************************************************************/

/*******************************************************************************
* Outline     : CB_1HZ_RTC
* Description   : Callback function to the periodic interrupt generated every 
*          1 sec. It is used to update the time on the debug LCD.
* Argument    : none
* Return value  : none
*******************************************************************************/
void CB_1HZ_RTC(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare a variable for storing RTC status */
  uint8_t rtc_status = 0x0;
    
  /* Read the time and status flags */
  err &=  R_RTC_Read
      (
        PDL_RTC_READ_CURRENT,
        &rtc_status,
        &time_data,
        PDL_NO_PTR
      );

  /* Skip processing the day of the week */
  time_data &= 0x00FFFFFF;
  
  /* Check if the periodic interrupt flag was raised and if the clock is
     running in normal operation */
  if(rtc_status & 0x01)
  {
    /* Convert the data to string & display on the LCD */
    uint32_ToBCDString(lcd_buffer, 0, time_data);
    
    /* Update time on the debug LCD */
    Display_LCD(LCD_LINE2, lcd_buffer);
  }
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_1HZ_RTC
*******************************************************************************/

/*******************************************************************************
* Outline      : uint32_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*         which is inserted into the character array passed as an 
*         argument.
* Argument     : uint8_t * string  - Pointer to array to store BCD string 
*         uint8_t pos    - Array element to start BCD string from.
*         uint32_t number  - 32bit integer to convert.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
static void uint32_ToBCDString(uint8_t * output_string, uint8_t pos, uint32_t number)
{
  /* Local digit position variable */
  uint8_t disp_ptr=0,digit = 6u;
  
  /* Local string buffer variable */
  uint8_t buffer_string[6] = "000000";
  
  /* Decrement digit each iteration, until digit is zero */
  while(digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] = (uint8_t) (0x30 + (number % 16));
    
    /* Move to the next decimal digit */
    number >>= 4;
  }
  
  /* Set digit position to zero */
  digit = 0u;
  
  /* Loop through each digit, and replace leading zeros with a space
     character */
  while(disp_ptr < 6u)
  {
    /* Copy digit from buffer string to output string */
    output_string[digit + pos] = buffer_string[disp_ptr];
      
    /* Increment the loop counter */
    digit++;
    disp_ptr++;
    
    /* Add colon between hours & minutes and between minutes & seconds. */
    if((digit == 5)||(digit == 2))
    {
      output_string[digit + pos] = ':';  

      /* Increment the loop counter */
      digit++;
    }
  }
}
/*******************************************************************************
* End of function uint32_ToBCDString
*******************************************************************************/
