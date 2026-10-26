/***********************************************************************************
FILE NAME  		RSKUSB.cpp

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

#include "stdafx.h"
#include "RSKUSB.h"
#include "GUI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Number of characters that LCD has.
#define LCD_SIZE 16 //2*8

/* FORMAT OF DATA TRANSFERED BETWEEN HOST AND DEVICE*/
/*
OUT
The host sends (OUT transfer) 17 bytes. (OUT_SIZE)
The first byte is a cmd byte the following 16 are the 16 ASCII LCD characters values.
The bits in the cmd byte are defined below:
*/
#define OUT_BIT_LED 0x01 //Toggle LED
#define OUT_BIT_ADC 0x02 //Read ADC
#define OUT_BIT_LCD 0x04 //Update LCD
/*
IN
The device sends (IN transfer) 5 bytes. (IN_SIZE)
The first byte contains information as defined by the bits below.
The remaining 4 bytes (may) hold a 32 bit ADC value in little endian. 
*/
#define IN_BIT_LED	0x01 //LED state (1 = ON, 0 = OFF)
#define IN_BIT_ADC	0x02 //ADC data is valid
#define IN_BIT_SW	0x04 //A switch has been pressed.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRSKUSB::CRSKUSB(CGUI* pGUI):
m_oUSBHID(pGUI),
m_pUSBComms(&m_oUSBHID),
m_bConnected(FALSE),
//Store GUI but don't use here so that GUI can construct this in its own constructor.
m_pGUI(pGUI) 
{
	//Initialise Buffers to zeros
	memset(m_OUTBuffer, 0, OUT_SIZE);
	memset(m_INBuffer, 0, IN_SIZE);
}

CRSKUSB::~CRSKUSB()
{
}

//Trys to connect to a device with the given VID and PID.
//If a connection is made then a pending read will be set up in a worker thread.
void CRSKUSB::Connect(USHORT VendorID, USHORT  ProductID)
{
	if(FALSE == m_bConnected)
	{
		if(m_pUSBComms->Connect(VendorID, ProductID))
		{
			m_bConnected = TRUE;

			//Inform the GUI
			m_pGUI->Connected(TRUE);

			//Start Read Thread
			AfxBeginThread(ReadThreadStatic, this);
		}
		else
		{
			m_pGUI->MessageMain("Failed to connect.");
		}
	}
}

//If connected this will disconenct and inform the GUI
void CRSKUSB::Disconnect()
{
	if(TRUE == m_bConnected)
	{
		m_bConnected = FALSE;

		m_pUSBComms->Disconnect();

		//Inform the GUI
		m_pGUI->Connected(FALSE);	
	}
}

//Send a command to the device to toggle an LED
void CRSKUSB::ToggleLED()
{
	Write(TRUE, FALSE, FALSE, NULL);
}

//Send a command to the device to read the ADC and return the result
void CRSKUSB::ReadADC()
{
	Write(FALSE, TRUE, FALSE, NULL);
}

//Send a command to the device to set the LCD with the supplied string
void CRSKUSB::SetLCD(CString str)
{
	ASSERT(LCD_SIZE == str.GetLength());
	Write(FALSE, FALSE, TRUE, &str);
}

//Puts together a buffer based on the supplied parameters
//and sends it to the device.
void CRSKUSB::Write(BOOL bLED, BOOL bADC, BOOL bLCD, CString* pStrLCD)
{
	if(TRUE == m_bConnected)
	{
		m_OUTBuffer[0] = 0;
		if(TRUE == bLED)
		{
			m_OUTBuffer[0] |= OUT_BIT_LED;
		}
		if(TRUE == bADC)
		{
			m_OUTBuffer[0] |= OUT_BIT_ADC;
		}
		if(TRUE == bLCD)
		{
			m_OUTBuffer[0] |= OUT_BIT_LCD;

			for(int i = 0; i<LCD_SIZE; i++)
			{
				m_OUTBuffer[1+i] = (char)pStrLCD->GetAt(i);
			}
		}

		m_pUSBComms->Write(m_OUTBuffer, OUT_SIZE);
	}
}

//Static function necessary to be start of worker thread.
//Just used to call non static member.
UINT CRSKUSB::ReadThreadStatic(LPVOID pParam)
{
	//Call non-static member function
	return ((CRSKUSB*)pParam)->ReadThread();
}


/*
ReadThread:
Continuously try and read from the device.
When data is read it will be processed.
This allows a device to send messages when it wants to rather than just
in response to a command from the host.

If an error occours when doing a read then a Disconnection will be made.
(NOTE: When the device is reset or disconnected a read error will ocoour.)

NOTE: When performing a SW Disconnect the pending read does not terminate!
Because of this you can not use an infinite timeout on the Read.
Each time the read times-out it checks that a disconnection hasn't been made
before starting another pending read.


Only returns - i.e ends thread - if error or disconnection occurs.
Returns window error code. 
*/
UINT CRSKUSB::ReadThread()
{ 
	//Keep trying to do a read while we are connected
	DWORD dwError = ERROR_READ_FAULT;

	while(TRUE == m_bConnected)
	{
		dwError = m_pUSBComms->Read(m_INBuffer, IN_SIZE);
		if(dwError == ERROR_SUCCESS)
		{
			CString str;
			/*Have we been sent a new ADC value?*/
			if(0 != (m_INBuffer[0] & IN_BIT_ADC))
			{
				//The ADC value is stored in little endian format in 4 bytes
				DWORD dwADCValue = (0x000000FF & (DWORD)m_INBuffer[1]);
				dwADCValue |= ((0x0000FF00 & ((DWORD)m_INBuffer[2] << 8)));
				dwADCValue |= ((0x00FF0000 & ((DWORD)m_INBuffer[3] << 16)));
				dwADCValue |= ((0xFF000000 & ((DWORD)m_INBuffer[4] << 24)));
				//ADC Value
				str.Format("ADC Value = h'%.4lX.", dwADCValue);
				m_pGUI->MessageMain(str);
			}

			/*Has a switch been pressed?*/
			if(0 != (m_INBuffer[0] & IN_BIT_SW))
			{
				m_pGUI->MessageMain("Switch Pressed.");
			}
		}
		else
		{
			//An error has occoured.
			//If this was just part of disconnecting then not a real error.
			//Disconnect if not already disconnected.
			if(TRUE == m_bConnected)
			{
				AfxGetMainWnd()->MessageBox("Communications Failure\nDisconnecting from device.",
							"USB Demonstration", MB_OK | MB_ICONEXCLAMATION);
				Disconnect();
			}
		}
	}
	
	return dwError;
}