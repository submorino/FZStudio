
// DKDlg.h : header file
//

#pragma once


// CDKDlg dialog
class CDKDlg : public CDialog
{
// Construction
public:
	CDKDlg(CWnd* pParent = NULL);	// standard constructor
	~CDKDlg();

// Dialog Data
	enum { IDD = IDD_DK_DIALOG };

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
	DECLARE_MESSAGE_MAP()

private:
	HMODULE m_hAntiHook;
};
