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
* Creation Date: 2015-01-21
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
#include "r_cg_mtu2.h"
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_lcd_appv2.h"
#include "rskrx113def.h"
#include "r_rsk_utility.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static void poll_switch (void);

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    __enable_interrupt();
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    R_MTU2_C4_Start(); /* Start the Multi-Function Timer Unit */
    R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " RUN ");

    while (1U)
    {
        static uint8_t current_vary_duty_cycle = TRUE; /* Current status for run mode */
        uint32_t calc_duty_percent = 0; /* Store for calculated duty cycle */
        uint8_t text_str[7] = "      "; /* String for LCD character display */

        /* Has the switch SW1 been pressed to stop/start duty cycle variation ?*/
        poll_switch();

        if (current_vary_duty_cycle != g_vary_duty_cycle)
        {
            /* It has changed - which way? */
            if (FALSE == g_vary_duty_cycle)
            {
                /* Calculate the percentage */
                calc_duty_percent = (uint32_t) (((uint32_t) g_duty_update * 100u) / TICKS_PER_CYCLE);

                /* Convert the calculated percentage into a BCD character string */
                uint16_to_bcd_string((char*) text_str, 0u, (uint16_t) calc_duty_percent);

                /* Display status and duty cycle percentage */
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " STOP");
                R_LCD_DisplayPanelString(PANEL_LCD_LINE3, text_str);
            }
            else
            {
                /* variation restarted */
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " RUN ");

                /* Blank off previous percentage display */
                R_LCD_DisplayPanelString(PANEL_LCD_LINE3, "      ");
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

    R_LCD_Create();
    R_LCD_Start();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
 * Function Name : poll_switch
 * Description   : Function to poll the status of SW1 on Port PJ0 and cancel
 * Argument      : none
 * Return value  : none
 ******************************************************************************/
static void poll_switch (void)
{
    if (FALSE == SW1)
    {
        if (TRUE == g_vary_duty_cycle)
        {
            g_vary_duty_cycle = FALSE;
        }
        else
        {
            g_vary_duty_cycle = TRUE;
        }
    }

    while (FALSE == SW1)
    {

        /* Loop until switch is released */
    }

    /* Delay for 100ms to de-bounce switch */
    delay100ms();
}
/******************************************************************************
 * End of function poll_switch
 ******************************************************************************/

/* End user code. Do not edit comment generated here */
