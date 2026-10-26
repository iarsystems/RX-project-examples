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
* File Name     : wdt.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines WDT, ADC and timer control functions           
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Compare match timer RPDL function definitions */
#include "r_pdl_cmt.h"
/* Interrupt control RPDL function definitions */
#include "r_pdl_intc.h"
/* WDT RPDL function definitions */
#include "r_pdl_wdt.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro defines */
#include "lcd.h"
/* Switch function prototypes and macro defines used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "wdt.h"

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Watchdog timer overflow interrupt callback function prototype declaration */
void CB_Overflow_WDT(void);
/* Timer period interrupt callback function prototype declaration */
void CB_Timer_WDT(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Flash/WDT reset interrupt period global variable */
double gTimer_Period = 1E-3;
/* Declare temporary ADC result container variable */
volatile uint16_t adc_result[16];

/*******************************************************************************
* Outline      : Init_WDT
* Description  : This function configures the ADC unit for single conversion,
*                the watchdog for a 1.342 sec timeout period, and the timer unit 
*                to create a periodic interrupt every 100ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_WDT(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the port pin P40 as analog channel AN000 input */
    err &=  R_ADC_12_Set
            (
                PDL_ADC_12_PIN_AN000_P40 
            );
    
    /* Configure the ADC unit for scan operation. After successful AD
       conversion, the callback function CB_ADConversion is executed. */    
    err &=  R_ADC_12_CreateUnit
            (
                0,
                PDL_ADC_12_SCAN_CONTINUOUS,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_FUNC,
                PDL_NO_DATA,
                PDL_NO_FUNC,
                PDL_NO_DATA
            );                      

    /* Configure the ADC channel AN000
       Use the sampling time as 5 uS  */
    err &=  R_ADC_12_CreateChannel
            (
                0,
                0,
                PDL_ADC_12_CH_GROUP_A|PDL_ADC_12_CH_SAMPLE_AND_HOLD_ENABLE,
                5E-6
            );

    /* Start the ADC conversion */
    err &=  R_ADC_12_Control
            (
                PDL_ADC_12_0_ON
            );  

    /* Configure CMT unit to create a periodic interrupt every 100ms,
       and execute the callback function CB_Timer_WDT every timer overflow
       period */
    err &=  R_CMT_Create
            (
                 1,
                 PDL_CMT_PERIOD,
                 100E-3,        
                 CB_Timer_WDT,
                 5      
            );  

    /* Configure the WDT unit to timeout after 1.342 Sec, and call the 
       callback function CB_WDTOverflow when the WDT overflows - 
       Watchdog period calculation - 
           Period = (16384 x 2048)/25 MHz = 1.342 Sec   */
    err &=  R_WDT_Set
            (
                PDL_WDT_TIMEOUT_16384|PDL_WDT_PCLK_DIV_2048|PDL_WDT_TIMEOUT_NMI 
            );  
            
    /* Configure the WDT to generate NMI interrupt on overflow */
    err &=  R_INTC_CreateExtInterrupt
            (
                PDL_INTC_NMI,
                PDL_INTC_FILTER_DISABLE|PDL_INTC_FALLING|PDL_INTC_WDT_ENABLE,
                CB_Overflow_WDT,
                PDL_NO_DATA
            );          

    /* Start the WDT */
    err &=  R_WDT_Control
            (
                PDL_WDT_RESET_COUNTER
            );      
            
    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/*******************************************************************************
* End of function Init_WDT
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Timer_WDT
* Description  : Timer period interrupt callback function. This function resets
*                the watchdog timer to prevent it overflowing, and toggles the 
*                user LEDs. The function also changes the time period between
*                timer interrupts, by changing the timer period to the value 
*                stored in the variable 'gTimer_Period'.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Timer_WDT(void)
{
    /* Declare error flag */
    bool err = true;

    /* Reset the watchdog timer */
    err &=  R_WDT_Control
            (
                PDL_WDT_RESET_COUNTER
            );

    /* Fetch the result from the ADC conversion */
    err &=  R_ADC_12_Read
            (
                0,
                adc_result,
                0
            );
    
    /* Set the timer period global variable value relative to the ADC result.
       A Timer period is equal to the ADC value multiplied by a factor of
       340E-6, added to a 10us offset. When the ADC read value crosses 
       4000 (0xFA0), the CMT overflow period exceeds watchdog overflow period
       as given below - 
       (4000 x 340e-6) + 10e-6 = 1.36 sec.    */
    gTimer_Period = (float) ((adc_result[0] * 340E-6) + 10E-6);

    /* Toggle user LEDs (LED0-LED3) */
    err &=  R_IO_PORT_Modify
            (
                PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6|PDL_IO_PORT_1_7,
                PDL_IO_PORT_XOR,
                1
            );
            
    /* Re-configure the timer interrupt period with the value of the
       global variable 'gTimer_Period' */
    err &=  R_CMT_Control
            (
                1,
                PDL_CMT_PERIOD|PDL_CMT_START,
                gTimer_Period
            );  
            
    /* Halt in while loop when RPDL errors detected */  
    while(!err);        
}
/*******************************************************************************
* End of function CB_Timer_WDT
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Overflow_WDT
* Description  : This callback function is executed when the watchdog timer
*                overflows. The function sets the user LEDs ON, displays
*                "Watchdog Overflow" on the debug LCD, and waits in an infinite
*                while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Overflow_WDT(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Turn all user LEDs on */
    err &=  R_IO_PORT_Write
            (
                PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6|PDL_IO_PORT_1_7,
                0
            );  
    
    /* Display "Watchdog Overflow" message on the debug LCD */
    Display_LCD(LCD_LINE1, "Watchdog");
    Display_LCD(LCD_LINE2, "Overflow");

    /* Halt in while loop when RPDL errors detected */  
    while(!err);        

    /* Infinite while loop */   
    while(1);
}
/*******************************************************************************
* End of function CB_Overflow_WDT
*******************************************************************************/
