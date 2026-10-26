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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : timer_event.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Provides timer event catching functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* CMT RPDL function defintions */
#include "r_pdl_cmt.h"
/* MTU RPDL function defintions */
#include "r_pdl_mtu3.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
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
/* Defines LCD display string */
uint8_t CountDisplay[8];
/* Defines count value buffer variable */
uint8_t gEventCount = 0;
/* Declare a variable to indicate displaying of count value */
uint8_t gDisplayCount = 0;

/******************************************************************************
* Local Defined constants
******************************************************************************/

#define MTU3_CH_SELECT__COMPARE_COUNTER     (2U)

#define MTU3_CH_SELECT__DISPLAY_TIMER       (1U)

#define MTU3_COMPARE_COUNTER__CMP_VALUE     (99U)

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT period interrupt LCD update callback function prototype declaration */
void CB_CMT_UpdateLCD(void);
/* 16bit integer to BCD character string function prototype declaration */
void uint16_ToBCDString(uint8_t*, uint8_t, uint16_t);

/*******************************************************************************
* Outline      : Init_TimerEvent
* Description  : The function disables swicth 2 interrupts then initialises timer
*                2 of the MTU3 unit, to be clocked from the falling edge from
*                switch SW2. It also conigures the CMT timer used to create
*                a 100ms periodic interrupt which calls the CB_CMTUpdateLCD
*                callback function.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_TimerEvent(void)
{
    /* Declare error flag */
    bool err = true;

    /* Allocate a copy of the create structure for channel 2 */
    R_MTU3_Create_structure ch2_create_parameters;

    /* Allocate a copy of the control structure for channel 2 */
    R_MTU3_ControlChannel_structure ch2_control_parameters;

    /* Disable SW1 interrupts */
    err &=  R_INTC_ControlExtInterrupt
                (
                    PDL_INTC_IRQ0,
                    PDL_INTC_DISABLE
                 );

    /* Disable SW2 interrupts */
    err &=  R_INTC_ControlExtInterrupt
                (
                    PDL_INTC_IRQ1,
                    PDL_INTC_DISABLE
                 );

    /* Disable SW3 interrupts */
    err &=  R_INTC_ControlExtInterrupt
                (
                    PDL_INTC_IRQ3,
                    PDL_INTC_DISABLE
                 );

    /* Load the default MTU3 channel configurations */
    R_MTU3_Create_load_defaults(&ch2_create_parameters);

    /* Configure the MTU pin */
    err &= R_MTU3_Set
                (
                    PDL_MTU3_PIN_CLKABCD_C
                );

    /* Set the non-default options for channel 2's structure configurations */
    ch2_create_parameters.data2 = PDL_MTU3_MODE_NORMAL;
    ch2_create_parameters.data3 = PDL_MTU3_CLK_MTCLKC | PDL_MTU3_CLEAR_TGRA | PDL_MTU3_CLK_FALLING;
    ch2_create_parameters.data5 = PDL_MTU3_BUFFER_AC_CM_A;
    ch2_create_parameters.data7 = PDL_NO_DATA;
    ch2_create_parameters.data9 = 0;
    ch2_create_parameters.data10 = MTU3_COMPARE_COUNTER__CMP_VALUE;
    ch2_create_parameters.data11 = 0;
    ch2_create_parameters.data12 = 0;
    ch2_create_parameters.data13 = 0;
    ch2_create_parameters.data14 = 0;
    ch2_create_parameters.data15 = 0;
    ch2_create_parameters.func1 = PDL_NO_FUNC;

    /* Configure MTU3 channel 2 with the
       create structure configuration settings */
    err &= R_MTU3_Create
                    (
                        MTU3_CH_SELECT__COMPARE_COUNTER,
                        &ch2_create_parameters
                    );

    /* Load the default MTU3 control parameters */
    R_MTU3_ControlUnit_load_defaults(&ch2_control_parameters);

    /* Set the control options for channel 2 */
    ch2_control_parameters.data2 = PDL_MTU3_START;
    /* Set the starting count value to zero */
    ch2_control_parameters.data4 = 0x0000;

    /* Start the operation of channel 2 */
    err &= R_MTU3_ControlChannel
    (
        MTU3_CH_SELECT__COMPARE_COUNTER,
        &ch2_control_parameters
    );

    /* Display update timer */
    err &= R_CMT_Create(
                          MTU3_CH_SELECT__DISPLAY_TIMER,
                          PDL_CMT_PERIOD,
                          100E-3,
                          CB_CMT_UpdateLCD,
                          7
                        );

    /* Halt in while loop when RPDL errors detected */
    if(err == false)
    {
        while(1);
    }
}
/******************************************************************************
* End of function Init_TimerEvent
******************************************************************************/

