#if !defined(AFX_DLGSUBFOLDING_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_)
#define AFX_DLGSUBFOLDING_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubPattern.h : header file
//
#include "Controls\btnst.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern dialog
class CDlgVDisp;
class CDlgSubFolding : public CDialog
{
// Construction
public:
	CDlgSubFolding(CWnd* pParent = NULL);   // standard constructor
    void   InitControls();
	void   UpdateControlsData();
    void   ApplyInspData(long nSelectROI);

// Dialog Data
	//{{AFX_DATA(CDlgSubPattern)
	enum { IDD = IDD_DLG_SUB_FOLDING };

	long		m_editArea;
	long		m_editSlope;
	CString     m_editTabColor;
	long        m_editRoiSize;
	long        m_nLocROI;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubPattern)
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
	long                  m_nPage;


	// Generated message map functions
	//{{AFX_MSG(CDlgSubPattern)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinArea(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSlope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinTabColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRoiSize(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioPat4Type();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBPATTERN_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_)
