/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME      LibUSB.h
DESCRIPTION    This is an implementation of the API defined by the
        abstract base class CUSBComms which is designed to
        communicate with a LibUSB device.
        
        It requires from LibUSB - Win32:-
        Header: usb.h
        Lib:    libusb.lib

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_LIBUSB_H__E5767DA1_3723_42AF_ADD4_37A163761A29__INCLUDED_)
#define AFX_LIBUSB_H__E5767DA1_3723_42AF_ADD4_37A163761A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "usb.h" //LibUSB file. Set Visual studio to include the libusb include path.
#include "USBComms.h"

class CLibUSB : public CUSBComms  
{
public:
  CLibUSB(CGUI* pGUI);
  virtual ~CLibUSB();

  bool Connect(USHORT VendorID, USHORT  ProductID);
  void Disconnect();

  DWORD Read(char* pBuffer, DWORD dwSize);
  DWORD Write(char* pBuffer, DWORD dwSize);

protected: //Methods

  void FindEndpointAddresses();

  DWORD ReadBulk(char* pBuffer, DWORD dwSize);
  DWORD ReadInterrupt(char* pBuffer, DWORD dwSize);

  usb_dev_handle *open_dev(USHORT MY_VID, USHORT  MY_PID);
  void DisplayDeviceInfo(usb_dev_handle* udev, struct usb_device *dev);

  /*The following functions were adapted from libusb supplied test code*/
  void print_endpoint(struct usb_endpoint_descriptor *endpoint);
  void print_altsetting(struct usb_interface_descriptor *iFaceDes);
  void print_interface(struct usb_interface *iFace);
  void print_configuration(struct usb_config_descriptor *config);

protected: //Data

  usb_dev_handle* handle; /* the device handle */
  int ep_read;
  int ep_write;
  int ep_read_interrupt;
  int timeout;
};

#endif // !defined(AFX_LIBUSB_H__E5767DA1_3723_42AF_ADD4_37A163761A29__INCLUDED_)
