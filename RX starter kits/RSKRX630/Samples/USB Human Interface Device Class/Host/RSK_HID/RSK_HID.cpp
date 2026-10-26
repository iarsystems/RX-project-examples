/***********************************************************************************
FILE NAME  		RSK_HID.cpp
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

#include "stdafx.h"
#include "RSK_HID.h"
#include "RSK_HIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSK_HIDApp

BEGIN_MESSAGE_MAP(CRSK_HIDApp, CWinApp)
	//{{AFX_MSG_MAP(CRSK_HIDApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSK_HIDApp construction

CRSK_HIDApp::CRSK_HIDApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRSK_HIDApp object

CRSK_HIDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRSK_HIDApp initialization

BOOL CRSK_HIDApp::InitInstance()
{
	AfxInitRichEdit(); //Required if using rich edit controls.
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CRSK_HIDDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
