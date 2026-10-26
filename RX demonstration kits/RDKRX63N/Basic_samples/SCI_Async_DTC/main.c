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
* http://www.renesas.com/disclaimer
*******************************************************************************/


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
* MTU3 is configured to toggle LED15 at 20Hz (every 50ms)
* SCI2 is configured for 115200 BAUD, 8-N-1 communication.
* The DTC is configured to handle both TXI and RXI SCI interrutps
* On reception of data, the DTC copies this data to RX.Data
* Pressing SW1 of the will transmit the received data.
* Use a terminal emulator such as TearTerm to send and receive data.
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
#include "dtc.h"
#include "mtu.h"
#include "sci.h"
#include "lcd.h"
#include "intrinsics.h"


/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);


/******************************************************************************
    Variables                                                           
******************************************************************************/ 
extern struct _Rx Rx;


/******************************************************************************
   Function     : main
   Description   : Initialises                    
                    MTU3  - 50ms interrupt                                       
                    DTC   - will handle the SCI2 TXI and RXI interrutps                    
                    SCI2 - 115200 BAUD, 8-N-1
                    while(1) - will monitor SW1 and on detection of a press
                    will transmit any received data.
   Argument    : none
   Return value : none
******************************************************************************/
void main(void)
{
  unsigned short Index;
  unsigned long DebounceCount;
  
  HardwareSetup();
  __enable_interrupt();
  
  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "SCI Async   ");  
  lcd_display(LCD_LINE2, "DTC         ");  
  lcd_display(LCD_LINE4, "Press SW1 to");  
  lcd_display(LCD_LINE5, "transmit the");
  lcd_display(LCD_LINE6, "data typed  ");  
  lcd_display(LCD_LINE7, "on the PC   ");  
  lcd_display(LCD_LINE8, "keyboard.   ");  
   
  for(Index=0; Index<256; Index++)
  {
    Rx.Data[Index] = ' ';
  }
  
  /* Initialise DTC.                                              *
   * This will 'take' the DTC ISR and transfer the AD result into *
   * AD_Buffer.                                                   *
   * The DTC is configured for normal mode, full address mode     */
  InitDTC();
  
  /* Initialise MTU3.  This timer will toggle LED15 at 50ms rate  *
   * We can use this to check that board is running and           *
   * check we have the clocks setup correctly                     */
  InitMTU3();
  
  /* Initialise SCI2                                              */
  InitSCI2(115200L, TX_ENABLE, RX_ENABLE);
  
  /* Start the timer MTU3                                         */
  StartMTU3();
  
  while(1)
  {
    while(SW1 != SW_ACTIVE);
    for(DebounceCount=0;DebounceCount<1000000; DebounceCount++);
    while(SW1 == SW_ACTIVE);
    for(DebounceCount=0;DebounceCount<1000000; DebounceCount++);
    
    WriteStringDTC(Rx.Data);                            
  }
}


