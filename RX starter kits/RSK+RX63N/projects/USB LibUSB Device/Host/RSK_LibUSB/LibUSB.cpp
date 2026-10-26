/***********************************************************************************
FILE NAME  		LibUSB.cpp
DESCRIPTION		This is an implementation of the API defined by the
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

#include "stdafx.h"
#include "LibUSB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//This is the error code that libusb returns if there has been a timeout
#define ETIMEDOUT (-116)

#define INVALID_EP_ADDRESS 0xFF

#define TIMEOUT_WRITE 1000 //1 second

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////// 

CLibUSB::CLibUSB(CGUI* pGUI):
CUSBComms(pGUI),
handle(NULL),
timeout(TIMEOUT_WRITE)
{
	//Endpoint addresses will be set in function FindEndpointAddresses on Connect.
	ep_read = INVALID_EP_ADDRESS;
	ep_write = INVALID_EP_ADDRESS;
	ep_read_interrupt = INVALID_EP_ADDRESS;
}

CLibUSB::~CLibUSB()
{
}

/**************************************************************************************************
Function Name:      Connect()

Description:        Trys to connect to the LibUSB device with the supplied VID and PID

Parameters:         VendorID
                    ProductID 

Return value:       'true' if a connection has been made.
**************************************************************************************************/
bool CLibUSB::Connect(USHORT VendorID, USHORT  ProductID)
{
	usb_init(); /* initialize the library */
	usb_find_busses(); /* find all busses */
	usb_find_devices(); /* find all connected devices */

	//Try and open a device with specified VID/PID
	handle = open_dev(VendorID, ProductID);
	if(NULL == handle)
    {
      m_pGUI->MessageMain("Device not found.");
      return FALSE;
    }

	//Device opened. Set Configuration.
	if(usb_set_configuration(handle, 1) < 0)
    {
	  m_pGUI->MessageMain("Function usb_set_configuration failed.");
      usb_close(handle);
      return FALSE;
    }

	/*Claim interface*/
	if(usb_claim_interface(handle, 0) < 0)
    {
      m_pGUI->MessageMain("Function usb_claim_interface failed.");
      usb_close(handle);
      return FALSE;
    }

	FindEndpointAddresses();

	return TRUE;
}


//
/**************************************************************************************************
Function Name:      FindEndpointAddresses()

Description:        Find out the endpoint addresses for BULK IN, BULK OUT and INTERRUPT IN(if supported)
					as these are required for LibUSB functions usb_bulk_read, usb_bulk_write etc.
					NOTE: This will use the first endpoint of a certain type found.
Parameters:         
Return value:       void
**************************************************************************************************/
void CLibUSB::FindEndpointAddresses()
{

	//Get dev from handle
	struct usb_device *dev = usb_device(handle);
	if(dev == NULL)
	{
		return;
	}

	struct usb_endpoint_descriptor* p_endpoint;
	int NumEndpoints = dev->config->interface->altsetting->bNumEndpoints;
	unsigned char bEndpointAddress;
	unsigned char bmAttributes;

	//For each endpoint
	for(int index = 0; index < NumEndpoints; index++)
	{
		//Get the Endpoint Descriptor
		p_endpoint = &dev->config->interface->altsetting->endpoint[index];
		bEndpointAddress = p_endpoint->bEndpointAddress;
		bmAttributes = p_endpoint->bmAttributes;

		//See what type of endpoint it is by examining bmAttributes and bEndpointAddress.
		//Is this a BULK endpoint (Check 'Transfer Type' bits, bits 1:0)
		if((bmAttributes & 0x03) == 0x02)
		{
			//IN or OUT (Check Direction bit, bit 7)
			if(bEndpointAddress & 0x80)
			{
				//Bulk IN
				//Only set variable if not already set so we are using first endpoints found.
				if(INVALID_EP_ADDRESS == ep_read)
				{
					ep_read = bEndpointAddress;
				}
			}
			else
			{
				//Bulk OUT
				//Only set variable if not already set so we are using first endpoints found.
				if(INVALID_EP_ADDRESS == ep_write)
				{
					ep_write = bEndpointAddress;
				}
			}
		}

		//Is this an Interrupt IN? (Check 'Transfer Type' bits, bits 1:0)
		if((bmAttributes & 0x03) == 0x03)
		{
			//IN? (Check Direction bit, bit 7)
			if(bEndpointAddress & 0x80)
			{
				//Interrupt IN
				//Only set variable if not already set so we are using first endpoints found.
				if(INVALID_EP_ADDRESS == ep_read_interrupt)
				{
					ep_read_interrupt = bEndpointAddress;
				}
			}
		}

	}

}


