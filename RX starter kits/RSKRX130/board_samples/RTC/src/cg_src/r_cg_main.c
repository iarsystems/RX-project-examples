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
* Version      : Applilet4 for RX130 V1.00.01.02 [07 Mar 2016]
* Device(s)    : R5F51305AxFN
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2016-09-23
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
#include "r_cg_tmr.h"
#include "r_cg_cmt.h"
#include "r_cg_rtc.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */

#include "r_okaya_lcd.h"
#include "r_rsk_switch.h"
#include "rskrx130def.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

static void cb_switch_press(void);
static void display_time(void);
static void display_alarm(void);

/* Flag to indicate when the user has cancelled the alarm */
static uint8_t alarm_cancelled = FALSE;

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

    /* Initialize the switch module */
    R_SWITCH_Init();

    /* Set the call back function when SW1 or SW2 is pressed */
    R_SWITCH_SetPressCallback(cb_switch_press);

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSKRX130 ");
    R_LCD_Display(1, (uint8_t *)" RTC ");

    /* Start the RTC and enable the constant period interrupt */
    R_RTC_Start();
    R_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);

    while (1U)
    {
        if (TRUE == g_rtc_interrupt)
        {
            /* Clear down the interrupt flag and display the new time */
            g_rtc_interrupt = FALSE;
            display_time();

            /* Check the alarm status and display/process accordingly */
            if (ALARM_ON == g_alarm_status)
            {
                display_alarm();
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
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: cb_switch_press
* Description  : Switch press callback function.
* Argument     : none
* Return value : none
***********************************************************************************************************************/
static void cb_switch_press (void)
{
    /* Check if switch 1 was pressed */
    if (g_switch_flag & SWITCHPRESS_1)
    {
         /* Cancel the alarm, only if it is active */
         if (ALARM_ON == g_alarm_status)
         {
             alarm_cancelled = TRUE;
         }

        /* Clear flag */
        g_switch_flag = 0x0;
    }
}
/***********************************************************************************************************************
* End of function cb_switch_press
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: display_time
* Description  : function to get the current time from the RTC and display it on the LCD.
* Argument     : none
* Return value : none
***********************************************************************************************************************/
static void display_time (void)
{
    /* Create a new instance of data structure RTCCounterValue */
    rtc_calendarcounter_value_t st_rtc_data;

    char                        time_str[12] = "           ";

    /* Read the RTC time */
    R_RTC_Get_CalendarCounterValue(&st_rtc_data);

    /* Read the hours value */
    /* When using 12H mode, bit 6 of the HOUR sfr indicates AM(0)/PM(1),
       need to ensure this bit is not considered displaying the time */
    time_str[1] = (char)(((st_rtc_data.rhrcnt >> 4) & 0x03)  + 0x30);
    time_str[2] = (char)((st_rtc_data.rhrcnt & 0x0F) + 0x30);
    time_str[3] = ':';

    /* Read the minutes value */
    time_str[4] = (char)((st_rtc_data.rmincnt >> 4) + 0x30);
    time_str[5] = (char)((st_rtc_data.rmincnt & 0x0F) + 0x30);
    time_str[6] = ':';

    /* Read the seconds value */
    time_str[7] = (char)((st_rtc_data.rseccnt >> 4)  + 0x30);
    time_str[8] = (char)((st_rtc_data.rseccnt & 0x0F) + 0x30);

    /* Read the AM/PM bit, check if it indicates AM
       and add the appropriate symbol */
    if (st_rtc_data.rhrcnt & _40_RTC_HOUR_ATTRIB_PM)
    {
        time_str[9] = 'p';

        /* Additional check for 12 pm, RTC will contain 0 in the rhrcnt */
        if (0 == (st_rtc_data.rhrcnt & 0x3f))
        {
            time_str[1] = (char)0x31;
            time_str[2] = (char)0x32;
        }
    }
    else
    {
        time_str[9] = 'a';
    }
    time_str[10] = 'm';

    R_LCD_Display(2, (uint8_t *)time_str);

}
/***********************************************************************************************************************
* End of function display_time
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: display_alarm
* Description  : function to display the alarm on LCD and LEDs and check for user cancellation.
* Argument     : none
* Return value : none
***********************************************************************************************************************/
static void display_alarm (void)
{
    /* Variable used to count the number of seconds after the alarm interrupt
       occurs */
    static uint8_t alarm_timeout = 30u;

    /* Check for alarm cancelled flag or timeout */
    if ((TRUE == alarm_cancelled) || (0 == (alarm_timeout--)))
    {
        /* Clear down flags and blank out alarm areas on LCD and turn off LED*/
        g_alarm_status = ALARM_OFF;
        alarm_cancelled = FALSE;
        R_LCD_Display(3, (uint8_t *)"       ");
        R_LCD_Display(4, (uint8_t *)"                     ");
        LED1 = LED_OFF;
    }
    else
    {
        rtc_calendarcounter_value_t st_rtc_data;

        /* Read the RTC time */
        R_RTC_Get_CalendarCounterValue(&st_rtc_data);

        /* toggle ALARM every second */
        if ((st_rtc_data.rseccnt % 2) == 0)
        {
            R_LCD_Display(3, (uint8_t *)" ALARM");
            LED1 = LED_OFF;
        }
        else
        {
            R_LCD_Display(3, (uint8_t *)"      ");
            LED1 = LED_ON;
        }
        R_LCD_Display(4, (uint8_t *)" Press SW1 to cancel");
    }
}
/***********************************************************************************************************************
* End of function display_alarm
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
