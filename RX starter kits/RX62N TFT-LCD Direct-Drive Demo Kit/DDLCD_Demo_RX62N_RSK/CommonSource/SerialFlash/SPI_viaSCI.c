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

/*********************************************************************************                                                                                   *
FILE NAME      SPI_H836077.c
DESCRIPTION:  Main routine for SPI sample code running on the Quark (H8/36077) board.
This program uses SCI3 channel 1.  The Chip Select for the flash device is set 
p2_3.

Copyright   : 2005 Renesas Electronics Corporation.
All Rights Reserved
**********************************************************************************/

/**********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
11.08.2005 RTA-MGF First Release
05.10.2007 RTA-JMB Revised to support Neutrino project

**********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/


/**********************************************************************************
User Includes
***********************************************************************************/
#include "global.h"
#include "hwsetup.h"
#include "spi.h"

/* ************************* Global variables ********************************/

/*****************************************************************************
Name:    SPI_open
Arguments:  None
Returns:  None
Description: Configure uart0 for SPI, Mode 3, assert slave select

NOTE:  You must set the transmit enable and receive enable bits at the
    same time to make the simultaneous transmit/receive work correctly.
*****************************************************************************/
void SPI_open(void)
{
  /* enable module */
  FLASH_SCI_ENABLE;
  
  /* disable slave select */
  PINo(FLASH_SCS) = 1;
  DDRo(FLASH_SCS);

  /* set clock high when SCI disabled */
  PINo(FLASH_SCK) = 1;
  DDRo(FLASH_SCK);

  /* disable transmit/recieve, set as master mode */
  FLASH_SCI.SCR.BYTE = 0x00;

  /* select clock synchronous mode, clock rate to phi, 8 bit data length */
  FLASH_SCI.SMR.BYTE = 0x80;
  
  /* reverse bit order to MSB first */
  FLASH_SCI.SCMR.BIT.SDIR = 0x01;

  /* clear status flags */
  (void)FLASH_SCI.SSR.BYTE;
  FLASH_SCI.SSR.BYTE = 0x00;

  /* enable SPI TX/RX */
  FLASH_SCI.SCR.BYTE |= 0x30; /* MUST set TE and RE in TX/RX mode */

  /* Set communication at as fast as possible...we're nowhere near Flash limit */
  FLASH_SCI.BRR = 0;

  /* assert slave select */
  PINo(FLASH_SCS) = 0;
}

/*****************************************************************************
Name:    SPI_close
Arguments:  None
Returns:  None
Description: Disable UART 0 for SPI and disable slave select
*****************************************************************************/
void SPI_close(void)
{
  /* disable slave select */
  PINo(FLASH_SCS) = 1;
  DDRo(FLASH_SCS);

  /* disable transmit and receive */
  FLASH_SCI.SCR.BYTE &= ~0x30; /* MUST clear TE and RE in TX/RX mode */
}

/*****************************************************************************
Name:        SPI_send_receive
Arguments:
      pSrc - pointer to data buffer with data to be transmitted. If NULL, const 0xFF as source.
      pDest - pointer to location to put the received data (can be same as pSrc if desired). 
              If NULL, receive data discarded.
      usBytes - number of bytes to be sent/received
Returns:     none
Description: Performs SPI transfers.
*****************************************************************************/
#include <machine.h>
void SPI_send_receive(unsigned char const *pSrc, unsigned char *pDest, unsigned long ulBytes )
{
  unsigned long i;


  /* Transmit data, wait for rx to be completed, copy received data to buffer
   and increment data pointer */
  for(i = 0; i < ulBytes ; i++)
  {
    /* ensure transmit register is empty */
    while (!FLASH_SCI.SSR.BIT.TDRE) ;
    FLASH_SCI.TDR = (pSrc == NULL) ? 0xFF : pSrc[i];
    FLASH_SCI.SSR.BIT.TDRE = 0;
    
    while( !FLASH_SCI.SSR.BIT.RDRF ) ;
    if (pDest == NULL)
      (void)FLASH_SCI.RDR;
    else
      pDest[i] = FLASH_SCI.RDR;
    FLASH_SCI.SSR.BIT.RDRF = 0;
  }
}
