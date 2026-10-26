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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : rtc.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines RTC controlling functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides standard boolean type definitions */
#include <stdbool.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "rtc.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Variable used to store the string to be displayed on the LCD */
char lcd_buffer[9];

/* Variable used to hold current time */
uint32_t time_data = 0x0;

/* Variable used to hold current time */
uint32_t date_data = 0x0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Integer to string function prototype declaration */
static void uint32_to_bcd_string (char * const output_string,
                                            const uint8_t pos, uint32_t number);

/* Subclock configuration function prototype declaration */
static void configure_subclock (void);

/*******************************************************************************
* Function Name: init_rtc
* Description  : The RTC initialisation function. This function presets the
*                time & date of the RTC. It configures the RTC to generate
*                an interrupt every second. The alarm is also configured to
*                trigger at a specified time.
* Argument     : none
* Return value : none
*******************************************************************************/
void init_rtc (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Check if the MCU has come from a cold start (power on reset) */
    if (0x0 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        /* Set the warm start flag */
        SYSTEM.RSTSR1.BIT.CWSF = 0x1u;

        /* Configure and start the sub-clock oscillator */
        configure_subclock();

    }
    else
    {
        /* Start sub-clock */
        SYSTEM.SOSCCR.BIT.SOSTP = 0x0;

        /* Wait for the register modification to complete */
        while (0x0 != SYSTEM.SOSCCR.BIT.SOSTP)
        {
        }
    }

    /* Supply sub-clock to RTC module */
    RTC.RCR3.BIT.RTCEN = 0x1u;

    /* Wait for at least 5 cycles of sub-clock */
    volatile uint32_t wait = 0x6000u;
    while (wait--)
    {
        /* Do nothing until sub-clock startup is complete */
    }

    /* It is now safe to set the RTC registers */

    /* Write 0 to RTC start bit */
    RTC.RCR2.BIT.START = 0x0;

    /* Wait for start bit to clear */
    while (0x0 != RTC.RCR2.BIT.START)
    {
        /* Do nothing until start bit is clear */
    }

    /* Set the RTC to calendar count mode */
    RTC.RCR2.BIT.CNTMD = 0x0;

    /* Wait for count mode bit to clear */
    while (0x0 != RTC.RCR2.BIT.CNTMD)
    {
    }

    /* Reset the RTC unit */
    RTC.RCR2.BIT.RESET = 0x1u;

    /* Wait until reset is complete */
    while (RTC.RCR2.BIT.RESET)
    {
        /* Do nothing until reset is complete */
    }

    /* Write 0 to RTC start bit */
    RTC.RCR2.BIT.START = 0x0;

    /* Wait for start bit to clear */
    while (0x0 != RTC.RCR2.BIT.START)
    {
        /* Do nothing until start bit is clear */
    }

    /* Operate RTC in 24-hr mode */
    RTC.RCR2.BIT.HR24 = 0x1u;

    /* Disable automatic adjustment */
    RTC.RCR2.BIT.AADJP = 0x0;
    RTC.RCR2.BIT.AADJE = 0x0;

    /* Configure the clock as follows -
        Initial time - 11:59:30     */
    RTC.RSECCNT.BYTE = 0x30u;
    RTC.RMINCNT.BYTE = 0x59u;
    RTC.RHRCNT.BYTE = 0x11u;

    /* Configure the date as follows -
        Initial date - 21/11/2011   */
    RTC.RDAYCNT.BYTE = 0x21u;
    RTC.RMONCNT.BYTE = 0x11u;
    RTC.RYRCNT.WORD = 0x0011u;

    /* Configure the alarm as follows -
        Alarm time - 12:00:00
        Enable the hour, minutes and seconds alarm */
    RTC.RSECAR.BYTE = 0x80u;
    RTC.RMINAR.BYTE = 0x80u;
    RTC.RHRAR.BYTE = 0xD2u;

    /* Alarm enable bits are undefined after a reset,
       disable non-required alarm features */
    RTC.RWKAR.BIT.ENB = 0x0;
    RTC.RDAYAR.BIT.ENB = 0x0;
    RTC.RMONAR.BIT.ENB = 0x0;
    RTC.RYRAREN.BIT.ENB = 0x0;

    /* Enable alarm and periodic interrupts,
    generate periodic interrupts every 1 second */
    RTC.RCR1.BYTE = 0xE5u;

    /* Verify RCR1 register write */
    while (0xE5u != RTC.RCR1.BYTE)
    {
        /* Do nothing until write completes */
    }

    /* Start the clock */
    RTC.RCR2.BIT.START = 0x1u;

    /* Wait until the start bit is set to 1 */
    while (0x1 != RTC.RCR2.BIT.START)
    {
    }

    /* Set LED1 port pin to output */
    LED1_PORT_DIR = 0x1u;

    /* Turn off LED1 */
    LED1 = LED_OFF;

    /* Enable RTC Alarm interrupts */
    IPR(RTC,ALM) = 0x0Au;
    IEN(RTC,ALM) = 0x1u;
    IR(RTC,ALM) = 0x0;

    /* Enable RTC Periodic interrupts */
    IPR(RTC,PRD) = 0x07u;
    IEN(RTC,PRD) = 0x1u;
    IR(RTC,PRD) = 0x0;
}
/*******************************************************************************
* End of function init_rtc
*******************************************************************************/

