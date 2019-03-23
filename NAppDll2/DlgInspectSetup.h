#if !defined(AFX_DLGINSPECTSETUP_H__62EA2A31_F2BB_4E92_8355_907CC217BA1E__INCLUDED_)
#define AFX_DLGINSPECTSETUP_H__62EA2A31_F2BB_4E92_8355_907CC217BA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInspectSetup.h : header file
//
#include "Controls\DigitST.h"
#include "Controls\RangeSlider.h"
#include "Controls\BtnST.h"
#include "Modules\VisionLib.h"
#include "Modules\ImageBMP.h"
#include "DlgSubProfile.h"
//#include "DlgSubSLine.h"
#include "DlgSubPattern.h"
//#include "DlgSubFolding.h"
#include "DlgSubBlob.h"
#include "DlgSubEtc.h"

/*
#include "DlgSubDefect.h"
#include "DlgSubMark.h"
"*/
#include "ModelData.h"
#include "resource.h"
#include "afxwin.h"

typedef enum
{
	TAB_PROFILE = 0,
//	TAB_SLINE = 1,
	TAB_PATTERN,
//	TAB_FOLDING,
    	TAB_BLOB,
	TAB_ETC,
	/*

	TAB_BLOB,
	TAB_MARK,
	TAB_DEFECT,
	*/
};

/////////////////////////////////////////////////////////////////////////////
// CDlgInspectSetup dialog
class CDlgVDisp;
class CDlgInspectSetup : public CDialog
{
public:
	CDlgInspectSetup(CWnd* pParent = NULL);   // standard constructor
    ~CDlgInspectSetup();

    void     InitControls();
	void     ChangeControlTAB(long nTab);
    void     InitImageBMP();
	void     UpdateSelectItemData();
	void     CreateSubDialogs();
	void     DisplayImageToFrame(LPBYTE pImage, long nImageWidth, long nImageHeight);
	void     SetSlidePos();
	void     SetComboTitle(long nROI);

public:
  	CWnd				 *m_pWndParent;
	CDlgVDisp            *m_pParent;

	CDlgSubProfile   	 *m_pProfileDlg;
//	CDlgSubSLine         *m_pSLineDlg;
	CDlgSubPattern	     *m_pPatternDlg;
	CDlgSubBlob  	     *m_pBlobDlg;
	CDlgSubEtc           *m_pEtcDlg;

	/*
	CDlgSubFolding       *m_pFoldingDlg;
	CDlgSubMark          *m_pMarkDlg;
	CDlgSubDefect        *m_pDefectDlg;
	*/

    SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
	LPBYTE                m_pImage;

	long				  m_nSelTab;
	float                 m_dAngle;
	BOOL                  m_bHScrollFirst;
	BOOL                  m_bHScrollSecond;
	BOOL                  m_bVScrollFirst;
	BOOL                  m_bVScrollSecond;

	CPoint                m_ptLDelta;
	CPoint                m_ptLStartPnt;

// Dialog Data
	//{{AFX_DATA(CDlgInspectSetup)
	enum { IDD = IDD_DLG_INSPECT_SETUP };
	CTabCtrl	m_ctrlTab;
	CStatic	    m_fmProfile;

	CButtonST	m_btnModify;
	CButtonST	m_btnApply;
	CButtonST	m_btnTest;

	CString		m_editLowLimit;
	CString		m_editHighLimit;
	CString		m_editOffsetV;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInspectSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	BOOL     SetImageMapping(LPBYTE fm_R, LPBYTE fm_G, LPBYTE fm_B);

private:
	CImageBMP            *m_pImageBMP;
	LPBYTE                m_fmImage;

	long                  m_nFrameWidth;
	long                  m_nFrameHeight;
   	BOOL                  m_nToggle;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInspectSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnItemModify();
	afx_msg void OnBtnItemApply();
	afx_msg void OnBtnItemTest();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeMultiTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckDispRoi();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnItemCw();
	afx_msg void OnBtnItemCcw();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangeComboTitle();
	CComboBox m_comboTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINSPECTSETUP_H__62EA2A31_F2BB_4E92_8355_907CC217BA1E__INCLUDED_)
