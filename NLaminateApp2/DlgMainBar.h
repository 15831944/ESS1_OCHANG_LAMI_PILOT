#pragma once

#include "Controls\\BkDialogST.h"
#include "Controls\\BtnST.h"

// CDlgMainBar 대화 상자입니다.

class CDlgMainBar : public CBkDialogST 
{
	DECLARE_DYNAMIC(CDlgMainBar)

public:
	CDlgMainBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMainBar();

	void         InitButtons();
	void         OnPostEraseBkgnd(CDC* pDC);
	void         SetTransLanguage(long nSel);
	void         SetCommStatus(long nSel, BOOL bCheck);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAIN_BAR };

	CButtonST	 m_btnToolMain;
	CButtonST	 m_btnToolSearch;
	CButtonST	 m_btnToolResult;
	CButtonST	 m_btnToolSetup;
	CButtonST    m_btnToolPlot;
	CButtonST	 m_btnToolLoad;
	CButtonST	 m_btnToolSave;
	CButtonST	 m_btnToolGrab;
	CButtonST    m_btnToolLive;
	CButtonST	 m_btnToolExit;
	CButtonST	 m_btnToolZoomIn;
	CButtonST    m_btnToolZoomOut;
	CButtonST    m_btnToolFit;
	CButtonST    m_btnToolOrigin;
	CButtonST    m_btnToolPlc;
	CButtonST    m_btnToolUmac;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnBnClickedBtnSetup();
	afx_msg void OnBnClickedBtnPlot();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnGrab();
	afx_msg void OnBnClickedBtnLive();
	afx_msg void OnBnClickedBtnZoomin();
	afx_msg void OnBnClickedBtnZoomout();
	afx_msg void OnBnClickedBtnFit();
	afx_msg void OnBnClickedBtnOrigin();
	afx_msg void OnBnClickedBtnLoadImage();
	afx_msg void OnBnClickedBtnSaveImage();
	afx_msg void OnBnClickedBtnResult();
	afx_msg void OnBnClickedCheckCommPlc();
	afx_msg void OnBnClickedCheckCommUmac();
};
