// DlgShortSetup.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgShortSetup.h"
#include "NPioControlDll.h"
#include "NPlotDll.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgShortSetup dialog
extern CMainFrame    *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgShortSetup, CDialogEx)

CDlgShortSetup::CDlgShortSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShortSetup::IDD, pParent)
{
	m_nCOM1 = 1;
	m_nCOM2 = 2;
}

CDlgShortSetup::~CDlgShortSetup()
{
}

void CDlgShortSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_EDIT_COM1, m_editCOM1);
	DDX_Control(pDX, IDC_EDIT_COM2, m_editCOM2);
	DDX_Control(pDX, IDC_EDIT_TEST_VOLT, m_editTestVolt);
	DDX_Control(pDX, IDC_EDIT_LIMIT_CURR, m_editLimitCurr);
	DDX_Control(pDX, IDC_EDIT_START_DELAY_TIME, m_editStartDelayTime);
	DDX_Control(pDX, IDC_EDIT_VOLT_UP_TIME, m_editRampingTime);
	DDX_Control(pDX, IDC_EDIT_TEST_TIME, m_editHoldingTime);
	DDX_Control(pDX, IDC_EDIT_LIMIT_BAD, m_editContactCurr);
}

BEGIN_MESSAGE_MAP(CDlgShortSetup, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgShortSetup::OnBnClickedBtnApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COM1, &CDlgShortSetup::OnDeltaposSpinCom1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COM2, &CDlgShortSetup::OnDeltaposSpinCom2)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgShortSetup message handlers
BOOL CDlgShortSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitButtonControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShortSetup::SetTransLanguage(long nValue)
{
	if (nValue==0)
	{
		m_btnApply.SetWindowText(_T("저장"));
	}
	else if (nValue==0)
	{
		m_btnApply.SetWindowText(_T("SAVE"));
	}
	else
		m_btnApply.SetWindowText(_T("保存"));
}

void CDlgShortSetup::InitButtonControls()
{
	WINDOWPLACEMENT wp;
	RECT rc;

	m_btnApply.SetIcon(IDI_ICON_SAVE);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnApply.DrawTransparent();

	m_editCOM1.SetBkColor(RGB(255, 255, 255));
	m_editCOM1.SetFontSize(18);

	m_editCOM2.SetBkColor(RGB(255, 255, 255));
	m_editCOM2.SetFontSize(18);

	m_editTestVolt.SetBkColor(RGB(255, 255, 255));
	m_editTestVolt.SetFontSize(18);

	m_editLimitCurr.SetBkColor(RGB(255, 255, 255));
	m_editLimitCurr.SetFontSize(18);

	m_editStartDelayTime.SetBkColor(RGB(255, 255, 255));
	m_editStartDelayTime.SetFontSize(18);

	m_editRampingTime.SetBkColor(RGB(255, 255, 255));
	m_editRampingTime.SetFontSize(18);

	m_editHoldingTime.SetBkColor(RGB(255, 255, 255));
	m_editHoldingTime.SetFontSize(18);

	m_editContactCurr.SetBkColor(RGB(255, 255, 255));
	m_editContactCurr.SetFontSize(18);

	GetDlgItem(IDC_SPIN_COM1)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_COM1)->SetWindowPos(NULL, rc.left, rc.top, 30, 32, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_COM2)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_COM2)->SetWindowPos(NULL, rc.left, rc.top, 30, 32, SWP_SHOWWINDOW);
}

void CDlgShortSetup::UpdateControlsData()
{
	CString str;

	m_nCOM1 = m_pMainFrm->m_Setting.m_nCOM1;
	if (m_nCOM1==0) str = _T("NONE");
	else str.Format(_T("COM%d"), m_nCOM1);
	m_editCOM1.SetTextOut(str);

	m_nCOM2 = m_pMainFrm->m_Setting.m_nCOM2;
	if (m_nCOM2==0) str = _T("NONE");
	else str.Format(_T("COM%d"), m_nCOM2);
	m_editCOM2.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fTestVolt);
	m_editTestVolt.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fLimitCurr);
	m_editLimitCurr.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fStartDelayTime);
	m_editStartDelayTime.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fRampingTime);
	m_editRampingTime.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fHoldingTime);
	m_editHoldingTime.SetTextOut(str);

	str.Format(_T("%5.2f"), m_pMainFrm->m_Setting.m_fContactCurr);
	m_editContactCurr.SetTextOut(str);
}

void CDlgShortSetup::OnBnClickedBtnApply()
{
	CString str, strMessage;
	long nLngSel;
	
	UpdateData(true);

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	if (nLngSel==0) strMessage = _T(" 변경된 데이터를 저장하시겠습니까 ? ");
	else if (nLngSel==1) strMessage = _T(" Do you want apply the changed data ? ");
	else strMessage = _T(" 要保存变更的参数吗 ");
	
	if (MessageBox(strMessage, _T(" Message box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pMainFrm->m_Setting.m_nCOM1 = m_nCOM1;
		m_pMainFrm->m_Setting.m_nCOM2 = m_nCOM2;

		m_pMainFrm->m_Setting.m_fRampingTime = (float)m_editRampingTime.GetTextValue();
		m_pMainFrm->m_Setting.m_fLimitCurr = (float)m_editLimitCurr.GetTextValue();
		m_pMainFrm->m_Setting.m_fHoldingTime = (float)m_editHoldingTime.GetTextValue();
		m_pMainFrm->m_Setting.m_fContactCurr = (float)m_editContactCurr.GetTextValue();
		m_pMainFrm->m_Setting.m_fStartDelayTime = (float)m_editStartDelayTime.GetTextValue();
		m_pMainFrm->m_Setting.m_fTestVolt = (float)m_editTestVolt.GetTextValue();

		NPlotDll_SetPosHoriRefLine(DEF_SHORT_PLOT_NUM, TRUE, m_pMainFrm->m_Setting.m_fContactCurr, 
			                                                 m_pMainFrm->m_Setting.m_fLimitCurr);
//		m_pMainFrm->m_pView->m_pDlgTestShort->m_pDlgShortMain->SetShortTestCondition();
	}
}

void CDlgShortSetup::OnDeltaposSpinCom1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	CString str;

	if (pNMUpDown->iDelta < 0)
	{
		m_nCOM1 -= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		m_nCOM1 -= pNMUpDown->iDelta;
	}
	else 
		return;

	if (m_nCOM1<1)  m_nCOM1 = 0;
	else if(m_nCOM1>6)  m_nCOM1 = 6;

	if (m_nCOM1==0) str = _T("NONE");
	else str.Format(_T("COM%d"), m_nCOM1);
	m_editCOM1.SetTextOut(str);

	*pResult = 0;
}

void CDlgShortSetup::OnDeltaposSpinCom2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	CString str;

	if (pNMUpDown->iDelta < 0)
	{
		m_nCOM2 -= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		m_nCOM2 -= pNMUpDown->iDelta;
	}
	else 
		return;

	if (m_nCOM2<1)  m_nCOM2 = 0;
	else if(m_nCOM2>6)  m_nCOM2 = 6;

	if (m_nCOM2==0) str = _T("NONE");
	else str.Format(_T("COM%d"), m_nCOM2);
	m_editCOM2.SetTextOut(str);

	*pResult = 0;
}

void CDlgShortSetup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	    UpdateControlsData();
}
