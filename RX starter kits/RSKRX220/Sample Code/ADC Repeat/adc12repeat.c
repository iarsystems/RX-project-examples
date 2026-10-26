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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : adc12repeat.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines ADC repeat functions used for 12bit ADC unit.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Defines
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Provides declarations of functions defined in this file */
#include "adc12repeat.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/* Declare a variable used to specify if the delay
function should only generate the specific delay or not */
uint8_t g_timer_mode_flag = 0x0;

/* Declare a variable to hold the periodic delay specified */
volatile uint16_t g_periodic_delay;

/* Declare a variable to store the global delay count value */
volatile uint32_t g_delay_counter = 0x0;

/* Periodic Delay end callback function */
void (*g_periodic_delay_callback)(void) = NULL;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* init_timer function prototype */
static void init_timer (void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_tostring (char * const output_string, uint8_t pos,
                                                   const uint16_t input_number);

/* timer_delay function prototype */
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void));

/* Display update callback */
static void display_update_cb (void);

/*******************************************************************************
* Function Name: init_adc12_repeat
* Description  : ADC initialisation function. This function configures the ADC
*                unit for continuous scan operation, then configures a timer
*                for a periodic interrupt every 300 milliseconds.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_adc12_repeat (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;

    /* Set continuous scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x2u;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u;

    /* Set the sampling cycle to 255 states (approximately 5 us) */
    S12AD.ADSSTR0 = 0xFFu;

    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 0x1u;

    /* Configure a 300 ms periodic delay used
       to update the ADC result on to the LCD */
    timer_delay(300u, PERIODIC_MODE, display_update_cb);
}
/*******************************************************************************
* End of function init_adc12_repeat
*******************************************************************************/

/*******************************************************************************
* Function Name: init_timer
* Description  : This function initialises the CMT channel 2.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set CMT2 interrupt priority level to 5 */
    IPR(CMT2,CMI2) = 0x5u;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1u;

    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;

}
/*******************************************************************************
* End of function init_timer
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_tostring
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Arguments    : (char *)output_string        - Pointer to char array that will
*                                               hold character string.
*                (uint8_t)pos                 - uint8_t number, element number
*                                               to begin inserting the character
*                                               string from (offset).
*                (const uint16_t)input_number - 16 bit integer to convert into
*                                               a string.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_tostring (char * const output_string, uint8_t pos,
                                                    const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a = 0x00;
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;

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
* End of function uint16_tostring
*******************************************************************************/

/******************************************************************************
* Function Name: timer_delay
* Description  : Function used to create delays in milliseconds for controlling
*                the debug LCD function's calls.
* Arguments    : (const uint32_t)user_delay - Timer length in milliseconds.
*                (uint8_t)timer_mode        - Sets timer to run continuously
*                                             and start a callback on timeout
*                                             (PERIODIC_MODE macro) or run once
*                                             and return after delay (0).
*                (void(*))callback          - Callback function for periodic
*                                             timer.
* Return Value : none
******************************************************************************/
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void))
{
    /* Clear the timer's count */
    g_delay_counter = 0x0;

    /* Select the PCLK clock division as PCLK/32 = 625KHz */
    CMT2.CMCR.BIT.CKS = 0x1u;

    /* Store a copy of the user delay value to g_periodic_delay */
    g_periodic_delay = user_delay;

    /* Set the callback */
    if (callback)
    {
        g_periodic_delay_callback = callback;
    }

    /* Specify the timer period (1ms) */
    CMT2.CMCOR = 625u;

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x1u;

    /* Skip the following instructions if a periodic timer is required */
    if (PERIODIC_MODE != timer_mode)
    {
        /* Wait for the timer to timeout */
        while (g_delay_counter != g_periodic_delay)
        {
            /* Wait */
        }

        /* Stop CMT2 count */
        CMT.CMSTR1.BIT.STR2 = 0x0;
    }
}
/******************************************************************************
* End of function timer_delay
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMTU1_CMT2_isr
* Description  : CMT2 interrupt handler.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=30
__interrupt  void Excep_CMTU1_CMT2_isr(void)
{
    /* Increment counter */
    g_delay_counter++;

    /* If period has been reached and periodic mode is set,
       run the callback and reset the counter */
    if ((g_periodic_delay < g_delay_counter) && g_periodic_delay_callback)
    {
        g_delay_counter = 0;

        /* This was checked to be not NULL above */
        g_periodic_delay_callback();
    }

}
/******************************************************************************
* End of function Excep_CMTU1_CMT2_isr
******************************************************************************/

/******************************************************************************
* Function Name: display_update_cb
* Description  : Read the current ADC value and update the display
* Arguments    : none
* Return Value : none
******************************************************************************/
static void display_update_cb (void)
{
    /* Declare temporary character string */
    char lcd_buffer[9] = "=H\'WXYZ\0";

    /* Fetch ADC value */
    g_adc_result = S12AD.ADDR0;

    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */
    uint16_tostring(lcd_buffer, 3u, g_adc_result);

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);
}
/******************************************************************************
* End of function display_update_cb
******************************************************************************/


