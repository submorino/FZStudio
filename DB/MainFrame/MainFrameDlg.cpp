
// MainFrameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrame.h"
#include "MainFrameDlg.h"
#include "DBMgr.h"
#include "MoneyMgr.h"
#include <ctime>

#define SAVEDB_TIMER	1001
#define GENERATEDATA_TIMER	1002
#define UPDATELIST_TIMER	1003

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMainFrameDlg dialog




CMainFrameDlg::CMainFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainFrameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CString strCurFile;
    GetModuleFileName(NULL, strCurFile.GetBuffer(MAX_PATH), MAX_PATH);
    strCurFile.ReleaseBuffer();
    strCurFile = strCurFile.Left(strCurFile.ReverseFind('\\'));
    ::SetCurrentDirectory(strCurFile);	
}

CMainFrameDlg::~CMainFrameDlg()
{
	delete DBMGR;
	delete MONEYMGR;
}

void CMainFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERINFO, m_list_userinfo);
}

BEGIN_MESSAGE_MAP(CMainFrameDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMainFrameDlg message handlers

BOOL CMainFrameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	/*{
		get_logon_info_req req;
		get_logon_info_ack ack;
		DBMGR->get_logon_info(req, ack);
	}

	{
		put_logon_info_req req;
		put_logon_info_ack ack;

		char buf[32];
		for (int i=100; i<11100; ++i)
		{
			logon_info *account = new logon_info();
			account->id = i;
			account->logonname = itoa(i,buf, 16); 
			account->password = "123456789";
			req.vectorreq.push_back(account);
		}
		DBMGR->put_logon_info(req, ack);
	}

	{
		put_money_info_req req;
		put_money_info_ack ack;

		char buf[32];
		for (int i=100; i<11100; ++i)
		{
			money_info *money = new money_info();
			money->id = i;
			money->score = 1000;
			money->gold = 100000;
			money->ticketcount = 0;
			req.vectorreq.push_back(money);
		}
		DBMGR->put_money_info(req, ack);
	}

	{
		get_money_info_req req;
		get_money_info_ack ack;
		DBMGR->get_money_info(req, ack);
	}*/

	DWORD dwStyle = m_list_userinfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_list_userinfo.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list_userinfo.InsertColumn( 0, "ID", LVCFMT_LEFT, 90 );//插入列
	m_list_userinfo.InsertColumn( 1, "score", LVCFMT_LEFT, 100 );
	m_list_userinfo.InsertColumn( 2, "gold", LVCFMT_LEFT, 100 );
	m_list_userinfo.InsertColumn( 3, "ticketcount", LVCFMT_LEFT, 100 );

	get_money_info_req req;
	get_money_info_ack ack;
	DBMGR->get_money_info(req, ack);

	char buf[32];
	int i=0;
	std::vector<money_info*>::iterator iter = ack.vectorack.begin();
	for ( ; iter!=ack.vectorack.end(); ++iter)
	{
		m_list_userinfo.InsertItem(i, _itoa((*iter)->id, buf, 10));
		m_list_userinfo.SetItemText(i, 1, _itoa((*iter)->score, buf, 10));
		m_list_userinfo.SetItemText(i, 2, _itoa((*iter)->gold, buf, 10));
		m_list_userinfo.SetItemText(i, 3, _itoa((*iter)->ticketcount, buf, 10));
		++i;
	}
	

	SetTimer(GENERATEDATA_TIMER, 10, NULL);
	SetTimer(SAVEDB_TIMER, 10000, NULL);
	SetTimer(UPDATELIST_TIMER, 100, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainFrameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainFrameDlg::OnPaint()
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
HCURSOR CMainFrameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainFrameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SAVEDB_TIMER)
	{
		MONEYMGR->Save();
	}
	else if(nIDEvent == GENERATEDATA_TIMER)
	{
		srand((unsigned)time(0));

		money_info money = {0};
		money.id = rand()%20;
		money.score = rand()%10000;
		money.gold = rand()%100000;
		money.ticketcount = rand()%100;

		MONEYMGR->AddMoney(money);
	}
	else if (nIDEvent == UPDATELIST_TIMER)
	{
		char buf[32];
		for (int i=0; i<100; i++)
		{
			int count = m_list_userinfo.GetItemCount();
			money_info money = {0};
			MONEYMGR->PopMoney(money);
			if (money.id == 0)	break;

			int j = 0;
			for ( ; j<count; ++j)
			{
				int id = atoi(m_list_userinfo.GetItemText(j, 0));
				if (money.id == id)
				{
					m_list_userinfo.SetItemText(j, 1, _itoa(money.score, buf, 10));
					m_list_userinfo.SetItemText(j, 2, _itoa(money.gold, buf, 10));
					m_list_userinfo.SetItemText(j, 3, _itoa(money.ticketcount, buf, 10));
					break;
				}
			}
			if (j == count)
			{
				m_list_userinfo.InsertItem(count, _itoa(money.id, buf, 10));
				m_list_userinfo.SetItemText(count, 1, _itoa(money.score, buf, 10));
				m_list_userinfo.SetItemText(count, 2, _itoa(money.gold, buf, 10));
				m_list_userinfo.SetItemText(count, 3, _itoa(money.ticketcount, buf, 10));
			}

		}
	}

	CDialog::OnTimer(nIDEvent);
}