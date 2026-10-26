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


/*
 * wdt.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */
 

/******************************************************************************
System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/ 
#include "wdt.h"


#if (AUTO_START_WDT==1)

/******************************************************************************
    Create WDT start value if Auto Start is specified
******************************************************************************/     
#pragma public_equ = "__OFS0", WDTRSTIRQ_NMI   | \
                               WDTRPSS_100     | \
                               WDTRPES_0       | \
                               WDTCKS_8192     | \
                               WDTTOPS_16384   | \
                               WDTSTRT_START   | \
                               0xe001ffff
#else

/******************************************************************************
    Create default value for OFS0. This is normally done in the linker
    configuration file, but is removed there in order to be able to set
    other values from the source code (which is done when WDT autostart
    is enabled).
******************************************************************************/     
#pragma public_equ = "__OFS0", 0xffffffff

/******************************************************************************
    Function     : InitWDT
    Description   : Initialises the WDT to generate an NMI on undeflow,
    not a RESET
    Argument    : none
    Return value : none
******************************************************************************/
void InitWDT(void)
{
  WDT.WDTCR.WORD = 0x3383;
  WDT.WDTRCR.BYTE = 0;    /* 0 = NMI, 1 = RESET       */
                          /* Error in User Manual.    */  
                          /* Reported to Japan        */
}

/******************************************************************************
    Function     : StartWDT
    Description   : Starts the WDT counting
    Argument    : none
    Return value : none
******************************************************************************/
void StartWDT(void)
{
  WDT.WDTRR = 0x00;
  WDT.WDTRR = 0xff;
}

#endif
