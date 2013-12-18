#pragma once

#include "BaseConf.h"
#include "MyStruct.h"

class CGameServerConf : public CBaseConf
{
public:
	CGameServerConf();
	CGameServerConf(const std::string& filepath);

	virtual ~CGameServerConf();

public:
	bool GetGameServer(ULONG gameid, Game_Server& gameserver);
	std::vector<Game_Item*>* GetGameList();

protected:
	virtual bool LoadConf();

private:
	GameServer_List m_gameserver_list;
	Game_List m_game_list;
};
