#pragma once
#include "afxcmn.h"


// CProcessViewDlg 对话框

class CProcessViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessViewDlg)

public:
	CProcessViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROCESSVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_process;
	afx_msg void OnBnClickedOk();
	int m_select_pid;
	CString m_select_pname;
};
