/***********************************************************************************
FILE NAME  		LibUSBDlg.cpp

DESCRIPTION		1. The 'About' Dialog.
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

#include "stdafx.h"
#include "RSK_LibUSB.h"
#include "LibUSBDlg.h"
#include "DlgLCD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Default VID/PID
#define VENDOR_ID	0x045B //Renesas
#define PRODUCT_ID	0x2016 //Default PID "RSK Generic LibUSB Application"


//Messages to update dialog
#define WM_MYMESSAGE_UPDATEFROMTHREAD_MAIN  (WM_USER + 1)
#define WM_MYMESSAGE_UPDATEFROMTHREAD_OTHER (WM_USER + 2)

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Disable compiler warning C4355.
//Using 'this' in constructor below is safe as long as it isn't dereferenced.
#pragma warning(disable:4355)

/////////////////////////////////////////////////////////////////////////////
// CLibUSBDlg dialog

CLibUSBDlg::CLibUSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibUSBDlg::IDD, pParent),
	m_ProductID(PRODUCT_ID),
	m_VendorID(VENDOR_ID),
	m_bConnected(FALSE),
	m_oRSKUSB((CGUI*)this)
{
	//{{AFX_DATA_INIT(CLibUSBDlg)
	m_strMain = _T("");
	m_strOther = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLibUSBDlg::~CLibUSBDlg()
{
	//NOTE OnClose function (WM_CLOSE) is used.
}

//Call to tell the dialog that a connection has been made or broken.
//Sets the button states accordingly.
void CLibUSBDlg::Connected(BOOL bConnected)
{
	m_bConnected = bConnected;

	if(TRUE == bConnected)
	{
		//Connected
		MessageMain("Device Connected and ready to use:\n");
		SetButtons(TRUE);
	}
	else
	{
		MessageMain("Disconnected.");
		//Disconnected
		SetButtons(FALSE);
	}
}

//Send a message to the main window of the dialog
//This can be called from a worker thread.
void CLibUSBDlg::MessageMain(CString str)
{
	//Provide thread synchronisation for updating the CStrings.
	CSingleLock oSingleLock(&oCriticalSection);
	oSingleLock.Lock();

	m_strMain += str;
	m_strMain += "\r\n";

	//Using windows messaging so that these functions can be called
	//from a worker thread.
	PostMessage(WM_MYMESSAGE_UPDATEFROMTHREAD_MAIN);
}

//Send a message to the other window of the dialog
//This can be called from a worker thread.
void CLibUSBDlg::MessageOther(CString str)
{
	//Provide thread synchronisation for updating the CStrings.
	CSingleLock oSingleLock(&oCriticalSection);
	oSingleLock.Lock();

	m_strOther += str;
	m_strOther += "\r\n";

	//Using windows messaging so that these functions can be called
	//from a worker thread.
	PostMessage(WM_MYMESSAGE_UPDATEFROMTHREAD_OTHER);
}

//Set the state of the buttons according to the conencted status
void CLibUSBDlg::SetButtons(BOOL bConnected)
{
	CButton* pConnected = (CButton*)GetDlgItem(IDC_BUTTON_CONNECT);
	CButton* pDisconnected = (CButton*)GetDlgItem(IDC_BUTTON_DISCONNECT);
	CButton* pLED = (CButton*)GetDlgItem(IDC_BUTTON_LED);
	CButton* pADC = (CButton*)GetDlgItem(IDC_BUTTON_ADC);
	CButton* pLCD = (CButton*)GetDlgItem(IDC_BUTTON_LCD);

	
	pConnected->EnableWindow(bConnected?FALSE:TRUE);
	pDisconnected->EnableWindow(bConnected);
	pLED->EnableWindow(bConnected);
	pADC->EnableWindow(bConnected);
	pLCD->EnableWindow(bConnected);

	/*Set focus*/
	if(TRUE == bConnected)
	{
		GotoDlgCtrl(pLED);
	}
	else
	{
		GotoDlgCtrl(pConnected);
	}
}



void CLibUSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLibUSBDlg)
	DDX_Control(pDX, IDC_RICHEDIT_2, m_RichEditOther);
	DDX_Control(pDX, IDC_RICHEDIT_MAIN, m_RichEditMain);
	DDX_Text(pDX, IDC_RICHEDIT_MAIN, m_strMain);
	DDX_Text(pDX, IDC_RICHEDIT_2, m_strOther);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLibUSBDlg, CDialog)
	//{{AFX_MSG_MAP(CLibUSBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_LED, OnButtonLED)
	ON_BN_CLICKED(IDC_BUTTON_ADC, OnButtonADC)
	ON_BN_CLICKED(IDC_BUTTON_LCD, OnButtonLCD)
	ON_MESSAGE(WM_MYMESSAGE_UPDATEFROMTHREAD_MAIN, UpdateFromThreadMain)
	ON_MESSAGE(WM_MYMESSAGE_UPDATEFROMTHREAD_OTHER, UpdateFromThreadOther)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibUSBDlg message handlers

