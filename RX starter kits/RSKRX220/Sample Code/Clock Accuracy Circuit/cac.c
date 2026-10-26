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
* File Name     : cac.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines CAC configuration and control funcitons
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Defines
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides boolean type definitions */
#include <stdbool.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "cac.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

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
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                               uint16_t number);

/* timer_delay function prototype */
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void));

/* Display update callback */
static void display_update_cb (void);

/*******************************************************************************
* Function Name: init_cac
* Description  : CAC initialisation function. This function starts the HOCO at
*                32MHz and sets the CAC to measure its frequency, using the
*                main clock as a reference clock, then configures a timer for
*                a periodic interrupt every 300 milliseconds.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_cac (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CAC module clock stop mode */
    MSTP_CAC = 0x0;

    /* Set the HOCO to oscillate at 32MHz */
    SYSTEM.HOCOCR2.BIT.HCFRQ = 0x0;

    /* Start oscillating the HOCO */
    SYSTEM.HOCOCR.BIT.HCSTP = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set PA0 as a peripheral pin */
    PORTA.PMR.BIT.B0 = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Set PA0 as the CACREF pin */
    MPC.PA0PFS.BIT.PSEL = 0x7u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Set the CAC to measure the HOCO */
    CAC.CACR1.BIT.FMCS = 0x2u;

    /* Enable the CACREF pin */
    CAC.CACR1.BIT.CACREFE = 0x1u;

    /* Select reference clock source (internal by default) */
    CAC.CACR2.BIT.RPS = USE_INTERNAL_CLOCK;

    /* Use the main clock oscillator as a frequency reference */
    CAC.CACR2.BIT.RSCS = 0x0;

    /* Divide the reference clock by 8192 */
    CAC.CACR2.BIT.RCDS = 0x3u;

    /* Set upper frequency error value */
    CAC.CAULVR = FREQ_ERROR_UPPER;

    /* Set lower frequency error value */
    CAC.CALLVR = FREQ_ERROR_LOWER;

    /* Enable the frequency error interrupt */
    CAC.CAICR.BIT.FERRIE = 0x1u;

    /* Set frequency error interrupt priority to 7 */
    IPR(CAC,FERRF) = 0x7u;

    /* Clear the frequency error interrupt flag */
    IR(CAC,FERRF) = 0x0;

    /* Enable frequency error interrupt requests */
    IEN(CAC,FERRF) = 0x1u;

    /* Start the CAC measurement */
    CAC.CACR0.BIT.CFME = 0x1u;

    /* Configure a 300 ms periodic delay used
       to update the frequency result on to the LCD */
    timer_delay(300u, PERIODIC_MODE, display_update_cb);
}
/*******************************************************************************
* End of function init_cac
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
* Function Name: uint16_tobcdstring
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char *)string   - Pointer to array to store BCD string
*                (uint8_t)pos     - Array element to start BCD string from.
*                (uint16_t)number - 16bit integer to convert.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                                uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool    leading_zero = true;

    /* Local string buffer variable */
    char    buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30u + (number % 10u));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (digit < 5u)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator
           is false */
        if ((0x30 != buffer_string[digit]) || (!leading_zero))
        {
            /* Copy digit from buffer string to output string */
            output_string[digit + pos] = buffer_string[digit];

            /* Set the leading zero indicator variable to false */
            leading_zero = false;
        }

        /* Increment the loop counter */
        digit++;
    }
}
/*******************************************************************************
* End of function uint16_tobcdstring
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
        while ((g_delay_counter != g_periodic_delay))
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
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMTU1_CMT2_isr(void)
{
    /* Increment counter */
    g_delay_counter++;

    /* If period has been reached and periodic mode is set,
       run the callback and reset the counter */
    if ((g_periodic_delay < g_delay_counter) && g_periodic_delay_callback)
    {
        g_delay_counter = 0x0;

        /* This was checked to be not NULL above */
        g_periodic_delay_callback();
    }

}
/******************************************************************************
* End of function Excep_CMTU1_CMT2_isr
******************************************************************************/

/******************************************************************************
* Function Name: display_update_cb
* Description  : Read the current accuracy count value, calculate the clock
*                frequency and update the display.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void display_update_cb (void)
{
    /* Declare temporary character string */
    char lcd_buffer[9] = "     kHz";

    /* Fetch and convert the count value into frequency in Hz */
    const uint32_t freq_hz = (uint32_t)(CAC.CACNTBR * FREQ_CONVERSION);

    /* Convert into kHz for display */
    const uint16_t freq_khz = (uint16_t) (freq_hz / 1000);

    /* Convert frequency result into a character string, and store in the
       local string lcd_buffer */
    uint16_tobcdstring(lcd_buffer, 0, freq_khz);


    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);
}
/******************************************************************************
* End of function display_update_cb
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CAC_FERRF_isr
* Description  : Frequency error interrupt handler. Prints a message to the
*                Debug LCD and clears the error flag.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CAC_FERRF
__interrupt  void Excep_CAC_FERRF_isr(void)
{
    /* Print a message to the Debug LCD */
    Display_LCD(LCD_LINE2, "Freq Err");

    /* Clear the frequency error flag */
    CAC.CAICR.BIT.FERRFCL = 0x1u;

}
/******************************************************************************
* End of function Excep_CAC_FERRF_isr
******************************************************************************/

