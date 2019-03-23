//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_DLGSETUP_H__795C799A_3D14_446F_AD97_C9D5FC72625A__INCLUDED_)
#define AFX_DLGSETUP_H__795C799A_3D14_446F_AD97_C9D5FC72625A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : header file
//
#include "DlgInspectSetup.h"
#include "DlgMappingSetup.h"
#include "DlgTeachSetup.h"
#include "DlgLightingSetup.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgVisionSetup dialog
typedef enum
{
	TAB_INSPECT=0,
	TAB_TEACH,
	TAB_MAPPING,
	TAB_LIGHTING
};

class CDlgVDisp;
class CDlgVisionSetup : public CDialog
{
// Construction
public:
	CDlgVisionSetup(CWnd* pParent = NULL);   // standard constructor
    ~CDlgVisionSetup();

    void   InitDialogs();
	void   MouseMove(UINT nFlags, CPoint point);
	void   LButtonDown();
    void   UpdateFrameRect();
    long   GetSelectedTab();

public:
	CWnd				 *m_pWndParent;
	CImageList            m_imglistOnOff;

	CDlgMappingSetup	 *m_pMappingDlg;
	CDlgInspectSetup	 *m_pInspectDlg;
	CDlgTeachSetup       *m_pTeachingDlg;
	CDlgLightingSetup    *m_pLightingDlg;

	long				  m_nSelTab;

    // Dialog Data
	//{{AFX_DATA(CDlgVisionSetup)
	enum { IDD = IDD_DLG_VISION_SETUP };
	CTabCtrl	m_ctrlTab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVisionSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CDlgVDisp            *m_pParent;

    SETUP_MODEL_DATA     *m_pData;
	SETUP_MODEL_DATA     *m_pBackData;

	LPBYTE                m_pImage;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgVisionSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeMultiTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__795C799A_3D14_446F_AD97_C9D5FC72625A__INCLUDED_)
