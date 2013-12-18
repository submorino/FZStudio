#include "InstallAPIHook.h"

static void InstallAPIHooks()
{
	HookAPI("user32.dll", "SetWindowsHookExA", (PROC)SetWindowsHookExACallbackProc, (PROC*)&SetWindowsHookExANextHook);
	HookAPI("user32.dll", "SetWindowsHookExW", (PROC)SetWindowsHookExWCallbackProc, (PROC*)&SetWindowsHookExWNextHook);

}

static void UninstallAPIHooks()
{
	if (SetWindowsHookExANextHook)
		HookAPI("user32.dll", "SetWindowsHookExA", (PROC)SetWindowsHookExANextHook, NULL);
	if (SetWindowsHookExWNextHook)
		HookAPI("user32.dll", "SetWindowsHookExW", (PROC)SetWindowsHookExWNextHook, NULL);
}