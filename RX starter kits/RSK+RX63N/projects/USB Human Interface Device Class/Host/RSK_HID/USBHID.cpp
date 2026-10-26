/***********************************************************************************
FILE NAME  		USBHID.cpp

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

#include "stdafx.h"
#include "USBHID.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Max number of USB devices to search for looking for a matching VID/PID
#define MAX_USB_DEVICE_INDEX 10

//Max size of the string that the HID access functions can return.
#define MAX_UNICODE_STRING_LENGTH 255

//Utility Functon for converting UNICODE to ASCII.
static VOID ConvertUNI2ASC(PCHAR sz);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**************************************************************************************************
Function Name:      CUSBHID()

Description:        Constructor
					Create the events used to provide independant overlapped IO for read and write.

Parameters:         CGUI*: Pointer to the GUI so this can send messages to the GUI.

Return value:       - 
**************************************************************************************************/
CUSBHID::CUSBHID(CGUI* pGUI):
CUSBComms(pGUI),
m_bConnected(false),
m_VendorID(0),
m_ProductID(0),
m_hDev(INVALID_HANDLE_VALUE),
m_bReadPending(FALSE)
{
	//Create event for asynchronous Reads.
	m_oOverlappedRead.Offset = 0; // Important
	m_oOverlappedRead.OffsetHigh = 0; // Important
	m_oOverlappedRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	//Create event for asynchronous Writes.
	m_oOverlappedWrite.Offset = 0; // Important
	m_oOverlappedWrite.OffsetHigh = 0; // Important
	m_oOverlappedWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}



/**************************************************************************************************
Function Name:      ~CUSBHID()

Description:		Destructor.
					If connected to a device then disconnect.   

Parameters:         N/A

Return value:       - 
**************************************************************************************************/
CUSBHID::~CUSBHID()
{
	if(true == m_bConnected)
	{
		Disconnect();
	}
}

/**************************************************************************************************
Function Name:      Connect()

Description:        Trys to connect to a HID USB device with the supplied VID and PID

Parameters:         VendorID
                    ProductID 

Return value:       'true' if a connection has been made.
**************************************************************************************************/
bool CUSBHID::Connect(USHORT VendorID, USHORT  ProductID)
{
	GUID oGUID_HID;
	HDEVINFO hHDEVINFO;

	//In-case already connected to another device.
	Disconnect();

	//Copy required VID/PID
	m_VendorID = VendorID;
	m_ProductID = ProductID;

	//*** START - RSK SAMPLE SPECIFIC ***
	//Reset flag
	m_bOtherRSKHIDDetected = FALSE;
	//*** END - RSK SAMPLE SPECIFIC ***

	//Get HID GUID for use in SetupDiGetClassDevs
	HidD_GetHidGuid(&oGUID_HID);

	//Retrieve a device information set that contains all devices of class HID
	hHDEVINFO = SetupDiGetClassDevs(&oGUID_HID, NULL, AfxGetMainWnd()->GetSafeHwnd(),
						DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);


	//For every device connected find its VID and PID
	SP_DEVICE_INTERFACE_DATA  oDeviceInterface;
	oDeviceInterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	int iDeviceIndex = 0;
	while(!m_bConnected && (iDeviceIndex <= MAX_USB_DEVICE_INDEX))
	{
		
		//Retrieve a context structure for a device interface of a device information set
		if(SetupDiEnumDeviceInterfaces(hHDEVINFO, NULL,
								&oGUID_HID,
								iDeviceIndex,
								&oDeviceInterface))
		{
			if(CheckDetails(hHDEVINFO, &oDeviceInterface))
			{
				//Have found the HID device matching the VID/PID
				m_bConnected = true;
				break;
			}
		}
		else
		{
			if(ERROR_NO_MORE_ITEMS == GetLastError())
			{
				//Have not found a HID device with the requested VID/PID
				break;
			}
		}
								

		iDeviceIndex++;
	}

	//Destroy the device information set. 
	SetupDiDestroyDeviceInfoList(hHDEVINFO);


	//*** START - RSK SAMPLE SPECIFIC ***
	//If not connected and the m_bOtherRSKHIDDetected flag is set then
	//tell user the possible reason for them not connecting is that the RSK
	//is running the wrong sample.
	if(TRUE == m_bOtherRSKHIDDetected)
	{
		CString str, strMessage;
		str = "Unable to connect to device:\n\n";
		strMessage += str;
		str.Format("An RSK with the specified PID (h'%04X) has not been found.\n",
					ProductID);
		strMessage += str;
		str.Format("However, an RSK with a different PID (h'%04X) has been detected.\n\n",
					m_OtherRSKHIDDetectedPID);
		strMessage += str;
		str = "If this is the RSK you intended to connect to then you need to either\n";
		strMessage += str;
		str = "change the application running on it or specify a different PID when trying again.\n";
		strMessage += str;
		AfxGetMainWnd()->MessageBox(strMessage, "RSK HID. Error Connecting.", MB_ICONINFORMATION);
	}
	//*** END - RSK SAMPLE SPECIFIC ***

	return m_bConnected;
}


