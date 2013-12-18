#pragma once

#include "LogonServerConf.h"
#include "GameServerConf.h"
#include "PlayerConf.h"


class CConfigMgr
{
	CConfigMgr();
public:
	~CConfigMgr();

public:
	static CConfigMgr* getInstance() { 
		return m_pConfigMgr != NULL ? m_pConfigMgr : m_pConfigMgr = new CConfigMgr; 
	}
	
	CLogonServerConf* getLogonServerConf() { return m_pLogonServer; }
	CGameServerConf*  getGameServerConf()  { return m_pGameServer;  }
	CPlayerConf*	  getPlayerConf()	   { return m_pPlayerConf; }


private:
	static CConfigMgr *m_pConfigMgr;

private:
	CLogonServerConf *m_pLogonServer;
	CGameServerConf	 *m_pGameServer;
	CPlayerConf		 *m_pPlayerConf;
};

#define CONFIGMGR	CConfigMgr::getInstance()