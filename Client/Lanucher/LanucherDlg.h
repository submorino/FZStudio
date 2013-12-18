
// LanucherDlg.h : header file
//

#pragma once

#include "LogonServerConf.h"
#include "GameServerConf.h"
#include "Player.h"
#include "task.h"
#include "Worker.h"

// CLanucherDlg dialog
class CLanucherDlg : public CDialog
{
// Construction
public:
	CLanucherDlg(CWnd* pParent = NULL);	// standard constructor
	~CLanucherDlg();

// Dialog Data
	enum { IDD = IDD_LANUCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCbnSelchangeComboGamelist();
	afx_msg void OnCbnSelchangeComboLogonserver();
	afx_msg void OnBnClickedButtonLogon();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonExit();
	DECLARE_MESSAGE_MAP()

private:
	CString GetAppRunDir();
public:
	CComboBox m_combo_logonServer;
	CEdit m_edit_logonName;
	CEdit m_edit_password;
	CComboBox m_combo_gamelist;
	CStatic m_static_gameServerInfo;
	CStatic m_static_LogonServerInfo;

	CPlayer m_player;
	CThreadPool<CMyWorker> m_threadPool;
};
