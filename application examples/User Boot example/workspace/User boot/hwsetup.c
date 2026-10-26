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
#include "rskRX63Ndefines.h"


/******************************************************************************
Function     : HardwareSetup
Description 	: Enables access to the MPC registers
Calls InitCGC() to initialise the Clock Generation Circuit
Configures the I/O pins connected to the LEDs as outputs                  
Argument  	: none
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
  LED0 = OFF;	            
  LED1 = OFF;	            
  LED2 = OFF;	            
  LED3 = OFF;	            
  
  /* Set Port Direction Registers                                          */
  LED0_PDR = OUTPUT;	    
  LED1_PDR = OUTPUT;	    
  LED2_PDR = OUTPUT;	    
  LED3_PDR = OUTPUT;	    
}