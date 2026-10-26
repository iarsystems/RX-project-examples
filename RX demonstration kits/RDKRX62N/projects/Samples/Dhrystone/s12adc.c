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
* File Name     : S12ADC
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RSK/RDK RX62n
* Description   : Low-level code for the 12-bit ADC
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
Include files
******************************************************************************/
#include "platform.h"
#include  <machine.h>
#include  <iorx62n.h>
#include "s12adc.h"


/******************************************************************************
Private (static) data
******************************************************************************/
uint16_t adcReading ;
uint32_t adcCount ;
uint32_t bufSize ;
uint16_t *pPingBuffer ;   /* Pointer to start of ping buffer */
uint16_t *pPongBuffer ;   /* Pointer to start of pong buffer */
uint16_t *pNextReading ;  /* Pointer to location for next ADC reading */
uint32_t hotBuffer ;      /* Toggles between BUFFER_PING & BUFFER_PONG */

uint32_t bufferLock ;     /* To grant exclusive access to ping or pong buffer */

/******************************************************************************
Prototypes for local functions
******************************************************************************/
void S12ADC_Error (void) ;


/******************************************************************************
* Function name : S12ADC_Init
* Description   : Sets up the 12-bit ADC
* Arguments     : none
* Return Value  : none
******************************************************************************/
void S12ADC_Init (uint16_t *pPing, uint16_t *pPong, uint32_t size)
{
  /* Save pointers to ping & pong buffers */
  pPingBuffer  = pPing ;
  pPongBuffer  = pPong ;
  bufSize      = size ;
  pNextReading = pPingBuffer ;  /* Start loading the ping buffer */
  adcCount     = 0 ;

  MSTP(S12AD)  = 0 ;

#ifdef PLATFORM_RSK62N
  PORT4.ICR.BIT.B0 = 1 ;        /* Turn on input buffer for the pin */
#else
  PORT4.ICR.BIT.B4 = 1 ;        /* Turn on input buffer for the pin */
#endif

  /* A/D Control Register - ADCSR
  b7    ADST:0  Stop a scan conversion process
  b6    ADCS:0  Single scan mode
  b5    ---
  b4    ADIE:1  Enables conversion complete IRQ to ICU
  b3:b2 CKS:3   PCLK
  b1    TRGE:1  Enables conversion to start w/ trigger
  b0    EXTRG:0 Scan conversion start by a timer source
  */
  S12AD.ADCSR.BYTE = 0x1E;

  /* A/D Channel Select Register - ADANS
  b15:b8  ---
  b7:b0   ANS: 1 in a bit indicates that channel is scanned
  */
#ifdef PLATFORM_RSK62N
  S12AD.ADANS.WORD = 0x01;    /* Scan only AD0 */
#else
  S12AD.ADANS.WORD = 0x10 ;   /* Scan only AD4 */
#endif

  /* A/D-converted Value Addition Mode Select Register - ADADS
  b15:b8  ---
  b7:b0   ADS: 1 in a bit indicates integration enabled for that channel
  */
  S12AD.ADADS.WORD = 0x00 ;   /* No integration */

  /* A/D-Converted Value Addition Count Select Register - ADADC
  b7:b2   ---
  b1:b0   ADC: 00 = 1 time conversion (same as normal conversion)
  */
  S12AD.ADADC.BYTE = 0x00 ;   /* 1-time conversion */

  /* A/D Control Extended Register (ADCER)
  b15     ADRFMT:0  Right align the data in the result registers
  b14:b6  ---
  b5      ACE:0 Disables automatic clearing of ADDRn after it is read
  b4:b0   ---
  */
  S12AD.ADCER.WORD = 0x00 ;

  /* A/D Start Triggger Select Register - ADSTRGR
  b7:b4   ---
  b3:b0   ADSTRS: 1, Compare match A from MTU0
  */
  S12AD.ADSTRGR.BYTE = 0x01 ;
}
/******************************************************************************
* Function name : S12ADC_Start
* Description   : Starts the 12-bit ADC by starting MTU0
* Arguments     : none
* Return Value  : none
******************************************************************************/
void S12ADC_Start()
{

  /*
  Creates a 100 uS timer on MTU0 that triggers AD conversions
  */

  /* Power up MTU unit 0 */
  MSTP(MTU0) = 0 ;

  /* MTU0 is set to count falling edges of PCLK */

  /* Timer Control Register (TCR)
  b7:b5   CCLR - TCNT counter cleared by TGRA compare match
  b4:b3   CKEG - Count at rising edge
  b2:b0   TPSC - Internal clock: counts on PCLK/1
  */
  MTU0.TCR.BYTE = 0x20  ;

  /* Timer Mode Register (TMDR)
  b7      ICSELD - Input capture source is TIOCDn pin
  b6      ICSELB - Input capture source is TIOCBn pin
  b5      BFB - TPUm.TGRB operates normally
  b4      BFA - TPUm.TGRA operates normally
  b3:b0   MD - Normal operation
  */
  MTU0.TMDR.BYTE = 0x00 ;

  /* Timer I/O Control Register (TIOR)
  b7:b4   IOB - Output compare, output disabled
  b3:b0   IOA - Output compare, output disabled
  */
  MTU0.TIORH.BYTE = 0x00 ;
  MTU0.TIORL.BYTE = 0x00 ;

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
  MTU0.TIER.BYTE = 0x81 ;

  /* Timer general register A
  Triggers ADC conversions at a rate of 1/48,000,000 * this value
  Sample values:
  48000 =   1.0 mS =   1 kHz
  4800  = 100.0 uS =  10 kHz
  480   =  10.0 uS = 100 kHz
  240   =   5.0 uS = 200 kHz
  120   =   2.5 uS = 400 kHz
  */
  MTU0.TGRA = 120 ;

  /* Set up variables for ping pong buffer control */
  adcCount     = 0 ;            /* No readings in buffer yet */
  pNextReading = pPingBuffer ;  /* Start loading the ping buffer */
  hotBuffer    = BUFFER_PING ;  /* Currently recording to ping buffer */
  bufferLock   = BUFFER_NONE ;  /* Neither buffer is locked to start */

  /* NEW FOR THIS LAB: SETUP DMAC0 to fill the PING/PONG buffers instead of ADC interrupt */

  /* Power up DMAC 0 */
  MSTP(DMAC0) = 0 ;

  /* Source address is ADC results register */
#ifdef PLATFORM_RSK62N
  DMAC0.DMSAR = (void *) &S12AD.ADDRA ;
#else
  DMAC0.DMSAR = (void *) &S12AD.ADDR4;
#endif

  /* Destination starts in PING buffer */
  DMAC0.DMDAR = (void *) pPingBuffer ;

  /* Number of transfers */
  DMAC0.DMCRA = bufSize ;

  /* DMA Transfer Mode Register (DMTMD)
  b15:b14   MD: 0 = normal transfer
  b13:b12   DTS: 0b10 = No repeat area
  b11:b10   ----
  b9:b8     SZ: 0b01 = 16-bit data
  b7:b2     ----
  b1:b0     DCTG: 0b01 = Trigger = peripheral interrupt
  */
  DMAC0.DMTMD.WORD = 0x2101 ;

  /* DMA Interrupt Setting Register (DMINT)
  b7:b5     ----
  b4        DTIE: Transfer end interrupt enable
  b3        ESIE: Transfer escape end interrupt enable
  b2        RPTIE: Repeat size end interrupt enable
  b1        SARIE: Source address extended repeat area overflow interrupt enable
  b0        DARIE: Destination address extended repeat area overflow interrupt enable
  */
  DMAC0.DMINT.BYTE = 0x10 ;

  /* DMA Address Mode Register (DMAMD)
  b15:b14   SM: Source Address is fixed
  b13       ----
  b12:b8    SARA: No Source Address Extended Repeat Area
  b7:b6     DM: Destination address increments
  b5        ----
  b4:b0     DARA: No Destination Address Exteneded Repeat Area
  */
  DMAC0.DMAMD.WORD = 0x0080 ;

  /* Route the ADC interrupt to the DMAC */
  ICU.DMRSR0 = VECT(S12AD,ADI) ;

  /* Enable DMAC transfers */
  DMAC0.DMCNT.BIT.DTE = 1 ;

  /* Enable DMCA activation */
  DMAC.DMAST.BIT.DMST = 1 ;

  /* Enable ADC interrupt in ICU at high priority */
  IPR(S12AD, ADI) = 2;
  IEN(S12AD, ADI) = 1;

  /* Enable DMAC interrupt in ICU at high priority */
  IPR(DMAC,DMAC0I) = 1 ;
  IEN(DMAC,DMAC0I) = 1 ;

  /* Start MTU0 */
  MTUA.TSTR.BYTE |= 0x01 ;

}

