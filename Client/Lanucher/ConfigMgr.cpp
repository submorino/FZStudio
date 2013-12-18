#include "StdAfx.h"
#include "Common.h"
#include "ConfigMgr.h"

CConfigMgr * CConfigMgr::m_pConfigMgr = NULL;
CConfigMgr::CConfigMgr()
: m_pLogonServer(NULL)
, m_pGameServer(NULL)
, m_pPlayerConf(NULL)
{
	std::string strAppRunPath; 
	GetAppRunDir(strAppRunPath);
	std::string filepath = strAppRunPath + "\\..\\config\\server.ini";
	m_pLogonServer = new CLogonServerConf(filepath);

	filepath = strAppRunPath + "\\..\\config\\ServerTree.xml";
	m_pGameServer = new CGameServerConf(filepath);

	m_pPlayerConf = new CPlayerConf();
}

CConfigMgr::~CConfigMgr()
{
	SAFE_DELETE(m_pLogonServer);
	SAFE_DELETE(m_pGameServer);
	SAFE_DELETE(m_pPlayerConf);

	m_pConfigMgr = NULL;
}

