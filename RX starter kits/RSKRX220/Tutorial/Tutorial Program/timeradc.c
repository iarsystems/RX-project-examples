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
* File Name     : timeradc.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
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

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "timeradc.h"

/* Flashing LED function definitions */
#include "flashLED.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* start_timer function declaration */
static void start_timer (void);

/* start_adc function declaration */
static void start_adc (void);

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* The variable g_adc_result is used to contain the value of the 12 bit ADC */
volatile uint16_t g_adc_result;

/* Declare a variable to selec*/
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global count value */
volatile uint32_t g_delay_counter = 0;

/*******************************************************************************
* Function Name: timer_adc
* Description  : Function initialises the Timer and ADC peripherals needed to
*                 vary the LED flash rate based on the setting of potentiometer.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void timer_adc (void)
{
    /* Call the Timer start function */
    start_timer();

    /* Call the ADC start function */
    start_adc();
}

/*******************************************************************************
* End of function timer_adc
*******************************************************************************/

/*******************************************************************************
* Function Name: start_timer
* Description  : Configures CMT channel 1 to call the 'CB_TimerADC' callback
*                function which starts the AD conversion
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void start_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CMT1 module clock stop mode */
    MSTP_CMT1 = 0x0;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set CMT1 interrupt priority level to 10 */
    IPR(CMT1,CMI1) = 0xAu;

    /* Enable CMT1 interrupts */
    IEN(CMT1,CMI1) = 0x1u;

    /* Clear CMT1 interrupt flag */
    IR(CMT1,CMI1) = 0x0;

    /* Set CMT2 interrupt priority level to 5 */
    IPR(CMT2,CMI2) = 0x5u;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1u;

    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;

    /* Select the PCLK clock division as PCLK/128 = 156.25KHz */
    CMT1.CMCR.BIT.CKS = 0x2u;

    /* Specify the timer period */
    CMT1.CMCOR = (uint16_t)((S12AD.ADDR0 * 16u) + 1u);

    /* Enable the compare match interrupt */
    CMT1.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT1 count */
    CMT.CMSTR0.BIT.STR1 = 0x1u;
}
/*******************************************************************************
* End of function start_timer
*******************************************************************************/

/*******************************************************************************
* Function Name: start_adc
* Description  : Initialises the S12AD ADC module's channel AN000 for
*                continuous scan operations, then starts the conversions.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void start_adc (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Clear the S12AD interrupt flag */
    IR(S12AD, S12ADI0) = 0x0;

    /* Set the S12AD interrupt level to 5 */
    IPR(S12AD, S12ADI0) = 0x5u;

    /* Enable S12AD interrupt requests */
    IEN(S12AD, S12ADI0) = 0x1u;

    /* Use the AN000 (Potentiometer) pin
       as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;

    /* Set continuous scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x2u;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 0x1u;

    /* start ADC */
    S12AD.ADCSR.BIT.ADST = 0x1u;
}
/*******************************************************************************
* End of function start_adc
*******************************************************************************/

/******************************************************************************
* Function Name: timer_delay
* Description  : Function used to create delays in milliseconds
* Arguments    : (const uint32_t)user_delay - Length of timer delay in ms
* Return Value : none
******************************************************************************/
void timer_delay (const uint32_t user_delay)
{
    /* Clear the timer's count */
    g_delay_counter = 0x0;

    /* Stors a copy of the user delay value to g_periodic_delay */
    g_periodic_delay = user_delay;

    /* Select the PCLK clock division as PCLK/32 = 625KHz */
    CMT2.CMCR.BIT.CKS = 0x1u;

    /* Specify the timer period (1ms) */
    CMT2.CMCOR = (uint16_t)(625u);

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x1u;

    /* Wait for the timer to timeout */
    while (g_delay_counter < user_delay)
    {
        /* Wait */
    }

    /* Stop CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x0;

    /* Clear counter */
    g_delay_counter = 0x0;
}
/******************************************************************************
* End of function timer_delay
******************************************************************************/

/******************************************************************************
* Function Name: Excep_S12AD0_S12ADI0_isr
* Description  : S12AD interrupt handler. This function stores the A/D value
*                into variable g_adc_result.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_S12AD_S12ADI0
__interrupt  void Excep_S12AD0_S12ADI0_isr(void)
{
    /* Fetch ADC value */
    g_adc_result = S12AD.ADDR0;
}
/******************************************************************************
* End of function Excep_S12AD0_S12ADI0_isr
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMTU0_CMT1_isr
* Description  : CMT1 interrupt handler. This function calls the function
*                Toggle_LED to toggle the user LEDs and updates the timer's
*                period using the A/D conversion value.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CMT1_CMI1
__interrupt  void Excep_CMTU0_CMT1_isr(void)
{
    /* Toggle LEDs */
    toggle_led();

    /* Update the timer */
    CMT1.CMCOR = (uint16_t)(((g_adc_result / 50u) * 200u) + 3000u);
}
/******************************************************************************
* End of function Excep_CMTU0_CMT1_isr
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMTU1_CMT2_isr
* Description  : CMT2 interrupt handler. This function increments the
*                g_delay_counter variable.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMTU1_CMT2_isr(void)
{
    /* Increment counter */
    g_delay_counter++;
}
/******************************************************************************
* End of function Excep_CMTU1_CMT2_isr
******************************************************************************/

