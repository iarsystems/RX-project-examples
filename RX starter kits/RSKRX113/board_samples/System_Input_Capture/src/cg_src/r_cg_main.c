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
#include "r_cg_lpc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_rtc.h"
#include "r_cg_sci.h"
#include "r_cg_riic.h"
#include "r_cg_s12ad.h"
/* Start user code for include. Do not edit comment generated here */
#include <string.h>
#include <stdio.h>
#include "rskrx113def.h"
#include "r_lcd_appv2.h"
#include "r_rsk_nvm.h"
#include "r_rsk_async.h"
#include "r_cg_s12ad.h"
#include "r_rsk_utility.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

static void rtc_handler(void);
static void serial_handler(void);
static void switch_handler();
static void send_nvm (void);

extern volatile uint8_t g_rtc_period_flag;
extern volatile uint8_t g_rx_char;
extern volatile uint8_t g_rx_flag;
extern volatile uint8_t g_sw2_flag;
extern volatile uint8_t g_sw3_flag;
extern volatile uint8_t g_adc_flag;


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
    __enable_interrupt();
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
    	/* Place device in to Deep Sleep Mode */
    	R_LPC_DeepSleep();

    	/* >> Interrupt has woken MCU from Deep Sleep << */
    	if (TRUE == g_rtc_period_flag)
    	{
    		/* RTC Periodic Interrupt was wake up cause */
    		rtc_handler();

    	}

    	if (TRUE == g_rx_flag)
    	{
    		/* Serial Rx Interrupt was wake up cause */
    		serial_handler();
    	}

    	if ( (TRUE == g_sw2_flag) |  (TRUE == g_sw3_flag) )
    	{
    		/* Key press was wake up cause */
    		switch_handler();
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
    /* Turn off all LEDS*/
    LED0 = (LED_OFF);
    LED1 = (LED_OFF);
    LED2 = (LED_OFF);
    LED3 = (LED_OFF);

    /* Enable UART reception and transmission */
    R_ASYNC_Init();

    /* Create and start the LCD driver */
	R_LCD_Create();
	R_LCD_Start();

	/* Enable the interrupt associated with SW2 and SW3 */
	R_ICU_IRQ2_Start();
	R_ICU_IRQ3_Start();

	/* Start the Real Time Clock and enable the constant period interrupt at an initial 1 second */
	R_RTC_Start();
	R_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);

	/* Start the IIC Controller */
	R_RIIC0_Start();

	/* Init. Non-volatile memory */
	R_NVM_Init();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */






/******************************************************************************
* Function Name : send_nvm
* Description   : Sends the contents of NVM to the terminal via UART1
*                 formatted in csv strings
* Argument      : none
* Return value  : none
******************************************************************************/
static void send_nvm (void)
{
    /* temporary storage for NVM record */
    nvm_record_t   nvm_rec;
    char temp_str[64];

    /* Get the NVM record count */
    uint16_t       r;
    const uint16_t total_records = R_NVM_GetRecordCount();

    /* Banner title */
    sprintf(temp_str, "\r\nRenesas Data Logger: Displaying %d records\r\n", total_records);
    R_ASYNC_TextWrite(temp_str);

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
        R_ASYNC_TextWrite(temp_str);

        /* Time */
        sprintf(temp_str, "%d%d:%d%d:%d%d,", UPPER_NIBBLE(nvm_rec.log_time.rhrcnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rhrcnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.rmincnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rmincnt),
                                             UPPER_NIBBLE(nvm_rec.log_time.rseccnt),
                                             LOWER_NIBBLE(nvm_rec.log_time.rseccnt));

        /* Additional check for 12 am/pm, RTC will contain 0 in the rhrcnt */
        if (0 == (nvm_rec.log_time.rhrcnt & 0x3f))
        {
            temp_str[0] = (char)0x31;
            temp_str[1] = (char)0x32;
        }

        R_ASYNC_TextWrite(temp_str);

        /* Temperature */
        sprintf(temp_str, "%f,", nvm_rec.temperature);
        R_ASYNC_TextWrite(temp_str);

        /* ADC */
        sprintf(temp_str, "%03x\r\n", nvm_rec.adc_value);
        R_ASYNC_TextWrite(temp_str);

    }

}

