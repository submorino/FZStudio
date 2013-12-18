#pragma once

#include "taskbase.h"
#include "MyStruct.h"
#include "Player.h"

class CTaskLogonBase : public CTaskBase
{
public:
	CTaskLogonBase(CPlayer* player, const std::string& ip, USHORT port)
		: CTaskBase()
		, m_player(player)
		, m_serverIp(ip)
		, m_port(port)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped){};

protected:
	CPlayer *m_player;
	std::string m_serverIp;
	USHORT	m_port;
};

class CTaskLogoutBase : public CTaskBase
{
public:
	CTaskLogoutBase(CPlayer* player)
		: CTaskBase()
		, m_player(player)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped){};

protected:
	CPlayer *m_player;
};

class CTaskLogon : public CTaskLogonBase
{
public:
	CTaskLogon(CPlayer* player, const std::string& ip, USHORT port)
		: CTaskLogonBase(player, ip, port)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
};

class CTaskLogout : public CTaskLogoutBase
{
public:
	CTaskLogout(CPlayer* player)
		: CTaskLogoutBase(player)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
};

class CTaskEnterGame : public CTaskLogonBase
{
public:
	CTaskEnterGame(CPlayer* player, const std::string& ip, USHORT port)
		: CTaskLogonBase(player, ip, port)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
};

class CTaskExitGame : public CTaskLogoutBase
{
public:
	CTaskExitGame(CPlayer* player)
		: CTaskLogoutBase(player)
	{
	}

public:
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
};