/****************************************************************************
Function Name:      Disconnect()
Description:        Closes the device handle.
Parameters:         -
Return value:       - 
*****************************************************************************/
void CUSBHID::Disconnect()
{
	m_bConnected = false;

	/*Now wait for the pending read to complete before we close the file handle*/
	while(TRUE == m_bReadPending)

	m_VendorID = 0;
	m_ProductID = 0;

	if(m_hDev != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDev);
		m_hDev = INVALID_HANDLE_VALUE;
	}
}

/**************************************************************************************************
Function Name:      Read()

Description:        Blocks untill reads an Input Report or an error occours or
					a disconnection happens (m_bConnected is set FALSE).

					NOTE: The size of the report must match the size of the report that
					the HID device supports..

Parameters:         pReport - Pointer to Report buffer
                    dwReportSize - Size of report to read.

Return value:       Windows Error Code - 0 is OK
**************************************************************************************************/
DWORD CUSBHID::Read(char* pReport, DWORD dwReportSize)
{
	DWORD dwNumRead;
	DWORD dwError;

	m_bReadPending = TRUE;

	/*When using HID the first byte in the buffer returned by ReadFile will
	be an ID byte, the actual report will follow.
	This ID byte is not sent from the device.
	The user of this API doesn't need to know this so use a temporary buffer to 
	use in ReadFile and then just copy the report data into the supplied buffer.
	*/
	DWORD dwAdjustedSize = dwReportSize+1;

	char* pTempBuffer = new char[dwAdjustedSize]; 

	if(0 == ReadFile(m_hDev, pTempBuffer, dwAdjustedSize, &dwNumRead, &m_oOverlappedRead))
	{
		dwError = GetLastError();

		if(ERROR_IO_PENDING == dwError)
		{
			//Wait for operation to complete or fail
			while(!HasOverlappedIoCompleted(&m_oOverlappedRead) && m_bConnected)
			{
				//Sleep to limit CPU usage
				Sleep(10);
			}

			//If we have disconnected then cancel the pending IO
			if(FALSE == m_bConnected)
			{
				CancelIo(m_hDev);
				dwError = ERROR_OPERATION_ABORTED;
			}
			else
			{
				//Find out how many bytes have been read
				if(GetOverlappedResult(m_hDev, &m_oOverlappedRead, &dwNumRead, TRUE))
				{
					if(dwNumRead == dwAdjustedSize)
					{
						dwError = ERROR_SUCCESS;
					}
					else
					{
						dwError = ERROR_READ_FAULT;
					}
				}
				else
				{
					//GetOverlappedResult failed
					dwError = GetLastError();
				}
			}
		}
	}
	else
	{
		//ReadFile returned with success straight away i.e not using asynchronous operation.
		//Check number read is as expected.
		if(dwNumRead == dwAdjustedSize)
		{
			dwError = ERROR_SUCCESS;
		}
		else
		{
			dwError = ERROR_READ_FAULT;
		}
	}

	//If data was read OK.
	if(ERROR_SUCCESS == dwError)
	{
		//Copy the actual report data from the temp buffer 
		//to the supplied buffer
		memcpy(pReport, &pTempBuffer[1], dwReportSize);

		//Tell GUI the data read
		//A sleep here allows any Write operation that may have caused this read
		//to finish updating the GUI and therefore it is easier for the user to read.
		Sleep(100);
		CString str;
		CString str2;
		str.Format("Read %d bytes of data:", dwReportSize);
		m_pGUI->MessageOther(str);
		str.Empty();
		for(DWORD i = 0; i < dwReportSize; i++)
		{
			str2.Format(" h'%.02X ", 0xFF & (unsigned)pReport[i]);
			str += str2;
		}
		m_pGUI->MessageOther(str);
	}
	else
	{
		//Write error message unless this read failure is because of a disconnection
		if(TRUE == m_bConnected)
		{
			m_pGUI->MessageOther("Error reading data.");
		}
	}

	//Cleanup
	delete[] pTempBuffer;


	m_bReadPending = FALSE;
	return dwError;
}

