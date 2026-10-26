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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RX64M V1.00.01.01 [09 May 2014]
* Device(s)    : R5F564MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* Description  : This file implements main function.
* Creation Date: 30/06/2014
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
#include "r_cg_rtc.h"
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
    MODE_SW_STANDBY,
    MODE_NORMAL3,
    MODE_DEEP_SW_STANDBY
} op_mode_t;
/* Local functions set the various low power modes */
static void normal_mode (void);
static void sleep_mode (void);
static void software_standby_mode (void);
static void deep_software_standby_mode (void);
static void display_time (void);

/* End user code. Do not edit comment generated here */


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

    /* Initialise the debug LCD */
    R_LCD_Init();
    
    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSK+RX64M ");
    R_LCD_Display(1, (uint8_t *)" Low_Power_Mode ");
    
    /* Start the RTC */
    R_RTC_Start();
    
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
                
            case MODE_SW_STANDBY:
            {
                software_standby_mode();
                op_mode++;
            }
            break;
                
            case MODE_DEEP_SW_STANDBY:
            {
                deep_software_standby_mode();
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
    EI(); // Enable interrupts 
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

    /* Start the RTC and enable the constant period interrupt */
    R_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);

    /* clear the switch flag */
    g_switch_flag = 0;
    
    /* Wait for switch SW1 to be pressed */
    while (1 != g_switch_flag)
    {
        if (TRUE == g_rtc_interrupt)
        {
            /* Clear down the interrupt flag and display the new time */
            g_rtc_interrupt = FALSE;
            display_time();
        }
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
*                 to exit, then eneters sleep mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void sleep_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Sleep Mode     ");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");
    
    /* Disable RTC periodic interrupt */
    R_RTC_Set_ConstPeriodInterruptOff();
    
    R_LPC_Sleep();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function sleep_mode
******************************************************************************/

/******************************************************************************
* Function Name : software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then eneters sleep mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void software_standby_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" SW Standby Mode");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");
   
    /* Disable RTC periodic interrupt */
    R_RTC_Set_ConstPeriodInterruptOff();
    
    R_LPC_SoftwareStandby();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/******************************************************************************
* End of function software_standby_mode
******************************************************************************/

/******************************************************************************
* Function Name : deep_software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
******************************************************************************/
static void deep_software_standby_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Deep SW Standby Mode");
    R_LCD_Display(4, (uint8_t *)" RESET to exit");

    /* Disable RTC periodic interrupt */
    R_RTC_Stop();
    
    R_LPC_DeepSoftwareStandby();

}
/******************************************************************************
* End of function deep_software_standby_mode
******************************************************************************/

 /******************************************************************************
* Function Name : display_time
* Description   : function to get the current time from the RTC and display it 
*                 on the LCD.
* Argument      : none
* Return value  : none
******************************************************************************/
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
/******************************************************************************
* End of function display_time
******************************************************************************/

/* End user code. Do not edit comment generated here */
