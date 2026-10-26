/* Adapted for use with IAR Embedded Workbench */

/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/


/******************************************************************************
*
*                             PROJECT OVERVIEW
*
* The project shows how to configure some basic functionality.
* Some text is written to the LCD display, a LED is flashing and some empty
* interrupt functions are provided.
* 
* In order to trigger the bus error interrupt, open the 'Memory' window from
* the 'View' menu while running the debugger and write 0x10000000 in the
* 'Go to' input field.
******************************************************************************/


/******************************************************************************
* Project Includes
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <iorx63n.h>
#include "yrdkrx63n.h"
#include "yrdkrx62n_rspi_api.h"
#include "lcd.h"
#include "switches.h"
#include "hwsetup.h"
#include "intrinsics.h"


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
  
  hardware_setup();
  __enable_interrupt();

  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  
  /* Open the terminal I/O window to see the output */
  printf("This is the debug console\r\n");
  
  /* The three pushbuttons on the YRDK board are tied to interrupt lines,
     set them up here */
  switches_initialize();

  /* This is the main loop. It does nothing but toggle LED4 periodically */
  while (1)
  {
    for (led_counter = 0; led_counter < 1000000; led_counter++)
    {
    }
    LED4 = ~LED4;
  }
}
