/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************
FILE NAME      GUI.h
DESCRIPTION    Base class for GUI.
        Abstract Class - no implementation.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
08.11.2007 RTE-PIN First Release
***********************************************************************************/

#if !defined(AFX_GUI_H__18C12E8D_6AAB_4BAB_901A_B1238FE93689__INCLUDED_)
#define AFX_GUI_H__18C12E8D_6AAB_4BAB_901A_B1238FE93689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGUI  
{
public:
  CGUI();
  virtual ~CGUI();

  virtual void Connected(BOOL bConnected) = 0;
  virtual void MessageMain(CString str) = 0;
  virtual void MessageOther(CString str) = 0;
};

#endif // !defined(AFX_GUI_H__18C12E8D_6AAB_4BAB_901A_B1238FE93689__INCLUDED_)
