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
* File Name     : libusb_app.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description : This LibUSB application will allow a host using LibUSB to
*          connect to this device.
*
*          Please note that LibUSB must be installed on your host PC before
*               using this. For more details see 'Renesas Starter Kit USB Sample 
*               Code User's Manual'.
*          The host will be able to send data, via BULK OUT,
*          to the device to make it:
*            1. Toggle a LED.
*            2. Read the ADC. (Device responds with BULK IN)
*            3. Write to the LCD.
*        
*          The Device will tell the host, via INTERRUPT IN, when a user
*          has pressed a switch.
*          See below "FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE".
***********************************************************************************/
/***********************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
***********************************************************************************/

/***********************************************************************************
System Includes
***********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Provides a structure to access on-chip I/O registers. */
#include "iorx63n.h"
/* Common defines for widely used items. */



/***********************************************************************************
User Includes
***********************************************************************************/
/* The following header file defines common functions used for the rskRX63Ndef */
#include "rskrx63ndef.h"
/* The following header file defines functions used for the ADC */
#include "adc.h"
/* The following header file defines functions used in file hwsetup.c */
#include "hwsetup.h"
/* The following header file defines functions used in file switch.c */
#include "switch.h"
/* The following header file defines functions used for the LCD */
#include "lcd.h"
/* The following header file defines functions used in file usb_hid.c */
//#include "usb_hid.h"
/* The following header file defines functions used in file usb_hal.c */
#include "usb_hal.h"
/* The following header file defines functions used in file usb_core.c */
#include "usb_core.h"
/* The following header file defines functions used in file usbdescriptors.c */
#include "usbdescriptors.h"
/* The following header file defines functions used in file libusb_app.c */
#include "libusb_app.h"

/***********************************************************************************
Defines
***********************************************************************************/

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE*/
/*
OUT
The host sends (OUT transfer) 17 bytes.
The first byte is a cmd byte the following 16 are the 16
ASCII LCD characters values.
The bits in the cmd byte are defined below:
*/
#define OUT_BIT_LED 0x01 /*Toggle LED*/
#define OUT_BIT_ADC 0x02 /*Read ADC*/
#define OUT_BIT_LCD 0x04 /*Update LCD*/
/*
IN
The device sends (IN transfer) 5 bytes.
The first byte contains information as defined by the bits below.
The remaining 4 bytes (may) hold a 32 bit ADC value in little endian. 
*/
#define IN_BIT_LED  0x01 /*LED state (1 = ON, 0 = OFF)*/
#define IN_BIT_ADC  0x02 /*ADC data is valid*/
#define IN_BIT_SW  0x04 /*A switch has been pressed.*/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*Max amount of data that host will send*/
#define CMD_BUFFER_SIZE 1 + (2*NUMB_CHARS_PER_LINE)
/*Max amount of data that device will send*/
#define RSP_BUFFER_SIZE 5
/***********************************************************************************
Variables
***********************************************************************************/

/*Connected flag*/
static volatile bool g_bConnected = false;
static uint8_t CMD_Buffer[CMD_BUFFER_SIZE];
static volatile bool g_bWaitForCmd;
static volatile bool g_bWaitForSwitch;
static volatile USB_ERR g_CmdError;
static uint8_t RSP_Buffer[RSP_BUFFER_SIZE]; 

/***********************************************************************************
Private Function Prototypes
***********************************************************************************/
static USB_ERR CBUnhandledSetupPacket(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                    uint8_t** _ppBuffer);
static void CBDoneControlOut(USB_ERR _err, uint32_t _NumBytes);                                      
static void CBCable(bool _bConnected);
static void CBError(USB_ERR _err);

/*BULK Callbacks*/
static void CBDoneBulkOut(USB_ERR _err, uint32_t _NumBytes);
static void CBDoneBulkIn(USB_ERR _err);

static void SendINData(bool _bADC, bool _bSWPressed, uint32_t _ADCValue,
            bool _bUseInterruptPipe);
            
