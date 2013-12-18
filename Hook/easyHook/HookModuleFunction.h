#pragma once

#include "common.h"

class CEasyHook {
public:
	static bool bHook;
	static HHOOK g_hHookCBT;
public:
	CEasyHook(void);
	static bool IsExclude();
	static LRESULT CALLBACK HCBTProc(int nCode,WPARAM wParam,LPARAM lParam);
	static HHOOK CALLBACK DetourSetWindowsHookEx(__in int idHook,
		__in HOOKPROC lpfn,
		__in_opt HINSTANCE hmod,
		__in DWORD dwThreadId);
	static NTSTATUS NTAPI DetourZwOpenProcess(
		OUT PHANDLE ProcessHandle,
		IN ACCESS_MASK DesiredAccess,
		IN void* ObjectAttributes,
		IN void* ClientId OPTIONAL);
};