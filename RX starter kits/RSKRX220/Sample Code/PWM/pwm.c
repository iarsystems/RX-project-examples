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
* File Name     : pwm.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines MTU3 configuration functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

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

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

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
void cb_switch_press (void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_tobcdstring (char * const output_string, const uint8_t pos,
                                                              uint16_t number);


/******************************************************************************
Global Variables
******************************************************************************/

/* Variable for storing the event count */
volatile uint16_t g_duty_percentage;

/* Variable for halting updates and displaying the duty cycle */
volatile bool gVaryDutyCycle = true;

/* Variable for starting clearance of LCD's line
   two only after the first SW1 press */
volatile bool gClearLCD = false;


/*******************************************************************************
* Function Name: init_pwm
* Description  : Function configures the MTU's output pin MTIOC0A. It then
*                initialises the MTU's channel 3 to output a 1KHz PWM signal,
*                with interrupt generations enabled.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_pwm (void)
{
    /* Configure pin 3 of port B as an output */
    PORTB.PDR.BIT.B3 = 0x1u;

    /* Configure pin 3 of port B for peripheral function */
    PORTB.PMR.BIT.B3 = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Configure pin 2 of port C for MTIOC4B outputs */
    MPC.PB3PFS.BIT.PSEL = 0x1u;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the MTU0 module clock stop mode */
    MSTP_MTU0 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set MTU0 TGIA0 interrupt priority level to 7 */
    IPR(MTU0, TGIA0) = TIMER_IPL;

    /* Enable MTU0 TGIA0 interrupts */
    IEN(MTU0, TGIA0) = 0x1u;

    /* Clear MTU0 TGIA0 interrupt flag */
    IR(MTU0, TGIA0) = 0x0;

    /* Set MTU0 TGIB0 interrupt priority level to 7 */
    IPR(MTU0, TGIB0) = TIMER_IPL;

    /* Enable MTU0 TGIB0 interrupts */
    IEN(MTU0, TGIB0) = 0x1u;

    /* Clear MTU0 TGIB0 interrupt flag */
    IR(MTU0, TGIB0) = 0x0;

    /* Set clock to PCLK/1=48MHz */
    MTU0.TCR.BIT.TPSC = 0x0;

    /* Count at falling edge */
    MTU0.TCR.BIT.CKEG = 0x1u;

    /* TCNT cleared by TGRA compare match */
    MTU0.TCR.BIT.CCLR = 0x1u;

    /* Operate timers in pwm mode */
    MTU0.TMDR.BYTE = 0x02u;

    /* Initial MTIOC0A output is 1 and toggled at
       TGRA compare match. */
    MTU0.TIORH.BIT.IOA = 0x7u;

    /* TGRB output is disabled */
    MTU0.TIORH.BIT.IOB = 0x3u;

    /* Enable TGIEA interrupts */
    MTU0.TIER.BIT.TGIEA = 0x1u;

    /* Enable TGIEB interrupts (for LED) */
    MTU0.TIER.BIT.TGIEB = 0x1u;

    /* Enable access to protected MTU registers */
    MTU.TRWER.BIT.RWE = 0x1u;

    /* Use TGRA to set the pwm cycle to 1KHz*/
    MTU0.TGRA = PWM_FREQ;

    /* TGRB used to set the pwm duty cycle to 90% */
    MTU0.TGRB = PWM_FREQ / 10u;

    /* Set a switch press callback function */
    set_switch_release_callback(cb_switch_press);

    /* Start the TCNT counter */
    MTU.TSTR.BIT.CST0 = 0x1u;
}
/*******************************************************************************
* End of function init_pwm
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_tobcdstring
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Arguments    : (char * const)string - Pointer to array to store BCD string
*                (uint8_t)pos         - Array element to start BCD string from.
*                (uint16_t)number     - 16bit integer to convert.
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
    uint8_t buffer_string[6] = "00000";

    /* Decrement digit each iteration, until digit is zero */
    while (digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (uint8_t) (0x30u + (number % 10));

        /* Move to the next decimal digit */
        number /= 10u;
    }

    /* Set digit position to zero */
    digit = 0u;

    /* Loop through each digit, and replace leading zeros with a space
       character */
    while (digit < 5u)
    {
        /* Check if digit is not a zero character, or if leading_zero indicator is
           false */
        if ((0x30u != buffer_string[digit]) || (!leading_zero))
        {
            /* Copy digit from buffer string to output string */
            output_string[digit + pos] = (char) buffer_string[digit];

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

/*******************************************************************************
* Function Name: cb_switch_press
* Description  : Switch Press callback function. This function checks which
*                switch was pressed. The second line of the debud LCD is cleared
*                if switch SW1 is pressed and if the gClearLCD flag is set to
*                true, allowing the duty cycle to be varied. If the flag isn't
*                set but SW1 has been pressed, duty cycle variation is stopped
*                and the duty is displayed as a
*                percentage.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void cb_switch_press (void)
{
    /* Local LCD buffer string */
    char  lcd_buffer[] = "D.C:   %";
    uint32_t calc_duty_percent = 0;

    /* Has switch SW1 not been pressed and LCD clearance allowed? */
    if ((SWITCHPRESS_1 != (g_switch_flag & SWITCHPRESS_1)) &&
        (true == gClearLCD))
    {
        /* Allow duty cycle to be updated */
        gVaryDutyCycle = true;

        /* Clear the second line of the debug LCD */
        Display_LCD(LCD_LINE2, "        ");
    }

    /* Set the gVaryDutyCycle to false and display the duty
       cycle as a percentage if SW1 has been pressed */
    if ((g_switch_flag & SWITCHPRESS_1))
    {
        /* Set flag to indicate stoppage of duty cycle updates. */
        gVaryDutyCycle  = false;

        /* Set to true to allow clearance of LCD's line 2 */
        gClearLCD = true;

        /* Calculate the percentage */
        calc_duty_percent = (uint32_t)((g_duty_percentage * 100u) / PWM_FREQ);

        /* Convert the calculated percentage into a BCD character string */
        uint16_tobcdstring(lcd_buffer, 2u, (uint16_t)calc_duty_percent);

        /* Display contents of lcd_buffer onto the debug LCD */
        Display_LCD(LCD_LINE2, lcd_buffer);
    }

    /* Clear the switch press flags */
    g_switch_flag &= 0x0Fu;
}
/*******************************************************************************
* End of function cb_switch_press
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_MTU0_TGIA0_isr
* Description  : MTU0 TGIA0 interrupt handler. This function updates the duty
*                cycle by 1%, resets the duty to 10% if it reaches 90%, and
*                automatically toggles the MTIOC0A pin on every TGRA output
*                compare match.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_MTU0_TGIA0
__interrupt  void Excep_MTU0_TGIA0_isr(void)
{
    /* Stop the TCNT counter */
    MTU.TSTR.BIT.CST0 = 0x0;

    /* Switch LED3 back on */
    LED3 = LED_ON;

    /* Update the duty cycle if SW1 hasn't been pressed. */
    if (true == gVaryDutyCycle)
    {
        /* Reset the duty cycle to 10% */
        MTU0.TGRB += 12u;

        /* Read duty cycle */
        g_duty_percentage = MTU0.TGRB;

        /* Check if the duty cycle equals 90% */
        if (g_duty_percentage > (PWM_FREQ - (PWM_FREQ / 10)))
        {
            /* Reset the duty cyccle to 10% */
            MTU0.TGRB = PWM_FREQ / 10u;
        }
    }

    /* Start the TCNT counter */
    MTU.TSTR.BIT.CST0 = 0x1u;
}
/*******************************************************************************
* End of function Excep_MTU0_TGIA0_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_MTU0_TGIB0_isr
* Description  : MTU0 TGIB0 interrupt handler. This function switches LED3 off
*                when the duty cycle timer reaches 0.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_MTU0_TGIB0
__interrupt  void Excep_MTU0_TGIB0_isr(void)
{
    /* Switch LED3 off at end of high period */
    LED3 = LED_OFF;
}
/*******************************************************************************
* End of function Excep_MTU0_TGIB0_isr
*******************************************************************************/

