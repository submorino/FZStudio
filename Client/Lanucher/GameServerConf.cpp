#include "StdAfx.h"
#include "GameServerConf.h"
#include "../TinyXML/tinyxml.h"

CGameServerConf::CGameServerConf()
: CBaseConf()
{
}

CGameServerConf::CGameServerConf(const std::string& filepath)
: CBaseConf(filepath)
{
	LoadConf();
}

CGameServerConf::~CGameServerConf()
{
	try
	{
		// 释放游戏列表内存
		Game_List::iterator iterGameList = m_game_list.begin();
		for ( ; iterGameList!=m_game_list.end(); iterGameList++)
		{
			if (*iterGameList != NULL)
			{
				delete *iterGameList;
				*iterGameList = NULL;
			}
		}
		m_game_list.clear();

		// 释放游戏服务器列表内存
		GameServer_List::iterator iter = m_gameserver_list.begin();
		while (iter != m_gameserver_list.end())
		{
			if (iter->second != NULL)
			{
				delete iter->second;
				iter->second = NULL;
			}
			++iter;
		}
		m_gameserver_list.clear();
	}
	catch(...)
	{
	}
}

bool CGameServerConf::LoadConf()
{
	try
	{
		std::map<ULONG, ULONG> gameId2ServerId; //key=gameid, value=serverid;
		TiXmlDocument *myDocument = new TiXmlDocument(m_filepath.c_str());
		myDocument->LoadFile();

		TiXmlElement *RootElement = myDocument->RootElement();
		TiXmlNode *elem = NULL;
		TiXmlElement *elemAttribute = NULL;
		if (elem = RootElement->FirstChild("GameList"))
		{
			TiXmlElement *pChildElem = elem->FirstChildElement();
			do
			{
				Game_Item *pGame = new Game_Item;
				pGame->id = atoi(pChildElem->Attribute("id"));
				pGame->name = pChildElem->Attribute("name");
				pGame->serverid = 0;

				m_game_list.push_back(pGame);

			} while (pChildElem = pChildElem->NextSiblingElement());
		}
		if (elem = RootElement->FirstChild("ServerList"))
		{
			TiXmlElement *pChildElem = elem->FirstChildElement();
			do
			{
				ULONG serverId = atol(pChildElem->Attribute("serverid"));
				if (m_gameserver_list.find(serverId) == m_gameserver_list.end())
				{
					Game_Server *pServer = new Game_Server;
					pServer->id = serverId;
					pServer->name = pChildElem->Attribute("servername");
					pServer->fullname = pChildElem->Attribute("fullname");
					pServer->ip = pChildElem->Attribute("ip");
					pServer->port = atol(pChildElem->Attribute("port"));
					pServer->nowVer = atol(pChildElem->Attribute("NowVer"));
					pServer->LowVer = atol(pChildElem->Attribute("LowVer"));
					pServer->maxUser = atol(pChildElem->Attribute("maxUser"));

					m_gameserver_list[pServer->id] = pServer;
				}
				
				ULONG gameId = atol(pChildElem->Attribute("gameid"));
				gameId2ServerId[gameId] = serverId;

			} while (pChildElem = pChildElem->NextSiblingElement());
		}
		
		Game_List::iterator iter = m_game_list.begin();
		for ( ; iter!=m_game_list.end(); iter++)
		{
			if (*iter == NULL)	continue;

			std::map<ULONG, ULONG>::iterator iterMap = gameId2ServerId.find((*iter)->id);
			if (iterMap != gameId2ServerId.end())
			{
				(*iter)->serverid = iterMap->second;
			}
		}

		delete myDocument;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CGameServerConf::GetGameServer(ULONG gameid, Game_Server& gameserver)
{
	Game_List::iterator iterGameList = m_game_list.begin();
	for ( ; iterGameList!=m_game_list.end(); iterGameList++)
	{
		if (*iterGameList != NULL && gameid==(*iterGameList)->id)
		{
			ULONG gameServerId = (*iterGameList)->serverid;
			GameServer_List::iterator iter = m_gameserver_list.find(gameServerId);
			if (iter != m_gameserver_list.end() && iter->second)
			{
				gameserver =  *(iter->second);
				return true;
			}
		}
	}

	return false;
}

std::vector<Game_Item*>* CGameServerConf::GetGameList()
{
	return &m_game_list;
}
