// Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#include "detours.h"
#pragma comment(lib, "detours.lib")

#define		TYPE_RegCreateKeyExA	0
#define		TYPE_RegCreateKeyExW	1
#define		TYPE_RegSetValueExA		2
#define		TYPE_RegSetValueExW		3
#define		TYPE_RegDeleteKeyA		4
#define		TYPE_RegDeleteKeyW		5
#define		TYPE_RegDeleteValueA	6
#define		TYPE_RegDeleteValueW	7
#define		TYPE_RegQueryValueExA	8
#define		TYPE_RegQueryValueExW	9

#define		WM_MonPIDInvalid		WM_USER + 5642


#pragma comment(linker, "/OPT:nowin98")

#pragma data_seg(".Shard")
HWND	hWnd = NULL;
HHOOK	hhook = NULL;
DWORD	PID = 0;
DWORD	ExplorerPID = 0;
DWORD	RegMonPID = 0;
BOOL	HOOK_RegCreateKeyEx = TRUE;
BOOL	HOOK_RegSetValueEx = TRUE;
BOOL	HOOK_RegDeleteKey = TRUE;
BOOL	HOOK_RegDeleteValue = TRUE;
BOOL	HOOK_RegQueryValueEx = TRUE;

#pragma data_seg()
#pragma comment(linker,"/section:.Shard,rws")

bool bTryed = false;
bool bIntercepted = false;


struct PARAMS
{
	DWORD	PID;
	HKEY	hKey;
	char	buffer1[1024];
	int		cbBuffer1;
	char	buffer2[1024];
	int		cbBuffer2;
	DWORD	type;
	long	result;
};




