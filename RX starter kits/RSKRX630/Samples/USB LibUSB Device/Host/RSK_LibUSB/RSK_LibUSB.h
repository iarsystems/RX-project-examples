/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME      RSK_LibUSB.h

DESCRIPTION    The Application (CWinApp based) class.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

#if !defined(AFX_RSK_LIBUSB_H__C0B23C6A_96BC_406C_9407_0E221FD27819__INCLUDED_)
#define AFX_RSK_LIBUSB_H__C0B23C6A_96BC_406C_9407_0E221FD27819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"    // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRSK_LibUSBApp:
// See RSK_LibUSB.cpp for the implementation of this class
//

class CRSK_LibUSBApp : public CWinApp
{
public:
  CRSK_LibUSBApp();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRSK_LibUSBApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

// Implementation

  //{{AFX_MSG(CRSK_LibUSBApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSK_LIBUSB_H__C0B23C6A_96BC_406C_9407_0E221FD27819__INCLUDED_)
