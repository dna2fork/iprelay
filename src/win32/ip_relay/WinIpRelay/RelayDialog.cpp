// RelayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinIpRelay.h"
#include "RelayDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
#define DEFAULT_PORT    9000
#define DEFAULT_HOST    "localhost"
#define DEFAULT_TIMEOUT 86400

/////////////////////////////////////////////////////////////////////////////
// CRelayDialog dialog


CRelayDialog::CRelayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRelayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRelayDialog)
	m_local_port  = DEFAULT_PORT;
	m_remote_host = _T(DEFAULT_HOST);
	m_remote_port = DEFAULT_PORT;
	m_timeout     = DEFAULT_TIMEOUT;
	//}}AFX_DATA_INIT
}


void CRelayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRelayDialog)
	DDX_Text(pDX, IDC_RELAY_LOCAL_PORT, m_local_port);
	DDV_MinMaxUInt(pDX, m_local_port, 0, 65535);
	DDX_Text(pDX, IDC_RELAY_REMOTE_HOST, m_remote_host);
	DDX_Text(pDX, IDC_RELAY_REMOTE_PORT, m_remote_port);
	DDV_MinMaxUInt(pDX, m_remote_port, 0, 65535);
	DDX_Text(pDX, IDC_RELAY_TIMEOUT, m_timeout);
	DDV_MinMaxLong(pDX, m_timeout, 0, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRelayDialog, CDialog)
	//{{AFX_MSG_MAP(CRelayDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelayDialog message handlers
