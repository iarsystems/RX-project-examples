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
* File Name     : pwm.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
*******************************************************************************/
/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define for number of 48MHz PCLK timer ticks in a ms */
#define PCLKB_TICKS_PER_MS 48000

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* LCD controlling function prototypes & macro defines */
#include "lcd.h"

/* Switch handler function definitions */
#include "switch.h"

/* Declares function prototypes defined in this file */
#include "pwm.h"

/******************************************************************************
* Local Function Prototypes
******************************************************************************/
/* Callback function prototype for switch presses */
static void cb_switch_press(void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_bcd_string(char * const output_string, const uint8_t pos,
                                    uint16_t number);

/******************************************************************************
Global Variables
******************************************************************************/
/* Variable for storing the TGRB event count */
static volatile uint16_t tgrb_current_value;

/* Variable for halting updates and displaying the duty cycle */
static volatile bool vary_duty_cycle = true;

/*******************************************************************************
* Function Name : Init_PWM
* Description   : Function configures the MTU's output pin MTIOC3A. It then
*                 initialises the MTU's channel 3 to output a 1KHz PWM signal,
*                 with interrupt generations enabled.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_PWM (void)
{
    /* Configure pin 3 of port 3 as an output */
    PORT3.PDR.BIT.B3 = 0x1;

    /* Configure pin 3 of port 3 for peripheral function */
    PORT3.PMR.BIT.B3 = 0x1;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;

    /* Configure pin 3 of port 1 for MTIOC3A outputs */
    MPC.P33PFS.BIT.PSEL = 0x1;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the MTU3 module clock stop mode */
    MSTP_MTU3 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set MTU3 TGIA3 interrupt priority level to 7 */
    IPR(MTU3,TGIA3) = 0x7;

    /* Enable MTU3 TGIA3 interrupts */
    IEN(MTU3,TGIA3) = 0x1;

    /* Clear MTU3 TGIA3 interrupt flag */
    IR(MTU3,TGIA3) = 0x0;

    /* Clock PCLK/4=24MHz, count at both edges,
       TCNT cleared by TGRA compare match */
    MTU3.TCR.BIT.TPSC = 0x1;
    MTU3.TCR.BIT.CKEG = 0x2;
    MTU3.TCR.BIT.CCLR = 0x1;

    /* Operate timers in pwm mode - TGRA and TGRC operate normally with Buffer A
       TGRB and TGRD operate normally with Buffer B */
    MTU3.TMDR1.BYTE = 0x02;

    /* Initial MTIOC3A output is 0 and set to 1 at TGRB compare match.
       TGRA compare match resets to 0. This corresponds to the PWM mode 1
       example in the Hardware Manual Section 21.3.5, Figure 21.27 */
    MTU3.TIORH.BYTE = 0x61;

    /* Enable TGIEA interrupts only*/
    MTU3.TIER.BYTE = 0x01;

    /* Use TGRA to set the pwm cycle to 1KHz */
    MTU3.TGRA = PCLKB_TICKS_PER_MS;

    /* TGRB used to set the pwm duty cycle to 90% */
    MTU3.TGRB = PCLKB_TICKS_PER_MS / 10;

    /* Set a switch press callback function */
    SetSwitchReleaseCallback(cb_switch_press);

    /* Start the TCNT counter */
    MTU.TSTRA.BIT.CST3 = 0x1;
}
/*******************************************************************************
* End of function Init_PWM
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
* Function Name : cb_switch_press
* Description   : Switch Press callback function. This function checks which
*                 switch was pressed. The second line of the debud LCD is
*                 cleared if switch SW1 is pressed and if the clear_lcd flag is
*                 true, allowing the duty cycle to be varied. If the flag isn't
*                 set but SW1 has been pressed, duty cycle variation is stopped
*                 and the duty is displayed as a percentage.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void cb_switch_press (void)
{
    /* Variable for starting clearance of LCD's line
    two only after the first SW1 press */
    static bool clear_lcd = false;

    /* Local LCD buffer string */
    char        lcd_buffer[] = "D.C:   %";
    uint32_t    calc_duty_percent = 0;

    /* Set the vary_duty_cycle to false and display the duty
       cycle as a percentage if SW1 has been pressed */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Set flag to indicate stoppage of duty cycle updates. */
        vary_duty_cycle  = false;

        /* Set to true to allow clearance of LCD's line 2 */
        clear_lcd = true;

        /* Calculate the percentage */
        calc_duty_percent =
            (uint32_t)(100 - ((tgrb_current_value * 100) / PCLKB_TICKS_PER_MS));

        /* Convert the calculated percentage into a BCD character string */
        uint16_to_bcd_string(lcd_buffer, 2u, (uint16_t)calc_duty_percent);

        /* Display contents of lcd_buffer onto the debug LCD */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }

    /* Either SW2 or SW3 has been pressed - is LCD clearance allowed? */
    else if (true == clear_lcd)
    {
        /* Allow duty cycle to be updated */
        vary_duty_cycle  = true;

        /* Clear the second line of the debug LCD */
        Display_LCD(LCD_LINE2, "JA6 - 13");
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function cb_switch_press
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_MTU3_TGIA3
* Description  : MTU3 TGIA3 interrupt handler. This function updates the duty
*                cycle by 1%, resets the duty to 10% if it reaches 90%, and
*                automatically toggles the MTIOC3A pin on every TGRA output
*                compare match.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_MTU3_TGIA3
__interrupt  void Excep_MTU3_TGIA3(void)
{
    /* Stop the TCNT counter */
    MTU.TSTRA.BIT.CST3 = 0x0;

    /* Clear the compare match flag bit TGFA in the TSR,
       need to read 1 then write 0 to clear it*/
    if (0x1 == MTU3.TSR.BIT.TGFA)
    {
        MTU3.TSR.BIT.TGFA = 0x0;
    }

    /* Update the duty cycle if SW1 hasn't been pressed. */
    if (true == vary_duty_cycle)
    {
        /* Decrease the duty cycle by 0.025 % */
        /* Cast required to remove compiler warning  -
           Implicit conversion: int to unsigned short. */
        MTU3.TGRB += (uint16_t)(PCLKB_TICKS_PER_MS / 4000);

        /* Read duty cycle */
        tgrb_current_value = MTU3.TGRB;

        /* Check if the duty cycle equals 10% */
        if (tgrb_current_value >
                (PCLKB_TICKS_PER_MS - (PCLKB_TICKS_PER_MS / 10)))
        {
            /* Reset the duty cycle to 90% */
            MTU3.TGRB = PCLKB_TICKS_PER_MS / 10;
        }
    }

    /* Start the TCNT counter */
    MTU.TSTRA.BIT.CST3 = 0x1;
}
/*******************************************************************************
* End of function Excep_MTU3_TGIA3
*******************************************************************************/