static void DecipherOUT(uint8_t* _Buffer);
/* Function is used to de-bounce the switches. */
static void SwitchHandler(void);

/**********************************************************************************
* Outline     : LibUSB_APP_Main
* Description   : Starts the LIB USB application.
*          This will not return unless there is an error.
* Argument    : none
* Return value  : Error code.
**********************************************************************************/
USB_ERR LibUSB_APP_Main(void)
{  
  USB_ERR err;
  
  DEBUG_MSG_LOW( ("LibUSB: Application Starting.\r\n"));
  
  /*LCD*/
  Init_LCD();
  Display_LCD(LCD_LINE1,"Renesas ");
  Display_LCD(LCD_LINE2,"LibUSB  ");
  
  /* Configure the ADC channel that is connected to the potentiometer. AN0 */
    Init_ADC();
    
  /*Initialise the USB core*/
  err = USBCORE_Init(gStringDescriptorManufacturer.pucData,
             gStringDescriptorManufacturer.length,
             gStringDescriptorProduct.pucData,
             gStringDescriptorProduct.length,
             gStringDescriptorSerialNum.pucData,
             gStringDescriptorSerialNum.length,
             gDeviceDescriptor.pucData,
             gDeviceDescriptor.length,
             gConfigurationDescriptor.pucData,
             gConfigurationDescriptor.length,
             (CB_SETUP_PACKET)CBUnhandledSetupPacket,
             (CB_DONE_OUT)CBDoneControlOut,
             (CB_CABLE)CBCable,
             (CB_ERROR)CBError);
  
  
  if(USB_ERR_OK == err)
  {
    /*Register a callback for when an RSK switch is pressed*/
    SetSwitchPressCallback(SwitchHandler);
    
    while(1)
    {    
      /*If not connected*/
      if(false == g_bConnected)
      {
        /*Wait for connection*/
        while(false == g_bConnected);
        /*Reset HAL incase it hasn't just been initialised.*/
        USBHAL_Reset();
        
        /*Reset cmd wait flag*/
        g_bWaitForCmd = false;
      }
      
      /*Reset switch wait flag*/
      g_bWaitForSwitch = true;
      
      /*If not already waiting for a command*/
      if(false == g_bWaitForCmd)
      {   
        /*Be ready to receive a command from the host*/
        g_bWaitForCmd = true;
        err = USBHAL_Bulk_OUT(CMD_BUFFER_SIZE, CMD_Buffer,
                       (CB_DONE_OUT)CBDoneBulkOut);
      }
  
      if(USB_ERR_OK == err)
      {               
        /*Wait for cmd to be received or a switch to be pressed*/
        while((true == g_bWaitForCmd) &&
            (true == g_bConnected) &&
            (true == g_bWaitForSwitch)){;}
  
        /*Process Cmd if have received one OK*/
        if((false == g_bWaitForCmd) && (USB_ERR_OK == g_CmdError))
        {
          DEBUG_MSG_LOW( ("LibUSB: Cmd Received.\r\n"));
          DecipherOUT(CMD_Buffer);
        }
        
        /*If a switch was pressed then inform host*/
        if(false == g_bWaitForSwitch)
        {
          /*Use Interrupt Pipe*/
          SendINData(false, true, 0, true);
        }
      }
    }
  }
  
  return err;
}

/***********************************************************************************
* End of function LibUSB_APP_Main
***********************************************************************************/

/***********************************************************************************
* Outline     : CBUnhandledSetupPacket
* Description   : Called from the USB Core when it can't deal with
*          a setup packet.
*          Compiler warning may occur as arguments are unused
*                 in this application, but required by API
*                 
* Argument    : _pSetupPacket - Setup packet.
*          _pNumBytes - (OUT)Buffer size.
*          _ppBuffer - (OUT)Buffer.
* Return value  : none
***********************************************************************************/
static USB_ERR CBUnhandledSetupPacket(SetupPacket* _pSetupPacket,
                  uint16_t* _pNumBytes,
                  uint8_t** _ppBuffer)
{
  /*Don't support any setup requests - 
    relying on the USBCore completely.*/
  return USB_ERR_UNKNOWN_REQUEST;
}
/***********************************************************************************
* End of function CBUnhandledSetupPacket
***********************************************************************************/

