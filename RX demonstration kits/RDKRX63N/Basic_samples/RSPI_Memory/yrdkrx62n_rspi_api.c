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
* File Name     : yrdkrx62n_rspi_api.c
* Version       : 1.00
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Functions for using RSPI devices on the YRDKRX63N, based
*                 on the same functionality for YRDKRX62N.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 06.09.2011     1.00        First release
*           19.01.2012                 Revised to add YRDKRX63N. CS revisions
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <iorx63n.h>
#include "yrdkrx62n_rspi_api.h"
#include "yrdkrx63n.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#ifndef NULL
#define NULL  0
#endif

/*******************************************************************************
* Local global variables
*******************************************************************************/  
static uint16_t YRDKRX62N_RSPI_Access_Locked_By = NO_DEVICE_SELECTED; /* use spi_cs enum for pids */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Select
* Description  : Enable device chip select on SPI channel
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*******************************************************************************/
void YRDKRX62N_RSPI_Select(uint16_t chip_select)
{
  switch (chip_select)
  {
  case SDMICRO_SELECTED:          /* Enable the SD card */
    SDMICRO_CS = 0;
    break;
  case FLASH_SELECTED:            /* Enable Micron flash */
    FLASH_CS = 0;    
    break;
  case WIFI_SELECTED:             /* Enable Redpine WiFi card */
    WIFI_CS = 0;
    break;
  case LCD_SELECTED:              /* Enable Okaya display */
    LCD_CS = 0;
    break;
  default:
    return;
  }     
} /* End of function YRDKRX62N_RSPI_Select() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Deselect
* Description  : Disables device chip select
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*******************************************************************************/
void YRDKRX62N_RSPI_Deselect(uint16_t chip_select)
{
  switch (chip_select)
  {
  case SDMICRO_SELECTED:          /* Disable the SD card */
    SDMICRO_CS = 1;
    break;
  case FLASH_SELECTED:            /* Disable the Micron flash */
    FLASH_CS = 1;
    break;
  case WIFI_SELECTED:             /* Disable the Redpine WiFi card */
    WIFI_CS = 1;
    break;
  case LCD_SELECTED:              /* Disable the Okaya display */
    LCD_CS = 1;
    break;
  default:
    return;
  }     
} /* End of function YRDKRX62N_RSPI_Deselect() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Init
* Description  : Initializes SPI channel, adapted to YRDKRX63N
* Arguments    : none
* Return Value : none
*******************************************************************************/
void YRDKRX62N_RSPI_Init(void)
{
  /* Init RSPI channel 0 */
  
  /* Initialize SPI channel 0 in SPI master mode, on the 'A' pins,  
     keep all SSLs disabled */       
  
  SYSTEM.PRCR.WORD = 0xA502; /* Protect off */
  
  /* Turn on RSPI0 */
  MSTP(RSPI0) = 0;
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* Set pin control register (SPPCR) */
  RSPI0.SPPCR.BYTE = 0x00;
  
  /* Set RSPI bit rate (SPBR) */
  /* -Set baud rate to   8.0 Mbps = 48MHz / (2 * (  2 + 1) * 2^0) */
  /* -Set baud rate to 187.5 Kbps = 48MHz / (2 * (127 + 1) * 2^0) */
  RSPI0.SPBR = 255; /* values from 1 to 255 (12 Mbps to 93.75 Kbps) work (was 2) */
  /* Larger values give a longer pause before "success!" shows on the LCD. */
  
  /* Set RSPI data control register (SPDCR) */
  /* -SPDR is accessed in longwords (32 bits) 
     -Transfer 1 frame at a time */
  RSPI0.SPDCR.BYTE = 0x20;
  
  /* Set RSPI clock delay registers (SPCKD) */
  /*  RSPI0.SPCKD.BYTE = 7; */
  
  /* Set RSPI slave select negation delay register (SSLND) */
  /*  RSPI0.SSLND.BYTE = 7; */
  
  /* Set RSPI next-access delay register (SPND) */
  /*  RSPI0.SPND.BYTE = 7; */
  
  /* Set RSPI control register 2 (SPCR2) */
  /* -Disable Idle interrupt */
  RSPI0.SPCR2.BYTE = 0x00;
  
  /* Set RSPI command register 0 (SPCMD0) */
  /* -MSB first
     -8 bits data length
     -SSL0 (handled manually)
     -Use bit rate % 1       
  */       
  RSPI0.SPCMD0.WORD = 0x0400;    
  
  /* Setup ICU for RSPI */
  IPR(RSPI0, SPRI0) = 3; /* Set shared IPL for RSPI0 */
  IEN(RSPI0, SPRI0) = 0; /* Disable receive buffer full interrupt */
  IEN(RSPI0, SPTI0) = 0; /* Disable transmit buffer empty interrupt */
  //  IEN(RSPI0, SPII0) = 1; /* Enable RSPI idle interrupt */
  IR (RSPI0, SPRI0) = 0; /* Clear pending receive buffer full interrupts */
  
  /* Port C -  SPI signals, chip selects, peripheral reset */
  PORTC.PODR.BYTE = 0x00;    /* */
  PORTC.PMR.BYTE  = 0x00;    /* All GPIO for now */
  MPC.PC5PFS.BYTE = 0x0D;    /* PC5 is RSPCKA */
  MPC.PC6PFS.BYTE = 0x0D;    /* PC6 is MOSIA */
  MPC.PC7PFS.BYTE = 0x0D;    /* PC7 is MISOA */
  PORTC.PMR.BYTE  = 0xE0;    /* PC5-7 assigned to SPI peripheral */
  PORTC.PODR.BYTE = 0x17;    /* Chip selects high to start */
  PORTC.PDR.BYTE  = 0x7F;    /* All outputs except MISO */
  
  SDMICRO_CS = 1; /* Disable slave selects */
  FLASH_CS   = 1;
  WIFI_CS    = 1;
  LCD_CS     = 1;
  
  /* Set RSPI control register (SPCR):     
  b0 1 Clock synchronous operation (3-wire)
  b1 0 Full duplex operation
  b2 0 No detection of fault mode error
  b3 1 Master mode
  b4 0 SPEIE diaable error    interrupt
  b5 1 SPTIE enable  transmit interrupt 
  b6 1 SPE   enable  function
  b7 1 SPRIE enable  receive  interrupt */
  RSPI0.SPCR.BYTE = 0xE9;
  
  /* Set RSPI Slave Select Polarity Register (SSLP):
  b0 0 SSL0 signal is 0-active
  b1 0 SSL1 signal is 0-active
  b2 0 SSL2 signal is 0-active
  b3 0 SSL3 signal is 0-active */
  RSPI0.SSLP.BYTE = 0x00;
} /* End of function YRDKRX62N_RSPI_Init() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Tx_Ready
* Description  : Make sure the transmit register is empty, blocks until it is.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void YRDKRX62N_RSPI_TxReady(void)
{
  while (RSPI0.SPSR.BIT.IDLNF) /* wait until the transmit register is empty */
  {
  }
} /* End of function () */