/**************************************************************************************************
Function Name:      Write()

Description:        Blocks untill the supplied Output Report has been written.
					NOTE: The size of the report must match the size of the report that
					the HID device supports.

Parameters:         unsigned char* pReport - Pointer to Report
                    DWORD dwReportSize - Size of the report

Return value:       Windows Error Code - 0 is OK
**************************************************************************************************/
DWORD CUSBHID::Write(char* pReport, DWORD dwReportSize)
{
	DWORD dwNumWrote;
	DWORD dwError = ERROR_SUCCESS;


	//Message to GUI saying what we will try and send.
	CString str;
	CString str2;
	str.Format("Sent %d bytes of data:", dwReportSize);
	m_pGUI->MessageOther(str);
	str.Empty();
	for(DWORD i = 0; i < dwReportSize; i++)
	{
		str2.Format(" h'%.02X ", 0xFF & (unsigned)pReport[i]);
		str += str2;
	}
	m_pGUI->MessageOther(str);


	/*When using HID an ID byte must be sent as the first byte in the buffer
	to WriteFile. If the default ID of 0 is sent this will not actually be sent to the
	device. This is the behaviour we want. The user of this API doesn't need to know
	this so need to add the ID BYTE at this stage.
	*/
	char* pTempBuffer = new char[dwReportSize+1];
	pTempBuffer[0] = 0; //Default ID
	//Copy report to new buffer straight after the ID byte.
	memcpy(&pTempBuffer[1], pReport, dwReportSize);

	if(0 == WriteFile(m_hDev, pTempBuffer, dwReportSize+1, &dwNumWrote, &m_oOverlappedWrite))
	{
		dwError = GetLastError();

		if(ERROR_IO_PENDING == dwError)
		{
			//Wait for operation to complete or fail
			while(!HasOverlappedIoCompleted(&m_oOverlappedWrite) && m_bConnected)
			{
				//Sleep to avoid limit CPU usage
				Sleep(10);
			}

			//Find out how many bytes have been written
			if(GetOverlappedResult(m_hDev, &m_oOverlappedWrite, &dwNumWrote, TRUE))
			{
				if(dwNumWrote == (dwReportSize+1))
				{
					dwError = ERROR_SUCCESS;
				}
				else
				{
					dwError = ERROR_WRITE_FAULT;
				}
			}
			else
			{
				//GetOverlappedResult failed
				dwError = GetLastError();
			}
		}
	}
	else
	{
		//WriteFile returned with success straight away i.e not using asynchronous operation.
		//Check number wrote is as expected.
		if(dwNumWrote == dwReportSize+1)
		{
			dwError = ERROR_SUCCESS;
		}
		else
		{
			dwError = ERROR_READ_FAULT;
		}
	}

	//If the data was sent OK.
	if(ERROR_SUCCESS != dwError)
	{
		m_pGUI->MessageOther("Error sending data!");
	}

	//Cleanup
	delete[] pTempBuffer;

	return dwError;
}

