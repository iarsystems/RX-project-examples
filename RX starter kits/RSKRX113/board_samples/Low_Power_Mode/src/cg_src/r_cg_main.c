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
* Creation Date: 2015-01-27
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
#include "r_cg_lpc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_lcd_appv2.h"
#include "r_cg_lpc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Enumerated list for the power mode sequence */
typedef enum
{
    NORMAL, SLEEP, DEEP_SLEEP, SOFTWARE_STANDBY
} power_mode_t;

/* Switch flag global - used to indicate switch press from interrupt routine */
volatile uint8_t g_switch_flag = 0x00;

/* Power mode to enter next SW press - initialised to Normal */
power_mode_t g_next_power_mode = NORMAL;

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
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1)
    {
        /* Perform any required actions before entering a Low Power Mode */
        R_LPC_PrepForLPM();

        /* Switch - enter the power mode determined by g_next_power_mode */
        switch (g_next_power_mode)
        {
            case NORMAL :
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " NORM");
            break;

            case SLEEP :
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " SLP ");
                R_LPC_Sleep();
            break;

            case DEEP_SLEEP :
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " DSLP");
                R_LPC_DeepSleep();
            break;

            case SOFTWARE_STANDBY :
                R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " SSBY");
                R_LPC_SoftwareStandby();
            break;

            default :
            break;
        }

        /* Perform any required actions after exiting a Low Power Mode */
        R_LPC_RecoverFromLPM();

        while (0x00 == g_switch_flag)
        {

            /* Loop here until switch is pressed */
        };

        R_CMT_DebounceDelay();

        /* Reset the global switch pressed flag*/
        g_switch_flag = 0;

        /* Setup the next power mode to enter */
        if (SOFTWARE_STANDBY == g_next_power_mode)
        {
            /* If exiting Software Standby, the next mode will be Normal */
            g_next_power_mode = NORMAL;
        }
        else
        {
            /* Set to the next one in the enum list */
            g_next_power_mode++;
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
    __enable_interrupt();
    /* Start user code. Do not edit comment generated here */
    R_CMT0_Start();
    R_LCD_Create();
    R_LCD_Start();

    /* Enable the interrupt associated with SW2 */
    R_ICU_IRQ2_Start();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