/**********************************************************************************
* Outline     : CBDoneControlOut
* Description   : A Control Out has completed in response to a
*          setup packet handled in CBUnhandledSetupPacket.
*        
*          NOT USED - compiler warning may occur as arguments
*                 are unused in this application, but required by API
*        
* Argument    : _err - Error code.
*          _NumBytes - The number of bytes received from the host.  
* Return value  : none
**********************************************************************************/
static void CBDoneControlOut(USB_ERR _err, uint32_t _NumBytes)
{
  /*Not expecting this*/
  assert(0);
}
/***********************************************************************************
* End of function CBDoneControlOut
***********************************************************************************/

/***********************************************************************************
* Outline     : CBCable
* Description   : Callback when the USB cable is connected or disconnected.
*          Sets g_bConnected flag.
* Argument    : _bConnected: true = Connected, false = Disconnected.
* Return value  : none
***********************************************************************************/
static void CBCable(bool _bConnected)
{  
  if(true == _bConnected)
  {
    DEBUG_MSG_LOW( ("LibUSB: Cable Connected\r\n"));  
    
    g_bConnected = true;
  }
  else
  {
    DEBUG_MSG_LOW( ("LibUSB: Cable Disconnected\r\n"));
    g_bConnected = false;
  }
} 
/***********************************************************************************
* End of function CBCable
***********************************************************************************/

/***********************************************************************************
* Outline     : CBError 
* Description   : Callback saying that an error has occoured in a
*           lower layer.
* Argument    : _err - error code.
* Return value  : none
***********************************************************************************/
static void CBError(USB_ERR _err)
{
  DEBUG_MSG_LOW(("LibUSB: ***CBError***\r\n"));
  
  /*If the terminal sends data to us when we're not expecting any
  then would get this and it's not an error.
  However, could be that data is arriving too quick for us to read
  and echo out.*/
  if(USB_ERR_BULK_OUT_NO_BUFFER == _err)
  {
    DEBUG_MSG_LOW(("LibUSB: No Bulk Out Buffer.\r\n"));
  }
  else
  {
    /*Try resetting HAL*/
    USBHAL_Reset();
  }
}
/***********************************************************************************
* End of function CBError
***********************************************************************************/

/***********************************************************************************
* Outline     : CBDoneBulkOut
* Description   : A Bulk OUT has completed.
*          Set the g_bWaitForCmd flag.
* Argument    : _err: Error code
*          _NumBytes: Number of bytes received
* Return value  : none
***********************************************************************************/
static void CBDoneBulkOut(USB_ERR _err, uint32_t _NumBytes)
{
  assert(USB_ERR_OK == _err);
  assert(_NumBytes == CMD_BUFFER_SIZE);
  
  /*Cmd has been received*/
  g_CmdError = _err;
  g_bWaitForCmd = false;
}
/***********************************************************************************
* End of function CBDoneBulkOut
***********************************************************************************/

/***********************************************************************************
* Outline     : CBDoneBulkIn 
* Description   : Bulk IN has completed.
* Argument    : _err: Error code
* Return value  : none
***********************************************************************************/
static void CBDoneBulkIn(USB_ERR _err)
{
  assert(USB_ERR_OK == _err);
  /*This will occur after sending the ADC value to the host.*/
  /*Nothing to do*/
}
/***********************************************************************************
* End of function CBDoneBulkIn
***********************************************************************************/

