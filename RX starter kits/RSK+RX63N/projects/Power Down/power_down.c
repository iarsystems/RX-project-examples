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
* File Name     : power_down.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines interrupt service routine functions for the switch
*                 interrupts. De-bounces switch and sets a key press flag. 
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard C99 boolean types */
#include <stdbool.h>
/* Defines standard C99 integer types */
#include <stdint.h>
/* Defines machine-level functions used in this file */
#include <machine.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N device registers used in this file */
#include "iorx63n.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines RX63N switches and LEDs */
#include "rskrx63ndef.h"
/* Declares functions defined in this file */
#include "power_down.h"

/*******************************************************************************
* Global Variables 
*******************************************************************************/
/* Enter standby flag variable */
bool gEnterStandby = false;

/*******************************************************************************
* Outline       : Init_PowerDown
* Description   : Initialise the power down registers.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_PowerDown(void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = 0xA50B;    

    /* Set interrupt wake sources */
    SYSTEM.DPSIER1.BYTE = 0xFF;
    SYSTEM.DPSIER2.BYTE = 0xFF;
}
/*******************************************************************************
* End of function Init_PowerDown
*******************************************************************************/

/*******************************************************************************
* Outline      : Excep_BRK
* Description  : Iterrupt routine to execute a WAIT instruction if 
*                 there was a BREAK
* Argument     : none
* Return value : none
*******************************************************************************/

#pragma vector=0
__interrupt void Excep_BRK(void)
{
  
  __wait_for_interrupt();
  
}    


/*******************************************************************************
* Outline       : EnterStandby_PowerDown
* Description   : Changes to the internal HOCO, and enters standby.
* Argument      : none
* Return value  : none
*******************************************************************************/
void EnterStandby_PowerDown(void)
{
    /* Disable register protection */
    SYSTEM.PRCR.WORD = 0xA50B;    
    
    /* Start oscillating the HOCO */
    SYSTEM.HOCOCR.BIT.HCSTP = 0;
    
    /* Wait for the HOCO to stabilise */
    Delay_us(20000u);
    
    /* Switch system clock to HOCO */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x1;
    
    /* Wait for the HOCO to stabilise */
    Delay_us(20000u);
    
    /* Turn off LED3 */
    LED3 = LED_OFF;
        
    /* Trigger MCU power down by calling the brk function */    
    brk();
        
    /* MCU will wake from power down mode here */
    
    /* Execute nop before proceeding to ensure MCU is ready */
    nop();
    
    /* Switch system clock to HOCO */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x1;
    
    /* Wait for the HOCO to stabilise */
    Delay_us(20000u);
}
/*******************************************************************************
* End of function EnterStandby_PowerDown
*******************************************************************************/
