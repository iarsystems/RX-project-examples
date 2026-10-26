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
* File Name    : CMT.c
* Version      : 1.00
* Device       : RX63x
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : YRDKRX63N
* Description  : Simple driver for Compare Match Timer (CMT) peripheral
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 29.08.2011     1.00        First release
*           19.01.2012                 CS revisions. Ported to FIT environment.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <iorx63n.h>
#include "CMT.h"
#include "S12ADC.h"
#include "lcd.h"
#include "yrdkrx63n.h"

/******************************************************************************
Macro Definitions
*******************************************************************************/
/* Number of CMT ticks in 100 ms (assuming 48 MHz PCLK and clock select = PCLK / 128) */
#define TICK_INTERVAL (48000000 / 128 / 10) // 37500

/*******************************************************************************
* Function name: CMT_init
* Description  : Sets up CMT0 to generate interrupts at 100 ms
* Argument     : none
* Return value : none
*******************************************************************************/
void CMT_init (void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */

  /* Power up CMT0 */
  MSTP(CMT0) = 0;   
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */

  /* Stop the clock */
  CMT.CMSTR0.BIT.STR0 = 0;
  
  /* Trigger 100 ms from now */
  CMT0.CMCOR = CMT0.CMCNT + TICK_INTERVAL;
  
  /* CMCR - Compare Match Timer Control Register
      b6      CMIE: 1 = Compare match interrupt (CMIn) enabled
      b1:b0   CKS:  2 = Clock selects is PCLK/128 (375 kHz @ PCLK = 48 MHz) 
  */
  CMT0.CMCR.WORD = 0x0042;
  
  /* Set interrupt priority in ICU */
  IPR(CMT0, CMI0) = 0x01;
  
  /* Enable the interrupt in the ICU */
  IEN(CMT0, CMI0) = 1;
  
  /* Start the clock running */ 
  CMT.CMSTR0.BIT.STR0 = 1;
} /* End of function CMT_init() */

/*******************************************************************************
* Function name: CMT_isr
* Description  : Interrupt Service Routine for CMT match interrupt.  Reads the 
*                results register from the ADC.  The ADC is running in continuous
*                scan mode so the results register will contain the latest reading.
*                The CMT counter (CMCNT) is reset to zero when this interrupt is
*                taken so no further action is required to keep the CMT firing
*                at regular intervals.  
*
*                The processing in this ISR must take less time than the CMT 
*                compare interval.  In this example, the compare interval is
*                set to 100 ms and the processing time for the ISR takes
*                roughly 12 ms.  Use the Performance Counters in HEW to measure
*                execution of your own code.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector = VECT_CMT0_CMI0
__interrupt void CMT_isr (void)
{
  /* Latest ADC reading in counts*/
  uint16_t adc_result;
  
  /* The ADC counts converted to a voltage */
  float adc_volts;
  
  /* String to display */
  char result_string[20];
  
  /* Toggle an LED with each interrupt */
  LED4 = ~LED4;
  
  /* Read the latest results from the ADC */
  adc_result = S12ADC_read();
  
  /* Convert to a string */
  sprintf(result_string, "Reading=%4d", adc_result);
  
  /* Update the display */
  lcd_display(LCD_LINE6, (const uint8_t *)result_string);
  
  /* Convert S12ADC counts to volts and display */    
  adc_volts = ((float) adc_result) / MAX_COUNTS * (VREFH - VREFL);
  
  /* Convert to a string */
  sprintf(result_string, "Volts  =%4.2f", adc_volts);
  
  /* Update the display */
  lcd_display(LCD_LINE8, (const uint8_t *)result_string);
} /* End of CMT_isr() */
