
// LanucherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lanucher.h"
#include "LanucherDlg.h"
#include <sstream>
#include "ConfigMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define THREADPOOL_SIZE	5

long g_lCurrId = -1;

// CLanucherDlg dialog
CLanucherDlg::CLanucherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLanucherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_threadPool.Initialize((void*)321, THREADPOOL_SIZE);
}

CLanucherDlg::~CLanucherDlg()
{
	m_threadPool.Shutdown();
}

void CLanucherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOGONSERVER, m_combo_logonServer);
	DDX_Control(pDX, IDC_EDIT_LOGONNAME, m_edit_logonName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_password);
	DDX_Control(pDX, IDC_COMBO_GAMELIST, m_combo_gamelist);
	DDX_Control(pDX, IDC_STATIC_GAMESERVERINFO, m_static_gameServerInfo);
	DDX_Control(pDX, IDC_STATIC_LOGONSERVERINFO, m_static_LogonServerInfo);
}

BEGIN_MESSAGE_MAP(CLanucherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGON, &CLanucherDlg::OnBnClickedButtonLogon)
	ON_BN_CLICKED(IDC_BUTTON_START, &CLanucherDlg::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_GAMELIST, &CLanucherDlg::OnCbnSelchangeComboGamelist)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGONSERVER, &CLanucherDlg::OnCbnSelchangeComboLogonserver)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CLanucherDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CLanucherDlg::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// CLanucherDlg message handlers

BOOL CLanucherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
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
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLanucherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLanucherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLanucherDlg::OnCbnSelchangeComboGamelist()
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

void CLanucherDlg::OnCbnSelchangeComboLogonserver()
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

CString CLanucherDlg::GetAppRunDir()
{
	CString strCurFile;
	GetModuleFileName(NULL, strCurFile.GetBuffer(MAX_PATH), MAX_PATH);
	strCurFile.ReleaseBuffer();
	return strCurFile.Left(strCurFile.ReverseFind('\\'));
}

void CLanucherDlg::OnBnClickedButtonLogon()
{
	CString serverName;
	m_combo_logonServer.GetWindowText(serverName);
	Logon_Server server;
	if (CONFIGMGR->getLogonServerConf()->GetLogonServer(serverName.GetBuffer(), server))
	{
		std::vector<CPlayer*> *pPlayerList = CONFIGMGR->getPlayerConf()->GetPlayerList();
		for (int i=0; i<pPlayerList->size(); ++i)
		{
			CTaskBase *pLogonTask = new CTaskLogon((*pPlayerList)[i], /*server.ip*/"58.63.236.238", /*server.port*/80);
			m_threadPool.QueueRequest((CMyWorker::RequestType) pLogonTask);
		}
	}
}

void CLanucherDlg::OnBnClickedButtonLogout()
{
	std::vector<CPlayer*> *pPlayerList = CONFIGMGR->getPlayerConf()->GetPlayerList();
	for (int i=0; i<pPlayerList->size(); ++i)
	{
		CTaskBase *pLogonTask = new CTaskLogout((*pPlayerList)[i]);
		m_threadPool.QueueRequest((CMyWorker::RequestType) pLogonTask);
	}
}

void CLanucherDlg::OnBnClickedButtonStart()
{
	int index = m_combo_gamelist.GetCurSel();
	ULONG gameserverid = m_combo_gamelist.GetItemData(index);
	Game_Server item;
	if (CONFIGMGR->getGameServerConf()->GetGameServer(gameserverid, item))
	{
		std::vector<CPlayer*> *pPlayerList = CONFIGMGR->getPlayerConf()->GetPlayerList();
		for (int i=0; i<pPlayerList->size(); ++i)
		{
			CTaskBase *pEnterGameTask = new CTaskEnterGame((*pPlayerList)[i], item.ip, item.port);
			m_threadPool.QueueRequest((CMyWorker::RequestType) pEnterGameTask);
		}
	}
}

void CLanucherDlg::OnBnClickedButtonExit()
{
	std::vector<CPlayer*> *pPlayerList = CONFIGMGR->getPlayerConf()->GetPlayerList();
	for (int i=0; i<pPlayerList->size(); ++i)
	{
		CTaskBase *pExitGameTask = new CTaskExitGame((*pPlayerList)[i]);
		m_threadPool.QueueRequest((CMyWorker::RequestType) pExitGameTask);
	}
}
