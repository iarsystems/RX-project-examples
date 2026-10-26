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
* File Name    : adc10_repeat.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Defines ADC repeat functions used for 10bit ADC unit.
*******************************************************************************/
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* CMT RPDL function defintions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "ADC10repeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global ADC result container variable */
uint16_t usADC_Result = 0x0;
/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT interrupt callback function prototype declaration */
void CB_CMTADC(void);
/* 16 bit integer to character string function prototype declaration */
void uint16_ToString(uint8_t *, uint8_t, uint16_t);

/*******************************************************************************
* Outline     : Init_ADC10Repeat
* Description   : ADC initialisation function. This function configures the ADC
*          unit for repeat operation, then configures a CMT timer for a
*          periodic interrupt every 250ms.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_ADC10Repeat(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Configure the ADC channel that is connected to the potentiometer. A
     callback function 'ADC_callback' is set to be called everytime the ADC 
     completes a conversion */
    err &=  R_ADC_10_Create
          (
            0, 
            PDL_ADC_10_CHANNELS_OPTION_1 | \
            PDL_ADC_10_MODE_CONTINUOUS_SCAN,
            PDL_ADC_10_TRIGGER_SOFTWARE,
            50E6,
            6E-7, 
            PDL_NO_FUNC, 
            7
          );
             
  /* PDL function updates the A/D value on the debug LCD every 250ms */         
  err &= R_CMT_Create
          (
            0, 
            PDL_CMT_PERIOD, 
            250E-3, 
                CB_CMTADC, 
            5
          );
          
  /* Start A/D conversion */
  err &= R_ADC_10_Control
          (
            PDL_ADC_10_0_ON
          );  
    
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function Init_ADC10Repeat
*******************************************************************************/

/*******************************************************************************


* Outline     : CB_CMTADC
* Description   : Timer callback function, which is called at the end of each
*          timer period. When executed, it initiates the AD conversion.
* Argument    : none
* Return value  : none
*******************************************************************************/
void CB_CMTADC(void)
{
  /* Declare error flag */
  bool err = true;

  /* Declare temporary character string */
  uint8_t lcd_buffer[] = "=H\'WXYZ\0";
    
  /* Poll ADC unit until conversion is complete and store result in the
     global variable usADC_Result */  
  err &=  R_ADC_10_Read
      (
        0,
        &usADC_Result
      );
    
  /* Convert ADC result into a character string, and store in the local
     string lcd_buffer */    
  uint16_ToString(lcd_buffer, 3, usADC_Result);
    
  /* Display the contents of the local string lcd_buffer */
  DisplayLCD(LCD_LINE2, lcd_buffer);  
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_CMTADC
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
void uint16_ToString(uint8_t *output_string, uint8_t pos, uint16_t input_number)
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
    output_string[pos] = (int8_t)((a < 0x0A) ? (uint8_t)((a + 0x30)) : (uint8_t)((a + 0x37)));
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = (uint16_t)(mask >> 4u);
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= (uint8_t)(4u);
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/
