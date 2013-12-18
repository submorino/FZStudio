#include "HookModuleFunction.h"
#include "include\detours.h"
#include "Psapi.h"
#include "common.h"
#include "log.h"
#include "struct.h"

bool CEasyHook::bHook = false;
HHOOK CEasyHook::g_hHookCBT = NULL;

Real_ZwOpenProcess ZwOpenProcess = NULL;
Real_SetWindowsHookExW RealSetWindowsHookExW = NULL;

CEasyHook::CEasyHook()
{
}

LRESULT CALLBACK CEasyHook::HCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//当其它程序启动时，预计会先有这个通知，而不是等到调用完SetWindowsHookEx才通知
	__try{
		if (!CEasyHook::bHook) 
		{
			if(!IsExclude())
			{
				WCHAR wzExeName[MAX_PATH];
				WCHAR wzExeNameHook[MAX_PATH];
				GetModuleBaseName(GetCurrentProcess(),NULL,wzExeName,ARRAYOF(wzExeName));
				wsprintf(wzExeNameHook,L"%s bHook %d\n",wzExeName,CEasyHook::bHook);
				OutputDebugStringW(wzExeNameHook);

				Trace(L"DetourTransactionBegin",DetourTransactionBegin());
				Trace(L"DetourUpdateThread",DetourUpdateThread(GetCurrentThread()));	
				RealSetWindowsHookExW = (Real_SetWindowsHookExW)DetourFindFunction("user32.dll","SetWindowsHookExW");
				ZwOpenProcess = (Real_ZwOpenProcess)DetourFindFunction("ntdll.dll","ZwOpenProcess");

				InterlockedExchange((LONG*)&CEasyHook::bHook,true);
				Trace(L"DetourAttach",DetourAttach(&(PVOID&)RealSetWindowsHookExW, &CEasyHook::DetourSetWindowsHookEx));
				Trace(L"DetourAttach",DetourAttach(&(PVOID&)ZwOpenProcess,&CEasyHook::DetourZwOpenProcess));

				Trace(L"DetourTransactionCommit",DetourTransactionCommit());
			}
		}

	}__except(1)
	{
		OutputDebugStringA("HCBTProc __except \n");
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

NTSTATUS NTAPI CEasyHook::DetourZwOpenProcess(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN void* ObjectAttributes,
	IN void* ClientId OPTIONAL)
{
	Real_ZwOpenProcess ZwOpenProcess = NULL;
	NTSTATUS ntats = 0;
	__try{
		ntats = (*ZwOpenProcess)(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);
		DbgPrint(L"DetourZwOpenProcess - ZwOpenProcess");
	}
	__except(1){
	}

	return ntats;
}

HHOOK CALLBACK CEasyHook::DetourSetWindowsHookEx(__in int idHook,
												 __in HOOKPROC lpfn,
												 __in_opt HINSTANCE hmod,
												 __in DWORD dwThreadId)
{
	HHOOK iHook = NULL;
	__try{
		//if(IsExclude()){
		//	//修改HOOKPROC lpfn 参数
		//	DbgPrint(L"my DetourSetWindowsHookEx - SetWindowsHookExW");
		//	return(*RealSetWindowsHookExW)(idHook,lpfn, hmod,dwThreadId);
		//}

		DbgPrint(L"kill DetourSetWindowsHookEx - SetWindowsHookExW");
		return (*RealSetWindowsHookExW)(idHook,lpfn,hmod,dwThreadId);

	}__except(1)
	{
	}

	return iHook;
}

bool CEasyHook::IsExclude()
{
	WCHAR wzExeName[MAX_PATH];
	//过滤一下自己的进程
	GetModuleBaseName(GetCurrentProcess(),NULL,wzExeName,ARRAYOF(wzExeName));

	if(_wcsicmp(wzExeName, L"DK.exe") == 0 || _wcsicmp(wzExeName, L"devenv.exe") == 0
		|| _wcsicmp(wzExeName, L"Dbgview.exe") == 0){
			return true;
	}
	return false;
}

