/*********************************************************************************                                                                                   *
FILE NAME      SPI_viaRSPI.c
DESCRIPTION:  Main routine for RSPI sample code running on RX62N MCU, RSPI1.
This program uses RSPI1.  The Chip Select for the flash device is set to P3_1.

The RSPI configuration has the baud rate generator set for maximum speed, which is
PCLK/2 or 25 MHz.

This program sets up RSPI1 in SPI master mode.

Copyright   : 2010 Renesas Electronics Corporation.
All Rights Reserved
**********************************************************************************/

/**********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
03.31.2010 RTA-JMB/ATX First Release

**********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/


/**********************************************************************************
User Includes
***********************************************************************************/
#include "spi.h"
/*lint -save -e* */

#include <iorx62n.h>

/*lint -restore */

/* ************************* Global variables ********************************/

#define FLASH_RSPI RSPI1
#define MOSI_PIN  PORT2.DR.BIT.B6
#define SCK_PIN   PORT2.DR.BIT.B7
#define SSL0_PIN  PORT3.DR.BIT.B1

/*****************************************************************************
Name:    SPI_init
Arguments:  None
Returns:  None
Description: Initial configuration RSPI for master mode
*****************************************************************************/
void SPI_init(void)
{ 
  /* Make sure SPI1 is running (take it out of stop mode) */
  MSTP(RSPI1) = 0 ;  
  
  /* Set up the port pins used by SPI */
  IOPORT.PFHSPI.BIT.RSPIS = 0 ;   /* Select proper bank of pins for SPI1 */
  IOPORT.PFHSPI.BIT.SSL0E = 0 ;   /* SSL0 (P3.1) is inactive (toggled as GPIO instead) */
  IOPORT.PFHSPI.BIT.MISOE = 1 ;   /* MISO (P3.0) is active */
  IOPORT.PFHSPI.BIT.MOSIE = 1 ;   /* MOSI (P2.6) is active */
  IOPORT.PFHSPI.BIT.RSPCKE = 1 ;  /* SCK  (P2.7) is active */
  
  /* General configuration for this RSK. Configure all unused pins as output low. */
  /* Disable slave select */
  PORT3.DR.BIT.B1 = 1 ;     /* Set level to inactive */
  PORT3.DDR.BIT.B1 = 1 ;    /* Make it an output */

  /* MISO is an input */
  PORT3.DDR.BIT.B0 = 0 ;    /* Make it an input */
  PORT3.ICR.BIT.B0 = 1 ;    /* Enable input buffer for peripheral */

  /* MOSI is an output */
  PORT2.DR.BIT.B6 = 1 ;     /* Set level to inactive */
  PORT2.DDR.BIT.B6 = 1 ;    /* Make it an output */

  /* SCK is an output, idles high */
  PORT2.DR.BIT.B7 = 1 ;     /* Set level to inactive */
  PORT2.DDR.BIT.B7 = 1 ;    /* Make it an output */
    
  /* Initialize SPI (per flowchart in hardware manual) */
  FLASH_RSPI.SPPCR.BYTE = 0x00 ;
  FLASH_RSPI.SPBR = 0x00 ;       /* Full speed! */
  FLASH_RSPI.SPDCR.BYTE = 0x24 ;      /* 32-bit data, 1 frame, 1 chip select */
  FLASH_RSPI.SPCKD.BYTE = 0x01 ;      /* 2 clock delay after chip select asserted */
  FLASH_RSPI.SSLND.BYTE = 0x00 ;      /* No delay after de-asserting SSL */
  FLASH_RSPI.SPND.BYTE = 0x01 ;       /* 2 clock delay before next access to SPI device */
  FLASH_RSPI.SPCR2.BYTE = 0x00 ;      /* No parity, no idle interrupts */
  FLASH_RSPI.SPCMD0.WORD = 0x0783 ;   /* MSB first, 8-bit data, keep SSL low */
  FLASH_RSPI.SPCR.BYTE = 0x48 ;       /* Enable RSPI in master mode */
}

/*****************************************************************************
Name:    SPI_open
Arguments:  None
Returns:  None
Description: Configure RSPI for master mode
*****************************************************************************/
void SPI_open(void)
{
  static char need_SPI_init = 1; 
   
  if(need_SPI_init)
  {
    SPI_init();
    need_SPI_init = 0;
  }
   
  /* Assert chip select */
  SSL0_PIN = 0 ;
  

}



/*****************************************************************************
Name:    SPI_close
Arguments:  None
Returns:  None
Description: Disable UART 0 for SPI and disable slave select
*****************************************************************************/
void SPI_close(void)
{
  /* De-assert chip select */
  SSL0_PIN = 1 ;
  
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

void SPI_send_receive(unsigned char const *pSrc, unsigned char *pDest, unsigned long ulBytes )
{
  unsigned long i ;  
    
  for (i = 0; i < ulBytes; i++)
  {
    /* ensure transmit register is empty */
    while (FLASH_RSPI.SPSR.BIT.IDLNF) ;
    FLASH_RSPI.SPDR.LONG = (pSrc == NULL) ? 0xFF : pSrc[i];
    /* Wait for transfer to complete */
    while (FLASH_RSPI.SPSR.BIT.IDLNF) ;
    if (pDest == NULL)
      (void)FLASH_RSPI.SPDR.LONG;
    else
      pDest[i] = (unsigned char) (FLASH_RSPI.SPDR.LONG & 0xFF) ;
  }
}
