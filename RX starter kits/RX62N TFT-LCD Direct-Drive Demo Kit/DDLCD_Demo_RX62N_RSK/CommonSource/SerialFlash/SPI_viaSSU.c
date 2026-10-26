/* Adapted for use with IAR Embedded Workbench */
/*********************************************************************************                                                                                   *
FILE NAME      SPI_viaSCI_2456.c
DESCRIPTION:  Main routine for SPI sample code running on M16C/26A MCU, UART0, Mode3.
This program uses UART0.  The Chip Select for the flash device is set to P6_0.  The

The UART configuration has
the baud rate generator set for divide by 2 mode.  The MCU clock is set for 20 MHz (the
maximum for the M16C Tiny family) which gives a max 10 MHz SPI clock rate.  The clock
speed was verified using a scope

This program sets up UART0 in SPI master mode.  The configuration allows setting
the peripheral up for any of the 2 SPI modes (1 or 3) and allows using 3 wire (single slave,
no slave select needed) or 4 wire (port P6_0 and P10_4 are used for slave selects
to be consistent with the hardware manual example.  However, any available port pins
can be used for slave select outputs).  Write Protect pin is controlled by port P7_5,
and the code unprotects during command sequences.

The Transfer Clock (baud rate) was limited to 769230 (u0brg=0x0c), only because in this design
(not the RSK or SKP) the MCU ran at 5V, the M25P40 ran at 3V, and there were two transistors
used to raise the serial output (Q) up to 5V so the M16C RxD0 pin would recognize a high level.
The impedance/capacitance in the circuit was high enough to limit the transmission speed below the M16C max
possible - 10Mbps.

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
#include "spi.h"
/*lint -save -e* */

//#include "io_r8c25.h"           /* Definition of I/O Register for R8C/25  */
#include "iodefine.h"

/*lint -restore */
//#include "taudef.h"             /* Board support package for Tau */
#include "global.h"
#include "hwsetup.h"

/* ************************* Global variables ********************************/

/*****************************************************************************
Name:    SPI_open
Arguments:  None
Returns:  None
Description: Configure uart0 for SPI, Mode 3, assert slave select
*****************************************************************************/

void SPI_init(void)
{
    /* General configuration for this RSK. Configure all unused pins as output low. */
  /* disable slave select */
  PINo(FLASH_SCS) = 1;
  DDRo(FLASH_SCS);

  /* set clock high when SCI disabled */
  PINo(FLASH_SCK) = 1;
  DDRo(FLASH_SCK);
  
  /* Enable SSU module.  */
  SYSTEM.EXMSTPCRL.BIT.MSTP23 = 0;  
/*******************************************************************************/  
  /* Clear TE and RE flags */
  SSU.SSER.BYTE = 0x00;
  
  /* Select Master mode of operation */
  SSU.SSCRH.BIT.MSS = 0x1;
  
  /* Select SSCK pin as a serial clock */
  SSU.SSCRH.BIT.SCKS = 0x1;
  
  SSU.SSCRH.BIT.BIDE = 0x0;
  
  /* Select Clock synchronous mode of operation */
  SSU.SSCRL.BIT.SSUMS = 0x1;

  /* Select 8 bits Transmit/Receive Data Length */
  SSU.SSCRL.BIT.DATS = 0;

  /* Select transfer clock rate as PHI/4 */
  SSU.SSMR.BIT.CKS = 0x1;

  /* Clock Polarity Select, Low output in idle mode, and high 
    output in active mode */
  SSU.SSMR.BIT.CPOS = 0x0;

  /* Select MSB first */
  SSU.SSMR.BIT.MLS = 0x1;
  
  /* Configure SSCK0-A as output pin */
  PFC.PFCR5.BIT.SSCK0S = 0x00;
  
  /* Configure SSI0-A as input pin */
  PFC.PFCR5.BIT.SSI0S = 0x00;
  
  /* Configure SSO0-A as output pin */
  PFC.PFCR5.BIT.SSO0S = 0x00;

  /* Select SCS pin as I/O */
  SSU.SSCRH.BIT.CSS = 0x0;

  /* SS Control Register 2 (SSCR2)
  b1:b0  - Reserved   - 00
  b2    - SSODTS  - 0 (Selects the data output timing of the SSO pin)
  b3     - SCSATS  - 0 (Selects the assertion timing of the SCS pin)
  b4    - TENDSTS  - 1 (Sets the TEND bit after the last bit is transmitted)
  b5    - SCSOS    - 0 (SCS pin used as CMOS)
  b6    - SSCKOS  - 0 (SSCK Pin used as CMOS)
  b7    - SDOS    - 0 (Serial Data Pin used as CMOS) */
  
  SSU.SSCR2.BYTE = 0x10;
  
  /* SS Enable Register (SSER)
  b0    - CEIE    - 0 (Conflict Error Interrupt is disabled)
  b1    - RIE    - 0 (Receive Interrupt Enable)
  b2    - TIE    - 0 (Transmit Interrupt Enable)
  b3    - TEIE    - 0 (Transmit End Interrupt Enable)
  b4    - Reserved  - 0
  b5    - Reserved  - 0
  b6    - RE    - 1 (Receive Enabled)
  b7    - TE    - 1 (Transmit Enable) */
  
  //SSU.SSER.BYTE = 0xC0;  
}

void SPI_open(void)

{
static char need_SPI_init = 1; 
   
if(need_SPI_init)
   {
   SPI_init();
   need_SPI_init = 0;
   }

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
  PINo(FLASH_SCS) = 1;
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
  unsigned long i;

  if (pSrc == NULL)
  {
    /* Rx Only */
    SSU.SSER.BYTE = 0x40;
    /* start reception */
    (void)SSU.SSRDR0;
    for(i = 0; i < ulBytes ; i++)
    {
      /* wait for data to be shifted in */
      while (SSU.SSSR.BIT.RDRF != 0x1)      
        ;
      
      pDest[i] = SSU.SSRDR0;
    }
  }
  else if (pDest == NULL)
  {
    /* Tx only */
    SSU.SSER.BYTE = 0x80;
    for(i = 0; i < ulBytes ; i++)
    {
      /* ensure transmit register is empty */
      while (SSU.SSSR.BIT.TDRE != 0x1)
        ;
      /* Write the data to SSTDR */
      SSU.SSTDR0 = pSrc[i];
    }
    /* wait for last data to be sent */
    while (SSU.SSSR.BIT.TEND != 0x1)      
      ;
    SSU.SSSR.BIT.TEND = 0;
  }
  else
  {
    /* Tx/Rx */
    SSU.SSER.BYTE = 0xC0;
    for(i = 0; i < ulBytes ; i++)
    {
      /* ensure transmit register is empty */
      while (SSU.SSSR.BIT.TDRE != 0x1)
        ;
      /* Write the data to SSTDR */
      SSU.SSTDR0 = pSrc[i];
    
      /* wait for data to be shifted in */
      while (SSU.SSSR.BIT.RDRF != 0x1)      
        ;
      
      pDest[i] = SSU.SSRDR0;
    }
    /* wait for last data to be sent */
    while (SSU.SSSR.BIT.TEND != 0x1)      
      ;
    SSU.SSSR.BIT.TEND = 0;
  }
  
  SSU.SSER.BYTE = 0x00;
}
