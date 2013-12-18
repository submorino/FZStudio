// ProcessViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UI.h"
#include "ProcessViewDlg.h"
#include <Tlhelp32.h>


// CProcessViewDlg 对话框

IMPLEMENT_DYNAMIC(CProcessViewDlg, CDialog)

CProcessViewDlg::CProcessViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessViewDlg::IDD, pParent)
{

}

CProcessViewDlg::~CProcessViewDlg()
{
}

void CProcessViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_list_process);
}


BEGIN_MESSAGE_MAP(CProcessViewDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProcessViewDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CProcessViewDlg 消息处理程序

BOOL CProcessViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list_process.InsertColumn(0, "PID");
	m_list_process.SetColumnWidth(0, 60);
	m_list_process.InsertColumn(1, "PName");
	m_list_process.SetColumnWidth(1, 160);
	DWORD dwStyle = m_list_process.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;
	m_list_process.SetExtendedStyle(dwStyle); 

	PROCESSENTRY32 pinfo;
	ZeroMemory(&pinfo,sizeof(PROCESSENTRY32));
	pinfo.dwSize = sizeof(PROCESSENTRY32);

	MODULEENTRY32 minfo;
	ZeroMemory(&minfo,sizeof(MODULEENTRY32));
	minfo.dwSize = sizeof(MODULEENTRY32);

	TCHAR shortpath[256] = {0};

	HANDLE hProcess=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	BOOL report=::Process32First(hProcess,&pinfo);

	int n = 0;
	TCHAR buf[16] = {0};
	while(report)
	{
		HANDLE hModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pinfo.th32ProcessID);

		Module32First(hModule, &minfo); 

		GetShortPathName(minfo.szExePath,shortpath,256);

		report=Process32Next(hProcess, &pinfo);   

		m_list_process.InsertItem(n, _itoa(pinfo.th32ProcessID, buf, 10)); 
		m_list_process.SetItemText(n, 1, pinfo.szExeFile);
		n++;
	}

	return TRUE;
}

void CProcessViewDlg::OnBnClickedOk()
{
	int row = m_list_process.GetSelectionMark();
	if (row != -1)
	{
		CString strPid = m_list_process.GetItemText(row, 0);
		m_select_pid = atoi(strPid.GetBuffer());
		strPid.ReleaseBuffer();
		m_select_pname = m_list_process.GetItemText(row, 1);
	}
	else
	{
		m_select_pid = -1;
		m_select_pname = "";
	}

	OnOK();
}
