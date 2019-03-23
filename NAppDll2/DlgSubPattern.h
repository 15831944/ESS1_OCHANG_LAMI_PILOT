#if !defined(AFX_DLGSUBPATTERN_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_)
#define AFX_DLGSUBPATTERN_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_

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
class CDlgSubPattern : public CDialog
{
// Construction
public:
	CDlgSubPattern(CWnd* pParent = NULL);   // standard constructor
    void   InitControls();
	void   UpdateControlsData();
    void   ApplyInspData(long nSelectROI);

// Dialog Data
	//{{AFX_DATA(CDlgSubPattern)
	enum { IDD = IDD_DLG_SUB_PATTERN };
	CComboBox	m_comboPatNum;
	long		m_editMatchScore;
	long		m_editSearchRate;
	long        m_editPatSizeX;
	long        m_editPatSizeY;
	long        m_nLocROI;
	CButtonST   m_btnRegist;
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
	afx_msg void OnBtnRegist();
	afx_msg void OnDeltaposSpinMatchScore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSearchRate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPatSizeX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPatSizeY(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnBtnNext();
	afx_msg void OnBtnPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioLocLeft();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBPATTERN_H__7C271199_5087_42A1_9E0E_29556341D110__INCLUDED_)
