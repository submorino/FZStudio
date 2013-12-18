#pragma once
#include "afxcmn.h"
#include "ConfigMgr.h"
#include "Player.h"
#include "task.h"
#include "Worker.h"
#include "LogonWnd.h"

// CClientDlg dialog

class CClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClientDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTreeUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void InitTreeCtrl();
public:
	CComboBox m_combo_logonServer;
	CEdit m_edit_logonName;
	CEdit m_edit_password;
	CComboBox m_combo_gamelist;
	CStatic m_static_gameServerInfo;
	CStatic m_static_LogonServerInfo;

	CPlayer m_player;
	CThreadPool<CMyWorker> m_threadPool;

	CTreeCtrl m_treectrl_user;

	CLogonWnd *m_pLogonWnd;
	CImageList m_imageListFolder;
};
