/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME    Dlglcd.h
DESCRIPTION  Dialog for entering the characters for a 16 character (2*8) LCD.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_DLGLCD_H__7A140E18_5D36_4131_A50B_DC79D27A8385__INCLUDED_)
#define AFX_DLGLCD_H__7A140E18_5D36_4131_A50B_DC79D27A8385__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlglcd.h : header file
//

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLCD dialog

class CDlgLCD : public CDialog
{
// Construction
public:
  CDlgLCD(CWnd* pParent = NULL);   // standard constructor
  int GetNumChars();
  CString GetText();
  void SetText(CString str);

// Dialog Data
  //{{AFX_DATA(CDlgLCD)
  enum { IDD = IDD_DIALOG_LCD };
  CString  m_str1;
  CString  m_str2;
  CString  m_str3;
  CString  m_str4;
  CString  m_str5;
  CString  m_str6;
  CString  m_str7;
  CString  m_str8;
  CString  m_str9;
  CString  m_str10;
  CString  m_str11;
  CString  m_str12;
  CString  m_str13;
  CString  m_str14;
  CString  m_str15;
  CString  m_str16;
  //}}AFX_DATA

protected: //Data
  CString m_strText; 


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDlgLCD)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDlgLCD)
  virtual BOOL OnInitDialog();
  afx_msg void OnChangeEdit1();
  afx_msg void OnChangeEdit2();
  afx_msg void OnChangeEdit3();
  afx_msg void OnChangeEdit4();
  afx_msg void OnChangeEdit5();
  afx_msg void OnChangeEdit6();
  afx_msg void OnChangeEdit7();
  afx_msg void OnChangeEdit8();
  afx_msg void OnChangeEdit9();
  afx_msg void OnChangeEdit10();
  afx_msg void OnChangeEdit11();
  afx_msg void OnChangeEdit12();
  afx_msg void OnChangeEdit13();
  afx_msg void OnChangeEdit14();
  afx_msg void OnChangeEdit15();
  afx_msg void OnChangeEdit16();
  afx_msg void OnSetfocusEdit1();
  afx_msg void OnSetfocusEdit2();
  afx_msg void OnSetfocusEdit3();
  afx_msg void OnSetfocusEdit4();
  afx_msg void OnSetfocusEdit5();
  afx_msg void OnSetfocusEdit6();
  afx_msg void OnSetfocusEdit7();
  afx_msg void OnSetfocusEdit8();
  afx_msg void OnSetfocusEdit9();
  afx_msg void OnSetfocusEdit10();
  afx_msg void OnSetfocusEdit11();
  afx_msg void OnSetfocusEdit12();
  afx_msg void OnSetfocusEdit13();
  afx_msg void OnSetfocusEdit14();
  afx_msg void OnSetfocusEdit15();
  afx_msg void OnSetfocusEdit16();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLCD_H__7A140E18_5D36_4131_A50B_DC79D27A8385__INCLUDED_)
