/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : Timer.c
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : 
* Description   : High-resolution timer that runs a PCLK rate for benchmarking
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
Include files
******************************************************************************/
#include <iorx62n.h>
#include "timer.h"
#include "machine.h"

/******************************************************************************
* Function name : TimerInit
* Description   : Sets up a 32-bit cascaded timer on TPU1 & TPU2 running @ PCLK
* Arguments     : none
* Return Value  : none
******************************************************************************/
void TimerInit (void)
{
  /* 
  Create a free running counter based on PCLK (48 MHz) 
  A 32-bit counter is created by cascading TPU channels TPU1 & TPU2
  */

  /* Power up TPU unit 0 */
  MSTP(MTU1) = 0 ;

  /* TPU1 is set to count falling edges of TPU2 overflows */

  /* Timer Control Register (TCR) 
  b7:b5   CCLR - TCNT counter cleared by counter clearing for another channel
  b4:b3   CKEG - Count at falling edge
  b2:b0   TPSC - Counts on TPU2.TCNT overflow/underflow
  */
  MTU1.TCR.BYTE = 0x67  ;

  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU1.TMDR.BYTE = 0x00 ;

  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU1.TIOR.BYTE = 0x00 ;

  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation disabled
  b6      reserved
  b5      TCIEU - Underflow interrupts disabled
  b4      TCIEV - Overflow interrupts disabled
  b3      TGIED - TGRD interrupt disabled
  b2      TGIEC - TGRC interrupt disabled
  b1      TGIEB - TGRB interrupt disabled
  b0      TGIEA - TGRA interrupt disabled
  */
  MTU1.TIER.BYTE = 0x00 ;

  /* TPU2 is set to count falling edges of PCLK/1 (48 MHz) */

  /* Timer Control Register (TCR) 
  b7:b5   CCLR - TCNT counter cleared by counter clearing for another channel
  b4:b3   CKEG - Count at falling edge
  b2:b0   TPSC - Internal clock: counts on PCLK/1
  */
  MTU2.TCR.BYTE = 0x60  ;

  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU2.TMDR.BYTE = 0x00 ;

  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU2.TIOR.BYTE = 0x00 ;

  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation disabled
  b6      reserved
  b5      TCIEU - Underflow interrupts disabled
  b4      TCIEV - Overflow interrupts disabled
  b3      TGIED - TGRD interrupt disabled
  b2      TGIEC - TGRC interrupt disabled
  b1      TGIEB - TGRB interrupt disabled
  b0      TGIEA - TGRA interrupt disabled
  */
  MTU2.TIER.BYTE = 0x00 ;

  /* Timer Synchronous Register (TSYRA)
  b7:b6   ---
  b5      SYNC5 - TCNT operates independently
  b4      SYNC4 - TCNT operates independently
  b3      SYNC3 - TCNT operates independently
  b2      SYNC2 - TCNT performs synchronous operation
  b1      SYNC1 - TCNT performs synchronous operation
  b0      SYNC0 - TCNT operates independently
  */
  MTUA.TSYR.BYTE = 0x06 ;   /* Sync TPU1 and TPU 2*/

  /* Clear counter register for both TPU channels synchronously */
  MTU1.TCNT = 0x0000 ;

}

/******************************************************************************
* Function name : TimerStart
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
void TimerStart (void)
{
  /* Clear counter register for both TPU channels synchronously */
  MTU1.TCNT = 0x0000 ;

  /* Start channels TPU1 and TPU2 */
  MTUA.TSTR.BYTE |= 0x06 ;
}

/******************************************************************************
* Function name : TimerStop
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
uint32_t TimerStop (void)
{
  /* Stop TPU1 and TPU2 to freeze end time */
  MTUA.TSTR.BYTE &= ~0x06 ;

  /* Make a 32-bit counter from the TPU cascaded channels */
  return ((MTU1.TCNT << 16) | (MTU2.TCNT)) ;
}
