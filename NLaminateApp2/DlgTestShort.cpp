// DlgTestShort.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgTestShort.h"
#include "MainFrm.h"
#include "NPlotDll.h"
#include "NPioControlDll.h"
#include "DlgPassWord.h"
#include "afxdialogex.h"

extern CMainFrame *m_pMainFrm;

// CDlgTestShort 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgTestShort, CDialog)


CDlgTestShort::CDlgTestShort(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTestShort::IDD, pParent)
{
	m_bToggleSW = FALSE;
}

CDlgTestShort::~CDlgTestShort()
{
	if (m_pDlgShortMain)  delete m_pDlgShortMain;
    if (m_pDlgShortSetup) delete m_pDlgShortSetup;
	if (m_pDlgShortQuery) delete m_pDlgShortQuery;
}

void CDlgTestShort::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_COMM_STATUS1, m_checkCommStat1);
	DDX_Control(pDX, IDC_CHECK_COMM_STATUS2, m_checkCommStat2);
	DDX_Control(pDX, IDC_SSPANEL_TIME, m_panTime);
	DDX_Control(pDX, IDC_BTN_AUTO, m_btnToolMain);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnToolSearch);
	DDX_Control(pDX, IDC_BTN_SETUP, m_btnToolSetup);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnToolReset);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnToolExit);
	DDX_Control(pDX, IDC_SSPANEL_MESSAGE, m_panMessage);
	DDX_Control(pDX, IDC_BTN_RUN, m_btnRun);

}

BEGIN_MESSAGE_MAP(CDlgTestShort, CBkDialogST)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_AUTO, &CDlgTestShort::OnBnClickedBtnAuto)
	ON_BN_CLICKED(IDC_BTN_SETUP, &CDlgTestShort::OnBnClickedBtnSetup)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgTestShort::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgTestShort::OnBnClickedBtnExit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RESET, &CDlgTestShort::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RUN, &CDlgTestShort::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_CHECK_COMM_STATUS1, &CDlgTestShort::OnBnClickedCheckCommStatus1)
END_MESSAGE_MAP()

