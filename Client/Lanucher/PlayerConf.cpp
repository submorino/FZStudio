#include "StdAfx.h"
#include "PlayerConf.h"


CPlayerConf::CPlayerConf()
: CBaseConf()
{
	LoadConf();
}

CPlayerConf::CPlayerConf(const std::string& filepath)
: CBaseConf(filepath)
{
	LoadConf();
}

CPlayerConf::~CPlayerConf()
{
	std::vector<CPlayer*>::iterator iter = m_playerList.begin();
	for ( ; iter!=m_playerList.end() ; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_playerList.clear();
}

bool CPlayerConf::LoadConf()
{
	for (int i=100; i<110; ++i)
	{
		CPlayer *player = new CPlayer;
		char buf[16] = {0};
		sprintf(buf, "player-%03d", i);
		player->setPlayerName(buf);
		player->SetPlayerId(i);
		player->setPlayerPassword("pw123456");
		m_playerList.push_back(player);
	}
	return true;
}