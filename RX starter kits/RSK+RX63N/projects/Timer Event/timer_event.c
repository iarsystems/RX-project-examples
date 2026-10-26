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
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Provides timer event catching functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines macros relating to the RX63N user LEDs and switches */
#include "rskrx63ndef.h"
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
extern volatile uint8_t gSwitchFlag;
/* Declare a variable to store count value */
volatile uint16_t gEventCount = 0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT2 initialisation function prototype */
static void Init_Timer(void);
/* 16 bit integer to character string function prototype declaration */
static void u16ToBCDString(uint8_t*, uint8_t, uint16_t);

/*******************************************************************************
* Outline      : Init_TimerEvent
* Description  : The function disables switch 2 interrupts then initialises timer 
*                 2 of the MTU3 unit, to be clocked from the falling edge from 
*                 switch SW2. It also conigures the CMT timer used to create 
*                 a 100ms periodic interrupt which calls the CB_CMTUpdateLCD
*                 callback function.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_TimerEvent(void)
{
    /* Initialise CMT2 */
    Init_Timer();
    
    /* Disable user switch interrupts */
    ControlSwitchInterrupts(0);
    
    /* Configure pin 4 of port C as an input */
    PORTC.PDR.BIT.B4 = 0;
    
    /* Configure pin 4 of port C for peripheral function */
    PORTC.PMR.BIT.B4 = 1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;
    
    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;
    
    /* Configure pin 4 of port C for TCLKE input */
    MPC.PC4PFS.BIT.PSEL = 0x4u;
    
    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;
    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the TMR3 module clock stop mode */
    MSTP_TPU6 = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set TPU6 to clock from external pin TCLKE */
    TPU6.TCR.BIT.TPSC = 0x4u;
    
    /* Start counter */
    TPUB.TSTR.BIT.CST6 = 1u;
}
/*******************************************************************************
* End of function Init_TimerEvent
*******************************************************************************/

/*******************************************************************************
* Outline      : Init_Timer
* Description  : This function initialises the CMT channel 2.
* Argument     : none
* Return value : none
*******************************************************************************/
static void Init_Timer(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2;

    /* Specify the timer period (100ms @ 375KHz = 37500 or 0x927C counts) */
    CMT2.CMCOR = 0x927C;

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;

    /* Set CMT2 interrupt priority level to 5*/
    IPR(CMT2,CMI2) = 0x5;
    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;    
    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;
}
/*******************************************************************************
* End of function Init_Timer
*******************************************************************************/

/******************************************************************************
* Outline      : u16ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*                 which is inserted into the character array passed as an 
*                 argument.
* Argument     : uint8_t * string    - Pointer to array to store BCD string 
*                 uint8_t pos        - Array element to start BCD string from.
*                 uint16_t number    - 16bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                 array passed.
******************************************************************************/
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
    uint8_t buffer_string[] = "00000";
    
    /* Decrement digit each iteration, until digit is zero */
    while(digit--)
    {
        /* Write BCD value to string element */
        buffer_string[digit] = (uint8_t)(0x30 + (number % 10));
        
        /* Move to the next decimal digit */
        number /= 10u;
    }
    
    /* Set digit position to zero */
    digit = 0u;
    
    /* Loop through each digit, and replace leading zeros with a space
       character */
    while(digit < 5u)
    {
        /* Check if digit is not a zero character, 
           or if leading_zero indicator is false */
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

/*******************************************************************************
* Outline      : Excep_CMTU1_CMT2
* Description  : CMT2 interrupt handler is called by the CMT period interrupt,
*                 every 100ms. The function reads the count value of CMT's
*                 channel 2, converts it into a BCD character string and 
*                 displays it on the debug LCD.
* Argument      : none    
* Return value  : none
********************************************************************************/
#pragma vector = VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{
    LED0 = LED_ON;
    /* Declare temporary character string */
    uint8_t lcd_buffer[] = "Count: 0";
    
    /* Check if count value is 99 or higher */
    if(TPU6.TCNT > 99u)
    {
        /* Stop the timer channel */
        TPUB.TSTR.BIT.CST6 = 0;
        
        /* Reset the counter value to 0 */
        TPU6.TCNT = 0;
        
        /* Restart the timer channel */
        TPUB.TSTR.BIT.CST6 = 1u;
    }
        
    /* Fetch count value and store in global variable gEventCount' */
    gEventCount = (uint16_t)TPU6.TCNT;
        
    /* Convert ADC result into a character string, and store in the local
       string lcd_buffer */    
    u16ToBCDString(lcd_buffer, 3, gEventCount);

    /* Display the contents of the local string lcd_buffer */
    Display_LCD(LCD_LINE2, lcd_buffer);    
    
    LED0 = LED_OFF;
}
/********************************************************************************
* End of function Excep_CMTU1_CMT2
********************************************************************************/
