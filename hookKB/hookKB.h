// hookKB.h : main header file for the HOOKKB DLL
//

#if !defined(AFX_HOOKKB_H__1689D17C_017F_4315_822F_8E9460F5ABB3__INCLUDED_)
#define AFX_HOOKKB_H__1689D17C_017F_4315_822F_8E9460F5ABB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHookKBApp
// See hookKB.cpp for the implementation of this class
//

class CHookKBApp : public CWinApp
{
public:
  CHookKBApp();
   LRESULT __declspec(dllexport)__stdcall CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam);
   BOOL __declspec(dllexport)__stdcall installhook();
 //  BOOL __declspec(dllexport) UnHook();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHookKBApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHookKBApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKKB_H__1689D17C_017F_4315_822F_8E9460F5ABB3__INCLUDED_)
