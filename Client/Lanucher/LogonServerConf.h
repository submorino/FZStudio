#pragma once

#include "BaseConf.h"
#include "MyStruct.h"

class CLogonServerConf : CBaseConf
{
public:
	CLogonServerConf();
	CLogonServerConf(const std::string filepath);
	virtual ~CLogonServerConf();

protected:
	virtual bool LoadConf();

public:
	std::vector<Logon_Server*>* GetLogonServerList();
	
	bool GetLogonServer(std::string servername, Logon_Server& server);

private:
	std::vector<Logon_Server*> m_serverlist;
};
