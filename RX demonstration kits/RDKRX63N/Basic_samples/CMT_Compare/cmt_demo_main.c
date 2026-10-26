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
/******************************************************************************
* File Name     : cmt_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : CMT demo project
*
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Click 'Go' to start the software.
*         
*                 3. The debug LCD will show the name of the sample and a 
*                    description of the project.
*
*                 4. The LED's are toggled every 100ms by the Compare Match 
*                    Timer.  CMT channel 0 is set to generate an interrupt
*                    every 100 ms.  During the interrupt the LED's are toggled.
*                    The CMT is a great choice for this type of periodic task
*                    because is automatically clears and re-arms itself, so 
*                    no further servicing of the CMT is necessary.  This makes
*                    it suitable for tasks like generating the base tick for 
*                    a real-time operating system (RTOS) or pacing a simple
*                    scheduler.
*
*                 5. Place a scope probe at connector JN2 pin 16 to see the 
*                    signal toggle with each compare match.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           19.01.2012                 CS revisions. Ported to FIT environment.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "CMT.h"
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
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "  Compare   ");
  lcd_display(LCD_LINE4, "Match Timer:");
  lcd_display(LCD_LINE5, "CMT0 @ 100mS");
  lcd_display(LCD_LINE6, "toggles LEDs");
  lcd_display(LCD_LINE7, "Probe signal");
  lcd_display(LCD_LINE8, " JN2 pin 16 ");
  
  /* 
  Open the terminal I/O window to see the output. 
  */
  printf("This is the debug console\r\n");
  
  /* Initialize and start the Compare Match Timer */    
  CMT_init();
  
  /* This is the main loop.  It does nothing; all processing is done in the CMT ISR (see CMT.c) */
  while (1)
  {
  }
  
} /* End of function main() */

