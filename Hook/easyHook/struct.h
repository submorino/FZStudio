#pragma once
#include <windows.h>

typedef HHOOK (WINAPI * 
			   Real_SetWindowsHookExW)(
			   __in int idHook,
			   __in HOOKPROC lpfn,
			   __in_opt HINSTANCE hmod,
			   __in DWORD dwThreadId);

typedef NTSTATUS (NTAPI *
				  Real_ZwOpenProcess)(
				  OUT PHANDLE ProcessHandle,
				  IN ACCESS_MASK DesiredAccess,
				  IN void* ObjectAttributes,
				  IN void* ClientId OPTIONAL);