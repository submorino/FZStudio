#pragma once
#include "Player.h"
#include "BaseConf.h"

class CPlayerConf : public CBaseConf
{
public:
	CPlayerConf();
	CPlayerConf(const std::string& filepath);
	virtual ~CPlayerConf();

public:
	std::vector<CPlayer*>* GetPlayerList() { return &m_playerList; }

protected:
	virtual bool LoadConf();

private:
	std::vector<CPlayer*> m_playerList;
};
