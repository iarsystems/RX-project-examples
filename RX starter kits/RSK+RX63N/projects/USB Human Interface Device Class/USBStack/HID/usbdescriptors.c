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
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Descriptors required to enumerate a device as a USB HID Class.
*      This sets up a Report IN and a Report OUT.
*
*      NOTE: This will need to be modified for a particular
*      product as it includes company/product specific data including
*      string descriptors specifying
*      Manufacturer, Product and Serial Number.
***********************************************************************************/
/***********************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
***********************************************************************************/

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
#define PID 0x2013

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
  /* Usage Page - Vendor defined*/
  0x06, 0xA0, 0xFF,
  /* Usage ID within this page (Vendor defined)*/
  0x09, 0x00,
  /* Collection App (Windows requires an Application Collection) */
  0xA1, 0x01,
  
  /* *** The INPUT REPORT *** */
  /* Usage ID within this page*/
  0x09, 0x00,
  /*Logical Min 0 */
  0x15, 0x00,
  /*Logical Max 255 */
  0x26, 0xFF, 0x00,  
  /* Size 8 Bits (Each Field will be 8bits) */
  0x75, 0x08,
  /* Count (Number of fields(bytes) in INPUT report) */
  0x95, INPUT_REPORT_SIZE,
  /* Input Report - type variable data */
  0x81, 0x02,
  
  /* *** The OUTPUR REPORT *** */
  /* Usage ID within this page (Vendor defined)*/
  0x09, 0x00,
  /*Logical Min 0 */
  0x15, 0x00,
  /*Logical Max 255 */
  0x26, 0xFF, 0x00,  
  /* Size 8 Bits (Each Field will be 8bits) */
  0x75, 0x08,
  /* Count (Number of fields(bytes) in INPUT report) */
  0x95, OUTPUT_REPORT_SIZE,
  /* Output Report - type variable data */
  0x91, 0x02,
  
  /* End collection */
  0xC0
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
#define STRING_PRODUCT_SIZE 44
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
  'n', 0x00, 's', 0x00, 't', 0x00, 'r', 0x00,
  'a', 0x00, 't', 0x00, 'i', 0x00, 'o', 0x00,
  'n', 0x00
};

const DESCRIPTOR gStringDescriptorProduct =
{
  STRING_PRODUCT_SIZE,
  gStringDescriptorProductData  
};

/*Serial number string "1.1" */
#define STRING_SERIAL_NUM_SIZE 8
static const uint8_t gStringDescriptorSerialNumData[STRING_SERIAL_NUM_SIZE] = 
{
  /* Length of this descriptor*/
  STRING_SERIAL_NUM_SIZE,
  /* Descriptor Type = STRING */
  0x03,
  /* Descriptor Text (unicode) */
  '1', 0x00, '.', 0x00, '1', 0x00
};

const DESCRIPTOR gStringDescriptorSerialNum = 
{
  STRING_SERIAL_NUM_SIZE,
  gStringDescriptorSerialNumData  
};