/******************************************************************************
* Function name : DMAC_Isr
* Description   : Callback for DMAC transfer complete interrupt
* Arguments     : None
* Return Value  : None
******************************************************************************/
#pragma vector = VECT_DMAC_DMAC0I
__interrupt void DMAC_Isr(void)
{

  /* Switch between PING & PONG buffer */
  if (hotBuffer == BUFFER_PING)
  {
    /* Make sure PONG buffer has been unlocked, if not then error */
    //if (bufferLock == BUFFER_PONG) S12ADC_Error() ;

    /* Lock PING buffer, switch to PONG buffer */
    //bufferLock = BUFFER_PING ;
    hotBuffer = BUFFER_PONG ;

    /* Destination is PONG buffer */
    DMAC0.DMDAR = (void *) pPongBuffer ;

  }
  else
  {
    //if (bufferLock == BUFFER_PING) S12ADC_Error() ;

    /* Lock PONG buffer, switch to PING buffer */
    //bufferLock = BUFFER_PONG ;
    hotBuffer = BUFFER_PING ;

    /* Destination is PING buffer */
    DMAC0.DMDAR = (void *) pPingBuffer ;
  }

  /* Number of transfers */
  DMAC0.DMCRA = bufSize ;

  /* Enable DMAC transfers */
  DMAC0.DMCNT.BIT.DTE = 1 ;

  /* Enable DMCA activation */
  DMAC.DMAST.BIT.DMST = 1 ;
}