/****************************************************************************
Function Name:      Disconnect()
Description:        Closes the device handle.
Parameters:         -
Return value:       - 
*****************************************************************************/
void CLibUSB::Disconnect()
{
	usb_release_interface(handle, 0);
	usb_close(handle);
	handle = NULL;

	//Clear endpoint addresses
	ep_read = INVALID_EP_ADDRESS;
	ep_write = INVALID_EP_ADDRESS;
	ep_read_interrupt = INVALID_EP_ADDRESS;
}

/**************************************************************************************************
Function Name:      Read()

Description:        Blocks untill reads from the device or until timeout or error.
					Trys first Interrupt IN then Bulk IN.

Parameters:         pReport - Pointer to buffer
                    dwSize - Number of bytes to read.

Return value:       Windows Error Code:
					ERROR_SUCCESS, ERROR_TIMEOUT, ERROR_SUCCESS or ERROR_READ_FAULT
**************************************************************************************************/
DWORD CLibUSB::Read(char* pBuffer, DWORD dwSize)
{
	DWORD err;
	
	/*Try read form Interrupt IN */
	err = ReadInterrupt(pBuffer, dwSize);

	/*If not read from interrupt read from bulk.
	NOTE: 
	If the device doesn't actually support an interrupt IN pipe
	then an error is returned. As early versions of the device firmware
	didn't support Interrupt IN then ignore this error.*/
	if(ERROR_TIMEOUT == err || (ERROR_READ_FAULT == err))
	{
		/*Try read form Bulk IN */
		err = ReadBulk(pBuffer, dwSize);
	}
	//else don't try BULK if INTERRUPT has succeeded

	return err;

}


/**************************************************************************************************
Function Name:      ReadBulk()

Description:        Blocks untill reads from the device (Bulk IN) or until timeout or error.

Parameters:         pReport - Pointer to buffer
                    dwSize - Number of bytes to read.

Return value:       Windows Error Code:
					ERROR_SUCCESS, ERROR_TIMEOUT, ERROR_SUCCESS or ERROR_READ_FAULT
**************************************************************************************************/
DWORD CLibUSB::ReadBulk(char* pBuffer, DWORD dwSize)
{
	DWORD err;
	int error;

	//Check we know the endpoint address for bulk read.
	if(INVALID_EP_ADDRESS == ep_read)
	{
		m_pGUI->MessageOther("ERROR: Endpoint Address Not Known!");
	}

	error = usb_bulk_read(handle, ep_read, pBuffer, dwSize, 250);

	if((DWORD)error == dwSize)
	{
		err = ERROR_SUCCESS;

		CString str;
		CString str2;
		str.Format("BULK IN: Read %d bytes of data:", dwSize);
		m_pGUI->MessageOther(str);
		str.Empty();
		for(DWORD i = 0; i < dwSize; i++)
		{
			str2.Format(" h'%.02X ", 0xFF & (unsigned)pBuffer[i]);
			str += str2;
		}
		m_pGUI->MessageOther(str);
	}
	else if(error == ETIMEDOUT)
	{
		err = ERROR_TIMEOUT;
	}
	else
	{
		m_pGUI->MessageOther("Error reading data.");
		err = ERROR_READ_FAULT;
	}

	return err;

}