/*******************************************************************************
* Function Name: SPI_send_receive
* Description  : Performs SPI transfers
* Arguments    : pSrc -  
*                   pointer to data buffer with data to be transmitted.
*                   If NULL, const 0xFF as source.
*                pDest - 
*                   pointer to location to put the received data (can be
*                   same as pSrc if desired).  
*                   If NULL, receive data discarded.
*                usBytes - 
*                   number of bytes to be sent/received
* Return Value : none
*******************************************************************************/
void SPI_send_receive(uint8_t const *pSrc, uint8_t *pDest, uint16_t usBytes)
{      
  uint16_t i;  
  uint32_t temp;
  
  for (i = 0; i < usBytes; i++)
  {
    YRDKRX62N_RSPI_TxReady(); /* wait until transmit register is empty */
    
    if (pSrc)                 /* If writing, */
    {
      temp = pSrc[i];       /* transmit the next byte. */
    }
    else                      /* else */
    {
      temp = 0xFFFFFFFF;    /* transmit 0xFF */
    }
    
    RSPI0.SPDR.LONG = temp;    
    
    /* Transfer is complete when a byte has been shifted in (full duplex) */
    while (IR(RSPI0, SPRI0) == 0)
    {
    }
    
    temp = RSPI0.SPDR.LONG; /* Read received data. */
    
    if (pDest)            /* If there is a destination, */
    {
      pDest[i] = temp;  /* save the received data. */
    }
    
    IR(RSPI0, SPRI0) = 0; /* Clear pending read interrupts */
  }
} /* End of function SPI_send_receive() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Read
* Description  : Performs SPI transfers
* Arguments    : pDest - 
*                   Pointer to location to put the received data 
*                   Returned value will be incremented by number
*                   of bytes received.
*                usBytes - 
*                   number of bytes to be received
* Return Value : true - 
*                   success
*                false - 
*                   failure
*******************************************************************************/
bool YRDKRX62N_RSPI_Read(uint8_t *pDest, uint16_t usBytes)
{
  uint16_t i;  
  uint32_t temp;
  bool     ret = true;
  
  for (i = 0; i < usBytes; i++)
  {
    YRDKRX62N_RSPI_TxReady();   /* wait until transmit register is empty */
    
    RSPI0.SPDR.LONG = 0xFFFFFFFF; /* If just reading then transmit 0xFF */
    
    /* Transfer is complete when a byte has been shifted in (full duplex) */
    while (IR(RSPI0, SPRI0) == 0)
    {
    }
    
    temp = RSPI0.SPDR.LONG; /* Read received data. */
    
    if (pDest)            /* If there is a destination, */
    {
      pDest[i] = temp;  /* save the received data. */
    }
    
    IR(RSPI0, SPRI0) = 0; /* Clear pending read interrupts */
  }
  
  return ret;
} /* End of function YRDKRX62N_RSPI_Read() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Write
* Description  : Write to an SPI device
* Arguments    : pSrc -  
*                   Pointer to data buffer with data to be transmitted.
*                   Returned value will be incremented by number of attempted
*                   writes.
*                usBytes - 
*                   number of bytes to be sent
* Return Value : true - 
*                   success
*                false - 
*                   failure
*******************************************************************************/
bool YRDKRX62N_RSPI_Write(uint8_t *pSrc, uint16_t usBytes)
{
  bool     ret = true;
  uint16_t i;  
  uint32_t temp;
  
  for (i = 0; i < usBytes; i++)
  {
    YRDKRX62N_RSPI_TxReady(); /* Wait until transmit register is empty. */
    
    if (pSrc)                 /* If writing, */
    {
      temp = pSrc[i];       /* transmit the next byte. */
    }
    else                      
    {
      temp = 0xFFFFFFFF;    /* transmit 0xFF */
    }
    
    RSPI0.SPDR.LONG = temp;    
    
    /* Transfer is complete when a byte has been shifted in (full duplex) */
    while (IR(RSPI0, SPRI0) == 0)
    {
    }
    
    temp = RSPI0.SPDR.LONG; /* Read received data. If transmit only, then ignore it */
    
    IR(RSPI0, SPRI0) = 0; /* Clear pending interrupts */
  }
  
  return ret;
} /* End of function YRDKRX62N_RSPI_Write() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Lock
* Description  : Get lock on RSPI access
* Arguments    : pid - 
*                   unique program ID
* Return Value : true -  
*                   lock     acquired
*                false - 
*                   lock not acquired
*******************************************************************************/
bool YRDKRX62N_RSPI_Lock(uint16_t pid)
{
  bool ret = true;
  
  if (YRDKRX62N_RSPI_Access_Locked_By == NO_DEVICE_SELECTED)
  {
    YRDKRX62N_RSPI_Access_Locked_By =  pid;
  }
  else
  {
    ret = false;
  }
  
  return ret;
} /* End of function YRDKRX62N_RSPI_Lock() */

/*******************************************************************************
* Function Name: YRDKRX62N_RSPI_Unlock
* Description  : Unlock RSPI access
* Arguments    : pid -
*                   program ID that locked access.
* Return Value : true  -
*                   correct pid
*                false -
*                   wrong   pid     
*******************************************************************************/
bool YRDKRX62N_RSPI_Unlock(uint16_t pid)
{
  bool ret = true;
  
  if (YRDKRX62N_RSPI_Access_Locked_By == pid)
  {
    YRDKRX62N_RSPI_Access_Locked_By =  NO_DEVICE_SELECTED;
  }
  else
  {
    ret = false;
  }
  
  return ret;
} /* End of function YRDKRX62N_RSPI_Unlock() */


/*******************************************************************************
* End of file: yrdkrx62n_rspi_api.c
*******************************************************************************/
