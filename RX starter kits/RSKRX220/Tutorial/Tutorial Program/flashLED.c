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
* File Name     : flashled.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines functions used for flashing the LEDs
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Flashing LED function definitions */
#include "flashLED.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* The global variable 'g_switch_flag' is used as a flag whenever a switch is 
   pressed */
extern volatile uint8_t g_switch_flag;

/*******************************************************************************
* Function Name: flash_led
* Description  : The LED flash function used at the beginning of the program
* Arguments    : none
* Return Value : none
*******************************************************************************/
void flash_led (void)
{
    /* Declare a delay count variable */
    volatile uint32_t ulLed_Delay = 0;
    
    /* Declare a flash count variable */
    uint16_t Flash_Count = LEDFLASHCOUNT;

    /* Flash the LEDs for 200 times or until a user switch is pressed */
    while ((0x00 == g_switch_flag) && (0 < --Flash_Count))
    {
        for (ulLed_Delay = 0; ulLed_Delay < LEDFLASHDELAY; ++ulLed_Delay)
        {
            /* Delay briefly for LED flash */
        }
        
        /* Toggles the LEDs after a specific delay. */
        toggle_led();
    }
    
    /* Reset the g_switch_flag flag variable */
    g_switch_flag = 0x00;
}
/*******************************************************************************
* End of function flash_led
*******************************************************************************/

/*******************************************************************************
* Function Name: toggle_led
* Description  : Function used to toggle the output of all user LEDs
* Arguments    : none
* Return Value : none
*******************************************************************************/
void toggle_led (void)
{
    /* Toggle the state of the user LEDs    */
    LED0 = (uint8_t) ~LED0;
    LED1 = (uint8_t) ~LED1;
    LED2 = (uint8_t) ~LED2;
    LED3 = (uint8_t) ~LED3;
}
/*******************************************************************************
* End of function toggle_led
*******************************************************************************/

