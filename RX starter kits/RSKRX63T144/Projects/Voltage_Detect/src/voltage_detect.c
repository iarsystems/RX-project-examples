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
* File Name     : voltage_detect.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines voltage detection functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
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
/* Defines R5F563TE RSK defines */
#include "rskrx63tdef.h"

/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Provides declarations for functions defined in this file */
#include "voltage_detect.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Local Variables
*******************************************************************************/
/* VCC above Vdet flag variable declaration */
static volatile bool vcc_above_vdet = true;

/*******************************************************************************
* Function Name : Init_VoltageDetect
* Description   : Initialise the voltage detection registers and the NMI
*                 interrupt.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_VoltageDetect (void)
{
    /* Disable write protection of the system registers related to the LVD */
    SYSTEM.PRCR.WORD = 0xA508;

    /* Set vdet1 to 4.5V */
    SYSTEM.LVDLVLR.BYTE = 0x0A;

    /* Disable digital filter */
    SYSTEM.LVD1CR0.BIT.LVD1DFDIS = 0x1;

    /* Enable vdet1 interrupt */
    SYSTEM.LVD1CR0.BIT.LVD1RI = 0x0;

    /* Set vdet1 to trigger when VCC is below trigger value */
    SYSTEM.LVD1CR1.BYTE = 0x02;

    /* Enable output comparison */
    SYSTEM.LVD1CR0.BIT.LVD1CMPE = 0x1;

    /* Clear the vdet1 flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Ensure vdet flag is cleared */
    while (0x1 == SYSTEM.LVD1SR.BIT.LVD1DET)
    {
        /* Wait */
    }

    /* Enable vdet1 interrupt */
    SYSTEM.LVD1CR0.BIT.LVD1RIE = 0x1;

    /* Enable vdet1 */
    SYSTEM.LVCMPCR.BIT.LVD1E = 0x1;

    /* Enable vdet1 NMI interrupt */
    ICU.NMIER.BIT.LVD1EN = 0x1;

    /* Re-enable write protection of the system registers */
    SYSTEM.PRCR.BIT.PRC3 = 0xA500;
}
/*******************************************************************************
* End of function Init_VoltageDetect
*******************************************************************************/

/*******************************************************************************
* Function Name : NonMaskableInterrupt
* Description   : NMI interrupt handler. The function is called when an NMI
*                 interrupt is triggered (voltage detection interrupt triggers
*                 an NMI). If VCC < VDET, the function sets the voltage detect
*                 point to above VDET and vice versa. The function also updates
*                 the vcc_above_vdet flag.
* Argument      : none
* Return value  : none
*******************************************************************************/
__interrupt void __NMI_handler (void)
{
    /* Clear vdet1 passage detection flag */
    SYSTEM.LVD1SR.BIT.LVD1DET = 0x0;

    /* Clear NMI interrupt flag */
    ICU.NMICLR.BIT.LVD1CLR = 0x1;

    /* Check if VCC is below trigger voltage */
    if (0x0 == SYSTEM.LVD1SR.BIT.LVD1MON)
    {
        /* Set vdet to trigger when VCC > trigger voltage */
        SYSTEM.LVD1CR1.BYTE = 0x00;

        /* Set global vdet status flag */
        vcc_above_vdet = false;
    }
    /* VCC is above trigger voltage */
    else
    {
        /* Set vdet to trigger VCC < trigger voltage */
        SYSTEM.LVD1CR1.BYTE = 0x02;

        /* Set global vdet status flag */
        vcc_above_vdet = true;

        /* Turn off LED3 */
        LED3 = LED_OFF;
    }
}
/*******************************************************************************
* End of function NonMaskableInterrupt
*******************************************************************************/

/*******************************************************************************
* Function Name : LedFlash_VoltageDetect
* Description   : This function toggles all the user LEDs if vcc_above_vdet flag
*                 is true, or turns LED3 and the other LEDs off if the flag is
*                 false. The function also waits in a while loop to make the
*                 LED toggle a visible flash.
* Argument      : none
* Return value  : none
*******************************************************************************/
void LedFlash_VoltageDetect (void)
{
    /* Check if vcc_above_vdet is true (VCC above trigger voltage) */
    if (vcc_above_vdet)
    {
        /* Toggle all user LEDs */
        LED0 = (uint8_t)~LED0;
        LED1 = (uint8_t)~LED1;
        LED2 = (uint8_t)~LED2;
        LED3 = (uint8_t)~LED3;

        /* Write VCC status to LCD */
        Display_LCD(LCD_LINE2, "VCC>VDET");
    }
    /* vcc_above_vdet is false (VCC below trigger voltage) */
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
    uint32_t wait_count = 0x000FFFFF;

    /* Decrement the count variable to create delay */
    while (wait_count--)
    {
        /* Wait */
    }
}
/*******************************************************************************
* End of function LedFlash_VoltageDetect
*******************************************************************************/

