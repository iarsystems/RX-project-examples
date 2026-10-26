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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F565NEDxFC
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N-2MB
* Description  : This file implements main function.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.05.2017 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "r_smc_entry.h"
#include "r_okaya_lcd.h"
#include "r_rsk_nvm.h"
#include "Config_S12AD0.h"
#include "Config_S12AD1.h"
#include "Config_RTC.h"
#include "Config_SCI8.h"
#include "Config_RIIC0.h"
#include "Config_ICU.h"
#include "Config_CMT0.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    DATA_ACQ_HALF_SEC,
    DATA_ACQ_ONE_SEC,
    DATA_ACQ_ONE_MIN

} data_acquisition_period_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* result of NVM store */
static uint8_t nvm_store_ok;

/* Prototype declaration for get_adc */
static uint16_t get_adc(void);

/* Prototype declaration for get_temp */
static double get_temp(void);

/* Prototype declaration for lcd_display_time */
static void lcd_display_time(rtc_calendarcounter_value_t * const pstRTC_Data);

/* Prototype declaration for lcd_display_temp */
static void lcd_display_temp(const double temperature);

/* Prototype declaration for lcd_display_adc */
static void lcd_display_adc(const uint16_t adc_value);

/* Prototype declaration for snooze_mode */
static void snooze_mode(void);

/* Prototype declaration for send_nvm */
static void send_nvm(void);

/* Prototype declaration for rtc_handler */
static void rtc_handler(void);

/* Prototype declaration for switch_handler */
static void switch_handler(void);

/* Prototype declaration for serial_handler */
static void serial_handler(void);

/* local variables to this module */
static char temp_str[64];

static volatile data_acquisition_period_t current_acq_period = DATA_ACQ_ONE_SEC;

void main(void);

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

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSK+RX65N-2MB ");
    R_LCD_Display(1, (uint8_t *)" System_Input_Capture ");
    R_LCD_Display(2, (uint8_t *)"  ");

    /* Start the A/D converter */
    R_Config_S12AD0_Start();

    /* Start RTC */
    R_Config_RTC_Start();
    R_Config_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);

    /* Set up SCI8 receive buffer and callback function */
    R_Config_SCI8_Serial_Receive((uint8_t *)&g_rx_char, 1);

    /* Enable SCI8 operations */
    R_Config_SCI8_Start();

    /* Enable I2C eeprom interface */
    R_Config_RIIC0_Start();

    /* Initialize the NVM */
    nvm_store_ok = R_NVM_Init();

    /* Start IRQ (SW1, SW2, SW3) */
    R_Config_ICU_IRQ11_Start();
    R_Config_ICU_IRQ13_Start();
    R_Config_ICU_IRQ15_Start();

    while (1U)
    {
        /* Check PC terminal for key presses */
        /* Check the contents of g_rx_char */
        if (0 != g_rx_char)
        {
            /* Serial Rx Interrupt was wake up cause */
            serial_handler();
        }

        /* g_data_acq_flag gets set by the RTC periodic interrupt */
        if (TRUE == g_data_acq_flag)
        {
            /* RTC Periodic Interrupt was wake up cause */
            rtc_handler();
        }

        if ( ((TRUE == g_sw1_flag) || (TRUE == g_sw2_flag)) || (TRUE == g_sw3_flag) )
        {
            /* Key press was wake up cause */
            switch_handler();
        }
    }

}
/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_adc
* Description  : Reads the ADC result.
* Argument     : none
* Return value : uint16_t adc value
***********************************************************************************************************************/
static uint16_t get_adc(void)
{
    /* A variable to retrieve the adc result */
    uint16_t adc_result;

    /* Start a conversion */
    R_Config_S12AD0_Start();

    /* Wait for the A/D conversion to complete */
    while (FALSE == g_adc0_complete)
    {
        /* Wait */
    }

    /* Stop conversion */
    R_Config_S12AD0_Stop();

    /* Clear ADC flag */
    g_adc0_complete = FALSE;

    R_Config_S12AD0_Get_ValueResult(ADCHANNEL0, &adc_result);

    return (adc_result);
}

/***********************************************************************************************************************
* End of function get_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_temp
* Description  : Reads the ADC result using the temperature sensor
*                converts to float and returns
* Argument     : none
* Return value : double temperature value
***********************************************************************************************************************/
static double get_temp(void)
{

    /* Temperature voltage */
    uint16_t int_temp_hex;

    /* floating point for temperature calculation */
    double   temperature;

    /* Starts the AD1 converter */
    R_Config_S12AD1_Start();

    /* Wait for the A/D conversion to complete */
    while (FALSE == g_adc1_complete)
    {
        /* Wait */
    }

    /* Stop conversion */
    R_Config_S12AD1_Stop();

    /* Clear ADC flag */
    g_adc1_complete = FALSE;

    R_Config_S12AD1_Get_ValueResult(ADTEMPSENSOR, &int_temp_hex);

    /* Plug into equation to calculate temperature */
    temperature = (((((int_temp_hex * 3.3F)/4096) - 1.21F) / 0.004F) + 25.0);

    return (temperature);
}

