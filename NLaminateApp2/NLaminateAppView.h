
// NLaminateAppView.h : CNLaminateAppView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CNLaminateAppView();
	DECLARE_DYNCREATE(CNLaminateAppView)

public:
	enum{ IDD = IDD_NLAMINATEAPP_FORM };

    void            InitDialogs();
    void            SelectDialog(int nSel);

// Ư���Դϴ�.
public:
//	CNLaminateAppDoc* GetDocument() const;
	
// �۾��Դϴ�.
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
// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CNLaminateAppView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg LONG OnDeleteDataPath(UINT, LONG);	

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_fmDialog;
};

/*
#ifndef _DEBUG  // NLaminateAppView.cpp�� ����� ����
inline CNLaminateAppDoc* CNLaminateAppView::GetDocument() const
   { return reinterpret_cast<CNLaminateAppDoc*>(m_pDocument); }
#endif
   */

