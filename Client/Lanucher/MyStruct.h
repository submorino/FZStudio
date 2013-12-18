#pragma once

#include <string>
#include <vector>
#include <map>


struct Logon_Server
{
	std::string name;
	std::string ip;
	ULONG		port;
};

struct Game_Server
{
	ULONG		id;
	std::string	name;
	std::string fullname;
	std::string ip;
	ULONG		port;
	ULONG		nowVer;
	ULONG		LowVer;
	ULONG		maxUser;
};

struct Game_Item
{
	ULONG		id;
	std::string	name;
	ULONG		serverid;
};

typedef std::map<ULONG, Game_Server*> GameServer_List; //key=serverid, value=Game_Server
typedef std::vector<Game_Item*> Game_List; 

struct Logon_Info
{
	int id;

	std::string logonname;
	std::string password;
};

#define SAFE_DELETE(p)	do{ if(p){delete p; p=NULL;} }while(0)