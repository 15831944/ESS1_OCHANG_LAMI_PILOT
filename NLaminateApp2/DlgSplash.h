//{{AFX_INCLUDES()
#include "Controls\\dsspanel.h"
//}}AFX_INCLUDES

#if !defined(AFX_DLGSPLASH_H__102877D7_8BDC_4B0B_A36E_CB7B21941F26__INCLUDED_)
#define AFX_DLGSPLASH_H__102877D7_8BDC_4B0B_A36E_CB7B21941F26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSplash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash dialog
class CDlgSplash : public CDialog
{
private:
	WORD m_nFinish;

// Construction
public:
	CDlgSplash(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
	//{{AFX_DATA(CDlgSplash)
	enum { IDD = IDD_DIALOG_SPLASH };
	CDSSPanel	m_panMessage;
	CDSSPanel	m_panVersion;
	//}}AFX_DATA


    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSplash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgSplash)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG

	LRESULT OnSplashInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSPLASH_H__102877D7_8BDC_4B0B_A36E_CB7B21941F26__INCLUDED_)
