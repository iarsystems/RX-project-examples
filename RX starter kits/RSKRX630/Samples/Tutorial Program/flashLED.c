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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : flashLED.c
* Version     : 1.00
* Device     : R5F5630E
* Tool-Chain   : Renesas RX Toolchain 1.2.0.0
* H/W Platform  : RSKRX630
* Description   : Defines functions used for flashing the LEDs
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>
/* Defines RX630 LEDs and switches */
#include "rskrx630def.h"
/* Flashing LED function definitions */
#include "flashLED.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Compare Match Timer callback function prototype */
void Toggle_LED(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The global variable 'KeyPress' is used as a flag whenever a switch is 
   pressed */
extern uint8_t gSwitchFlag;
/* Variable used to count down the number of LED flashes */
static uint16_t gFlashCount = 0xC8;

/*******************************************************************************
* Outline     : Flash_LED
* Description   : The LED flash function used at the beginning of the program
* Argument    : none
* Return value  : none
*******************************************************************************/
void Flash_LED (void)
{
  /* Declare a delay count variable */
  uint32_t ulLed_Delay = 0;

  /* Flash the LEDs for 200 times or until a user switch is pressed */
  while((gSwitchFlag == 0)&&(--gFlashCount > 0))
  {
    for (ulLed_Delay = 0; ulLed_Delay < 4000000; ++ulLed_Delay)
    {
    }
     
    /* Toggles the LEDs after a specific delay. */
      Toggle_LED();
  }
  
  /* Reset the gSwitchFlag flag variable */
  gSwitchFlag = 0;
}
/*******************************************************************************
* End of function Flash_LED
*******************************************************************************/

/*******************************************************************************
* Outline     : Toggle_LED
* Description   : Function used to toggle the output of all user LEDs
* Argument    : none
* Return value  : none
*******************************************************************************/
void Toggle_LED(void)
{
  /* Toggle the state of the user LEDs  */
  LED0 = (uint8_t)~LED0;
  LED1 = (uint8_t)~LED1;
  LED2 = (uint8_t)~LED2;
  LED3 = (uint8_t)~LED3;
}
/*******************************************************************************
* End of function Toggle_LED
*******************************************************************************/
