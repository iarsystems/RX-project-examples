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
* File Name     : dmac.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines DMAC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 20.09.2010                 First release
*           20.01.2012                 CS revisions. Ported to FIT environment.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>

#include "dmac.h"

/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
void (*dmac_callback)(void) = 0; /* Start out with NULL function pointer.  */
                                 /* Can be set later in dmac_callback_set(). */


/*******************************************************************************
* Function name : dmac_init
* Description   : This function configures DMAC channel 0 for a single transfer 
*                 with each trigger event. Transfer is triggered by interrupt 
*                 from other perpheral. 
* Arguments     : trigger_vector - 
*                     the interrupt vector that is routed to the DMAC.
*                 dmac_source -
*                     the address that the DMAC will read from.
*                 dmac_dest -
*                     the address that the DMAC will write to.
*                 num_transfers - 
*                     The number of transfers the DMAC is to do.
* Return value  : none
*******************************************************************************/
void dmac_init(uint8_t trigger_vector, 
               void * dmac_source, 
               void * dmac_dest, 
               uint16_t num_transfers)
{
  /* Configure the DMAC to perform a single transfer with each trigger event. Transfer
  is triggered by interrupt from other perpheral. */ 
  
  /* DMA Transfer Mode Register (DMTMD) */
  /* b15 b14 b13 b12 b11 b10 b9 b8 b7 b6 b5 b4 b3 b2 b1 b0  */
  /* |--MD--|--DTS--| ----- |-SZ -|-----------------|DCTG | */
  /*    |      |        |      |          |           | 
        |      |        |      |          |           +-- b1,b0   : DMA Request Source Select.
        |      |        |      |          + ------------- b2 to b7: Reserved. Always Read/Write 0.
        |      |        |      +------------------------- b8,b9   : Transfer Data Size Select.
        |      |        +-------------------------------- b11,b10 : Reserved. Always Read/Write 0.
        |      +----------------------------------------- b13,b12 : Repeat Area Select.
        +------------------------------------------------ b15,b14 : Transfer Mode Select. */             
  DMAC0.DMTMD.BIT.MD   = 0x0;  // Normal mode.
  DMAC0.DMTMD.BIT.DTS  = 0x2;  // Source is repeated (in block mode).      
  DMAC0.DMTMD.BIT.SZ   = 0x1;  // 16-bit transfer size.
  DMAC0.DMTMD.BIT.DCTG = 0x1;  // Interrupt from peripheral activates transfer.
  /* DMACA0.DMTMD.WORD    = 0x2101; */
    
  /* DMA Address Mode Register (DMAMD)                      */
  /* b15 b14 b13 b12 b11 b10 b9 b8 b7 b6 b5 b4 b3 b2 b1 b0  */
  /* |--SM--| - |----SARA[4:0]----|-DM--|- |--DARA[4:0]---| */
  /*    |     |          |           |   |        | 
        |     |          |           |   |        +-- b4 to b0 : Destination Address Extended Repeat Area.
        |     |          |           |   + ---------- Reserved : Always Read/Write 0.
        |     |          |           +--------------- b7, b6   : Destination Address Update Mode.
        |     |          +--------------------------- b12 to b8: Source Address Extended Repeat Area.
        |     +-------------------------------------- b13      : Reserved. Always Read/Write 0.
        +-------------------------------------------- b15,b14  : Source Address Update Mode. */
  DMAC0.DMAMD.WORD   = 0x0000;// Clear all bits.      
  DMAC0.DMAMD.BIT.SM = 0x0;   // DMAC source address fixed.
  DMAC0.DMAMD.BIT.DM = 0x2;   // DMAC destination address incremented.             
  /* DMACA0.DMAMD.WORD  = 0x0080; */
  
  /* DMA Source Address Register (DMSAR) */
  DMAC0.DMSAR = dmac_source; /* Source start address */
  
  /* DMA Destination Address Register (DMDAR) */
  DMAC0.DMDAR = dmac_dest;   /* Destination start address */
  
  /* DMA Transfer Count Register (DMCRA) */
  /* b25 to b16: DMCRAH.  Repeat or Block size (not used in normal mode). */
  /* b15 to b0:  DMCRAL.  The number of transfer operations to do */  
  DMAC0.DMCRA = num_transfers;   // The number of transfer operations to do.
  
  /* Write to the ICU.DMRSRm register while the DMA transfer enable bit of the
  DMA transfer enable register (DMACm.DMCNT.DTE) is cleared to 0. */
  DMAC0.DMCNT.BIT.DTE = 0;
  
  /* Route the trigger interrupt to the DMAC through the ICU. */
  ICU.DMRSR0 = trigger_vector;
  
  /* DMA Interrupt Setting Register (DMINT) */
  /*  b7 b6 b5  b4   b3   b2    b1    b0    */
  /* |--------|DTIE|ESIE|RPTIE|SARIE|DARIE  */
  /*    |        |    |    |     |     | 
        |        |    |    |     |     +-- b0: Destination address extended repeat 
        |        |    |    |     |              area overflow interrupt enable.
        |        |    |    |     + ------- b1: Source address extended repeat area 
        |        |    |    |                    overflow interrupt enable
        |        |    |    +-------------- b2: Repeat size end interrupt enable
        |        |    +------------------- b3: Transfer escape end interrupt enable.
        |        +------------------------ b4: Transfer end interrupt enable. 
        +--------------------------------- b7 to b5: Reserved. Always read/write 0. */
  DMAC0.DMINT.BIT.DTIE = 1;  //1: Enables the transfer end interrupt request.
  
  /* DMA Transfer Enable Register (DMCNT) 
      b0:       DTE; 1 Enables DMA transfer. 
      b7 to b1: Reserved. Always read/write 0. 
  */    
  DMAC0.DMCNT.BIT.DTE = 1; /* Gets cleared when transfer count decrements to 0. */
  
  /* DMA Start Register (DMAST) */
  DMAC.DMAST.BIT.DMST = 1; /* Enables activation for all the DMA channels. */
  
  IPR(DMAC, DMAC0I)   = 1; /* Enable DMAC interrupt in ICU. */
  IEN(DMAC, DMAC0I)   = 1; /* Set interrupt priority. */
} /* End of function dmac_init() */


/******************************************************************************
* Function name : dmac_callback_set
* Description   : Receives a pointer to a function and sets it as the callback
*          function for the DMAC interrupt. This callback is called from
*          the DMAC transfer complete ISR. 
* Arguments    : func - 
*                     The pointer to the callback function.
* Return Value  : none
******************************************************************************/
void dmac_callback_set(void(*func)(void))
{
  dmac_callback = func;
} /* End of function dmac_callback_set() */


/******************************************************************************
* Function name : dmac_isr
* Description   : Callback for DMAC transfer complete interrupt
* Arguments     : None
* Return Value  : None
******************************************************************************/
#pragma vector = VECT_DMAC_DMAC0I
__interrupt void dmac_isr(void)
{
  if (dmac_callback)
  {
    dmac_callback();
  } 
} /* End of dmac_isr() */

/*******************************************************************************
* End of file dmac.c
*******************************************************************************/
