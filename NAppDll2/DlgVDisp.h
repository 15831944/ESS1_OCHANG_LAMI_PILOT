#if !defined(AFX_DLGVDISP_H__81117BD3_F715_448C_AEBE_21F22F7769FB__INCLUDED_)
#define AFX_DLGVDISP_H__81117BD3_F715_448C_AEBE_21F22F7769FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVDisp.h : header file
//
#include "Modules\ImageModule.h"
#include "Modules\CPUTimer.h"
#include "ModelData.h"
#include "ModuleLink.h"
#include "DlgVisionSetup.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgVDisp dialog
class CDlgVDisp : public CDialog
{
// Construction
public:
	CDlgVDisp(CWnd* pParent = NULL);   // standard constructor
	~CDlgVDisp();   // standard constructor

 	void     (*m_lpCallBack)();  
	void      SetCallBackEvent(long pCallData) { m_pCallData = (NAppDll_DATA *)pCallData; }

	long      GetCameraNum() { return m_nCamID; }
	void      SetCameraNum(long nCamID);
	long      GetRawImagePtr() { return (long)m_pImageModule->m_pRawImage; }
	void      SetRawImagePtr(long pImage);
//	long      GetDisplayImagePtr() { return (long)m_pImageModule->m_pDispImage; }
	void      LoadImageData(LPCTSTR strFileName);

	BOOL      LoadCalFromFile();
	BOOL      SaveCalToFile();
	BOOL      LoadOffsetFile(CString strFileName);

	void      CreateModelData(LPCTSTR strFileName);
	BOOL      LoadModelData(LPCTSTR strFileName);
	void      SaveModelData(LPCTSTR strFileName);

	void      SetImageSize(long nWidth, long nHeight);
	void	  SetFrameSize(long nWidth, long nHeight, long nBand, long nDepth);
//	void      UpdateLUT(long pImage);
//	void      UpdateScrollPos();
	void      SetZoomScale(float fScale);
	void	  SetRemoteLight(BOOL bLight);
	long      GetBrightnessValue(long nCh);

	void	  InvalidateView();
	void	  ShowSetupDialog();
	void      SetEventFuncCall(long nContNo);

	void	  InitResultData();
	long	  GetResultData();
	long	  ExecuteInspection();

// Dialog Data
	//{{AFX_DATA(CDlgVDisp)
	enum { IDD = IDD_DLG_VISION_DISP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CDlgVisionSetup    *m_pDlgVisionSetup;
	CModelData         *m_pModelData;
	CImageModule       *m_pImageModule;
	CModuleLink        *m_pModuleLink;
	NAppDll_DATA       *m_pCallData;
	CCPUTimer           m_clTimer;

	SETUP_MODEL_DATA   *m_pData;
    SETUP_MODEL_DATA   *m_pBackData;
	CAL_DATA            m_pCAL;
	SYSTEM_DATA         m_pSYS;

	CRect               m_rcPlotBounds;
	CString             m_strFullPathName;
	CString             m_strSaveFileName;
    BOOL                m_bLeftBtnDown;
	BOOL                m_bRightBtnDown;
	BOOL                m_bDLeftBtnDown;
	BOOL                m_bModifySetup;
	BOOL                m_bSaveImage;
	long                m_nCamID;
	long                m_nLngSel;


protected:
	CRect      m_rcBound;
	CWnd      *m_pParent;

	CDC        m_memDC;
	CBitmap	   m_bmpBuffer;
	CRect      m_rcRect;
	POINT2     m_ptLStartPnt;
	POINT2     m_ptLEndPnt;
	POINT2     m_ptLDelta;

	CPoint     m_ptRStartPnt;
	CPoint     m_ptREndPnt;

	// Generated message map functions
	//{{AFX_MSG(CDlgVDisp)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLoadImageData();
	afx_msg void OnSaveImageData();
	afx_msg void OnAddRectRoi();
	afx_msg void OnRemoveRoi();
	afx_msg void OnShowSetup();
	afx_msg void OnPaint();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVDISP_H__81117BD3_F715_448C_AEBE_21F22F7769FB__INCLUDED_)
