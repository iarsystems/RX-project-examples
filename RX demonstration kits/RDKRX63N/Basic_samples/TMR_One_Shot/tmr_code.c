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
* File Name     : TMR_Normal_mode.c
* Version       : 1.00
* Device(s)     : RX6xx
* Description   : Defines TMR functions used for the TMR One Shot demo.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 02.12.2011     1.00        First release
*           23.01.2012                 Ported to FIT workspace. CS revisions.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>
#include "tmr_code.h"

/*******************************************************************************
Imported external functions.
*******************************************************************************/
extern void tmr_callback(void);

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function name : tmr_start
* Description   : Create a one-second one-shot timer based on PCLK (48 MHz)
* Arguments     : none
* Return Value  : none
******************************************************************************/
void tmr_start (void)
{
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  /* Power up the TMR unit using the appropriate Module Stop Control Register */
  MSTP(TMR0) = 0;
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* Timer Control Register (TCR) 
  b7      CMIEB
  b6      CMIEA: 1 = compare match A interrupt requests are enabled
  b5      OVIE
  b4:b3   CCLR:  0 = TCNT clearing disabled
  b2:b0   Reserved. The write value should always be 0.
  */
  TMR0.TCR.BYTE = 0x40; /* compare match A interrupt enabled on the high-order counter of the pair */
  TMR1.TCR.BYTE = 0x00;
  
  /* Timer Counter Control Register (TCCR)
  b7      TMRIS
  b6:b5   Reserved. The write value should always be 0.
  b4:b3   CSS: 3 = cascade TMR 0 and TMR 1
  b2:b0   CKS: 2 = Count at PCLK / 8192
  */
  TMR01.TCCR = 0x180E; /* cascade TMR0 and TMR1, TMR0 count when TMR1 overflows, TMR1 counts from PCLK / 8192 */
  
  /* Timer Control/Status Register (TCSR)
  b7:b5   Reserved. The write value should always be 1.
  b4      ADTE
  b3:b2   OSB
  b1:b0   OSA
  */
  TMR0.TCSR.BYTE = 0xE0;
  TMR1.TCSR.BYTE = 0xE0;
  
  /* Timer Constant Registers (TCORA and/or TCORB)
  *  b15:b8 Timer Constant Register 0
  *  b07:b0 Timer Constant Register 1
  */  
  TMR01.TCORA = 48000000 / 8192; /* delay length in timer counts. */ 
  /* If PCLK = 48 MHz and we are using PCLK / 8192 to increment the timer then */
  /* one count  =      8,192 /    48 =   170.667 uS */
  /* one second = 48,000,000 / 8,192 = 5,859.375 counts */
  
  /* Clear counter register */
  TMR01.TCNT = 0;
  
  IR (TMR0, CMIA0);     /* interrupt reset */
  IPR(TMR0, CMIA0) = 4; /* interrupt priority set */
  IEN(TMR0, CMIA0) = 1; /* interrupt enable */
} /* End of function tmr_start() */

/******************************************************************************
* Function name : tmr_stop
* Description   : Stops the 8-bit timers
* Arguments     : 
* Return Value  : 
******************************************************************************/
void tmr_stop (void)
{
  /* Timer Counter Control Register (TCCR)
  b7      TMRIS
  b6:b5   Reserved. The write value should always be 0.
  b4:b0   CSS:CKS 0x00 = Clock input prohibited
  */
  TMR0.TCCR.BYTE = 0x00; /* stop clock 0 */
  TMR1.TCCR.BYTE = 0x00; /* stop clock 1 */
} /* End of function tmr_stop() */

/******************************************************************************
* Function name: tmr_isr
* Description  : Sample ISR for TMR interrupt
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_TMR0_CMIA0
__interrupt void tmr_isr (void) 
{
  tmr_stop();              /* Stop the 8-bit TMR */
  
  tmr_callback();
} /* End of tmr_isr() */

/*******************************************************************************
* End of file: TMR_code.c
*******************************************************************************/
