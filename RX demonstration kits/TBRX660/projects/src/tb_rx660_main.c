/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : tb_rx660_main.c
 * Version      : 1.00
 * Device(s)    : R5F56609BDFP
 * Tool-Chain   : CC-RX v3.04.00
 * Smart Configurator : v21.7.0
 * H/W Platform : Target Board for RX660
 * Description  : The function of this program selects either the user switch
 *                interrupt or the timer interrupt according to the mode
 *                selection at startup and controls the lighting of the LED by
 *                using the interrupt.
 * Operation    : Auto mode   : (1-1) Power on reset.
 *                              (1-2) LED0, LED1 blink alternate.(interval 500 ms)
 *                Manual mode : (2-1) Press down SW1 button with reset.
 *                              (2-2) When SW1 is clicked, LED0, LED1 blink
 *                                    alternately.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 17.01.2022 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "tbrx660def.h"
#include "r_smc_entry.h"
#include "Config_ICU.h"
#include "Config_CMT0.h"
#include "Config_PORT.h"

void main(void);

/**********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void main(void)
{
    LED0 = LED_OFF;
    LED1 = LED_OFF;

    if (SW1_PUSH == SW1) /*SW1_PUSH = Manual Mode,  Other = Auto Mode*/
    {
        R_Config_ICU_IRQ4_Start();
    }
    else
    {
        R_Config_CMT0_Start();
    }
    while(1)
    {
        ; /* Blink LED0, LED1 Alternately by Interrupt */
    }

}
/**********************************************************************************************************************
 * End of function main(void)
 *********************************************************************************************************************/
