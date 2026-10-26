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
* File Name    : mtu_pwm_demo_main.c
* Version      : 1.0 
* Device(s)    : RX600 family
* Tool-Chain   : IDE, Compiler (inc. version)
* H/W Platform : YRDKRX63N
* Description  : This is the main entry point for the MTU PWM mode1 demo.
*
* Operation    : 1. Compile and download the sample code. Click 'Go'
*                   to start the software.   
*                2. Observe LEDS alternately brightening and dimming by means
*                   of PWM switching. The PWM duty cycle is alternately ramped
*                   up then down continually.
*                3. Observe PWM waveform at header JN2 pin 23 with oscilloscope.
*                4. LED9 is switched directly by the PWM output pin from the
*                   MTU. The remaining LEDs are all switched by port command
*                   during the MTU compare-match interrupt. Note that the duty
*                   cycle has been intentionally inverted between LED9 and the
*                   rest so that it is easily distinguished from the others.
*
* note:        : Demo runs continuously.
*******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 18.01.2012 1.00     First Release
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "mtu_pwm_mode1_demo.h"
#include "lcd.h"

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  __enable_interrupt();
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, " YRDKRX63N  ");
  lcd_display(LCD_LINE2, "MTU PWM Demo");
  lcd_display(LCD_LINE3, " MTIOC4A on ");
  lcd_display(LCD_LINE4, "output PE2, ");    
  lcd_display(LCD_LINE5, "JN2:23, LED9");
  lcd_display(LCD_LINE6, " Other LEDs ");
  lcd_display(LCD_LINE7, " switched in");
  lcd_display(LCD_LINE8, " MTU ISR.    ");
  
  /* Open the terminal I/O window to see the output. */
  printf("This is the debug console\r\n");
  
  mtu_initialize();   /* Set up the MTU. */
  
  mtu_start();        /* Start the PWM demo.*/
  
  /* Do nothing in main loop. Demo is performed during MTU interrupts.  */
  while (1)
  {
  }
} /* End of function main() */

/*******************************************************************************
* End of file: mtu_pwm_demo_main.c
*******************************************************************************/
