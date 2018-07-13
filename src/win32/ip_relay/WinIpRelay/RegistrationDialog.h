#if !defined(AFX_REGISTRATIONDIALOG_H__E6C266E7_B468_412D_A44D_27693B603822__INCLUDED_)
#define AFX_REGISTRATIONDIALOG_H__E6C266E7_B468_412D_A44D_27693B603822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegistrationDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDialog dialog

class CRegistrationDialog : public CDialog
{
// Construction
public:
	CRegistrationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegistrationDialog)
	enum { IDD = IDD_REGISTRATION_DIALOG };
	CString	m_reg_filename;
	CString	m_reg_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegistrationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegistrationDialog)
	afx_msg void OnRegBrowse();
	afx_msg void OnUpdateRegFilename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTRATIONDIALOG_H__E6C266E7_B468_412D_A44D_27693B603822__INCLUDED_)
