
// NLaminateAppView.h : CNLaminateAppView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "DlgExecAuto.h"
#include "DlgSetting.h"
#include "DlgPlot.h"
#include "DlgQuery.h"
#include "DlgResult.h"
#include "DlgMainBar.h"
#include "DlgSubBar.h"
#if SHORTAGE_USE
#include "DlgTestShort.h"  
#endif // SHORTAGE_USE
#include "afxwin.h"

class CNLaminateAppView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CNLaminateAppView();
	DECLARE_DYNCREATE(CNLaminateAppView)

public:
	enum{ IDD = IDD_NLAMINATEAPP_FORM };

    void            InitDialogs();
    void            SelectDialog(int nSel);

// 특성입니다.
public:
//	CNLaminateAppDoc* GetDocument() const;
	
// 작업입니다.
public:
	CDlgSetting     *m_pDlgSetting;
	CDlgResult      *m_pDlgResult;
	CDlgQuery       *m_pDlgQuery;
	CDlgPlot        *m_pDlgPlot;
	CDlgExecAuto    *m_pDlgExecAuto;

	CDlgMainBar     *m_pDlgMainBar;
	CDlgSubBar      *m_pDlgSubBar;
#if _SHORTAGE_USE
	CDlgTestShort   *m_pDlgTestShort;
#endif
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CNLaminateAppView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg LONG OnDeleteDataPath(UINT, LONG);	

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_fmDialog;
};

/*
#ifndef _DEBUG  // NLaminateAppView.cpp의 디버그 버전
inline CNLaminateAppDoc* CNLaminateAppView::GetDocument() const
   { return reinterpret_cast<CNLaminateAppDoc*>(m_pDocument); }
#endif
   */

