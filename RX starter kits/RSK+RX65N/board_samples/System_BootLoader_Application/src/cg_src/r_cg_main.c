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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX65N V1.00.00.08 [25 Aug 2016]
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2016-10-12
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

#include "r_okaya_lcd.h"
#include "r_rsk_async.h"
#include "rskrx65ndef.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

static void toggle_leds (void);
volatile uint8_t g_switch_flag;

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#if __LITTLE_ENDIAN__
#pragma public_equ="__MDES",0xFFFFFFFFUL
#else
#pragma public_equ="__MDES",0xFFFFFFF8UL
#endif
#pragma public_equ="__OFS0",0xFFFFFFFFUL
#pragma public_equ="__OFS1",0xFFFFFFFFUL


static void R_MAIN_UserInit(void);
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

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSK+RX65N ");
    R_LCD_Display(1, (uint8_t *)" System BootLoder ");
    R_LCD_Display(2, (uint8_t *)" Application ");
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

    R_ICU_IRQ8_Start();
    R_CMT1_Start();

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

/***********************************************************************************************************************
End of Function toggle_leds
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
