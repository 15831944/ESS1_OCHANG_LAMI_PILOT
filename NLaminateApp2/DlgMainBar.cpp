// DlgToolBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgMainBar.h"
#include "NAppDll.h"
#include "MainFrm.h"
#include "DlgPassWord.h"
#include "mmsystem.h"
#include "afxdialogex.h"

extern CMainFrame   *m_pMainFrm;
// CDlgMainBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMainBar, CDialog)

CDlgMainBar::CDlgMainBar(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgMainBar::IDD, pParent)
{

}

CDlgMainBar::~CDlgMainBar()
{

}

void CDlgMainBar::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_AUTO, m_btnToolMain);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnToolSearch);
	DDX_Control(pDX, IDC_BTN_RESULT, m_btnToolResult);
	DDX_Control(pDX, IDC_BTN_PLOT, m_btnToolPlot);
	DDX_Control(pDX, IDC_BTN_SETUP, m_btnToolSetup);
	DDX_Control(pDX, IDC_BTN_LOAD_IMAGE, m_btnToolLoad);
	DDX_Control(pDX, IDC_BTN_SAVE_IMAGE, m_btnToolSave);
	DDX_Control(pDX, IDC_BTN_GRAB, m_btnToolGrab);
	DDX_Control(pDX, IDC_BTN_LIVE, m_btnToolLive);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnToolExit);
	DDX_Control(pDX, IDC_BTN_ZOOM_IN, m_btnToolZoomIn);
	DDX_Control(pDX, IDC_BTN_ZOOM_OUT, m_btnToolZoomOut);
	DDX_Control(pDX, IDC_BTN_ZOOM_FIT, m_btnToolFit);
	DDX_Control(pDX, IDC_BTN_ZOOM_ORIGIN, m_btnToolOrigin);
	DDX_Control(pDX, IDC_CHECK_COMM_PLC, m_btnToolPlc);
	DDX_Control(pDX, IDC_CHECK_COMM_UMAC, m_btnToolUmac);
}

BEGIN_MESSAGE_MAP(CDlgMainBar, CBkDialogST)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgMainBar::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_AUTO, &CDlgMainBar::OnBnClickedBtnAuto)
	ON_BN_CLICKED(IDC_BTN_SETUP, &CDlgMainBar::OnBnClickedBtnSetup)
	ON_BN_CLICKED(IDC_BTN_PLOT, &CDlgMainBar::OnBnClickedBtnPlot)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgMainBar::OnBnClickedBtnSearch)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_GRAB, &CDlgMainBar::OnBnClickedBtnGrab)
	ON_BN_CLICKED(IDC_BTN_LIVE, &CDlgMainBar::OnBnClickedBtnLive)
	ON_BN_CLICKED(IDC_BTN_ZOOM_IN, &CDlgMainBar::OnBnClickedBtnZoomin)
	ON_BN_CLICKED(IDC_BTN_ZOOM_OUT, &CDlgMainBar::OnBnClickedBtnZoomout)
	ON_BN_CLICKED(IDC_BTN_ZOOM_FIT, &CDlgMainBar::OnBnClickedBtnFit)
	ON_BN_CLICKED(IDC_BTN_ZOOM_ORIGIN, &CDlgMainBar::OnBnClickedBtnOrigin)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, &CDlgMainBar::OnBnClickedBtnLoadImage)
	ON_BN_CLICKED(IDC_BTN_SAVE_IMAGE, &CDlgMainBar::OnBnClickedBtnSaveImage)
	ON_BN_CLICKED(IDC_BTN_RESULT, &CDlgMainBar::OnBnClickedBtnResult)
	ON_BN_CLICKED(IDC_CHECK_COMM_PLC, &CDlgMainBar::OnBnClickedCheckCommPlc)
	ON_BN_CLICKED(IDC_CHECK_COMM_UMAC, &CDlgMainBar::OnBnClickedCheckCommUmac)
END_MESSAGE_MAP()

