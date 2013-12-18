#include "log.h"
#include "Psapi.h"
#include "HookModuleFunction.h"

void DbgPrint(WCHAR* FunctionName)
{
	WCHAR wzExeName[MAX_PATH];
	WCHAR wzExeNameHook[MAX_PATH];
	GetModuleBaseName(GetCurrentProcess(),NULL,wzExeName,ARRAYOF(wzExeName));
	wsprintf(wzExeNameHook,L"call %s %s bHook %d\n",FunctionName,wzExeName,CEasyHook::bHook);
	OutputDebugStringW(wzExeNameHook);
}

void Trace(WCHAR* FunctionName,LONG result)
{
	WCHAR wzExeName[MAX_PATH];
	WCHAR message[MAX_PATH];
	GetModuleBaseName(GetCurrentProcess(),NULL,wzExeName,ARRAYOF(wzExeName));
	
	switch(result)
	{
	case NO_ERROR:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"NO_ERROR\n!");
		OutputDebugStringW(message);
		break;
	case ERROR_INVALID_BLOCK:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"ERROR_INVALID_BLOCK\n!");
		OutputDebugStringW(message);
		break;
	case ERROR_INVALID_HANDLE:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"ERROR_INVALID_HANDLE\n!");
		OutputDebugStringW(message);
		break;
	case ERROR_INVALID_OPERATION:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"ERROR_INVALID_OPERATION\n!");
		OutputDebugStringW(message);
		break;
	case ERROR_NOT_ENOUGH_MEMORY:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"ERROR_NOT_ENOUGH_MEMORY\n!");
		OutputDebugStringW(message);
		break;
	case ERROR_INVALID_DATA :
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"ERROR_INVALID_DATA\n!");
		OutputDebugStringW(message);
		break;
	default:
		wsprintf(message,L"call %s %s bHook %d result:%s\n",FunctionName,wzExeName,CEasyHook::bHook,L"Other Codes\n!");
		OutputDebugStringW(message);
		break;
	}
}