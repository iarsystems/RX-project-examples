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
* File Name     : timer_capture.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines timer capture functions used in this project.
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "timer_capture.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Switch press flag global variable */
extern volatile uint8_t gSwitchFlag;
/* Global timer count container variable */  
volatile uint16_t gTimerCount = 0x0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Clock tick timer callback function prototype declaration */
void CB_TimerClockTick(void);
/* Integer to BCD string function prototype declaration */
void uint16_ToBCDString(uint8_t*, uint8_t, uint16_t);
/* Switch press callback function prototype declaration */
void CB_SwitchPress(void);
/* Switch release callback function prototype declaration */
void CB_SwitchRelease(void);
/* Update debug LCD function prototype declaration */
void Update_TimerCapture(void);

/*******************************************************************************
* Outline       : Init_TimerCapture
* Description   : This function initialises a CMT timer to run at 1KHz, and call
*                 the callback function CB_TimerClockTick. The function then
*                 configures the callback functions for the switch press and 
*                 release interrupts.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TimerCapture(void)
{
  /* Declare error flag */
  bool err = true;

  /* Timer is configure */
  err &=  R_CMT_Create
    (
    2,
    PDL_CMT_FREQUENCY,
    1000,
    CB_TimerClockTick,
    1
    );

  /* Stop the CMT timer from running */
  err &=  R_CMT_Control
    (
    2,
    PDL_CMT_STOP,
    0
    );  

  /* Configure the callback function from the switch press interrupt */
  SetSwitchPressCallback(CB_SwitchPress);

  /* Configure the callback function from the switch release interrupt */
  SetSwitchReleaseCallback(CB_SwitchRelease);

  /* Halt in while loop when RPDL errors detected */  
  while (!err);
}
/*******************************************************************************
* End of function Init_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_TimerClockTick
* Description   : This function is called every millisecond by the CMT period
*                 interrupt. The function increments the gTimerCount variable
*                 each time it is called, which acts as a clock tick pulse used 
*                 to count the switch press duration.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_TimerClockTick(void)
{
  /* Increment the timer count value */
  gTimerCount++;
}
/*******************************************************************************
* End of function CB_TimerClockTick
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_SwitchPress
* Description   : Switch press callback function. This function resets the value
*                 of gTimerCount, and starts the CMT timer if switch SW1 has
*                 been pressed.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_SwitchPress(void)
{
  /* Check if switch SW1 has been pressed */
  if (gSwitchFlag & SWITCHHOLD_1)
  {
    /* Declare error flag */
    bool err = true;

    /* Reset timer value */
    gTimerCount = 0x0;

    /* Start CMT timer */
    err &=  R_CMT_Control
      (
      2,
      PDL_CMT_START,
      0
      );

    /* Halt in while loop when RPDL errors detected */  
    while (!err);
  }
}
/*******************************************************************************
* End of function CB_SwitchPress
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_SwitchRelease
* Description   : Switch release callback function. This function stops the CMT
*                 timer, and calls the Update_TimerCapture function if switch SW1
*                 has been released.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_SwitchRelease(void)
{
  /* Declare error flag */
  bool err = true;

  /* Check if switch SW1 has been released */
  if (gSwitchFlag & SWITCHPRESS_1)
  {
    /* Stop the CMT timer */
    err &=  R_CMT_Control
      (
      2, 
      PDL_CMT_STOP,
      0
      );

    /* Call the update function */    
    Update_TimerCapture();
  }

  /* Reset the switch flag */
  gSwitchFlag &= 0x0F;

  /* Halt in while loop when RPDL errors detected */
  while (!err);
}
/*******************************************************************************
* End of function CB_SwitchRelease
*******************************************************************************/

/*******************************************************************************
* Outline       : Update_TimerCapture
* Description   : This function converts the timer count value into a BCD string,
*                 and writes it to the debug LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Update_TimerCapture(void)
{
  /* Declare temporary lcd buffer string */
  uint8_t lcd_buffer[] = "      ms";

  /* Check if the count value is greater than 9999ms */
  if (gTimerCount > 9999u)
  {
    /* Display that the count value is greater than 10 seconds on the debug
    LCD screen */
    DisplayLCD(LCD_LINE4, " >10 sec");
  }
  /* Count value is less than 9999ms */
  else
  {
    /* Convert the timer value into a BCD character string, and insert into 
    the temporary string lcd_buffer */
    uint16_ToBCDString(lcd_buffer, 1u, gTimerCount);

    /* Display the contents of lcd_buffer onto the debug LCD */
    DisplayLCD(LCD_LINE4, lcd_buffer);
  }
}
/*******************************************************************************
* End of function Update_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline       : uint16_ToBCDString
* Description   : This function converts an 16bit integer into a BCD string,
*                 which is inserted into the character array passed as an 
*                 argument.
* Argument      : uint8_t * string  - Pointer to array to store BCD string 
*                 uint8_t pos       - Array element to start BCD string from.
*                 uint16_t number   - 16bit integer to convert.
* Return value  : none
* Note          : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, uint16_t number)
{
  /* Local digit position variable */
  uint8_t digit = 5u;

  /* Digit is a leading zero indicator variable */
  bool leading_zero = true;

  /* Local string buffer variable */
  uint8_t buffer_string[] = "00000";

  /* Decrement digit each iteration, until digit is zero */
  while (digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] = 0x30 + (number % 10);

    /* Move to the next decimal digit */
    number /= 10u;
  }

  /* Set digit position to zero */
  digit = 0u;

  /* Loop through each digit, and replace leading zeros with a space
  character */
  while (digit < 5u)
  {
    /* Check if digit is not a zero character, or if leading_zero indicator 
    is false */
    if ((buffer_string[digit] != 0x30) | (!leading_zero))
    {
      /* Copy digit from buffer string to output string */
      output_string[digit + pos] = buffer_string[digit];

      /* Set the leading zero indicator variable to false */
      leading_zero = false;  
    }

    /* Increment the loop counter */
    digit++;
  }
}
/*******************************************************************************
* End of function uint16_ToBCDString
*******************************************************************************/
