/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/

/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */

/**********************************************************************
* File Name    : usb_hid.c
* Version     : 1.00
* Device     : Renesas Generic MCU.
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK Generic
* Description   : Human Interface Device (HID) USB Class.
        Supports an IN and an OUT HID Report.

        NOTE: This module does not have any knowledge of the
        contents of the HID reports.    
******************************************************************************/

/******************************************************************************
* History     : 07.05.2009 Ver. 1.00 First Release
**********************************************************************/

/***********************************************************************************
System Includes
***********************************************************************************/
#include <assert.h>

/***********************************************************************************
User Includes
***********************************************************************************/
#include "usb_common.h"
#include "usb_hal.h"
#include "usb_core.h"
#include "usbdescriptors.h"
#include "usb_hid.h"

/***********************************************************************************
Defines
***********************************************************************************/
/* HID Descriptor Types */
#define HID_DESCRIPTOR       0x21
#define HID_REPORT_DESCRIPTOR   0x22

/* HID Class Request Codes */
#define GET_REPORT 0x01
#define SET_REPORT 0x09

/***********************************************************************************
Local Types
***********************************************************************************/

/***********************************************************************************
Variables
***********************************************************************************/
/*OUT Report buffer*/ 
static uint8_t g_OutputReport[OUTPUT_REPORT_SIZE];
/*IN Report buffer*/
static uint8_t g_InputReport[INPUT_REPORT_SIZE];
/*Callback when a Report OUT is received */
CB_REPORT_OUT g_CBReportOUT;

/***********************************************************************************
Private Function Prototypes
***********************************************************************************/
static USB_ERR ProcessStandardSetupPacket(SetupPacket* _pSetupPacket,
                      uint16_t* _pNumBytes,
                       uint8_t** _ppBuffer);
                      
static USB_ERR ProcessGetDescriptor(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                     const uint8_t** _ppBuffer);
                      
static USB_ERR ProcessClassSetupPacket(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                    uint8_t** _ppBuffer);

/*Callbacks required by USB_CORE*/
static USB_ERR CBUnhandledSetupPacket(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                    uint8_t** _ppBuffer);
                    
static void CBDoneControlOut(USB_ERR err, uint32_t NumBytes);
static void CBCable(bool _bConnected);
static void CBError(USB_ERR _err);
static void CBDoneInterruptIN(USB_ERR _err);

static void CopyInputReport(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE]);

/**********************************************************************
* Outline     : USBHID_Init
* Description   : Initialises this module.
* Argument    : _ReportIN - A report to send to the host when it asks
*          for it.
*          _cb - A Callback function to be called when a OUT
*          report is received.
* Return value  : Error Code.
**********************************************************************/

USB_ERR USBHID_Init(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE],
          CB_REPORT_OUT _cb)
{
  USB_ERR err = USB_ERR_OK;  

  DESCRIPTOR *descriptorProductData   = createDescriptorProductData();
  DESCRIPTOR *descriptorSerialNumData = createDescriptorSerialNumData();

  /*Initialise the USB core*/
  err = USBCORE_Init(gStringDescriptorManufacturer.pucData,
             gStringDescriptorManufacturer.length,
             descriptorProductData->pucData,
             descriptorProductData->length,
             descriptorSerialNumData->pucData,
             descriptorSerialNumData->length,
             gDeviceDescriptor.pucData,
             gDeviceDescriptor.length,
             gConfigurationDescriptor.pucData,
             gConfigurationDescriptor.length,
             (CB_SETUP_PACKET)CBUnhandledSetupPacket,
             (CB_DONE_OUT)CBDoneControlOut,
             (CB_CABLE)CBCable,
             (CB_ERROR)CBError);
  
  /*Store call back*/           
  g_CBReportOUT = _cb;
  
  /*Initilise the IN Report*/
  CopyInputReport(_ReportIN);
  
  return err;               
}
/***********************************************************************************
End of function USBHID_Init
***********************************************************************************/

/**********************************************************************
* Outline     : USBHID_ReportIN 
* Description   : Send IN Report to host.
*          Uses Interrupt IN.
* Argument    : _ReportIN: The report to send to the host.
* Return value  : Error code
**********************************************************************/

USB_ERR USBHID_ReportIN(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE])
{
  USB_ERR err = USB_ERR_OK;
  
  CopyInputReport(_ReportIN);
  
  /*Send Report using INTERRUPT IN*/
  USBHAL_Interrupt_IN(INPUT_REPORT_SIZE, g_InputReport, CBDoneInterruptIN);
  
  return err;
}
/***********************************************************************************
End of function USBHID_ReportIN
***********************************************************************************/

/**********************************************************************
* Outline     : CBUnhandledSetupPacket
* Description   : Called from USB core when it can't deal with a setup
*           packet.
*          This is a function of type CB_SETUP_PACKET.
*        
* Argument    : _pSetupPacket - Setup packet.
*          _pNumBytes - (OUT)Buffer size.
*          _ppBuffer - (OUT)Buffer.
*        
* Return value  : Error code
**********************************************************************/

