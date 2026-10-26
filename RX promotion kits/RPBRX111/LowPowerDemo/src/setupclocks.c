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
* File Name     : setupclocks.c
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Routines for setting up the clock mode
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the nop() function */
#include    <machine.h>

/* Standard integer types with specified widths definitions */
#include    <stdint.h>

/* Register address definitions */
#include <iorx111.h> 

/* Clock Mode setup function prototypes */
#include    "setupclocks.h" 

/* Hardware platform definitions */
#include    "rsk_defines.h"
 
/*******************************************************************************
* Function Name : set_clock_mode
* Description   : Configures clocks for selected operation modes.
* Argument      : uint8_t clock_mode indicates Clock Mode setting
*               : uint8_t cpu_calc indicates CPU load setting
* Return value  : none
*******************************************************************************/
void set_clock_mode (const uint8_t clock_mode, const uint8_t cpu_calc)
{
    volatile uint32_t i;

    /* Enable or Disable the High Speed On-Chip Oscillator
        according to Clock Mode */
    if (SUB_32k == clock_mode)
    {
        /* Turn off the High Speed On-Chip Oscillator power supply */    
        SYSTEM.HOCOCR.BYTE = 0x01;
    }
    else if ((HOCO_32M == clock_mode) || (DP_SLEEP == clock_mode) ||
              (SW_STBY == clock_mode) || (SW_SLEEP == clock_mode) ||
              (LOCO_4M == clock_mode) )
    {
        /* Turn on the High Speed On-Chip Oscillator power supply */    
        SYSTEM.HOCOCR.BYTE = 0x00;
        
        /* wait for High Speed On-Chip Oscillator to stabilise */
        for (i = 0; i < 100; i++)
        {
            nop();
        }
    }
    else
    {
        /* Do nothing */
    }

    /* Check if selected mode uses sub clock*/
    if ((SUB_32k == clock_mode) || (DP_SLEEP == clock_mode) ||
       (SW_SLEEP == clock_mode))
    {        
         /* Specify high drive for the Sub Clock circuit */
        RTC.RCR3.BIT.RTCDV = 1;

        /* Start the subclock */
        SYSTEM.SOSCCR.BIT.SOSTP = 0x0;
    }
    else
    {
        /* Ensure sub clock is stopped */
        SYSTEM.SOSCCR.BIT.SOSTP = 0x1;
    }

    /* Specify the clock dividers for ICLK, PCLKB, PCLKD, BCLK & FCLK based upon
       the measurement type we wish to do.
       The types are
       CPU_MIN
       CPU_TYP
       CPU_MAX
       See rsk_defines.h for more info.
    */

    /* Set System Clock Control Register for SUB_32k or LOCO_4M  */
    if ((SUB_32k == clock_mode) || (LOCO_4M == clock_mode))
    {
        /* Set values according to cpu_calc */
        if (CPU_MIN == cpu_calc)
        {
            /* Set Peripheral Module Clock D to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKD     = 6;

            /* Set Peripheral Module Clock B to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKB     = 6;

            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK      = 0;

            /* Set FlashIF Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.FCK     = 6;             
        }
        else
        {
            /* Set Peripheral Module Clock D to x 1 */
            SYSTEM.SCKCR.BIT.PCKD     = 0;

            /* Set Peripheral Module Clock B to x 1 */
            SYSTEM.SCKCR.BIT.PCKB     = 0;

            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK      = 0;

            /* Set FlashIF Clock to x 1 */
            SYSTEM.SCKCR.BIT.FCK      = 0;
        }
    }

    /* Set System Clock Control Register for HOCO_32M  */
    else if (HOCO_32M == clock_mode)
    {
        if (CPU_MIN == cpu_calc)
        {
            /* Set Peripheral Module Clock D to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKD     = 6;

            /* Set Peripheral Module Clock B to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKB     = 6;

            /* Set System Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.ICK      = 5;

            /* Set FlashIF Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.FCK      = 6;
        }
        else
        {
            /* Set Peripheral Module Clock D to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKD     = 5;

            /* Set Peripheral Module Clock B to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKB     = 5;

            /* Set System Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.ICK      = 5;

            /* Set FlashIF Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.FCK      = 5;
        }
    }

    /* Set System Clock Control Register for SW_DP_STBY, SW_STBY & SW_SLEEP */           
    else
    {
        /* Set Peripheral Module Clock D to x 1 */
        SYSTEM.SCKCR.BIT.PCKD     = 0;

        /* Set Peripheral Module Clock B to x 1 */
        SYSTEM.SCKCR.BIT.PCKB     = 0;

        /* Set System Clock to x 1 */
        SYSTEM.SCKCR.BIT.ICK     = 0;

        /* Set FlashIF Clock to x 1 */
        SYSTEM.SCKCR.BIT.FCK     = 0;
    } 

    /* Switch from Low Speed On-Chip Oscillator to the desired clock */
    if (SUB_32k == clock_mode)
    {
        /* Use Subclock */
        SYSTEM.SCKCR3.BIT.CKSEL = 0x3;

        /* Stop the LOCO */
        SYSTEM.LOCOCR.BIT.LCSTP = 0x1;
    }
    else if ((DP_SLEEP == clock_mode) ||
             (SW_STBY == clock_mode)  ||
             (SW_SLEEP == clock_mode) ||
             (HOCO_32M == clock_mode))
    {
        /* Use High Speed On Chip Oscillator */
        SYSTEM.SCKCR3.BIT.CKSEL = 0x1;

        /* Stop the LOCO */
        SYSTEM.LOCOCR.BIT.LCSTP = 0x1;
    }
    else
    {
        /* Use Low Speed On Chip Oscillator */
        SYSTEM.SCKCR3.BIT.CKSEL = 0x0;
    }

    /* Wait for the Operating Power Control Mode Transition Status Flag
       in the Operating Power Control Register before continuing */
    while (SYSTEM.OPCCR.BIT.OPCMTSF == 1)
    {

    }
    
    /* Match the Operating Power Control Mode to the
       chosen frequency and voltage
       SUB_32k  : Low-speed operating mode
       LOCO_4M  : Middle-speed operating mode
       HOCO_32M : High-speed operating mode     */

    /* Set Operating Power Control Mode in the
       Operating Power Control Register
       for SUB_32k Clock Mode */
    if (SUB_32k == clock_mode)
    {
        /* Set Low Speed Operating Mode */
        SYSTEM.SOPCCR.BIT.SOPCM = 0x1;
    }
    /* Set Operating Power Control Mode in the Operating Power Control Register
       for LOCO_4M Clock Mode */
    else if (LOCO_4M == clock_mode)
    {
        /* Set Middle Speed Operating Mode */
        SYSTEM.OPCCR.BIT.OPCM = 0x2;
    }

    /* Set Operating Power Control Mode in the Operating Power Control Register
       for HOCO_32M, SW_DP_STBY, SW_STBY & SW_SLEEP modes */
    else
    {
        /* Set High Speed Operating Mode */
        SYSTEM.OPCCR.BIT.OPCM = 0x00;        
    }


    /* Wait for the Operating Power Control Mode Transition Status Flag
       in the Operating Power Control Register before continuing */
    while ((1 ==SYSTEM.OPCCR.BIT.OPCMTSF) || (1 == SYSTEM.SOPCCR.BIT.SOPCMTSF))
    {

    }
}
/******************************************************************************
* End of function set_clock_mode
******************************************************************************/



