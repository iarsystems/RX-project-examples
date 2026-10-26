/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This file implements main function.
* Creation Date: 2022-01-18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_okaya_lcd.h"

#include "Config_ICU.h"
#include "Config_CMT0.h"
#include "Config_LPC.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Operation mode definition */
typedef enum
{
    MODE_NORMAL1,
    MODE_SLEEP,
    MODE_NORMAL2,
    MODE_SW_STANDBY,
    MODE_NORMAL3,
    MODE_DEEP_SW_STANDBY
} e_op_mode_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* Local functions set the various low power modes */
static void normal_mode (void);
static void sleep_mode (void);
static void software_standby_mode (void);
static void deep_software_standby_mode (void);

static void display_time (void);


void main (void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD.
       Casting for use as characters. */
    R_LCD_Display(0, (uint8_t *)" RSKRX660  ");

    /* Casting for use as characters. */
    R_LCD_Display(1, (uint8_t *)" Low_Power_Mode ");

    /* Start the RTC and enable the constant period interrupt */
    R_Config_RTC_Start();

    /* start the interrupt for SW1 */
    R_Config_ICU_IRQ9_Start();

    while (1U)
    {
        /* Operating mode variable */
        static volatile e_op_mode_t s_op_mode = MODE_NORMAL1;

        switch (s_op_mode)
        {
            case MODE_NORMAL1:
            {
                normal_mode();
                s_op_mode = MODE_SLEEP;
                break;
            }

            case MODE_SLEEP:
            {
                sleep_mode();
                s_op_mode = MODE_NORMAL2;
                break;
            }

            case MODE_NORMAL2:
            {
                normal_mode();
                s_op_mode = MODE_SW_STANDBY;
                break;
            }

            case MODE_SW_STANDBY:
            {
                software_standby_mode();
                s_op_mode = MODE_NORMAL3;
                break;
            }

            case MODE_NORMAL3:
            {
                normal_mode();
                s_op_mode = MODE_DEEP_SW_STANDBY;
                break;
            }

            case MODE_DEEP_SW_STANDBY:
            {
                deep_software_standby_mode();
                break;
            }

            default:
            {
                /* Do nothing */
                break;
            }
        }
    }
}
/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : normal_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void normal_mode(void)
{
    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" Normal Mode         ");

    /* Casting for use as characters. */
    R_LCD_Display(4, (uint8_t *)" Press SW1       ");

    /* clear the switch flag */
    g_switch_flag = 0;

    /* Wait for switch SW1 to be pressed */
    while (1 != g_switch_flag)
    {
        display_time();
    }

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function normal_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : sleep_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void sleep_mode(void)
{
    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" Sleep Mode     ");

    /* Casting for use as characters. */
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_Config_LPC_Sleep();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function sleep_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void software_standby_mode(void)
{
    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" SW Standby Mode");

    /* Casting for use as characters. */
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_Config_LPC_SoftwareStandby();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function software_standby_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : deep_software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void deep_software_standby_mode(void)
{
    /* Casting for use as characters. */
    R_LCD_Display(3, (uint8_t *)" Deep SW Standby Mode");

    /* Casting for use as characters. */
    R_LCD_Display(4, (uint8_t *)" RESET to exit");

    R_Config_LPC_DeepSoftwareStandby();

}
/***********************************************************************************************************************
* End of function deep_software_standby_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : display_time
* Description   : function to get the current time from the RTC and display it on the LCD.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void display_time(void)
{
    /* Create a new instance of data structure RTCCounterValue */
    rtc_calendarcounter_value_t st_rtc_data;

    char_t time_str[12] = "           ";

    /* Read the RTC time */
    R_Config_RTC_Get_CalendarCounterValue(&st_rtc_data);

    /* Read the hours value */
    /* When using 12H mode, bit 6 of the HOUR sfr indicates AM(0)/PM(1),
       need to ensure this bit is not considered displaying the time */
    time_str[1] = (((char_t)st_rtc_data.rhrcnt >> 4) & 0x03)  + 0x30;

    /* Set the hour lower value */
    time_str[2] = ((char_t)st_rtc_data.rhrcnt & 0x0F) + 0x30;

    /* ASCII code for ':' */
    time_str[3] = 0x3A; 

    /* Set the minutes upper value */
    time_str[4] = ((char_t)st_rtc_data.rmincnt >> 4) + 0x30;

    /* Set the minutes lower value */
    time_str[5] = ((char_t)st_rtc_data.rmincnt & 0x0F) + 0x30;

    /* ASCII code for ':' */
    time_str[6] = 0x3A; 

    /* Set the seconds upper value */
    time_str[7] = ((char_t)st_rtc_data.rseccnt >> 4) + 0x30;

    /* Set the seconds lower value  */
    time_str[8] = ((char_t)st_rtc_data.rseccnt & 0x0F) + 0x30;

    /* Set the AM/PM bit, check if it indicates AM
       and add the appropriate symbol */
    if (st_rtc_data.rhrcnt & _40_RTC_HOUR_ATTRIB_PM)
    {
        time_str[9] = 'p';

        /* Additional check for 12 pm, RTC will contain 0 in the rhrcnt */
        if (0 == (st_rtc_data.rhrcnt & 0x3f))
        {
            time_str[1] = '1';
            time_str[2] = '2';
        }
    }
    else
    {
        time_str[9] = 'a';
    }
    time_str[10] = 'm';

    /* Casting for use as characters. */
    R_LCD_Display(2, (uint8_t *)time_str);
}

/***********************************************************************************************************************
* End of function display_time
***********************************************************************************************************************/
