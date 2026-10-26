/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :hwsetup.c                                             */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Hardware Setup file                                   */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
02.28.2007 RTA-PMM First Release 
************************************************************************/
#include <machine.h>
#include "Global.h"
#include "ad_dtc_RX62N.c"
//#include "da_dtc_RX610.c"

#pragma location=".CDTC_VECT"
__root const unsigned long dtc_vectors = 0;    /* start of DTC vector table */

#pragma section=".CDTC_VECT"

extern uint8_t ResourcesLoaded;

void initLEDs()
{
  PINo(LED_0) = 1;
  DDRo(LED_0);
  PINo(LED_1) = 1;
  DDRo(LED_1);
  PINo(LED_2) = 1;
  DDRo(LED_2);
}

void HardwareSetup(void)
{
  /* Configure SCKCR based on header request */
  {
    uint32_t sckcr = 0;
    sckcr += (ICLK_MUL==8) ? (0ul << 24) : (ICLK_MUL==4) ? (1ul << 24) : (ICLK_MUL==2)? (2ul << 24) : (1ul << 24);
    sckcr += (BCLK_MUL==8) ? (0ul << 16) : (BCLK_MUL==4) ? (1ul << 16) : (BCLK_MUL==2)? (2ul << 16) : (1ul << 16);
    sckcr += (PCLK_MUL==8) ? (0ul <<  8) : (PCLK_MUL==4) ? (1ul <<  8) : (PCLK_MUL==2)? (2ul <<  8) : (1ul <<  8);
    SYSTEM.SCKCR.LONG = sckcr;
  }
  
  /* turn on external address and data bus */
  SYSTEM.SYSCR0.WORD = 0x5A03;   /* enable the external bus...must also set key and internal ROME bit */
  IOPORT.PF4BUS.BYTE = 0xFF;     /* Enable A0-A15 */
  IOPORT.PF5BUS.BYTE = 0x10;     /* Enable D8-D15 */
  IOPORT.PF6BUS.BYTE = 0xD0;     /* Enable SDRAM pins */
  
  { uint32_t delay = 100*100; while (--delay > 0); } /* delay at least 100uS after SDCLK active */
  
  BSC.SDIR.WORD = 0x00F0;         /* SDRAM initialization...AutoRefresh interval=3 cycles, Precharge cycles count=3cycles, 15 times */
  BSC.SDICR.BIT.INIRQ = 1;        /* starte SDRAM initialization */
  while(BSC.SDSR.BIT.INIST == 1)  /* wait for initialization complete */
    ;
  BSC.SDCMOD.BIT.EMODE = 0;       /* set endian to little */
  BSC.SDCCR.BIT.BSIZE = 0;        /* set SDRAM to 16bit access */
  BSC.SDMOD.WORD = 0x0220;        /* Set write single location, CL=2 */
  BSC.SDRFCR.BIT.REFW = 5-1;      /* set refresh cycle count to 5 */
  BSC.SDRFCR.BIT.RFC = 720-1;     /* set refresh period to 720 clock cycles (15uS at 48Mhz) */
  BSC.SDTR.LONG = 0x000002;       /* set RAS=1, RCD=1, RP=1, WR=1, CL=2, cycles */
  BSC.SDADR.BIT.MXC = 1;          /* set shift to 9 bits (for 8Mx16 SDRAM) */
  BSC.SDRFEN.BIT.RFEN = 1;        /* enable auto refresh */
  BSC.SDCCR.BIT.EXENB = 1;        /* enable SDRAM operation...this should be last operation */

#if 0
  /* configure SRAM */
  IOPORT.PF3BUS.BYTE = 0x1F;      /* Enable A16-A20 */
  BSC.CS7MOD.BIT.WRMOD = 1;       /* use single write control */
  BSC.CS7CR.WORD = 1;             /* enable CS7 space as 16 bit memory space */
  BSC.CS7WCR2.LONG = 0;           /* set all extensions to zero */
  BSC.CS7WCR1.LONG = 0x01010101;  /* set all read/write accesses to one state */
  IOPORT.PF5BUS.BIT.WR1BC1E = 1;  /* use BC1 pin */
  IOPORT.PF1CSS.BIT.CS7S = 1;     /* set CS7-b as active */
  IOPORT.PF0CSE.BIT.CS7E = 1;     /* enable CS7 pin */
#endif

  /* connect MTU channel 3 to B pins */
  IOPORT.PFCMTU.BYTE = 0x0C;

  /* connect ExDMA 0 to C pins */
  IOPORT.PF7DMA.BYTE = 0x20;
    
  /* Enable hardware as necessary */
  MSTP(DTC) = 0;    /* enable DTC */
  MSTP(EXDMAC) = 0; /* enable ExDMA */
  MSTP(MTU0) = 0;   /* enable TPU 0-5 */
  MSTP(CRC) = 0;    /* enable CRC 0-5 */

  MTUA.TOER.BYTE = 0xFF; /* turn on all OE based outputs */
  
  /* enable DTC */
  DTC.DTCVBR = __section_begin(".CDTC_VECT");
  DTC.DTCCR.BYTE = 0;
  DTC.DTCADMOD.BYTE = 0;
  DTC.DTCST.BYTE = 1;
}


//#pragma interrupt DefaultISR
//void DefaultISR(void)
//{
//  for(;;)
//   brk() ;
//}  
