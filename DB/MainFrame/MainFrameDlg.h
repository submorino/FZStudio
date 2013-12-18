
// MainFrameDlg.h : header file
//

#pragma once


// CMainFrameDlg dialog
class CMainFrameDlg : public CDialog
{
// Construction
public:
	CMainFrameDlg(CWnd* pParent = NULL);	// standard constructor
	~CMainFrameDlg();

// Dialog Data
	enum { IDD = IDD_MAINFRAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_userinfo;
};