static USB_ERR CBUnhandledSetupPacket(SetupPacket* _pSetupPacket,
                  uint16_t* _pNumBytes,
                  uint8_t** _ppBuffer)
{
  USB_ERR err = USB_ERR_OK;
  
  switch(EXTRACT_bmRequest_TYPE(_pSetupPacket->bmRequest))
  {
    case REQUEST_STANDARD:
    {
      /* Standard Type */
      err = ProcessStandardSetupPacket(_pSetupPacket,
                       _pNumBytes, _ppBuffer);
      break;
    }
    case REQUEST_CLASS:
    {
      /*Class Type */
      err = ProcessClassSetupPacket(_pSetupPacket, _pNumBytes, _ppBuffer);
      break;
    }
    case REQUEST_VENDOR:
    default:
    {
      DEBUG_MSG_LOW(("USBHID: Unsupported Request type\r\n"));
      err = USB_ERR_UNKNOWN_REQUEST;
    }
  }
  
  return err;
}
/**********************************************************************************
End of function CBUnhandledSetupPacket
***********************************************************************************/   

/**********************************************************************
* Outline     : ProcessStandardSetupPacket
* Description   : Process a Standard Setup Packet that the lower layers couldn't.
*          This function does not return.
* Argument    : _pSetupPacket: Setup Packet
*          _pNumBytes: (OUT)If this  can handle this then
*                 this will be set with the size of the data.
*                 (If there is no data stage then this will be set to zero)
*          _ppBuffer: (OUT)If this  can handle this then
*                 this will be set to point to the data(IN) or a buffer(OUT).
*                 (If there is a data stage for this packet).
* Return value  : Error Code
**********************************************************************/

static USB_ERR ProcessStandardSetupPacket(SetupPacket* _pSetupPacket,
                      uint16_t* _pNumBytes,
                       uint8_t** _ppBuffer)
{
  USB_ERR err = USB_ERR_OK;
    
  switch(_pSetupPacket->bRequest)
  {
    case GET_DESCRIPTOR:
    {
      err = ProcessGetDescriptor(_pSetupPacket, _pNumBytes,
                  (const uint8_t**)_ppBuffer);
      break;
      
    }
    default:
    {
      DEBUG_MSG_LOW(("USBHID: Unsupported Standard Setup request %d\r\n",
             _pSetupPacket->bRequest));
      err = USB_ERR_UNKNOWN_REQUEST;  
    }
  }

  return err;
}
/**********************************************************************************
End of function ProcessStandardSetupPacket
***********************************************************************************/   

/**********************************************************************
* Outline     : ProcessGetDescriptor
* Description   : Process a Get Descriptor request that the lower layers couldn't.
* Argument    : _pSetupPacket: Setup Packet
*          _pNumBytes: (OUT)If this  can handle this then
*                 this will be set with the size of the data.
*                 (If there is no data stage then this will be set to zero)
*          _ppBuffer: (OUT)If this  can handle this then
*                 this will be set to point to the data(IN)(Descriptor).
*                 (If there is a data stage for this packet).
* Return value  : Error Code
**********************************************************************/

static USB_ERR ProcessGetDescriptor(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                     const uint8_t** _ppBuffer)
{
  USB_ERR err = USB_ERR_OK;
  
  /*USB Core handles normal Get Descriptor requests,
  so only need to support HID specific here.*/
  uint8_t DescriptorType = (uint8_t)((_pSetupPacket->wValue >> 8) & 0x00FF);
  switch(DescriptorType)
  {
    /* Hid Descriptor */
    case HID_DESCRIPTOR:
    {
      DEBUG_MSG_LOW(("USBHID: GET_DESCIPTOR - HID_DESCRIPTOR\r\n"));
      /* The HID Descriptor is stored within the configuration descriptor */
      /*Data IN response */
      *_pNumBytes = HID_DESCRIPTOR_SIZE;
      *_ppBuffer = &gConfigurationDescriptor.
              pucData[START_INDEX_OF_HID_WITHIN_CONFIG_DESC];
      break;
    }

    /* HID Report Descriptor */
    case HID_REPORT_DESCRIPTOR:
    {
      DEBUG_MSG_LOW(("USBHID: GET_DESCIPTOR - HID_REPORT_DESCRIPTOR\r\n"));
      /*Data IN response */
      *_pNumBytes = HID_REPORT_DESCRIPTOR_SIZE;
      *_ppBuffer = gHIDReportDescriptor.pucData;

      break;
    }
    default:
    {
      DEBUG_MSG_LOW(("USBHID: Unsupported GetDescriptor type %d\r\n",
             DescriptorType));
      err = USB_ERR_UNKNOWN_REQUEST;  
    }
  }
  
  return err;
}
/**********************************************************************************
End of function ProcessGetDescriptor
***********************************************************************************/   

