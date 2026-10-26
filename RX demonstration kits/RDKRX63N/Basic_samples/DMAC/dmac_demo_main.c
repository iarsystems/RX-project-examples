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
* File Name     : dmac_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This sample demonstrates the DMAC unit by performing continuous
*                 DMA transfers from a single source address, the output of the 
*                 12-bit ADC, and filling double destination buffer arrays with 
*                 the results.
*
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Open the 'Live Watch' window ('View' menu) and drag the
*                    variables 'g_dmac_dest_buf_1' and 'g_dmac_dest_buf_2'
*                    (dmac_demo.c module) to it.
*                                        
*                 3. Click the + symbols in the watch window to expand these array 
*                    variables. The contents of the DMAC transfer destination
*                    buffers can now be observed in the variable watch window when 
*                    the program runs.
*                
*                 4. Click 'Go' to start the software.
*                 
*                 5. Observe the DMAC transfer destination buffers being filled with 
*                    the ADC readings. Turn the potentiometer on the YRDK to see
*                    the values changing.
*
*******************************************************************************/
/*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*           20.01.2012                  CS revisions. Ported to FIT environment.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "lcd.h"
#include "yrdkrx63n.h"

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern void dmac_demo_start(void);

/******************************************************************************
* Function name: main
* Description  : Main program function.
*                This function first initialises the on-board LCD,
*                and displays instructions on the screen.
*                It then calls the DMAC demo application which configures and 
*                starts the DMAC transfer process. 
*                The function then waits in an infinite while loop,
*                while the DMAC demo proceeds in the background.
*                LED4 is also periodically flashed in the meantime.
*
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  __enable_interrupt();

  /* Used to pace toggling of the LED */
  uint32_t led_counter;
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display instructions on the LCD */  
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "Observe DMAC");
  lcd_display(LCD_LINE4, "destination "); 
  lcd_display(LCD_LINE5, "buffers in  ");        
  lcd_display(LCD_LINE6, "in the Live ");    
  lcd_display(LCD_LINE7, "Watch window");
  lcd_display(LCD_LINE8, "..running...");   
  
  /* 
  Open the terminal I/O window to see the output. 
  Take advantage of this for observing debug output statements.
  */
  printf("This is the debug console\r\n");
  
  /* Start the DMAC demo. The data transfer will procede as a background "task". */
  dmac_demo_start();
  
  /* This is the main loop.  
     Notice that the DMAC transfer continues even though the CPU is occupied 
     in this repeating long delay loop.
     LED4 is periodically toggled. */
  while (1)
  {
    /* The DMAC can perform it's transfer concurrently while the CPU is doing this */
    for (led_counter = 0; led_counter < 1000000; led_counter++)
    {
    }
    LED4 = ~LED4;
  }
} /* End of function main() */

/*******************************************************************************
* End of file: dmac_demo_main.c
*******************************************************************************/