/******************************************************************************
* Outline      : CB_CMT_UpdateLCD
* Description  : This callback function is called by the CMT period interrupt,
*                every 100ms. The function reads the count value of the timer
*                channel 2, converts it into a BCD character string and
*                displays it on the debug LCD.
* Argument     : none
* Return value : none
******************************************************************************/
void CB_CMT_UpdateLCD(void)
{
    uint16_t    tmp16Count = 0;
    /* Declare error flag */
    bool err = true;

    /* Declare lcd buffer character string & 8bit timer count container
       variable */
    uint8_t lcd_buffer[] = "Count: 0";

    /* Allocate a copy of the control structure for channel 2 */
    R_MTU3_ControlChannel_structure ch2_control_parameters;

    /* Load the default MTU3 control parameters */
    R_MTU3_ControlUnit_load_defaults(&ch2_control_parameters);

    /* Load the "Stop timer" paremter */
    ch2_control_parameters.data2 = PDL_MTU3_STOP;

    /* Stop the operation of channel 2 */
    err &= R_MTU3_ControlChannel
    (
        MTU3_CH_SELECT__COMPARE_COUNTER,
        &ch2_control_parameters
    );

    /* Read the status flags and registers of channel 2 */
    err &= R_MTU3_ReadChannel
                    (
                        MTU3_CH_SELECT__COMPARE_COUNTER,
                        PDL_NO_PTR,
                        &tmp16Count,
                        PDL_NO_PTR,
                        PDL_NO_PTR,
                        PDL_NO_PTR,
                        PDL_NO_PTR,
                        PDL_NO_PTR,
                        PDL_NO_PTR
                    );

    /*   Move the 8bit value retreived in a 16bit uint to
    a global 8bit value in "gEventCount" */
    gEventCount = (uint8_t)( 0x00ff  & tmp16Count );

    /* Convert timer value into a BCD character string */
  uint16_ToBCDString(lcd_buffer, 3u, tmp16Count);

    /* Start updating the count after the first switch press */
    if((gEventCount > 0)||(gDisplayCount))
    {
        /* Display contents of lcd_buffer onto the debug LCD */
        DisplayLCD(LCD_LINE2, lcd_buffer);

        /* Set flag to 1 */
        gDisplayCount = 1;
    }

    /* Re-start the timer */
    ch2_control_parameters.data2 = PDL_MTU3_START;

    /* Restart the operation of channel 2 */
    err &= R_MTU3_ControlChannel
    (
        MTU3_CH_SELECT__COMPARE_COUNTER,
        &ch2_control_parameters
    );

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/******************************************************************************
* End of function CB_CMT_UpdateLCD
******************************************************************************/

/******************************************************************************
* Outline      : uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*                which is inserted into the character array passed as an
*                argument.
* Argument     : uint8_t * string   - Pointer to array to store BCD string
*                uint8_t pos        - Array element to start BCD string from.
*                uint16_t number    - 16bit integer to convert.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
******************************************************************************/
void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, uint16_t number)
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
        /* Check if digit is not a zero character, or if leading_zero indicator is
           false */
        if((buffer_string[digit] != 0x30) | (!leading_zero))
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
* End of function uint16_ToBCDString
*******************************************************************************/
