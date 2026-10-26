/***********************************************************************************
FILE NAME  		USBComms.cpp

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
#include "stdafx.h"
#include "USBComms.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Abstract base class - no implementation.

CUSBComms::CUSBComms(CGUI* pGUI):
m_pGUI(pGUI)
{

}

CUSBComms::~CUSBComms()
{

}
