/***********************************************************************************
FILE NAME  		USBHID.h

DESCRIPTION		This is an implementation of the API defined by the
				abstract base class CUSBComms which is designed to
				communicate with a HID device.

				It requires the Windows DDK / WDK:
				Headers:  hidsdi.h, Setupapi.h, hidpi.h, hidusage.h and sal.h.		
				Libs: HID.Lib and SetupAPI.Lib.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_USBHID_H__9C78688D_B69D_4EB5_9874_3955878C3977__INCLUDED_)
#define AFX_USBHID_H__9C78688D_B69D_4EB5_9874_3955878C3977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*	NOTE: These headers are found in the Windows DDK
	You will need to add these to the list of include directories in Visual Studio.
	Tools->Options->Directories
	e.g "C:\WinDDK\6001.18001\inc\api"
	
	Also need to link to HID.Lib and SetupAPI.Lib from the DDK.
	This project includes the 'additional library path'
	"C:\WinDDK\6001.18001\lib\wxp\i386".
	You may need to change this for your version of DDK.
*/
extern "C"
{
	#include <sal.h> //Not required with older DDK versions.
	#include <hidsdi.h>
	#include <Setupapi.h>
}

#include "USBComms.h"

class CUSBHID : public CUSBComms
{
public:
	CUSBHID(CGUI* pGUI);
	virtual ~CUSBHID();

	bool Connect(USHORT VendorID, USHORT  ProductID);
	void Disconnect();

	DWORD Read(char* pReport, DWORD dwReportSize);

	DWORD Write(char* pReport, DWORD dwReportSize);

protected:

	bool CheckDetails(HDEVINFO hHDEVINFO, SP_DEVICE_INTERFACE_DATA* poDeviceInterface);
	void DisplayDeviceInfo();

	volatile bool m_bConnected;
	volatile BOOL m_bReadPending;

	HANDLE m_hDev;

	USHORT m_VendorID;
	USHORT  m_ProductID;

	OVERLAPPED m_oOverlappedRead;
	OVERLAPPED m_oOverlappedWrite;

	//*** RSK SAMPLE SPECIFIC ***
	//Flag to indicate that another RSK HID device has been detected.
	BOOL m_bOtherRSKHIDDetected;
	USHORT m_OtherRSKHIDDetectedPID;
	//*** END - RSK SAMPLE SPECIFIC ***
};

#endif // !defined(AFX_USBHID_H__9C78688D_B69D_4EB5_9874_3955878C3977__INCLUDED_)