/**************************************************************************************************
Function Name:      ReadInterrupt()

Description:        Blocks untill reads from the device (Interrupt IN) or until timeout or error.

Parameters:         pReport - Pointer to buffer
                    dwSize - Number of bytes to read.

Return value:       Windows Error Code:
					ERROR_SUCCESS, ERROR_TIMEOUT, ERROR_SUCCESS or ERROR_READ_FAULT
**************************************************************************************************/
DWORD CLibUSB::ReadInterrupt(char* pBuffer, DWORD dwSize)
{
	DWORD err;
	int error;

	//Check we know the endpoint address for interrupt IN.
	if(INVALID_EP_ADDRESS == ep_read_interrupt)
	{
		m_pGUI->MessageOther("ERROR: Endpoint Address Not Known!");
	}

	error = usb_interrupt_read(handle, ep_read_interrupt, pBuffer, dwSize, 250);

	if((DWORD)error == dwSize)
	{
		err = ERROR_SUCCESS;

		CString str;
		CString str2;
		str.Format("\r\nINTERRUPT IN: Read %d bytes of data:", dwSize);
		m_pGUI->MessageOther(str);
		str.Empty();
		for(DWORD i = 0; i < dwSize; i++)
		{
			str2.Format(" h'%.02X ", 0xFF & (unsigned)pBuffer[i]);
			str += str2;
		}
		m_pGUI->MessageOther(str);
	}
	else if(error == ETIMEDOUT)
	{
		err = ERROR_TIMEOUT;
	}
	else
	{
		m_pGUI->MessageOther("Error reading data.");
		err = ERROR_READ_FAULT;
	}

	return err;
}

/**************************************************************************************************
Function Name:      Write()

Description:        Blocks untill writes to the device (Bulk OUT) or until timeout or error.

Parameters:         pBuffer - Pointer to buffer
                    dwSize - Number of bytes to write.

Return value:       Windows Error Code:
					ERROR_SUCCESS or ERROR_WRITE_FAULT
**************************************************************************************************/
DWORD CLibUSB::Write(char* pBuffer, DWORD dwSize)
{
	DWORD err;
	int error;

	//Message to GUI saying what we will try and send.
	CString str;
	CString str2;
	str.Format("BULK OUT: Sending %d bytes of data:", dwSize);
	m_pGUI->MessageOther(str);
	str.Empty();
	for(DWORD i = 0; i < dwSize; i++)
	{
		str2.Format(" h'%.02X ", 0xFF & (unsigned)pBuffer[i]);
		str += str2;
	}
	m_pGUI->MessageOther(str);

	//Check we know the endpoint address for bulk write.
	if(INVALID_EP_ADDRESS == ep_write)
	{
		m_pGUI->MessageOther("ERROR: Endpoint Address Not Known!");
	}

	error = usb_bulk_write(handle, ep_write, pBuffer, dwSize, timeout);
	
	if((DWORD)error == dwSize)
	{
		err = ERROR_SUCCESS;
	}
	else
	{
		err = ERROR_WRITE_FAULT;
		m_pGUI->MessageOther("Error sending data!");
	}

	return err;
}

/**************************************************************************************************
Function Name:      open_dev

Description:        Searches through all attached devices for one that matches the specified VID/PID.
					If found then opens it and writes it's descriptors out to the GUI.

Parameters:         MY_VID - VID required.
                    MY_PID - PID required.

Return value:       NULL if not found otherwise handle to device.
**************************************************************************************************/
usb_dev_handle* CLibUSB::open_dev(USHORT MY_VID, USHORT  MY_PID)
{
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle* handle = NULL;

  //For each bus ofund
  for(bus = usb_get_busses(); bus; bus = bus->next) 
  {
	  //For each device
      for(dev = bus->devices; dev; dev = dev->next) 
      {
		  //Does this have the VID/PID we are looking for?
          if(dev->descriptor.idVendor == MY_VID
             && dev->descriptor.idProduct == MY_PID)
          {
			  //Found device. Try and open it.
              handle = usb_open(dev);
			  if(NULL != handle)
			  {
				  m_pGUI->MessageMain("Device Opened.");
				  //Have found the device and opened it.
				  DisplayDeviceInfo(handle, dev);
			  }
          }
      }
  }

  return handle;
}