/**************************************************************************************************
Function Name:      CheckDetails()

Description:        Check VID/PID is what we are looking for.
					If correct leave open with m_hDev set.

Parameters:         HDEVINFO hHDEVINFO - Device Infomation Set.
                    SP_DEVICE_INTERFACE_DATA* poDeviceInterface - Context structure for a
																	device interface.

Return value:       True if this matches the VID/PID otherwise false.
**************************************************************************************************/
bool CUSBHID::CheckDetails(HDEVINFO hHDEVINFO, SP_DEVICE_INTERFACE_DATA* poDeviceInterface)
{
	bool bReturn = false;
	DWORD dwRequiredSize;
	
	HIDD_ATTRIBUTES oHIDAttributes;
	SP_DEVINFO_DATA oDeviceData; // Result Not used
	oDeviceData.cbSize = sizeof(SP_DEVINFO_DATA);

	//Retrieve detailed information about a specified device interface
	// First call to get required size of SP_DEVICE_INTERFACE_DETAIL_DATA object. 
	SetupDiGetDeviceInterfaceDetail(hHDEVINFO,
									poDeviceInterface,
									NULL, // So will fail but we will get required size
									0, // So will fail but we will get required size
									&dwRequiredSize,
									&oDeviceData);

	DWORD dwError = GetLastError();

	// Use Required size
	if(dwError == ERROR_INSUFFICIENT_BUFFER)
	{
		//Allocate buffer from the size we found out above
		PSP_DEVICE_INTERFACE_DETAIL_DATA poDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)new\
														TCHAR[dwRequiredSize+sizeof(DWORD)];

		poDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		if(SetupDiGetDeviceInterfaceDetail(hHDEVINFO,
									poDeviceInterface,
									poDetail,
									dwRequiredSize,
									&dwRequiredSize,
									&oDeviceData))
		{
			DWORD dwDesiredAccess =  GENERIC_WRITE|GENERIC_READ;
			DWORD dwShareMode = 0; // No sharing
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
			DWORD dwCreationDisposition = OPEN_EXISTING;
			DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED; // Asynchronous IO
			
			m_hDev = CreateFile(poDetail->DevicePath,
							dwDesiredAccess,
							dwShareMode,
							lpSecurityAttributes,
							dwCreationDisposition,
							dwFlagsAndAttributes,
							NULL);

			if(m_hDev != INVALID_HANDLE_VALUE)
			{
				//Get the VID/PID
				if(HidD_GetAttributes(m_hDev, &oHIDAttributes))
				{
					if((oHIDAttributes.VendorID == m_VendorID) &&
						(oHIDAttributes.ProductID == m_ProductID))
					{
						//This device has the VID/PID requested.
						m_pGUI->MessageMain("Device Opened.");
						//Write out device info to the GUI
						DisplayDeviceInfo();

						bReturn = true;
					}
				}
			}
		}

		//Cleanup
		delete [] poDetail;
	}

	//If this wasn't the device we are looking for then close the handle to it
	if((FALSE == bReturn) && (m_hDev != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hDev);

		//*** START - RSK SAMPLE SPECIFIC ***
		#define RENESAS_VID	0x045B //Renesas VID
		#define RENESAS_PID	0x2013 //PID - "RSK Generic HID Application"
		#define RENESAS_RSK_PID_START 0x2000
		#define RENESAS_RSK_PID_END	  0x20FF	
		//If trying to open the Renesas RSK Generic HID Application and another
		//RSK HID device is found then set flag.
		if((RENESAS_VID == m_VendorID) && (RENESAS_PID == m_ProductID))
		{
			//Have we found a Renesas device?
			if(RENESAS_VID == oHIDAttributes.VendorID)
			{
				//Is it an RSK
				if((oHIDAttributes.ProductID >= RENESAS_RSK_PID_START) &&
					oHIDAttributes.ProductID <= RENESAS_RSK_PID_END)
				{
					m_bOtherRSKHIDDetected = TRUE;
					m_OtherRSKHIDDetectedPID = oHIDAttributes.ProductID;
				}
			}
		}
		//*** END - RSK SAMPLE SPECIFIC ***
	}


	return bReturn;
}

/**************************************************************************************************
Function Name:      DisplayDeviceInfo
Description:        Write details about the currently opened device (m_hDev)
					to the GUI.

Parameters:         -
Return value:       -
**************************************************************************************************/
void CUSBHID::DisplayDeviceInfo()
{
	//Buffer to receive strings.
	char sz[MAX_UNICODE_STRING_LENGTH];

	//Manufacturer string
	CString strMaunfacturer = "Manufacturer: Unknown";
	if(TRUE == HidD_GetManufacturerString(m_hDev, sz, MAX_UNICODE_STRING_LENGTH))
	{
		//If a unicode string has been returned then convert to 
		//ASCII for writing to GUI.
		if(sz[1] == '\0')
		{
			//Convert from unicode to ASCII.
			ConvertUNI2ASC(sz);
		}
		strMaunfacturer.Format("Manufacturer: \"%s\"", sz);
	}
	m_pGUI->MessageMain(strMaunfacturer);

	//Product string
	CString strProduct = "Product: Unknown";
	if(TRUE == HidD_GetProductString(m_hDev, sz, MAX_UNICODE_STRING_LENGTH))
	{
		//If a unicode string has been returned then convert to 
		//ASCII for writing to GUI.
		if(sz[1] == '\0')
		{
			//Convert from unicode to ASCII.
			ConvertUNI2ASC(sz);
		}
		strProduct.Format("Product: \"%s\"", sz);
	}
	m_pGUI->MessageMain(strProduct);

	//Serial String
	CString strSerial = "Serial Number: Unknown";
    if(TRUE == HidD_GetSerialNumberString(m_hDev, sz, MAX_UNICODE_STRING_LENGTH))
	{
		//If a unicode string has been returned then convert to 
		//ASCII for writing to GUI.
		if(sz[1] == '\0')
		{
			//Convert from unicode to ASCII.
			ConvertUNI2ASC(sz);
		}
		strSerial.Format("Serial: \"%s\"", sz);
	}
	m_pGUI->MessageMain(strSerial);
}

/**************************************************************************************************
Function Name:      UNI2ASC
Description:        Convert a UNICODE string to an ASCII string.
Parameters:         sz: [IN] UNICODE string.
						[OUT] ASCII string.
					
Return value:       -
**************************************************************************************************/
VOID ConvertUNI2ASC(PCHAR sz)
{	
	int i;
	for(i=0; i<MAX_UNICODE_STRING_LENGTH; i+=2)
	{
		sz[i/2] = sz[i];
		if(sz[i]==0)
		{
			return;
		}
	}
}
