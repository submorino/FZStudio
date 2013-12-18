#pragma once
#include "afxwin.h"


// CLogonWnd dialog

class CLogonWnd : public CDialog
{
	DECLARE_DYNAMIC(CLogonWnd)

public:
	CLogonWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonWnd();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnCbnSelchangeComboGamelist();
	afx_msg void OnCbnSelchangeComboLogonserver();

public:
	void SetOnlineMode(const std::vector<std::string>& onlinePlayers);
	void SetOnlineMode(const std::string& onlinePlayer);

	void SetOfflineMode(const std::vector<std::string>& offinePlayers);
	void SetOfflineMode(const std::string& offinePlayer);
private:
	CComboBox m_combo_logonServer;
	CComboBox m_combo_gamelist;
	CStatic m_static_gameServerInfo;
	CStatic m_static_LogonServerInfo;
public:
	CButton m_bnLogin;
	CButton m_bnLogout;
};