/***********************************************************************************************************************
* End of function get_temp
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: lcd_display_time
* Description  : function to get the current time from the RTC and display it 
*                on the LCD.
* Argument     : rtc_calendarcounter_value_t * stRTC_Data
* Return value : none
***********************************************************************************************************************/
static void lcd_display_time(rtc_calendarcounter_value_t * const pstRTC_Data)
{
    char time_str[12] = " ";
    char interval_str[30];

    /* Read the hours value */
    /* When using 12H mode, bit 6 of the HOUR sfr indicates AM(0)/PM(1),
       need to ensure this bit is not considered displaying the time */
    time_str[1] = (char)(((pstRTC_Data->rhrcnt >> 4) & 0x03)  + 0x30);
    time_str[2] = (char)((pstRTC_Data->rhrcnt & 0x0F) + 0x30);
    time_str[3] = ':';

    /* Additional check for 12 am/pm, RTC will contain 0 in the rhrcnt */
    if (0 == (pstRTC_Data->rhrcnt & 0x3f))
    {
        time_str[1] = (char)0x31;
        time_str[2] = (char)0x32;
    }

    /* Read the minutes value */
    time_str[4] = (char)((pstRTC_Data->rmincnt >> 4) + 0x30);
    time_str[5] = (char)((pstRTC_Data->rmincnt & 0x0F) + 0x30);
    time_str[6] = ':';

    /* Read the seconds value */
    time_str[7] = (char)((pstRTC_Data->rseccnt >> 4)  + 0x30);
    time_str[8] = (char)((pstRTC_Data->rseccnt & 0x0F) + 0x30);

    /* Read the AM/PM bit, check if it indicates AM
       and add the appropriate symbol */
    if (pstRTC_Data->rhrcnt & _40_RTC_HOUR_ATTRIB_PM)
    {
        time_str[9] = 'p';

    }
    else
    {
        time_str[9] = 'a';
    }
    time_str[10] = 'm';

    R_LCD_Display(2, (uint8_t *)time_str);

    /* Also display the curent acquisition interval */
    strcpy(interval_str, " Acq. Period: ");

    switch (current_acq_period)
    {
        case DATA_ACQ_HALF_SEC:
            strcat(interval_str, "0.5 sec");
            break;
        case DATA_ACQ_ONE_SEC:
            strcat(interval_str, "1 sec  ");
            break;
        case DATA_ACQ_ONE_MIN:
            strcat(interval_str, "1 min  ");
            break;
        default:

            /* Do nothing */
            break;
    }

    R_LCD_Display(3, (uint8_t *)interval_str);

}

/***********************************************************************************************************************
* End of function lcd_display_time
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: lcd_display_temp
* Description  : function to get the current time from the RTC and display it 
*                on the LCD.
* Argument     : temperature as double precision floating point
* Return value : none
***********************************************************************************************************************/
static void lcd_display_temp(const double temperature)
{
    char temperature_str[CHAR_PER_LINE];

    sprintf(temperature_str, " Temp: %0.2f deg. C ", temperature);

    R_LCD_Display(4, (uint8_t *)temperature_str);

}

/***********************************************************************************************************************
* End of function lcd_display_temp
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: lcd_display_adc
* Description  : function to get the current time from the RTC and display it 
*                on the LCD.
* Argument     : uint16_t adc_value
* Return value : none
***********************************************************************************************************************/
static void lcd_display_adc(const uint16_t adc_value)
{
    char adc_str[20];

    sprintf(adc_str, " ADC: %03XH       ", adc_value);
    R_LCD_Display(5, (uint8_t *)adc_str);

}

/***********************************************************************************************************************
* End of function lcd_display_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: snooze_mode
* Description  : Executes the instruction to send the device into software standby mode but with RTC wake capability.
*                ADC conversion is performed and if the threshold is still below 0x200 then we re-enter software standby
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void snooze_mode(void)
{
    do
    {
        /* Go into software standby mode */
        R_Config_LPC_SoftwareStandby();

    } while (get_adc() < 0x200);


    /* RTC counter need 1/128s for SW Standby mode */
    R_CMT_MsDelay(8);

    /* When exiting from SoftwareStandby it is required to re-initialize the
     * Flash Memory Controller as FWEPROR is initialized SW standby */
    R_NVM_SleepRecover();
}

