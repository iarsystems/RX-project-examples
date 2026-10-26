/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME    DlgConnect.h
DESCRIPTION  Dialog for entering a VID and PID.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_DLGCONNECT_H__BC5C6B68_6ABA_4876_B822_A13BE5C62077__INCLUDED_)
#define AFX_DLGCONNECT_H__BC5C6B68_6ABA_4876_B822_A13BE5C62077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConnect.h : header file

#include "HexEdit.h"
#include "Resource.h"
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConnect dialog

class CDlgConnect : public CDialog
{
// Construction
public:
  CDlgConnect(USHORT VID, USHORT PID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDlgConnect)
  enum { IDD = IDD_DIALOG_CONNECT };
  CHexEdit  m_EditVID;
  CHexEdit  m_EditPID;
  //}}AFX_DATA

  UINT  m_VID;
  UINT  m_PID;


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDlgConnect)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDlgConnect)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONNECT_H__BC5C6B68_6ABA_4876_B822_A13BE5C62077__INCLUDED_)
