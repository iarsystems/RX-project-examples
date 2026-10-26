/***********************************************************************************
FILE NAME  		USBComms.h

DESCRIPTION		Abstract base class providing an API for communication
				between host and device.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_USBCOMMS_H__0651BE45_4DC9_40AB_B5D7_BD9C0828A4FA__INCLUDED_)
#define AFX_USBCOMMS_H__0651BE45_4DC9_40AB_B5D7_BD9C0828A4FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GUI.h"

class CUSBComms  
{
public:
	CUSBComms(CGUI* m_pGUI);
	virtual ~CUSBComms();

	virtual bool Connect(USHORT VendorID, USHORT  ProductID) = 0;
	virtual void Disconnect() = 0;

	virtual DWORD Read(char* pBuffer, DWORD dwSize) = 0;

	virtual DWORD Write(char* pBuffer, DWORD dwSize) = 0;

protected:
	//Allows the implementation of this to be able to access the GUI
	//for message output.
	CGUI* m_pGUI;
};

#endif // !defined(AFX_USBCOMMS_H__0651BE45_4DC9_40AB_B5D7_BD9C0828A4FA__INCLUDED_)
