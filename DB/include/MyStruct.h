#pragma once

#include <vector>
#include <string>

struct logon_info
{
	int id;

	std::string logonname;
	std::string password;
};

struct player_info
{
	int id;

	std::string playername;
	std::string phone;
	std::string address;
	std::string postcode;
	bool bman;
	std::string identitycard;

	std::string email;
};

struct money_info
{
	int id;

	int score;
	int gold;
	int ticketcount;
};
