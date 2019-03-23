#pragma once

#include "Controls\\BkDialogST.h"
#include "Controls\\BtnST.h"
#include "Controls\\dsspanel.h"
#include "Controls\\sscommand.h"
#include "afxwin.h"
#include "DlgShortMain.h"
#include "DlgShortSetup.h"
#include "DlgQuery.h"

// CDlgTestShort 대화 상자입니다.

class CDlgTestShort : public CBkDialogST
{
	DECLARE_DYNAMIC(CDlgTestShort)

public:
	CDlgTestShort(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTestShort();

	void         OnPostEraseBkgnd(CDC* pDC);
	void         InitControls();
	void         SelectDialog(int nSel);
    void         UpdateResultList(long nCAM, long pResData);
	void         WriteTextMessage(CString str);
	void         SetCommStatus(long nSel, BOOL bCheck);
	void         SetTransLanguage(long nValue);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEST_SHORT };

public :
	CButtonST        m_checkCommStat1;
	CButtonST        m_checkCommStat2;

	CDlgShortMain   *m_pDlgShortMain;
    CDlgShortSetup  *m_pDlgShortSetup;
	CDlgQuery       *m_pDlgShortQuery;

	CButtonST	     m_btnToolMain;
	CButtonST	     m_btnToolSearch;
	CButtonST	     m_btnToolSetup;
	CButtonST        m_btnToolReset;
	CButtonST	     m_btnToolExit;
	CButtonST        m_btnRun;
	CDSSPanel	     m_panTime;
	CDSSPanel        m_panMessage;

	BOOL             m_bToggleSW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnBnClickedBtnSetup();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedCheckCommStatus1();
};
