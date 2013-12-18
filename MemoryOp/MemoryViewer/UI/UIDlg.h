// UIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUIDlg �Ի���
class CUIDlg : public CDialog
{
// ����
public:
	CUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonSelectprocess();
	afx_msg void OnBnClickedButtonQuery();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_read_memory;
	CStatic m_static_process;
	CEdit m_edit_address;

	int m_curProcessId;
	CString m_curProcessName;

	afx_msg void OnBnClickedButtonWrite();
	CEdit m_edit_value;
	afx_msg void OnBnClickedButtonSuspendProcess();
	CButton m_bnSuspendProcess;
	afx_msg void OnBnClickedButtonWindowtool();
};
