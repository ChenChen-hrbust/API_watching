// KBrecorder.h : main header file for the KBRECORDER application
//

#if !defined(AFX_KBRECORDER_H__D56DF64B_F0BA_4665_9770_00140F6C8AA8__INCLUDED_)
#define AFX_KBRECORDER_H__D56DF64B_F0BA_4665_9770_00140F6C8AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKBrecorderApp:
// See KBrecorder.cpp for the implementation of this class
//

class CKBrecorderApp : public CWinApp
{
public:
	CKBrecorderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKBrecorderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKBrecorderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KBRECORDER_H__D56DF64B_F0BA_4665_9770_00140F6C8AA8__INCLUDED_)
