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
* File Name     : iwdt.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines IWDT functions
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

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
uint16_t g_cmt_period = 0;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* init_timer function declaration */
static void init_timer (void);

/* init_adc function declaration */
static void init_adc (void);

/*******************************************************************************
* Function Name: init_iwdt
* Description  : This function checks to see if the last reset was caused by
*                the independent WDT underflowing. A message is displayed on
*                the debug LCD to inform the user if the reset resulted from
*                an underflow otherwise the ADC unit is configured for
*                singleshot operation. The non-maskable interrupted is also
*                configured. The independent watchdog timer is configured
*                for a 131ms timeout period and the CMT unit for a periodic
*                interrupt every 500ms.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_iwdt (void)
{
    /* Check if underflow occured */
    if (IWDT.IWDTSR.BIT.UNDFF)
    {
        /* Display 'IWDT Overflow' message on the debug LCD */
        Display_LCD(LCD_LINE1, "  IWDT  ");
        Display_LCD(LCD_LINE2, "Underflo");

        /* Turn on LED0-LED3 */
        LED0 = LED_ON;
        LED1 = LED_ON;
        LED2 = LED_ON;
        LED3 = LED_ON;

        while (1)
        {
            /* Halt here due to watchdog reset */
        }
    }
    else
    {
        /* Configure the ADC unit for one shot operation. After succesful AD
        conversion, the interrupt handler Excep_S12AD_S12ADI0 is executed. */
        init_adc();

        /* Protection off */
        SYSTEM.PRCR.WORD = PROTECTOFF;

        /* Start the IWDT clock */
        SYSTEM.ILOCOCR.BIT.ILCSTP = 0x0;

        /* Protection on */
        SYSTEM.PRCR.WORD = PROTECTON;

        /* Configure the IWDT time out period to 1024 cycles and clock
           division to IWDTCLK/64, set the refresh window as 0%-100% */
        IWDT.IWDTCR.WORD = 0x3340u;

        /* Configure the reset output */
        IWDT.IWDTRCR.BIT.RSTIRQS = 0x1u;

        /* Configure CMT unit to create a periodic interrupt,
        and execute the interrupt handler Excep_CMTU1_CMT1 every period */
        init_timer();

    }
}
/*******************************************************************************
* End of function init_iwdt
*******************************************************************************/

/*******************************************************************************
* Function Name: init_adc
* Description  : Initialises the S12AD ADC module's channel AN000 for
*                continuous scan operations, then starts the conversions.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_adc (void)
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
    PORT4.PMR.BYTE = 0x01;

    /* Set single scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x0;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 0x1u;

    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 0x1u;
}
/*******************************************************************************
* End of function init_adc
*******************************************************************************/

/*******************************************************************************
* Function Name: init_timer
* Description  : Configures CMT channel 1 to call the 'CB_TimerADC' callback
*                function which starts the AD conversion
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CMT1 module clock stop mode */
    MSTP_CMT1 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set CMT1 interrupt priority level to 10 */
    IPR(CMT1,CMI1) = 0xAu;

    /* Enable CMT1 interrupts */
    IEN(CMT1,CMI1) = 0x1u;

    /* Clear CMT1 interrupt flag */
    IR(CMT1,CMI1) = 0x0;

    /* Select the PCLK clock division as PCLK/512 = 39.06KHz */
    CMT1.CMCR.BIT.CKS = 0x3u;

    /* Specify the timer period */
    CMT1.CMCOR = g_cmt_period;

    /* Enable the compare match interrupt */
    CMT1.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT1 count */
    CMT.CMSTR0.BIT.STR1 = 0x1u;
}
/*******************************************************************************
* End of function init_timer
*******************************************************************************/

/******************************************************************************
* Function Name: Excep_S12AD0_S12ADI0_isr
* Description  : S12AD interrupt handler. This function is executed after every
*                succesful AD conversion. The function fetches the ADC result,
*                and uses it to calculate a new timer period for the LED
*                flash/IWDT refresh ISR.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_S12AD_S12ADI0
__interrupt  void Excep_S12AD0_S12ADI0_isr(void)
{
    /* Declare temporary ADC result container variable */
    uint16_t adc_result = 0x0;

    /* Fetch the result from the ADC conversion */
    adc_result = S12AD.ADDR0;

    /* Set the timer period global variable value relative to the ADC result.
       An increase of 4 in ADC value is approximately equivalent to 1ms */
    g_cmt_period = (uint16_t)((adc_result + 20u) * 7u);
}
/*******************************************************************************
* End of function Excep_S12AD0_S12ADI0_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_CMTU0_CMT1_isr
* Description  : Timer period interrupt handler function. This function resets
*                the watchdog timer to prevent it underflowing, and toggles the
*                user LEDs. The function also changes the time period between
*                timer interrupts, by changing the timer period to the value
*                stored in the variable, g_cmt_period.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_CMT1_CMI1
__interrupt  void Excep_CMTU0_CMT1_isr(void)
{
    /* Prevent the independent watchdog timer from
       underflowing by refreshing the count-down counter */
    IWDT.IWDTRR = 0x00u;
    IWDT.IWDTRR = 0xFFu;

    /* Toggle user LEDs */
    LED0 = (uint8_t) ~LED0;
    LED1 = (uint8_t) ~LED1;
    LED2 = (uint8_t) ~LED2;
    LED3 = (uint8_t) ~LED3;

    /* Trigger AD conversion to start */
    S12AD.ADCSR.BIT.ADST = 0x1u;

    /* Reconfigure the timer interrupt period with the value of the
       global variable g_cmt_period */
    CMT1.CMCOR = g_cmt_period;
}
/*******************************************************************************
* End of function Excep_CMTU0_CMT1_isr
*******************************************************************************/

