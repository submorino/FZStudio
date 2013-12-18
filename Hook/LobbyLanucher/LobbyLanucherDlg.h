// LobbyLanucherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>


// CLobbyLanucherDlg �Ի���
class CLobbyLanucherDlg : public CDialog
{
// ����
public:
	CLobbyLanucherDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CLobbyLanucherDlg::~CLobbyLanucherDlg();

// �Ի�������
	enum { IDD = IDD_LOBBYLANUCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void LanucherGame(CString& strGamePath, int count=1);
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonLanucher();
	CEdit m_edit_gamepath;
	CButton m_bn_browse;
	std::vector<HANDLE> ProcessHandleList;
	afx_msg void OnBnClickedButtonResume();

	int m_processid;
};
