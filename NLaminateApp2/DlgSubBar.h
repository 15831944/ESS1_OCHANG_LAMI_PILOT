#pragma once

#include "Controls\\BkDialogST.h"
#include "Controls\\BtnST.h"
#include "Controls\\sscommand.h"

// CDlgSubBar 대화 상자입니다.

class CDlgSubBar : public CBkDialogST
{
	DECLARE_DYNAMIC(CDlgSubBar)

public:
	CDlgSubBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSubBar();

	CButtonST     m_btnModel;
	CButtonST     m_btnInsp;
	CButtonST     m_btnVision;
	CButtonST     m_btnReset;
	CButtonST     m_checkCam1;
	CButtonST     m_checkCam2;
	CButtonST     m_checkOkNg;
	CDSSPanel     m_panMessage;
	CSSCommand	  m_btnRun;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SUB_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void         InitButtons();
	void         SetCheckLED(long nCAM, BOOL bOnOff);
	void         SetTransLanguage(long nValue);
	void         WriteTextMessage(CString str);
	void         OnPostEraseBkgnd(CDC* pDC);

	afx_msg void OnBtnModel();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnVision();
	afx_msg void OnBtnInsp();
	afx_msg void OnClickSscommRun();
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedCheckCam1();
};
