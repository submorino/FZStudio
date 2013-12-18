// UIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UI.h"
#include "UIDlg.h"
#include <map>
#include "ProcessViewDlg.h"
#include "../MemoryViewer/ProgressData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUIDlg �Ի���




CUIDlg::CUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUIDlg::IDD, pParent)
	, m_curProcessId(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CString strCurFile;
    GetModuleFileName(NULL, strCurFile.GetBuffer(MAX_PATH), MAX_PATH);
    strCurFile.ReleaseBuffer();
    strCurFile = strCurFile.Left(strCurFile.ReverseFind('\\'));
    ::SetCurrentDirectory(strCurFile);	
}

void CUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_list_read_memory);
	DDX_Control(pDX, IDC_STATIC_PROCESSNAME, m_static_process);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_edit_address);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edit_value);
	DDX_Control(pDX, IDC_BUTTON_SUSPEND_PROCESS, m_bnSuspendProcess);
}

BEGIN_MESSAGE_MAP(CUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SELECTPROCESS, &CUIDlg::OnBnClickedButtonSelectprocess)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CUIDlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CUIDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND_PROCESS, &CUIDlg::OnBnClickedButtonSuspendProcess)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWTOOL, &CUIDlg::OnBnClickedButtonWindowtool)
END_MESSAGE_MAP()


// CUIDlg ��Ϣ�������

BOOL CUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_list_read_memory.InsertColumn(0, "Address");
	m_list_read_memory.SetColumnWidth(0, 80);
	m_list_read_memory.InsertColumn(1, "Value");
	m_list_read_memory.SetColumnWidth(1, 300);
	m_list_read_memory.InsertColumn(2, "");
	m_list_read_memory.SetColumnWidth(2, 160);
	DWORD dwStyle = m_list_read_memory.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;
	m_list_read_memory.SetExtendedStyle(dwStyle); 

	m_edit_address.SetWindowText("0x0040505D");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#include <atlutil.h>
void CUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUIDlg::OnPaint()
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
HCURSOR CUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUIDlg::OnBnClickedButtonSelectprocess()
{
	CProcessViewDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_curProcessId = dlg.m_select_pid;
		m_curProcessName = dlg.m_select_pname;

		m_static_process.SetWindowText(m_curProcessName);
	}
}

void CUIDlg::OnBnClickedButtonQuery()
{
	CString strAddress;
	m_edit_address.GetWindowText(strAddress);
	if (m_curProcessId >= 0 && !strAddress.IsEmpty())
	{
		int address;
		StrToIntEx(strAddress.GetBuffer(), STIF_SUPPORT_HEX, &address);
		strAddress.ReleaseBuffer();
		byte buffer[16] = {0};
		if (!CProgressData::ReadMemory(m_curProcessId, (LPVOID)address, buffer, sizeof(buffer)/sizeof(buffer[0])))
		{
			MessageBox("���ڴ�ʧ��", "���ڴ�"); 
			return;
		}
		CString value(""), strValue("");
		CString tmp;
		for (int i=0; i<16; ++i)
		{
			tmp.Format("%02X ", buffer[i]);
			value += tmp;
			tmp.Format("%c", buffer[i]);
			strValue += tmp;
		}
		if (!value.IsEmpty())
		{
			int row = 0;
			for (; row<m_list_read_memory.GetItemCount(); ++row)
			{
				if (m_list_read_memory.GetItemData(row) == address)
				{
					m_list_read_memory.SetItemText(row, 1, value);
					m_list_read_memory.SetItemText(row, 2, strValue);
					return;
				}
			}
			row = m_list_read_memory.GetItemCount();
			CString tmp;
			tmp.Format("0x%08X", address);
			m_list_read_memory.InsertItem(row, tmp);
			m_list_read_memory.SetItemText(row, 1, value);
			m_list_read_memory.SetItemText(row, 2, strValue);
			m_list_read_memory.SetItemData(row, address);
		}
	}
}
void CUIDlg::OnBnClickedButtonWrite()
{
	//0x0040505d 4214877
	CString strAddress, strValue;
	m_edit_address.GetWindowText(strAddress);
	m_edit_value.GetWindowText(strValue);
	if (m_curProcessId >= 0 && !strAddress.IsEmpty() && !strValue.IsEmpty())
	{
		int address;
		StrToIntEx(strAddress.GetBuffer(), STIF_SUPPORT_HEX, &address);
		strAddress.ReleaseBuffer();
		byte buffer[16] = {0};
		memcpy(buffer, strValue.GetBuffer(), min(16, strValue.GetLength()));
		strValue.ReleaseBuffer();
		if (!CProgressData::WriteMemory(m_curProcessId, (LPVOID)address, buffer, sizeof(buffer)/sizeof(buffer[0])))
		{
			MessageBox("д�ڴ�ʧ��", "д�ڴ�");
			return;
		}
	}
}

void CUIDlg::OnBnClickedButtonSuspendProcess()
{
	static bool bSuspend = false;

	if (m_curProcessId >= 0)
	{
		HANDLE hToken;

		if (!OpenProcessToken(GetCurrentProcess(), 
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		{
			return;
		}
		CProgressData::SetPrivilege(hToken, SE_DEBUG_NAME ,TRUE);

		if (!bSuspend)
		{
			if (CProgressData::SuspendProcess(m_curProcessId))
			{
				m_bnSuspendProcess.SetWindowText("���ѽ���");
				bSuspend = true;
			}
		}
		else
		{
			if (CProgressData::ResumeProcess(m_curProcessId))
			{
				m_bnSuspendProcess.SetWindowText("��ͣ����");
				bSuspend = false;
			}
		}
	}

	//// ��ֻ���ڴ����д�������ԣ�����������쳣 ���ڼ������д���@err,hr �ɹ۲���󷵻أ�
	//char buf[4] = {'a','b','c','d'};
	//char buf2[4] = {0};

	//LPVOID lpvBase = VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//if (lpvBase){
	//	DWORD lpflOldProtect = 0;
	//	memcpy(lpvBase,buf,4);
	//	VirtualProtect(lpvBase,10,PAGE_WRITECOPY,&lpflOldProtect);
	//	//memcpy(lpvBase,buf2,4);
	//}
	//__asm
	//{
	//	lea esi, buf2
	//	mov ecx,100
	//	mov edi,dword ptr lpvBase
	//	rep movsb 
	//}

}

void CUIDlg::OnBnClickedButtonWindowtool()
{
	::ShellExecute(this->GetSafeHwnd(), "open", "zDump.exe", "", "", SW_SHOW);
}
