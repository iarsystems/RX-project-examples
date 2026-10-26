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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : adc_mtu_timer.c
* Version       : 1.0
* Description   : Creates a settable timer on MTU0 that triggers AD conversions
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*           20.01.2012                 CS revisions. Ported to FIT environment.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <iorx63n.h>
#include "adc_mtu_timer.h"
#include "yrdkrx63n.h"


/******************************************************************************
* Function name: adc_mtu_init
* Description  : Initializes MTU0 for triggering other peripheral.
* Arguments    : sample_rate -
*                    the value that will be used to set TGRA
* Return value : none
******************************************************************************/
void adc_mtu_init(uint16_t sample_rate)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  MSTP(MTU0) = 0;    /* Clear module stop state bit to enable the MTU */
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* Timer Control Register (TCR) 
  b7:b5   CCLR - TCNT counter cleared by TGRA compare match
  b4:b3   CKEG - Count at rising edge
  b2:b0   TPSC - Internal clock: counts on PCLK/1
  */
  MTU0.TCR.BYTE = 0x20;
  
  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU0.TMDR.BYTE = 0x00;
  
  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU0.TIORH.BYTE = 0x00;
  MTU0.TIORL.BYTE = 0x00;
  
  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation enabled
  b6      reserved
  b5      TCIEU - Underflow interrupts disabled
  b4      TCIEV - Overflow interrupts disabled
  b3      TGIED - TGRD interrupt disabled
  b2      TGIEC - TGRC interrupt disabled
  b1      TGIEB - TGRB interrupt disabled
  b0      TGIEA - TGRA interrupt enabled
  */
  MTU0.TIER.BYTE = 0x81;
  
  /* Timer general register A
  For 48MHz plck, triggers ADC conversions at a rate of 1/48,000,000 * this value
  Sample values:
  48000 =   1.0 mS =   1 kHz
  4800  = 100.0 uS =  10 kHz
  1000  =  20.83uS =  48 kHz 
  480   =  10.0 uS = 100 kHz
  240   =   5.0 uS = 200 kHz
  120   =   2.5 uS = 400 kHz
  */
  MTU0.TGRA = sample_rate;
} /* End of function adc_mtu_init() */


/******************************************************************************
* Function name: adc_mtu_start
* Description  : Start the MTU
* Arguments    : none
* Return value : none
******************************************************************************/
void adc_mtu_start(void)
{
  /* Start MTU0 */
  MTU.TSTR.BIT.CST0 = 1;
} /* End of function adc_mtu_start() */


/******************************************************************************
* Function name: adc_mtu_stop
* Description  : Stop the MTU
* Arguments    : none
* Return value : none
******************************************************************************/
void adc_mtu_stop(void)
{
  /* Stop MTU0 */
  MTU.TSTR.BIT.CST0 = 0;
} /* End of function adc_mtu_stop() */

/*******************************************************************************
* End of file adc_mtu_timer.c
*******************************************************************************/
