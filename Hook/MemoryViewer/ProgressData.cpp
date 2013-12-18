#include "ProgressData.h"
#include <stdio.h>
#include <Tlhelp32.h>

CProgressData::CProgressData(void)
{
}

CProgressData::~CProgressData(void)
{
}

BOOL CProgressData::ReadMemory(DWORD dwProgressId, LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwSize)
{
	HANDLE hToken;
 
	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{
		return FALSE;
	}
	SetPrivilege(hToken, SE_DEBUG_NAME ,TRUE);

	HANDLE hProgress = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProgressId);
	if(hProgress)
	{
		DWORD OldProtect;
		if (VirtualProtect(lpBaseAddress, dwSize, PAGE_EXECUTE_READWRITE, &OldProtect))
		{
			BOOL bRet = ::ReadProcessMemory(hProgress, lpBaseAddress, lpBuffer, dwSize, NULL);
			VirtualProtect(lpBaseAddress, dwSize, OldProtect, NULL);
			return bRet;
		}
		else
		{
			DWORD dw = GetLastError();

			TCHAR szBuf[80]; 
			LPVOID lpMsgBuf;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

			wsprintf(szBuf, 
				"%s failed with error %d: %s", 
				"error code", dw, lpMsgBuf); 

			//MessageBox(NULL, szBuf, "Error", MB_OK); 
			OutputDebugString(szBuf);

			LocalFree(lpMsgBuf);

		}
	}
	return FALSE;
}

BOOL CProgressData::WriteMemory(DWORD dwProgressId, LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwSize)
{
	HANDLE hToken;

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{
		return FALSE;
	}
	SetPrivilege(hToken, SE_DEBUG_NAME ,TRUE);

	HANDLE hProgress = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProgressId);
	if(hProgress)
	{
		SuspendProcess(dwProgressId);
		
		BOOL bRet = FALSE;
		DWORD OldProtect;
		if (VirtualProtect(lpBaseAddress, dwSize, PAGE_EXECUTE_READWRITE, &OldProtect))
		{
			bRet = ::WriteProcessMemory(hProgress, lpBaseAddress, lpBuffer, dwSize, NULL);
			VirtualProtect(lpBaseAddress, dwSize, OldProtect, NULL);
		}

		ResumeProcess(dwProgressId);
		return bRet;
	}
	return FALSE;
}


BOOL CProgressData::SetPrivilege(
				  HANDLE hToken,          // access token handle
				  LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
				  BOOL bEnablePrivilege   // to enable or disable privilege
				  ) 
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if ( !LookupPrivilegeValue( 
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid ) )        // receives LUID of privilege
	{
		char buf[256];
		sprintf(buf, "LookupPrivilegeValue error: %u\n", GetLastError());
		OutputDebugStringA( buf); 
		return FALSE; 
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if ( !AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL) )
	{ 
		printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
		return FALSE; 
	} 

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	} 

	return TRUE;
}

BOOL CProgressData::SuspendProcess(DWORD   dwProcessID)//挂起程序，必须知道进程得PID 
{ 
	BOOL bResult = TRUE; 
	HANDLE hThread; 
	HANDLE hSnapshot; 
	THREADENTRY32 ThreadEntry32; 
	BOOL bExist; 

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessID); 
	if(hSnapshot == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}

	ThreadEntry32.dwSize = sizeof(THREADENTRY32);                     
	bExist = Thread32First(hSnapshot,&ThreadEntry32); 
	while(bExist=Thread32Next(hSnapshot,&ThreadEntry32)) 
	{ 
		if(ThreadEntry32.th32OwnerProcessID == dwProcessID) 
		{ 
			hThread = OpenThread(THREAD_SUSPEND_RESUME,FALSE,ThreadEntry32.th32ThreadID); 
			if(hThread != NULL) 
			{ 
				SuspendThread(hThread); 
				CloseHandle(hThread); 
			} 
			else 
			{ 
				CloseHandle(hSnapshot); 
				bResult = FALSE; 
			} 
		} 
	} 

	CloseHandle(hSnapshot); 

	return bResult; 
} 

BOOL   CProgressData::ResumeProcess(DWORD   dwProcessID)
{ 
	BOOL                       bResult   =   FALSE; 
	HANDLE                   hThread; 
	HANDLE                   hSnapshot; 
	THREADENTRY32     ThreadEntry32; 
	BOOL                       bExist; 

	hSnapshot   =   CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessID); 
	if(hSnapshot   !=   INVALID_HANDLE_VALUE) 
	{ 
		ThreadEntry32.dwSize   =   sizeof(THREADENTRY32);                     
		bExist   =   Thread32First(hSnapshot,&ThreadEntry32); 
		while(bExist   =   Thread32Next(hSnapshot,&ThreadEntry32)) 
		{ 
			if(ThreadEntry32.th32OwnerProcessID   ==   dwProcessID) 
			{ 
				hThread   =   OpenThread(THREAD_SUSPEND_RESUME,FALSE,ThreadEntry32.th32ThreadID); 
				if(hThread   !=   NULL) 
				{ 
					ResumeThread(hThread); 
					CloseHandle(hThread); 
					bResult   =   TRUE; 
				} 
				else 
				{ 
					CloseHandle(hSnapshot); 
				} 
			} 
		} 
	} 

	CloseHandle(hSnapshot); 

	return   bResult; 
} 
