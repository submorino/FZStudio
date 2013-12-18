#include "stdafx.h"
#include "task.h"

void CTaskLogon::DoTask(void *pvParam, OVERLAPPED *pOverlapped)
{
	::Sleep(300);
	m_player->Logon(m_serverIp, m_port);
}

void CTaskLogout::DoTask(void *pvParam, OVERLAPPED *pOverlapped)
{
	m_player->Logout();
}

void CTaskEnterGame::DoTask(void *pvParam, OVERLAPPED *pOverlapped)
{
	::Sleep(300);
	m_player->EnterGame(m_serverIp, m_port);
}

void CTaskExitGame::DoTask(void *pvParam, OVERLAPPED *pOverlapped)
{
	m_player->ExitGame();
}