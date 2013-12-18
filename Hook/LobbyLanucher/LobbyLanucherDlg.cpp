// LobbyLanucherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LobbyLanucher.h"
#include "LobbyLanucherDlg.h"
#include "../MemoryViewer/ProgressData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLobbyLanucherDlg �Ի���




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


// CLobbyLanucherDlg ��Ϣ�������

BOOL CLobbyLanucherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLobbyLanucherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	OpenDlg.m_ofn.lpstrTitle = "ѡ����Ϸ������";         
	OpenDlg.m_ofn.lpstrInitialDir = m_szDir;       //��ʼĿ¼   
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
		MessageBox("��ѡ����Ϸ·��");
		return;
	}

	CFileFind ff;
	if(!ff.FindFile(strGamePath))
	{
		MessageBox("������Ϸ·���Ƿ���ȷ");
		return;
	}
	ff.Close();

	LanucherGame(strGamePath);
}

void CLobbyLanucherDlg::LanucherGame(CString& strGamePath, int count/*=1*/)
{
	//// ������Ϸ����
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
