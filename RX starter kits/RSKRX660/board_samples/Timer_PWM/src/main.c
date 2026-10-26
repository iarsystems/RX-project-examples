/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This file implements main function.
* Creation Date: 2022-01-18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_okaya_lcd.h"
#include "r_rsk_switch.h"
#include "r_rsk_utility.h"

#include "rskrx660def.h"

#include "Config_MTU3.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

static void cb_switch_press (void);

void main (void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Initialize the switch module */
    R_SWITCH_Init();

    /* Set the call back function when SW1 is pressed */
    R_SWITCH_SetPressCallback(cb_switch_press);

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD
       Casting for use as characters. */
    R_LCD_Display(0, (uint8_t *)" RSKRX660  ");

    /* Casting for use as characters. */
    R_LCD_Display(1, (uint8_t *)" Timer_PWM ");

    /* Casting for use as characters. */
    R_LCD_Display(2, (uint8_t *)" PWM Varying 10-90%");

    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" 1KHz on LED0");

    /* Start the MTU3 */
    R_Config_MTU3_Start();

    while (1U)
    {
        static uint8_t s_current_vary_duty_cycle = TRUE;

        /* Has the switch been pressed to stop/start duty cycle variation ?*/
        if (s_current_vary_duty_cycle != g_vary_duty_cycle)
        {
            /* It has changed - which way? */
            if (FALSE == g_vary_duty_cycle)
            {
                /* Local LCD buffer string */
                char_t   lcd_buffer[]      = " PWM Stopped at xx%";
                uint32_t calc_duty_percent = 0;

                /* Calculate the percentage.
                   Casting to ensure use of correct data type. */
                calc_duty_percent = (uint32_t)(((uint32_t)g_duty_update * 100u) / TICKS_PER_CYCLE);

                /* Convert the calculated percentage into a BCD character string.
                   Casting the size to a uint16_t type is valid because
                   since the computed duty value does not exceed 0xFFFF. */
                uint16_to_bcd_string(lcd_buffer, 16u, (uint16_t)calc_duty_percent);

                /* Variation stopped */
                R_LCD_Display(2, (uint8_t *)lcd_buffer);
            }
            else
            {
                /* Variation restarted.
                   Casting for use as characters. */
                R_LCD_Display(2, (uint8_t *)" PWM Varying 10-90%");
            }

            /* Store new state - to avoid continuously updating LCD */
            s_current_vary_duty_cycle = g_vary_duty_cycle;
        }
    }
}
/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : cb_switch_press
* Description   : Switch press callback function.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void cb_switch_press(void)
{
    /* Check if switch 1 was pressed */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Change flag to indicate stoppage of duty cycle updates. */
        if (TRUE == g_vary_duty_cycle)
        {
            g_vary_duty_cycle  = FALSE;
        }
        else
        {
            g_vary_duty_cycle  = TRUE;
        }
    }
}

/***********************************************************************************************************************
* End of function cb_switch_press
***********************************************************************************************************************/