// CDlgMainBar 메시지 처리기입니다.
BOOL CDlgMainBar::OnInitDialog()
{
	CBkDialogST::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_MAINBAR_WIDTH, DEF_MAINBAR_HEIGHT, SWP_SHOWWINDOW);

	SetBitmap(IDB_BITMAP_SKY);
	InitButtons();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMainBar::OnPostEraseBkgnd(CDC* pDC)
{
	m_btnToolMain.SetBk(pDC);
	m_btnToolSearch.SetBk(pDC);
	m_btnToolResult.SetBk(pDC);
	m_btnToolSetup.SetBk(pDC);
	m_btnToolPlot.SetBk(pDC);
	m_btnToolExit.SetBk(pDC);
	m_btnToolPlc.SetBk(pDC);
	m_btnToolUmac.SetBk(pDC);
}

void CDlgMainBar::InitButtons()
{
	m_btnToolMain.SetIcon(IDI_ICON_TOOL_AUTO);
	m_btnToolMain.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolMain.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolMain.DrawTransparent();
	m_btnToolMain.EnableWindow(FALSE);

	m_btnToolSetup.SetIcon(IDI_ICON_TOOL_SETUP);
	m_btnToolSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolSetup.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolSetup.DrawTransparent();

	m_btnToolPlot.SetIcon(IDI_ICON_TOOL_PLOT);
	m_btnToolPlot.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolPlot.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolPlot.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolPlot.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolPlot.DrawTransparent();
	
	m_btnToolResult.SetIcon(IDI_ICON_TOOL_SEARCH);
	m_btnToolResult.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolResult.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolResult.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolResult.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolResult.DrawTransparent();

	m_btnToolSearch.SetIcon(IDI_ICON_TOOL_SEARCH);
	m_btnToolSearch.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolSearch.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolSearch.DrawTransparent();
	
	m_btnToolLoad.SetIcon(IDI_ICON_TOOL_LOAD);
	m_btnToolLoad.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolLoad.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolLoad.DrawTransparent();
	
	m_btnToolSave.SetIcon(IDI_ICON_TOOL_SAVE);
	m_btnToolSave.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolSave.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolSave.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolSave.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolSave.DrawTransparent();

	m_btnToolGrab.SetIcon(IDI_ICON_TOOL_GRAB);
	m_btnToolGrab.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolGrab.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolGrab.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolGrab.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolGrab.DrawTransparent();

	m_btnToolLive.SetIcon(IDI_ICON_TOOL_LIVE);
	m_btnToolLive.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolLive.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolLive.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolLive.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolLive.DrawTransparent();

	m_btnToolZoomIn.SetIcon(IDI_ICON_TOOL_ZOOMIN);
	m_btnToolZoomIn.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolZoomIn.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolZoomIn.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolZoomIn.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolZoomIn.DrawTransparent();

	m_btnToolZoomOut.SetIcon(IDI_ICON_TOOL_ZOOMOUT);
	m_btnToolZoomOut.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolZoomOut.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolZoomOut.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolZoomOut.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolZoomOut.DrawTransparent();

	m_btnToolFit.SetIcon(IDI_ICON_TOOL_FIT);
	m_btnToolFit.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolFit.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolFit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolFit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolFit.DrawTransparent();

	m_btnToolOrigin.SetIcon(IDI_ICON_TOOL_ORIGIN);
	m_btnToolOrigin.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolOrigin.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolOrigin.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolOrigin.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolOrigin.DrawTransparent();
	
	m_btnToolExit.SetIcon(IDI_ICON_TOOL_EXIT);
	m_btnToolExit.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnToolExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnToolExit.DrawBorder(TRUE);
	m_btnToolExit.DrawTransparent();

	m_btnToolPlc.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnToolPlc.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolPlc.DrawTransparent();
	m_btnToolPlc.DrawBorder(FALSE);
	m_btnToolPlc.EnableWindow(TRUE);

	m_btnToolUmac.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnToolUmac.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnToolUmac.DrawTransparent();
	m_btnToolUmac.DrawBorder(FALSE);
	m_btnToolUmac.EnableWindow(TRUE);

	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);
}