/***********************************************************************************
* Outline     : SendINData
* Description   : Create the data to send to the host.
*          Send to the host using bulk IN or interrupt IN pipe.
* Argument    : _bADC:    true if providing new ADC value.
*          _bSWPressed:true if switch pressed.
*          _ADCValue:  ADC Value.
*          _bUseInterruptPipe: true to use Interrupt Pipe
*                  false to use BULK pipe.
* Return value  : none
***********************************************************************************/
static void SendINData(bool _bADC, bool _bSWPressed, uint32_t _ADCValue,
            bool _bUseInterruptPipe)
{
  /*Clear to start*/
  RSP_Buffer[0] = 0;
  
  /*Set LED Bit*/
  if(LED_ON == LED0)
  {
    RSP_Buffer[0] |= IN_BIT_LED;
  }

  /*Set new ADC value bit?*/
  if(false != _bADC)
  {
    RSP_Buffer[0] |= IN_BIT_ADC;
  }
  
  /*Set switch toggled  bit?*/
  if(false != _bSWPressed)
  {
    RSP_Buffer[0] |= IN_BIT_SW;
  }
  
  /*Set ADC Value (even if not setting new ADC value bit)*/
  /*Little Endian*/
  RSP_Buffer[1] = (uint8_t)(_ADCValue & 0xFF);
  RSP_Buffer[2] = (uint8_t)((_ADCValue >> 8) & 0xFF);
  RSP_Buffer[3] = (uint8_t)((_ADCValue >> 12) & 0xFF);
  RSP_Buffer[4] = (uint8_t)((_ADCValue >> 16) & 0xFF);
  
  if(false == _bUseInterruptPipe)
  {
    /*Send using BULK IN*/
    USBHAL_Bulk_IN(RSP_BUFFER_SIZE, RSP_Buffer, CBDoneBulkIn);
  }
  else
  {
    /*Send using INTERRUPT IN*/
    USBHAL_Interrupt_IN(RSP_BUFFER_SIZE, RSP_Buffer, CBDoneBulkIn);
  }
}
/***********************************************************************************
* End of function SendINData
***********************************************************************************/

/***********************************************************************************
* Outline     : DecipherOUT  
* Description   : Desciphers the OUT transfer
* Argument    : none
* Return value  : none
***********************************************************************************/
static void DecipherOUT(uint8_t* _Buffer)
{  
  uint16_t ADCValue;
  volatile bool bRPDLReturn;
  
  /*Toggle LED?*/
  if(0 != (_Buffer[0] & OUT_BIT_LED))
  {
    LED0 = (uint8_t)~LED0;
  }
  
  /*Read ADC?*/
  if(0 != (_Buffer[0] & OUT_BIT_ADC))
  {
    /* Start AD conversion */
    Start_ADC();
    
    /* Fetch ADC value */
    ADCValue = Read_ADC();
    
    DEBUG_MSG_LOW( ("LibUSB: ADC Value = %hd\r\n", ADCValue));
    
    /*Send response*/
    /*Use BULK Pipe*/
    SendINData(true, false, (uint32_t)ADCValue, false);
  }
  
  /*Update LCD*/
  if(0 != (_Buffer[0] & OUT_BIT_LCD))
  {
    char szLine1[NUMB_CHARS_PER_LINE+1];
    char szLine2[NUMB_CHARS_PER_LINE+1];
    szLine1[NUMB_CHARS_PER_LINE] = '\0';
    szLine2[NUMB_CHARS_PER_LINE] = '\0';
    memcpy(szLine1, &_Buffer[1], NUMB_CHARS_PER_LINE);
    memcpy(szLine2, &_Buffer[1+NUMB_CHARS_PER_LINE], NUMB_CHARS_PER_LINE);
    
    Display_LCD(LCD_LINE1, (uint8_t*)szLine1);
    Display_LCD(LCD_LINE2, (uint8_t*)szLine2);
  }
}
/***********************************************************************************
* End of function DecipherOUT
***********************************************************************************/

/***********************************************************************************
* Outline     : SwitchHandler  
* Description   : Called when a RSK switch is pressed.
* Argument    : none
* Return value  : none
***********************************************************************************/
void SwitchHandler(void)
{
  /*Set flag to say a switch has been pressed*/
  g_bWaitForSwitch = false;
} 
/***********************************************************************************
* End of function SwitchHandler
***********************************************************************************/

