// easyHook.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include <windows.h>
#include <Psapi.h>
#include "common.h"
#include "easyHook.h"
#include "HookModuleFunction.h"


EASYHOOK_API void ActivateAntiHook(int activate)
{
	if (activate)
	{
		if (CEasyHook::g_hHookCBT != NULL)	return;

		//��װHOOK��ģ���õľ����DLLģ�������̲߳���Ϊ0��ʾȫ��
		CEasyHook::g_hHookCBT = ::SetWindowsHookEx(WH_CBT, &CEasyHook::HCBTProc, GetModuleHandle(TEXT("easyHook.dll")), 0);
	}
	else
	{
		if (CEasyHook::g_hHookCBT == NULL)	return;

		UnhookWindowsHookEx(CEasyHook::g_hHookCBT);
	}
}


//HMODULE
//WINAPI
//fakeLoadLibraryW(
//    __in LPCWSTR lpLibFileName
//    )
//{
//}

EASYHOOK_API void Inject()
{
	OutputDebugStringA("Inject");
	/*DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());	
	
	ZwOpenProcess = (Real_ZwOpenProcess)DetourFindFunction("ntdll.dll","LoadLibrary");
	//InterlockedExchange((LONG*)&CEasyHook::bHook,true);
	Trace(L"Inject",DetourAttach(&(PVOID&)RealSetWindowsHookExW, &CEasyHook::DetourSetWindowsHookEx));
	Trace(L"Inject",DetourAttach(&(PVOID&)ZwOpenProcess,&CEasyHook::DetourZwOpenProcess));
	DetourTransactionCommit();*/
}

EASYHOOK_API void Eject()
{
}
