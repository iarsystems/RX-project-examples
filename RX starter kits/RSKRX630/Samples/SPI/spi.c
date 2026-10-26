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
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : spi.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : Defines SPI functions used in this file. 
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Declares SPI functions defined in this file */
#include "spi.h"
/* Defines MCU device registers used in this file */
#include "iorx630.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* SPI Master receive buffer */
volatile uint8_t gMasterRxBuffer[32];
/* Transmit message buffer variable (imported) */
extern const uint8_t gTransmitMessage[32];

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Slave receive interrupt function prototypes */
void SlaveRxInterrupt_SPI(void);
/* Master receive interrupt function prototypes */
void MasterRxInterrupt_SPI(void);

/*******************************************************************************
* Outline     : Init_SPI
* Description   : Function initialises the SPI master and slave channels, and the
*          receive interrupts.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_SPI(void)
{  
  /* Set pins from general IO to SPI */
  PORTE.PMR.BYTE |= 0x0F;
  PORTA.PMR.BYTE |= 0xD0;
  PORTB.PMR.BYTE |= 0x01;
  
  /* Disable MPC register protection */
  MPC.PWPR.BIT.B0WI = 0;
  MPC.PWPR.BIT.PFSWE = 1;
  
  /* Disable PFS register protection */
  SYSTEM.PRCR.WORD = 0xA50B;  
  
  /* Configure channel 0 SPI pins */
  MPC.PB0PFS.BYTE = 0x0D;
  MPC.PA6PFS.BYTE = 0x0D;
  MPC.PA7PFS.BYTE = 0x0D;
  MPC.PA4PFS.BYTE = 0x0D;
  
  /* Configure channel 1 SPI pins */
  MPC.PE1PFS.BYTE = 0x0E;
  MPC.PE2PFS.BYTE = 0x0E;
  MPC.PE3PFS.BYTE = 0x0D;
  MPC.PE0PFS.BYTE = 0x0D;  
    
  /* Re-enable PFS register protection */
  SYSTEM.PRCR.BIT.PRKEY = 0xA5u;
  SYSTEM.PRCR.WORD &= 0xFF00;  
    
  /* Enable RSPI0 and 1 peripherals */
  MSTP_RSPI1 = 0;
  MSTP_RSPI0 = 0;
  
  /* Configure SPI channels 0 & 1 to operate in 3-wire mode */
  RSPI0.SPCR.BIT.SPMS = 1;
  
  /* Set channel 1 to be SPI master and channel 0 to be slave */
  RSPI1.SPCR.BIT.MSTR = 1;
  RSPI0.SPCR.BIT.MSTR = 0;
  
  /* Enable receive interrupt for both channels */
  ICU.IER[4].BIT.IEN7 = 1;
  ICU.IER[5].BIT.IEN2 = 1;
  RSPI1.SPCR.BIT.SPRIE = 1;
  RSPI0.SPCR.BIT.SPRIE = 1;
  
  /* Set receive interrupt priority for both channels */
  ICU.IPR[39].BYTE = 5u;
  ICU.IPR[42].BYTE = 6u;
      
  /* Set bit rate to PCLK/4096 */
  RSPI1.SPBR = 255u;
  RSPI0.SPBR = 255u;
  
  /* Set number of transmit frames to 1 for both channels */
  RSPI1.SPDCR.BIT.SPFC = 0;
  RSPI0.SPDCR.BIT.SPFC = 0;
      
  /* Set command register 0 data length to 16bits */
  RSPI1.SPCMD0.BIT.SPB = 0xF;
  RSPI0.SPCMD0.BIT.SPB = 0xF;
  
  /* Set command register 0 bit rate divisor to 0 */
  RSPI1.SPCMD0.BIT.BRDV = 0;
  RSPI0.SPCMD0.BIT.BRDV = 0;
    
  /* Empty the transmit buffer for both channels */
  RSPI1.SPDR.LONG = 0x00000000;
  RSPI0.SPDR.LONG = 0x00000000;
    
  /* Enable both SPI channels */
  RSPI1.SPCR.BIT.SPE = 1u;
  RSPI0.SPCR.BIT.SPE = 1u;
}
/*******************************************************************************
* End of function Init_SPI
*******************************************************************************/

