/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME      LibUSBDlg.h

DESCRIPTION    1. The 'About' Dialog.
        2. The main modal dialog of this dialog based application.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_LIBUSBDLG_H__28611AAE_FDFC_4F4F_B37F_866C20373E54__INCLUDED_)
#define AFX_LIBUSBDLG_H__28611AAE_FDFC_4F4F_B37F_866C20373E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h> //For CCriticalSection

#include "LibUSBDlg.h"
#include "RSKUSB.h"
#include "GUI.h"
#include "DlgConnect.h"


/////////////////////////////////////////////////////////////////////////////
// CLibUSBDlg dialog

class CLibUSBDlg : public CDialog, public CGUI
{
// Construction
public:
  CLibUSBDlg(CWnd* pParent = NULL);  // standard constructor
  ~CLibUSBDlg();


  void Connected(BOOL bConnected);
  void MessageMain(CString str);
  void MessageOther(CString str);

// Dialog Data
  //{{AFX_DATA(CLibUSBDlg)
  enum { IDD = IDD_RSK_LIBUSB_DIALOG };
  CRichEditCtrl  m_RichEditOther;
  CRichEditCtrl  m_RichEditMain;
  CString  m_strMain;
  CString  m_strOther;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CLibUSBDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  HICON m_hIcon;
  USHORT m_ProductID;
  USHORT m_VendorID;
  volatile BOOL m_bConnected;
  CRSKUSB m_oRSKUSB;
  CFont m_oFontMain;
  CFont m_oFontOther;

  int m_LinesMain;
  int m_LinesOther;

  //Provides Thread safe access to m_strMain and m_strOther; 
  CCriticalSection oCriticalSection;

protected:

  void SetButtons(BOOL bConnected);
  LRESULT UpdateFromThreadMain(WPARAM wParam, LPARAM lParam);
  LRESULT UpdateFromThreadOther(WPARAM wParam, LPARAM lParam);

  // Generated message map functions
  //{{AFX_MSG(CLibUSBDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnButtonConnect();
  afx_msg void OnButtonDisconnect();
  afx_msg void OnButtonLED();
  afx_msg void OnButtonADC();
  afx_msg void OnButtonLCD();
  afx_msg void OnClose();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBUSBDLG_H__28611AAE_FDFC_4F4F_B37F_866C20373E54__INCLUDED_)
