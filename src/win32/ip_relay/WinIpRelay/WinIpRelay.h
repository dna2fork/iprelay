// WinIpRelay.h : main header file for the WINIPRELAY application
//

#if !defined(AFX_WINIPRELAY_H__96324E72_3842_45DE_A10B_019B44804288__INCLUDED_)
#define AFX_WINIPRELAY_H__96324E72_3842_45DE_A10B_019B44804288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinIpRelayApp:
// See WinIpRelay.cpp for the implementation of this class
//

class CWinIpRelayApp : public CWinApp
{
public:
	CWinIpRelayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinIpRelayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinIpRelayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINIPRELAY_H__96324E72_3842_45DE_A10B_019B44804288__INCLUDED_)
