#pragma once
#include "afxcmn.h"


// CProcessViewDlg �Ի���

class CProcessViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessViewDlg)

public:
	CProcessViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROCESSVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_process;
	afx_msg void OnBnClickedOk();
	int m_select_pid;
	CString m_select_pname;
};
