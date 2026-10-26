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
 * wdt.h
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */
 
 
#ifndef _WDT_H 
#define _WDT_H

/******************************************************************************
    Macro to enable / disable WDT Auto Start Mode
******************************************************************************/ 
#define     AUTO_START_WDT  0

/******************************************************************************
    Macros to set the WDT value is using AutoStart Mode
******************************************************************************/ 
#define     WDTRSTIRQ_NMI   0x00000000  
#define     WDTRSTIRQ_RST   0x10000000  

#define     WDTRPSS_25      0x00000000
#define     WDTRPSS_50      0x04000000
#define     WDTRPSS_75      0x08000000
#define     WDTRPSS_100     0x0C000000

#define     WDTRPES_75      0x00000000
#define     WDTRPES_50      0x01000000
#define     WDTRPES_25      0x02000000
#define     WDTRPES_0       0x03000000

#define     WDTCKS_4        0x00100000
#define     WDTCKS_64       0x00400000
#define     WDTCKS_128      0x00F00000
#define     WDTCKS_512      0x00600000
#define     WDTCKS_2048     0x00700000
#define     WDTCKS_8192     0x00800000


#define     WDTTOPS_1024    0x00000000
#define     WDTTOPS_4096    0x00040000
#define     WDTTOPS_8192    0x00080000
#define     WDTTOPS_16384   0x000C0000

#define     WDTSTRT_HALT    0x00020000
#define     WDTSTRT_START   0x00000000


/******************************************************************************
 Function Prototypes
******************************************************************************/
void InitWDT(void);
void StartWDT(void);

#endif
