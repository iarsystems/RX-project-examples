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
* File Name     : wdt.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines ADC and timer functions
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
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
#include "iorx63n.h"
/* Defines RSK switches and LEDs */
#include "rskrx63ndef.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares WDT functions defined in this file */
#include "wdt.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Local CMT initialisation function prototype */
static void InitCMT_WDT(void);
/* Local ADC initialisation function prototype */
static void InitADC_WDT(void);
/* Local WDT initialisation function prototype */
static void InitWDT_WDT(void);

/******************************************************************************
* Outline       : Init_WDT
* Description   : This function groups together the ADC, CMT and WDT inital-
*                  isation functions; and calls them one by one.
* Argument          : none    
* Return value  : none
******************************************************************************/
void Init_WDT(void)
{    
    /* Initialise the ADC unit */
    InitADC_WDT();
    
    /* Initialise the CMT unit */
    InitCMT_WDT();
    
    /* Initialise the WDT unit */
    InitWDT_WDT();    
}
/******************************************************************************
* End of function Init_WDT
******************************************************************************/

/******************************************************************************
* Outline       : InitCMT_WDT
* Description   : This function initialises the WDT registers, and configures
*                  WDT unit to trigger an NMI interrupt when the WDT count
*                  underflows. 
* Argument      : none    
* Return value  : none
******************************************************************************/
static void InitWDT_WDT(void)
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
* End of function InitWDT_WDT
******************************************************************************/

/******************************************************************************
* Outline        : InitADC_WDT
* Description    : The function configures the ADC unit to perform repeat
*                  conversions of the voltage from the potentiometer. The 
*                  function also checks to see if the pot. has been turned fully
*                  anti-clockwise, and waits until it has done.
* Argument          : none    
* Return value  : none
******************************************************************************/
static void InitADC_WDT(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
            
    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01;
    
    /* ADC clock = PCLK/8, continuous scan mode */
    S12AD.ADCSR.BYTE |= 0x40;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001;    
    
    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 1;
    
    /* Start AD conversion */
    S12AD.ADCSR.BIT.ADST = 1;
    
    /* Wait for one conversion to complete */
    uint16_t wait = 0xFFF;
    while(wait--);
    
    /* Check if AD value is greater than 0x003 */
    if(S12AD.ADDR0 > 0x003)
    {
        /* Instruct user to rotate pot fully anti-clockwise */
        Display_LCD(LCD_LINE1, "Turn Pot");
        Display_LCD(LCD_LINE2, " Left   ");
        
        /* Wait until user turns pot fully anti-clockwise */
        while(S12AD.ADDR0 > 0x003); 
        
        /* Display previous LCD message */
        Display_LCD(LCD_LINE1, "Watchdog");
        Display_LCD(LCD_LINE2, "        ");
    }
}
/******************************************************************************
* End of function InitADC_WDT
******************************************************************************/

/******************************************************************************
* Outline       : InitCMT_WDT
* Description   : The function configures a CMT unit to trigger a periodic
*                  interrupt, used to reset the WDT count.
* Argument      : none    
* Return value  : none
******************************************************************************/
static void InitCMT_WDT(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
        
    /* Enable CMT2 interrupts */
    ICU.IR[IR_CMT2_CMI2].BIT.IR = 0;    
    ICU.IPR[IPR_CMT2_CMI2].BYTE = 0x05;
    ICU.IER[IER_CMT2_CMI2].BIT.IEN6 = 1;    
        
    /* Select the PCLK clock division as PCLK/512 */ 
    CMT2.CMCR.BIT.CKS = 0x3;
    
    /* Specify the timer period */
    CMT2.CMCOR = 0x0300;        
    
    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;
}
/******************************************************************************
* End of function InitCMT_WDT
******************************************************************************/

/******************************************************************************
* Outline       : Excep_CMTU1_CMT2
* Description   : CMT1 interrupt handler. 
* Argument      : none    
* Return value  : none
******************************************************************************/
#pragma vector = VECT_CMT2_CMI2
__interrupt void Excep_CMTU1_CMT2(void)
{
    /* Fetch ADC value */
    uint16_t adc_val = S12AD.ADDR0;

    /* Update the CMT comapre match value relative to the ADC value */
    CMT2.CMCOR = (uint16_t) ((adc_val*15u) + 0xF00u);
    
    /* Toggle the user LEDs */
    if(LED_ON == LED0)
    {
        /* Set all user LEDs to off */
        LED0 = LED1 = LED2 = LED3 = LED_OFF;
    }
    else
    {
        /* Set all user LEDs to ON */
        LED0 = LED1 = LED2 = LED3 = LED_ON;
    }
    
    /* Refresh the WDT counter */
    WDT.WDTRR = 0x00;
    WDT.WDTRR = 0xFF;
}
/******************************************************************************
* End of function Excep_CMTU1_CMT2
******************************************************************************/

/******************************************************************************
* Outline       : NonMaskableInterrupt
* Description   : NMI interrupt handler. Function is triggered by an NMI 
*                  interrupt (triggered by WDT underflow). The function sets all
*                  user LEDs ON staticly, updates the debug LCD to indicate a
*                  WDT interrupt has occured and waits in an infinite while loop.
* Argument      : none    
* Return value  : none
******************************************************************************/

//IAR note: this is the hardcoded name for this handler
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
    while(1);    
}
/******************************************************************************
* End of function NonMaskableInterrupt
******************************************************************************/
