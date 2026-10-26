/***********************************************************************************
FILE NAME  	DlgLCD.cpp
DESCRIPTION	Dialog for entering the characters for a 16 character (2*8) LCD.

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
#include "Dlglcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Number of characters the LCD has.
#define LCD_NUM_CHARS 16 //(2*8)

//Default text to diaplay on LCD after a reset.
#define DEFAULT_LCD_TEXT "Renesas USB HID "

/////////////////////////////////////////////////////////////////////////////
// CDlgLCD dialog


CDlgLCD::CDlgLCD(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLCD::IDD, pParent),
	m_strText(DEFAULT_LCD_TEXT)
{
	//{{AFX_DATA_INIT(CDlgLCD)
	m_str1 = _T("");
	m_str2 = _T("");
	m_str3 = _T("");
	m_str4 = _T("");
	m_str5 = _T("");
	m_str6 = _T("");
	m_str7 = _T("");
	m_str8 = _T("");
	m_str9 = _T("");
	m_str10 = _T("");
	m_str11 = _T("");
	m_str12 = _T("");
	m_str13 = _T("");
	m_str14 = _T("");
	m_str15 = _T("");
	m_str16 = _T("");
	//}}AFX_DATA_INIT
}

void CDlgLCD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLCD)
	DDX_Text(pDX, IDC_EDIT1, m_str1);
	DDV_MaxChars(pDX, m_str1, 1);

	DDX_Text(pDX, IDC_EDIT2, m_str2);
	DDV_MaxChars(pDX, m_str2, 1);

	DDX_Text(pDX, IDC_EDIT3, m_str3);
	DDV_MaxChars(pDX, m_str3, 1);

	DDX_Text(pDX, IDC_EDIT4, m_str4);
	DDV_MaxChars(pDX, m_str4, 1);

	DDX_Text(pDX, IDC_EDIT5, m_str5);
	DDV_MaxChars(pDX, m_str5, 1);

	DDX_Text(pDX, IDC_EDIT6, m_str6);
	DDV_MaxChars(pDX, m_str6, 1);

	DDX_Text(pDX, IDC_EDIT7, m_str7);
	DDV_MaxChars(pDX, m_str7, 1);

	DDX_Text(pDX, IDC_EDIT8, m_str8);
	DDV_MaxChars(pDX, m_str8, 1);

	DDX_Text(pDX, IDC_EDIT9, m_str9);
	DDV_MaxChars(pDX, m_str9, 1);

	DDX_Text(pDX, IDC_EDIT10, m_str10);
	DDV_MaxChars(pDX, m_str10, 1);

	DDX_Text(pDX, IDC_EDIT11, m_str11);
	DDV_MaxChars(pDX, m_str11, 1);

	DDX_Text(pDX, IDC_EDIT12, m_str12);
	DDV_MaxChars(pDX, m_str12, 1);

	DDX_Text(pDX, IDC_EDIT13, m_str13);
	DDV_MaxChars(pDX, m_str13, 1);

	DDX_Text(pDX, IDC_EDIT14, m_str14);
	DDV_MaxChars(pDX, m_str14, 1);

	DDX_Text(pDX, IDC_EDIT15, m_str15);
	DDV_MaxChars(pDX, m_str15, 1);

	DDX_Text(pDX, IDC_EDIT16, m_str16);
	DDV_MaxChars(pDX, m_str16, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLCD, CDialog)
	//{{AFX_MSG_MAP(CDlgLCD)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, OnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, OnChangeEdit9)
	ON_EN_CHANGE(IDC_EDIT10, OnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT11, OnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT12, OnChangeEdit12)
	ON_EN_CHANGE(IDC_EDIT13, OnChangeEdit13)
	ON_EN_CHANGE(IDC_EDIT14, OnChangeEdit14)
	ON_EN_CHANGE(IDC_EDIT15, OnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT16, OnChangeEdit16)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, OnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, OnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, OnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, OnSetfocusEdit5)
	ON_EN_SETFOCUS(IDC_EDIT6, OnSetfocusEdit6)
	ON_EN_SETFOCUS(IDC_EDIT7, OnSetfocusEdit7)
	ON_EN_SETFOCUS(IDC_EDIT8, OnSetfocusEdit8)
	ON_EN_SETFOCUS(IDC_EDIT9, OnSetfocusEdit9)
	ON_EN_SETFOCUS(IDC_EDIT10, OnSetfocusEdit10)
	ON_EN_SETFOCUS(IDC_EDIT11, OnSetfocusEdit11)
	ON_EN_SETFOCUS(IDC_EDIT12, OnSetfocusEdit12)
	ON_EN_SETFOCUS(IDC_EDIT13, OnSetfocusEdit13)
	ON_EN_SETFOCUS(IDC_EDIT14, OnSetfocusEdit14)
	ON_EN_SETFOCUS(IDC_EDIT15, OnSetfocusEdit15)
	ON_EN_SETFOCUS(IDC_EDIT16, OnSetfocusEdit16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLCD message handlers

BOOL CDlgLCD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Make sure picture is on top because as it is transparent it will let
	//the windows beneath it be seen - which is what we want.
	CWnd* pWnd = GetDlgItem(IDC_STATIC_BMP_LCD);
	pWnd->BringWindowToTop();

	//Display current text.
	SetText(m_strText);
	UpdateData(FALSE);
	
	/*Set Focus on first edit*/
	pWnd = GetDlgItem(IDC_EDIT1);
	pWnd->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//OnChange handlers.
