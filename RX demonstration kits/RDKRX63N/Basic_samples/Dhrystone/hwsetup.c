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
 * hwsetup.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */


/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "yrdkrx63n.h"


/******************************************************************************
   Function     : HardwareSetup
   Description   : Enables access to the MPC registers
                  Calls InitCGC() to initialise the Clock Generation Circuit
                  Configures the I/O pins connected to the LEDs as outputs                  
   Argument    : none
   Return value : none
******************************************************************************/
void HardwareSetup(void)
{        
  /* Gain access to the Port Function Select Registers                     */
  /* Refer to section 21 of the Hardware User Manual for further details   */
  MPC.PWPR.BIT.B0WI = 0;
  MPC.PWPR.BIT.PFSWE = 1;
  
  /* Configure LED - I/O pins as Outputs                                   */
  /* First set the Data Levels                                             */
  LED4 = LED_OFF;	            
  LED5 = LED_OFF;	            
  LED6 = LED_OFF;	            
  LED7 = LED_OFF;	            
  LED8 = LED_OFF;	            
  LED9 = LED_OFF;	            
  LED10 = LED_OFF;	            
  LED11 = LED_OFF;	            
  LED12 = LED_OFF;	            
  LED13 = LED_OFF;	            
  LED14 = LED_OFF;	            
  LED15 = LED_OFF;	            
  
  /* Set Port Direction Registers for LED                                  */
  LED4_PDR = OUTPUT;	    
  LED5_PDR = OUTPUT;	    
  LED6_PDR = OUTPUT;	    
  LED7_PDR = OUTPUT;	    
  LED8_PDR = OUTPUT;	    
  LED9_PDR = OUTPUT;	    
  LED10_PDR = OUTPUT;	    
  LED11_PDR = OUTPUT;	    
  LED12_PDR = OUTPUT;	    
  LED13_PDR = OUTPUT;	    
  LED14_PDR = OUTPUT;	    
  LED15_PDR = OUTPUT;
  
  /* Port 5 - LCD */
  PORT5.PODR.BYTE = 0x00 ;    /* All outputs low to start */
  PORT5.PDR.BYTE  = 0x13 ;    /* SCI 2 TxD, LCD_RS, PWMLP_OUT are outputs */

  /* Port C -  SPI signals, chip selects, peripheral reset */
  PORTC.PODR.BYTE = 0x00 ;    /* */
  PORTC.PMR.BYTE  = 0x00 ;    /* All GPIO for now */
  MPC.PC5PFS.BYTE = 0x0D ;    /* PC5 is RSPCKA */
  MPC.PC6PFS.BYTE = 0x0D ;    /* PC6 is MOSIA */
  MPC.PC7PFS.BYTE = 0x0D ;    /* PC7 is MISOA */
  PORTC.PMR.BYTE  = 0xE0 ;    /* PC5-7 assigned to SPI peripheral */
  PORTC.PODR.BYTE = 0x17 ;    /* All outputs low to start */
  PORTC.PDR.BYTE  = 0x7F ;    /* All outputs except MISO */
}
