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

		//安装HOOK，模块用的句柄是DLL模块句柄，线程参数为0表示全局
		CEasyHook::g_hHookCBT = ::SetWindowsHookEx(WH_CBT, &CEasyHook::HCBTProc, GetModuleHandle(TEXT("easyHook.dll")), 0);
	}
	else
	{
		if (CEasyHook::g_hHookCBT == NULL)	return;

		UnhookWindowsHookEx(CEasyHook::g_hHookCBT);
	}
}
