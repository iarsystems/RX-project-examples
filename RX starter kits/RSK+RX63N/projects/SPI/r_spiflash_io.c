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
* File Name     : r_spiflash_io.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : MCU/Board level SPI Flash HAL. This file provides functions to
*                 implement and control SPI protocol communications, and is
*                 MCU and platform specific (RSK+RX63N). The functions defined
*                 are provided to the flash memory device HAL file, and should
*                 not be called in user application code. 
* Note          : 1. The functions defined in this file assume total control of
*                    the SPI channel, and any access to it outside these 
*                    funtions may cause undefined behaviour.
* Limitations   : 1. When transfering multiple bytes, the driver code polls the
*                    SPI unit until it reports it is idle before sending the
*                    next byte. In practice, this results in a time gap
*                    between 8bit transfers which reduces the effective bit rate
*                    of the transfers by approximately half. Using the MCU's 
*                    FIFO and packing data into 32bit transers could improve the
*                    effective bit rate dramatically.
*                 2. Most of the transfers are blocking functions, as interrupts
*                    are not used. Resource usage could be reduced by handled the
*                    transfers with an interrupt handler.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides standard boolean type definitions. */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines MCU registers used in this file */
#include "iorx63n.h"
/* Provides prototypes for functions defined in this file */
#include "r_spiflash_io.h"

/*******************************************************************************
* Private Local Function Prototypes
*******************************************************************************/
/* Wait until SPI unit is idle private local function prototype */
static bool idle_wait_io_spiflash(void);
/* Control chip select signal private location function prototype */
static void assert_cs_io_spiflash(bool);
/* Enable SPI channel RX/TX function prototype */
static void enable_channel_io_spiflash(bool);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* SPI streaming transfer enable status flag, private global */
static volatile bool gRxTxStreamEnabled = false;


#pragma vector=VECT_RSPI1_SPTI1
__interrupt void Excep_RSPI1_SPTI1(void)
{
  //Dummy interrupt handler. Added because if we don't define a handler
  //the default interrupt handler will be called when the interrupt occurs.
}


