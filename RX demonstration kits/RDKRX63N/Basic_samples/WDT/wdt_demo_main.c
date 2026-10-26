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
* File Name     : wdt_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Watchdog timer demonstration project.
*                 The watchdog timer is not cleared while SW1 is pressed.
*                 Press SW1 for more than 700 mS and a processor reset will be
*                 triggered.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "switches.h"
#include "wdt.h"
#include "lcd.h"
#include "yrdkrx63n.h"


/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  /* Used to pace toggling of the LED */
  uint32_t led_counter;
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "  WDT Demo  ");
  lcd_display(LCD_LINE4, "No watchdog ");
  lcd_display(LCD_LINE5, "feeding when");
  lcd_display(LCD_LINE6, "SW1 is down.");
  
  /* 
  Open the terminal I/O window to see the output. 
  */
  printf("This is the debug console\r\n");
  
  /* The three pushbuttons on the YRDK board are tied to interrupt lines, 
  set them up here */
  switches_initialize();
  
  wdt_init(); /* initialize the watchdog timer */
  
  /* This is the main loop. */
  while (1)
  {
    char buf[13];
    sprintf(buf, "CNTVAL=%5d", WDT.WDTSR.BIT.CNTVAL);
    lcd_display(LCD_LINE8, (const uint8_t *) buf);
    
    if(PORT4.PIDR.BYTE & 1)  /* If SW1 (on P40) is not pressed, */
    {
      wdt_feed_watchdog(); /* feed the watchdog. */
    }
    
    for (led_counter = 0; led_counter < 1000000; led_counter++)
    {
    }
    LED4 = ~LED4;
  }
} /* End of function main(). */

/*******************************************************************************
End of file: wdt_demo_main.c
*******************************************************************************/