/***********************************************************************************************************************
* End of function snooze_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: send_nvm
* Description  : Sends the contents of NVM to the terminal via UART1
*                formatted in csv strings
* Argument     : none
* Return value : none
***********************************************************************************************************************/
static void send_nvm(void)
{
    /* temporary storage for NVM record */
    nvm_record_t   nvm_rec;

    /* Get the NVM record count */
    uint16_t       r;
    const uint16_t total_records = R_NVM_GetRecordCount();

    /* Banner title */
    sprintf(temp_str, "\r\nRenesas Data Logger: Displaying %d records\r\n", total_records);
    R_SCI8_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

    /* Get each of the records in turn and and send to the terminal via USB COM port */
    for (r = 0; r < total_records; r++)
    {
        /* Get the NVM record */
        R_NVM_GetRecord(r, &nvm_rec);

        /* format the data into a .csv string */
        /* Date first */

        sprintf(temp_str, "%d%d/%d%d/%d%d,", UPPER_NIBBLE(nvm_rec.log_time.rdaycnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rdaycnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.rmoncnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rmoncnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.ryrcnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.ryrcnt));

        R_SCI8_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

        /* Time */

        sprintf(temp_str, "%d%d:%d%d:%d%d,", UPPER_NIBBLE(nvm_rec.log_time.rhrcnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rhrcnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.rmincnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rmincnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.rseccnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rseccnt));

        /* Additional check for 12 am/pm, RTC will contain 0 in the rhrcnt */
        if ((nvm_rec.log_time.rhrcnt))
        {
            temp_str[0] = (char)0x31;
            temp_str[1] = (char)0x32;
        }

        R_SCI8_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

        /* Temperature and ADC */
        sprintf(temp_str, "%f,%03x\r\n", nvm_rec.temperature, nvm_rec.adc_value);
        R_SCI8_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

    }

}

/***********************************************************************************************************************
* End of function send_nvm
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: rtc_handler
* Description  : This function handles and RTC event.
*              : Reads the ADC & RTC values, updates the display and writes to NVM.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void rtc_handler(void)
{
    /* Variables used to acquire data */
    rtc_calendarcounter_value_t     st_rtc_data;
    uint16_t                        adc_value;
    double                          temperature;

    do{
        /* Read the RTC time */
        R_Config_RTC_Get_CalendarCounterValue(&st_rtc_data);

        /* Get the temperature */
        temperature = get_temp();

        /* Get the ADC value */
        adc_value = get_adc();

        /* Display acquired data on the LCD */
        lcd_display_time(&st_rtc_data);
        lcd_display_temp(temperature);
        lcd_display_adc(adc_value);

        g_data_acq_flag = FALSE;

        /* Store the data in NVM */
        if (TRUE == nvm_store_ok)
        {
            nvm_store_ok = R_NVM_Store(&st_rtc_data, temperature, adc_value);
        }
        else
        {
            R_LCD_Display(6, (uint8_t *)" FULL");
        }

        /* Go to snooze if the ADC value drops back below 0x200 */
        if (0x200 > adc_value)
        {
            /* Tell the user we are going into snooze mode */
            R_LCD_Display(4, (uint8_t *)"                     ");
            R_LCD_Display(5, (uint8_t *)"Snooze mode.....     ");
            R_LCD_Display(6, (uint8_t *)"                    ");

            /* Enter snooze mode */
            snooze_mode();
        }
    }while(TRUE == g_data_acq_flag);

}

/***********************************************************************************************************************
* End of function rtc_handler
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: switch_handler
* Description  : This function responds to a user switch being pressed.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void switch_handler(void)
{
    R_CMT_DebounceDelay();

    /* Stop RTC */
    R_Config_RTC_Stop();

    if (TRUE == g_sw1_flag)
    {
        /* Reset the global flag */
        g_sw1_flag = FALSE;

        /* Set the constant period to 0.5 second */
        R_Config_RTC_Set_ConstPeriodInterruptOn(PES_1_2_SEC);
        current_acq_period = DATA_ACQ_HALF_SEC;
    }
    else if (TRUE == g_sw2_flag)
    {
        /* Reset the global flag */
        g_sw2_flag = FALSE;

        /* Set the constant period to 1 seconds */
        R_Config_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);
        current_acq_period = DATA_ACQ_ONE_SEC;
    }
    else if (TRUE == g_sw3_flag)
    {
        /* Reset the global flag */
        g_sw3_flag = FALSE;

        /* Stop the constant period interrupt, use alarm interrupt (1 minute) */
        R_Config_RTC_Set_ConstPeriodInterruptOff();
        current_acq_period = DATA_ACQ_ONE_MIN;
    }
    else
    {
        /* Error unknown switch pressed */
    }

    /* Start RTC */
    R_Config_RTC_Start();

    /* Set the data acq flag, to cause an LCD update before the next wake up */
    g_data_acq_flag = TRUE;
}

/***********************************************************************************************************************
* End of function switch_handler
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: serial_handler
* Description  : This function handles a serial character received.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void serial_handler(void)
{
    if (('e' == g_rx_char) || ('E' == g_rx_char))
    {
        uint16_t total_records = R_NVM_GetRecordCount();
        g_rx_char = 0;

        /* Erase the NVM */
        R_NVM_Erase();
        nvm_store_ok = TRUE;

        /* Tell the user what we have done */
        sprintf(temp_str, "\r\nErased % d records\r\n", total_records);
        R_SCI8_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

        R_LCD_Display(6, (uint8_t *)"        ");
    }
    else if (('r' == g_rx_char) || ('R' == g_rx_char))
    {
        g_rx_char = 0;

        /* Send the NVM records to the terminal */
        send_nvm();

    }
    else
    {
        /* Nothing to do */
    }
}

/***********************************************************************************************************************
* End of function serial_handler
***********************************************************************************************************************/

