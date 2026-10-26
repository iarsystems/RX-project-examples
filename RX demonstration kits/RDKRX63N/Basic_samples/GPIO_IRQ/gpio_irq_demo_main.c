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
* File Name     : gpio_irq_demo_main.c
* Version       : 1.0
* H/W Platform  : YRDKRX63N
* Description   : GPIO and Pin IRQ demonstration.
*                 This project demonstrates the use and setting of GPIO ports
*                 to read the state of input pins, and to control output pins
*                 In addition transitions on an input pin can be made to 
*                 generate an interrupt (IRQ). The YRDK on-board switches are
*                 set up as pin IRQs and an LED is set up to be turned on or off
*                 through the control of an output pin. 
*
* Operation     : 1. Build and download the sample code to the YRDK.
*                 2. Click 'Go' to start the software. Instructions will be
*                    displayed on the LCD. 
*                 3. Press and hold switch 1 to observe LED4 turn on and
*                    stay on.
*                 4. Release switch 1 to see LED4 turn off. Repeat as many
*                    times as desired.
*                 5. The "switch down" and "switch up" counts are displayed on
*                    the LCD.
*                 6. Press switch 2 to reset the count of how many times
*                    switch 1 has been pressed and released.               
*
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           23.01.2012                 CS revisions
*******************************************************************************/

/******************************************************************************
* Includes <System Includes>, "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <intrinsics.h>
#include "switches.h"
#include "lcd.h"

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  uint8_t a_string[16];
  
  static uint32_t last_sw1_down_count;
  static uint32_t last_sw1_up_count;
  
  __enable_interrupt();

  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "GPIO and IRQ");
  lcd_display(LCD_LINE3, "Push SW1 to ");
  lcd_display(LCD_LINE4, "light LED4  ");
  
  lcd_display(LCD_LINE6, "Push SW2 to ");
  lcd_display(LCD_LINE7, "zero counts");    
  
  /*
  Open the terminal I/O window to see the output.
  */
  printf("This is the debug console\r\n");
  
  /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
  switches_initialize();
  
  /* This is the main loop. */
  while (1)
  {
    /* If there was a change in either counter value... */
    if((last_sw1_down_count != g_sw1_down_count) || (last_sw1_up_count != g_sw1_up_count))       
    {
      /* update counts and display the counter values. */
      sprintf((char *) a_string, "Dn:%2d Up:%2d", g_sw1_down_count, g_sw1_up_count); 
      lcd_display(LCD_LINE8, a_string);
      
      last_sw1_down_count = g_sw1_down_count;  
      last_sw1_up_count   = g_sw1_up_count;
    }
  }
} /* End of function main() */

/*******************************************************************************
* End of file: gpio_irq_demo_main.c
*******************************************************************************/
