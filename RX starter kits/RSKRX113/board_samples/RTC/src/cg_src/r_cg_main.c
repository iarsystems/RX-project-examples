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
* Creation Date: 2015-01-20
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
#include "r_cg_port.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_lcd_appv2.h"
#include "rskrx113def.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Number of seconds for the alarm to auto-cancel */
#define ALARM_TIMEOUT_SECONDS (30)

/* Flag to indicate the cancellation of an alarm event */
static uint8_t alarm_cancelled = FALSE;

/* Global variable to time the auto-cancel of an alarm event */
static uint8_t alarm_timeout = ALARM_TIMEOUT_SECONDS;

static void process_time (void);
static void process_alarm (void);
static void poll_alarm_cancel_switch (void);

/* RTC Alarm flag global - used to indicate an RTC Alarm Event */
uint8_t g_rtc_alarm_flag = FALSE;

/* RTC constant period global - used to flag an RTC constant period event  */
uint8_t g_rtc_const_flag = FALSE;

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma public_equ="__OFS0",0xFFFFFFFF
#pragma public_equ="__OFS1",0xFFFFFF7F
//#pragma location = "OPTBYTE"
//__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
//#pragma location = "OPTBYTE"
//__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


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

        if (TRUE == g_rtc_const_flag)
        {
            /* Clear the constant period interrupt flag */
            g_rtc_const_flag = FALSE;

            /* Display the time */
            process_time();

            /* Handle the alarm condition if set */
            if (TRUE == g_rtc_alarm_flag)
            {
                process_alarm();
            }
        }

        /* If an alarm event is active, check the alarm cancel switch */
        if (TRUE == g_rtc_alarm_flag)
        {
            poll_alarm_cancel_switch();
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
    R_RTC_Start();

    /* Enable the RTC Constant period interrupt at 1 second intervals */
    R_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);

    /* Turn off all LEDs */
    LED0 = (LED1 = (LED2 = (LED3 = LED_OFF)));

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/******************************************************************************
 * Function Name : process_time
 * Description   : function to display time display and alarm time-out
 * Argument      : none
 * Return value  : none
 ******************************************************************************/
static void process_time (void)
{
    /* Create a new instance of the RTC data structure */
    rtc_calendarcounter_value_t st_rtc_data;

    uint8_t time_str[7] = "      ";

    /* Read the RTC time */
    R_RTC_Get_CalendarCounterValue(&st_rtc_data);

    /* First digit is unused */
    time_str[0] = ' ';

    /* Read the hours value */
    /* RTC uses rhrcnt = 0x00 to represent 12, check and compensate   */
    if (0x00 == (st_rtc_data.rhrcnt & 0x3F))
    {
        st_rtc_data.rhrcnt += 0x12;
    }

    /* When using 12H mode, bit 6 of the HOUR sfr indicates AM(0)/PM(1),
     need to ensure this bit is not considered displaying the time */
    time_str[1] = (char) (((st_rtc_data.rhrcnt >> 4) & 0x01) + 0x30);
    time_str[2] = (char) ((st_rtc_data.rhrcnt & 0x0F) + 0x30);

    /* toggle : every second */
    if ((st_rtc_data.rseccnt % 2) == 0)
    {
        time_str[3] = ':';
    }
    else
    {
        time_str[3] = ' ';

    }

    /* Read the minutes value */
    time_str[4] = (char) ((st_rtc_data.rmincnt >> 4) + 0x30);
    time_str[5] = (char) ((st_rtc_data.rmincnt & 0x0F) + 0x30);

    /* Update the time on the LCD panel.   */
    R_LCD_DisplayPanelString(PANEL_LCD_LINE2, time_str);

    /* First digit is unused */
    time_str[0] = ' ';

    /* Read the seconds value */
    time_str[1] = (char) ((st_rtc_data.rseccnt >> 4) + 0x30);
    time_str[2] = (char) ((st_rtc_data.rseccnt & 0x0F) + 0x30);
    time_str[3] = '\0';

    R_LCD_DisplayPanelString(PANEL_LCD_LINE3, time_str);

    /* Read the AM/PM bit, turn on/off the appropriate symbol */
    if (st_rtc_data.rhrcnt & 0x40)
    {
        /* Turn on the PM symbol */
        R_LCD_DisplaySymbol(LCD_PM_ON);

        /* Turn off the AM symbol */
        R_LCD_DisplaySymbol(LCD_AM_OFF);
    }
    else
    {
        /* Turn off the PM symbol */
        R_LCD_DisplaySymbol(LCD_PM_OFF);

        /* Turn on the AM symbol */
        R_LCD_DisplaySymbol(LCD_AM_ON);
    }
}

/******************************************************************************
 * End of function process_time
 ******************************************************************************/

/******************************************************************************
 * Function Name : process_alarm
 * Description   : function to display the alarm on LCD and LEDs and check for
 *                 alarm cancellation.
 * Argument      : none
 * Return value  : none
 ******************************************************************************/
static void process_alarm (void)
{
    /* If the time out has reached zero, cancel the alarm */
    if (0 == alarm_timeout)
    {
        /* Set the alarm cancelled flag*/
        alarm_cancelled = TRUE;

        /* Re-load the time out for the next event */
        alarm_timeout = ALARM_TIMEOUT_SECONDS;
    }
    else
    {
        /* Decrement the alarm timeout */
        alarm_timeout--;
    }

    /* Check for alarm cancelled flag or timeout */
    if (TRUE == alarm_cancelled)
    {
        /* Clear down flags and blank out alarm areas on LCD and turn off LED*/
        g_rtc_alarm_flag = FALSE;
        alarm_cancelled = FALSE;

        /* Clear the bottom LCD line */
        R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) "     ");

        /* Turn off the 'ALARM' LCD Symbol */
        R_LCD_DisplaySymbol(LCD_ALARM_OFF);

        /* Turn off LED1 */
        LED1 = LED_OFF;

    }
    else
    {
        /* Display ALM on the bottom LCD line */
        R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t*) " ALM ");

        /* Turn on the 'ALARM' LCD Symbol */
        R_LCD_DisplaySymbol(LCD_ALARM_ON);

        /* Turn on all user LEDs */
        LED1 = LED_ON;
    }
}
/******************************************************************************
 * End of function process_alarm
 ******************************************************************************/

/******************************************************************************
 * Function Name : poll_alarm_cancel_switch
 * Description   : Function to poll the status of SW1 on Port PJ0 and cancel
 * 			    an alarm event if pressed.
 * Argument      : none
 * Return value  : none
 ******************************************************************************/
static void poll_alarm_cancel_switch (void)
{
    if (FALSE == SW1)
    {
        /* If SW1 is pressed set the alarm cancel flag */
        alarm_cancelled = TRUE;
    }
}
/******************************************************************************
 * End of function poll_alarm_cancel_switch
 ******************************************************************************/

/* End user code. Do not edit comment generated here */
