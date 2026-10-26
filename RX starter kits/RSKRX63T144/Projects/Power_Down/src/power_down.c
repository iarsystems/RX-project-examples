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
* File Name     : power_down.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines power down control functions.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines machine-level functions used in this file */
#include <machine.h>

/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"


/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines switch functions used in this file */
#include "switch.h"

/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* Declares functions defined in this file */
#include "power_down.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Enter standby flag variables */
bool g_enter_standby        = false;
bool g_enter_deep_standby   = false;

/*******************************************************************************
* Function Name: Init_PowerDown
* Description  : Initialise the Low Power Consumption registers for
*                software standby mode
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_PowerDown (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Enable software standby mode */
    SYSTEM.SBYCR.BIT.SSBY = 0x1;

    /* Disable deep software standby mode */
    SYSTEM.DPSBYCR.BIT.DPSBY = 0x0;

    /* Enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;
}
/*******************************************************************************
* End of function Init_PowerDown
*******************************************************************************/

/*******************************************************************************
* Function Name: EnterSwStandby_PowerDown
* Description  : Power down/wake sequence for SW Standby
* Argument     : none
* Return value : none
*******************************************************************************/
void EnterSwStandby_PowerDown (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* See the hardware manual 11.5.3.1 Transition to Software Standby Mode
       for an explanation of the following steps */
    /* Switch interrupts are already set to wake from standby.
       These have priority of 7, which is higher than set in the PSW.IPL[3:0] */

    /* Disable SW1 and SW2 interrupts. */
    IEN(ICU, IRQ0) = 0x0;
    IEN(ICU, IRQ1) = 0x0;

    /* check to see that the last register write worked ok */
    if (0x0 == IEN(ICU, IRQ1))
    {
        /* Display mode to LCD */
        Display_LCD(LCD_LINE2, "SWSTBY  ");

        /* Trigger MCU power down by calling the brk function - this causes a break
           exception which then issues the WAIT - see Excep_BRK() in intprg.c */
        brk();

        /* MCU will wake from power down mode here */
        /* Execute nop before proceeding to ensure MCU is ready */
        nop();

        Display_LCD(LCD_LINE2, "Awake  ");

    }
    else
    {
        Display_LCD(LCD_LINE2, "STBYFail");
    }

    /* Restore default switch interrupt configurations */
    InitialiseSwitchInterrupts();

    /* Enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;


}
/*******************************************************************************
* End of function EnterSwStandby_PowerDown
*******************************************************************************/

/*******************************************************************************
* Function Name: EnterDeepSwStandby_PowerDown
* Description  : Power down sequence for Deep SW Standby
* Argument     : none
* Return value : none
*******************************************************************************/
void EnterDeepSwStandby_PowerDown (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* See the hardware manual 11.5.4 Deep Software Standby Mode
       for an expalnation of the following steps */
    /* Enable deep software standby mode */
    SYSTEM.DPSBYCR.BIT.DPSBY = 0x1;

    /* Configure the cancelling of deep software standby mode using SW3 */
    SYSTEM.DPSIER0.BYTE = 0x04;

    /* Need a wait of at least 6 PBCLK cycles ahere as specified in HW
       manual section 11.2.10, before we Clear the DPSIFR0 flag from
       any previous wake from deep sw standby. This is achieved with the
       following register write */
    SYSTEM.DPSIER2.BYTE = 0x00;

    /* Clear the Interrupt flag before we go to deep sw standby */
    SYSTEM.DPSIFR0.BIT.DIRQ2F = 0x0;

     /* Enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Display mode to LCD */
    Display_LCD(LCD_LINE2, "DPSWSTBY");


    /* Trigger MCU power down by calling the brk function - this causes a break
       exception which then issues the WAIT - see Excep_BRK() in intprg.c */
    brk();

    /* We will never get here, as a wake from deep software standby causes a
       reset */


}
/*******************************************************************************
* End of function EnterDeepSwStandby_PowerDown
*******************************************************************************/


/*******************************************************************************
* Function Name: DisplayResetSource
* Description  : If wake from deep sw standy is detected, clear flags and
*                display "DeepWake " on LCD
* Argument     : none
* Return value : none
*******************************************************************************/
void DisplayResetSource (void)
{
    /* Check the state of the DPSIFR0 flag for SW3 interrupt source */
    if ((0x1 == SYSTEM.DPSIFR0.BIT.DIRQ2F) && (0x1 == SYSTEM.RSTSR0.BIT.DPSRSTF))
    {
        /* Disable register protection */
        SYSTEM.PRCR.WORD = PROTECTOFF;

        /* Clear the interrupt flag registers */
        SYSTEM.DPSIFR0.BIT.DIRQ2F = 0x0;
        SYSTEM.RSTSR0.BIT.DPSRSTF = 0x0;

         /* Enable register protection */
        SYSTEM.PRCR.WORD = PROTECTON;

        // Display wake from deep standby */
        Display_LCD(LCD_LINE2, "DeepWake");
    }

}
/*******************************************************************************
* End of function DisplayResetSource
*******************************************************************************/


/******************************************************************************
* Function Name : CB_Switch_Press
* Description   : Switch press callback funtion. Checks for SW press and if
*                 SW1 sets the g_enter_deep_standby flag, if SW2 sets the
*                 g_enter_standby flag.
* Argument      : none
* Return value  : none
******************************************************************************/
void CB_Switch_Press (void)
{
    /* Check if switch 1 was pressed */
    if ((g_switch_flag & 0x0F) & SWITCHHOLD_1)
    {
        /* Set the g_enter_deep_standby flag */
        g_enter_deep_standby = true;

        /* Clear switch flag */
        g_switch_flag = 0x0;
    }
    else if ((g_switch_flag & 0x0F) & SWITCHHOLD_2)
    {
        /* Set the g_enter_standby flag */
        g_enter_standby = true;

        /* Clear switch flag */
        g_switch_flag = 0x0;
    }
    else
    {
        /* Do nothing */
    }
}
/******************************************************************************
* End of function CB_Switch_Press
******************************************************************************/

/******************************************************************************
* Function Name : ToggleLEDs
* Description   : Function waits for approximately 100ms, then toggles the
*                 state of the user LEDs.
* Argument      : none
* Return value  : none
******************************************************************************/
void ToggleLEDs (void)
{
    /* Declare wait counter variable */
    volatile long wait = 0xFFFFF;

    /* Wait in while loop until counter variable reaches 0 */
    while (wait--)
    {
         /* Wait */
    }

    /* Toggle LEDs 0 - 3 */
    LED0 = (uint8_t)~LED0;
    LED1 = (uint8_t)~LED1;
    LED2 = (uint8_t)~LED2;
    LED3 = (uint8_t)~LED3;
}
/******************************************************************************
* End of function ToggleLEDs
******************************************************************************/

