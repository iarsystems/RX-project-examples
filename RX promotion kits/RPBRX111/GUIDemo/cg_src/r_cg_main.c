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
* Creation Date: 08.10.2013
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
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* RPB pin definitions */
#include "rpbrx111.h"
/* For Debug Console */
#include <stdio.h>
#include <stdlib.h>
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

#include "commands.h"
#include "bootflags.h"

/* Flag to store current sleep state */
sleep_state_t g_sleep_state;
static uint8_t g_jtag_enabled = 1;

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFFFFUL;
volatile bootflags_t BootFlag;

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

    /* Check boot mode flag for lower clock speeds for current measurement */
    BootFlag = get_boot_flag();
//    switch (get_boot_flag())
    switch (BootFlag)
    {
    case BOOT_SUBCLK:
        set_boot_flag(BOOT_UNDEFINED);
        Select_Operating_Mode(LOW_SPEED);
        g_jtag_enabled = 0;
        break;

    case BOOT_MIDCLK:
        set_boot_flag(BOOT_UNDEFINED);
        Select_Operating_Mode(MIDDLE_SPEED);
        g_jtag_enabled = 0;
        break;

    default:
        /* we get here if boot flag has not been set or has been cleared - run program normally */
        break;
    }

    while (1U)
    {
        /* Set sleep state based on flag. On wake, MCU will resume inside
           R_LPC_...() function. */
        switch (g_sleep_state)
        {
            case MODE_SLEEP:
            {
                R_LPC_Sleep();

                /* Clear sleep flag and resume */
                g_sleep_state = MODE_RUN;

               break;
            }
            case MODE_DEEPSLEEP:
            {
                R_LPC_DeepSleep();

                /* Clear sleep flag and resume */
                g_sleep_state = MODE_RUN;

                break;
            }
            case MODE_SWSTANDBY:
            {
                R_LPC_SoftwareStandby();

                /* Clear sleep flag and resume */
                g_sleep_state = MODE_RUN;

                break;
            }
            /* Nothing to do in run mode, fall through */
            case MODE_RUN:
            default:
            {
                /* Command processing over debug console */
                if (1 == g_jtag_enabled)
                {
                    cmd_process();
                }
                break;
            }
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

    /* Stop the LOCO */
    SYSTEM.LOCOCR.BIT.LCSTP = 0x1u;

    /* Pull up power mode pins */
    PORT3.PCR.BYTE |= 0x07u;

    /* Enable power mode setting interrupts */
    R_ICU_IRQ0_Start();
    R_ICU_IRQ1_Start();
    R_ICU_IRQ2_Start();

    /* Start RTC */
    R_RTC_Start();

    /* Set initial state */
    g_sleep_state = MODE_RUN;
    IDLE_EN = POWER_OFF;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
