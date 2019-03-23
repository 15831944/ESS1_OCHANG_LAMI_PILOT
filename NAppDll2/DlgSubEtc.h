#if !defined(AFX_DLGSUBETC_H__FE0DDFA9_91BA_4E60_9B37_32FEF12DCB18__INCLUDED_)
#define AFX_DLGSUBETC_H__FE0DDFA9_91BA_4E60_9B37_32FEF12DCB18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubEtc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSubEtc dialog

#include "Controls\btnst.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSubEtc dialog
class CDlgVDisp;
class CDlgSubEtc : public CDialog
{
// Construction
public:
	CDlgSubEtc(CWnd* pParent = NULL);   // standard constructor

	void     InitControls();
	void     UpdateControlsData();
    void     ApplyInspData(long nSelectROI);

// Dialog Data
	//{{AFX_DATA(CDlgSubEtc)
	enum { IDD = IDD_DLG_SUB_ETC };
	CComboBox	m_comboType;
	CButtonST   m_btnRegist;
	long	m_editGrayValue;
	long    m_editSharpness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubEtc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDlgVDisp            *m_pParent;
	SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
	LPBYTE                m_pImage;

	// Generated message map functions
	//{{AFX_MSG(CDlgSubEtc)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeComboType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioLocRight();
	afx_msg void OnBnClickedBtnRegist();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBETC_H__FE0DDFA9_91BA_4E60_9B37_32FEF12DCB18__INCLUDED_)