DETOUR_TRAMPOLINE(LONG WINAPI Real_RegCreateKeyExA(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD), RegCreateKeyExA);
LONG WINAPI Mine_RegCreateKeyExA(
								HKEY hKey,
								LPCSTR lpSubKey,
								DWORD Reserved,
								LPSTR lpClass,
								DWORD dwOptions,
								REGSAM samDesired,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								PHKEY phkResult,
								LPDWORD lpdwDisposition
							  )
{
	LONG nRet;
	__try{
		nRet = Real_RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
		if(!HOOK_RegCreateKeyEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpSubKey, strlen(lpSubKey));
			//memcpy(params.buffer2, lpClass, 1024);
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegCreateKeyExA;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegCreateKeyExW(HKEY, LPCWSTR, DWORD, LPWSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD), RegCreateKeyExW);
LONG WINAPI Mine_RegCreateKeyExW(
								 HKEY hKey,
								 LPCWSTR lpSubKey,
								 DWORD Reserved,
								 LPWSTR lpClass,
								 DWORD dwOptions,
								 REGSAM samDesired,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 PHKEY phkResult,
								 LPDWORD lpdwDisposition
								 )
{
	LONG nRet;
	__try{
		nRet = Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
		if(!HOOK_RegCreateKeyEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpSubKey, wcslen(lpSubKey)*2);
			//memcpy(params.buffer2, lpClass, 1024);
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegCreateKeyExW;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegSetValueExA(HKEY, LPCSTR, DWORD, DWORD, CONST BYTE*, DWORD), RegSetValueExA);
LONG WINAPI Mine_RegSetValueExA(
								HKEY hKey,
								LPCSTR lpValueName,
								DWORD Reserved,
								DWORD dwType,
								CONST BYTE* lpData,
								DWORD cbData
								)
{
	LONG nRet;
	__try{
		nRet = Real_RegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
		if(!HOOK_RegSetValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL || lpData == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpValueName, strlen(lpValueName));
			params.type = dwType;
			if(cbData>1024)
			{
				memcpy(params.buffer2, lpData, 1024);
				params.cbBuffer2 = 1024;
			}
			else
			{
				memcpy(params.buffer2, lpData, cbData);
				params.cbBuffer2 = cbData;
			}
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegSetValueExA;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegSetValueExW(HKEY, LPCWSTR, DWORD, DWORD, CONST BYTE*, DWORD), RegSetValueExW);
LONG WINAPI Mine_RegSetValueExW(
								HKEY hKey,
								LPCWSTR lpValueName,
								DWORD Reserved,
								DWORD dwType,
								CONST BYTE* lpData,
								DWORD cbData
								)
{
	LONG nRet;
	__try{
		nRet = Real_RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
		if(!HOOK_RegSetValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL || lpData == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpValueName, wcslen(lpValueName)*2);
			params.type = dwType;
			if(cbData>1024)
			{
				memcpy(params.buffer2, lpData, 1024);
				params.cbBuffer2 = 1024;
			}
			else
			{
				memcpy(params.buffer2, lpData, cbData);
				params.cbBuffer2 = cbData;
			}
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegSetValueExW;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}

DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteKeyA(HKEY, LPCSTR), RegDeleteKeyA);
LONG WINAPI Mine_RegDeleteKeyA(
								HKEY hKey,
								LPCSTR lpSubKey
								)
{
	LONG nRet;
	__try{
		nRet = Real_RegDeleteKeyA(hKey, lpSubKey);
		if(!HOOK_RegDeleteKey)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpSubKey, strlen(lpSubKey));
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegDeleteKeyA;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}
DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteKeyW(HKEY, LPCWSTR), RegDeleteKeyW);
LONG WINAPI Mine_RegDeleteKeyW(
							   HKEY hKey,
							   LPCWSTR lpSubKey
							   )
{
	LONG nRet;
	__try{
		nRet = Real_RegDeleteKeyW(hKey, lpSubKey);
		if(!HOOK_RegDeleteKey)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpSubKey == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpSubKey, wcslen(lpSubKey)*2);
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegDeleteKeyW;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}


DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteValueA(HKEY, LPCSTR), RegDeleteValueA);
LONG WINAPI Mine_RegDeleteValueA(
							   HKEY hKey,
							   LPCSTR lpValueName
							   )
{
	LONG nRet;
	__try{
		nRet = Real_RegDeleteValueA(hKey, lpValueName);
		if(!HOOK_RegDeleteValue)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpValueName, strlen(lpValueName));
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegDeleteValueA;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}
DETOUR_TRAMPOLINE(LONG WINAPI Real_RegDeleteValueW(HKEY, LPCWSTR), RegDeleteValueW);
LONG WINAPI Mine_RegDeleteValueW(
								 HKEY hKey,
								 LPCWSTR lpValueName
								 )
{
	LONG nRet;
	__try{
		nRet = Real_RegDeleteValueW(hKey, lpValueName);
		if(!HOOK_RegDeleteValue)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(lpValueName == NULL)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			memcpy(params.buffer1, lpValueName, wcslen(lpValueName)*2);
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegDeleteValueW;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
	}__finally{
	};
	return nRet;
}




DETOUR_TRAMPOLINE(LONG WINAPI Real_RegQueryValueExA(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD), RegQueryValueExA);
LONG WINAPI Mine_RegQueryValueExA(
				  HKEY hKey,
				  LPCSTR lpValueName,
				  LPDWORD lpReserved,
				  LPDWORD lpType,
				  LPBYTE lpData,
				  LPDWORD lpcbData)
{
	LONG nRet;
	__try{
		nRet = Real_RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
		if(!HOOK_RegQueryValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(nRet != ERROR_SUCCESS)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;

			
			if(lpValueName == NULL || /*lpType == NULL ||*/ lpData == NULL || lpcbData == NULL)
				return nRet;
			
			memcpy(params.buffer1, lpValueName, strlen(lpValueName));
			
			if(lpType == NULL)
				params.type = 0;
			else
				params.type = *lpType;
			
			if(*lpcbData == 0)
			{
				return nRet;
			}
			else if(*lpcbData>1024)
			{
				memcpy(params.buffer2, lpData, 1024);
				params.cbBuffer2 = 1024;
			}
			else
			{
				memcpy(params.buffer2, lpData, *lpcbData);
				params.cbBuffer2 = *lpcbData;
			}
			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegQueryValueExA;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
		
	}__finally{
	};
	return nRet;
}
DETOUR_TRAMPOLINE(LONG APIENTRY Real_RegQueryValueExW(HKEY, LPCWSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD), RegQueryValueExW);
LONG APIENTRY Mine_RegQueryValueExW(
								  HKEY hKey,
								  LPCWSTR lpValueName,
								  LPDWORD lpReserved,
								  LPDWORD lpType,
								  LPBYTE lpData,
								  LPDWORD lpcbData)
{
	LONG nRet;
	__try{
		nRet = Real_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
		if(!HOOK_RegQueryValueEx)
			return nRet;
		if(GetCurrentProcessId() == ExplorerPID)
			return nRet;
		if(GetCurrentProcessId() == RegMonPID)
			return nRet;
		if((PID == 0) || ((GetCurrentProcessId() == PID) && (PID>0)) )
		{
			if(nRet != ERROR_SUCCESS)
				return nRet;
			COPYDATASTRUCT data;
			PARAMS params = {0};
			params.PID = GetCurrentProcessId();
			params.hKey = hKey;
			
			if(lpValueName == NULL || /*lpType == NULL ||*/ lpData == NULL || lpcbData == NULL)
				return nRet;
			
			memcpy(params.buffer1, lpValueName, wcslen(lpValueName)*2);
			
			if(lpType == NULL)
				params.type = 0;
			else
				params.type = *lpType;
			
			if(*lpcbData == 0)
			{
				return nRet;
			}
			else if(*lpcbData>1024)
			{
				memcpy(params.buffer2, lpData, 1024);
				params.cbBuffer2 = 1024;
			}
			else
			{
				memcpy(params.buffer2, lpData, *lpcbData);
				params.cbBuffer2 = *lpcbData;
			}

			params.result = nRet;
			
			data.cbData = sizeof(PARAMS);
			data.lpData = &params;
			data.dwData = TYPE_RegQueryValueExW;
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&data);
		}
		
	}__finally{
	};
	return nRet;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Real_PostQuitMessage(UINT), PostQuitMessage);
BOOL WINAPI Mine_PostQuitMessage(UINT uExitCode)
{
	BOOL nRet;
	__try{
		if(GetCurrentProcessId() == PID)
		{
			PostMessage(hWnd, WM_MonPIDInvalid, 0, 0);
			PID = 0;
		}
		nRet = Real_PostQuitMessage(uExitCode);
	}__finally{
	};
	return nRet;
}




// *************************************************************************************
void Intercept()
{
	DetourFunctionWithTrampoline((PBYTE)Real_RegCreateKeyExA, (PBYTE)Mine_RegCreateKeyExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegCreateKeyExW, (PBYTE)Mine_RegCreateKeyExW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegSetValueExA, (PBYTE)Mine_RegSetValueExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegSetValueExW, (PBYTE)Mine_RegSetValueExW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteKeyA, (PBYTE)Mine_RegDeleteKeyA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteKeyW, (PBYTE)Mine_RegDeleteKeyW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteValueA, (PBYTE)Mine_RegDeleteValueA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegDeleteValueW, (PBYTE)Mine_RegDeleteValueW);
	DetourFunctionWithTrampoline((PBYTE)Real_RegQueryValueExA, (PBYTE)Mine_RegQueryValueExA);
	DetourFunctionWithTrampoline((PBYTE)Real_RegQueryValueExW, (PBYTE)Mine_RegQueryValueExW);
	DetourFunctionWithTrampoline((PBYTE)Real_PostQuitMessage, (PBYTE)Mine_PostQuitMessage);
}

void UnIntercept()
{
	DetourRemove((PBYTE)Real_RegCreateKeyExA,(PBYTE)Mine_RegCreateKeyExA);
	DetourRemove((PBYTE)Real_RegCreateKeyExW,(PBYTE)Mine_RegCreateKeyExW);
	DetourRemove((PBYTE)Real_RegSetValueExA,(PBYTE)Mine_RegSetValueExA);
	DetourRemove((PBYTE)Real_RegSetValueExW,(PBYTE)Mine_RegSetValueExW);
	DetourRemove((PBYTE)Real_RegDeleteKeyA, (PBYTE)Mine_RegDeleteKeyA);
	DetourRemove((PBYTE)Real_RegDeleteKeyW, (PBYTE)Mine_RegDeleteKeyW);
	DetourRemove((PBYTE)Real_RegDeleteValueA, (PBYTE)Mine_RegDeleteValueA);
	DetourRemove((PBYTE)Real_RegDeleteValueW, (PBYTE)Mine_RegDeleteValueW);
	DetourRemove((PBYTE)Real_RegQueryValueExA, (PBYTE)Mine_RegQueryValueExA);
	DetourRemove((PBYTE)Real_RegQueryValueExW, (PBYTE)Mine_RegQueryValueExW);
	DetourRemove((PBYTE)Real_PostQuitMessage, (PBYTE)Mine_PostQuitMessage);
}
// *************************************************************************************


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:			
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{		
			if ( bIntercepted ){
				bIntercepted = false;
				UnIntercept();
			}				
		}
		break;
    }
    return TRUE;
}

