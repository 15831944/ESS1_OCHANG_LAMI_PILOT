#if !defined(AFX_DLGSUBPROFILE_H__44F2B00E_1345_4963_98F6_D57B1B54553D__INCLUDED_)
#define AFX_DLGSUBPROFILE_H__44F2B00E_1345_4963_98F6_D57B1B54553D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubProfile.h : header file
//
#include "Controls\btnst.h"
#include "Modules\TransformLib.h"
#include "Modules\VisionLib.h"
#include "AxisPlot.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSubProfile dialog
class CDlgVDisp;
class CDlgSubProfile : public CDialog
{
// Construction
public:
	CDlgSubProfile(CWnd* pParent = NULL);   // standard constructor
    void     InitControls();
	void     UpdateControlsData();
    void     DrawProfile(CDC *pDC);
    void     AdjustRadioControls(long nSel);
	void     ApplyInspData(long nSelectROI);
	void     DrawScrollBar(CDC *pDC);
	void     SetScrollPosH(long nFirst, long nSecond);
	void     SetScrollPosV(long nFirst, long nSecond);
	void     GetScrollPosH();
	void     GetScrollPosV();
	void     DrawFillCircle(CDC *pDC, long x, long y, long nRadius, COLORREF nColor);

// Dialog Data
	//{{AFX_DATA(CDlgSubProfile)
	enum { IDD = IDD_DLG_SUB_PROFILE };
	CComboBox	m_comboType;
	CComboBox	m_comboSampleNum;
	CComboBox	m_comboOverlapRate;
	CComboBox	m_comboRate;
//	CComboBox	m_comboMethod;
	CButtonST	m_btnPol1;
	CButtonST	m_btnPol2;
    
	long	    m_editSlope;
	long		m_editMaxThr;
	long		m_editMinThr;

	BOOL		m_bCheckScrollX;
	BOOL		m_bCheckScrollY;

	float       m_fFirstH;
	float       m_fSecondH;
	float       m_fFirstV;
	float       m_fSecondV;

	COLORREF	m_colorF;
	COLORREF	m_colorB;

	CRect		m_rtScrollH;
	CRect		m_rtScrollV;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubProfile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	BOOL     m_bCheckVert;
	BOOL     m_bSelLeftPol;
	long     m_nFirstPolDirType;
	long     m_nSecondPolDirType;
	long     m_nFirstScanDirType;
	long     m_nSecondScanDirType;

	long     m_nScanType;
	long     m_nFirstThresh;
	long     m_nSecondThresh;
    long     m_nInspectType;
	long     m_nScanDir;
	long     m_nLocROI;
	long     m_nGOMType;

private:
	void     InitialPlotRect();
    void     DrawBasic(CDC *pDC);
    void     DrawXAxisGrid(CDC *pDC);
    void     DrawYAxisGrid(CDC *pDC);
	void	 DrawRangerLine(CDC *pDC);
    void     DrawPoly(CDC *pDC);
    void     DrawVerticalProfile();
    void     DrawHorizonProfile();
	void	 DrawXProfile(CDC *pDC);
	void	 DrawYProfile(CDC *pDC);
    void     GetXSumGray(LPBYTE m_pImage, long *pFt, long x1, long y1, long x2, long y2, long nPitch);
    void     GetYSumGray(LPBYTE m_pImage, long *pFt, long x1, long y1, long x2, long y2, long nPitch);

// Implementation
protected:
	CDlgVDisp            *m_pParent;
	CTransformLib         m_libTrans;
	CVisionLib            m_libVis;
 
	SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;
    AXIS_PLOT             m_Axis;
	LPBYTE                m_pImage;

	long                  m_nSelectedROI;
	float                 m_dAngle;

	// Generated message map functions
	//{{AFX_MSG(CDlgSubProfile)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioTop1();
	afx_msg void OnRadioBot1();
	afx_msg void OnRadioRight1();
	afx_msg void OnRadioLeft1();
	afx_msg void OnRadioTop2();
	afx_msg void OnRadioBot2();
	afx_msg void OnRadioLeft2();
	afx_msg void OnRadioRight2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeComboMethod();
	afx_msg void OnDeltaposSpinSlope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSlope2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSlope3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboType();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboPol1;
	CComboBox m_comboPol2;
	afx_msg void OnCbnSelchangeComboPol1();
	afx_msg void OnCbnSelchangeComboPol2();
	afx_msg void OnBnClickedBtnPol1();
	afx_msg void OnBnClickedBtnPol2();
	afx_msg void OnBlackWhite();
	afx_msg void OnWhiteBlack();
	afx_msg void OnBnClickedRadioLocLeft();
	afx_msg void OnBnClickedRadioLocRight();
	afx_msg void OnBnClickedRadioMultiple();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBPROFILE_H__44F2B00E_1345_4963_98F6_D57B1B54553D__INCLUDED_)