/**************************************************************************************************
Function Name:      DisplayDeviceInfo

Description:        Writes the descriptors of the specified device out to the GUI.

Parameters:         udev: Device handle.
					dev:  Device information structure.
Return value:		-
**************************************************************************************************/
void CLibUSB::DisplayDeviceInfo(usb_dev_handle* udev, struct usb_device *dev)
{
	char string[256];
	int ret, i;
  
	//Manufacturer String
	CString strMaunfacturer = "Manufacturer: Unknown";
	if(dev->descriptor.iManufacturer)
	{
		ret = usb_get_string_simple(udev, dev->descriptor.iManufacturer, string,
								  sizeof(string));
		if(ret > 0)
		{
			strMaunfacturer.Format("Manufacturer: \"%s\"", string);
		}
	}
	m_pGUI->MessageMain(strMaunfacturer);

	  
	//Product String
	CString strProduct = "Product: Unknown";
    if(dev->descriptor.iProduct)
	{
		ret = usb_get_string_simple(udev, dev->descriptor.iProduct, string, 
								  sizeof(string));
		if(ret > 0)
		{
			strProduct.Format("Product: \"%s\"", string);
		}
	}
	m_pGUI->MessageMain(strProduct);

	//Serial String
	CString strSerial = "Serial Number: Unknown";
    if(dev->descriptor.iProduct)
	{
		ret = usb_get_string_simple(udev, dev->descriptor.iSerialNumber, string, 
								  sizeof(string));
		if(ret > 0)
		{
			strSerial.Format("Serial: \"%s\"", string);
		}
	}
	m_pGUI->MessageMain(strSerial);


	//Device Descriptor
	CString str;
	CString strDevice;

	m_pGUI->MessageMain("\nDevice Descriptor:-");

	str.Format("      bLength:            %d\n", dev->descriptor.bLength);
	strDevice += str;
	str.Format("      bDescriptorType:    %d\n", dev->descriptor.bDescriptorType);
	strDevice += str;
	str.Format("      bcdUSB:             %02Xh\n", dev->descriptor.bcdUSB);
	strDevice += str;
	str.Format("      bDeviceClass:       %d\n", dev->descriptor.bDeviceClass);
	strDevice += str;
	str.Format("      bDeviceSubClass:    %d\n", dev->descriptor.bDeviceSubClass);
	strDevice += str;
	str.Format("      bDeviceProtocol:    %d\n", dev->descriptor.bDeviceProtocol);
	strDevice += str;
	str.Format("      bMaxPacketSize0:    %d\n", dev->descriptor.bMaxPacketSize0);
	strDevice += str;
	str.Format("      idVendor:           %02Xh\n", dev->descriptor.idVendor);
	strDevice += str;
	str.Format("      idProduct:          %02Xh\n", dev->descriptor.idProduct);
	strDevice += str;
	str.Format("      bcdDevice:          %d\n", dev->descriptor.bcdDevice);
	strDevice += str;
	str.Format("      iManufacturer:      %d\n", dev->descriptor.iManufacturer);
	strDevice += str;
	str.Format("      iProduct:           %d\n", dev->descriptor.iProduct);
	strDevice += str;
	str.Format("      iSerialNumber:      %d\n", dev->descriptor.iSerialNumber);
	strDevice += str;
	str.Format("      bNumConfigurations: %d\n", dev->descriptor.bNumConfigurations);
	strDevice += str;

	m_pGUI->MessageMain(strDevice);

    
	//For each configuration
    for(i = 0; i < dev->descriptor.bNumConfigurations; i++)
	{
		print_configuration(&dev->config[i]);
	} 
}

