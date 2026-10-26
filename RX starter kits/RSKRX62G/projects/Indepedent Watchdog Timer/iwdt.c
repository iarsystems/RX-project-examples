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
* File Name     : iwdt.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Defines IWDT functions             
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* INTC RPDL function definitions */
#include "r_pdl_intc.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* IWDT function definitions */
#include "r_pdl_iwdt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "iwdt.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* IWDT status flag */
uint16_t IWDT_Status;
/* CMT period update variable */
float gCMT_Period = 5E-3;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* AD conversion complete callback function prototype declaration */
void CB_ADConversion(void);
/* Timer period interrupt callback function prototype declaration */
void CB_CMTPeriod(void);

/*******************************************************************************
* Outline       : Init_IWDT
* Description   : This function checks to see if the last reset was caused by 
*                 the independent WDT underflowing. A message is displayed on 
*                 the debug LCD to inform the user if the reset resulted from 
*                 an underflow otherwise the ADC unit is configured for 
*                 singleshot operation. The non-maskable interrupted is also 
*                 configured. The independent watchdog timer is configured 
*                 for a 131ms timeout period and the CMT unit for a periodic 
*                 interrupt every 500ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_IWDT(void)
{
    /* Declare error flag */
    bool err = true;
        
    /* Read the IWDT status */      
    err &= R_IWDT_Read
            (
                &IWDT_Status
            );
    
    /* Check if underflow occured */        
    if((IWDT_Status & 0x4000) != 0)
    {
        /* Display 'IWDT Overflow' message on the debug LCD */
        DisplayLCD(LCD_LINE1, "  IWDT  ");
        DisplayLCD(LCD_LINE2, "Underflo");
        
        /* Turn on LED0-LED2 */
        err &= R_IO_PORT_Write
                    (
                        PDL_IO_PORT_7_1|PDL_IO_PORT_7_2|PDL_IO_PORT_7_3,
                        0
                    );
    
        /* Turn on LED3 */
        err &= R_IO_PORT_Write
                    (
                        PDL_IO_PORT_3_3,
                        0
                    );
        
        while(1);
    }   
    else
    {   
        /* Configure the ADC unit for one shot operation. After succesful AD
        conversion, the callback function CB_ADConversion is executed. */
        err &=  R_ADC_10_Create
                    (
                        0,
                        PDL_ADC_10_MODE_SINGLE|PDL_ADC_10_CHANNELS_OPTION_0,
                        PDL_ADC_10_TRIGGER_SOFTWARE,
                        50E6,
                        6E-7,
                        CB_ADConversion,
                        4
                    );
                    
        /* Configure the IWDT */
        err &= R_IWDT_Set
                        (
                            PDL_IWDT_TIMEOUT_16384 | PDL_IWDT_CLOCK_OCO_1
                        );
                
        /* Configure the NMI interrupt */
        err &= R_INTC_CreateExtInterrupt
                        (
                            PDL_INTC_NMI,
                            PDL_NO_DATA,
                            PDL_NO_FUNC,
                            0
                        );
    
        /* Configure CMT unit to create a periodic interrupt every 500ms,
        and execute the callback function CB_CMTimer every period */                    
        err &= R_CMT_Create
                        (
                            1,
                            PDL_CMT_PERIOD,
                            500E-3,             
                            CB_CMTPeriod,
                            5       
                        );
    }   
        
    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/*******************************************************************************
* End of function Init_IWDT
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_ADConversion
* Description  : This callback function is executed after every succesful AD
*                conversion. The function fetches the ADC result, and uses it to
*                calculate a new timer period for the LED flash/IWDT refresh ISR.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_ADConversion(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Declare temporary ADC result container variable */
    uint16_t adc_result = 0x0;
    
    /* Fetch the result from the ADC conversion */
    err &=  R_ADC_10_Read
                        (
                            0,
                            &adc_result
                         );
        
    /* Set the timer period global variable value inversely relative to the ADC 
       result. A Timer period is the equivalence of the ADC value multiplied 
       by a factor of 0.00018, added to a 10us offset */
    gCMT_Period = (float) ((adc_result * 180E-6) + 10E-6);
    
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function CB_ADConversion
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTPeriod
* Description  : Timer period interrupt callback function. This function resets
*                the watchdog timer to prevent it underflowing, and toggles the 
*                user LEDs. The function also changes the time period between
*                timer interrupts, by changing the timer period to the value 
*                stored in the variable, gCMT_Period.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMTPeriod(void)
{
    /* Declare error flag */
    bool err = true;
    
    /* Refresh the down-counter if still 
       within the refresh-allowed period (>40ms) */     
    if(gCMT_Period >= (float)0.18)
    {           
        /* Prevent the independent watchdog timer from 
           underflowing by refreshing the count-down counter */     
        err &= R_IWDT_Control
                        (
                            PDL_IWDT_REFRESH    
                        );              
    }
    
    /* Toggle user LEDs */
    err &=  R_IO_PORT_Modify
                        (
                            PDL_IO_PORT_7_1|PDL_IO_PORT_7_2|PDL_IO_PORT_7_3,
                            PDL_IO_PORT_XOR,
                            1
                        );
            
    /* Toggle user LEDs */      
    err &=  R_IO_PORT_Modify
                        (
                            PDL_IO_PORT_3_3,
                            PDL_IO_PORT_XOR,
                            1
                        );
            
    /* Trigger AD conversion to start */
    err &=  R_ADC_10_Control
                        (
                            PDL_ADC_10_0_ON
                        );
            
    /* Reconfigure the timer interrupt period with the value of the
       globabl variable gCMT_Period */
    err &=  R_CMT_Control
                        (
                            1,
                            PDL_CMT_PERIOD|PDL_CMT_START,
                            gCMT_Period
                        );  
                    
    /* Halt in while loop when RPDL errors detected */  
    while(!err);        
}
/*******************************************************************************
* End of function CB_CMTPeriod
*******************************************************************************/
