/***********************************************************************************
FILE NAME  	DlgConnect.cpp
DESCRIPTION	Dialog for entering a VID and PID.

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
#include "DlgConnect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConnect dialog

//Constructor - Supply default VID and PID.
CDlgConnect::CDlgConnect(USHORT VID, USHORT PID, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnect::IDD, pParent),
	m_VID(VID),
	m_PID(PID)
{
	//{{AFX_DATA_INIT(CDlgConnect)
	//}}AFX_DATA_INIT
}


void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConnect)
	DDX_Control(pDX, IDC_EDIT_VID, m_EditVID);
	DDX_Control(pDX, IDC_EDIT_PID, m_EditPID);
	//}}AFX_DATA_MAP

	if(!pDX->m_bSaveAndValidate)
	{
		//Set edit boxes with current values
		m_EditVID.SetValue(m_VID);
		m_EditPID.SetValue(m_PID);
	}
	else
	{
		//Read values from edit boxes.
		m_VID = m_EditVID.GetValue();
		m_PID = m_EditPID.GetValue();
	}
}


BEGIN_MESSAGE_MAP(CDlgConnect, CDialog)
	//{{AFX_MSG_MAP(CDlgConnect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConnect message handlers