//Set Focus to the next edit.
void CDlgLCD::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	//Set focus to the next edit, unless this change was a delete
	CWnd* pWnd = GetDlgItem(IDC_EDIT2);
	UpdateData();
	if(!m_str1.IsEmpty()){pWnd->SetFocus();}
}
void CDlgLCD::OnChangeEdit2(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT3);
	UpdateData();
	if(!m_str2.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit3(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT4);
	UpdateData();
	if(!m_str3.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit4(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT5);
	UpdateData();
	if(!m_str4.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit5(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT6);
	UpdateData();
	if(!m_str5.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit6(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT7);
	UpdateData();
	if(!m_str6.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit7(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT8);
	UpdateData();
	if(!m_str7.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit8(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT9);
	UpdateData();
	if(!m_str8.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit9(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT10);
	UpdateData();
	if(!m_str9.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit10(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT11);
	UpdateData();
	if(!m_str10.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit11(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT12);
	UpdateData();
	if(!m_str11.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit12(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT13);
	UpdateData();
	if(!m_str12.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit13(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT14);
	UpdateData();
	if(!m_str13.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit14(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT15);
	UpdateData();
	if(!m_str14.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit15(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT16);
	UpdateData();
	if(!m_str15.IsEmpty()){pWnd->SetFocus();}}
void CDlgLCD::OnChangeEdit16(){
	CWnd* pWnd = GetDlgItem(IDC_EDIT1);
	UpdateData();
	if(!m_str16.IsEmpty()){pWnd->SetFocus();}}


//Return the total number of characters in this LCD
int CDlgLCD::GetNumChars()
{
	return LCD_NUM_CHARS;
}

//Set the LCD.
//String supplied must be equall to the number of characters the LCD has.
void CDlgLCD::SetText(CString str)
{
	ASSERT(str.GetLength() == GetNumChars());

	if(str.GetLength() == GetNumChars())
	{
		m_strText = str;

		m_str1 = str.GetAt(0);
		m_str2 = str.GetAt(1);
		m_str3 = str.GetAt(2);
		m_str4 = str.GetAt(3);
		m_str5 = str.GetAt(4);
		m_str6 = str.GetAt(5);
		m_str7 = str.GetAt(6);
		m_str8 = str.GetAt(7);
		m_str9 = str.GetAt(8);
		m_str10 = str.GetAt(9);
		m_str11 = str.GetAt(10);
		m_str12 = str.GetAt(11);
		m_str13 = str.GetAt(12);
		m_str14 = str.GetAt(13);
		m_str15 = str.GetAt(14);
		m_str16 = str.GetAt(15);
	}
}

//Return the LCD text in a single string
CString CDlgLCD::GetText()
{
	return m_strText;
}

//OnSetfocus handlers
void CDlgLCD::OnSetfocusEdit1() 
{
	// TODO: Add your control notification handler code here

	//Select the character so user doesn't have to.
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit2() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit3() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit4() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit5() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit6() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT6);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit7() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT7);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit8() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT8);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit9() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT9);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit10() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT10);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit11() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT11);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit12() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT12);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit13() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT13);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit14() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT14);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit15() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT15);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnSetfocusEdit16() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT16);
	pEdit->PostMessage(EM_SETSEL,0,-1);
}

void CDlgLCD::OnOK() 
{
	// TODO: Add extra validation here
	
	// Update text.

	/*If a string is empty set it as a space*/
	if(m_str1.IsEmpty()){m_str1 = " ";}
	if(m_str2.IsEmpty()){m_str2 = " ";}
	if(m_str3.IsEmpty()){m_str3 = " ";}
	if(m_str4.IsEmpty()){m_str4 = " ";}
	if(m_str5.IsEmpty()){m_str5 = " ";}
	if(m_str6.IsEmpty()){m_str6 = " ";}
	if(m_str7.IsEmpty()){m_str7 = " ";}
	if(m_str8.IsEmpty()){m_str8 = " ";}
	if(m_str9.IsEmpty()){m_str9 = " ";}
	if(m_str10.IsEmpty()){m_str10 = " ";}
	if(m_str11.IsEmpty()){m_str11 = " ";}
	if(m_str12.IsEmpty()){m_str12 = " ";}
	if(m_str13.IsEmpty()){m_str13 = " ";}
	if(m_str14.IsEmpty()){m_str14 = " ";}
	if(m_str15.IsEmpty()){m_str15 = " ";}
	if(m_str16.IsEmpty()){m_str16 = " ";}

	m_strText = m_str1;
	m_strText += m_str2;
	m_strText += m_str3;
	m_strText += m_str4;
	m_strText += m_str5;
	m_strText += m_str6;
	m_strText += m_str7;
	m_strText += m_str8;
	m_strText += m_str9;
	m_strText += m_str10;
	m_strText += m_str11;
	m_strText += m_str12;
	m_strText += m_str13;
	m_strText += m_str14;
	m_strText += m_str15;
	m_strText += m_str16;	
	
	CDialog::OnOK();
}
