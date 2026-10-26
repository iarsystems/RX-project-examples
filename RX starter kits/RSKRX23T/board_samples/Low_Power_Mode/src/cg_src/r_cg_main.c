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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2016-02-12
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
#include "r_cg_lpc.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */

#include "r_okaya_lcd.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Some of the code in this file is generated using "Code Generator".
 * Warnings exist in this module. */

/* Operation mode definition */
typedef enum
{
    MODE_NORMAL1,
    MODE_SLEEP,
    MODE_NORMAL2,
    MODE_DEEP_SLEEP,
    MODE_NORMAL3,
    MODE_SW_STANDBY
} op_mode_t;

/* Local functions set the various low power modes */
static void normal_mode (void);
static void sleep_mode (void);
static void software_standby_mode (void);
static void deep_sleep_mode (void);

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#if __LITTLE_ENDIAN__
#pragma public_equ="__MDES",0xFFFFFFFFUL
#else
#pragma public_equ="__MDES",0xFFFFFFF8UL
#endif
#pragma public_equ="__OFS0",0xFFFFFFFFUL
#pragma public_equ="__OFS1",0xFFFFFFFFUL


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

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSKRX23T ");
    R_LCD_Display(1, (uint8_t *)" Low_Power_Mode ");

    /* start the interrupt for SW1 */
    R_ICU_IRQ5_Start();

    while (1U)
    {
        /* Operating mode variable */
        static volatile op_mode_t op_mode = MODE_NORMAL1;

        switch (op_mode)
        {
            case MODE_NORMAL1:
            case MODE_NORMAL2:
            case MODE_NORMAL3:
            {
                normal_mode();
                op_mode++;
            }
            break;

            case MODE_SLEEP:
            {
                sleep_mode();
                op_mode++;
            }
            break;

            case MODE_DEEP_SLEEP:
            {
                deep_sleep_mode();
                op_mode++;
            }
            break;

            case MODE_SW_STANDBY:
            {
                software_standby_mode();
                op_mode = MODE_NORMAL1;
            }
            break;

            default:
            {
                /* Do nothing */
            }
            break;
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
* Function Name : normal_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit
* Argument      : none
* Return value  : none
******************************************************************************/
static void normal_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Normal Mode          ");
    R_LCD_Display(4, (uint8_t *)" Press SW1       ");

    /* clear the switch flag */
    g_switch_flag = 0;

    /* Wait for switch SW1 to be pressed */
    while (1 != g_switch_flag)
    {
        ;
    }

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function normal_mode
******************************************************************************/

/******************************************************************************
* Function Name : sleep_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void sleep_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Sleep Mode     ");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_LPC_Sleep();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function sleep_mode
******************************************************************************/

/******************************************************************************
* Function Name : deep_sleep_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters deep_sleep_mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void deep_sleep_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Deep Sleep Mode     ");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_LPC_DeepSleep();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function deep_sleep_mode
******************************************************************************/

/******************************************************************************
* Function Name : software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters software_standby_mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void software_standby_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" SW Standby Mode");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_LPC_SoftwareStandby();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function software_standby_mode
******************************************************************************/

/* End user code. Do not edit comment generated here */
