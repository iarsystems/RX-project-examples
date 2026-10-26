/***********************************************************************************
FILE NAME  		RSKUSB.h

DESCRIPTION		This is the link between the GUI and the actual
				raw data transfer. Hence this contains the knowledge
				of the structure of the messages used.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_RSKUSB_H__1DBC4696_B0B3_40F3_AF99_D4961A985A4C__INCLUDED_)
#define AFX_RSKUSB_H__1DBC4696_B0B3_40F3_AF99_D4961A985A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "USBHID.h"

class CGUI;

//Data transfer sizes.
#define IN_SIZE  5
#define OUT_SIZE 17

class CRSKUSB  
{
public:
	CRSKUSB(CGUI* pGUI);
	virtual ~CRSKUSB();

	void Connect(USHORT VendorID, USHORT  ProductID);
	void Disconnect();
	void ToggleLED();
	void ReadADC();
	void SetLCD(CString str);

protected:
	
	void Write(BOOL bLED, BOOL bADC, BOOL bLCD, CString* pStrLCD);
	static UINT ReadThreadStatic(LPVOID pParam);
	UINT ReadThread();


protected:
	CGUI* m_pGUI;
	CUSBComms* m_pUSBComms;
	CUSBHID m_oUSBHID;
	volatile BOOL m_bConnected;
	char m_INBuffer[IN_SIZE];
	char m_OUTBuffer[OUT_SIZE];

};

#endif // !defined(AFX_RSKUSB_H__1DBC4696_B0B3_40F3_AF99_D4961A985A4C__INCLUDED_)
