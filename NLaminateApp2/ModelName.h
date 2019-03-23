//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_MODELNAME_H__97DDC3C4_FB3B_46F5_BC0E_CA3AB3FEC056__INCLUDED_)
#define AFX_MODELNAME_H__97DDC3C4_FB3B_46F5_BC0E_CA3AB3FEC056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelName dialog

class CModelName : public CDialog
{
// Construction
public:
	CModelName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelName)
	enum { IDD = IDD_DIALOG_MODELNAME };
	CString	m_sModelName;
	//}}AFX_DATA
	
	CString m_sTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELNAME_H__97DDC3C4_FB3B_46F5_BC0E_CA3AB3FEC056__INCLUDED_)
