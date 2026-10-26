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
* File Name     : analog_compare.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines the analog comparator configuration functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "analog_compare.h"

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Name: init_analog_compare
* Description  : This function configures the analog comparator A
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_analog_compare (void)
{
    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Select external reference voltage for comparator */
    SYSTEM.LVCMPCR.BIT.EXVREFINP1 = 0x1u;

    /* Select external input voltage for comparator */
    SYSTEM.LVCMPCR.BIT.EXVCCINP1 = 0x1u;

    /* Set detection condition to be both rising and falling */
    SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 0x2u;

    /* Use maskable interrupt */
    SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 0x1u;

    /* Enable output comparison */
    SYSTEM.LVD1CR0.BIT.LVD1CMPE = 0x1u;

    /* Enable comparator interrupt */
    SYSTEM.LVD1CR0.BIT.LVD1RIE = 0x1u;

    /* Enable comparator A1 */
    SYSTEM.LVCMPCR.BIT.LVD1E = 0x1u;

    /* Clear the comparator status detection flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Ensure detection flag is cleared */
    while (SYSTEM.LVD1SR.BIT.LVD1DET)
    {
        /* Wait until flag is clear */
    }

    /* Enable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set priority level on comparator interrupt requests */
    IPR(CMPA, CMPA1) = COMPARATOR_IPL;

    /* Enable comparator A1 interrupt requests */
    IEN(CMPA, CMPA1) = 0x1u;

    /* Clear interrupt flag */
    IR(CMPA, CMPA1) = 0x0;

}
/******************************************************************************
* End of function init_analog_compare
******************************************************************************/

/******************************************************************************
* Function Name: Excep_LVD1_isr
* Description  : Interrupt service routine for the CMPA comparator interrupt.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_LVD_LVD1
__interrupt  void Excep_LVD1_isr(void)
{
    /* Display comparator reference on the LCD  */
    Display_LCD(LCD_LINE1, "COMPA1  ");

    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Clear the comparator detection status flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Ensure detection flag is cleared */
    while (SYSTEM.LVD1SR.BIT.LVD1DET)
    {
        /* Wait until detection flag has cleared */
    }

    /* Enable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Check if the CMPA1 voltage has risen above the reference voltage */
    if (SYSTEM.LVD1SR.BIT.LVD1MON)
    {
        /* Turn off LED2 & LED3 */
        LED2 = LED_OFF;
        LED3 = LED_OFF;

        /* Turn on LED0 & LED1  */
        LED0 = LED_ON;
        LED1 = LED_ON;

        /* Display the comparison result on LCD */
        Display_LCD(LCD_LINE2, "VI>=VREF");
    }

    /* The CMPA1 voltage is less than the reference voltage */
    else
    {
        /* Turn off LED2 & LED3 */
        LED0 = LED_OFF;
        LED1 = LED_OFF;

        /* Turn on LED0 & LED1  */
        LED2 = LED_ON;
        LED3 = LED_ON;

        /* Display the comparison result on LCD */
        Display_LCD(LCD_LINE2, "VI<VREF ");
    }

    /* Wait for approx 500ms to avoid flickering on the display in case the
       input supply is not stable. */
    DisplayDelay(500000u);
}
/******************************************************************************
* End of function Excep_LVD1_isr
******************************************************************************/

