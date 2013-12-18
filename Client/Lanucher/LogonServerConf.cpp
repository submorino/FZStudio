#include "StdAfx.h"
#include "LogonServerConf.h"


CLogonServerConf::CLogonServerConf()
: CBaseConf()
{
}

CLogonServerConf::CLogonServerConf(const std::string filepath)
: CBaseConf(filepath)
{
	LoadConf();
}

CLogonServerConf::~CLogonServerConf()
{
	try
	{
		// 释放游戏列表内存
		std::vector<Logon_Server*>::iterator iter = m_serverlist.begin();
		for ( ; iter!=m_serverlist.end(); iter++)
		{
			if (*iter != NULL)
			{
				delete *iter;
				*iter = NULL;
			}
		}
		m_serverlist.clear();
	}
	catch(...)
	{
	}
}

bool CLogonServerConf::LoadConf()
{
	FILE *pFile;
	if ((pFile = _tfopen(m_filepath.c_str(), _T("r"))) == NULL)	return false;
	fclose(pFile);

	TCHAR buffer[4096];
	TCHAR str[65];
	int nRet = GetPrivateProfileSection(_T("loginServer"), buffer,  _countof(buffer), m_filepath.c_str());
	if (nRet == 0)	return false;

	TCHAR keyValue[4096] = {0};
	TCHAR seps[] = _T("=:");
	TCHAR *token;
	int j = 0;
	for (int i=0; i<nRet; ++i)
	{
		keyValue[j++] = buffer[i];
		if (buffer[i] == '\0')
		{
			Logon_Server *server = new Logon_Server;

			token = _tcstok( keyValue, seps );
			server->name = token!=NULL ? token : _T("");

			token = _tcstok( NULL, seps );
			server->ip = token!=NULL ? token : _T("");

			token = _tcstok( NULL, seps );
			server->port = token!=NULL ? _ttoi(token) : 0;

			m_serverlist.push_back(server);

			memset(keyValue, 0,  sizeof(keyValue)/sizeof(keyValue[0]));
			j = 0;
		}
	}

	return true;
}

std::vector<Logon_Server*>* CLogonServerConf::GetLogonServerList()
{
	return &m_serverlist;
}

bool CLogonServerConf::GetLogonServer(std::string servername, Logon_Server& server)
{
	std::vector<Logon_Server*>::iterator iter = m_serverlist.begin();
	for ( ; iter!=m_serverlist.end(); iter++)
	{
		if (*iter != NULL && (*iter)->name.compare(servername) == 0)
		{
			server = *(*iter);
			return true;
		}
	}
	return false;
}