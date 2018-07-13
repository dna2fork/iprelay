; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinIpRelayDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinIpRelay.h"

ClassCount=5
Class1=CWinIpRelayApp
Class2=CWinIpRelayDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WINIPRELAY_DIALOG
Class4=CRelayDialog
Resource4=IDD_ADD_RELAY_DIALOG
Class5=CRegistrationDialog
Resource5=IDD_REGISTRATION_DIALOG

[CLS:CWinIpRelayApp]
Type=0
HeaderFile=WinIpRelay.h
ImplementationFile=WinIpRelay.cpp
Filter=N

[CLS:CWinIpRelayDlg]
Type=0
HeaderFile=WinIpRelayDlg.h
ImplementationFile=WinIpRelayDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SAVE_CONFIG

[CLS:CAboutDlg]
Type=0
HeaderFile=WinIpRelayDlg.h
ImplementationFile=WinIpRelayDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353

[DLG:IDD_WINIPRELAY_DIALOG]
Type=1
Class=CWinIpRelayDlg
ControlCount=15
Control1=IDC_RELAY_LIST,listbox,1353777411
Control2=IDC_ADD_RELAY,button,1342242816
Control3=IDC_DELETE_RELAY,button,1342242816
Control4=IDC_SAVE_LOGS,button,1342242816
Control5=IDC_CLEAR_LOGS,button,1342242816
Control6=IDC_LOAD_CONFIG,button,1342242816
Control7=IDC_SAVE_CONFIG,button,1342242816
Control8=IDC_ABOUT_US,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_CONFIG_NAME,edit,1350633600
Control12=IDC_STATIC,button,1342177287
Control13=IDOK,button,1342242816
Control14=IDC_LOGS,RICHEDIT,1353713860
Control15=IDC_COPYRIGHT,static,1342308865

[DLG:IDD_ADD_RELAY_DIALOG]
Type=1
Class=CRelayDialog
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_RELAY_LOCAL_PORT,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_RELAY_REMOTE_HOST,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RELAY_REMOTE_PORT,edit,1350631552
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_RELAY_TIMEOUT,edit,1350631552

[CLS:CRelayDialog]
Type=0
HeaderFile=RelayDialog.h
ImplementationFile=RelayDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CRelayDialog
VirtualFilter=dWC

[DLG:IDD_REGISTRATION_DIALOG]
Type=1
Class=CRegistrationDialog
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_REG_FILENAME,edit,1350631552
Control4=IDC_REG_BROWSE,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_REG_URL_LINK,static,1342373888
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_REG_STATUS,static,1342308352

[CLS:CRegistrationDialog]
Type=0
HeaderFile=RegistrationDialog.h
ImplementationFile=RegistrationDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_REG_FILENAME
VirtualFilter=dWC

