/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products.

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : led.c
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Contains general purpose LED drivers
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines Special Flag Registers (SFR)s for RX63N */
#include "iorx63t_clr.h"
#include "led.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define LED0_PODR       (PORT7.PODR.BIT.B1)
#define LED1_PODR       (PORT7.PODR.BIT.B2)
#define LED2_PODR       (PORT7.PODR.BIT.B3)
#define LED3_PODR       (PORT3.PODR.BIT.B3)

#define LED0_PMR        (PORT7.PMR.BIT.B1)
#define LED1_PMR        (PORT7.PMR.BIT.B2)
#define LED2_PMR        (PORT7.PMR.BIT.B3)
#define LED3_PMR        (PORT3.PMR.BIT.B3)

#define LED0_PDR        (PORT7.PDR.BIT.B1)
#define LED1_PDR        (PORT7.PDR.BIT.B2)
#define LED2_PDR        (PORT7.PDR.BIT.B3)
#define LED3_PDR        (PORT3.PDR.BIT.B3)

#define LED_DELAY       (0xF0000)


/******************************************************************************
* Function Name: led_initialize
* Description  : Sets up the I/O pins that drive the LEDs
* Arguments    : None
* Return value : None
*******************************************************************************/
void led_initialize(void)
{
    /* All LEDs are off */
    LED0_PODR = LED_OFF;
    LED1_PODR = LED_OFF;
    LED2_PODR = LED_OFF;
    LED3_PODR = LED_OFF;

    /* Pins are output */
    LED0_PDR = 1;
    LED1_PDR = 1;
    LED2_PDR = 1;
    LED3_PDR = 1;
}

/******************************************************************************
* Function Name : led_set
* Description   : Sets the LED on or off
* Arguments     : led -
*                   led number (LED0 thru LED3)
*               : state -
*                   led state (LED_ON or LED_OFF)
* Return value  : none
*******************************************************************************/
void led_set(uint32_t led, uint32_t state)
{
    switch (led)
    {
        case LED0:
            LED0_PODR = state; /* set the output to state paramter */
            break;

        case LED1:
            LED1_PODR = state;
            break;

        case LED2:
            LED2_PODR = state;
            break;

        case LED3:
            LED3_PODR = state;
            break;

        default:
            break;
    }
}

/******************************************************************************
* Function Name : led_toggle
* Description   : Toggles the LED state
* Arguments     : led -
*                   led number (LED0 thru LED3)
* Return value  : none
*******************************************************************************/
void led_toggle(uint32_t led)
{
    switch (led)
    {
        case LED0:
            LED0_PODR ^= 1; /* exclusive or to toggle the state */
            break;

        case LED1:
            LED1_PODR ^= 1;
            break;

        case LED2:
            LED2_PODR ^= 1;
            break;

        case LED3:
            LED3_PODR ^= 1;
            break;

        default:
            break;
    }
}

/******************************************************************************
* Function Name : led_test
* Description   : Blinks the LEDs
* Arguments     : nr_led_flashes
*                   number of times to flash LEDs
* Return value  : none
*******************************************************************************/
void led_test(uint32_t nr_led_flashes)
{
    uint32_t i = LED_DELAY;
    uint32_t j;

    LED0_PODR = LED_ON;
    LED1_PODR = LED_OFF;
    LED2_PODR = LED_ON;
    LED3_PODR = LED_OFF;

    for (j = 0; j < nr_led_flashes; j++)
    {
        while(i--)
        {
            ; /* Delay, do nothing. */
        }

        i = LED_DELAY;

        LED0_PODR ^= 1; /* invert the state. Causes blink. */
        LED1_PODR ^= 1;
        LED2_PODR ^= 1;
        LED3_PODR ^= 1;
    }

    LED0_PODR = LED_OFF;
    LED1_PODR = LED_OFF;
    LED2_PODR = LED_OFF;
    LED3_PODR = LED_OFF;
}