// CDlgTestShort 메시지 처리기입니다.
BOOL CDlgTestShort::OnInitDialog()
{
	CBkDialogST::OnInitDialog();

	InitControls();
	SelectDialog(DEF_BUTTON_SHORT_MAIN);

	SetTimer(1000, 1000, NULL);
	SetTimer(2000, 2000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgTestShort::SetTransLanguage(long nValue)
{
	m_btnRun.SetWindowText(_T("STOP"));

	if (nValue==0)
	{
		m_btnToolMain.SetWindowText(_T("메인"));
		m_btnToolSearch.SetWindowText(_T("조회"));
		m_btnToolSetup.SetWindowText(_T("설정"));
		m_btnToolReset.SetWindowText(_T("초기화"));
		m_btnToolExit.SetWindowText(_T("종료"));
//		m_btnRun.SetWindowText(_T("정지"));

		m_btnToolMain.SetTooltipText(_T("이 버튼을 실행하면 메인창이 스크린에 출력됩니다."));
		m_btnToolSetup.SetTooltipText(_T("이 버튼을 실행하면 설정창이 스크린에 출력됩니다."));
		m_btnToolSearch.SetTooltipText(_T("이 버튼을 실행하면 검색하여 리스트에 출력됩니다."));
		m_btnToolReset.SetTooltipText(_T("이 버튼을 실행하면 데이타가 초기화 됩니다."));
		m_btnToolExit.SetTooltipText(_T("이 버튼의 기능은 프로그램을 종료합니다."));
	}
	else if (nValue==1)
	{
		m_btnToolMain.SetWindowText(_T("AUTO"));
		m_btnToolSetup.SetWindowText(_T("SETUP"));
		m_btnToolSearch.SetWindowText(_T("SEARCH"));
		m_btnToolReset.SetWindowText(_T("RESET"));
		m_btnToolExit.SetWindowText(_T("EXIT"));
//		m_btnRun.SetWindowText(_T("STOP"));

		m_btnToolMain.SetTooltipText(_T(" When you click this button, the main window is displayed on the screen. "));
    	m_btnToolSetup.SetTooltipText(_T(" When you click this button, the setup window is displayed on the screen. "));
	    m_btnToolSearch.SetTooltipText(_T(" When you click this button, the query window is displayed on the screen. "));
		m_btnToolReset.SetTooltipText(_T(" Count data Initialize "));
		m_btnToolExit.SetTooltipText(_T(" The function of this button to terminate the program. "));
	}
	else
	{
		m_btnToolMain.SetWindowText(_T("主要"));
		m_btnToolSearch.SetWindowText(_T("查询"));
		m_btnToolSetup.SetWindowText(_T("设定"));
		m_btnToolReset.SetWindowText(_T("初始化"));
		m_btnToolExit.SetWindowText(_T("结束"));
//		m_btnRun.SetWindowText(_T("停止"));

		m_btnToolMain.SetTooltipText(_T(" 按这个钮扣就主画面出现在SCREEN上 "));
    	m_btnToolSetup.SetTooltipText(_T(" 按这个钮扣就设定画面出现在SCREEN上 "));
	    m_btnToolSearch.SetTooltipText(_T(" 按这个钮扣就询问画面出现在SCREEN上 "));
		m_btnToolReset.SetTooltipText(_T(" 数量进行初期化吗 "));
		m_btnToolExit.SetTooltipText(_T(" 此按钮的功能为，终止系统 "));		
	}
}

void CDlgTestShort::InitControls()
{
    m_pDlgShortMain = new CDlgShortMain((CWnd*)this);
    m_pDlgShortSetup = new CDlgShortSetup((CWnd*)this);
	m_pDlgShortQuery = new CDlgQuery((CWnd*)this);

	m_pDlgShortMain->Create(IDD_DIALOG_SHORT_MAIN, GetDlgItem(IDC_STATIC_SHORT_DIALOG));
	m_pDlgShortSetup->Create(IDD_DIALOG_SHORT_SETUP, GetDlgItem(IDC_STATIC_SHORT_DIALOG));
	m_pDlgShortQuery->Create(IDD_DIALOG_QUERY, GetDlgItem(IDC_STATIC_SHORT_DIALOG));
	m_pDlgShortQuery->InitPlotControl(1);

	m_btnToolMain.SetIcon(IDI_ICON_TOOL_AUTO);
	m_btnToolMain.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolMain.DrawTransparent();

	m_btnToolSetup.SetIcon(IDI_ICON_TOOL_SETUP);
	m_btnToolSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolSetup.DrawTransparent();

	m_btnToolReset.SetIcon(IDI_ICON_RESET);
	m_btnToolReset.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolReset.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolReset.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolReset.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolReset.DrawTransparent();

	m_btnToolSearch.SetIcon(IDI_ICON_TOOL_SEARCH);
	m_btnToolSearch.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolSearch.DrawTransparent();

	m_btnRun.SetIcon(IDI_ICON_TOOL_STOP);
	m_btnRun.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnRun.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnRun.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnRun.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnRun.DrawBorder(TRUE);
	m_btnRun.DrawTransparent();

	m_btnToolExit.SetIcon(IDI_ICON_TOOL_EXIT);
	m_btnToolExit.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolExit.DrawBorder(TRUE);
	m_btnToolExit.DrawTransparent();

	m_checkCommStat1.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_checkCommStat1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_checkCommStat1.DrawBorder(FALSE);
	m_checkCommStat1.DrawTransparent();

	m_checkCommStat2.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_checkCommStat2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_checkCommStat2.DrawBorder(FALSE);
	m_checkCommStat2.DrawTransparent();

	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);
}

void CDlgTestShort::SelectDialog(int nSel)
{
    CString str;

	m_pDlgShortMain->ShowWindow(SW_HIDE);
	m_pDlgShortSetup->ShowWindow(SW_HIDE);
	m_pDlgShortQuery->ShowWindow(SW_HIDE);

	m_btnToolMain.EnableWindow(TRUE);
	m_btnToolSetup.EnableWindow(TRUE);
	m_btnToolSearch.EnableWindow(TRUE);

	switch(nSel)
	{
		case DEF_BUTTON_SHORT_MAIN:
        	m_pDlgShortMain->ShowWindow(SW_SHOW);
			m_btnToolMain.EnableWindow(FALSE);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[MAIN SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_SHORT_SETUP:
        	m_pDlgShortSetup->ShowWindow(SW_SHOW);
			m_btnToolSetup.EnableWindow(FALSE);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[SETUP SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_SHORT_QUERY:
        	m_pDlgShortQuery->ShowWindow(SW_SHOW);
			m_btnToolSearch.EnableWindow(FALSE);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[QUERY SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_SHORT_EXIT:
        	m_pMainFrm->ExitProgram();
			break;
	}
}

void CDlgTestShort::UpdateResultList(long nCAM, long pResData)
{

}

void CDlgTestShort::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}

void CDlgTestShort::OnPostEraseBkgnd(CDC* pDC)
{
	m_btnToolMain.SetBk(pDC);
	m_btnToolSearch.SetBk(pDC);
	m_btnToolSetup.SetBk(pDC);
	m_btnToolExit.SetBk(pDC);
	m_checkCommStat1.SetBk(pDC);
	m_checkCommStat2.SetBk(pDC);
}

void CDlgTestShort::OnBnClickedBtnAuto()
{
	SelectDialog(DEF_BUTTON_SHORT_MAIN);
}


void CDlgTestShort::OnBnClickedBtnSetup()
{
	CDlgPassWord dlg;

	if (m_pMainFrm->m_bExecRealTime || m_pMainFrm->m_bShortTest) return;
	if (dlg.DoModal()==IDOK) 
	    SelectDialog(DEF_BUTTON_SHORT_SETUP);
}

void CDlgTestShort::OnBnClickedBtnSearch()
{
	CDlgPassWord dlg;

	if (m_pMainFrm->m_bExecRealTime) return;
	if (dlg.DoModal()==IDOK) 
	    SelectDialog(DEF_BUTTON_SHORT_QUERY);
}

void CDlgTestShort::OnBnClickedBtnExit()
{
	SelectDialog(DEF_BUTTON_SHORT_EXIT);
}

void CDlgTestShort::WriteTextMessage(CString str)
{
	m_panMessage.SetWindowText(str);
}

void CDlgTestShort::SetCommStatus(long nSel, BOOL bCheck)
{
	if (nSel==0) m_checkCommStat1.SetCheck(bCheck);
	else  m_checkCommStat2.SetCheck(bCheck);
}

void CDlgTestShort::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bRet=TRUE;

	if (nIDEvent==1000)
	{
		CString str;
		CTime tmCurrent	= CTime::GetCurrentTime();

		str.Format(_T("%04dY %02dM %02dD   %02d : %02d : %02d"), 
		         	tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), tmCurrent.GetHour(), tmCurrent.GetMinute(), tmCurrent.GetSecond());
		m_panTime.SetCaption(str);
	}

	if (nIDEvent==2000)
	{
		if (!m_bToggleSW)
		{
			bRet = NComPort_GetVersion(m_pMainFrm->m_Setting.m_nCOM1);
			//bRet = m_pDlgShortMain->GetTesterVersion(m_pMainFrm->m_Setting.m_nCOM1);
			if (!bRet) SetCommStatus(0, TRUE);
			else SetCommStatus(0, FALSE);
		}
		else
		{
			bRet = NComPort_GetVersion(m_pMainFrm->m_Setting.m_nCOM2);
//			bRet = m_pDlgShortMain->GetTesterVersion(m_pMainFrm->m_Setting.m_nCOM2);
			if (!bRet) SetCommStatus(1, TRUE);
			else SetCommStatus(1, FALSE);
		}

		m_bToggleSW = !m_bToggleSW;
	}
	
	CBkDialogST::OnTimer(nIDEvent);
}

void CDlgTestShort::OnBnClickedBtnReset()
{
	CString strT;
	long nLngSel;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	if (m_pMainFrm->m_bExecRealTime || m_pMainFrm->m_bShortTest) return;

	if (nLngSel==0) strT = _T(" 검사 수량을 초기화하시겠습니까 ? ");
	else if (nLngSel==1) strT = _T(" Do you want to reset count ? ");
	else strT = _T(" 数量进行初期化吗？ ");

	if (MessageBox(strT, _T(" MESSAGE BOX "), MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		m_pMainFrm->m_Setting.m_nTotalShortCount = 0;

		m_pDlgShortMain->InitListControls();
		m_pDlgShortMain->InitShortProc();
//		NPlotDll_InitialData(DEF_SHORT_PLOT_NUM);

		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'S');
		NComPort_ClearCount(m_pMainFrm->m_Setting.m_nCOM1);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'R');

		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'S');
		NComPort_ClearCount(m_pMainFrm->m_Setting.m_nCOM2);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'R');
	}
}

void CDlgTestShort::OnBnClickedBtnRun()
{
	long nLngSel;
	BeginWaitCursor();
	
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	if (!m_pMainFrm->m_bExecRealTime && !m_pMainFrm->m_bShortTest)
	{
		KillTimer(2000);
		m_pMainFrm->m_bShortTest = TRUE;
		m_pDlgShortMain->InitShortProc();
		m_btnRun.SetIcon(IDI_ICON_TOOL_RUN);

		m_btnRun.SetWindowText(_T("RUN"));
	}
	else
	{
		if (!m_pMainFrm->m_bExecRealTime)
		     SetTimer(2000, 2000, NULL);

		m_pMainFrm->m_bShortTest = FALSE;
		m_pDlgShortMain->CloseShortProc();
		m_btnRun.SetIcon(IDI_ICON_TOOL_STOP);
		
		m_btnRun.SetWindowText(_T("STOP"));
	}

  	EndWaitCursor();
}

void CDlgTestShort::OnBnClickedCheckCommStatus1()
{
	// TODO: Add your control notification handler code here
}
