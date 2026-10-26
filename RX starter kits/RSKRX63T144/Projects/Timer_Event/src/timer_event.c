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
* File Name     : timer_event.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Provides timer event catching functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Provides declarations of functions defined in this file */
#include "timer_event.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declare a variable used to count the number of elapsed timer periods */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global delay count value */
volatile uint32_t g_delay_counter = 0;

/* Declare a variable used to store the switch press count */
uint16_t g_event_count;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT2 initialisation function prototype */
static void init_timer(void);

/* timer_delay function prototype */
static void timer_delay(const uint32_t user_delay, const uint8_t unit,
                            const uint8_t timer_mode);

/*******************************************************************************
* Function Name: Init_TimerEvent
* Description  : The function disables switch 2 interrupts then initialises
*                timer 3 of the MTU3 unit, to be clocked from the falling edge
*                of MTCLKA. It also conigures the CMT2 timer used to create
*                a 300ms periodic interrupt, whose handler is used to update the
*                LCD with the switch press count.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_TimerEvent (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Configure Port2_1 as an input (used as clock input) */
    PORT2.PDR.BIT.B1 = 0x0;

    /* Configure Port2_1 for peripheral function */
    PORT2.PMR.BIT.B1 = 0x1;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;

    /* Configure PortB_3 for MTCLKA inputs */
    MPC.P21PFS.BIT.PSEL = 0x2;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the MTU3 module clock stop mode */
    MSTP_MTU3 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Disable SW2 interrupt requests */
    IEN(ICU, IRQ1) = 0x0;

    /* Set TCNT clock source to MTCLKA external clock, clock counted at
       falling edge, no clearing of TCNT counter. */
    MTU3.TCR.BIT.TPSC = 0x6;
    MTU3.TCR.BIT.CKEG = 0x1;
    MTU3.TCR.BIT.CCLR = 0x0;

    /* TCNT Counter clearing by compare match A only. */
    MTU3.TCR.BIT.CCLR = 0x1;

    /* Select normal operation  */
    MTU3.TMDR1.BIT.MD = 0x0;

    /* No output after compare match of A or B */
    MTU3.TIORH.BYTE = 0x00;

    /* Set compare match A value  */
    MTU3.TGRA = 0x63;

    /* Start the timer */
    MTU.TSTRA.BIT.CST3 = 0x1;

    /* Configure a 300 ms periodic delay used
       to update the switch press count on to the LCD */
    timer_delay(300, 'm', PERIODIC_MODE);
}
/*******************************************************************************
* End of function Init_TimerEvent
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

    /* Set CMT2 interrupt priority level to 5*/
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
* Function Name : timer_delay
* Description   : Function used to create delays in milliseconds or
*                 microseconds depending on the user selection for controlling
*                 the debug LCD function's calls.
* Argument      : uint32_t  -   user_delay (Maximum 697 ms, 10922 uS)
*                 int8      -   unit
*                 int8      -   timer_mode
* Return value  : none
*******************************************************************************/
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
/*******************************************************************************
* End of function timer_delay
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_bcd_string
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Argument     : char * string      - Pointer to array to store BCD string
*                uint8_t pos        - Array element to start BCD string from.
*                uint16_t number    - 16bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_bcd_string (char * const output_string, const uint8_t pos,
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
        buffer_string[digit] = (char) (0x30 + (number % 10));

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
* End of function uint16_to_bcd_string
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_CMT2_CMI2
* Description  : CMT2 interrupt handler is called by the CMT period interrupt,
*                every 300ms. The function reads the count value of MTU3
*                channel 3, converts it into a BCD character string and
*                displays it on the debug LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Declare a variable to indicate whether to display count value */
    static bool    display_count = false;

    /* Declare temporary character string */
    char           lcd_buffer[] = "Count: 0";

    /* Fetch switch count value */
    g_event_count = MTU3.TCNT;

    /* Check if SW2 press was pressed */
    if (g_event_count >= 1u)
    {
        /* Set flag to true to allow TCNT updates on the LCD */
        display_count = true;
    }


    /* Check if SW2 was pressed */
    if (true == display_count)
    {
        /* Convert switch press count into a character string, and store in the
           local string lcd_buffer */
        uint16_to_bcd_string(lcd_buffer, 3, g_event_count);

        /* Display the contents of the local string lcd_buffer */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }
}
/*******************************************************************************
* End of function Excep_CMT2_CMI2
*******************************************************************************/

