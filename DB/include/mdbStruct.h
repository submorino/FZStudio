#pragma once

#include "MyStruct.h"


struct put_logon_info_req
{
	~put_logon_info_req()
	{
		std::vector<logon_info*>::iterator iter = vectorreq.begin();
		for ( ; iter!=vectorreq.end(); ++iter)
			delete *iter;

		vectorreq.clear();
	}

	std::vector<logon_info*> vectorreq;
};

struct put_logon_info_ack
{
	int lRetCode;
};

struct get_logon_info_req
{
};

struct get_logon_info_ack
{
	~get_logon_info_ack()
	{
		std::vector<logon_info*>::iterator iter = vectorack.begin();
		for ( ; iter!=vectorack.end(); ++iter)
			delete *iter;

		vectorack.clear();
	}

	std::vector<logon_info*> vectorack;
};

struct put_player_info_req
{
	~put_player_info_req()
	{
		std::vector<player_info*>::iterator iter = vectorreq.begin();
		for ( ; iter!=vectorreq.end(); ++iter)
			delete *iter;

		vectorreq.clear();
	}

	std::vector<player_info*> vectorreq;
};

struct put_player_info_ack
{
	int lRetCode;
};

struct get_player_info_req
{
};

struct get_player_info_ack
{
	~get_player_info_ack()
	{
		std::vector<player_info*>::iterator iter = vectorack.begin();
		for ( ; iter!=vectorack.end(); ++iter)
			delete *iter;

		vectorack.clear();
	}

	std::vector<player_info*> vectorack;
};

struct put_money_info_req
{
	~put_money_info_req()
	{
		std::vector<money_info*>::iterator iter = vectorreq.begin();
		for ( ; iter!=vectorreq.end(); ++iter)
			delete *iter;

		vectorreq.clear();
	}

	std::vector<money_info*> vectorreq;
};

struct put_money_info_ack
{
	int lRetCode;
};

struct get_money_info_req
{
};

struct get_money_info_ack
{
	~get_money_info_ack()
	{
		std::vector<money_info*>::iterator iter = vectorack.begin();
		for ( ; iter!=vectorack.end(); ++iter)
			delete *iter;

		vectorack.clear();
	}

	std::vector<money_info*> vectorack;
};
