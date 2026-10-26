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
* File Name     : cac.c
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines CAC configuration and control funcitons
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
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

/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

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

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* init_timer function prototype */
static void init_timer(void);

/* 32 bit integer to character string function prototype declaration */
static void uint32_tobcdstring(char * const output_string, const uint8_t pos,
                                                               uint32_t number);

/* timer_delay function prototype */
static void timer_delay_ms(const uint32_t user_delay, const uint8_t timer_mode);

/*******************************************************************************
* Function Name: Init_CAC
* Description  : CAC initialisation function. This function starts the IWDT
*                clock and sets the CAC to measure the frequency of IWDTCLK,
*                using the PCLKB clock as a reference , then configures a timer
*                for a periodic interrupt every 300 milliseconds.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void Init_CAC (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CAC module clock stop mode */
    MSTP_CAC = 0x0;

    /* Start the IWDT clock */
    SYSTEM.ILOCOCR.BIT.ILCSTP = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set P01 as a peripheral pin */
    PORT0.PMR.BIT.B1 = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;

    /* Set P01 as the CACREF pin */
    MPC.P01PFS.BIT.PSEL = 0x5u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Set the CAC to measure the IWDTCLK */
    CAC.CACR1.BIT.FMCS = 0x4;

    /* Enable the CACREF pin */
    CAC.CACR1.BIT.CACREFE = 0x1;

    /* Select reference clock source (internal by default) */
    CAC.CACR2.BIT.RPS = USE_INTERNAL_CLOCK;

    /* Use the 187.5 KHz PCLKB as a frequency reference */
    CAC.CACR2.BIT.RSCS = 0x5;

    /* Divide the PCLKB reference clock by 8192 = 22.88 Hz */
    CAC.CACR2.BIT.RCDS = 0x3;

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
    timer_delay_ms(300u, PERIODIC_MODE);
}
/*******************************************************************************
* End of function Init_CAC
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
* Function Name: uint32_tobcdstring
* Description  : This function converts an 32bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char *)string   - Pointer to array to store BCD string
*                (uint8_t)pos     - Array element to start BCD string from.
*                (uint32_t)number - 32bit integer to convert.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint32_tobcdstring (char * const output_string, const uint8_t pos,
                                                                uint32_t number)
{
    /* Local digit position variable */
    uint8_t digit = 6u;

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
    while (digit < 6u)
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
* End of function uint32_tobcdstring
*******************************************************************************/

/******************************************************************************
* Function Name : timer_delay_ms
* Description   : Function used to create delays in milliseconds.
* Argument      : uint32_t  -   user_delay (Maximum 2097 ms)
*                 uint8_t   -   timer mode (0 normal, 1 PERIODIC_MODE)
* Return value  : none
******************************************************************************/
static void timer_delay_ms (const uint32_t user_delay, const uint8_t timer_mode)
{
    /* Clear the timer's count */
    g_delay_counter = 0;

    {
        /* Select the PCLK clock division as PCLK/8 = 23.43KHz */
        CMT2.CMCR.BIT.CKS = 0x0;

        /* Store a copy of the user delay value to g_periodic_delay */
        g_periodic_delay = (uint16_t)(user_delay * 23);

        /* Specify the timer period */
        CMT2.CMCOR = g_periodic_delay;
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
* End of function timer_delay_ms
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMT2_CMI2
* Description  : CMT2 interrupt handler.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Declare temporary character string */
    char lcd_buffer[9] = "      Hz";

    /* Fetch and convert the count value into frequency in Hz */
    const uint32_t freq_hz = (uint32_t)((float)CAC.CACNTBR * FREQ_CONVERSION);

    /* Convert frequency result into a character string, and store in the
       local string lcd_buffer */
    uint32_tobcdstring(lcd_buffer, 0, freq_hz);

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);
}
/******************************************************************************
* End of function Excep_CMT2_CMI2
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CAC_FERRF
* Description  : Frequency error interrupt handler. Prints a message to the
*                Debug LCD and clears the error flag.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CAC_FERRF
__interrupt  void Excep_CAC_FERRF(void)
{
    /* Print a message to the Debug LCD */
    Display_LCD(LCD_LINE2, "Freq Err");

    /* Clear the frequency error flag */
    CAC.CAICR.BIT.FERRFCL = 0x1u;

}
/******************************************************************************
* End of function Excep_CAC_FERRF
******************************************************************************/

