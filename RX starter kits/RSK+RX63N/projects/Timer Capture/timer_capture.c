/* Adapted for use with IAR Embedded Workbench */
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
* ELECTRONICS CORPORATION NOR A NY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
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
* File Name     : timer_capture.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>
/* Following header file provides Boolen type definitions */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Defines RX63N port registers */
#include "iorx63n.h"
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Switch handler function definitions */
#include "switch.h"
/* Declares function prototypes defined in this file */
#include "timer_capture.h"

/******************************************************************************
* Global Variables
******************************************************************************/
/* Static global TPU overflow counter variable */
static volatile uint16_t gOverflowCount = 0;

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* Switch press callback function prototype */
void SwitchPressCB_TimerCapture(void);
/* Switch release callback function prototype */
void SwitchReleaseCB_TimerCapture(void);
/* 16 bit integer to character string function prototype declaration */
static void u16ToBCDString(uint8_t *, uint8_t, uint16_t);

/*******************************************************************************
* Outline       : Init_TimerCapture
* Description   : Function configures TPU3 for input capture used to measure the
*                  interval between the falling and rising edge detected on the
*                  TIOCC3 pin. The TIOCC3 is configured as an input pin and set
*                  for peripheral use. The timer clock source is configured as
*                  PCLK/4096 (11.718KHz). The function also configures switch
*                 interrupt callback functions.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_TimerCapture(void)
{
    /* Configure pin 2 of port 2 as an input */
    PORT2.PDR.BIT.B2 = 0;

    /* Configure pin 2 of port 2 for peripheral function */
    PORT2.PMR.BIT.B2 = 1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00u;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40u;

    /* Configure pin 2 of port 2 for TIOCC3 inputs */
    MPC.P22PFS.BIT.PSEL = 0x3u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80u;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;

    /* Cancel the MTU3 module clock stop mode */
    MSTP_TPU3 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;

    /* Set TGI3C interrupt priority level to 7 */
    IPR(TPU3, TGI3C) = 0x7;
    /* Enable TGI3C interrupts */
    IEN(TPU3, TGI3C) = 0x1;
    /* Clear TGI3C IR flag */
    IR(TPU3, TGI3C) = 0x0;

    /* Set ICU group 4 interrupt priority level to 7 */
    IPR(ICU, GROUP4) = 0x7;
    /* Enable ICU group 4 interrupts */
    IEN(ICU, GROUP4) = 0x1;
    /* Clear ICU group 4 IR flag */
    IR(ICU, GROUP4) = 0x0;
    /* Enable TPU3 overflow interrupts to trigger group 4 interrupt */
    ICU.GEN[4].BIT.EN2 = 1;

    /* Select PCLK/4096=11.718KHz, clock counted at
       falling edge, no clearing of TCNT counter. */
    TPU3.TCR.BYTE = 0x07u;

    /* Select normal operation and TIOCA3 pin
       as the TGRC input capture input source */
    TPU3.TMDR.BYTE = 0x80;

    /* Enable noise filter */
    TPU3.NFCR.BIT.NFBEN = 1u;

    /* Configure TPU3 incapture to trigger on rising and failling edge of
       TIOCC4 pin */
    TPU3.TIORL.BYTE = 0x0Au;

    /* Enable the timer TPU3 to start running */
    TPUA.TSTR.BIT.CST3 = 1u;

    /* Enable TIOCC3 input capture interrupts */
    TPU3.TIER.BIT.TGIEC = 1u;

    /* Enable TPU3 counter overflow interrupts */
    TPU3.TIER.BIT.TCIEV = 1u;

    /* Set pin D0 (IO0) to be an output */
    PORTD.PDR.BIT.B0 = 1;

    /* Set the switch press down callback function */
    SetSwitchPressCallback(SwitchPressCB_TimerCapture);

    /* Set the switch release callback function */
    SetSwitchReleaseCallback(SwitchReleaseCB_TimerCapture);
}
/*******************************************************************************
* End of function Init_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline       : SwitchPressCB_TimerCapture
* Description   : Switch press down callback function. This function checks if
*                 switch SW1 has been held down, and if so it sets the IO0 pin
*                 high.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SwitchPressCB_TimerCapture(void)
{
    /* Check if switch SW1 is held down */
    if( SWITCHHOLD_1 == (gSwitchFlag & 0x0Fu) )
    {
        /* Set the IO0 pin high */
        PORTD.PODR.BIT.B0 = 1u;
    }
}
/*******************************************************************************
* End of function SwitchPressCB_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline       : SwitchReleaseCB_TimerCapture
* Description   : Switch press callback function. This function checks if switch
*                 SW1 has been released, and if so it sets the IO0 pin low.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SwitchReleaseCB_TimerCapture(void)
{
    /* Check if switch SW1 has been released */
    if( SWITCHPRESS_1 == (gSwitchFlag & 0xF0u) )
    {
        /* Set the IO0 pin low */
        PORTD.PODR.BIT.B0 = 0;
    }
}
/*******************************************************************************
* End of function SwitchReleaseCB_TimerCapture
*******************************************************************************/