void CDlgMainBar::SetTransLanguage(long nValue)
{
	if (nValue==0)
	{
		m_btnToolMain.SetWindowText(_T("메인"));
		m_btnToolResult.SetWindowText(_T("결과"));
		m_btnToolSetup.SetWindowText(_T("설정"));
		m_btnToolSearch.SetWindowText(_T("조회"));
		m_btnToolPlot.SetWindowText(_T("그래프"));
		m_btnToolExit.SetWindowText(_T("종료"));

		m_btnToolMain.SetTooltipText(_T("이 버튼을 실행하면 메인창이 스크린에 출력됩니다."));
		m_btnToolResult.SetTooltipText(_T("이 버튼을 실행하면 결과창을 스크린에 출력됩니다."));
		m_btnToolSetup.SetTooltipText(_T("이 버튼을 실행하면 설정창이 스크린에 출력됩니다."));
		m_btnToolSearch.SetTooltipText(_T("이 버튼을 실행하면 검색하여 리스트에 출력됩니다."));
		m_btnToolPlot.SetTooltipText(_T("이 버튼을 실행하면 그래프창을 스크린에 출력됩니다."));
		m_btnToolExit.SetTooltipText(_T("이 버튼의 기능은 프로그램을 종료합니다."));
		m_btnToolFit.SetTooltipText(_T("출력창과 Fiting된 영상을 화면에 출력합니다."));
		m_btnToolOrigin.SetTooltipText(_T("1:1 원본 영상을 화면에 출력합니다."));
		m_btnToolZoomOut.SetTooltipText(_T("Zoom Out영상을 화면에 출력합니다."));
		m_btnToolZoomIn.SetTooltipText(_T("Zoom In영상을 화면에 출력합니다."));
		m_btnToolLoad.SetTooltipText(_T("이 버튼을 실행하면 저장된 영상을 화면에 출력합니다."));
		m_btnToolSave.SetTooltipText(_T("이 버튼을 실행하면 영상을 파일로 저장됩니다."));
		m_btnToolGrab.SetTooltipText(_T("Grab 영상을 화면에 출력합니다."));
		m_btnToolLive.SetTooltipText(_T("Live 영상을 화면에 출력합니다."));
	}
	if (nValue==1)
	{
		m_btnToolMain.SetWindowText(_T("MAIN"));
		m_btnToolResult.SetWindowText(_T("RESULT"));
		m_btnToolSetup.SetWindowText(_T("SETTING"));
		m_btnToolSearch.SetWindowText(_T("SEARCH"));
		m_btnToolPlot.SetWindowText(_T("PLOT"));
		m_btnToolExit.SetWindowText(_T("EXIT"));

		m_btnToolExit.SetTooltipText(_T(" The function of this button to terminate the program. "));
		m_btnToolOrigin.SetTooltipText(_T(" The original image is displayed on the screen. "));
		m_btnToolFit.SetTooltipText(_T(" The fitting image is displayed on the screen. "));
		m_btnToolZoomOut.SetTooltipText(_T(" The zoomin image is displayed on the screen. "));
		m_btnToolZoomIn.SetTooltipText(_T(" The zoomout image is displayed on the screen.  "));
		m_btnToolLive.SetTooltipText(_T(" The live vidio images from camera  in real time is displayed on the screen. "));
		m_btnToolGrab.SetTooltipText(_T(" The acquisition images from camera is displayed on the screen. "));
		m_btnToolSave.SetTooltipText(_T(" When you click this button, the image is saved to a file. "));
		m_btnToolLoad.SetTooltipText(_T(" The function of this button to the image output on the screen. "));
		m_btnToolSearch.SetTooltipText(_T(" When you click this button, the query window is displayed on the screen. "));
		m_btnToolResult.SetTooltipText(_T(" When you click this button, the result window is displayed on the screen. "));
		m_btnToolPlot.SetTooltipText(_T(" When you click this button, the plot window is displayed on the screen. "));
		m_btnToolMain.SetTooltipText(_T(" When you click this button, the main window is displayed on the screen. "));
		m_btnToolSetup.SetTooltipText(_T(" When you click this button, the setup window is displayed on the screen. "));
	}
	if (nValue==2)
	{
		m_btnToolMain.SetWindowText(_T("主要"));
		m_btnToolResult.SetWindowText(_T("结果"));
		m_btnToolSetup.SetWindowText(_T("设定"));
		m_btnToolSearch.SetWindowText(_T("查询"));
		m_btnToolPlot.SetWindowText(_T("图形"));
		m_btnToolExit.SetWindowText(_T("结束"));

		m_btnToolMain.SetTooltipText(_T("按这个钮扣就主画面出现在SCREEN上"));
		m_btnToolResult.SetTooltipText(_T("按这个钮扣就结果画面出现在SCREEN上"));
		m_btnToolSetup.SetTooltipText(_T("按这个钮扣就设定画面出现在SCREEN上"));
		m_btnToolSearch.SetTooltipText(_T("按这个钮扣就询问画面出现在SCREEN上"));
		m_btnToolPlot.SetTooltipText(_T("按这个钮扣就图形画面出现在SCREEN上"));
		m_btnToolExit.SetTooltipText(_T("此按钮的功能为，终止系统"));
		m_btnToolFit.SetTooltipText(_T("与输出画面fitting的映像显示在画面上"));
		m_btnToolOrigin.SetTooltipText(_T("把原本影像输出在画面上"));
		m_btnToolZoomOut.SetTooltipText(_T("把Zoom in映像输出在画面上"));
		m_btnToolZoomIn.SetTooltipText(_T("把Zoom out映像输出在画面上"));
		m_btnToolLoad.SetTooltipText(_T("按这个钮扣就贮藏映像出现在SCREEN上"));
		m_btnToolSave.SetTooltipText(_T("按这个钮扣就把映像贮藏为文件形式"));
		m_btnToolGrab.SetTooltipText(_T("把Grab映像输出在画面上"));
		m_btnToolLive.SetTooltipText(_T("把Live映像输出在画面上"));
	}
}

