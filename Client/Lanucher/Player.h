#pragma once
#include "MyStruct.h"
#include <WINSOCK2.H>

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	
public:
	bool Logon(const std::string& ip, USHORT port);
	bool Logout();

	bool EnterGame(const std::string& ip, USHORT port);
	bool ExitGame();

public:
	bool isLogon() 
	{ 
		long val = ::InterlockedCompareExchange(&m_bLogon, 0, 0); 
		return val==1;
	}
	bool isEnterGame()
	{ 
		long val = ::InterlockedCompareExchange(&m_bEnterGame, 0, 0); 
		return val==1;
	}

public:
	long getPlayerId() { return m_playerId; }
	void SetPlayerId(long playerid) { m_playerId = playerid; }

	std::string getPlayerName() { return m_playerName; }
	void setPlayerName(const std::string& name) { m_playerName = name; }

	std::string getPlayerPassword() { return m_password; }
	void setPlayerPassword(const std::string& password) { m_password = password; }

public:
	long getLogonServerId() { return m_nLogonServerId; }
	long getEnterGameId()	{return m_nEnterGameId; }

protected:
	static DWORD WINAPI LogonThreadProc(LPVOID lpParameter);
	static DWORD WINAPI GameThreadProc(LPVOID lpParameter);

private:
	//bool m_bLogon;
	//bool m_bEnterGame;
	volatile long m_bLogon;
	volatile long m_bEnterGame;

private:
	long m_playerId;
	std::string m_playerName;
	std::string m_password;

private:
	long m_nLogonServerId;
	long m_nEnterGameId;

protected:
	SOCKET m_logonSocket;
	SOCKET m_gameSocket;

private:
	HANDLE m_hLogonThread;
	HANDLE m_hGameThread;

//private:
//	CRITICAL_SECTION m_csLogon;
//	CRITICAL_SECTION m_csGame;
};
