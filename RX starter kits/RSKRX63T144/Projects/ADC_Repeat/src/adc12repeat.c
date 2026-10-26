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
* File Name     : adc12repeat.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC repeat functions used for 12bit ADC unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Defines
*******************************************************************************/
/* Define a macro to indicate selection of a periodic timer mode */
#define PERIODIC_MODE 0x1

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "adc12repeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/* Declare a variable used to specify if the delay
function should only generate the specific delay or not */
uint8_t g_timer_mode_flag = 0;

/* Dclare a variable to hold the periodic delay specified */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global delay count value */
volatile uint32_t g_delay_counter = 0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* init_timer function prototype */
static void init_timer(void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string(char * const output_string, uint8_t pos,
                                    const uint16_t input_number);

/* timer_delay function prototype */
static void timer_delay(const uint32_t user_delay, const uint8_t unit,
                                    const uint8_t timer_mode);

/*******************************************************************************
* Function Name : Init_ADC12Repeat
* Description   : ADC initialisation function. This function configures the ADC
*                 unit for continuous scan operation, then configures a timer
*                 for a periodic interrupt every 300 milliseconds.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_ADC12Repeat (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */

    /* Enable Group B scan end Interrupt */
    S12AD.ADCSR.WORD |= 0x40;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001;

    /* Set ADCSR to continuous mode */
    S12AD.ADCSR.BIT.ADCS = 0x2;

    /* Set the sampling cycle to 255 states (approximately 5 us) */
    S12AD.ADSSTR0 = 0xFF;

    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 1;

    /* Configure a 300 ms periodic delay used
       to update the ADC result on to the LCD */
    timer_delay(300, 'm', PERIODIC_MODE);
}
/*******************************************************************************
* End of function Init_ADC12Repeat
*******************************************************************************/

/*******************************************************************************
* Function Name: init_timer
* Description  : This function initialises the CMT channel 2.
* Argument     : none
* Return value : none
*******************************************************************************/
static void init_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set CMT2 interrupt priority level to 5 */
    IPR(CMT2,CMI2) = 0x5;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;

    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;

}
/*******************************************************************************
* End of function init_timer
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold
*                                  character string.
*                            pos : uint8_t number, element number to begin
*                                  inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_string (char * const output_string, uint8_t pos,
                                const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a         = 0x00;
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask      = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (char)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/

/******************************************************************************
* Function Name : timer_delay
* Description   : Function used to create delays in milliseconds or
*                 microseconds depending on the user selection for controlling
*                 the debug LCD function's calls.
* Argument      : uint32_t  -   user_delay (Maximum 697 ms, 10922 uS)
*                 uint8     -   Unit
* Return value  : none
******************************************************************************/
static void timer_delay (const uint32_t user_delay, const uint8_t unit,
                            const uint8_t timer_mode)
{
    /* Clear the timer's count */
    g_delay_counter = 0;

    /* Check if microseconds delay is required */
    if (unit == 'u')
    {
        /* Select the PCLK clock division as PCLK/8 = 6MHz */
        CMT2.CMCR.BIT.CKS = 0x0;

        /* Store a copy of the user delay value to g_periodic_delay */
        g_periodic_delay = user_delay * 6;

        /* Specify the timer period */
        /* Cast required to avoid warning:
        Implicit conversion: unsigned long to unsigned short.  */
        CMT2.CMCOR = (uint16_t)g_periodic_delay;
    }

    /* Check if milliseconds delay is required */
    if (unit == 'm')
    {
        /* Select the PCLK clock division as PCLK/512 = 93.75KHz */
        CMT2.CMCR.BIT.CKS = 0x3;

        /* Store a copy of the user delay value to g_periodic_delay */
        g_periodic_delay = user_delay * 94;

        /* Specify the timer period */
        /* Implicit conversion: unsigned long to unsigned short.  */
        CMT2.CMCOR = (uint16_t)g_periodic_delay;
    }

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;

    /* Skip the following instructions if a periodic timer is required */
    if (PERIODIC_MODE != timer_mode)
    {
        /* Wait for the timer to timeout */
        while ((g_delay_counter != g_periodic_delay))
        {
            /* Wait */
        }

        /* Stop CMT2 count */
        CMT.CMSTR1.BIT.STR2 = 0;
    }
}
/******************************************************************************
* End of function timer_delay
******************************************************************************/

/******************************************************************************
* Function Name : Excep_CMT2_CMI2
* Description   : CMT2 interrupt handler.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Declare temporary character string */
    char lcd_buffer[9] = "=H\'WXYZ\0";

    /* Fetch ADC value */
    g_adc_result = S12AD.ADDR0;

    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */
    uint16_to_string(lcd_buffer, 3, g_adc_result);

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);
}
/******************************************************************************
* End of function Excep_CMT2_CMI2
******************************************************************************/