/***********************************************************************************************************************
* Function Name: rtc_handler
* Description  : This function handles and RTC event.
* 			   : Reads the ADC & RTC values, updates the display and writes to NVM.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void rtc_handler(void)
{
/* Create a new instance of the RTC data structure */
rtc_calendarcounter_value_t st_rtc_data;
float temperature;
uint16_t adc_pot_result;
uint8_t time_str[7] = "      ";
char ad_buf[6]  = "     ";

/* Create an ADC run for sampling the AN000 pot channel */
	R_S12AD_Create();

	/* Start an ADC Conversion */
	R_S12AD_Start();
	while (FALSE == g_adc_flag)
	{
		/* Loop while the ADC is sampling */
	}

	/* Reset the ADC Global flag */
	g_adc_flag = FALSE;
		R_S12AD_Get_ValueResult(ADCHANNEL0, &adc_pot_result);

	/* Convert ADC result into a character string, and store in the local string lcd_buffer */
    uint16_to_string(ad_buf, 0u, adc_pot_result);

    /* Display the ADC value - Line 3 is three characters, skip the unused zero  */
    R_LCD_DisplayPanelString( PANEL_LCD_LINE1, (uint8_t *) ad_buf + 1);

    /* Get a temperature sensor reading */
    R_S12AD_GetTemperature(&temperature);

    /* Read the RTC time */
    R_RTC_Get_CalendarCounterValue(&st_rtc_data);

    /* First digit is unused */
    time_str[0] = ' ';

    /* Read the hours value */
    /* RTC uses rhrcnt = 0x00 to represent 12, check and compensate   */
    if (0x00 == (st_rtc_data.rhrcnt & 0x3F) )
    {
    	st_rtc_data.rhrcnt += 0x12;
    }

    /* Read the hours value */
    /* When using 12H mode, bit 6 of the HOUR sfr indicates AM(0)/PM(1),
       need to ensure this bit is not considered displaying the time */
    time_str[1] = (char)(((st_rtc_data.rhrcnt >> 4) & 0x01)  + 0x30);
    time_str[2] = (char)((st_rtc_data.rhrcnt & 0x0F) + 0x30);

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
    time_str[4] = (char)((st_rtc_data.rmincnt >> 4) + 0x30);
    time_str[5] = (char)((st_rtc_data.rmincnt & 0x0F) + 0x30);

    /* Update the time on the LCD panel.   */
    R_LCD_DisplayPanelString(PANEL_LCD_LINE2, time_str);

    /* First digit is unused */
    time_str[0] = ' ';

    /* Assemble and display the time */
    time_str[1] = (char)((st_rtc_data.rseccnt >> 4)  + 0x30);
    time_str[2] = (char)((st_rtc_data.rseccnt & 0x0F) + 0x30);
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

    /* Store out the captured data to the Non Volatile memory */
    R_NVM_Store(&st_rtc_data, temperature,  adc_pot_result);

    /* Reset the global RTC event flag */
	g_rtc_period_flag = FALSE;
}

/***********************************************************************************
* End of function rtc_handler
*************************************************************************************/

/***********************************************************************************************************************
* Function Name: switch_handler
* Description  : This function responds to a user switch being pressed.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void switch_handler(void)
{
	R_CMT_DebounceDelay();

	if (TRUE == g_sw2_flag)
	{
		/* Reset the global flag */
		g_sw2_flag = FALSE;

		/* Set the constant period to 1 second */
		R_RTC_Set_ConstPeriodInterruptOn(PES_1_SEC);
	}
	else if (TRUE == g_sw3_flag)
	{
		/* Reset the global flag */
		g_sw3_flag = FALSE;

		/* Set the constant period to 2 seconds */
		R_RTC_Set_ConstPeriodInterruptOn(PES_2_SEC);
	}
	else
	{
		/* Error unknown switch pressed */
	}
}

/***********************************************************************************
* End of function switch_handler
*************************************************************************************/

/************************************************************************************
* Function Name: serial_handler
* Description  : This function handles a serial character received.
* Arguments    : None
* Return Value : None
***********************************************************************************/
static void serial_handler(void)
{

	if ( ('r' == g_rx_char) || ('R' == g_rx_char))
	{
		R_ASYNC_TextWrite("Read\r\n");
		send_nvm();
	}

	if ( ('e' == g_rx_char) || ('E' == g_rx_char))
	{
		R_ASYNC_TextWrite("Erase\r\n");
		R_NVM_Erase();
	}

	/* Reset the Serial Rx Global flag */
	g_rx_flag = FALSE;
}

/***********************************************************************************
* End of function serial_handler
*************************************************************************************/

/* End user code. Do not edit comment generated here */
