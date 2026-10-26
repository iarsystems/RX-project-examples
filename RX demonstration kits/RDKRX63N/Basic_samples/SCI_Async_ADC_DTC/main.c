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


/*
 * main.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */

/******************************************************************************
*
*                             PROJECT OVERVIEW
*
* The project configures the RX63N to run from the PLL.
* MTU0 is configured to trigger the S12AD using the MTU trigger signal.
* The S12AD samples AN002 and the A-D End Interrupt is handled by the DTC
* When the DTC has transfered 32 results, the CPU handles the A-D End Interrupt
* In the ISR, the 32 A-D results are averaged and transmitted via SCI2
* and can be viewed in a terminal emulator such as TearTerm.
* The S12AD can be started and stopped by entering commands via the terminal
* emulator.
*
******************************************************************************/


/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "yrdkrx63n.h"
#include "yrdkrx62n_rspi_api.h"
#include "main.h"
#include "mtu.h"                 
#include "sci.h"
#include "dtc.h"
#include "s12ad.h"
#include "lcd.h"
#include "intrinsics.h"


/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);


/******************************************************************************
   Function     : main
   Description   : Initialises                    
                    MTU3  - 50ms interrupt
                    MTU0  - 5ms interrupt                    
                    DTC   - will handle the S12AD interrupt
                    S12AD - AN002 channel, triggered by MTU0
                    SCI2 - 115200 BAUD, 8-N-1
                    while(1) - all processing is DTC and ISR driven
   Argument    : none
   Return value : none
******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();
  
  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  lcd_display(LCD_LINE1, "SCI Async   ");
  lcd_display(LCD_LINE2, "ADC DTC     ");

  /* Initialise MTU3.  This timer will toggle LED15 at 50ms rate   *
   * We can use this to check that board is running and            *
   * check we have the clocks setup correctly                      */
  InitMTU3();
  
  /* Initialise MTU0.                                              *
   * This timer will trigger the ADC every 5ms.                    *
   * The ADC result will be transferred into the array AD_Buffer[] *
   * by the DTC                                                    */
  InitMTU0();
  
  /* Initialise DTC.
   * This will 'take' the DTC ISR and transfer the AD result into  *
   * AD_Buffer.                                                    *
   * The DTC is configured for normal mode, full address mode      */
  InitDTC();
  
  /* Initialise S12AD Unit 0, Channel 0                            */
  InitS12ADSingleMode();
  
  /* Initialise SCI2                                               */
  InitSCI2(115200L, TX_ENABLE, RX_ENABLE);
  
  /* Start the timer MTU3                                          */
  StartMTU3();
  
  WriteString("\n\r");
  WriteString("Press 'g' or 'G' to START the ADC process.\n\r\n\r");
  WriteString("Press 's' or 'S' to STOP the ADC process.\n\r\n\r");
  
  while(1) 
  {
  }
}
