// KBrecorderDlg.h : header file
//

#if !defined(AFX_KBRECORDERDLG_H__AC4C894F_61E2_4F40_B50E_20FCAF82DA9A__INCLUDED_)
#define AFX_KBRECORDERDLG_H__AC4C894F_61E2_4F40_B50E_20FCAF82DA9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKBrecorderDlg dialog

class CKBrecorderDlg : public CDialog
{
// Construction
public:
	CKBrecorderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKBrecorderDlg)
	enum { IDD = IDD_KBRECORDER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKBrecorderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKBrecorderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KBRECORDERDLG_H__AC4C894F_61E2_4F40_B50E_20FCAF82DA9A__INCLUDED_)
