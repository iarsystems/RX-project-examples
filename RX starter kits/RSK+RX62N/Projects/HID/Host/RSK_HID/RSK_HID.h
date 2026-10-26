/***********************************************************************************
FILE NAME  		RSK_HID.h
DESCRIPTION		The Application (CWinApp based) class.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_RSK_HID_H__E4728449_06C7_4EB0_AE45_B46BD60847DE__INCLUDED_)
#define AFX_RSK_HID_H__E4728449_06C7_4EB0_AE45_B46BD60847DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRSK_HIDApp:
// See RSK_HID.cpp for the implementation of this class
//

class CRSK_HIDApp : public CWinApp
{
public:
	CRSK_HIDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSK_HIDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRSK_HIDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSK_HID_H__E4728449_06C7_4EB0_AE45_B46BD60847DE__INCLUDED_)
