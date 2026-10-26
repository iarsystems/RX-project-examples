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
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name    : usbdescriptors.c
* Version     : 1.00
* Device     : Renesas Generic MCU.
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK Generic
* Description   : Descriptors required to enumerate a device as a USB HID Class.
      This sets up a Report IN and a Report OUT.

      NOTE: This will need to be modified for a particular
      product as it includes company/product specific data including
      string descriptors specifying
      Manufacturer, Product and Serial Number.
**********************************************************************/
/**********************************************************************
* History     : 27.07.2010 Ver. 1.00 First Release
**********************************************************************/

/***********************************************************************************
User Includes
***********************************************************************************/
#include "usbdescriptors.h"

/***********************************************************************************
Defines
***********************************************************************************/
/*Vendor and Product ID*/
/*NOTE Please use your company Vendor ID when developing a new product.*/
#define VID 0x045B
#define PID 0x201B

/***********************************************************************************
Variables
***********************************************************************************/
/*  Device Descriptor  */
#define DEVICE_DESCRIPTOR_SIZE  18
static const uint8_t gDeviceDescriptorData[DEVICE_DESCRIPTOR_SIZE] = 
{
  /*Size of this descriptor*/
  DEVICE_DESCRIPTOR_SIZE,
  /*Device Descriptor*/
  0x01,
  /*USB Version 2.0*/
  0x00,0x02,
  /*Class Code - None as HID is defined in the Interface Descriptor*/
  0x00,
  /*Subclass Code*/
  0x00,
  /*Protocol Code*/
  0x00,
  /*Max Packet Size for endpoint 0*/
  CONTROL_IN_PACKET_SIZE,
  (uint8_t)(VID & 0xFF),  /*Vendor ID LSB*/
  (uint8_t)((VID>>8)& 0xFF),/*Vendor ID MSB*/
  (uint8_t)(PID & 0xFF),  /*Product ID LSB*/
  (uint8_t)((PID>>8)& 0xFF),/*Product ID MSB*/
  /*Device Release Number*/
  0x00,0x01,
  /*Manufacturer String Descriptor*/
  STRING_iMANUFACTURER,
  /*Product String Descriptor*/
  STRING_iPRODUCT,
  /*Serial Number String Descriptor*/
  STRING_iSERIAL,
  /*Number of Configurations supported*/
  0x01
};

const DESCRIPTOR gDeviceDescriptor =
{
  DEVICE_DESCRIPTOR_SIZE, gDeviceDescriptorData
};

/*
Configuration Descriptor
For HID this includes Interface Descriptors, HID descriptor and endpoint descriptor.
Ensure START_INDEX_OF_HID_WITHIN_CONFIG_DESC and HID_DESCRIPTOR_SIZE
are defined according to this.  
*/
#define CONFIG_DESCRIPTOR_SIZE_TOTAL  34 /*This includes Interfaces and Endpoints*/
static const uint8_t gConfigurationDescriptorData[CONFIG_DESCRIPTOR_SIZE_TOTAL] = 
{
  /*Size of this descriptor (Just the configuration part)*/
  0x09,
  /*Configuration Descriptor*/
  0x02,
  /*Combined length of all descriptors (little endian)*/
  CONFIG_DESCRIPTOR_SIZE_TOTAL,0x00,
  /*Number of interfaces*/
  0x01,
  /*This Interface Value*/
  0x01,
  /*No String Descriptor for this configuration*/
  0x00,
  /*bmAttributes - Self Powered(USB bus powered), No Remote Wakeup*/
  0xC0,
  /*bmAttributes - Not USB Bus powered, No Remote Wakeup*/    
  /*0x80,*/    
  /*bMaxPower (2mA units) 100mA (A unit load is defined as 100mA)*/
  50,

  /*Size of this descriptor*/
  0x09,
  /*INTERFACE Descriptor*/
  0x04,
  /*Index of Interface*/
  0x00,
  /*bAlternateSetting*/
  0x00,
  /*Number of Endpoints (Interrupt IN)*/
  0x01,
  /*HID Class code*/
  0x03,
  /*No Subclass*/
  0x00,
  /*No Protocol*/
  0x00,
  /*No String Descriptor for this interface*/
  0x00,
   
  /*Size of this descriptor*/
  0x09,
  /*HID Descriptor*/
  0x21,
  /*HID Class Specification Release Number 1.11*/
  0x11,0x01,
  /*No Target Country*/
  0x00,
  /*Number of HID Descriptors*/
  0x01,
  /*Type of HID Descriptor = "Report"*/
  0x22,
  /*Length Of HID Report Descriptor*/
  HID_REPORT_DESCRIPTOR_SIZE,00,

  /*Size of this descriptor*/
  0x07,
  /*ENDPOINT Descriptor*/
  0x05,
  /*bEndpointAddress - IN endpoint, endpoint number = 3*/
  0x83,
  /*Endpoint Type is Interrupt*/
  0x03,
  /*Max Packet Size*/
  INTERRUPT_IN_PACKET_SIZE,0x00,
  /*Polling Interval in mS*/
  0x0A
};

