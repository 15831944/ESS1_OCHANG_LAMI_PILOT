#if !defined(AFX_DLGMAPPINGSETUP_H__D7F42A2E_B079_4231_97C9_2C9A8F16DFA4__INCLUDED_)
#define AFX_DLGMAPPINGSETUP_H__D7F42A2E_B079_4231_97C9_2C9A8F16DFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMappingSetup.h : header file
//

#include "Controls\RangeSlider.h"
#include "Controls\BtnST.h"
#include "Modules\VisionLib.h"
#include "Controls\EditEx.h"
#include "AxisPlot.h"
#include "ModelData.h"
#include "DlgSliderBarCtrl.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMappingSetup dialog
class CDlgVDisp;
class CDlgMappingSetup : public CDialog
{
// Construction
public:
	CDlgMappingSetup(CWnd* pParent = NULL);   // standard constructor
    ~CDlgMappingSetup();

    void	 InitControls();
	void     UpdateControlsData();
	void	 SetLUTable();
	void	 UpdateProfile();

// Dialog Data
	//{{AFX_DATA(CDlgMappingSetup)
	enum { IDD = IDD_DLG_MAPPING_SETUP };
	CStatic	m_fmProfile;
	BOOL	m_chkEnhence;
	BOOL    m_bCalProc;
	CButtonST	m_btnApply;
	CButtonST   m_btnSave;
	//}}AFX_DATA

public:
  	CWnd   *m_pWndParent;

	CDlgSliderBarCtrl *m_pContSlide;
	CDlgSliderBarCtrl *m_pBrightSlide;
	CDlgSliderBarCtrl *m_pLightSlide;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMappingSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CDlgVDisp            *m_pParent;

    SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
    AXIS_PLOT             m_Axis;
	LPBYTE                m_pImage;
	CRect                 m_rcBounds;
	CEditEx				  m_editScaleFactor;
	CEditEx				  m_editRealSize;

	long                  m_nMaxCount;
	long                  m_pProfileData[IMAGE_LEVEL];

private:
	void	InitSlider();
	void	InitialPlotRect();
	void	DrawPoly(CDC *pDC);
	void	DrawRangerLine(CDC *pDC);
	void	DrawYAxisGrid(CDC *pDC);
	void	DrawXAxisGrid(CDC *pDC);
	void	DrawBasic(CDC *pDC);
	void	DrawProfile(CDC *pDC);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMappingSetup)
	afx_msg void OnPaint();
	afx_msg void OnCheckEnhence();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnItemApply();
	afx_msg void OnSetfocusEditScale();
	//}}AFX_MSG
	LRESULT OnRangeContrastChange(WPARAM wParam, LPARAM  lParam );
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnCal();
	afx_msg void OnBnClickedBtnExeCal();
	afx_msg void OnBnClickedBtnMappingSave();
	afx_msg void OnBnClickedBtnExeCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAPPINGSETUP_H__D7F42A2E_B079_4231_97C9_2C9A8F16DFA4__INCLUDED_)
