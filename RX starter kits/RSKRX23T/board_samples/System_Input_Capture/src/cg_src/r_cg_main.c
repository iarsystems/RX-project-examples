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
* Creation Date: 2016-02-15
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
#include "r_cg_tmr.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "r_cg_riic.h"
#include "r_cg_s12ad.h"
/* Start user code for include. Do not edit comment generated here */
#include <stdio.h>
#include <string.h>
#include "r_okaya_lcd.h"
#include "r_rsk_nvm.h"
#include "r_rsk_switch.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    DATA_ACQ_HALF_SEC,
    DATA_ACQ_ONE_SEC,
    DATA_ACQ_TWO_SEC,
    DATA_ACQ_ONE_MIN

} data_acquisition_period_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
/* result of NVM store */
static uint8_t nvm_store_ok;

/* Prototype declaration for cb_switch_press */
static void cb_switch_press (void);

/* Prototype declaration for get_adc */
static uint16_t get_adc (void);

/* Prototype declaration for lcd_display_time */
static void lcd_display_time (void);

/* Prototype declaration for lcd_display_adc */
static void lcd_display_adc (const uint16_t adc_value);

/* Prototype declaration for snooze_mode */
static void snooze_mode (void);

/* Prototype declaration for snooze_mode */
static void send_nvm (void);

/* local variables to this module */
static char temp_str[64];

static volatile data_acquisition_period_t current_acq_period = DATA_ACQ_ONE_SEC;

volatile uint8_t g_acq_period_count = PERIOD_ONE_SEC_COUNT;

