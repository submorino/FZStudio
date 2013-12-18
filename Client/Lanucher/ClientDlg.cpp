// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lanucher.h"
#include "ClientDlg.h"
#include "ConfigMgr.h"

#define THREADPOOL_SIZE	5


// CClientDlg dialog

IMPLEMENT_DYNAMIC(CClientDlg, CDialog)

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_pLogonWnd(NULL)
{
	m_threadPool.Initialize((void*)321, THREADPOOL_SIZE);
	m_pLogonWnd = new CLogonWnd(this);
}

CClientDlg::~CClientDlg()
{
	SAFE_DELETE(m_pLogonWnd);
	m_threadPool.Shutdown();
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_USER, m_treectrl_user);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_USER, &CClientDlg::OnTvnSelchangedTreeUser)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	InitTreeCtrl();

	m_pLogonWnd->Create(IDD_DIALOG_LOGIN, this);

	CRect rect;
	m_treectrl_user.GetWindowRect(rect);
	int x = rect.left + rect.Width();
	int y = rect.top;
	m_pLogonWnd->MoveWindow(x, y, 400, 460);

	m_pLogonWnd->ShowWindow(SW_SHOWNORMAL);
	
	return TRUE;
}

void CClientDlg::InitTreeCtrl()
{
	DWORD dwStyles = ::GetWindowLong(m_treectrl_user.m_hWnd,GWL_STYLE);
	dwStyles |= TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	::SetWindowLong(m_treectrl_user.m_hWnd,GWL_STYLE,dwStyles);

	HTREEITEM hRoot, hCur;//树控制项目句柄
	TV_INSERTSTRUCT TCItem;//插入数据项数据结构

	//// begin init game server
	//TCItem.hParent=TVI_ROOT;//增加根项
	//TCItem.hInsertAfter=TVI_LAST;//在最后项之后
	//TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	//TCItem.item.pszText="游戏服务器";
	//TCItem.item.lParam=0;//序号
	//TCItem.item.iImage=0;//正常图标
	//TCItem.item.iSelectedImage=1;//选中时图标
	//hRoot=m_treectrl_user.InsertItem(&TCItem);//返回根项句柄

	//std::vector<Logon_Server*> *p_LogonServerList;
	//p_LogonServerList = m_pLogonServer->GetLogonServerList();
	//std::vector<Logon_Server*>::iterator iter=p_LogonServerList->begin();
	//std::vector<Game_Item*>* pGameList = m_pGameServer->GetGameList();
	//std::vector<Game_Item*>::iterator iterGameList = pGameList->begin();
	//for (; iter!=p_LogonServerList->end(); iter++)
	//{
	//	TCItem.hParent=hRoot;
	//	TCItem.item.pszText=(char*)(*iter)->name.c_str();
	//	hCur=m_treectrl_user.InsertItem(&TCItem);
	//	if (pGameList != NULL)
	//	{
	//		for (iterGameList=pGameList->begin(); iterGameList!=pGameList->end(); iterGameList++)
	//		{
	//			TCItem.hParent=hCur;
	//			TCItem.item.pszText=(char*)(*iterGameList)->name.c_str();
	//			//TCItem.item.lParam=(i+1)*10+(j+1);//子项序号
	//			m_treectrl_user.InsertItem(&TCItem);
	//		}
	//	}
	//}
	//m_treectrl_user.Expand(hRoot,TVE_EXPAND);//展开上一级树
	//// end init game server

	m_imageListFolder.Create( 23, 21, ILC_COLOR24, 0, 0);

	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_ICON_PC));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_ICON_OFFLINE));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_ICON_ONLINE));
		
	m_treectrl_user.SetImageList(&m_imageListFolder, TVSIL_NORMAL);

	// begin init player info
	TCItem.hInsertAfter=TVI_LAST;//在最后项之后
	TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.lParam=0;//序号

	TCItem.hParent=TVI_ROOT;//增加根项
	TCItem.item.lParam=1;//序号
	TCItem.item.iImage=0;//正常图标
	TCItem.item.iSelectedImage=0;//选中时图标
	TCItem.item.pszText="已登录的账号";
	hRoot=m_treectrl_user.InsertItem(&TCItem);//返回根项句柄

	TCItem.hParent=TVI_ROOT;//增加根项
	TCItem.item.lParam=2;//序号
	TCItem.item.iImage=1;//正常图标
	TCItem.item.iSelectedImage=1;//选中时图标
	TCItem.item.pszText="未登录的账号";
	hRoot=m_treectrl_user.InsertItem(&TCItem);//返回根项句柄

	std::vector<CPlayer*> *pPlayerList = CONFIGMGR->getPlayerConf()->GetPlayerList();
	for (int i=0; i<pPlayerList->size(); ++i)
	{
		//memset(&TCItem, 0, sizeof(TV_INSERTSTRUCT));
		TCItem.hParent=hRoot;
		TCItem.item.lParam= 4 ;//序号
		TCItem.item.iImage=2;//正常图标
		TCItem.item.iSelectedImage=2;//选中时图标
		std::string strname = (*pPlayerList)[i]->getPlayerName();
		TCItem.item.pszText=(char*)strname.c_str();
		hCur=m_treectrl_user.InsertItem(&TCItem);
	}
	m_treectrl_user.Expand(hRoot,TVE_EXPAND);//展开上一级树
}

void CClientDlg::OnTvnSelchangedTreeUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (!pNMTreeView) return;
	TVITEM item = pNMTreeView->itemNew;
	switch (item.lParam)
	{
	case 0: //保留
		break;
	case 1:	//已登录根节点
		{
			std::string strPlayer;
			m_pLogonWnd->SetOnlineMode(strPlayer);
		}
		break;
	case 2: //未登录根节点
		{
			std::string strPlayer;
			m_pLogonWnd->SetOfflineMode(strPlayer);
		}
		break;
	case 3:	//已登录的账号
		{
			std::string strPlayer;
			m_pLogonWnd->SetOnlineMode(strPlayer);
		}
		break;
	case 4: //未登录的账号
		{
			std::string strPlayer;
			m_pLogonWnd->SetOfflineMode(strPlayer);
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (::IsWindow(m_treectrl_user.GetSafeHwnd()))
	{
		CRect rect;
		m_treectrl_user.GetWindowRect(rect);
		int x = rect.left + rect.Width();
		int y = rect.top;
		m_pLogonWnd->MoveWindow(x, y, 400, 460);
	}
}
