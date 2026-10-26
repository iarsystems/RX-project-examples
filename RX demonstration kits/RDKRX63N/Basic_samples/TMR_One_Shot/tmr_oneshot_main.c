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
* File Name     : tmr_oneshot_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* H/W Platform  : YRDKRX63N
* Description   : 8-bit timer (TMR) demonstration
*                 Press SW1 to start a one-second delay. 
*******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           23.01.2012                 Ported to FIT workspace. CS revisions.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include <iorx63n.h>
#include "r_switches.h"
#include "tmr_code.h"
#include "lcd.h"
#include "yrdkrx63n.h"

/*******************************************************************************
* Funtion prototypes
*******************************************************************************/
void sw1_callback(void);
void sw2_callback(void);
void sw3_callback(void);

void mtu_callback(void);

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
  uint16_t timer;
  static uint16_t last_timer;
  
  last_timer = TMR01.TCCR;
  
  __enable_interrupt();
  
  /* Initialize LCD */
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE3, "TMR One-Shot");
  lcd_display(LCD_LINE4, "1 sec delay ");
  lcd_display(LCD_LINE5, " Press SW1  ");
  lcd_display(LCD_LINE6, " JN2 pin 21 ");
  
  /* 
  Open the terminal I/O window to see the output.
  */
  printf("This is the debug console\r\n");
  
  /* The three pushbuttons on the YRDK board are tied to interrupt lines, 
  set them up here */
  R_SWITCHES_Init();
  
  /* This is the main loop.  */
  while (1)
  {
    uint8_t buf[13];
    sprintf((char *)buf, "TCNT=%7d", TMR01.TCNT);
    lcd_display(LCD_LINE7, buf);
    
    timer = TMR01.TCCR;
    if (last_timer != timer) /* If the timer status changed, */
    {
      last_timer  = timer;
      if (timer != 0) /* If the timer went on */
      {
        lcd_display(LCD_LINE8, "Delaying ...");
      }
      else                       /* If the timer went off */
      {
        lcd_display(LCD_LINE8, "            "); /* Clear the delaying message */
      }
    }
    
    for (led_counter = 0; led_counter < 1000000; led_counter++)
    {
    }
    LED4 = ~LED4;
  }
} /* End of function main() */


/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{
  IEN(ICU, IRQ8)  = 0;    /* turn off this interrupt (start of debounce) */
  
  PORTD.PODR.BYTE = 0x18; /* Light up the red LEDs */
  PORTE.PODR.BYTE = 0xFE;
  
  tmr_start();            /* start the 8-bit timer */
} /* End of function sw1_callback() */


/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
  __no_operation(); /* Add your code here. Note: this is executing inside an ISR. */
} /* End of function sw2_callback() */


/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback(void)
{
  __no_operation(); /* Add your code here. Note: this is executing inside an ISR. */
} /* End of function sw3_callback() */


/******************************************************************************
* Function name: tmr_callback
* Description  : Callback function that is executed when the tmr count match
*                triggers its interrupt. Called by tmr_isr in mtu_normal_mode.c
* Arguments    : none
* Return value : none
******************************************************************************/
void tmr_callback(void)
{
  PORTD.PODR.BYTE = ~0x18; /* Light up the green LED's */
  PORTE.PODR.BYTE =  0xF1;
  
  IR (ICU, IRQ8)  = 0;     /* Clear any pending SW1 interrupt */
  IEN(ICU, IRQ8)  = 1;     /* turn the SW1 interrupt back on (debounce complete) */
} /* End of function mtu_callback() */


/*******************************************************************************
* End of file: main.c
*******************************************************************************/
