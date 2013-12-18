#include "StdAfx.h"
#include "Player.h"

#pragma comment(lib,"ws2_32.lib")

CPlayer::CPlayer()
: m_bLogon(0)
, m_bEnterGame(0)
, m_playerId(0)
, m_playerName("")
, m_password("")
, m_nLogonServerId(0)
, m_nEnterGameId(0)
, m_logonSocket(INVALID_SOCKET)
, m_gameSocket(INVALID_SOCKET)
, m_hLogonThread(INVALID_HANDLE_VALUE)
, m_hGameThread(INVALID_HANDLE_VALUE)
{
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest=MAKEWORD(1,1);
	int err=WSAStartup(myVersionRequest,&wsaData);
}

CPlayer::~CPlayer()
{
	WSACleanup();
}

bool CPlayer::Logon(const std::string& ip, USHORT port)
{
	if (isLogon())
	{
		Logout();
	}

	m_logonSocket = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN sock_in;
	sock_in.sin_addr.S_un.S_addr=inet_addr(ip.c_str());
	sock_in.sin_family=AF_INET;
	sock_in.sin_port=htons(port);

	for (int i=0; i<5; ++i)
	{
		if (connect(m_logonSocket,(SOCKADDR*)&sock_in,sizeof(SOCKADDR)))
		{
			// send and recv packets to make a connection with logon server

			::InterlockedExchange(&m_bLogon, 1);
			m_hLogonThread = CreateThread(NULL, 0, LogonThreadProc, reinterpret_cast<LPVOID>(this), 0, NULL);
			return true;
		}

		::Sleep(500);
	}

	return false;
}

bool CPlayer::Logout()
{
	::InterlockedExchange(&m_bLogon, 0);
	if (m_hLogonThread != INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(m_hLogonThread, INFINITE);
		::CloseHandle(m_hLogonThread);
	}
	::closesocket(m_logonSocket);

	return true;
}

bool CPlayer::EnterGame(const std::string& ip, USHORT port)
{
	if (isEnterGame())
	{
		ExitGame();
	}

	m_gameSocket = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN sock_in;
	sock_in.sin_addr.S_un.S_addr=inet_addr(ip.c_str());
	sock_in.sin_family=AF_INET;
	sock_in.sin_port=htons(port);

	for (int i=0; i<5; ++i)
	{
		if (connect(m_gameSocket,(SOCKADDR*)&sock_in,sizeof(SOCKADDR)))
		{
			// send and recv packets to make a connection with game server
			::InterlockedExchange(&m_bEnterGame, 1);
			m_hGameThread = CreateThread(NULL, 0, GameThreadProc, reinterpret_cast<LPVOID>(this), 0, NULL);
			return true;
		}

		::Sleep(500);
	}

	return false;
}

bool CPlayer::ExitGame()
{
	::InterlockedExchange(&m_bEnterGame, 0);
	if (m_hGameThread != INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(m_hGameThread, INFINITE);
		::CloseHandle(m_hGameThread);
	}
	::closesocket(m_gameSocket);

	return false;
}

DWORD WINAPI CPlayer::LogonThreadProc(LPVOID lpParameter)
{
	CPlayer *pPlayer = reinterpret_cast<CPlayer*>(lpParameter);
	while (pPlayer->isLogon())
	{
		// communication with logon server
		char sendbuf[256] = {0};
		sprintf(sendbuf, "%c", '!');
		::send(pPlayer->m_logonSocket, sendbuf, (int)strlen(sendbuf), 0);
		::Sleep(100);
	}

	return 0;
}

DWORD WINAPI CPlayer::GameThreadProc(LPVOID lpParameter)
{
	CPlayer *pPlayer = reinterpret_cast<CPlayer*>(lpParameter);

	while (pPlayer->isEnterGame())
	{
		// communication with Game server
		char sendbuf[256] = {0};
		sprintf(sendbuf, "Player %s: sending game data test\n", pPlayer->getPlayerName().c_str());
		::send(pPlayer->m_gameSocket, sendbuf, (int)strlen(sendbuf), 0);
		::Sleep(10000);
	}

	return 0;
}

/*
hallver
session
dbid
logintype
username
password
snkey
buildver
compterid
computername
siteid
regident
*/