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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : main.c 
* Version     : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description   : Initialises the USB MSC application.
*******************************************************************************/
/*******************************************************************************
*   History     : 26.05.2010 Ver 1.0  
*******************************************************************************/

/*******************************************************************************
User Includes
*******************************************************************************/
/* The following header file defines functions used for the LCD */
#include "lcd.h"
/* Contains delcarations for the functions defined in this file */
#include "main.h"
/* MSC header*/
#include "usb_msc.h"
#include "hwsetup.h"
#include "intrinsics.h"

/*******************************************************************************
User Program Code Prototypes
*******************************************************************************/

/*******************************************************************************
System Definitions & Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Name : main
* Description  : This initialises the RSK for MSC Application.
* Parameters  : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  HardwareSetup();
  USBMSC_Init();
  
  __enable_interrupt();

  while(1);  
}
/*******************************************************************************
End main function
*******************************************************************************/

/*******************************************************************************
Function Name:   abort
Description:  Required by standard lib. when using assert.
Parameters:   none
Return value:   none
*******************************************************************************/
void abort(void)
{
  DEBUG_MSG_LOW(("\r\n *** ABORT *** \r\n"));

  while(1);
}
/*******************************************************************************
End of function abort
*******************************************************************************/















