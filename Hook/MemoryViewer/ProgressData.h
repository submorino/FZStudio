#pragma once
#include "windows.h"

class CProgressData
{
public:
	CProgressData(void);
	~CProgressData(void);

public:
	static BOOL ReadMemory(DWORD dwProgressId, LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwSize);
	static BOOL WriteMemory(DWORD dwProgressId, LPVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwSize);
	static BOOL SetPrivilege(
				  HANDLE hToken,          // access token handle
				  LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
				  BOOL bEnablePrivilege   // to enable or disable privilege
				  );

	
	static BOOL CProgressData::SuspendProcess(DWORD   dwProcessID);
	static BOOL CProgressData::ResumeProcess(DWORD   dwProcessID);


};
