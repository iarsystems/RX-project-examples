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
* File Name    : ADC12repeat.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines ADC repeat functions used for 12bit ADC unit.
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
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "ADC12repeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global ADC result container variable */
volatile uint16_t usADC_Result[16];
/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* 16 bit integer to character string function prototype declaration */
static void uint16_ToString(uint8_t *, uint8_t, uint16_t);
/* Timer interrupt callback function prototype declaration */
void CB_Timer_ADC12Repeat(void);

/*******************************************************************************
* Outline     : Init_ADC12Repeat
* Description   : ADC initialisation function. This function configures the ADC
*          unit for continuous scan operation, then configures a timer  
*          for a periodic interrupt every second.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_ADC12Repeat(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure the port pin P40-P43 as analog channel inputs AN000-AN0003 */
  err &= R_ADC_12_Set
      (
        PDL_ADC_12_PIN_AN000_P40 | PDL_ADC_12_PIN_AN001_P41|\
        PDL_ADC_12_PIN_AN002_P42 | PDL_ADC_12_PIN_AN003_P43
      );
  
  /* Configure the ADC in continuous scan mode */
  err &=  R_ADC_12_CreateUnit
      (
        0,
        PDL_ADC_12_SCAN_CONTINUOUS,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        PDL_NO_DATA
      );            

  /* Configure the ADC channel AN000
     Use the sampling time as 5 uS  */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        0,
        PDL_ADC_12_CH_GROUP_A,
        5E-6
      );

  /* Configure the ADC channel AN001
     Use the sampling time as 5 uS  */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        1,
        PDL_ADC_12_CH_GROUP_A,
        5E-6
      );

  /* Configure the ADC channel AN002
     Use the sampling time as 5 uS  */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        2,
        PDL_ADC_12_CH_GROUP_A,
        5E-6
      );

  /* Configure the ADC channel AN003
     Use the sampling time as 5 uS  */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        3,
        PDL_ADC_12_CH_GROUP_A,
        5E-6
      );

  /* Configure the timer to display an ADC value of each channel for 1 sec. 
     Also configure the interrupt callback function & interrupt priority  */
  err &=   R_CMT_Create
      (
        0, 
        PDL_CMT_PERIOD, 
        1, 
        CB_Timer_ADC12Repeat,
        5
      );
  
  /*  Start the ADC conversion */
  err &=   R_ADC_12_Control
      (
        PDL_ADC_12_0_ON
      );  
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function Init_ADC12Repeat
*******************************************************************************/

/*******************************************************************************
* Outline     : CB_Timer_ADC12Repeat
* Description   : Timer callback function, which is called at the end of each
*          timer period. When executed, it reads the AD conversion
*          result of the specified channel & displays on the LCD.
* Argument    : none
* Return value  : none
*******************************************************************************/
void CB_Timer_ADC12Repeat(void)
{
  /* Declare a variable to hold the channel number to be read  */
  static uint8_t ch; 
  
  /* Declare error flag */
  bool err = true;

  /* Declare temporary character string */
  uint8_t lcd_buffer[9] = "=H\'WXYZ\0";
  
  switch(ch)
  {
    /* Display "CH AN000" on the LCD  */
    case 0: Display_LCD(LCD_LINE1, "CH AN000");
        break;
        
    /* Display "CH AN001" on the LCD  */
    case 1: Display_LCD(LCD_LINE1, "CH AN001");
        break;

    /* Display "CH AN002" on the LCD  */
    case 2: Display_LCD(LCD_LINE1, "CH AN002");
        break;

    /* Display "CH AN003" on the LCD  */
    case 3: Display_LCD(LCD_LINE1, "CH AN003");
        break;
    
    /* Other than channels AN000-AN003  */
    default: 
        break;
  }
  
  /* Poll ADC unit until conversion is complete and store the result in a
     global variable 'usADC_Result' */
  err &=  R_ADC_12_Read
      (
        0,
        usADC_Result,
        PDL_NO_PTR
      );
    
  /* Convert ADC result into a character string, and store in the local
     string lcd_buffer */  
  uint16_ToString(lcd_buffer, 3, usADC_Result[ch]);
    
  /* Display the contents of the local string lcd_buffer */
  Display_LCD(LCD_LINE2, lcd_buffer);  

  /* Reset the channel number to 0 when reaches to 4 so that it will scan 
     only channels AN000 to AN003.  */
  if(++ch >= 4)
  {
    ch = 0;
  }
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_Timer_ADC12Repeat
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 16 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
static void uint16_ToString(uint8_t *output_string, uint8_t pos, 
      uint16_t input_number)
{
  /* Declare temporary character storage variable, and bit_shift variable */  
  uint8_t a = 0x00, bit_shift = 12u;
  
  /* Declare 16bit mask variable */
  uint16_t mask = 0xF000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while(bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a = (uint8_t)((input_number & mask) >> bit_shift);
    
    /* Convert the hex digit into an ASCII character, and store in output
       string */
    output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = (uint16_t) (mask >> 4u);
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/
