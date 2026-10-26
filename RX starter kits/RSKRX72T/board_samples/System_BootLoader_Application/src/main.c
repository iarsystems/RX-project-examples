/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F572TKCxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX72T
* Description  : This file implements main function.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_cg_macrodriver.h"
#include "Config_ICU.h"
#include "Config_CMT3.h"
#include "r_okaya_lcd.h"
#include "r_rsk_async.h"
#include "rskrx72tdef.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static void r_main_userinit(void);
static void toggle_leds (void);
volatile uint8_t g_switch_flag;

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    r_main_userinit();

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD.
       Casting for use as characters. */
    R_LCD_Display(0, (uint8_t *)" RSKRX72T ");

    /* Casting for use as characters. */
    R_LCD_Display(1, (uint8_t *)" System BootLoder ");

    /* Casting for use as characters. */
    R_LCD_Display(2, (uint8_t *)" Application ");

    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" Press SW1 ");

    /* Enable UART reception and transmission */
    R_ASYNC_Init();

    while (1U)
    {
        /* Handle a received serial character */
        if (('z' == g_rx_char) || ('Z' == g_rx_char)) /* Check for control chars from the serial terminal */
        {
            g_rx_char = 0x00; /* Character has been received and acted upon */
            toggle_leds(); /* Toggle the LEDs */
        }

        /* Handle a switch press */
        else if (1 == g_switch_flag)
        {
            R_CMT_DebounceDelay();
            g_switch_flag = 0; /* Reset the switch pressed flag */
            toggle_leds(); /* Toggle the LEDs */
        }
        else
        {
            /* Do Nothing */
        }
    }
}
/***********************************************************************************************************************
* End of Function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: r_main_userinit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_main_userinit(void)
{
    R_Config_ICU_IRQ0_Start();
    R_Config_CMT3_Start();

    /* Turn off all LEDs.
       Casting to correctly use MCU registers(LED0). */
    LED0 = (uint8_t)(LED1 = (LED2 = (LED3 = LED_OFF)));

}

/***********************************************************************************************************************
* End of Function r_main_userinit
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: toggle_leds
* Description  : Simply inverts the state of each LED
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void toggle_leds (void)
{
    /* Invert the state of LED0 */
    LED0 ^= 1;

    /* Invert the state of LED1 */
    LED1 ^= 1;

    /* Invert the state of LED2 */
    LED2 ^= 1;

    /* Invert the state of LED3 */
    LED3 ^= 1;
}

/***********************************************************************************************************************
* End of Function toggle_leds
***********************************************************************************************************************/