/**************************************************************************************************
Function Name:      print_endpoint

Description:        Write supplied endpoint descriptor information out to the GUI.
					Note. This function is based on libusb sample code.

Parameters:			endpoint: Endpoint information.
Return value:		-
**************************************************************************************************/
void CLibUSB::print_endpoint(struct usb_endpoint_descriptor *endpoint)
{
	CString str;
	CString strEndpoint;

	m_pGUI->MessageMain("Endpoint Descriptor:-");

	str.Format("      bEndpointAddress: %02Xh\n", endpoint->bEndpointAddress);
	strEndpoint += str;
	str.Format("      bmAttributes:     %02Xh\n", endpoint->bmAttributes);
	strEndpoint += str;
	str.Format("      wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
	strEndpoint += str;
	str.Format("      bInterval:        %d\n", endpoint->bInterval);
	strEndpoint += str;
	str.Format("      bRefresh:         %d\n", endpoint->bRefresh);
	strEndpoint += str;
	str.Format("      bSynchAddress:    %d\n", endpoint->bSynchAddress);
	strEndpoint += str;

	m_pGUI->MessageMain(strEndpoint);
}

/**************************************************************************************************
Function Name:      print_altsetting

Description:        Write supplied descriptor information out to the GUI.
					Note. This function is based on libusb sample code.

Parameters:			iFaceDes: Interface information.
Return value:		-
**************************************************************************************************/
void CLibUSB::print_altsetting(struct usb_interface_descriptor *iFaceDes)
{
	int i;

	CString str;
	CString strAltSetting;

	m_pGUI->MessageMain("Interface Descriptor:-");

	str.Format("      bInterfaceNumber:   %d\n", iFaceDes->bInterfaceNumber);
	strAltSetting += str;
	str.Format("      bAlternateSetting:  %d\n", iFaceDes->bAlternateSetting);
	strAltSetting += str;
	str.Format("      bNumEndpoints:      %d\n", iFaceDes->bNumEndpoints);
	strAltSetting += str;
	str.Format("      bInterfaceClass:    %d\n", iFaceDes->bInterfaceClass);
	strAltSetting += str;
	str.Format("      bInterfaceSubClass: %d\n", iFaceDes->bInterfaceSubClass);
	strAltSetting += str;
	str.Format("      bInterfaceProtocol: %d\n", iFaceDes->bInterfaceProtocol);
	strAltSetting += str;
	str.Format("      iInterface:         %d\n", iFaceDes->iInterface);
	strAltSetting += str;

	m_pGUI->MessageMain(strAltSetting);

	//For each endpoint....
	for (i = 0; i < iFaceDes->bNumEndpoints; i++)
	{
		print_endpoint(&iFaceDes->endpoint[i]);
	}
}

/**************************************************************************************************
Function Name:      print_interface

Description:        Write supplied descriptor information out to the GUI.
					Note. This function is based on libusb sample code.

Parameters:			iFace: Interface information.
Return value:		-
**************************************************************************************************/
void CLibUSB::print_interface(struct usb_interface *iFace)
{
	int i;

	//For each allernative setting...
	for (i = 0; i < iFace->num_altsetting; i++)
	{
		print_altsetting(&iFace->altsetting[i]);
	}
}

/**************************************************************************************************
Function Name:      print_configuration

Description:        Write supplied descriptor information out to the GUI.
					Note. This function is based on libusb sample code.

Parameters:			config: Configuration information.
Return value:		-
**************************************************************************************************/
void CLibUSB::print_configuration(struct usb_config_descriptor *config)
{
	CString str;
	CString strConfig;

	m_pGUI->MessageMain("Configuration Descriptor:-");

	int i;
	str.Format("      wTotalLength:         %d\n", config->wTotalLength);
	strConfig += str;
	str.Format("      bNumInterfaces:       %d\n", config->bNumInterfaces);
	strConfig += str;
	str.Format("      bConfigurationValue:  %d\n", config->bConfigurationValue);
	strConfig += str;
	str.Format("      iConfiguration:       %d\n", config->iConfiguration);
	strConfig += str;
	str.Format("      bmAttributes:         %02Xh\n", config->bmAttributes);
	strConfig += str;
	str.Format("      MaxPower:             %d\n", config->MaxPower);

	m_pGUI->MessageMain(strConfig);

	//For each interface...
	for (i = 0; i < config->bNumInterfaces; i++)
	{
		print_interface(&config->interface[i]);
	}
}