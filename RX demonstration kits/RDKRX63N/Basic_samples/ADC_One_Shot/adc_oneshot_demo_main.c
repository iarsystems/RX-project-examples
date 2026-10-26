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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : adc_oneshot_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This sample demonstrates use of the 12-bit A/D converter (S12ADC).
*                 The S12ADC is set up for one shot reading of channel AN4.
*                 AN4 is connected to the potentiometer VR1 on the YRDK RX62N board.  
*                 Each time SW1 is pressed the S12ADC is started and the reading
*                 is recorded and updated on the LCD display.
*
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Click 'Go' to start the software.
*         
*                 3. The debug LCD will show the name of the sample, 
*                    and instructions directing you to adjust pot VR1 and then 
*                    press SW1.
*
*                 4. The current ADC value in ADC counts and the counts converted
*                    to the equivalent voltage are displayed.
* 
*******************************************************************************/         
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           19.01.2011                 CS revisions. Ported to FIT format.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <intrinsics.h>
#include "S12ADC.h"
#include "lcd.h"
#include "yrdkrx63n.h"

/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/
void display_results (uint16_t adc_counts);

/*******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
*******************************************************************************/
void main(void)
{
  /* ADC reading */
  uint16_t adc_counts;
  
  uint32_t i; /* Common loop count variable. */
  
  __enable_interrupt();
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "   S12ADC   ");
  lcd_display(LCD_LINE4, "  ONE SHOT  ");
  lcd_display(LCD_LINE5, " ADJUST POT,");
  lcd_display(LCD_LINE6, " PRESS SW1  ");
  
  /* Setup the 12-bit A/D converter */
  S12ADC_init();
  
  /* 
  Open the terminal I/O window to see the output.
  */
  printf("This is the debug console\r\n");
  
  /* Make the port connected to SW1 an input */    
  PORT4.PDR.BIT.B0 = 0; 
  
  /* 
  This is the main loop.  It waits for SW1 to be pressed, then reads the 
  ADC and updates the LCD with the reading. 
  A basic spin loop delay is used to wait for the switch to settle.
  */
  while (1)
  {
    /* Wait for SW1 to be pressed */
    while (1 == PORT4.PIDR.BIT.B0)
    {
    }
    
    LED4 =LED_ON;   /* Feedback to indicate switch press was detected. */
    
    /* Start the A/D converter */ 
    S12ADC_start();
    
    /* Wait for the conversion to complete */
    while (false == S12ADC_conversion_complete())
    {
    }
    
    /* Fetch the results from the ADC */
    adc_counts = S12ADC_read();
    
    /* Show the results on the display */
    display_results(adc_counts);
    
    /* Simple delay loop to debounce the switch */
    for (i = 0; i < 1000000; i++)
    {
    }
    
    /* Wait for SW1 to be released */
    while (0 == PORT4.PIDR.BIT.B0)
    {
      /* Simple delay loop to debounce the switch */
      for (i = 0; i < 1000000; i++)
      {
      }
    }
    LED4 = LED_OFF;   /* Feedback to indicate switch was released. */
  }
} /* End function main() */


/*******************************************************************************
* Function name: display_results
* Description  : Converts ADC counts to volts and shows both on the LCD
* Arguments    : uint16_t adc_counts: current reading from ADC
* Return value : none
*******************************************************************************/
void display_results (uint16_t adc_counts)
{
  /* For converting the reading to volts */
  float adc_volts;
  /* String to display */
  char result_string[20];
  
  /* Convert to a string */
  sprintf(result_string, "Reading=%4d", adc_counts);
  
  /* Update the display */
  lcd_display(LCD_LINE7, (const uint8_t *)result_string);
  
  /* Convert S12ADC counts to volts and display */    
  adc_volts = ((float) adc_counts) / MAX_COUNTS * (VREFH - VREFL);
  
  /* Convert to a string */
  sprintf(result_string, "Volts  =%4.2f", adc_volts);
  
  /* Update the display */
  lcd_display(LCD_LINE8, (const uint8_t *)result_string);
} /* End function display_results() */


