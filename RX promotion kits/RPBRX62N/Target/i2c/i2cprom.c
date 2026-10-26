/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : i2cprom.c
* Version       : 1.0
* Device(s)     : RPBRX62N
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX62N
* Description   : IIC EEPROM read/write
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 06.10.2010  1.00    First release
*******************************************************************************/

#include <stdlib.h>

/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares functions defined in this file */

#include "i2cprom.h"

#define I2CSPEED  10

static unsigned start = 0;

/******************************************************************************
* Function name : ReadSDA
* Description   : Reads the IIC SDA line
* Arguments     : none
* Return Value  : the bit read
******************************************************************************/
static uint8_t ReadSDA(void)
{
  uint8_t bit;
  
  R_IO_PORT_Set
  (
    PDL_IO_PORT_1_3,
    PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON
  );
  
   R_IO_PORT_Read(PDL_IO_PORT_1_3, &bit);
  
  return bit;  
}

/******************************************************************************
* Function name : WriteSDA
* Description   : Writes the IIC SDA line
* Arguments     : bit - value to set SDA line to
* Return Value  : none
******************************************************************************/
static void WriteSDA(int bit)
{
  R_IO_PORT_Set
  (
    PDL_IO_PORT_1_3,
    PDL_IO_PORT_OUTPUT
  );
  
  R_IO_PORT_Write(PDL_IO_PORT_1_3, bit);
}

void ClrSDA(void)
{
   WriteSDA(0);
}

/******************************************************************************
* Function name : ReadSCL
* Description   : Reads the IIC SCL line
* Arguments     : none
* Return Value  : the bit read
******************************************************************************/
static uint8_t ReadSCL(void)
{
  uint8_t bit;
  
  R_IO_PORT_Set
  (
    PDL_IO_PORT_1_2,
    PDL_IO_PORT_INPUT | PDL_IO_PORT_INPUT_BUFFER_ON
  );
  
   R_IO_PORT_Read(PDL_IO_PORT_1_2, &bit);
  
  return bit;  
}

/******************************************************************************
* Function name : WriteSCL
* Description   : Writes the IIC SCL line
* Arguments     : bit - value to set the SDL line to
* Return Value  : none
******************************************************************************/
static void WriteSCL(int bit)
{
  R_IO_PORT_Set
  (
    PDL_IO_PORT_1_2,
    PDL_IO_PORT_OUTPUT
  );
  
  R_IO_PORT_Write(PDL_IO_PORT_1_2, bit);
}

void ClrSCL(void)
{
   WriteSCL(0);
}

/******************************************************************************
* Function name : I2CDelay
* Description   : Delays N milli-seconds
* Arguments     : none
* Return Value  : none
******************************************************************************/
static void I2CDelay(int _iHz)
{
  for (int i=0; i<100000; i++) ;
}

/******************************************************************************
* Function name : read_bit
* Description   : Reads one I2C bit following I2C protocol
* Arguments     : none
* Return Value  : the bit read
******************************************************************************/
static unsigned read_bit(void)
{
  unsigned bit;
  volatile int x;
 
  /* Let the slave drive data */
  x = ReadSDA();
  I2CDelay(I2CSPEED/2);
  /* Clock stretching */
  while (ReadSCL() == 0);
  /* SCL is high, now data is valid */
  bit = ReadSDA();
  I2CDelay(I2CSPEED/2);
  ClrSCL();
  return bit;
}

/******************************************************************************
* Function name : write_bit
* Description   : Writes one I2C bit following I2C protocol
* Arguments     : none
* Return Value  : none
******************************************************************************/
static int write_bit(unsigned bit)
{
  if (bit)
  {
    ReadSDA();
  }
  else
  {
    ClrSDA();
  }
  I2CDelay(I2CSPEED/2);
  /* Clock stretching */
  while (ReadSCL() == 0);
  /* SCL is high, now data is valid */
  /* If SDA is high, check that nobody else is driving SDA */
  if (bit)
  {
    if (ReadSDA() == 0)
    {
      return IIC_ARBITRATION_LOST;
    }
  }
  I2CDelay(I2CSPEED/2);
  ClrSCL();
  
  return 0;
}
 
