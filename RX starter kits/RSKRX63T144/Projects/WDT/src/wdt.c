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
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines WDT, ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard C99 integer types used in this file */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines MCU device registers used in this file */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Declares WDT functions defined in this file */
#include "wdt.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Local CMT initialisation function prototype */
static void init_cmt_wdt(void);

/* Local ADC initialisation function prototype */
static void init_adc_wdt(void);

/* Local WDT initialisation function prototype */
static void init_wdt(void);

/******************************************************************************
* Function Name : Init_WDT_Sample
* Description   : This function groups together the ADC, CMT and WDT inital-
*                 isation functions; and calls them one by one.
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_WDT_Sample (void)
{
    /* Initialise the ADC unit */
    init_adc_wdt();

    /* Initialise the CMT unit */
    init_cmt_wdt();

    /* Initialise the WDT unit */
    init_wdt();
}
/******************************************************************************
* End of function Init_WDT_Sample
******************************************************************************/

/******************************************************************************
* Function Name : init_wdt
* Description   : This function initialises the WDT registers, and configures
*                 WDT unit to trigger an NMI interrupt when the WDT count
*                 underflows.
* Argument      : none
* Return value  : none
******************************************************************************/
static void init_wdt (void)
{
    /* Set time out period to 8192 cycles
       Set WDT clock divisor to PCLK/2048
       Set window end position to 0%
       Set window start position to 100% */
    WDT.WDTCR.WORD = 0x3372;

    /* Set WDT overflow to trigger NMI interrupt */
    WDT.WDTRCR.BYTE = 0x00;

    /* Enable WDT triggered NMI interrupt */
    ICU.NMIER.BIT.WDTEN = 0x1;

    /* Start WDT by refreshing the counter */
    WDT.WDTRR = 0x00;
    WDT.WDTRR = 0xFF;
}
/******************************************************************************
* End of function init_wdt_wdt
******************************************************************************/

/******************************************************************************
* Function Name : init_adc_wdt
* Description   : The function configures the ADC unit to perform repeat
*                 conversions of the voltage from the potentiometer. The
*                 function also checks to see if the pot has been turned fully
*                 anti-clockwise, and waits until it has done.
* Argument      : none
* Return value  : none
******************************************************************************/
static void init_adc_wdt (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 0x1;

    /* Set ADCSR to continuous mode */
    S12AD.ADCSR.BIT.ADCS = 0x2;

    /* Set the sampling cycle to 255 states (approximately 5 us) */
    S12AD.ADSSTR0 = 0xFF;

    /* Start AD conversion */
    S12AD.ADCSR.BIT.ADST = 0x1;

    /* Wait for one conversion to complete */
    uint16_t wait = 0xFFF;
    while (wait--)
    {
        /* Wait */
    }

    /* Check if AD value is greater than 0x003 */
    if (S12AD.ADDR0 > 0x003)
    {
        /* Instruct user to rotate pot fully anti-clockwise */
        Display_LCD(LCD_LINE1, "Turn Pot");
        Display_LCD(LCD_LINE2, " Left   ");

        /* Wait until user turns pot fully anti-clockwise */
        while (S12AD.ADDR0 > 0x003)
        {
            /* Wait */
        }
    }
}
/******************************************************************************
* End of function init_adc_wdt
******************************************************************************/

/******************************************************************************
* Function Name : init_cmt_wdt
* Description   : The function configures a CMT unit to trigger a periodic
*                 interrupt, used to reset the WDT count.
* Argument      : none
* Return value  : none
******************************************************************************/
static void init_cmt_wdt (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set CMT2 interrupt priority level to 5 */
    IPR(CMT2,CMI2) = 0x5;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;

    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;

    /* Select the PCLK clock division as PCLK/512 */
    CMT2.CMCR.BIT.CKS = 0x3;

    /* Specify the timer period */
    CMT2.CMCOR = 0x0300;

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 0x1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x1;
}
/******************************************************************************
* End of function init_cmt_wdt
******************************************************************************/

/******************************************************************************
* Function Name : Excep_CMT2_CMI2
* Description   : CMT2 interrupt handler.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Fetch ADC value */
    const uint16_t adc_val = S12AD.ADDR0;

    /* Update the CMT compare match value relative to the ADC value */
    CMT2.CMCOR = (uint16_t) ((adc_val * 9u) + 0xF00u);

    /* Toggle all user LEDs */
    LED0 = (uint8_t)~LED0;
    LED1 = (uint8_t)~LED1;
    LED2 = (uint8_t)~LED2;
    LED3 = (uint8_t)~LED3;

    /* Refresh the WDT counter */
    WDT.WDTRR = 0x00;
    WDT.WDTRR = 0xFF;
}
/******************************************************************************
* End of function Excep_CMT2_CMI2
******************************************************************************/

/******************************************************************************
* Function Name : NonMaskableInterrupt
* Description   : NMI interrupt handler. Function is triggered by an NMI
*                 interrupt (triggered by WDT underflow). The function sets all
*                 user LEDs ON staticly, updates the debug LCD to indicate a
*                 WDT interrupt has occured and waits in an infinite while loop.
* Argument      : none
* Return value  : none
******************************************************************************/
__interrupt void __NMI_handler (void)
{
    /* Indicate WDT underflow on the debug LCD */
    Display_LCD(LCD_LINE2, "WDT IRQ ");

    /* Set all user LEDs on */
    LED0 = LED_ON;
    LED1 = LED_ON;
    LED2 = LED_ON;
    LED3 = LED_ON;

    /* Wait in infinite while loop */
    while (1)
    {
        /* Program waits here - reset to run again */
    }
}
/******************************************************************************
* End of function NonMaskableInterrupt
******************************************************************************/

