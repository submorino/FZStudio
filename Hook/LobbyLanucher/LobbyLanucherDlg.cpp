// LobbyLanucherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LobbyLanucher.h"
#include "LobbyLanucherDlg.h"
#include "../MemoryViewer/ProgressData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLobbyLanucherDlg 对话框




CLobbyLanucherDlg::CLobbyLanucherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLobbyLanucherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLobbyLanucherDlg::~CLobbyLanucherDlg()
{
	typedef std::vector<HANDLE>::iterator Iter;
	for(Iter it = ProcessHandleList.begin();it!=ProcessHandleList.end();it++){
		TerminateProcess(*it,0);
		CloseHandle(*it);
	}
}

void CLobbyLanucherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GAMEDIRECTORY, m_edit_gamepath);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_bn_browse);
}

BEGIN_MESSAGE_MAP(CLobbyLanucherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CLobbyLanucherDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_LANUCHER, &CLobbyLanucherDlg::OnBnClickedButtonLanucher)
	ON_BN_CLICKED(IDC_BUTTON_RESUME, &CLobbyLanucherDlg::OnBnClickedButtonResume)
END_MESSAGE_MAP()


// CLobbyLanucherDlg 消息处理程序

BOOL CLobbyLanucherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLobbyLanucherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CLobbyLanucherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLobbyLanucherDlg::OnBnClickedButtonBrowse()
{
	CString strFileName,strFilePath;
	CString m_szDir = "c:\\";

	char BASED_CODE szFilter[] = "exe file(*.exe)|*.exe; *.exe|All Files (*.*)|*.*||";         

	CFileDialog OpenDlg(true, NULL, NULL, /*OFN_ALLOWMULTISELECT |*/ OFN_EXPLORER, szFilter);         
	OpenDlg.m_ofn.lStructSize = sizeof(OpenDlg.m_ofn);         
	OpenDlg.m_ofn.hwndOwner = NULL;         
	OpenDlg.m_ofn.lpstrTitle = "选择游戏主程序";         
	OpenDlg.m_ofn.lpstrInitialDir = m_szDir;       //初始目录   
	if( OpenDlg.DoModal() == IDOK)         
	{         
		strFilePath = OpenDlg.GetPathName(); 
		m_edit_gamepath.SetWindowText(strFilePath);
	}

}

void CLobbyLanucherDlg::OnBnClickedButtonLanucher()
{	
	CString strGamePath;
	m_edit_gamepath.GetWindowText(strGamePath);

	if (strGamePath.IsEmpty())
	{
		MessageBox("请选择游戏路径");
		return;
	}

	CFileFind ff;
	if(!ff.FindFile(strGamePath))
	{
		MessageBox("请检查游戏路径是否正确");
		return;
	}
	ff.Close();

	LanucherGame(strGamePath);
}

void CLobbyLanucherDlg::LanucherGame(CString& strGamePath, int count/*=1*/)
{
	//// 载入游戏流程
	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle = false;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;

	PROCESS_INFORMATION PI;

	if(::CreateProcess(strGamePath,0,&sa,&sa,false,CREATE_SUSPENDED,NULL,NULL,
		&si,
		&PI)){
		ProcessHandleList.push_back(PI.hProcess);
		m_processid = PI.dwProcessId;
	}
	//::ShellExecute(this->GetSafeHwnd(), "open", strGamePath, NULL, NULL, SW_SHOW);
}

void CLobbyLanucherDlg::OnBnClickedButtonResume()
{
	//CProgressData::ResumeProcess(m_processid);
}