/******************************************************************************
* Function name : start_cond
* Description   : Starts I2C message
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/

/* START CONDITION: A high-to-low transition of SDA with SCL high is a start */
/* condition which must precede any other command (see Figure 5 on page 8).  */
static int start_cond(void)
{
  volatile unsigned char x=7,y=7;
  
  x = ReadSDA();
  y = ReadSCL();
  if (start)
  {
    /* set SDA to 1 */
    ReadSDA();
    I2CDelay(I2CSPEED/2);
    /* Clock stretching */
    while (ReadSCL() == 0);
  }
  if (ReadSDA() == 0)
  {
    return IIC_ARBITRATION_LOST;
  }
  /* SCL is high, set SDA from 1 to 0 */
  ClrSDA();
  I2CDelay(I2CSPEED/2);
  ClrSCL();
  start = 1;
  return 0;
}
 
/******************************************************************************
* Function name : stop_cond
* Description   : Stops I2C message
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/

/* STOP CONDITION: A low-to-high transition of SDA with SCL high is a stop   */
/* condition. After a read sequence, the stop command will place the EEPROM  */
/* in a standby power mode (see Figure 5 on page 8).                         */
static int stop_cond(void)
{
  /* set SDA to 0 */
  ClrSDA();
  I2CDelay(I2CSPEED/2);
  /* Clock stretching */
  while (ReadSCL() == 0);
  /* SCL is high, set SDA from 0 to 1 */
  if (ReadSDA() == 0)
  {
    return IIC_ARBITRATION_LOST;
  }
  I2CDelay(I2CSPEED/2);
  start = 0;
  
  return 0;
}
 
/******************************************************************************
* Function name : send_byte
* Description   : Sends I2C byte
* Arguments     : byte - data byte to send
* Return Value  : 0 for success, !0 for error
******************************************************************************/
static int send_byte(unsigned char byte)
{
  unsigned bit;
  int      err = 0;
 
  for (bit = 0; bit < 8; bit++)
  {
    err = write_bit(byte & 0x80);
    if (err) return err;
    byte <<= 1;
  }
  return err;
}
 
/******************************************************************************
* Function name : read_byte
* Description   : Receives I2C byte
* Arguments     : none
* Return Value  : the 8-bit byte read from I2C
******************************************************************************/
static unsigned char read_byte(void)
{
  unsigned char byte = 0;
  unsigned bit;
 
  for (bit = 0; bit < 8; bit++)
  {
    byte <<= 1;    
    byte |= read_bit();    
  }
  return byte;
}

/******************************************************************************
* Function name : wait_ack
* Description   : Receives I2C byte
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/
static unsigned char wait_ack(void)
{
  unsigned char nack=0;
  
  nack = read_bit();

  if (nack == 0) return 0;
  
  return IIC_NO_ACKNOWLEDGE;
}

/******************************************************************************
* Function name : send_ack
* Description   : Receives I2C byte
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/
int send_ack(void)
{
  return write_bit(0);
}

/******************************************************************************
* Function name : i2c_eeprom_write
* Description   : Sends data to EEPROM address
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/
int i2c_eeprom_write(unsigned char address, unsigned char *data, int n)
{
  int err = 0;
  
  start = 1;
  
  err = start_cond();
  if (err) return err;
  
  /* Send device address word. The device address word consists of a mandatory one,*/
  /* zero sequence for the first four most significant bits as shown. This is      */
  /* common to all the EEPROM devices. Bits 3,2,1 are A2/A1/A0 the device address. */
  /* The 16K does not use any device address bits but instead the 3 bits are used  */
  /* for memory page addressing. These page addressing bits on the 4K, 8K and 16K  */
  /* devices should be considered the most significant bits of the data word       */
  /* address which follows. The A0, A1 and A2 pins are no connect.                 */
    /* The eighth bit of the device address is the read/write operation select bit.  */
  /* A read operation is initiated if this bit is high and a write operation is    */
  /* initiated if this bit is low.                                                 */
  /* Bits 7/6/5/4 is then 0xA (1010b), bits 3/2/1/0 is 0x0 (0000b) for page 0 Wr.  */
  err = send_byte(0xA0);
  if (err) return err;

  /* All addresses and data words are serially transmitted to and from the         */
    /* EEPROM in 8-bit words. The EEPROM sends a zero to acknowledge that it has     */
    /* received each word. This happens during the ninth clock cycle.                */
  err = wait_ack();
  if (err) return err;

  // 8-bit data word address
    /* BYTE WRITE: A write operation requires an 8-bit data word address following   */
    /* the device address word and acknowledgment. Upon receipt of this address, the */
    /* EEPROM will again respond with a zero and then clock in the first 8-bit data  */
    /* word. Following receipt of the 8-bit data word, the EEPROM will output a zero */
    /* and the addressing device, such as a microcontroller, must terminate the write*/
  /* sequence with a stop condition. At this time the EEPROM enters an internally  */
  /* timed write cycle, tWR, to the nonvolatile memory. All inputs are disabled    */
  /* during this write cycle and the EEPROM will not respond until the write is    */
  /* complete.                                                                     */
  err = send_byte(address);
  if (err) return err;

  err = wait_ack();
  if (err) return err;
  
  for (int i=0; i<n; i++)
  {
    err = send_byte(data[i]);
    if (err) return err;

    err = wait_ack();
    if (err) return err;
  }
  
  err = stop_cond();
  if (err) return err;

  return err;
}

