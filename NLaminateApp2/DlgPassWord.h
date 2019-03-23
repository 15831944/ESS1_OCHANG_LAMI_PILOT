#if !defined(AFX_PSWDLG_H__0A2B03EC_EA5A_487F_8079_BA6802A721B7__INCLUDED_)
#define AFX_PSWDLG_H__0A2B03EC_EA5A_487F_8079_BA6802A721B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PswDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPassWord dialog
#include "Controls\\MacButtons.h"

class CDlgPassWord : public CDialog
{
private :
	void CreateXButton();

// Construction
public:
	CDlgPassWord(CWnd* pParent = NULL);   // standard constructor
	CString m_strMsg;
	CString m_strTitleName;
	void    InitControl();

// Dialog Data
	//{{AFX_DATA(CDlgPassWord)
	enum { IDD = IDD_DIALOG_PASSWORD };
	// NOTE: the ClassWizard will add data members here

	CMacMoreButton		m_btnPasswordModify;
	CString				m_strPswEdt;
	CString				m_strOldPswEdt;
	CString				m_strNewPswEdt;
	CString				m_strRePswEdt;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPassWord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect m_rectWindow;
	// Generated message map functions
	//{{AFX_MSG(CDlgPassWord)
	// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPasswordModify();
	afx_msg void OnBtnModify();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSWDLG_H__0A2B03EC_EA5A_487F_8079_BA6802A721B7__INCLUDED_)