/*******************************************************************************
* Outline     : MasterSend_SPI
* Description   : Function sends an arbitrary length 8bit message string, from 
*          SPI master to SPI slave channel.
* Argument      : uint16_t num_bytes - Length of message in bytes
*          uint8_t * message  - Pointer to location of message string
* Return value  : none
*******************************************************************************/
void MasterSend_SPI(uint16_t num_bytes, uint8_t * message)
{
  /* Iterate through each element of gMasterRxBuffer */
  for(uint8_t i = 0; i < sizeof(gMasterRxBuffer); i++)
  {
    /* Set array element to 0x00 */
    gMasterRxBuffer[i] = 0x00;
  }
  
  /* Initialise loop counter */
  uint16_t j = 0;
  
  /* Iterate through each message byte */
  while(j < num_bytes)
  {
    /* Load message byte into transmit container variable */
    uint8_t tx_byte = message[j];
    
    /* Write to data register */
    RSPI1.SPDR.LONG = (uint32_t) 0xA5000000 | (tx_byte << 16);
    
    /* Wait until transmission is complete */
    while(RSPI1.SPSR.BIT.IDLNF);
    
    /* Increment loop counter */
    j++;
  }
  
  /* Send a dummy transmission to receive the final byte from the slave */
  RSPI1.SPDR.LONG = 0xA3000000;
  
  /* Wait until transmission is complete */
  while(RSPI1.SPSR.BIT.IDLNF);
  
  /* Verify sent and received message strings match */
  uint8_t errors = 0;
  for(uint8_t k = 0; k < num_bytes; k++)
  {
    /* Compare message byte at position k */
    if(gTransmitMessage[k] != gMasterRxBuffer[k])
    {
      errors++;
    }
  }
  /* Check if any errors were found */
  if(errors)
  {
    /* Data mismatch, indicate SPI error on debug LCD */
    Display_LCD(LCD_LINE1, " SPI    ");
    Display_LCD(LCD_LINE2, " Error  "); 
  }
  else
  {
    /* Data mismatch, indicate SPI error on debug LCD */
    Display_LCD(LCD_LINE1, " SPI    ");
    Display_LCD(LCD_LINE2, " Success");
  }
}
/*******************************************************************************
* End of function MasterSend_SPI
*******************************************************************************/

/*******************************************************************************
* Outline     : Excep_RSPI0_SPRI0
* Description   : Slave receive interrupt handler. Function is triggered when
*          SPI channel 0 receives data, and calls the 
*          SlaveRxInterrupt_SPI function.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_RSPI0_SPRI0
__interrupt void Excep_RSPI0_SPRI0(void)
{
  /* Call the slave RX interrupt function */
  SlaveRxInterrupt_SPI();
}
/*******************************************************************************
* End of function Excep_RSPI0_SPRI0
*******************************************************************************/

/*******************************************************************************
* Outline     : Excep_RSPI1_SPRI1
* Description   : Slave receive interrupt handler. Function is triggered when
*          SPI channel 1 receives data, and calls the 
*          MasterRxInterrupt_SPI function.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_RSPI1_SPRI1
__interrupt void Excep_RSPI1_SPRI1(void)
{
  /* Call the master RX interrupt function */
  MasterRxInterrupt_SPI();
}
/*******************************************************************************
* End of function Excep_RSPI1_SPRI1
*******************************************************************************/

/*******************************************************************************
* Outline     : SlaveRxInterrupt_SPI
* Description   : Slave receive interrupt function. This function checks if it
*          received a valid data frame from the master channel, and 
*          echoes the message back by copying it into the slave transmit
*          buffer.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SlaveRxInterrupt_SPI(void)
{
  /* Declare temporary slave rx frame buffer, and fetch value from SPI unit */
  uint16_t slave_rx = (uint16_t) RSPI0.SPDR.LONG;
  
  /* Declare temporary slave tx frame buffer */
  uint16_t slave_tx = 0;
  
  /* Check if master is requesting data */
  if((slave_rx & 0xFF00) == 0xA500)
  {
    /* Resend the last byte received with 0xC5 in front */
    slave_tx = (uint16_t) (0xC500 | (slave_rx & 0x00FF));
    
    /* Copy slave transmit data to the SPI unit */
    RSPI0.SPDR.LONG = (uint32_t) slave_tx << 16;
  }
  /* Master is requesting final byte */
  else if((slave_rx & 0xFF00) == 0xA300)
  {
    /* Resend the last byte received with 0xC3 in front */
    slave_tx = (uint16_t) (0xC300 | (slave_rx & 0x00FF));
    
    /* Copy slave transmit data to the SPI unit */
    RSPI0.SPDR.LONG = (uint32_t) slave_tx << 16;
  }
  /* Master is not sending valid request */
  else
  {
    /* Empty the transmit buffer */
    RSPI0.SPDR.LONG = 0x00000000;
  }
}
/*******************************************************************************
* End of function SlaveRxInterrupt_SPI
*******************************************************************************/  

/*******************************************************************************
* Outline     : MasterRxInterrupt_SPI
* Description   : Master receive interrupt function. This function checks if it
*          received a valid data frame from the slave channel, and 
*          copies it into the gMasterRxBuffer array.
* Argument      : none
* Return value  : none
*******************************************************************************/
void MasterRxInterrupt_SPI(void)
{
  /* Declare static frame count variable */
  static uint16_t master_rx_count = 0;
  
  /* Declare temporary master rx frame buffer, and fetch value from SPI unit */
  uint16_t master_rx = (uint16_t) RSPI1.SPDR.LONG;
      
  /* Check if slave is sending valid frame */
  if((master_rx  & 0xFF00) == 0xC500)
  {
    /* Extract byte from received data frame, append to master receive buffer */
    gMasterRxBuffer[master_rx_count] = (uint8_t) master_rx;
  
    /* Increment the number of received frames */
    master_rx_count++;
  }
  /* Check if the slave is sending a final frame */
  else if((master_rx  & 0xFF00) == 0xC300)
  {
    /* Extract final byte from received data frame, append to master receive buffer */
    gMasterRxBuffer[master_rx_count] = (uint8_t) master_rx;
     
    /* Reset the receive count to 0 */
    master_rx_count = 0;
  }
  /* Invalid data frame received from slave */
  else
  {
    /* Do nothing */
  }
}
/*******************************************************************************
* End of function MasterRxInterrupt_SPI
*******************************************************************************/
