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
* File Name     : flashLED.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines functions used for flashing the LEDs
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Flashing LED function definitions */
#include "flashLED.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Compare Match Timer callback function prototype */
void CB_TimerLED(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The global variable 'KeyPress' is used as a flag whenever a switch is 
   pressed */
extern uint8_t gSwitchFlag;
/* Variable used to count down the number of LED flashes */
static uint16_t gFlashCount = 0xC8;

/*******************************************************************************
* Outline       : Flash_LED
* Description   : The LED flash function used at the beginning of the program
* Argument      : none
* Return value  : none
*******************************************************************************/
void Flash_LED (void)
{
    /* Declare error flag */
    bool err = true;
                             
    /* Configure compare match timer */
    err &=  R_CMT_Create
            (
                0, 
                PDL_CMT_PCLK_DIV_512, 
                0xFE0, 
                CB_TimerLED,
                3
            );  

    /* Checks if the flash count has been reached, 
       or if a button has been pressed */
    while((gSwitchFlag ==0)&&(gFlashCount > 0))
    {
        /* Waits for a switch press or 200 flashes to complete  */
    }

    /* Reset the gSwitchFlag flag variable */
    gSwitchFlag = 0;
    
    /* Stop Timer */
    err &=  R_CMT_Destroy
            (
                0
            );
        
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function Flash_LED
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_TimerLED
* Description   : Callback function that is executed every time a compare match 
*                 occurs in the timer peripheral. Function toggles LEDs and
*                 decrements flash counter.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_TimerLED(void)
{
    /* Blink LEDs */
    Toggle_LED();
    
    /* Decrement the flash count */
    gFlashCount--;
}
/*******************************************************************************
* End of function CB_TimerLED
*******************************************************************************/

/*******************************************************************************
* Outline       : Toggle_LED
* Description   : Function used to toggle the output of all user LEDs
* Argument      : none
* Return value  : none
*******************************************************************************/
void Toggle_LED(void)
{
    /* Declare error flag */
    bool err = true;

    /* Perform a logical XOR with each LED pin to 1, to invert the pin 
       each time ToggleLEDs() is called */
    err &=  R_IO_PORT_Modify
            (
                PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6|\
                PDL_IO_PORT_1_7,
                PDL_IO_PORT_XOR, 
                1
            );

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function Toggle_LED
*******************************************************************************/
