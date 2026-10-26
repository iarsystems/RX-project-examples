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
* File Name     : dtc_demo_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This sample demonstrates the DTC unit; by utilizing the DTC
*                 to transfer an ADC conversion to a 16 element array.
*       
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Open the 'Live Watch' window ('View' menu) and drag the
*                    variable 'g_dtc_destination' (dtc_demo.c module) to it.
*                    Click the + symbol in the watch window to expand the
*                    array variable.
*
*                 3. The contents of the variable 'g_dtc_destination' can be
*                    viewed in the watch window.
*
*                 4. Click 'Go' to start the software. Instructions will be
*                    displayed on the debug LCD.
*
*                 5. Adjust the potentiometer, VR1, and press the switch SW1.
*                    The result from the ADC conversion will be transferred
*                    into the 16 elements of the transfer destination array at
*                    1/2 second  intervals using a timer interrupt mapped to
*                    the DTC.
*
*                 6. While the example is taking samples, the VR1 pot can be
*                    turned to vary the ADC result. 
*
*                 7. Once the last array element has been transferred, the next 
*                    interrupt from the timer will stop the timer.
*                
*                 8. Repress the switch SW1 to repeat the process. 
*                    
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           23.01.2012                 CS revisions
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "switches.h"
#include "dtc_demo.h"
#include "lcd.h"
#include "yrdkrx63n.h"

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern volatile bool g_sw1_pressed;


/******************************************************************************
* Function name: main
* Description  : Main program function.
*                This function first initialises the on-board LCD,
*                and displays instructions on the screen.
*                The board switches are initialized so that switch1 can be used.
*                The function then waits in an infinite while loop,
*                polling switch1 which indicates that the DTC demo should be started.
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
  lcd_display(LCD_LINE3, "  DTC Demo  ");
  lcd_display(LCD_LINE4, "Press SW1 to");
  lcd_display(LCD_LINE5, "Start. See  ");
  lcd_display(LCD_LINE6, "ADC data in ");
  lcd_display(LCD_LINE7, "Live Watch. "); 
  
  /*
  Open the terminal I/O window to see the output .
  Take advantage of this for observing debug output statements.
  */
  printf("This is the debug console\r\n");
  
  /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
  switches_initialize();
  
  /* This is the main loop.  
    It polls for switch 1 being pressed and starts the DTC demo. 
    LED4 is periodically toggled. */
  while (1)
  {
    if (g_sw1_pressed)
    {
      g_sw1_pressed = false;    
      DtcDemoStart();  
    }
    
    /* The DTC can perform it's transfer concurrently while the CPU is doing this */
    for (led_counter = 0; led_counter < 1000000; led_counter++)
    {
    }
    LED4 = ~LED4;
  }
} /* End of function main() */

/*******************************************************************************
End of file: dtc_demo_main.c
*******************************************************************************/
