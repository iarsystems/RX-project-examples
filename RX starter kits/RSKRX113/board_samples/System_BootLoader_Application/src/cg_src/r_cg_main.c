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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2015-01-23
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_iwdt.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_lcd_appv2.h"
#include "r_rsk_async.h"
#include "rskrx113def.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
#define LCD_UPDATE_TICKS	(10)	/* Number of 100ms timer ticks between updating LCD */

static void toggle_leds (void);
static uint32_t last_tick_serviced = 0; /* Used to record when the LCD update routine was last serviced */
volatile uint8_t g_switch_flag;

extern volatile uint32_t g_ms_100_counter;

/* End user code. Do not edit comment generated here */

void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    uint8_t display_string[] = " DEMO"; /* String for the scrolling message */
    uint8_t i; /* Counter for the string rotation routine */

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

        /* Handle an LCD display update */
        else if (g_ms_100_counter != last_tick_serviced) /* If the 100ms timer has updated since we last checked */
        {
            last_tick_serviced = g_ms_100_counter; /* Take a record of when we serviced the LCD routine */

            if (0 == (g_ms_100_counter % LCD_UPDATE_TICKS)) /* Update the LCD at the defined number of ticks */
            {
                /* Loop the first character around */
                display_string[(sizeof(display_string)) - 1] = display_string[0];

                /* Swap over the rest of the chars */
                for (i = 0; i < ((sizeof(display_string)) - 1); i++)
                {
                    /* Loop the rest of the string around */
                    display_string[i] = display_string[i + 1];
                }

                /* And finally re-insert the null */
                display_string[(sizeof(display_string)) - 1] = 0x00;

                /* Display the string on the LCD */
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, display_string);
            }
        }
        else
        {
            /* Nothing of interest happened - do nothing */
        }
    }

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    EI();
    /* Start user code. Do not edit comment generated here */
    R_LCD_Create();
    R_LCD_Start();
    R_ICU_IRQ3_Start();
    R_CMT0_Start();

    /* Turn off all LEDs */
    LED0 = (LED1 = (LED2 = (LED3 = LED_OFF)));

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: toggle_leds
 * Description  : Simply inverts the state of each LED
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
static void toggle_leds (void)
{
    LED0 ^= 1; /* Toggle the state of LED0 */
    LED1 ^= 1; /* Toggle the state of LED1 */
    LED2 ^= 1; /* Toggle the state of LED2 */
    LED3 ^= 1; /* Toggle the state of LED3 */
}

/*****************************************************************************
 End of Function toggle_leds
 ******************************************************************************/
/* End user code. Do not edit comment generated here */
