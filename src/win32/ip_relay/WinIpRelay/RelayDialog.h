#if !defined(AFX_RELAYDIALOG_H__F416FF6A_82A8_49E7_B1AA_7A146CCB5776__INCLUDED_)
#define AFX_RELAYDIALOG_H__F416FF6A_82A8_49E7_B1AA_7A146CCB5776__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RelayDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRelayDialog dialog

class CRelayDialog : public CDialog
{
// Construction
public:
	CRelayDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRelayDialog)
	enum { IDD = IDD_ADD_RELAY_DIALOG };
	UINT	m_local_port;
	CString	m_remote_host;
	UINT	m_remote_port;
	long	m_timeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRelayDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRelayDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELAYDIALOG_H__F416FF6A_82A8_49E7_B1AA_7A146CCB5776__INCLUDED_)
