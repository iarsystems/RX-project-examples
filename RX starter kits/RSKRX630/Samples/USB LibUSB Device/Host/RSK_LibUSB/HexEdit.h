/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME      HexEdit.cpp
DESCRIPTION    Class used to override the default CEdit functionality to allow
        only hex input.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

#if !defined(AFX_HEXEDIT_H__98E722E0_3758_11D2_B637_E9CA302E8E40__INCLUDED_)
#define AFX_HEXEDIT_H__98E722E0_3758_11D2_B637_E9CA302E8E40__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HexEdit.h : header file
//

//Notfication messages


#ifdef _FLASHVIEWS_BUILD_DLL
#define CLASS_DLL_LINKAGE AFX_EXT_CLASS
#else
#define CLASS_DLL_LINKAGE __declspec(dllimport)
#endif

// Message to notify parent of change
#define HEXEDIT_CHANGE_NOTIFY  WM_USER + 1933

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CEdit
{
// Construction
public:
  CHexEdit();

// Attributes
public:

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHexEdit)
  protected:
  virtual void PreSubclassWindow();
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual void SetHexOnly(BOOL bHexOnly = FALSE);
  virtual void SetValue(DWORD Val);
  virtual DWORD GetValue(void);
  virtual ~CHexEdit();
  // Generated message map functions
protected:
  BOOL m_bHexOnly;
  //{{AFX_MSG(CHexEdit)
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChange();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  //}}AFX_MSG
  virtual void ProcessChange(void);

  CFont        m_fontEdit; // font for the edit field

  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__98E722E0_3758_11D2_B637_E9CA302E8E40__INCLUDED_)
