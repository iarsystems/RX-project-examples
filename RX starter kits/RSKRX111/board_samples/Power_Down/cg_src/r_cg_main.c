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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements main function.
* Creation Date: 2013-08-26
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
#include <stdbool.h>
#include "lcd.h"
#include "switch.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
bool g_enter_sleep = false;
bool g_enter_standby = false;
bool g_enter_deep_sleep = false;

static void cb_switch_press (void);
static void release_key (void);

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFFFFUL;


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
    Clear_Display();
    DisplayCenter(0, (uint8_t *)"Power Down");
    
    /* Start toggling the LEDs */
    R_CMT1_Start();

    /* Configure a switch press callback function */
    SetSwitchPressCallback(cb_switch_press);
    
    while (1)
    {
        /* Check if the g_enter_standby flag is true */
        if (g_enter_sleep)
        {
            /* Stop toggling the LEDs */
            R_CMT1_Stop();
            
            /* Switch off the LEDs in case they are ON */
            all_LED_off();  
            
            DisplayCenter(3, (uint8_t *)"Sleep Mode");
            
            /* wait for transmission to end */
            while (sci5_tx_is_busy())
            {
                /* Wait */
            }

            /* Ensure the key is released, otherwise it will generate interrupt on release */
            release_key();

            R_LPC_Sleep();
            
            /* restart SCI */
            R_SCI5_Start();

            /* Start toggling the LEDs */
            R_CMT1_Start();

            /* Clear sleep message */ 
            DisplayClearLine(3);
            
            /* Reset the flag */
            g_enter_sleep = false;

            /* Now make sure the key is released, otherwise we will generate another power down */
            release_key();
        }
       else if (g_enter_standby)
        {
            /* Stop toggling the LEDs */
            R_CMT1_Stop();
            
            /* Switch off the LEDs in case they are ON */
            all_LED_off();  
            
            DisplayCenter(3, (uint8_t *)"Standby Mode");
            
            /* wait for transmission to end */
            while (sci5_tx_is_busy())
            {
                /* Wait */
            }

            /* Ensure the key is released, otherwise it will generate interrupt on release */
            release_key();

            /* Enter standby mode */
            R_LPC_SoftwareStandby();
            
            /* Start toggling the LEDs */
            R_CMT1_Start();

            /* Clear standby message */ 
            DisplayClearLine(3);
            
            /* Reset the flag */
            g_enter_standby = false;

            /* Now make sure the key is released, otherwise we will generate another power down */
            release_key();
        }
        else if (g_enter_deep_sleep)
        {
            /* Stop toggling the LEDs */
            R_CMT1_Stop();
            
            /* Switch off the LEDs in case they are ON */
            all_LED_off();

            DisplayCenter(3, (uint8_t *)"Deep Sleep Mode");

            /* wait for transmission to end */
            while (sci5_tx_is_busy())
            {
                /* Wait */
            }
            
            /* Ensure the key is released, otherwise it will generate interrupt on release */
            release_key();

            /* Enter standby mode */
            R_LPC_DeepSleep();
            
            /* Clear deep sleep message */ 
            DisplayClearLine(3);

            /* Start toggling the LEDs */
            R_CMT1_Start();

            /* Reset the flag */
            g_enter_deep_sleep = false;

            /* Now make sure the key is released, otherwise we will generate another power down */
            release_key();
        }
        else
        {
            /* do nothing */
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
    Init_LCD();
    InitialiseSwitches();
     
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/******************************************************************************
* Function Name : cb_switch_press
* Description   : Switch press callback funtion. Checks for SW press and if
*                 SW1 sets the g_enter_deep_standby flag, if SW2 sets the 
*                 g_enter_standby flag.
* Argument      : none  
* Return value  : none
******************************************************************************/
static void cb_switch_press (void)
{   
    /* Check if switch 1 was pressed */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_1)
    {
        /* Set the g_enter_sleep flag */
        g_enter_sleep = true;
    }
    else if ((g_switch_flag & 0x0F) & SWITCHHOLD_2)
    {
        /* Set the g_enter_deep_sleep flag */
        g_enter_deep_sleep = true;
    }
    else if ((g_switch_flag & 0x0F) & SWITCHHOLD_3)
    {
        /* Set the g_enter_standby flag */
        g_enter_standby = true;
    }
    else
    {
        /* Do nothing */
    }
}
/******************************************************************************
* End of function cb_switch_press
******************************************************************************/

/******************************************************************************
* Function Name : release_key
* Description   :
* Argument      : none
* Return value  : none
******************************************************************************/
static void release_key (void)
{
    while (!g_switch_standby_ready)
    {
        /* Wait until key is relesed */
    }

    /* Clear switch flag */
    g_switch_flag = 0x0;
    g_enter_sleep = false;
    g_enter_deep_sleep = false;
    g_enter_standby = false;

}
/******************************************************************************
* End of function release_key
******************************************************************************/

/* End user code. Do not edit comment generated here */
