/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :main.c                                                */
/*  DATE        :Mon, Feb 12, 2007                                     */
/*  DESCRIPTION :touch screen Demo for the RLCD2378 kit                */        
/*                                                                     */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.02.2007 RTA-MEV 1st release
************************************************************************/
#define MEM_DECL

/* Renesas Direct LCD Driver include files */
#include "Global.h"
#include "LCD_demo.h"
#include "touchscreen.h"
#include <intrinsics.h>

/**********************************************************************************
Function Name:   main
Description:  Initialize the hardware, start the tasks, start the RTOS.
Arguments:   none.
Return value:   none
***********************************************************************************/
void main(void)
{
  // System Initialization for LCD DD Demo
  initLEDs();               // Initialize the port to drive the on-board LEDs
  initTouch();              // Initialize the touch screen
 
  EventManagerInit();       /* start the event manager tasks */
  
  __enable_interrupt();
  
  vTaskStartScheduler();    /* RTOS_USAGE */
}
