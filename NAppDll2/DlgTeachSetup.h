#if !defined(AFX_DLGTEACHSETUP_H__A1F7BDB4_9004_4FC1_80D1_39026CCFD1A1__INCLUDED_)
#define AFX_DLGTEACHSETUP_H__A1F7BDB4_9004_4FC1_80D1_39026CCFD1A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeachSetup.h : header file
//

#include "Modules\VisionLib.h"
#include "ModelData.h"
#include "Controls\BtnST.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMappingSetup dialog
class CDlgVDisp;
class CDlgTeachSetup : public CDialog
{
// Construction
public:
	CDlgTeachSetup(CWnd* pParent = NULL);   // standard constructor

	void     UpdateSelectItemData();
	void     SetMoveROI(BOOL bRet);

// Dialog Data
	//{{AFX_DATA(CDlgTeachSetup)
	enum { IDD = IDD_DLG_TEACH_SETUP };

	CListBox	m_listLinkItem;
	CListBox	m_listLocItem;
	CButtonST   m_btnCopy;
	CButtonST	m_btnApply;
	CButtonST	m_btnTest;
	CButtonST   m_btnSave;
	CButtonST   m_btnLink;
	CButtonST   m_btnLoc;
	CButtonST   m_btnDelete;
	CButtonST   m_btnToolDistPnt2Pnt;
	CButtonST   m_btnToolDistPnt2Line;
	CButtonST   m_btnToolDistLine2Line;
	CButtonST   m_btnToolDistCir2Line;
	CButtonST   m_btnToolAngPnt2Pnt;
    CButtonST   m_btnToolAngLine4Pnt;
	CButtonST   m_btnToolAngLine2Line;
	CButtonST   m_btnToolAng3Pnt;
	CButtonST   m_btnToolDiffPnt2Pnt;
	CButtonST   m_btnToolCirRoi2Roi;
	CString	    m_staticLinkItem1;
	CString	    m_staticLinkItem2;
	CString		m_strLowLimit;
	CString		m_strHighLimit;
	CString     m_strOffsetV;
	CString		m_strOffsetX;
	CString		m_strOffsetY;
	//}}AFX_DATA

public:
  	CWnd	   *m_pWndParent;
	CString     m_strRelation;
	long        m_nLinkType;
	BOOL        m_bRearOption;
	long        m_nSelectedROI1;
	long        m_nSelectedROI2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTeachSetup)
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
    void		InitControls();
	void		LinkRelationObject(long nLinkType);
	void        ToggleButtonControls(long nType);

	// Generated message map functions
	//{{AFX_MSG(CDlgTeachSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnItemApply();
	afx_msg void OnBtnLoc();
	afx_msg void OnBtnLink();
	afx_msg void OnBtnLinkInsert1();
	afx_msg void OnBtnLinkInsert2();
	afx_msg void OnBtnDelete();
	afx_msg void OnCheckNone();
	afx_msg void OnCheckDistPnt2Pnt();
	afx_msg void OnCheckDistLine2Line();
	afx_msg void OnCheckAngPnt2Pnt();
	afx_msg void OnCheckDistPnt2Line();
	afx_msg void OnCheckAngLine2Line();
	afx_msg void OnCheckDiffPnt2Pnt();
	afx_msg void OnSelchangeListItemLink();
	afx_msg void OnBnClickedBtnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_editModifyName;
	afx_msg void OnBnClickedBtnItemCopy();
	afx_msg void OnBnClickedRadioFirstRoi();
	afx_msg void OnBnClickedRadioSecond();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckDispRoi();
	afx_msg void OnStnClickedButtonLeft();
	afx_msg void OnBnClickedBtnCompens();
	afx_msg void OnMenuDist2edge();
	afx_msg void OnMenu1stLineToLine();
	afx_msg void OnMenu2stLineToLine();
	afx_msg void OnMenuNALineToLine();
	CString m_strValueA;

	afx_msg void OnMenuPntLine();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEACHSETUP_H__A1F7BDB4_9004_4FC1_80D1_39026CCFD1A1__INCLUDED_)