/*******************************************************************************
* Function Name: configure_subclock
* Description  : Restarts the sub-clock oscillator and waits for oscillation to
*                stabilise.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void configure_subclock (void)
{
    /* Indicate waiting for subclock to stablise on LCD */
    Display_LCD(LCD_LINE1, "Init Clk");

    /* Disable the sub-clock oscillator */
    SYSTEM.SOSCCR.BIT.SOSTP = 0x1u;

    /* Wait for register modification to complete */
    while (0x1 != SYSTEM.SOSCCR.BIT.SOSTP)
    {
        /* Do nothing until register is updated */
    }

    /* Disable the input from the sub-clock */
    RTC.RCR3.BYTE = 0x0Cu;

    /* Wait for the register modification to complete */
    while (0x0 != RTC.RCR3.BIT.RTCEN)
    {
        /* Do nothing until register is updated */
    }

    /* Wait for at least 5 cycles of sub-clock */
    volatile uint32_t wait = 0x6000u;
    while (wait--)
    {
        /* Do nothing until sub-clock is ready */
    }

    /* Start sub-clock */
    SYSTEM.SOSCCR.BIT.SOSTP = 0;

    /* Declare progress bar string */
    char progress[] = "        ";

    /* Perform 8 delay iterations */
    for (uint8_t i = 0; i < 8u; i++)
    {
        /* Wait in while loop for ~0.5 seconds */
        wait = 0xDB000u;
        while (wait--)
        {
            /* Do nothing for ~0.5 seconds */
        }

        /* Add block character to progress bar string */
        progress[i] = 0xFFu;

        /* Update progress bar string */
        Display_LCD(LCD_LINE2, progress);
    }
}
/*******************************************************************************
* End of function configure_subclock
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_RTC_ALM_isr
* Description  : RTC alarm interrupt function generated when the time set in
*                the alarm registers matches with the current RTC time.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_RTC_ALM
__interrupt  void Excep_RTC_ALM_isr(void)
{
    /* Turn on LED1 to indicate that the alarm is triggered */
    LED1 = LED_ON;

    /* Clear the interrupt flag */
    IR(RTC,ALM) = 0x0;
}
/*******************************************************************************
* End of function Excep_RTC_ALM_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_RTC_SLEEP_isr
* Description  : RTC periodic interrupt handler generated every 1 sec. It is
*                used to update the time on the debug LCD. It read the RTC
*                register values and converts them into a string before
*                displaying on the LCD.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_RTC_PRD
__interrupt  void Excep_RTC_SLEEP_isr(void)
{
    /* Turn on LED0 to indicate CPU activity */
    LED0 = LED_ON;

    /* Read the time and status flags */
    /* Read the seconds count register */
    time_data  = (uint32_t)(RTC.RSECCNT.BYTE & 0x0000007Fu);

    /* Read the minutes count register */
    time_data |= (RTC.RMINCNT.BYTE & 0x0000007Fu) << 8u;

    /* Read the hours count register */
    time_data |= (RTC.RHRCNT.BYTE  & 0x0000003Fu) << 16u;

    /* Convert the data to string & display on the LCD */
    uint32_to_bcd_string(lcd_buffer, 0, time_data);

    /* Update time on the debug LCD */
    Display_LCD(LCD_LINE2, lcd_buffer);

    /* Wait a short duration to extend LED toggle period */
    uint32_t wait = 0xFFu;
    while (wait--)
    {
        /* Do nothing until wait time elapsed */
    }

    /* Turn LED0 off to indicate end of clock update */
    LED0 = LED_OFF;
}
/*******************************************************************************
* End of function Excep_RTC_SLEEP_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: uint32_to_bcd_string
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char * const)string - Pointer to array to store BCD string
*                (const uint8_t)pos   - Array element to start BCD string from.
*                (uint32_t)number     - 32bit integer to convert.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint32_to_bcd_string (char * const output_string,
                                             const uint8_t pos, uint32_t number)
{
    /* Local digit position variable */
    uint8_t disp_ptr = 0;
    uint8_t digit = TIME_DIGITS;

    /* Local string buffer variable */
    char buffer_string[TIME_DIGITS + 1] = "000000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30u + (number % 16u));

        /* Move to the next decimal digit */
        number >>= 4u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (disp_ptr < TIME_DIGITS)
    {
        /* Copy digit from buffer string to output string */
        output_string[digit + pos] = buffer_string[disp_ptr];

        /* Increment the loop counter */
        digit++;
        disp_ptr++;

        /* Check the digit position. */
        if ((5u == digit) || (2u == digit))
        {
            /* Add colon between hours and minutes
               and between minutes & seconds. (hh:mm:ss) */
            output_string[digit + pos] = ':';

            /* Increment the loop counter */
            digit++;
        }
    }
}
/*******************************************************************************
* End of function uint32_to_bcd_string
*******************************************************************************/

