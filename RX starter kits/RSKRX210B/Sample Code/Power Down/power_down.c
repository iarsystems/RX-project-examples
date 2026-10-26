/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : power_down.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines CPU power down functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* RTC RPDL function definitions */
#include "r_pdl_rtc.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* Low power consumption RPDL function definitions */
#include "r_pdl_lpc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Contains the prototypes of the functions defined in this file */
#include "power_down.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Integer to string function prototype declaration */
static void uint32_ToBCDString(uint8_t *, uint8_t, uint32_t);
/* RTC periodic interrupt callback function prototype */
void CB_RTC_PowerDown(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Variable used to store the LCD display data */
uint8_t lcd_buffer[9];

/*******************************************************************************
* Outline       : Init_PowerDown
* Description   : This function first initialises the RTC for an periodic 
*                 interrupt which will occur every second. The CPU is then 
*                 placed in standby mode. At every second (RTC periodic 
*                 interrupt) the CPU will wake up & execute the RTC periodic
*                 interrupt handler, which updates the time on the LCD. The MCU
*                 will then be placed to standby mode again.    
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_PowerDown(void)
{   
    /* Declare error flag */
    bool err = true;
                
    /* Configure the clock as follows - 
        Initial time - 00:00:00 
        Initial date - 01/01/2011
        Interrupt - every 1 second
        callback function for RTC periodic function - CB_RTCPeriodic_PowerDown */
    err &=  R_RTC_Create
            (
                PDL_RTC_24_HOUR_MODE,
                PDL_NO_DATA,
                0xFF000000, 
                0x20110101, 
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,    
                PDL_RTC_PERIODIC_1_HZ,
                PDL_NO_DATA,    
                PDL_NO_DATA,    
                PDL_NO_FUNC, 
                PDL_NO_DATA,
                CB_RTC_PowerDown,        
                10
            );

    /* Start the clock */
    err &=  R_RTC_Control
            (
                PDL_RTC_CLOCK_START,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA
            );
                    
    /* Configure the registers related to low power consumption modes */
    err &=  R_LPC_Create
            (
                PDL_LPC_HIGH_SPEED_MODE | PDL_LPC_SOFTCUT_HOCO_LVD,
                PDL_NO_DATA,
                PDL_LPC_CANCEL_RTCI_ENABLE,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA
            );

    /* Halt in while loop when RPDL errors detected */
    while(!err);    
}
/*******************************************************************************
* End of function Init_PowerDown
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_RTC_PowerDown
* Description   : Callback function to the periodic interrupt generated every 
*                 second. It is used to update the time on the debug LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_RTC_PowerDown(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Declare a variable for storing RTC statuses */
    uint8_t rtc_status = 0x0;
    uint32_t time_read = 0x0;

    /* Read the time and status flags */
    err &=  R_RTC_Read
            (
                PDL_RTC_READ_CURRENT,
                &rtc_status,
                &time_read,
                PDL_NO_PTR
            );

    /* Check if the periodic interrupt flag was raised 
    and if the clock is running in normal operation */
    if(rtc_status & 0x01)
    {
        /* Convert the read data to character string    */
        uint32_ToBCDString(lcd_buffer, 0, time_read);
        
        /*  Delay to ensure proper functioning of the I/O ports connected to
            LCD after switching from the standby mode */
        err &=  R_CMT_CreateOneShot
                (
                    3, 
                    PDL_CMT_PERIOD, 
                    12e-3, 
                    PDL_NO_FUNC, 
                    0
                );      
        
        /* Update time on the debug LCD */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }
    
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function CB_RTC_PowerDown
*******************************************************************************/

/*******************************************************************************
* Outline      : uint32_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an 
*                argument.
* Argument     : uint8_t * string   - Pointer to array to store BCD string 
*                uint8_t pos        - Array element to start BCD string from.
*                uint32_t number    - 32bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint32_ToBCDString(uint8_t * output_string, uint8_t pos, 
            uint32_t number)
{
    /* Local digit position variable */
    uint8_t disp_ptr=0,digit = 6u;
    
    /* Local string buffer variable */
    uint8_t buffer_string[] = "000000";
    
    /* Decrement digit each iteration, until digit is zero */
    while(digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (uint8_t) (0x30 + (number % 16));
        
        /* Move to the next decimal digit */
        number >>= 4;
    }
    
    /* Set digit position to zero */
    digit = 0u;
    
    /* Loop through each digit, and replace leading zeros with a space
       character */
    while(disp_ptr < 6u)
    {
        /* Copy digit from buffer string to output string */
        output_string[digit + pos] = buffer_string[disp_ptr];
        
        /* Increment the loop counter */
        digit++;
        disp_ptr++;
        
        /* Add colon between hours & minutes and between minutes & seconds. */
        if((digit == 5)||(digit == 2))
        {
            output_string[digit + pos] = ':';   

            /* Increment the loop counter */
            digit++;
        }
    }
}
/*******************************************************************************
* End of function uint32_ToBCDString
*******************************************************************************/
