/***********************************************************************************
FILE NAME  		HexEdit.cpp
DESCRIPTION		Class used to override the default CEdit functionality to allow
				only hex input.

Copyright   : 2007 Renesas Technology Europe Ltd.
Copyright   : 2007 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

#include "stdafx.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
	m_bHexOnly = true;
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!m_bHexOnly)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	int nStart, nEnd;
	GetSel(nStart, nEnd);
	
	if(nEnd < 2)
		return;

	if(isxdigit(nChar))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if(isprint(nChar))
		return;

	if(nChar == 0xa3)	//isprint fails for this character
		return;

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	GetSel(nStart, nEnd);

	if(nStart == nEnd && nStart < 2)
		SetSel(2, 2);
}


void CHexEdit::OnChange() 
{
	ProcessChange();
	if(m_hWnd)
		GetParent()->SendMessage(HEXEDIT_CHANGE_NOTIFY, NULL, (LPARAM)this);
}

void CHexEdit::ProcessChange() 
{
	if(!m_bHexOnly)
		return;

	int nStart, nEnd, nOffset = 2;
	GetSel(nStart, nEnd);

	CString curText;
	CString NewText = "0x";
	GetWindowText(curText);

	if(!curText.GetLength())
	{
		SetWindowText(NewText);
		SetSel(2,2);
		return;
	}

	if(curText[0] == 'x')
	{
		//The 0 has been deleted
		NewText  = '0';
		NewText += curText;
		nOffset--;
	}
	else if(curText[0] == '0' && (curText.GetLength() < 2 || curText[1] != 'x'))
	{
		//The x has been deleted
		NewText += curText.Right(curText.GetLength() -1);
		nOffset--;
	}
	else if(curText[0] != '0' && (curText.GetLength() < 2 || curText[1] != 'x'))
	{
		NewText += curText;
	}
	else if(curText != "0x")
	{
		char *err = NULL;
		strtoul(curText, &err, 16);

		if(err && isprint(*err))
		{
			NewText = "0x";
			SetSel(2,2);
		}
		else
			return;
	}
	else
		return;

	if(NewText != "0x")
	{
		char *err = NULL;
		strtoul(NewText, &err, 16);

		if(err && isprint(*err))
		{
			NewText = "0x";
		}
	}

	SetWindowText(NewText);
	SetSel(nStart+nOffset, nEnd+nOffset);
}

int CHexEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetWindowText("0x");	
	SetHexOnly(TRUE);
	return 0;
}

void CHexEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
	SetHexOnly(TRUE);
//	SetWindowText("0x");	

	LOGFONT logfont;

	// get the old font size and use in new font
	GetFont()->GetLogFont(&logfont);
	m_fontEdit.CreateFont(logfont.lfHeight-1, logfont.lfWidth,
								 logfont.lfEscapement, logfont.lfOrientation, 
								 logfont.lfWeight, FALSE, FALSE, FALSE,
								 logfont.lfCharSet, logfont.lfOutPrecision,
								 logfont.lfClipPrecision, logfont.lfQuality, logfont.lfPitchAndFamily,
								 "Courier New");

	SetFont(&m_fontEdit);
}

DWORD CHexEdit::GetValue()
{
	CString Text;
	GetWindowText(Text);
	if(!Text.GetLength() ||  Text == "0x")
	{
		return(0);
	}

	char *err = NULL;
	DWORD dwValue = strtoul(Text, &err, 16);

	if(err && isprint(*err))
	{
		ASSERT(FALSE);
		return(0);
	}
return(dwValue);
}

void CHexEdit::SetValue(DWORD Val)
{
	char Text[128];

	if(Val <= 0xf)
		wsprintf(Text, "0x0%x", Val);
	else
		wsprintf(Text, "0x%x", Val);

	SetWindowText(Text);
}

void CHexEdit::SetHexOnly(BOOL bHexOnly)
{
	m_bHexOnly = bHexOnly;
	if(bHexOnly)
		SetLimitText(10);	//8 chars of Hex + 0x
	else
		SetLimitText(0);
}

void CHexEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_bHexOnly && nChar == VK_HOME)
	{
		int nStart, nEnd;
		GetSel(nStart, nEnd);
		if(nStart != nEnd || GetKeyState(VK_SHIFT) & 0x8000)
			SetSel(2, nEnd);
		else 
			SetSel(2, 2);
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHexEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
}
