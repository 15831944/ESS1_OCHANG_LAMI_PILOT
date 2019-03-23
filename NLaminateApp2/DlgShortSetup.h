#pragma once
#include "Controls\EditEx.h"
#include "Controls\BtnSt.h"

// CDlgShortSetup dialog

class CDlgShortSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShortSetup)

public:
	CDlgShortSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShortSetup();

	void      InitButtonControls();
	void      UpdateControlsData();
	void      SetTransLanguage(long nValue);

// Dialog Data
	enum { IDD = IDD_DIALOG_SHORT_SETUP };

	CButtonST   m_btnApply;
	CEditEx		m_editCOM1;
	CEditEx		m_editCOM2;

	CEditEx		m_editTestVolt;
	CEditEx		m_editLimitCurr;
	CEditEx		m_editStartDelayTime;
	CEditEx		m_editRampingTime;
	CEditEx		m_editHoldingTime;
	CEditEx		m_editContactCurr;

	long        m_nCOM1;
	long        m_nCOM2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnDeltaposSpinCom1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCom2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
