// LogonWnd.cpp : implementation file
//

#include "stdafx.h"
#include <sstream>
#include "ConfigMgr.h"
#include "Lanucher.h"
#include "LogonWnd.h"


// CLogonWnd dialog

IMPLEMENT_DYNAMIC(CLogonWnd, CDialog)

CLogonWnd::CLogonWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonWnd::IDD, pParent)
{

}

CLogonWnd::~CLogonWnd()
{
}

void CLogonWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOGONSERVER, m_combo_logonServer);
	DDX_Control(pDX, IDC_COMBO_GAMELIST, m_combo_gamelist);
	DDX_Control(pDX, IDC_STATIC_GAMESERVERINFO, m_static_gameServerInfo);
	DDX_Control(pDX, IDC_STATIC_LOGONSERVERINFO, m_static_LogonServerInfo);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_bnLogin);
	DDX_Control(pDX, IDC_BUTTON_LOGOUT, m_bnLogout);
}


BEGIN_MESSAGE_MAP(CLogonWnd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLogonWnd::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CLogonWnd::OnBnClickedButtonLogout)
	ON_CBN_SELCHANGE(IDC_COMBO_GAMELIST, &CLogonWnd::OnCbnSelchangeComboGamelist)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGONSERVER, &CLogonWnd::OnCbnSelchangeComboLogonserver)
END_MESSAGE_MAP()


// CLogonWnd message handlers
BOOL CLogonWnd::OnInitDialog()
{	
	CDialog::OnInitDialog();

	std::vector<Logon_Server*> *p_LogonServerList;
	p_LogonServerList = CONFIGMGR->getLogonServerConf()->GetLogonServerList();
	std::vector<Logon_Server*>::iterator iter=p_LogonServerList->begin();
	for (; iter!=p_LogonServerList->end(); iter++)
	{
		m_combo_logonServer.InsertString(-1, (*iter)->name.c_str());
	}
	m_combo_logonServer.SetCurSel(0);
	OnCbnSelchangeComboLogonserver();

	std::vector<Game_Item*>* pGameList = CONFIGMGR->getGameServerConf()->GetGameList();
	if (pGameList != NULL)
	{
		std::vector<Game_Item*>::iterator iterGameList = pGameList->begin();
		for ( ; iterGameList!=pGameList->end(); iterGameList++)
		{
			int row = m_combo_gamelist.InsertString(-1, (*iterGameList)->name.c_str());
			m_combo_gamelist.SetItemData(row, (*iterGameList)->id);
		}

		m_combo_gamelist.SetCurSel(0);
		OnCbnSelchangeComboGamelist();
	}

	return TRUE;
}

void SetOnlineMode(const std::vector<std::string>& onlinePlayers)
{
}

void CLogonWnd::SetOnlineMode(const std::string& onlinePlayer)
{
	m_combo_logonServer.EnableWindow(FALSE);
	m_combo_gamelist.EnableWindow(FALSE);
	m_bnLogin.EnableWindow(FALSE);
	m_bnLogout.EnableWindow(TRUE);
}

void CLogonWnd::SetOfflineMode(const std::string& offinePlayer)
{
	m_combo_logonServer.EnableWindow(TRUE);
	m_combo_gamelist.EnableWindow(TRUE);
	m_bnLogin.EnableWindow(TRUE);
	m_bnLogout.EnableWindow(FALSE);
}

void CLogonWnd::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
}

void CLogonWnd::OnBnClickedButtonLogout()
{
	// TODO: Add your control notification handler code here
}


void CLogonWnd::OnCbnSelchangeComboGamelist()
{
	int index = m_combo_gamelist.GetCurSel();
	ULONG gameserverid = m_combo_gamelist.GetItemData(index);
	Game_Server item;
	CONFIGMGR->getGameServerConf()->GetGameServer(gameserverid, item);
	std::stringstream ss;
	ss << "serverid: " << item.id << "\n"
		<< "servername: " << item.name << "\n"
		<< "fullname: " << item.fullname << "\n"
		<< "ip: " << item.ip << "\n"
		<< "port: " << item.port << "\n"
		<< "NowVer: " << item.nowVer << "\n"
		<< "LowVer: " << item.LowVer << "\n"
		<< "maxUser: " << item.maxUser << "\n";
	m_static_gameServerInfo.SetWindowText(ss.str().c_str());
}

void CLogonWnd::OnCbnSelchangeComboLogonserver()
{
	std::vector<Logon_Server*> *p_LogonServerList;
	p_LogonServerList = CONFIGMGR->getLogonServerConf()->GetLogonServerList();
	
	int index = m_combo_logonServer.GetCurSel();
	if (index > -1)
	{
		Logon_Server *pServer = p_LogonServerList->at(index);
		
		std::stringstream ss;
		ss << "Server Name: " << pServer->name << "\n"
			<< "IP: " << pServer->ip << "\n"
			<< "Port: " << pServer->port << "\n";
		m_static_LogonServerInfo.SetWindowText(ss.str().c_str());
	}
}