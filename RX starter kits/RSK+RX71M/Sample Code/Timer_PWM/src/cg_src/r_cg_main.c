/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RX64M V1.00.01.01 [09 May 2014]
* Device(s)    : R5F571MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* Description  : This file implements main function.
* Creation Date: 03/07/2014
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
#include "r_cg_mtu3.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_okaya_lcd.h"
#include "r_rsk_switch.h"
#include "rskrx71mdef.h"
#include "r_rsk_utility.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static void cb_switch_press (void);

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

    /* Initialise the switch module */
    R_SWITCH_Init();

    /* Set the call back function when SW1 or SW2 is pressed */
    R_SWITCH_SetPressCallback(cb_switch_press);

    /* Initialise the debug LCD */
    R_LCD_Init();
    
    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSK+RX71M ");
    R_LCD_Display(1, (uint8_t *)" Timer_PWM ");
    R_LCD_Display(2, (uint8_t *)" PWM Varying 10-90%");
    R_LCD_Display(3, (uint8_t *)" 1KHz on R281");
    
    /* Start the MTU3 */
    R_MTU3_C2_Start();
   
    while (1U)
    {
        static uint8_t current_vary_duty_cycle = TRUE;
        
        /* Has the switch been pressed to stop/start duty cycle variation ?*/
        if (current_vary_duty_cycle != g_vary_duty_cycle)
        {
            /* It has changed - which way? */
            if (FALSE == g_vary_duty_cycle)
            {
                /* Local LCD buffer string */
                char        lcd_buffer[] = " PWM Stopped at xx%";
                uint32_t    calc_duty_percent = 0;

                /* Calculate the percentage */
                calc_duty_percent =
                    (uint32_t)(((uint32_t)g_duty_update * 100u) / TICKS_PER_CYCLE);

                /* Convert the calculated percentage into a BCD character string */
                uint16_to_bcd_string(lcd_buffer, 16u, (uint16_t)calc_duty_percent);
                
                /* variation stopped */
                R_LCD_Display(2, (uint8_t *)lcd_buffer);
            }
            else
            {
                /* variation restarted */
                R_LCD_Display(2, (uint8_t *)" PWM Varying 10-90%");
            }
            
            /* Store new state - to avoid continuously updating LCD */
            current_vary_duty_cycle = g_vary_duty_cycle;
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
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
* Function Name : cb_switch_press
* Description   : Switch press callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
static void cb_switch_press (void)
{
    /* Check if switch 2 was pressed */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Set flag to indicate stoppage of duty cycle updates. */
        g_vary_duty_cycle  = FALSE;
    }

    /* Either SW2 has been pressed  - restart duty cycle variation*/
    else
    {
        /* Allow duty cycle to be updated */
        g_vary_duty_cycle  = TRUE;
    }

}

/******************************************************************************
* End of function cb_switch_press
******************************************************************************/

/* End user code. Do not edit comment generated here */
