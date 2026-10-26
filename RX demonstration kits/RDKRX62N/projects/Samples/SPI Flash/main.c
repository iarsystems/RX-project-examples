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
* File Name     : main.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Demonstrates usage of Renesas Peripheral Development Library
*                 (RPDL) to write to/read from the RSPI flash device.
*                 This program writes 2048 bytes of data to the flash device 
*                 located on RSPI-0, 64 bytes at a time.  It then proceeds to
*                 read the data back 512 bytes at a time and compares it to the 
*                 original data.  Upon successful completion, LED 4 is turned on.
*                 However, if the programming fails, LED 14 is turned on.
*
* Operation     : Build and download the sample code to the RDK+.
* 
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/
/*******************************************************************************
* Project Includes
*******************************************************************************/    
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Defines LCD functions used in this file */
#include "lcd.h"
/* Graphics library support */
#include "glyph_api.h"

/* Numonxy P5Q flash device definitions */
#include "P5Q.h"

/* RPDL definitions */
#include "r_pdl_spi.h"
#include "r_pdl_io_port.h"
#include "r_pdl_definitions.h"

#include "YRDKRX62N.h"
/* RSPI API library support */
#include "YRDKRX62N_RSPI_API.h"

/* sample file data */
#include "data_file.h"


#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

/* Local functions */
bool VerifyFlashDevice(void);
bool ProgramFlashDevice(void);


/* buffer for read back and compare */
uint8_t g_Buff[512];

ParameterType   parameterBlock;
ParameterType   *pB = &parameterBlock;

/*******************************************************************************
* Outline      : main
* Description  : The main program function. 
*                This function initializes the hardware and then performs 
*                an erase/write/read/verify to the M5Q flash device on the SPI.
*                2000 bytes of data gets programmed into sector 7 of the device, 
*                which is located at offset 0x0E0000 in the device.
*                Results of the sequence get displayed on the LCD and LED4 is 
*                for success, and LED14 for failure. 
*
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  bool err = true;

  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();    

  /* Initialise the LCD display on the RSPI bus */
  YRDKRX62N_RSPI_Init(RSPI_CHANNEL_0);
  InitialiseLCD();

  /* Display instructions on the debug LCD */
  DisplayLCD(LCD_LINE1, "Micron PCM");

  /* check the SPI flash device's manufacturer and device ID */
  err &= VerifyFlashDevice();

  /* now perform an erase/write/read/verify to the flash device */
  err &= ProgramFlashDevice();

  /* display results on the LCD and light an appropriate LED */
  if ( err == true )
  {
    R_IO_PORT_Modify( LED4, PDL_IO_PORT_XOR, 1 );
    DisplayLCD(LCD_LINE7, "-- Done --");
  }
  else
  {
    R_IO_PORT_Modify( LED14, PDL_IO_PORT_XOR, 1 );
    DisplayLCD(LCD_LINE7, "- Failed -");
  }

  /* Infinite while loop */    
  while (1);

}
/*******************************************************************************
* End of function main
*******************************************************************************/

/*******************************************************************************
* Outline      : VerifyFlashDevice
* Description  : Routine to validate RDKRX62N SPI attached flash device. 
* Argument     : none
* Return value : bool true, OK
*                     false, failed to verify the device
*******************************************************************************/
bool VerifyFlashDevice()
{
  bool err = true;

  if ( Flash_Success != Flash( ReadManufacturerIdentification, pB ) )
  {
    err = false;
  }
  else
  {
    if ( Flash_Success != Flash( ReadDeviceIdentification, pB ) )
    {
      err = false;
    }
  }

  return err;
}
/*******************************************************************************
* End of function VerifyFlashDevice
*******************************************************************************/


/*******************************************************************************
* Outline      : ProgramFlashDevice
* Description  : Demonstrate programming the RDKRX62N SPI attached flash device. 
* Argument     : none
* Return value : bool true, OK
*                     false, failed to verify the device
*******************************************************************************/
bool ProgramFlashDevice()
{
  bool        err = true;
  int32_t     byte_count;
  uint16_t    data_count;    
  char        displayString[13] ;

  /*  Clear the section of flash to write to. */
  DisplayLCD(LCD_LINE3, "Erase...");

  parameterBlock.SectorErase.ustSectorNr = 7;
  if ( Flash_Success != Flash( SectorErase, pB ))
  {
    err = false;
    DisplayLCD(LCD_LINE3, "Erase   Fail");
  }
  else
  {
    DisplayLCD(LCD_LINE3, "Erase   Done");
  }

  /*  write "filedata" out to the flash device ( 64 bytes at a time ) */
  /*    Clear the counter before counting the no. of bytes to write        */
  byte_count = 0;
  DisplayLCD(LCD_LINE4, "Program...  ");

  // Repeat the write process until required no. of bytes are written 
  while ((byte_count < (sizeof(filedata) - 1)) && (err == true))
  {
    // update the P5Q parameter block with pertinent flash info
    parameterBlock.PageProgram.udAddr                = 0x0E0000 + byte_count;
    parameterBlock.PageProgram.udNrOfElementsInArray = FLASH_PAGE_SIZE; 
    parameterBlock.PageProgram.pArray                = (void *)(filedata + byte_count);

    if ( Flash_Success != Flash( WriteEnable, pB ) )
    {
      err = false;
    }

    if ( Flash_Success != Flash( PageProgram, pB ))
    {
      err = false;
    }

    if ( Flash_Success != Flash( WriteDisable, pB ) )
    {
      err = false;
    }

    sprintf(displayString, "%d ", byte_count) ;
    DisplayLCD(LCD_LINE5, (void *)displayString);

    /* Update the counter with the total no. of bytes read from the file */
    byte_count += parameterBlock.PageProgram.udNrOfElementsInArray;    
  }

  DisplayLCD(LCD_LINE4, "Program Done");

  /* Read the data back 512 bytes at at time and compare to databuffer */
  /* Clear the counter before counting the number of bytes read         */
  byte_count = 0;
  DisplayLCD(LCD_LINE5, "Verify... ");

  /* Repeat the read process until required no. of bytes are read */
  while ((byte_count < (sizeof(filedata) - 1)) && (err == true))
  {
    /* Read the data 512 bytes at a time. */
    parameterBlock.PageProgram.udAddr                = 0x0E0000 + byte_count;    
    parameterBlock.PageProgram.udNrOfElementsInArray = 512;
    parameterBlock.PageProgram.pArray                = (void *)g_Buff;

    if ( Flash_Success != Flash( Read, pB ))
    {
      err = false;
    }
    else
    {
      /* Compare the contents of the file with the ROM data    */
      for (data_count = 0 ; data_count < sizeof(g_Buff) ; data_count++)
      {
        if (g_Buff[data_count] != filedata[byte_count + data_count])
        {
          err = false;
        }
      }
    }

    sprintf(displayString, "%d ", byte_count) ;
    DisplayLCD(LCD_LINE6, (void *) displayString);

    /* Update the counter with the total no. of bytes read from the file */
    byte_count += parameterBlock.PageProgram.udNrOfElementsInArray;
  }

  DisplayLCD(LCD_LINE5, "Verify  Done");
  DisplayLCD(LCD_LINE6, "            ");


  return err;
}
/*******************************************************************************
* End of function ProgramFlashDevice
*******************************************************************************/