/*******************************************************************************
* Outline       : Excep_GROUP_E4
* Description   : Group 4 interrupt service routine. This function is triggered
*                 when the timer TPU3 overflows. The function checks if IO0 pin
*                 is high or low. It its high, it increments the overflow
*                 counter global variable (allows counting past the 16bit timer
*                 limit).
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector = VECT_ICU_GROUP4
__interrupt void Excep_GROUP_E4(void)
{
    /* Check if the IO0 pin is high */
    if(PORT2.PIDR.BIT.B2)
    {
        /* Increment the overflow counter global variable */
        gOverflowCount++;
    }

    /* Clear the TPU3 overflow flag */
    TPU3.TSR.BIT.TCFV = 0;

    /* Cleat the group 4 IR flag */
    ICU.GCR[4].BIT.CLR2 = 1;
}
/*******************************************************************************
* End of function Excep_GROUP_E4
*******************************************************************************/

/*******************************************************************************
* Outline      : Excep_TPU3_TGI3C
* Description  : TPU3 input capture interrupt service routine. This function
*                checks if IO0 is high or low. It its high (switch press has just
*                started), it fetches the start count value from the TGRC
*                register. If IO0 is low (switch press has just ended), it
*                fetches the end count value from the TGRC register and
*                calculates the press duration in milliseconds. This is then
*                displayed on the debug LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_TPU3_TGI3C
__interrupt void Excep_TPU3_TGI3C(void)
{
    /* Declare static start count container variable */
    static uint16_t start_count = 0;

    /* Declare static end count container variable */
    static uint16_t end_count = 0;

    /* Check pin level (to indicate switch press or release) */
    if(PORT2.PIDR.BIT.B2)
    {
        /* Fetch the start count value from the TGRC register */
        start_count = TPU3.TGRC;
    }
    else
    {
        /* Fetch the end count value from the TGRC register */
        end_count = TPU3.TGRC;

        /* Declare container for the switch press duration in ms */
        uint16_t duration = 0;

        /* Check if no overflow occured (end count will be greater than
           start count) */
        if(0 == gOverflowCount)
        {
            /* Declare temporary count variable, set it to end count minus
               the start count */
            uint32_t count =  (uint32_t) end_count - start_count;

            /* Set the duration by dividing count by the frequency */
            duration = (uint16_t) (count / TPU3_FREQ_KHZ);

            /* Declare lcd buffer string */
            uint8_t lcd_buffer[9u] = "      ms";

            /* Convert duration in ms into bcd string, and insert into
               LCD buffer string */
            u16ToBCDString(lcd_buffer, 1, duration);

            /* Display switch press duration on the debug LCD */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }
        /* Check if 1 overflow has occured. Start and end count are either
           side of an overflow, but a full overflow count has not occured
           between */
        else if(1 == gOverflowCount)
        {
            /* Declare temporary count variable. Add difference between
               max timer count and start count, then add the end count */
            uint32_t count = (uint32_t) (65536 - start_count) + end_count;

            /* Set the duration by dividing count by the frequency */
            duration = (uint16_t) (count / TPU3_FREQ_KHZ);

            /* Declare lcd buffer string */
            uint8_t lcd_buffer[9u] = "      ms";

            /* Convert duration in ms into bcd string, and insert into
               LCD buffer string */
            u16ToBCDString(lcd_buffer, 1, duration);

            /* Display switch press duration on the debug LCD */
            Display_LCD(LCD_LINE2, lcd_buffer);
        }
        /* Multiple overflow counts have occured.  */
        else
        {
            /* Declare temporary count variable. Add difference between
               max timer count and start count, then add the end count */
            uint32_t count = (uint32_t) (65536 - start_count) + end_count;

            /* Since multiple overflows have occured, the overflow count
               can be OR'd into the top 16bits of the 32bit count value */
            count |= (uint32_t)gOverflowCount << 16u;

            /* Check if the count value results in a duration greater than
               60 seconds */
            if( (TPU3_FREQ_KHZ*1000*60) < count )
            {
                /* Indicate on the debug LCD that the count value is greater
                   than 60 seconds */
                Display_LCD(LCD_LINE2, "> 60 sec");
            }
            /* Count value results in a duration less than 60 seconds */
            else
            {
                /* Set the duration by dividing count by the frequency */
                duration = (uint16_t) (count / TPU3_FREQ_KHZ);

                /* Declare lcd buffer string */
                uint8_t lcd_buffer[9u] = "      ms";

                /* Convert duration in ms into bcd string, and insert into
                   LCD buffer string */
                u16ToBCDString(lcd_buffer, 1, duration);

                /* Display switch press duration on the debug LCD */
                Display_LCD(LCD_LINE2, lcd_buffer);
            }
        }

        /* Clear the overflow count value back to zero */
        gOverflowCount = 0;
    }
}
/*******************************************************************************
* End of function Excep_TPU3_TGIB3
*******************************************************************************/

/*******************************************************************************
* Outline      : u16ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*                 which is inserted into the character array passed as an
*                 argument.
* Argument     : uint8_t * string    - Pointer to array to store BCD string
*                 uint8_t pos        - Array element to start BCD string from.
*                 uint16_t number    - 16bit integer to convert.
* Return value : none
* Note           : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
static void u16ToBCDString( uint8_t * output_string,
                            uint8_t pos,
                            uint16_t number
                           )
{
    /* Local digit position variable */
    uint8_t digit = 5u;

    /* Digit is a leading zero indicator variable */
    bool leading_zero = true;

    /* Local string buffer variable */
    uint8_t buffer_string[5] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while(digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] =(uint8_t) (0x30 + (number % 10));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while(digit < 5u)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator is
           false */
        if((buffer_string[digit] != 0x30) || (!leading_zero))
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
* End of function u16ToBCDString
*******************************************************************************/
