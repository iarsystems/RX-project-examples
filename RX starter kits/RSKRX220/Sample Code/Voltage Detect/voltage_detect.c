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
* File Name     : voltage_detect.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines voltage detection functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides C99 type definitions. */
#include <stdint.h>

/* Defines standard boolean variable types */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Provides declarations for functions defined in this file */
#include "voltage_detect.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* VCC above Vdet flag variable declaration */
volatile bool g_vcc_above_vdet = true;

/*******************************************************************************
* Function Name: init_voltage_detect
* Description  : Initialise the voltage detection registers and the NMI
*                interrupt.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_voltage_detect (void)
{
    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Set vdet1 to 4.00V */
    SYSTEM.LVDLVLR.BYTE = 0x01u;

    /* Disable digital filter */
    SYSTEM.LVD1CR0.BIT.LVD1DFDIS = 1u;

    /* Enable vdet1 interrupt */
    SYSTEM.LVD1CR0.BIT.LVD1RI = 0x0;

    /* Set vdet1 to trigger when VCC falls below the trigger value */
    SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 0x1u;

    /* Enable output comparison */
    SYSTEM.LVD1CR0.BIT.LVD1CMPE = 0x1u;

    /* Clear the vdet1 flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Ensure vdet flag is cleared */
    while (SYSTEM.LVD1SR.BIT.LVD1DET)
    {
        /* Do nothing until flag is clear */
    }

    /* Enable vdet1 interrupt */
    SYSTEM.LVD1CR0.BIT.LVD1RIE = 0x1u;

    /* Enable vdet1 */
    SYSTEM.LVCMPCR.BIT.LVD1E = 0x1u;

    /* Delay for vdet1 startup */
    volatile uint16_t delay = 0xFFFFu;
    while (delay--)
    {
        /* Do nothing until delay has elapsed */
    }

    /* Enable vdet1 NMI interrupt */
    ICU.NMIER.BIT.LVD1EN = 0x1u;

    /* Re-enable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTON;

}
/*******************************************************************************
* End of function init_voltage_detect
*******************************************************************************/

/*******************************************************************************
* Function Name: NonMaskableInterrupt_isr
* Description  : NMI interrupt handler. The function is called when an NMI
*                interrupt is triggered (voltage detection interrupt triggers
*                an NMI). If VCC < VDET, the function sets the voltage detect
*                point to above VDET and vice versa. The function also updates
*                the g_vcc_above_vdet flag.
* Arguments    : none
* Return Value : none
*******************************************************************************/
__interrupt void __NMI_handler (void)
{
    /* Disable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Clear vdet1 passage detection flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Clear NMI interrupt flag */
    ICU.NMICLR.BIT.LVD1CLR = 0x1u;

    /* Check if VCC is below trigger voltage */
    if (0 == SYSTEM.LVD1SR.BIT.LVD1MON)
    {
        /* Set vdet to trigger when VCC > trigger voltage */
        SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 0x00;

        /* Set global vdet status flag */
        g_vcc_above_vdet = false;
    }

    /* VCC is above trigger voltage */
    else
    {
        /* Set vdet to trigger VCC < trigger voltage */
        SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 0x01u;

        /* Set global vdet status flag */
        g_vcc_above_vdet = true;

        /* Turn off LED3 */
        LED3 = LED_OFF;
    }

    /* Enable write protection of the system registers */
    SYSTEM.PRCR.WORD = PROTECTON;
}
/*******************************************************************************
* End of function NonMaskableInterrupt_isr
*******************************************************************************/

/*******************************************************************************
* Function Name: led_flash_voltage_detect
* Description  : This function toggles all the user LEDs if g_vcc_above_vdet
*                flag is true, or turns LED3 and the other LEDs off if the flag
*                is false. The function also waits in a while loop to make the
*                LED toggle a visible flash.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void led_flash_voltage_detect (void)
{
    /* Check if g_vcc_above_vdet is true (VCC above trigger voltage) */
    if (g_vcc_above_vdet)
    {
        /* Toggle all user LEDs */
        LED0 = (uint8_t) ~LED0;
        LED1 = (uint8_t) ~LED1;
        LED2 = (uint8_t) ~LED2;
        LED3 = (uint8_t) ~LED3;

        /* Write VCC status to LCD */
        Display_LCD(LCD_LINE2, "VCC>VDET");
    }

    /* g_vcc_above_vdet is false (VCC below trigger voltage) */
    else
    {
        /* Turn off LED0 to LED2 */
        LED0 = LED_OFF;
        LED1 = LED_OFF;
        LED2 = LED_OFF;

        /* Turn on LED3 */
        LED3 = LED_ON;

        /* Write VCC status to LCD */
        Display_LCD(LCD_LINE2, "VCC<VDET");
    }

    /* Define wait count variable */
    uint32_t wait_count = 0x000FFFFFu;

    /* Decrement the count variable to create delay */
    while (wait_count--)
    {
        /* Do nothing until LED flash delay has elapsed */
    }
}
/*******************************************************************************
* End of function led_flash_voltage_detect
*******************************************************************************/

