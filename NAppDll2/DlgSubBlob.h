#if !defined(AFX_DLGSUBBLOB_H__E62179AE_C616_4E36_93BB_28739DB178C3__INCLUDED_)
#define AFX_DLGSUBBLOB_H__E62179AE_C616_4E36_93BB_28739DB178C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubBlob.h : header file
//
#include "Controls\btnst.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSubBlob dialog
class CDlgVDisp;
class CDlgSubBlob : public CDialog
{
// Construction
public:
	CDlgSubBlob(CWnd* pParent = NULL);   // standard constructor

	void     InitControls();
	void     UpdateControlsData();
    void     ApplyInspData(long nSelectedROI);

// Dialog Data
	//{{AFX_DATA(CDlgSubBlob)
	enum { IDD = IDD_DLG_SUB_BLOB };
	CButtonST	          m_ctrlPolarity;
	CScrollBar	          m_ctrlThresh;
	long	m_editThresh;
	CString	m_editMaxArea;
	CString	m_editMinArea;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubBlob)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:

// Implementation
protected:
	CDlgVDisp            *m_pParent;
 
	SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
	LPBYTE                m_pImage;
	long                  m_nPolarity;

	// Generated message map functions
	//{{AFX_MSG(CDlgSubBlob)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlack();
	afx_msg void OnWhite();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnRegist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCheckMinMaxDist();
	afx_msg void OnSetfocusEditMinArea();
	afx_msg void OnSetfocusEditMaxArea();
	//}}AFX_MSG

	LRESULT On_MenuCallback(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPolarity();
	afx_msg void OnBnClickedRadioLocRight();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBBLOB_H__E62179AE_C616_4E36_93BB_28739DB178C3__INCLUDED_)
