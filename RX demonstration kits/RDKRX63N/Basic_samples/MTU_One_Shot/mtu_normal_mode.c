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
/******************************************************************************
* File Name     : MTU_Normal_mode.c
* Version       : 1.00
* Description   : Defines MTU functions used for the MTU One Shot demo.
*******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 02.12.2011     1.00        First release
*           23.12.2012                 moved application code out of isr and
*                                      into external callback.
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>
#include "mtu_normal_mode.h"

/*******************************************************************************
Imported external functions 
*******************************************************************************/
extern void mtu_callback(void);

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function name : mtu_initialize
* Description   : Create a one-second one-shot timer based on PCLK (48 MHz)
* Arguments     : none
* Return Value  : none
******************************************************************************/
void mtu_initialize (void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  /* Cancel CMT module stop state. */
  MSTP(MTU2) = 0;  
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* Timer Control Register (TCR) 
  b7:b5   CCLR: Counter Clear Source 1 = TCNT cleared by TGRA compare match/input capture 
  b4:b3   CKEG: Clock Edge           0 = count at rising edge
  b2:b0   TPSC: Time Prescaler       7 = count on PCLK / 1024 (PCLK = 48 MHz) 
  */
  MTU2.TCR.BYTE = 0x27; 
  
  /* Timer Mode Register (TMDR)
  b6      BFE - TPUm.TGRE operates normally
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU2.TMDR.BYTE = 0x00;
  
  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU2.TIOR.BYTE = 0x00;
  
  /* Timer General Register (TGR)
  *  b15:b0 TGRx either output compare or input capture register.
  *         x may be A, B, C, or D.
  */  
  MTU2.TGRA = 48000000 / 1024; /* one second = PCLK (48 MHz) / divider set in TCR above (1024) */
  
  /* Timer Interrupt Enable Register (TIER)
  b7      TTEG - A/D conversion start request generation
  b6      reserved
  b5      TCIEU - Underflow interrupts
  b4      TCIEV - Overflow interrupts
  b3      TGIED - TGRD interrupt
  b2      TGIEC - TGRC interrupt
  b1      TGIEB - TGRB interrupt
  b0      TGIEA - TGRA interrupt
  */
  MTU2.TIER.BYTE = 0x01; /* Enable TGRA interrupt */
  
  /* Timer Synchronous Register (TSYRA)
  b7:b6   ---
  b5      SYNC5 - TCNT operates independently
  b4      SYNC4 - TCNT operates independently
  b3      SYNC3 - TCNT operates independently
  b2      SYNC2 - TCNT performs synchronous operation
  b1      SYNC1 - TCNT performs synchronous operation
  b0      SYNC0 - TCNT operates independently
  */
  MTU.TSYR.BYTE = 0x00; /* No TPU syncing */
  
  /* Clear counter register */
  MTU2.TCNT = 0x0000;
  
  IR (MTU2, TGIA2);     /* interrupt reset */
  IPR(MTU2, TGIA2) = 4; /* interrupt priority set */
  IEN(MTU2, TGIA2) = 1; /* interrupt enable */
}

/******************************************************************************
* Function name: mtu_start
* Description  : Starts the timer counting operation.
* Arguments    : none
* Return value : none
******************************************************************************/
void mtu_start (void)
{
  MTU2.TCNT      = 0x0000; /* Clear counter register */
  MTU.TSTR.BYTE |= 0x04;   /* Start TPU2 */
}

/******************************************************************************
* Function name: mtu_stop
* Description  : Stops the timer counting operation.
* Arguments    : none
* Return value : none 
******************************************************************************/
void mtu_stop (void)
{
  MTU.TSTR.BYTE &= ~0x04; /* Stop TPU2 */
}

/******************************************************************************
* Function name: mtu_isr
* Description  : Sample ISR for mtu interrupt (must do hardware setup first!)
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_MTU2_TGIA2
__interrupt void mtu_isr (void) 
{
  mtu_stop();
  
  mtu_callback();
}

/*******************************************************************************
* End of file: MTU_Normal_Mode.c
*******************************************************************************/
