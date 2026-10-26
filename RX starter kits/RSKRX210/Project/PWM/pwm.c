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
* File Name      : pwm.c
* Version        : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : Defines timer functions used to generate a PWM waveform.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "pwm.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* 16 bit integer to BCD string function prototype declaration */
static void uint16_ToBCDString(uint8_t *, uint8_t, uint16_t);
/* Timer interrupt period callback function prototype declaration */
void CB_Timer_PWM(void);
/* Switch interrupt callback function prototype declaration */
void CB_Switch_PWM(void);

/*******************************************************************************
* Global Variables 
*******************************************************************************/
/* Duty cycle container global variable, initialised to 10% */
volatile uint8_t gDutyCycle = 10u;
/* PWM varying duty cycle on/off boolean global variable */
volatile bool gVaryDutyCycle = true;
/* Global switch press variable */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline      : Init_PWM
* Description  : This function initialises a timer unit to output a 1 KHz square
*         wave, and configures a callback function to modify the duty  
*         cycle from 10% to 90%.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_PWM(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure the TMO2 pin for PWM output */
  err &=   R_TMR_Set
      (
        PDL_TMR_TMR2_TMO2_P16
      );
  
  /* Configure the timer module to toggle the TMO2 pin at 1KHz, and execute 
     the callback function CB_Timer_PWM every period. The Timer 2 & Timer 3 
     are used to form a 16 bit timer  */
  err &=  R_TMR_CreatePeriodic
      (
        PDL_TMR_UNIT1,
        PDL_TMR_FREQUENCY|PDL_TMR_OUTPUT_HIGH,
        1E3,
        (double)gDutyCycle,
        PDL_NO_FUNC,
        CB_Timer_PWM,
        3
      );
      
  /* Configure switch callback function */
  SetSwitchPressCallback(CB_Switch_PWM);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_PWM
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Timer_PWM
* Description  : This callback function is executed every period of the timer
*         unit, UNIT1. The function's main code runs every 25th time the
*         function is called. The main code increments the duty cycle 
*         from 10% to 90%, and loops back around to 10% again. If the 
*         'gVaryDutyCycle' variable is false, the function does nothing.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Timer_PWM(void)
{
  /* Declare error flag */
  bool err = true;
  /* ISR count global variable */
  static uint8_t gISR_Count = 25u;
  
  /* Decrement the ISR counter by 1, and enter if-loop if the variable is zero
     and the gVaryDutyCycle variable is true */
  if(!(--gISR_Count) & gVaryDutyCycle)
  {
    /* Increment the duty cycle variable, and reset back to 10% if value is
       greater than 90% */
    if(++gDutyCycle > 90u)
    {
      /* Reset the duty cycle back to 10% */
      gDutyCycle = 10u;
    }
    
    /* Re-configure the timer unit with the new duty cycle */
    err &=  R_TMR_ControlPeriodic
        (
          PDL_TMR_UNIT1,
          PDL_TMR_FREQUENCY|PDL_TMR_START,
          1E3,
          (double)gDutyCycle
        );  
    
    /* Reset the ISR count value back to 25 */
    gISR_Count = 25u;
  }
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_Timer_PWM
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch_PWM
* Description  : This function is called any time a user switch is pressed. If
*         switch SW1 is pressed, the function sets the gVaryDutyCycle
*         variable to 'false', which then stops the PWM duty cycle from
*         varying. The function also displays the value of the duty cycle
*         on the debug LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Switch_PWM(void)
{
  /* Check if switch SW1 has been pressed */
  if(gSwitchFlag & SWITCHHOLD_1)
  {
    /* Local duty cycle container variable */
    uint16_t duty_cycle = (uint16_t)gDutyCycle;
    
    /* Local LCD buffer string */
    uint8_t lcd_buffer[] = "D.C: XY%";
    
    /* Set the vary-duty-cycle flag to false */
    gVaryDutyCycle = false;
    
    /* Convert the duty cycle value into a BCD string, inserted
       into the string lcd_buffer */
    uint16_ToBCDString(lcd_buffer, 2u, duty_cycle);
    
    /* Display the contents of lcd_buffer onto the debug LCD */
    Display_LCD(LCD_LINE2, lcd_buffer);  
  }
  
  /* Clear the switch press flags */
  gSwitchFlag &= 0x0F;
}
/*******************************************************************************
* End of function CB_Switch_PWM
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*         which is inserted into the character array passed as an 
*         argument.
* Argument     : uint8_t * string  - Pointer to array to store BCD string 
*         uint8_t pos    - Array element to start BCD string from.
*         uint16_t number  - 16bit integer to convert.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
static void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, 
      uint16_t number)
{
  /* Local digit position variable */
  uint8_t digit = 5u;
  
  /* Digit is a leading zero indicator variable */
  bool leading_zero = true;
  
  /* Local string buffer variable */
  uint8_t buffer_string[5] = "00000";
  
  /* Decrement digit each iteration, until digit is zero */
  while(digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] =(uint8_t) (0x30 + (number % 10));
    
    /* Move to the next decimal digit */
    number /= 10u;
  }
  
  /* Set digit position to zero */
  digit = 0u;
  
  /* Loop through each digit, and replace leading zeros with a space
     character */
  while(digit < 5u)
  {
    /* Check if digit is not a zero character, or if leading_zero indicator is
       false */
    if((buffer_string[digit] != 0x30) | (!leading_zero))
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
