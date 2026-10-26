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
* File Name     : timer_event.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Provides timer event catching functions used in this sample.
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
* Project Includes
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Provides declarations of functions defined in this file */
#include "timer_event.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Switch press flag global variable */
extern volatile uint8_t g_switch_flag;

/* Defines count value buffer variable */
uint8_t g_event_count = 0;

/* Declare a variable to hold the periodic delay specified */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global delay count value */
volatile uint32_t g_delay_counter = 0;

/* Declare a flag to indicate the switch has been pressed at least once */
volatile bool     g_display_enable = false;

/* Periodic Delay end callback function */
void (*g_periodic_delay_callback)(void) = NULL;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* CMT2 initialisation function prototype */
static void init_timer (void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                               uint16_t number);

/* timer_delay function prototype */
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void));

/* Display update function */
static void display_update_cb (void);


/*******************************************************************************
* Function Name: init_timer_event
* Description  : The function disables switch 2 interrupts then initialises the
*                TMR3 unit to be clocked from a falling edge produced by the
*                switch SW2. It also configures the CMT timer used to create
*                a 100ms periodic interrupt which calls the display_update_cb
*                callback function.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_timer_event (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Configure pin 6 of port A as an input (used as clock input) */
    PORTA.PDR.BIT.B6 = 0x0;

    /* Configure pin 6 of port A for peripheral function */
    PORTA.PMR.BIT.B6 = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Configure pin 6 of port A for TMCI3 inputs */
    MPC.PA6PFS.BIT.PSEL = 0x5u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the TMR3 module clock stop mode */
    MSTP_TMR3 = 0x0;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0x0;

    /* Enable digital filtering for SW2 pin (IRQ3)*/
    ICU.IRQFLTE0.BIT.FLTEN3 = 0x1u;

    /* Set the sampling clock frequency to PCLK/32=625KHz */
    ICU.IRQFLTC0.BIT.FCLKSEL3 = 0x2u;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Disable SW2 interrupt requests */
    IEN(ICU, IRQ3) = 0x0;

    /* TCNT Counter clearing by compare match A only */
    TMR3.TCR.BIT.CCLR = 0x1u;

    /* Set TCNT clock source to falling edge of external clock */
    TMR3.TCCR.BYTE = 0x02u;

    /* No output after compare match of A or B */
    TMR3.TCSR.BYTE = 0x00;

    /* Set compare match A value  */
    TMR3.TCORA = 0x63u;

    /* Configure a 300 ms periodic delay used
       to update the counter result on to the LCD */
    timer_delay(300u, PERIODIC_MODE, display_update_cb);
}
/*******************************************************************************
* End of function init_timer_event
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

    /* Set CMT2 interrupt priority level to 5*/
    IPR(CMT2, CMI2) = 0x5u;

    /* Enable CMT2 interrupts */
    IEN(CMT2, CMI2) = 0x1u;

    /* Clear CMT2 interrupt flag */
    IR(CMT2, CMI2) = 0x0;

}
/*******************************************************************************
* End of function init_timer
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
#pragma vector=30
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


/*******************************************************************************
* Function Name: display_update_cb
* Description  : This function reads the count value of TMR3, converts it into
*                a BCD character string and displays it on the debug LCD.
* Arguments     : none
* Return Value  : none
********************************************************************************/
static void display_update_cb (void)
{
    /* Fetch timer 3 value */
    g_event_count = TMR3.TCNT;

    /* Set the display enable flag if SW2 has been pressed.
       Note: This must use a flag as g_event_count resets to zero after 99 */
    if (g_event_count > 0x00u)
    {
        g_display_enable = true;
    }

    /* Check if SW2 has been pressed at least once */
    if (g_display_enable)
    {
        /* Declare temporary character string */
        char lcd_buffer[] = "Count: 0";

        /* Convert counter result into a character string, and store in the local
           string lcd_buffer */
        uint16_tobcdstring(lcd_buffer, 3u, (uint16_t)g_event_count);

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }
}
/********************************************************************************
* End of function display_update_cb
********************************************************************************/


/*******************************************************************************
* Function Name: uint16_tobcdstring
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
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
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                                uint16_t number)
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool leading_zero = true;

    /* Local string buffer variable */
    char buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (char) (0x30u + (number % 10));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (5u > digit)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator
           is false */
        if ((buffer_string[digit] != 0x30) || (!leading_zero))
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