const DESCRIPTOR gConfigurationDescriptor = 
{
  CONFIG_DESCRIPTOR_SIZE_TOTAL, gConfigurationDescriptorData      
};

/*
Report Descriptor
NOTE The size of this must be HID_REPORT_DESCRIPTOR_SIZE
*/
static const uint8_t gHIDReportDescriptorData[HID_REPORT_DESCRIPTOR_SIZE] = 
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x02,                    //     REPORT_ID (2)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};

const DESCRIPTOR gHIDReportDescriptor = 
{
  /* Must match value specified in HID descriptor! */
  HID_REPORT_DESCRIPTOR_SIZE, 
  gHIDReportDescriptorData 
};

/*String Descriptors*/
  /*Note Language ID is in USB Core */

/*Manufacturer string*/
#define STRING_MANUFACTURER_SIZE 16
/* "Renesas" */
static const uint8_t gStringDescriptorManufacturerData[STRING_MANUFACTURER_SIZE] = 
{
  /* Length of this descriptor*/
  STRING_MANUFACTURER_SIZE,
  /* Descriptor Type = STRING */
  0x03,
  /* Descriptor Text (unicode) */
  'R', 0x00, 'E', 0x00, 'N', 0x00, 'E', 0x00,
  'S', 0x00, 'A', 0x00, 'S', 0x00
};

const DESCRIPTOR gStringDescriptorManufacturer = 
{
  STRING_MANUFACTURER_SIZE,
  gStringDescriptorManufacturerData  
};

/*Product string*/
#define STRING_PRODUCT_SIZE 74
/* "HID USB Demonstration" */
static const uint8_t gStringDescriptorProductData[STRING_PRODUCT_SIZE] = 
{
  /* Length of this descriptor*/
  STRING_PRODUCT_SIZE,
  /* Descriptor Type = STRING */
  0x03,
  /* Descriptor Text (unicode) */
  'H', 0x00, 'I', 0x00, 'D', 0x00, ' ', 0x00,
  'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00,
  'D', 0x00, 'e', 0x00, 'm', 0x00, 'o', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
};

const DESCRIPTOR gStringDescriptorProduct =
{
  STRING_PRODUCT_SIZE,
  gStringDescriptorProductData  
};

/* We need some way of telling the user what the IP Address is, especially   */
/* when DHCPC is used. We do this by appending the IP Address to the Product */
/* Data field of the HID descriptor. This shows up in registry key:          */
/*   HKLM\SYSTEM\ControlSet001\Enum\USB\Vid_045b&Pid_201b\1.1\LocationInformation  */
/* and also in the device properties' Location field of Device Manager.      */

uint8_t g_uiIpAddr[IPADDR_STRING_SIZE] = 
{ 
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
};

DESCRIPTOR *createDescriptorProductData(void)
{
   static uint8_t stringDescriptorProductData[STRING_PRODUCT_SIZE];
   static DESCRIPTOR stringDescriptorProduct =
   {
   STRING_PRODUCT_SIZE,
   stringDescriptorProductData  
   };
   
   for (unsigned i= 0; i<STRING_PRODUCT_SIZE; i++)
   {
     stringDescriptorProductData[i] = gStringDescriptorProductData[i];
   }
   for (unsigned i=28; i<STRING_PRODUCT_SIZE; i++)
   {
     stringDescriptorProductData[i] = g_uiIpAddr[i-28];
   }
 
   return &stringDescriptorProduct;
}

/*Serial number string "1.1" */
#define STRING_SERIAL_NUM_SIZE 36
static const uint8_t gStringDescriptorSerialNumData[STRING_SERIAL_NUM_SIZE] = 
{
  /* Length of this descriptor*/
  STRING_SERIAL_NUM_SIZE,
  /* Descriptor Type = STRING */
  0x03,
  /* Descriptor Text (unicode) */
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00, ' ', 0x00, ' ', 0x00, ' ', 0x00,
  ' ', 0x00
};

DESCRIPTOR *createDescriptorSerialNumData(void)
{
   static uint8_t stringDescriptorSerialNumData[STRING_SERIAL_NUM_SIZE];
   static DESCRIPTOR stringDescriptorSerial =
   {
   STRING_SERIAL_NUM_SIZE,
   stringDescriptorSerialNumData  
   };
   
   for (unsigned i= 0; i<STRING_SERIAL_NUM_SIZE; i++)
   {
     stringDescriptorSerialNumData[i] = gStringDescriptorSerialNumData[i];
   }
   for (unsigned i=2; i<STRING_SERIAL_NUM_SIZE; i++)
   {
     stringDescriptorSerialNumData[i] = g_uiMACAddr[i-2];
   }
 
   return &stringDescriptorSerial;
}


const DESCRIPTOR gStringDescriptorSerialNum = 
{
  STRING_SERIAL_NUM_SIZE,
  gStringDescriptorSerialNumData  
};
