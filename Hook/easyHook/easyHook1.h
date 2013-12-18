// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EASYHOOK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EASYHOOK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef EASYHOOK_EXPORTS
#define EASYHOOK_API __declspec(dllexport)
#else
#define EASYHOOK_API __declspec(dllimport)
#endif
// This class is exported from the easyHook.dll

extern "C" {
	EASYHOOK_API void ActivateAntiHook(int activate);

	EASYHOOK_API void Inject();	
	EASYHOOK_API void Eject();
}
