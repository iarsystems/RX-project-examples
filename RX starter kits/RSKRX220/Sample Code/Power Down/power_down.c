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
* File Name     : power_down.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines power-down control functions. 
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Defines machine-level functions used in this file */
#include <machine.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines RX220 switches and LEDs */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Declares functions defined in this file */
#include "power_down.h"

/*******************************************************************************
* Global Variables 
*******************************************************************************/

/* Enter standby flag variable */
bool gEnterStandby = false;

/*******************************************************************************
* Function Name: init_power_down
* Description  : Initialise the power down registers.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_power_down (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;  
    
    /* Enable software standby mode */
    SYSTEM.SBYCR.BIT.SSBY = 0x1u;
    
    /* Enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;

}
/*******************************************************************************
* End of function init_power_down
*******************************************************************************/

/*******************************************************************************
* Function Name: enter_standby_power_down
* Description  : PowerDown/Wake sequence 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void enter_standby_power_down (void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;  
    
    /* Start oscillating the HOCO */
    SYSTEM.HOCOCR.BIT.HCSTP = 0x0;
    
    /* Wait for the HOCO to stabilise */
    volatile uint16_t delay = HOCO_DELAY;
    while (delay--)
    {
        /* Do nothing until oscillator is stable */
    }
    
    /* Switch system clock to HOCO. This is required as the MCU cannot
       resume from standby using the main crystal oscillator. */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x1u;
    
    /* Wait for the HOCO to stabilise */
    delay = HOCO_DELAY;
    while (delay--)
    {
        /* Do nothing until clock is ready */
    }
        
    /* Trigger MCU power down by calling the brk function */    
    brk();
        
    /* MCU will wake from power down mode here */
    
    /* Execute nop before proceeding to ensure MCU is ready */
    nop();
    
    /* Switch system clock to HOCO */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x1u;
    
    /* Wait for the HOCO to stabilise */
    delay = HOCO_DELAY;
    while (delay--)
    {
        /* Do nothing until oscillator is stable */
    }
}
/*******************************************************************************
* End of function enter_standby_power_down
*******************************************************************************/

