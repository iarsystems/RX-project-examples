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
* File Name     : timeradc.c
* Version       : 1.00
* Device        : R5F563TE
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History       : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* LCD controlling function prototypes & macro definitions */
#include "timeradc.h"

/* Flashing LED function definitions */
#include "flashled.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* start_timer function declaration */
static void start_timer(void);

/* start_adc function declaration */
static void start_adc(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/* Declare a variable to select delay on updating LCD */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global count value */
volatile uint32_t g_delay_counter = 0;

/*******************************************************************************
* Function Name : Timer_ADC
* Description   : Function initialises the Timer and ADC peripherals needed to
*                 vary the LED flash rate based on the setting of potentiometer.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Timer_ADC (void)
{
    /* Call the Timer start function */
    start_timer();

    /* Call the ADC start function */
    start_adc();
}
/*******************************************************************************
* End of function Timer_ADC
*******************************************************************************/

/*******************************************************************************
* Function Name : start_timer
* Description   : Configures CMT channel 1 to call the 'CB_TimerADC' callback
*                 function which starts the AD conversion
* Argument      : none
* Return value  : none
*******************************************************************************/
static void start_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CMT1 module clock stop mode */
    MSTP_CMT1 = 0;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set CMT1 interrupt priority level to 10 */
    IPR(CMT1,CMI1) = 0xA;

    /* Enable CMT1 interrupts */
    IEN(CMT1,CMI1) = 0x1;

    /* Clear CMT1 interrupt flag */
    IR(CMT1,CMI1) = 0x0;

    /* Set CMT2 interrupt priority level to 5 */
    IPR(CMT2,CMI2) = 0x5;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;

    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;

    /* Select the PCLK clock division as PCLK/512 = 93.75KHz */
    CMT1.CMCR.BIT.CKS = 0x3;

    /* Specify the timer period */
    CMT1.CMCOR = (uint16_t)((S12AD.ADDR0 * 16) + 1);

    /* Enable the compare match interrupt */
    CMT1.CMCR.BIT.CMIE = 0x1;

    /* Start CMT1 count */
    CMT.CMSTR0.BIT.STR1 = 0x1;
}
/*******************************************************************************
* End of function start_timer
*******************************************************************************/

/*******************************************************************************
* Function Name : start_adc
* Description   : Initialises the S12AD ADC module's channel AN000 for
*                 continuous scan operations, then starts the conversions.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void start_adc (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Clear the S12AD interrupt flag */
    IR(S12AD, S12ADI) = 0x0;

    /* Set the S12AD interrupt level to 5 */
    IPR(S12AD, S12ADI) = 0x5;

    /* Enable S12AD interrupt requests */
    IEN(S12AD, S12ADI) = 0x1;

    /* Use the AN000 (Potentiometer) pin, P4.0, P(56)
       as an I/O for peripheral functions */
    /* Enable Group B scan end Interrupt */
    S12AD.ADCSR.WORD |= 0x40;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001;

    /* Set ADCSR to continous sample mode */
    S12AD.ADCSR.BIT.ADCS = 0x2;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 1;

    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 1;
}
/*******************************************************************************
* End of function start_adc
*******************************************************************************/

/******************************************************************************
* Function Name : Timer_Delay
* Description   : Function used to create delays in milliseconds or
*                 microseconds depending on the user selection for controlling
*                 the debug LCD function's calls.
* Argument      : uint32_t  -   User_Delay (Maximum 697 ms, 10922 uS)
*                 int8      -   Unit
* Return value  : none
******************************************************************************/
void Timer_Delay (const uint32_t User_Delay, const int8_t Unit)
{
    /* Clear the timer's count */
    g_delay_counter = 0;

    /* Stor a copy of the user delay value to g_periodic_delay */
    g_periodic_delay = User_Delay;

    /* Check if microseconds delay is required */
    if (MICROSECS == Unit)
    {
        /* Select the PCLK clock division as PCLK/8 = 6MHz */
        CMT2.CMCR.BIT.CKS = 0x0;

        /* Specify the timer period */
        CMT2.CMCOR = (uint16_t)(User_Delay * 6);
    }

    /* Check if milliseconds delay is required */
    if (MILLISECS == Unit)
    {
        /* Select the PCLK clock division as PCLK/128 = 93.75KHz */
        CMT2.CMCR.BIT.CKS = 0x3;

        /* Specify the timer period */
        CMT2.CMCOR = (uint16_t)(User_Delay * 94);
    }

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;

    /* Wait for the timer to timeout */
    while (g_delay_counter != User_Delay)
    {
        /* Wait */
    }

    /* Stop CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0;

    /* Clear counter */
    g_delay_counter = 0;
}
/******************************************************************************
* End of function Timer_Delay
******************************************************************************/

/******************************************************************************
* Function Name : Excep_S12AD_S12ADI
* Description   : S12AD interrupt handler. This function stores the A/D value
*                 into variable g_adc_result.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_S12AD_S12ADI
__interrupt  void Excep_S12AD_S12ADI(void)
{
    /* Fetch ADC value */
    g_adc_result = S12AD.ADDR0;
}
/******************************************************************************
* End of function Excep_S12AD_S12ADI
******************************************************************************/

/******************************************************************************
* Function Name : Excep_CMT1_CMI1
* Description   : CMT1 interrupt handler. This function calls the function
*                 Toggle_LED to toggle the user LEDs and updates the timer's
*                 period using the A/D conversion value.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT1_CMI1
__interrupt  void Excep_CMT1_CMI1(void)
{
    /* Toggle LEDs */
    Toggle_LED();

    /* Update the timer */
    CMT1.CMCOR = (uint16_t)(((g_adc_result / 50) * 200) + 3000);
}
/******************************************************************************
* End of function Excep_CMT1_CMI1
******************************************************************************/

/******************************************************************************
* Function Name : Excep_CMT2_CMI2
* Description   : CMT2 interrupt handler. This function increments the
*                 g_delay_counter variable.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Increment counter */
    g_delay_counter++;
}
/******************************************************************************
* End of function Excep_CMT2_CMI2
******************************************************************************/

