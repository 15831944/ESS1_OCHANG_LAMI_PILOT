#pragma once


// CDlgModelName 대화 상자입니다.
class CDlgModelName : public CDialogEx
{
// Construction
public:
	CDlgModelName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModelName)
	enum { IDD = IDD_DIALOG_MODEL_NAME };
	CString	m_sModelName;
	//}}AFX_DATA
	
	CString m_sTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModelName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModelName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};