LRESULT CALLBACK MyShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{	
	if ( bTryed == false ){
		bTryed = true;
		Intercept();
		bIntercepted = true;
	}
	return ::CallNextHookEx( hhook, nCode, wParam, lParam );
}

extern "C" __declspec(dllexport) BOOL InstallHook(HWND handle)
{
	hhook = ::SetWindowsHookEx(WH_SHELL, MyShellProc, GetModuleHandle("hook.dll"), 0);
	hWnd = handle;
	RegMonPID = GetCurrentProcessId();
	return hhook > 0;
}

extern "C" __declspec(dllexport) BOOL UninstallHook()
{
	return ::UnhookWindowsHookEx(hhook);
}

extern "C" __declspec(dllexport) void SetFilter(DWORD pid)
{
	PID = pid;
}

extern "C" __declspec(dllexport) void SetHookFunction(BOOL RegCreateKeyEx, BOOL RegSetValueEx, BOOL RegDeleteKey, BOOL RegDeleteValue, BOOL RegQueryValueEx)
{
	HOOK_RegCreateKeyEx = RegCreateKeyEx;
	HOOK_RegSetValueEx = RegSetValueEx;
	HOOK_RegDeleteKey = RegDeleteKey;
	HOOK_RegDeleteValue = RegDeleteValue;
	HOOK_RegQueryValueEx = RegQueryValueEx;
}

extern "C" __declspec(dllexport) void SetExplorerPID(DWORD pid)
{
	ExplorerPID = pid;
}