/**********************************************************************
* Outline     : ProcessClassSetupPacket
* Description   : Process HID Class specific requests.
* Argument    : _pSetupPacket: Setup Packet
*          _pNumBytes: (OUT)If this  can handle this then
*                 this will be set with the size of the data.
*                 (If there is no data stage then this will be set to zero)
*          _ppBuffer: (OUT)If this  can handle this then
*                 this will be set to point to the data(IN) or a buffer(OUT).
*                 (If there is a data stage for this packet).
* Return value  : Error Code
**********************************************************************/

static USB_ERR ProcessClassSetupPacket(SetupPacket* _pSetupPacket,
                    uint16_t* _pNumBytes,
                     uint8_t** _ppBuffer)
{
  USB_ERR err = USB_ERR_OK;

  switch(_pSetupPacket->bRequest)
  {
    case SET_REPORT:
    {
      DEBUG_MSG_LOW(("USBHID: SET_REPORT\r\n"));
      /*HOST is going to send the output report*/
      /*Data OUT*/
      *_pNumBytes = OUTPUT_REPORT_SIZE;
      *_ppBuffer = g_OutputReport;
      break;
    }
    case GET_REPORT:
    {
      DEBUG_MSG_LOW(("USBHID: GET_REORT\r\n"));
      /*HOST has requested the input report*/
      /*Data IN*/
      *_pNumBytes = INPUT_REPORT_SIZE;
      *_ppBuffer = g_InputReport;
      break;
    }
    default:
    {
      DEBUG_MSG_LOW(("USBHID: Unsupported Class Setup request %d\r\n",
             _pSetupPacket->bRequest));
      err = USB_ERR_UNKNOWN_REQUEST;
    }
  }
  
  return err;
}
/***********************************************************************************
End of function ProcessClassSetupPacket
***********************************************************************************/

/**********************************************************************
* Outline     : CBDoneControlOut
* Description   : A Control Out has completed in response to a
*          setup packet handled in CBUnhandledSetupPacket.
* Argument    : _err: Error Code
*          _NumBytes: Number of bytes received.
* Return value  : none
**********************************************************************/

static void CBDoneControlOut(USB_ERR _err, uint32_t _NumBytes)
{
  DEBUG_MSG_LOW(("USBHID: CBDoneControlOut. Bytes =  %d\r\n", _NumBytes));
             
  /*Assume this is a SET_REPORT data stage*/
  assert(USB_ERR_OK == _err);
  assert(OUTPUT_REPORT_SIZE == _NumBytes);
  
  /*Call registered callback*/
  g_CBReportOUT((uint8_t(*)[OUTPUT_REPORT_SIZE])g_OutputReport);
}

/***********************************************************************************
End of function CBDoneControlOut
***********************************************************************************/   

/**********************************************************************
* Outline     : CBDoneInterruptIN 
* Description   : A Control IN has completed in response to a
*          setup packet handled in CBUnhandledSetupPacket.
* Argument    : _err: Error Code
* Return value  : none
**********************************************************************/

static void CBDoneInterruptIN(USB_ERR _err)
{
  assert(USB_ERR_OK == _err);
  /*Nothing to do*/
} 
/**********************************************************************************
End of function CBDoneInterruptIN
***********************************************************************************/   

/**********************************************************************
* Outline     : CBCable 
* Description   : Callback function called when the USB cable is
*          Connected/Disconnected.
*          Sets connected flag.
*        
* Argument    : _bConnected: true = Connected, false = Disconnected.
* Return value  : none
**********************************************************************/

static void CBCable(bool _bConnected)
{  
  if(true == _bConnected)
  {
    DEBUG_MSG_LOW(("USBHID: Cable Connected\r\n"));
  }
  else
  {
    DEBUG_MSG_LOW(("USBHID: Cable Disconnected\r\n"));
  }
}
/**********************************************************************************
End of function CBCable
***********************************************************************************/   

/**********************************************************************
* Outline     : CBError 
* Description   : One of the lower layers has reported an error.
*          Not expecting this but try resetting HAL.
* Argument    : none
* Return value  : none
**********************************************************************/

static void CBError(USB_ERR _err)
{
  DEBUG_MSG_LOW(("USBHID: ***CBError***\r\n"));
  assert(0);
  
  /*Reset HAL*/
  USBHAL_Reset();
}
/**********************************************************************************
End of function CBError
***********************************************************************************/   

/**********************************************************************
* Outline     : CopyInputReport
* Description   : Copy supplied input report to input report buffer.
* Argument    : _ReportIN - Input report
* Return value  : none
**********************************************************************/

static void CopyInputReport(uint8_t (*_ReportIN)[INPUT_REPORT_SIZE])
{
  uint16_t index;
    
  /*Copy INPUT Report - so can send it whenever a GET_REPORT
  request is received.*/
  for(index = 0; index<INPUT_REPORT_SIZE; index++)
  {
    g_InputReport[index] = (*_ReportIN)[index];
  }
}
/**********************************************************************************
End of function CopyInputReport
***********************************************************************************/   
