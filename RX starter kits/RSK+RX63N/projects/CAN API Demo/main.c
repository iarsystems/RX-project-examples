/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : main.c
* Version       : 1.0
* Description   : Empty application template
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 12.07.2011 1.00    First Release
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines standard I/O functions */
#include <stdio.h>
#include "led.h"
#include "switches.h"
#include "lcd.h"
#include "api_demo.h"

#include "intrinsics.h"
#include "hwsetup.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define LED_DELAY       (0xFF000)
#define LED_COUNT       (1000000)

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void main(void);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void led_demo(void);


/******************************************************************************
* Function Name : main
* Description   : Main program function
* Arguments     : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt();
    
    /* Initialize LED, push button swithes, and 2x8 LCD */
    led_initialize();
    switches_initialize();
    Init_LCD();

    /* Display message on LCD */
    Display_LCD(LCD_LINE1, "RSK");
    Display_LCD(LCD_LINE2, "RX63N");

    /* 
    STDOUT is routed through the virtual console window tunneled through the JTAG debugger.
    Enable terminal I/O in EWRX to view output.
    */
    printf("This is the debug console.\r\n");

    /* Demo LED APIs */
    led_demo();

    /* Run the CAN API Demo. */
    api_demo();
}

/******************************************************************************
* Function Name : led_demo
* Description   : Runs a simple LED test using provided APIs.
* Arguments     : none
* Return value  : none
*******************************************************************************/
static void led_demo(void)
{
    uint32_t i;

    /* Test LEDs */
    led_set(LED0, LED_ON);
    for (i = 0; i < LED_DELAY; i++)
    {
    }
    led_toggle(LED0);
    for (i = 0; i < LED_DELAY; i++)
    {
    }

    led_toggle(LED1);
    for (i = 0; i < LED_DELAY; i++)
    {
    }
    led_set(LED1, LED_OFF);
    for (i = 0; i < LED_DELAY; i++)
    {
    }

    led_set(LED2, LED_ON);
    for (i = 0; i < LED_DELAY; i++)
    {
    }
    led_toggle(LED2);
    for (i = 0; i < LED_DELAY; i++)
    {
    }

    led_toggle(LED3);
    for (i = 0; i < LED_DELAY; i++)
    {
    }
    led_set(LED3, LED_OFF);
    for (i = 0; i < LED_DELAY; i++)
    {
    }
}
