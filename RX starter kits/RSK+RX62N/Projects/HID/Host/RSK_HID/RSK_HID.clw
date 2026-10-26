; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgLCD
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rsk_hid.h"
LastPage=0

ClassCount=6
Class1=CDlgConnect
Class2=CDlgLCD
Class3=CHexEdit
Class4=CRSK_HIDApp
Class5=CAboutDlg
Class6=CRSK_HIDDlg

ResourceCount=4
Resource1=IDD_RSK_HID_DIALOG (English (U.S.))
Resource2=IDD_DIALOG_CONNECT
Resource3=IDD_ABOUTBOX (English (U.S.))
Resource4=IDD_DIALOG_LCD

[CLS:CDlgConnect]
Type=0
BaseClass=CDialog
HeaderFile=DlgConnect.h
ImplementationFile=DlgConnect.cpp

[CLS:CDlgLCD]
Type=0
BaseClass=CDialog
HeaderFile=DlgLCD.h
ImplementationFile=DlgLCD.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[CLS:CHexEdit]
Type=0
BaseClass=CEdit
HeaderFile=HexEdit.h
ImplementationFile=HexEdit.cpp

[CLS:CRSK_HIDApp]
Type=0
BaseClass=CWinApp
HeaderFile=RSK_HID.h
ImplementationFile=RSK_HID.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=RSK_HIDDlg.cpp
ImplementationFile=RSK_HIDDlg.cpp
LastObject=CAboutDlg

[CLS:CRSK_HIDDlg]
Type=0
BaseClass=CDialog
HeaderFile=RSK_HIDDlg.h
ImplementationFile=RSK_HIDDlg.cpp

[DLG:IDD_DIALOG_CONNECT]
Type=1
Class=CDlgConnect
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_VID,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_PID,edit,1350631552
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_LCD]
Type=1
Class=CDlgLCD
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631424
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_EDIT3,edit,1350631552
Control6=IDC_EDIT4,edit,1350631552
Control7=IDC_EDIT5,edit,1350631552
Control8=IDC_EDIT6,edit,1350631552
Control9=IDC_EDIT7,edit,1350631552
Control10=IDC_EDIT8,edit,1350631552
Control11=IDC_EDIT9,edit,1350631552
Control12=IDC_EDIT10,edit,1350631552
Control13=IDC_EDIT11,edit,1350631552
Control14=IDC_EDIT12,edit,1350631552
Control15=IDC_EDIT13,edit,1350631552
Control16=IDC_EDIT14,edit,1350631552
Control17=IDC_EDIT15,edit,1350631552
Control18=IDC_EDIT16,edit,1350631552
Control19=IDC_STATIC_BMP_LCD,static,1342179342

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_RSK_HID_DIALOG]
Type=1
Class=CRSK_HIDDlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RSK_HID_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_BUTTON_CONNECT,button,1342242817
Control2=IDC_BUTTON_DISCONNECT,button,1342242816
Control3=IDC_BUTTON_LED,button,1342242816
Control4=IDC_BUTTON_ADC,button,1342242816
Control5=IDC_BUTTON_LCD,button,1342242816
Control6=IDC_RICHEDIT_MAIN,RICHEDIT,1352665156
Control7=IDC_RICHEDIT_2,RICHEDIT,1352665156