void CDlgMainBar::OnBnClickedBtnAuto() 
{
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	m_btnToolMain.EnableWindow(FALSE);
	m_btnToolResult.EnableWindow(TRUE);
	m_btnToolSetup.EnableWindow(TRUE);
	m_btnToolSearch.EnableWindow(TRUE);
	m_btnToolPlot.EnableWindow(TRUE);
	m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_AUTO);
}

void CDlgMainBar::OnBnClickedBtnResult()
{
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	m_btnToolMain.EnableWindow(TRUE);
	m_btnToolResult.EnableWindow(FALSE);
	m_btnToolSetup.EnableWindow(TRUE);
	m_btnToolSearch.EnableWindow(TRUE);
	m_btnToolPlot.EnableWindow(TRUE);
	m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_RESULT);
}

void CDlgMainBar::OnBnClickedBtnSetup() 
{
	CDlgPassWord dlg;

	if (m_pMainFrm->m_bExecRealTime) return;
	if (dlg.DoModal()==IDOK) 
	{
		m_btnToolMain.EnableWindow(TRUE);
		m_btnToolResult.EnableWindow(TRUE);
		m_btnToolSetup.EnableWindow(FALSE);
		m_btnToolSearch.EnableWindow(TRUE);
		m_btnToolPlot.EnableWindow(TRUE);
		m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_SETUP);
	}
}

void CDlgMainBar::OnBnClickedBtnSearch() 
{
	CDlgPassWord dlg;

	if (m_pMainFrm->m_bExecRealTime) return;
	if (dlg.DoModal()==IDOK) 
	{
		m_btnToolMain.EnableWindow(TRUE);
		m_btnToolResult.EnableWindow(TRUE);
		m_btnToolSetup.EnableWindow(TRUE);
		m_btnToolSearch.EnableWindow(FALSE);
		m_btnToolPlot.EnableWindow(TRUE);
		m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_SEARCH);
	}
}

void CDlgMainBar::OnBnClickedBtnPlot()
{
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	m_btnToolMain.EnableWindow(TRUE);
	m_btnToolResult.EnableWindow(TRUE);
	m_btnToolSetup.EnableWindow(TRUE);
	m_btnToolSearch.EnableWindow(TRUE);
	m_btnToolPlot.EnableWindow(FALSE);
	m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_PLOT);
}

void CDlgMainBar::OnBnClickedBtnExit()
{
	m_pMainFrm->m_pView->SelectDialog(DEF_BUTTON_EXIT);
}

void CDlgMainBar::OnPaint() 
{
	CPaintDC dc(this); 

	CFont *pNewFont, *pOldFont;
	pNewFont = new CFont;
	pNewFont->CreateFont(17, 0, 0, 10, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = dc.SelectObject(pNewFont);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));

	dc.TextOut(510, 2, MAIN_WINDOW_TITLE);	

    dc.SelectObject(pOldFont); 
	delete pNewFont;
}