/******************************************************************************
* Function name : S12ADC_Isr
* Description   : Callback for ADC conversion complete interrupt
* Arguments     : none
* Return Value  : none
******************************************************************************/

#pragma vector = VECT_S12AD_ADI
__interrupt void S12ADC_Isr(void)
{

  // Fetch the value from ADDR
#ifdef PLATFORM_RSK62N
  adcReading = S12AD.ADDRA ;
#else
  adcReading = S12AD.ADDR4;
#endif

  // Save the value in the buffer
  *pNextReading++ = adcReading ;

  // Update the count of samples
  adcCount++ ;

  // If we've got 1024,
  if (adcCount == bufSize)
  {
    adcCount = 0 ;
    if (hotBuffer == BUFFER_PING)
    {
      /* Make sure PONG buffer has been unlocked, if not then error */
      if (bufferLock == BUFFER_PONG) S12ADC_Error() ;

      /* Lock PING buffer, switch to PONG buffer */
      bufferLock   = BUFFER_PING ;
      hotBuffer    = BUFFER_PONG ;
      pNextReading = pPongBuffer ;
    }
    else
    {
      if (bufferLock == BUFFER_PING) S12ADC_Error() ;

      /* Lock PONG buffer, switch to PING buffer */
      bufferLock   = BUFFER_PONG ;
      hotBuffer    = BUFFER_PING ;
      pNextReading = pPingBuffer ;
    }
  }
  // Main loop can check for
}

/******************************************************************************
* Function name : S12ADC_bufferReady
* Description   : Tells if any buffers are filled & ready for post-processing
* Arguments     : None
* Return Value  : BUFFER_NONE, BUFFER_PING, or BUFFER_PONG
******************************************************************************/
uint32_t S12ADC_BufferReady (void)
{
  return(bufferLock);
}

/******************************************************************************
* Function name : S12ADC_release
* Description   : Allows foreground to release buffer to ISR after post-processing
* Arguments     : None
* Return Value  : None
******************************************************************************/
void S12ADC_Release (void)
{
  bufferLock = BUFFER_NONE ;
}

/******************************************************************************
* Function name : Excep_MTU0_TGIA0
* Description   : ISR for MTU0 compare match A (used only for testing!)
* Arguments     : None
* Return Value  : None
******************************************************************************/

#pragma vector = 114
__interrupt void Excep_MTU0_TGIA0(void)
{
  LED4 = ~LED4 ;
}

/******************************************************************************
* Function name : S12ADC_error
* Description   : Handles the ISR outrunning the foreground postprocessing
* Arguments     : none
* Return Value  : none
******************************************************************************/
void S12ADC_Error (void)
{


  // The foreground task hasn't unlocked the buffer we need (it's too slow!)
  // Set a breakpoint in this function to detect maximum logging rate
  while (bufferLock != BUFFER_NONE)
  {
    nop() ;
  }
}

