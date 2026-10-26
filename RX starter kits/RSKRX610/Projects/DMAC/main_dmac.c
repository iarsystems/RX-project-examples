/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : main_dmac.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Demonstration of the DMAC unit. Program transfers a 32bit
*          word from a source address, to an incrementing destination
*          address.
* Instructions  : 1. Compile and download the sample code. Press 'Go'
*           to start the software.
*
*          2. The DMAC transfer will be begin when the program starts.
*             LED1 will light when the transfer is complete.
*
*          3. Once the transfer is complete, stop the program and add
*             the variable gDMA_DataBuff to the watch window. Observe
*           the contains of the array. The DMAC transfer will have
*           filled the array with alternated hex values 0xFF and 0xEE.
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* LCD function definitions */
#include "lcd.h"
/* DMAC function definitions */
#include "dmac.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Global variable, used to store data sent from the DMAC */
char gDMA_DataBuff[1024];


/*””FUNC COMMENT””*************************************************************
* Outline     : main
* Description   : The main program function. Function displays the the splash
*          screen to the LCD display, then calls the DMAC initialisation
*          function. The main function then waits in an infinite while
*          loop.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the LCD Display */
  InitialiseLCD();

  /* Display Splash Screen */
  DisplayLCD(LCD_LINE1, "  DMAC  ");

  /* Initialise DMAC */
  Init_DMAC();

  /* Infinite While Loop */
  while (1);
}
/******************************************************************************
End of function main
******************************************************************************/