/*******************************************************************************
* Outline     : init_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function initialises the SPI channel for SPI, and must be
*                 called before using any other SPI functions.
* Argument      : None
* Return value  : Function will always return true (no error checking possible)
*******************************************************************************/
bool init_io_spiflash(void)
{
    /* PWPR.PFSWE write protect off */
  MPC.PWPR.BYTE = 0x00u;  
  
  /* PFS register write protect off */
  MPC.PWPR.BYTE = 0x40u;  
    
    /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503u;  
    
    /* Set MOSI, SSL & RSPCK pins as outputs */
    PORT2.PDR.BYTE |= 0xC0u;
    PORT3.PDR.BYTE |= 0x02u;
    
    /* Set MISO pin as input */
    PORT3.PDR.BYTE &= ~0x01u;
    
    /* Set pin mode for MOSI, MISO, RSPCK & SSL */
    PORT2.PMR.BYTE |= 0xC0u;
    PORT3.PMR.BYTE |= 0x03u;

    /* Set MOSI pin PFS register */
    MPC.P26PFS.BIT.PSEL = 0x0Du;
    
    /* Set MISO pin PFS register */
    MPC.P30PFS.BIT.PSEL = 0x0Du;
    
    /* Set RSPCK pin PFS register */
    MPC.P27PFS.BIT.PSEL = 0x0Du;
    
    /* Set SSL pin PFS register */
    MPC.P31PFS.BIT.PSEL = 0x0Du;
    
    /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503u;
  
  /* Wake RSPI unit from standby mode */
    MSTP(RSPI1) = 0u;
    
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500u;

    /* Set SPCR register
    SPMS   : 1 - SPI Operation (three-wire, with manual SSL control)
    TXMD   : 0 - Full duplex communcations
    MODFEN : 1 - Disable mode fault detection
    MSTR   : 1 - Master mode
    SPEIE  : 1 - Enable RSPI error interrupts 
    SPTIE  : 1 - Enable RSPI transmit interrupts
    SPE    : 0 - Disable RSPI function
    SPRIE  : 1 - Enables RSPI receive interrupts */
    RSPI1.SPCR.BYTE = 0xBDu;
                  
    /* Set SSL pin to active low */
    RSPI1.SSLP.BIT.SSL0P = 0u;
    
    /* Set SPPCR register
    SPLP   : 0 - Normal mode (not loopback)
    SPLP2  : 0 - Normal mode (not loopback)
    MOIFV  : 1 - Set MOSI output idle to high
    MOIFE  : 1 - Enable MOSI output idle fixing */
    RSPI1.SPPCR.BYTE = 0x30u;
    
    /* Set RSPI sequence control to stay in mode 0 */
    RSPI1.SPSCR.BYTE = 0u;
    
    /* Set bit rate to 24Mbit/s, by setting SPBR to 0 */
    RSPI1.SPBR = 0x00;
            
    /* Set SPCDR register 
    SPFC   : 00 - 1 frame per transmission round
    SPRDTD : 0  - SPDR reads from the rx buffer
    SPLW   : 0  - SPDR is accessed as a word */
    RSPI1.SPDCR.BYTE = 0x00u;
    
    /* Set RSPCK clock delay setting to 1 cycle
       (time between SSL assert and first clock cycle) */
    RSPI1.SPCKD.BIT.SCKDL = 0x00u;
    
    /* Set SSL negation delay setting to 1 cycle
       (time between last clock cycle and SSL negation) */
    RSPI1.SSLND.BIT.SLNDL = 0x00u;
    
    /* Set next access delay to 1 RSPCK clock + 2 PCLK cycles 
       (time between SSL negation and setting of tx complete flag) */
    RSPI1.SPND.BIT.SPNDL = 0x00u;
    
    /* Set SPCR2 register
    SPPE   : 0 - Do not add parity bit
    SPOE   : 0 - Sets even parity
    SPIIE  : 0 - Disables RSPI idle interrupts 
    PTE    : 0 - Parity function self-test */
    RSPI1.SPCR2.BYTE = 0x00u;
    
    /* Set SPCMD0 register (command register 0)
    CPHA   : 0    - Sample on odd edge, variation on even edge
    CPOL   : 0    - RSPCK is low when idle
    BRDV   : 00   - Bit rate divider set to 0(no division)
    SSLA   : 00   - Use slave select SSLB0
    SSLKP  : 0    - Negate SSL after end of transfer
    SPB    : 0100 - Data frame length 8 bits
    LSBF   : 0    - Most significant bit first
    SPNDEN : 1    - Next access delay is defined by SPND register
    SLNDEN : 1    - SSL negation delay is defined by SSLND register
    SCKDEN : 1    - RSPCK clock delay is defined by SPCKD register */
    RSPI1.SPCMD0.WORD = 0xE400u;
    
    /* Set SPI command pointer to command register 0 */
    RSPI1.SPSSR.BIT.SPCP = 0x00u;
    
    /* Enable the SPI TX buffer empty interrupt */
    IPR(RSPI1, SPTI1) = 5u;
    IR(RSPI1, SPTI1)  = 0u;
    IEN(RSPI1, SPTI1) = 1u;    
    
    /* Enable manual SSL control */
    PORT3.PODR.BIT.B1 = 1;
    PORT3.PMR.BIT.B1 = 0;
    
    /* Always return true */
    return true;
}
/*******************************************************************************
* End of function init_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : rxtx_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function performs a basic send and receive transaction
*                 over SPI. The chip select signal is asserted for the duration
*                 of the transaction only. The send and receive array pointers
*                 passed must be able to store the number of bytes in the 
*                 transaction (num_bytes), to prevent buffer overrun.
* Argument      : uint8_t * tx_source   : Pointer to array of tx bytes 
*                 uint8_t * rx_dest     : Pointer to array to store rx bytes
*                 uint32_t  num_bytes   : Number of bytes to send and receive
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. Function times out waiting for SPI unit to be idle
*                      before transaction starts.
*                   2. Function times out waiting for SPI unit to be idle
*                      after sending any of the bytes (indicates transfer fail).
*******************************************************************************/
bool rxtx_io_spiflash(uint8_t * tx_source, uint8_t * rx_dest, uint32_t num_bytes)
{
    /* Create modifable rx and tx pointers */
    uint8_t * rx_ptr = rx_dest;
    uint8_t * tx_ptr = tx_source;
    
    /* Delcare error flag */
    bool err = false;
    
    /* Enable the SPI channel */
    enable_channel_io_spiflash(true);
    
    /* Wait until SPI unit is idle (or timeout occurs) */
    if(idle_wait_io_spiflash())
    {    
        /* Assert the slave select signal before beginning transfer */
        assert_cs_io_spiflash(true);
        
        /* Loop through each byte to be transferred */
        for(uint8_t i = 0; i < num_bytes; i++)
        {
            /* Declare local transmit buffer, and shift data byte
            into most significant byte position */
            uint32_t tx_buffer = (uint32_t)(*tx_ptr << 16);
        
            /* Insert the transmit byte into the transmit register */
            TX_DATA_REGISTER_IO_SPIFLASH = tx_buffer;   
        
            /* Wait until rx/tx transmission is complete */
            if(!idle_wait_io_spiflash())
            {
                /* Timed out waiting for transmission to complete, set error flag */
                err = true;
            }
        
            /* Declare temporary buffer to hold recieved data */
            uint32_t rx_data = RX_DATA_REGISTER_IO_SPIFLASH;
        
            /* Shift received data to fetch byte, and insert into rx destination */
            *rx_ptr = (uint8_t)(rx_data >> 16u);
            
            /* Increment rx & tx pointers */
            rx_ptr++;
            tx_ptr++;
        }
    }
    /* Timed out waiting for SPI unit to become idle */
    else
    {   
        /* Time out occured, set error flag true */
        err = true;
    }
    
    /* Unassert the slave select signal */
    assert_cs_io_spiflash(false);
    
    /* Disable the SPI channel */
    enable_channel_io_spiflash(false);
    
    /* Declare return value, and set it to inverse of error status */
    bool ret = (bool)!err;

    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function rxtx_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : control_stream_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function starts or stops a streaming SPI transfer,
*                 depending on the input (control). Once a streaming transfer has
*                 been started, the chip select is held in the asserted state.
*                 The chip select is unassered when streaming transfer ends. 
*                 This allows per-byte transfers in the same transcation by
*                 calling the rxtx_stream_io_spiflash function. This function
*                 must be used to start and end a streaming transfer. Failing
*                 to correctly end a streaming transfer can lock up certain
*                 SPI flash devices.
* Argument      : bool control   : true  - starts streaming transfer
*                                : false - ends streaming transfer 
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. Attempts to start streaming transfer when there is one
*                      already in progress.
*******************************************************************************/
bool control_stream_io_spiflash(bool control)
{
    /* Check if control is true (start stream transfer) */
    if(control)
    {
        /* Check if read stream enabled flag has been set */
        if(gRxTxStreamEnabled)
        {
            /* Cannot start stream read stream, one already in progress */
            return false;
        }        
    
        /* Set the stream read enabled flag */
        gRxTxStreamEnabled = true;
        
        /* Enable the SPI channel */
        enable_channel_io_spiflash(true);
    
        /* Assert the chip select to start stream read */
        assert_cs_io_spiflash(true);
    }
    /* Control input is false (stop stream transfer) */
    else
    {
        /* Unassert chip select to end stream read */
        assert_cs_io_spiflash(false);
        
        /* Disable the SPI channel */
        enable_channel_io_spiflash(false);
        
        /* Clear the stream read enabled flag */
        gRxTxStreamEnabled = false;
    }
    
    /* Return true (successful) */
    return true;
}
/*******************************************************************************
* End of function control_stream_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : rxtx_stream_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function performs a single byte transfer in a streaming
*                 transaction. This allows per-byte operations of read or write
*                 data during a long transaction, and means all the send and 
*                 receive does not need to buffered before it can be operated
*                 on (useful for very large transactions). Function can only be
*                 called once a streaming transfer has started.
* Argument      : uint8_t * rx_ptr  - Pointer to location to store rx byte
*                 uint8_t   tx_byte - Byte to send
* Return value  : Function returns true if successful, false if unsuccessful.
*                 Function will fail in the following conditions:
*                   1. Streaming transfer has not been started
*                   2. Function times out wait for SPI unit to be idle after
*                      the transfer (indicates failure)
*******************************************************************************/
bool rxtx_stream_io_spiflash(uint8_t * rx_ptr, uint8_t tx_byte)
{
    /* Check if read stream enabled flag has not been set */
    if(!gRxTxStreamEnabled)
    {
        /* Cannot fetch byte, read stream has not started */
        return false;
    }
    
    /* Declare return value */
    bool ret = true;
    
    /* Declare local transmit buffer, and shift data byte
            into most significant byte position */
    uint32_t tx_buffer = (uint32_t)(tx_byte << 16);
    
    /* Send tx byte */
    TX_DATA_REGISTER_IO_SPIFLASH = tx_buffer;
    
    /* Wait until rx/tx transmission is complete */
    if(!idle_wait_io_spiflash())
    {
        /* Timed out waiting for transmission to complete, set return value
           to false (failure) */
        ret = false;
    }
    
    /* Declare temporary buffer to hold recieved data */
    uint32_t rx_data = RX_DATA_REGISTER_IO_SPIFLASH;
    
    /* Shift received data to fetch byte, and insert into rx destination */
    *rx_ptr = (uint8_t)(rx_data >> 16u);
    
    /* Return from function */
    return ret;
}
/*******************************************************************************
* End of function control_stream_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : assert_cs_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function performs asserts or unasserts the chip select
*                 signal. This function should only be called in SPI functions
*                 in this file.
* Argument      : bool control - true:  Assert chip select
*                                false: Unassert chip select
* Return value  : No return value.
*******************************************************************************/
static void assert_cs_io_spiflash(bool control)
{
    /* Check if control option is true */
    if(control)
    {
        /* Set chip select pin low (SPI active) */
        PORT3.PODR.BIT.B1 = 0;
    }
    /* Control option is false */
    else
    {
         /* Set chip select pin high (SPI inactive) */
         PORT3.PODR.BIT.B1 = 1;
    }
}
/*******************************************************************************
* End of function assert_cs_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : enable_channel_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This function enables or disables the send and receive
*                 capability of the SPI channel, depending on the input.
*                 This function should not be called outside SPI functions in
*                 this file.
* Argument      : bool control - true:  Enable SPI channel RX/TX
*                                false: Disable SPI channel RX/TX
* Return value  : No return value.
*******************************************************************************/
static void enable_channel_io_spiflash(bool control)
{
    /* Check if control value is true */
    if(control)
    {
        /* Set chip select pin low (SPI active) */
        RSPI1.SPCR.BIT.SPE = 1;
    }
    /* Control option is false */
    else
    {
        /* Set chip select pin low (SPI active) */
        RSPI1.SPCR.BIT.SPE = 0;
    }
}      
/*******************************************************************************
* End of function enable_channel_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : idle_wait_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This is a blocking function that waits in a while loop until
*                 the SPI channel is reported as idle (transfer complete) or a
*                 timeout occurs (timeout value defined in header file).
*                 in this file.
* Argument      : No argument
* Return value  : Returns true if SPI unit becomes idle before a timeout occurs,
*                 otherwise returns false (indicating failure).
*******************************************************************************/
bool idle_wait_io_spiflash(void)
{      
    /* Declare timeout counter variable, set to timeout value */
    uint32_t timeout = BUSY_TIMEOUT_MSEC_IO_SPIFLASH;
    
    /* Multiple timeout value by iterations-per-millisecond timing value */
    timeout *= MILLISECOND_TIME_IO_SPIFLASH;
        
    /* Wait until RSPI channel is idle or timeout occurs */
    while((0u != RSPI1.SPSR.BIT.IDLNF) && (timeout-- > 0u));

    /* Declare return value */
    bool ret = true;

    /* Check if timeout occured */
    if(0u == timeout)
    {
        /* Set return value to false (error) */
        ret = false;
    }
    
    /* Return from function */
    return ret;
}           
/*******************************************************************************
* End of function idle_wait_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : delay_msec_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This is a blocking delay function that waits the specified 
*                 number of millseconds (msec). The number of loop iterations
*                 per millisecond is defined in the header file.
* Argument      : No argument
* Return value  : No return value
*******************************************************************************/
/* Millisecond delay loop */
void delay_msec_io_spiflash(uint32_t msec)
{
    /* Multiply microseconds value with msec time factor */
    volatile uint32_t iterations = msec * MILLISECOND_TIME_IO_SPIFLASH;
    
    /* Wait the specified number of microseconds */
    while(iterations--)
    {
        ;
    }
}
/*******************************************************************************
* End of function delay_msec_io_spiflash
*******************************************************************************/

/*******************************************************************************
* Outline     : delay_usec_io_spiflash
* Module        : SPI Flash, MCU/Board Level HAL
* Description   : This is a blocking delay function that waits the specified 
*                 number of microseconds (usec). The number of loop iterations
*                 per microsecond is defined in the header file.
* Argument      : No argument
* Return value  : No return value
*******************************************************************************/
/* Microsecond delay loop */
void delay_usec_io_spiflash(uint32_t usec)
{
    /* Multiply microseconds value with msec time factor */
    volatile uint32_t iterations = usec * MICROSECOND_TIME_IO_SPIFLASH;
    
    /* Wait the specified number of microseconds */
    while(iterations--)
    {
        ;
    }
}
/*******************************************************************************
* End of function delay_usec_io_spiflash
*******************************************************************************/
