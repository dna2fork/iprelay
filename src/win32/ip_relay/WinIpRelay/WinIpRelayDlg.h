// WinIpRelayDlg.h : header file
//

#if !defined(AFX_WINIPRELAYDLG_H__8BE6EB63_68F9_4942_B1D9_C051364744DD__INCLUDED_)
#define AFX_WINIPRELAYDLG_H__8BE6EB63_68F9_4942_B1D9_C051364744DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// relay node list 
typedef struct _WIPR_relay_node WIPR_relay_node;

/////////////////////////////////////////////////////////////////////////////
// CWinIpRelayDlg dialog

class CWinIpRelayDlg : public CDialog
{
// Construction
public:
	CWinIpRelayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWinIpRelayDlg)
	enum { IDD = IDD_WINIPRELAY_DIALOG };
	CRichEditCtrl	m_logs_ctrl;
	CListBox	m_relay_list;
	CString	m_config_name;
	CString	m_logs;
	CString	m_copyright;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinIpRelayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinIpRelayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddRelay();
	afx_msg void OnDeleteRelay();
	afx_msg void OnSaveLogs();
	afx_msg void OnClearLogs();
	afx_msg void OnLoadConfig();
	afx_msg void OnSaveConfig();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnAboutUs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    BOOL            m_config_changed;   // config changed indicator
    long            m_logs_size;        // logs string size        
    HANDLE          m_logs_mutex;       // m_logs mutex
    HANDLE          m_relay_head_mutex; // m_relay_head mutex
    WIPR_relay_node *m_relay_head;      // relay list head node 

    char *AddRelay( UINT local_port, CString& remote_host, UINT remote_port, long timeout );
    BOOL DeleteRelay( const char *display_name );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINIPRELAYDLG_H__8BE6EB63_68F9_4942_B1D9_C051364744DD__INCLUDED_)