#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

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
    /* Initialize the switch module */
    R_SWITCH_Init();

    /* Set the call back function when SW1 or SW2 is pressed */
    R_SWITCH_SetPressCallback(cb_switch_press);

    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSKRX23T ");
    R_LCD_Display(1, (uint8_t *)" System_Input_Capture ");
    R_LCD_Display(2, (uint8_t *)"  ");

    /* Start the A/D converter */
    R_S12AD_Start();

    /* Start 8bitTimer */
    R_TMR2_Start();

    /* Set up SCI1 receive buffer and callback function */
    R_SCI1_Serial_Receive((uint8_t *)&g_rx_char, 1);

    /* Enable SCI1 operations */
    R_SCI1_Start();

    /* Enable I2C eeprom interface */
    R_RIIC0_Start();

    /* Initialize the NVM */
    nvm_store_ok = R_NVM_Init();

    while (1U)
    {
        /* Variables used to acquire data */
        uint16_t                        adc_value;

        /* Check PC terminal for key presses */
        /* Check the contents of g_rx_char */
        if (('e' == g_rx_char) || ('E' == g_rx_char))
        {
            uint16_t total_records = R_NVM_GetRecordCount();
            g_rx_char = 0;

            /* Erase the NVM */
            R_NVM_Erase();
            nvm_store_ok = TRUE;

            /* Tell the user what we have done */
            sprintf(temp_str, "\r\nErased % d records\r\n", total_records);
            R_SCI1_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

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

        /* g_data_acq_flag gets set by the RTC periodic interrupt */
        if (TRUE == g_data_acq_flag)
        {

            /* Get the ADC value */
            adc_value = get_adc();

            /* Display acquired data on the LCD */
            lcd_display_time();
            lcd_display_adc(adc_value);

            g_data_acq_flag = FALSE;

            /* Store the data in NVM */
            if (TRUE == nvm_store_ok)
            {
                nvm_store_ok = R_NVM_Store(adc_value);
            }
            else
            {
                R_LCD_Display(6, (uint8_t *)" FULL");
            }

            /* Go to snooze if the ADC value drops back below 0x200 */
            if (0x200 > adc_value)
            {
                /* Tell the user we are going into snooze mode */
                R_LCD_Display(3, (uint8_t *)"                     ");
                R_LCD_Display(4, (uint8_t *)"Snooze mode.....     ");
                R_LCD_Display(5, (uint8_t *)"                    ");

                /* Enter snooze mode */
                snooze_mode();
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
* Function Name : cb_switch_press
* Description   : Switch press callback function.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void cb_switch_press (void)
{

    /* Check if switch 1 was pressed */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Increase acquisition period*/
        if (current_acq_period < DATA_ACQ_TWO_SEC)
        {
            current_acq_period++;
        }

        /* Clear flag */
        g_switch_flag = 0x0;
    }
    else if (g_switch_flag & SWITCHPRESS_2)
    {
        /* Decrease acquisition period*/
        if (current_acq_period > DATA_ACQ_HALF_SEC)
        {
            current_acq_period--;
        }

        /* Clear flag */
        g_switch_flag = 0x0;
    }
    else
    {
        /* do nothing */
    }

    /* Stop TMR */
    R_TMR2_Stop();

    R_TMR_ClearCount();

    /* Set the hardware for the required acquisition interval */
    switch (current_acq_period)
    {
        case DATA_ACQ_HALF_SEC:
            g_acq_period_count = PERIOD_HALF_SEC_COUNT;
            break;
        case DATA_ACQ_ONE_SEC:
            g_acq_period_count = PERIOD_ONE_SEC_COUNT;
            break;
        case DATA_ACQ_TWO_SEC:
            g_acq_period_count = PERIOD_TWO_SEC_COUNT;
            break;
        default:

            /* Do nothing */
            break;
    }

    /* Start TMR */
    R_TMR2_Start();

    /* Set the data acq flag, to cause an LCD update before the next wake up */
    g_data_acq_flag = TRUE;
}

/***********************************************************************************************************************
* End of function cb_switch_press
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : get_adc
* Description   : Reads the ADC result,
* Argument      : none
* Return value  : uint16_t adc value
***********************************************************************************************************************/
static uint16_t get_adc (void)
{
    /* A variable to retrieve the adc result */
    uint16_t adc_result;

    /* Stop the A/D converter being triggered from the pin ADTRG0n  */
    R_S12AD_Stop();

    /* Start a conversion */
    R_S12AD_SWTriggerStart();

    /* Wait for the A/D conversion to complete */
    while (FALSE == g_adc_complete)
    {
        /* Wait */
    }

    /* Stop conversion */
    R_S12AD_SWTriggerStop();

    /* Clear ADC flag */
    g_adc_complete = FALSE;

    R_S12AD_Get_ValueResult(ADCHANNEL0, &adc_result);

    /* Set AD conversion start trigger source back to ADTRG0n pin */
    R_S12AD_Start();

    return (adc_result);
}

/***********************************************************************************************************************
* End of function get_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : lcd_display_time
* Description   : function to get the acquisition interval time and display it
*                 on the LCD.
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void lcd_display_time (void)
{
    char interval_str[30];

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
        case DATA_ACQ_TWO_SEC:
            strcat(interval_str, "2 sec  ");
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
* Function Name : lcd_display_adc
* Description   : function to get the current time from the RTC and display it
*                 on the LCD.
* Argument      : uint16_t adc_value
* Return value  : none
***********************************************************************************************************************/
static void lcd_display_adc (const uint16_t adc_value)
{
    char adc_str[20];

    sprintf(adc_str, " ADC: %03XH       ", adc_value);
    R_LCD_Display(4, (uint8_t *)adc_str);

}

/***********************************************************************************************************************
* End of function lcd_display_adc
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: snooze_mode
* Description  : Executes the instruction to send the device into software standby mode but
*                with RTC wake capability.  When awoken, an ADC conversion is performed and if the threshold
*                is still below 0x200 then we re-enter software standby
*
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void snooze_mode (void)
{
    do
    {
        /* Go into sleep mode */
        R_LPC_Sleep();

    } while (get_adc() < 0x200);

    /* When exiting from SoftwareStandby it is required to re-initialize the
     * Flash Memory Controller as FWEPROR is initialized deep SW standby */
    R_NVM_SleepRecover();
}

/***********************************************************************************************************************
* End of function snooze_mode
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name : send_nvm
* Description   : Sends the contents of NVM to the terminal via UART1
*                 formatted in csv strings
* Argument      : none
* Return value  : uint16_t adc value
***********************************************************************************************************************/
static void send_nvm (void)
{
    /* temporary storage for NVM record */
    nvm_record_t   nvm_rec;

    /* Get the NVM record count */
    uint16_t       r;
    const uint16_t total_records = R_NVM_GetRecordCount();

    /* Banner title */
    sprintf(temp_str, "\r\nRenesas Data Logger: Displaying %d records\r\n", total_records);
    R_SCI1_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

    /* Get each of the records in turn and and send to the terminal via USB COM port */
    for (r = 0; r < total_records; r++)
    {
        /* Get the NVM record */
        R_NVM_GetRecord(r, &nvm_rec);

        /* Count and ADC */
        sprintf(temp_str, "%04x,%03x\r\n",nvm_rec.adc_getcount,nvm_rec.adc_value);
        R_SCI1_AsyncTransmit((uint8_t *)temp_str, (uint16_t)strlen(temp_str));

    }

}
/***********************************************************************************************************************
* End of function send_nvm
***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