/******************************************************************************
* Function name : i2c_eeprom_read
* Description   : Reads data from EEPROM address
* Arguments     : 
*   address - EEPROM address
*   data    - pointer to unsigned char buffer provided by caller
*   n       - the number of unsigned chars to read from EEPROM.
*             The buffer pointed to by data must be at least that size (n).
* Return Value  : 0 for success, !0 for error
******************************************************************************/
int i2c_eeprom_read(unsigned char address, unsigned char *data, int n)
{
  int err = 0;
  
  start = 1;
  
  /* RANDOM READ: A random read requires a “dummy” byte write sequence to load    */
  /* in the data word address. Once the device address word and data word address */
  /* are clocked in and acknowledged by the EEPROM, the microcontroller must      */
  /* generate another start condition.                                            */
  /* The microcontroller now initiates a current address read by sending a device */
  /* address with the read/write select bit high. The EEPROM acknowledges the     */
  /* device address and serially clocks out the data word. The microcontroller    */
  /* does not respond with a zero but does generate a following stop condition .  */
  
  /* Dummy write */
  err = start_cond();
  if (err) return err;
  
  err = send_byte(0xA0);
  if (err) return err;

  err = wait_ack();
  if (err) return err;
  
  err = send_byte(address);
  if (err) return err;

  err = wait_ack();
  if (err) return err;
  
  err = start_cond();
  if (err) return err;
  
  err = send_byte(0xA1);
  if (err) return err;

  err = wait_ack();
  if (err) return err;

  for (int i=0; i<n-1; i++)
  {  
    data[i] = read_byte();
    err = send_ack();
    if (err) return err;
  }
  
  data[n-1] = read_byte();

  /* The sequential read operation is terminated when the microcontroller */
  /* does not respond with a zero but does generate a following stop      */
  /* condition (see Figure 12 on page 12).                                */

  err = stop_cond();
  if (err) return err;

  return err;
}

/******************************************************************************
* Function name : i2c_eeprom_reset
* Description   : Restarts EEPROM I2C
* Arguments     : none
* Return Value  : 0 for success, !0 for error
******************************************************************************/
int i2c_eeprom_reset(void)
{
  /* MEMORY RESET: After an interruption in protocol, power loss or        */
  /* system reset, any 2-wire part can be reset by following these steps:  */
  /* 1. Clock up to 9 cycles.                                              */
  /* 2. Look for SDA high in each cycle while SCL is high.                 */
  /* 3. Create a start condition.                                          */
  volatile int x = ReadSDA();
  volatile int y = ReadSCL();
  for (int i=0; i<9; i++)
  {
    WriteSCL(0);
    I2CDelay(0);
    WriteSCL(1);
    I2CDelay(0);
    x = ReadSDA();
    y = ReadSCL();
    if (ReadSDA()&&ReadSCL()) return 0;
  }
  return IIC_RESET_FAILED;
}