BOOL CLibUSBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	//Initialise the state of the buttons
	SetButtons(FALSE);

	//Set font on edit boxes
	m_oFontMain.CreatePointFont(100, "Courier");
	m_oFontOther.CreatePointFont(100, "Arial");
	m_RichEditMain.SetFont(&m_oFontMain); //This is required but I don't understand why.
	m_RichEditOther.SetFont(&m_oFontOther);

	//Calculate how many vertical lines each edit box is (needed for scrolling)
	CSize oSize;
	CRect oRect;
	//main
	//Want height of txt.
	CClientDC cdcMain(&m_RichEditMain);
	cdcMain.SelectObject(&m_oFontMain);
	oSize = cdcMain.GetTextExtent("Dummy");
	//Size of edit
	m_RichEditMain.GetRect(&oRect);
	//Calculate number of lines.
	m_LinesMain = oRect.Height() / oSize.cy;
	//Other
	//Want height of txt.
	CClientDC cdcOther(&m_RichEditOther);
	cdcOther.SelectObject(&m_oFontOther); //This is required but I don't understand why.
	oSize = cdcOther.GetTextExtent("Dummy");
	//Size of edit
	m_RichEditOther.GetRect(&oRect);
	//Calculate number of lines.
	m_LinesOther = oRect.Height() / oSize.cy;

	// TODO: Add extra initialization here
	MessageMain("Started Application.");
	MessageOther("Started Application.");

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLibUSBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLibUSBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLibUSBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//Connect button has been clicked
void CLibUSBDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	CDlgConnect oDlg(m_VendorID, m_ProductID);
	if(IDOK == oDlg.DoModal())
	{
		m_ProductID = oDlg.m_PID;
		m_VendorID = oDlg.m_VID;
		m_oRSKUSB.Connect(m_VendorID, m_ProductID);
	}
}

//Disconnect button has been clicked
void CLibUSBDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	m_oRSKUSB.Disconnect();
}

//LED toggle button has been clicked
void CLibUSBDlg::OnButtonLED() 
{
	// TODO: Add your control notification handler code here
	MessageOther("\r\nLED Toggle:");

	m_oRSKUSB.ToggleLED();
}

//Read ADC button has been clicked
void CLibUSBDlg::OnButtonADC() 
{
	// TODO: Add your control notification handler code here
	MessageOther("\r\nRead ADC:");

	m_oRSKUSB.ReadADC();
}

void CLibUSBDlg::OnButtonLCD() 
{
	// TODO: Add your control notification handler code here

	//Show dialog so user can enter LCD text
	//Static so LCD data is not initialised to default values each time.
	static CDlgLCD oDlgLCD;
	if(IDOK == oDlgLCD.DoModal())
	{
		CString str = oDlgLCD.GetText();

		MessageOther("\r\nSet LCD:");

		m_oRSKUSB.SetLCD(str);
	}
}

//So that the updating of the GUI was possible from worker threads this
//can be called by posting a WM_MYMESSAGE_UPDATEFROMTHREAD_MAIN message.
//Updates the main display window.
LRESULT CLibUSBDlg::UpdateFromThreadMain(WPARAM wParam, LPARAM lParam)
{
	//Provide thread synchronisation for updating the CStrings.
	CSingleLock oSingleLock(&oCriticalSection);
	oSingleLock.Lock();

	//Update text of edit
	m_RichEditMain.SetWindowText(m_strMain);

	//Want to scroll so have a full screens worth with most recent showing.
	//First Scroll to start
	m_RichEditMain.LineScroll(-(m_RichEditMain.GetLineCount()+1));
	//Now scroll down the number of lines that the conrol has - lines visable
	m_RichEditMain.LineScroll(m_RichEditMain.GetLineCount() - m_LinesMain);
		
	return 0L;
}

//So that the updating of the GUI was possible from worker threads this
//can be called by posting a WM_MYMESSAGE_UPDATEFROMTHREAD_OTHER message.
//Updates the other display window.
LRESULT CLibUSBDlg::UpdateFromThreadOther(WPARAM wParam, LPARAM lParam)
{
	//Provide thread synchronisation for updating the CStrings.
	CSingleLock oSingleLock(&oCriticalSection);
	oSingleLock.Lock();

	//Update text of edit
	m_RichEditOther.SetWindowText(m_strOther);

	//Want to scroll so have a full screens worth with most recent showing.
	//First Scroll to start
	m_RichEditOther.LineScroll(-(m_RichEditOther.GetLineCount()+1));
	//Now scroll down the number of lines that the conrol has - lines visable
	m_RichEditOther.LineScroll(m_RichEditOther.GetLineCount() - m_LinesOther);
	
	return 0L;
}

//WM_CLOSE handler
void CLibUSBDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	//If connected then disconnect.
	if(TRUE == m_bConnected)
	{
		m_oRSKUSB.Disconnect();
	}

	
	CDialog::OnClose();
}