void CDlgMainBar::OnBnClickedBtnLoadImage()
{
	CString strT, strExt, strFolder;
	long n;
	float fScale;

	if (m_pMainFrm->m_bExecRealTime) return;

	//static TCHAR BASED_CODE szFilter[] = _T("Image Files (*.bmp;*.jpg)|*.bmp;*.jpg|");
	static TCHAR BASED_CODE szFilter[] = _T("Image Files (*.jpg)|*.jpg|");
	CFileDialog dlg(true, _T("*.jpg"), NULL , OFN_HIDEREADONLY, szFilter, NULL);
	if (dlg.DoModal() ==IDOK)
	{
    	m_pMainFrm->LoadImage(dlg.GetPathName());

		strT = dlg.GetPathName();
		n = strT.ReverseFind('\\');
		strFolder = strT.Left(n) + '\\' ;
		strExt = strT.Right(3);
		
		m_pMainFrm->m_strFileType = strExt;
    	m_pMainFrm->m_strFilePath = strFolder;

    	fScale = m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM];
		NAppDll_SetZoomScale(m_pMainFrm->m_nSelectCAM, fScale);
	}
}

void CDlgMainBar::OnBnClickedBtnSaveImage()
{
	if (m_pMainFrm->m_bExecRealTime) return;

	static TCHAR BASED_CODE szFilter[] = _T("Image Files (*.jpg)|*.jpg|");
	CFileDialog dlg(false, _T("*.jpg"), NULL , OFN_HIDEREADONLY, szFilter, NULL);
	if (dlg.DoModal() ==IDOK)
    	m_pMainFrm->SaveImage(dlg.GetPathName());
}

void CDlgMainBar::OnBnClickedBtnGrab()
{
	if (m_pMainFrm->m_bExecRealTime) return;
	m_pMainFrm->Grab();
}

void CDlgMainBar::OnBnClickedBtnLive()
{
	if (m_pMainFrm->m_bExecRealTime) return;
    m_pMainFrm->Live();
}

void CDlgMainBar::OnBnClickedBtnZoomin()
{
	float fScale;

	fScale = m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM]-0.05f;
	if (fScale<DEF_INIT_ZOOM_SCALE) fScale = DEF_INIT_ZOOM_SCALE;
	if (fScale>1.4f) fScale = 1.4f;

	NAppDll_SetZoomScale(m_pMainFrm->m_nSelectCAM, fScale);
	m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM] = fScale;

	m_pMainFrm->m_fZoomScale = fScale;
	m_pMainFrm->UpdateBadImage();
}

void CDlgMainBar::OnBnClickedBtnZoomout()
{
	float fScale;

	fScale = m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM]+0.05f;
	if (fScale<DEF_INIT_ZOOM_SCALE) fScale = DEF_INIT_ZOOM_SCALE;
	if (fScale>1.4f) fScale = 1.4f;

	NAppDll_SetZoomScale(m_pMainFrm->m_nSelectCAM, fScale);
	m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM] = fScale;

	m_pMainFrm->m_fZoomScale = fScale;
	m_pMainFrm->UpdateBadImage();
}

void CDlgMainBar::OnBnClickedBtnFit()
{
	float fScale;

	fScale = DEF_INIT_ZOOM_SCALE;
	NAppDll_SetZoomScale(m_pMainFrm->m_nSelectCAM, fScale);
	m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM] = fScale;

	m_pMainFrm->m_fZoomScale = fScale;
	m_pMainFrm->UpdateBadImage();
}

void CDlgMainBar::OnBnClickedBtnOrigin() 
{
	float fScale;

	fScale = 1.0f;
	NAppDll_SetZoomScale(m_pMainFrm->m_nSelectCAM, fScale);
	m_pMainFrm->m_Setting.m_fCamScale[m_pMainFrm->m_nSelectCAM] = fScale;

	m_pMainFrm->m_fZoomScale = fScale;
	m_pMainFrm->UpdateBadImage();
}

void CDlgMainBar::SetCommStatus(long nSel, BOOL bCheck)
{
	if (nSel==0) m_btnToolPlc.SetCheck(bCheck);
	else  m_btnToolUmac.SetCheck(bCheck);
}

void CDlgMainBar::OnBnClickedCheckCommPlc()
{
	// TODO: Add your control notification handler code here
}

void CDlgMainBar::OnBnClickedCheckCommUmac()
{
	// TODO: Add your control notification handler code here
}
