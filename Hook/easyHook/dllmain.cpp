// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"

#include "common.h"
#include "HookModuleFunction.h"
#include <sstream>

void init(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{

}


static CEasyHook gs;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		init(hModule,ul_reason_for_call,lpReserved);
	case DLL_THREAD_ATTACH:
		init(hModule,ul_reason_for_call,lpReserved);
	case DLL_THREAD_DETACH:
		init(hModule,ul_reason_for_call,lpReserved);
	case DLL_PROCESS_DETACH:
		init(hModule,ul_reason_for_call,lpReserved);
		break;
	}
	return TRUE;
}

