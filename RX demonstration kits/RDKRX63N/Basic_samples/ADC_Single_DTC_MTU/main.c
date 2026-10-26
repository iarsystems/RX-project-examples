/* Adapted for use with IAR Embedded Workbench */


/******************************************************************************
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
* http://www.renesas.com/disclaimer
******************************************************************************/

/******************************************************************************
*
*                             PROJECT OVERVIEW
*
* This project shows how to configure the S12AD peripheral in single-cycle scan 
* mode (S12AD), using the MTU to trigger the A-D Conversion and use the 
* DTC to handle the S12AD ISR to transfer the result from the result register to 
* RAM.  The DTC is configured for repeat mode, with a repeat size of 32.
* When 32 transfers are made the DTC reconfigures itself and starts the transfer
* process again.
* Therefore, the interrupt will never be sent to the CPU as the DTC handles it
* all of the time
*
* The project configures the Clock Generation Circuit to run from the PLL, via
* the external 12MHz XTAL. 
* The Clock Generation Circuit can be configured by the settings in cgc.h
* 
* When running, LED15 will toggle at 20Hz (every 50ms).
* The rate can be measured to verify the settings of the Clock Generation Circuit.
* 
* The A-D conversion is triggered by MTU0 every 5ms
*
******************************************************************************/


/*
 * main.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */


/******************************************************************************
System Includes                                                           
******************************************************************************/
#include <iorx63n.h>
#include <stdio.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "yrdkrx63n.h"
#include "yrdkrx62n_rspi_api.h"
#include "main.h"
#include "mtu.h"
#include "s12ad.h"  
#include "dtc.h"
#include "lcd.h"
#include "intrinsics.h"


/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);


/******************************************************************************
    Variables
******************************************************************************/
extern unsigned short S12AD_Buffer[];


/******************************************************************************
   Function     : main
   Description   : Initialises S12AD and MTU2_3 peripherals.
                  Starts the MTU_3 running.
                  Waits in a while(1) loop.
   Argument    : none
   Return value : none
******************************************************************************/
void main(void)
{
  /* Width of LCD display plus null termination */
  char dis_buf[13] = {0};

  HardwareSetup();
  __enable_interrupt();
  
  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  lcd_display(LCD_LINE1, "ADC Single  ");
  lcd_display(LCD_LINE2, "DTC MTU     ");
  lcd_display(LCD_LINE4, "ADC result: ");

  /* Initialise DTC.                                                       */
  /* This will 'take' the ADC ISR and transfer the AD result into          */
  /* AD_Buffer.                                                            */
  /* The DTC is configured for repeat mode, full address mode              */
  InitDTC();
  
  /* Initialise MTU3.                                                      */
  /* This timer will toggle LED15 at 50ms rate                              */  
  /* We can use this to check that board is running and                    */
  /* check we have the clocks setup correctly                              */
  InitMTU3();
  
  /* Initialise MTU0.                                                      */
  /* This timer will trigger the ADC every 5ms.                            */
  /* The ADC result will be transferred into the array AD_Buffer[]         */
  /* by the DTC                                                            */
  InitMTU0();
  
  /* Initialise ADC Unit 0, Channel 0                                      */
  InitS12ADSingleModeMTU();
  
  /* Start the timer MTU3                                                  */
  StartMTU3();
  
  /* Start the timer MTU0                                                  */
  StartMTU0();
  
  while(1)
  {
    sprintf(dis_buf, "%.4Xh  %.4Xh", S12AD_Buffer[0], S12AD_Buffer[1]);
    lcd_display(LCD_LINE5, (const uint8_t *)dis_buf);    
    sprintf(dis_buf, "%.4Xh  %.4Xh", S12AD_Buffer[2], S12AD_Buffer[3]);
    lcd_display(LCD_LINE6, (const uint8_t *)dis_buf);    
    sprintf(dis_buf, "%.4Xh  %.4Xh", S12AD_Buffer[4], S12AD_Buffer[5]);
    lcd_display(LCD_LINE7, (const uint8_t *)dis_buf);    
    sprintf(dis_buf, "%.4Xh  %.4Xh", S12AD_Buffer[6], S12AD_Buffer[7]);
    lcd_display(LCD_LINE8, (const uint8_t *)dis_buf);    
  }
}
