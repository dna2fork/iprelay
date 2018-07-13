// RegistrationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinIpRelay.h"
#include "RegistrationDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define REG_FILE_EXT         ".dat"
#define REG_FILE_EXT_FILTER  "Registration files (*.dat)|*.dat|All Files (*.*)|*.*||"
#define REG_DEFAULT_FILENAME "registration.dat"


/////////////////////////////////////////////////////////////////////////////
// CRegistrationDialog dialog


CRegistrationDialog::CRegistrationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegistrationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegistrationDialog)
	m_reg_filename = _T("");
	m_reg_status = _T("");
	//}}AFX_DATA_INIT
}


void CRegistrationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegistrationDialog)
	DDX_Text(pDX, IDC_REG_FILENAME, m_reg_filename);
	DDX_Text(pDX, IDC_REG_STATUS, m_reg_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegistrationDialog, CDialog)
	//{{AFX_MSG_MAP(CRegistrationDialog)
	ON_BN_CLICKED(IDC_REG_BROWSE, OnRegBrowse)
	ON_EN_UPDATE(IDC_REG_FILENAME, OnUpdateRegFilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDialog message handlers

void CRegistrationDialog::OnRegBrowse() 
{
    // creates new dialog
    CFileDialog file_dialog( TRUE, REG_FILE_EXT, REG_DEFAULT_FILENAME, 
                             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                             REG_FILE_EXT_FILTER, NULL );

    // displays file dialog
    if ( file_dialog.DoModal() != IDOK ) {
        return;
    }

    // updates registration file name
    m_reg_filename = file_dialog.GetFileName();
    UpdateData( FALSE );

}

void CRegistrationDialog::OnUpdateRegFilename() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
    // updates registration file name
    UpdateData( TRUE );	
}
