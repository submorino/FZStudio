#ifndef _HookImportFunction_H_
#define _HookImportFunction_H_

#include <windows.h>


BOOL HookAPI(LPCTSTR szImportModule, LPCTSTR szFunc, PROC paHookFuncs, PROC* paOrigFuncs);

#endif
