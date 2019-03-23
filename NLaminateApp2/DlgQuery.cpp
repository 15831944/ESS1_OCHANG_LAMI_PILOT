// DlgQuery.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgQuery.h"
#include "MainFrm.h"
#include "NAppDll.h"
#include "math.h"
#include "afxdialogex.h"
#include "NPlotDll.h"

// CDlgQuery 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgQuery, CDialogEx)

extern CMainFrame    *m_pMainFrm;
#define _LOG_MAX_COUNT_ 200

CDlgQuery::CDlgQuery(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgQuery::IDD, pParent)
{
	m_nShortQuery = 0;
	m_nPage	= 1;
	m_nTotalPage = 1;
	m_nTotalRecordCount = 0;
	m_nHeadCnt = 0;
	m_nImagePathIndex = -1;
	m_pPageFilePtr = NULL;
}

CDlgQuery::~CDlgQuery()
{
	if (m_pPageFilePtr) delete m_pPageFilePtr;

	NPlotDll_Close(DEF_GAUSE_PLOT_NUM);
	NPlotDll_Close(DEF_LIST_PLOT_NUM);
	NPlotDll_Close(DEF_SHORT_GAUSE_PLOT_NUM);
	NPlotDll_Close(DEF_SHORT_LIST_PLOT_NUM);
}

void CDlgQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_SEARCH, m_listSearch);
	DDX_Control(pDX, IDC_BTN_EXCEL_DATA, m_btnExcelData);
	DDX_Control(pDX, IDC_CHECK_GOOD, m_btnCheckGood);
	DDX_Control(pDX, IDC_CHECK_BAD, m_btnCheckBad);
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_PANEL_FROM_YY, m_panFromYY);
	DDX_Control(pDX, IDC_PANEL_TO_MT, m_panToMT);
	DDX_Control(pDX, IDC_PANEL_TO_DD, m_panToDD);
	DDX_Control(pDX, IDC_PANEL_TO_MM, m_panToMM);
	DDX_Control(pDX, IDC_PANEL_TO_TT, m_panToTT);
	DDX_Control(pDX, IDC_PANEL_TO_YY, m_panToYY);
	DDX_Control(pDX, IDC_PANEL_FROM_MT, m_panFromMT);
	DDX_Control(pDX, IDC_PANEL_FROM_TT, m_panFromTT);
	DDX_Control(pDX, IDC_PANEL_FROM_MM, m_panFromMM);
	DDX_Control(pDX, IDC_PANEL_FROM_DD, m_panFromDD);
	DDX_Control(pDX, IDC_SSPANEL_SEARCH, m_sspanSearch);
	DDX_Control(pDX, IDC_SSPANEL_PAGE, m_panPage);
	DDX_Control(pDX, IDC_BTN_EXPORT, m_btnExport);
	DDX_Control(pDX, IDC_CHECK_SEARCH_CAM1, m_btnCheckCam1);
	DDX_Control(pDX, IDC_CHECK_SEARCH_CAM2, m_btnCheckCam2);	
	DDX_Control(pDX, IDC_COMBO_MODELNAME, m_comboModelName);
	DDX_Control(pDX, IDC_COMBO_LOTID, m_comboLotID);
}

BEGIN_MESSAGE_MAP(CDlgQuery, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgQuery::OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_EXCEL_DATA, &CDlgQuery::OnBtnExcelData)
	ON_BN_CLICKED(IDC_BTN_PREV, &CDlgQuery::OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CDlgQuery::OnBtnNext)
	ON_BN_CLICKED(IDC_CHECK_GOOD, &CDlgQuery::OnCheckGood)
	ON_BN_CLICKED(IDC_CHECK_BAD, &CDlgQuery::OnCheckBad)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_YY, &CDlgQuery::OnDeltaposSpinFromYy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_MM, &CDlgQuery::OnDeltaposSpinFromMm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_DD, &CDlgQuery::OnDeltaposSpinFromDd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_TT, &CDlgQuery::OnDeltaposSpinFromTt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_YY, &CDlgQuery::OnDeltaposSpinToYy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_MM, &CDlgQuery::OnDeltaposSpinToMm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_DD, &CDlgQuery::OnDeltaposSpinToDd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_TT, &CDlgQuery::OnDeltaposSpinToTt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_MT, &CDlgQuery::OnDeltaposSpinFromMt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_MT, &CDlgQuery::OnDeltaposSpinToMt)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CDlgQuery::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CDlgQuery::OnBnClickedBtnExport)
	ON_WM_DRAWITEM()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgQuery, CDialogEx)
	ON_EVENT(CDlgQuery, IDC_LIST_SEARCH, DISPID_CLICK, CDlgQuery::ClickListSearch, VTS_NONE)
	ON_EVENT(CDlgQuery, IDC_LIST_SEARCH, DISPID_DBLCLICK, CDlgQuery::DblclkListSearch, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgQuery 메시지 처리기입니다.
BOOL CDlgQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	InitButtonControls();
	UpdateControlsData();
	InitListHead();

    return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgQuery::SetTransLanguage(long nValue)
{
	if (nValue==0)
	{
		m_btnExcelData.SetWindowText(_T("엑셀저장"));
		m_btnSearch.SetWindowText(_T("자료조회"));
		m_btnExport.SetWindowText(_T("PLOT저장"));
		m_btnNext.SetWindowText(_T("다음"));
		m_btnPrev.SetWindowText(_T("이전"));

		m_btnExcelData.SetTooltipText(_T(" 이 버튼을 실행하면 조회 결과 데이터를 EXCEL화일로 저장합니다. "));
		m_btnSearch.SetTooltipText(_T(" 이 버튼을 실행하면 검색하여 리스트에 출력됩니다. "));
		m_btnExport.SetTooltipText(_T(" 이 버튼을 실행하면 그래프 데이타를 영상파일로 저장됩니다. "));
		m_btnNext.SetTooltipText(_T(" 다음 데이터 리스트로 이동합니다.  "));
		m_btnPrev.SetTooltipText(_T(" 이전 데이터 리스트로 이동합니다. "));
	}
	else if (nValue==1)
	{
		m_btnExcelData.SetWindowText(_T("EXCEL"));
		m_btnSearch.SetWindowText(_T("SEARCH"));
		m_btnExport.SetWindowText(_T("EXPORT"));
		m_btnNext.SetWindowText(_T("NEXT"));
		m_btnPrev.SetWindowText(_T("PREV"));

		m_btnExcelData.SetTooltipText(_T(" When you click this button, the search data to be save as excel file. "));
		m_btnSearch.SetTooltipText(_T(" When you click this button, the search data is displayed to list controls. "));
		m_btnExport.SetTooltipText(_T(" When you click this button, the plot data to be save as image file. "));
		m_btnNext.SetTooltipText(_T(" Goes to the next list data group. "));
		m_btnPrev.SetTooltipText(_T(" Goes to the Previous list data group. "));
	}
	else
	{
		m_btnExcelData.SetWindowText(_T("EXCEL"));
		m_btnSearch.SetWindowText(_T("資料照會"));
		m_btnExport.SetWindowText(_T("PLOT貯藏"));
		m_btnNext.SetWindowText(_T("以後"));
		m_btnPrev.SetWindowText(_T("以前"));

		m_btnExcelData.SetTooltipText(_T(" 实行此按钮，可以把查询结果变换为EXCEL文本进行保存 "));
		m_btnSearch.SetTooltipText(_T(" 实行此按钮，进行查询后输出在LIST表上 "));
		m_btnExport.SetTooltipText(_T(" When you click this button, the plot data to be save as image file. "));
		m_btnNext.SetTooltipText(_T(" 移动到下一个数据表 "));
		m_btnPrev.SetTooltipText(_T(" 移动到上一个数据表 "));
	}
}

void CDlgQuery::InitButtonControls()
{
	WINDOWPLACEMENT wp;
	RECT rc;

	m_btnExcelData.SetIcon(IDI_ICON_EXCEL);
	m_btnExcelData.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnExcelData.DrawBorder(FALSE);
	m_btnExcelData.DrawTransparent();

	m_btnSearch.SetIcon(IDI_ICON_SEARCH);
	m_btnSearch.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnSearch.DrawBorder(FALSE);
	m_btnSearch.DrawTransparent();

	m_btnExport.SetIcon(IDI_ICON_APPLY2);
	m_btnExport.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnExport.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnExport.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnExport.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnExport.DrawBorder(FALSE);
	m_btnExport.DrawTransparent();

	m_btnNext.SetIcon(IDI_ICON_NEXT);
	m_btnNext.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnNext.DrawBorder(FALSE);
	m_btnNext.DrawTransparent();

	m_btnPrev.SetIcon(IDI_ICON_PRIV);
	m_btnPrev.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnPrev.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnPrev.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnPrev.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnPrev.DrawBorder(FALSE);
	m_btnPrev.DrawTransparent();

    GetDlgItem(IDC_SPIN_FROM_YY)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_FROM_YY)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_FROM_MM)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_FROM_MM)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_FROM_DD)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_FROM_DD)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_FROM_TT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_FROM_TT)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_FROM_MT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_FROM_MT)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_TO_YY)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_TO_YY)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_TO_MM)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_TO_MM)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_TO_DD)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_TO_DD)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

    GetDlgItem(IDC_SPIN_TO_TT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_TO_TT)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_TO_MT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_TO_MT)->SetWindowPos(NULL, rc.left, rc.top, 30, 35, SWP_SHOWWINDOW);
	
	m_btnCheckCam1.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnCheckCam1.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnCheckCam1.DrawBorder(FALSE);
	m_btnCheckCam1.DrawTransparent();
	m_btnCheckCam1.SetCheck(TRUE);

	m_btnCheckCam2.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnCheckCam2.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnCheckCam2.DrawBorder(FALSE);
	m_btnCheckCam2.DrawTransparent();
	m_btnCheckCam2.SetCheck(TRUE);

	m_btnCheckBad.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnCheckBad.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnCheckBad.DrawBorder(FALSE);
	m_btnCheckBad.DrawTransparent();
	m_btnCheckBad.SetCheck(TRUE);

	m_btnCheckGood.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnCheckGood.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnCheckGood.DrawBorder(FALSE);
	m_btnCheckGood.DrawTransparent();
	m_btnCheckGood.SetCheck(TRUE);
}

void CDlgQuery::InitPlotControl(long nID)
{
	m_nShortQuery = nID;

	NPlotDll_Open(DEF_GAUSE_PLOT_NUM+nID, (long)GetDlgItem(IDC_STATIC_GAUSE_PLOT));
	NPlotDll_LoadSetupData(DEF_GAUSE_PLOT_NUM+nID, DEF_GAUSPLOT_PATHNAME);
	NPlotDll_SetCaptionX(DEF_GAUSE_PLOT_NUM+nID, _T(""));

	NPlotDll_Open(DEF_LIST_PLOT_NUM+nID, (long)GetDlgItem(IDC_STATIC_LIST_PLOT));
	NPlotDll_LoadSetupData(DEF_LIST_PLOT_NUM+nID, DEF_LISTPLOT_PATHNAME);
	NPlotDll_SetCaptionX(DEF_LIST_PLOT_NUM+nID, _T(""));
}

void CDlgQuery::OnBtnSearch() 
{
	CString strModelName;
	long i;
	UpdateData(true);

	BeginWaitCursor();

	m_comboLotID.GetWindowText(strModelName);
	strModelName = WithoutSpaceToString(strModelName);
//	if (strModelName.GetLength() > 0)
//	{
	_tcscpy_s(m_pMainFrm->m_Search.m_strModelName, strModelName);

	m_nPage	= 1;
	m_nHeadCnt = 0;
	m_nTotalRecordCount = SearchTextDataFile();
	m_nTotalPage = m_nTotalRecordCount /_LOG_MAX_COUNT_ + 1;

	if (m_pPageFilePtr) 
	{
		delete m_pPageFilePtr;
		m_pPageFilePtr = NULL;
	}

	if (m_nTotalPage>0)
	{
		m_pPageFilePtr = new ULONGLONG[m_nTotalPage];
		for(i=0; i<m_nTotalPage; i++)
			m_pPageFilePtr[i] = 0;

		InitListHead();
		SetListData(m_nPage);

		if (m_nTotalRecordCount>0)
    		GetDlgItem(IDC_BTN_EXCEL_DATA)->EnableWindow(TRUE);
	}
//	}

	EndWaitCursor();
}

void CDlgQuery::UpdateControlsData()
{
	CString str, strPathName; 
	CTime tmCurrent	= CTime::GetCurrentTime();

	FillListBoxWithFileName();

	m_btnCheckGood.SetCheck(m_pMainFrm->m_Search.m_bGood);
	m_btnCheckBad.SetCheck(m_pMainFrm->m_Search.m_bBad);

 	m_pMainFrm->m_Search.m_tmFromYY = tmCurrent.GetYear();
	m_pMainFrm->m_Search.m_tmFromMM = tmCurrent.GetMonth();
	m_pMainFrm->m_Search.m_tmFromDD = tmCurrent.GetDay();
	m_pMainFrm->m_Search.m_tmFromTT = tmCurrent.GetHour();
	m_pMainFrm->m_Search.m_tmFromMT = tmCurrent.GetMinute();

	m_pMainFrm->m_Search.m_tmToYY = tmCurrent.GetYear();
	m_pMainFrm->m_Search.m_tmToMM = tmCurrent.GetMonth();
	m_pMainFrm->m_Search.m_tmToDD = tmCurrent.GetDay();
	m_pMainFrm->m_Search.m_tmToTT = tmCurrent.GetHour();
	m_pMainFrm->m_Search.m_tmToMT = tmCurrent.GetMinute();

	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmFromYY);
    m_panFromYY.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmFromMM);
	m_panFromMM.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmFromDD);
	m_panFromDD.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmFromTT);
	m_panFromTT.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmFromMT);
	m_panFromMT.SetCaption(str);

	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmToYY);
    m_panToYY.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmToMM);
	m_panToMM.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmToDD);
	m_panToDD.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmToTT);
	m_panToTT.SetCaption(str);
	str.Format(_T("%d"), m_pMainFrm->m_Search.m_tmToMT);
	m_panToMT.SetCaption(str);

	UpdateData(false);
}

void CDlgQuery::FillListBoxWithFileName()
{
	CFileFind cff;
	long nSel;
    CString str;

	/////////////////////////// 읽을 파일의 path를 지정한다.>>>>>>
	TCHAR FileName[80];
	_stprintf_s(FileName, _T("%s\\*.*"), MODEL_FILE_PATH);
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	if (!cff.FindFile(FileName,0)) return; // FindFile이 함수를 실행한 후에야 										   // FindNextFile 함수등을 쓸수 있다.	
	
	m_comboModelName.ResetContent();

	while(cff.FindNextFile())
    {
		str=cff.GetFileName();
	    if (cff.IsDirectory() && str.Find(_T("."))<0) 
		{
			m_comboModelName.AddString(str);
			nSel = m_comboModelName.GetCurSel();
			m_comboModelName.SetItemHeight(nSel, 20);
		}
    }

   	str=cff.GetFileName();	
	if(cff.IsDirectory() && str.Find(_T("."))<0) 
	{
		m_comboModelName.AddString(str);						// ComboBox에 넣는다.
		nSel = m_comboModelName.GetCurSel();
		m_comboModelName.SetItemHeight(nSel, 20);
	}
    
    cff.Close();
}

void CDlgQuery::InitListHead()
{
	m_nImagePathIndex = -1;
	long i, nColumn=0;
	CString strFlag;

	m_listSearch.SetRows(2);
    m_listSearch.SetRow(0);
	m_listSearch.SetCols(nColumn+1);
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(_T(" NUM. "));
	m_listSearch.SetColWidth(nColumn, 1000);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn); 
	m_listSearch.SetText(_T(" LOT NAME "));
	m_listSearch.SetColWidth(nColumn, 2000);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(_T(" YY-MM-DD "));
	m_listSearch.SetColWidth(nColumn, 1000);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(_T(" HH:MM:SS "));
	m_listSearch.SetColWidth(nColumn, 1000);
	nColumn++;

	for(i=0; i<m_nHeadCnt; i++)
	{
		m_listSearch.SetCols(nColumn+1);
		m_listSearch.SetCol(nColumn);
		m_listSearch.SetText((LPCTSTR)m_strHeadTitle[i]);
		m_listSearch.SetColWidth(nColumn, 1000);
		nColumn++;
	}
}

long CDlgQuery::SearchTextDataFile()
{
	BOOL bRet;
	BOOL bFlag;
	long  nResult, nTemp;
	ULONGLONG nFromTime, nToTime, nMinute;
	CString strSearchPath, strText, str, strT;
	CString strOutText, strFileName, strTemp;
	CFileFind finder;	
	long nFromYear, nFromMonth, nFromDay, nFromHour, nFromMinute;
	long nToYear, nToMonth, nToDay, nToHour, nToMinute;
	CStdioFile fpIn, fpOut;
	
	strFileName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
	bFlag = fpOut.Open(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	if (!bFlag) return 0;

	nFromYear = m_pMainFrm->m_Search.m_tmFromYY;
	nFromMonth = m_pMainFrm->m_Search.m_tmFromMM;
	nFromDay = m_pMainFrm->m_Search.m_tmFromDD;
	nFromHour = m_pMainFrm->m_Search.m_tmFromTT;
	nFromMinute = m_pMainFrm->m_Search.m_tmFromMT;

	strT.Format(_T("%04d"), nFromYear);
	str.Format(_T("%2s%02d%02d%02d%02d"), strT.Mid(2, 2), nFromMonth, nFromDay, nFromHour, nFromMinute);
	nFromTime = _ttoi(str);

	nToYear = m_pMainFrm->m_Search.m_tmToYY;
	nToMonth = m_pMainFrm->m_Search.m_tmToMM;
	nToDay = m_pMainFrm->m_Search.m_tmToDD;
	nToHour = m_pMainFrm->m_Search.m_tmToTT;
	nToMinute = m_pMainFrm->m_Search.m_tmToMT;

	strT.Format(_T("%04d"), nToYear);
	str.Format(_T("%2s%02d%02d%02d%02d"), strT.Mid(2, 2), nToMonth, nToDay, nToHour, nToMinute);
	nToTime = _ttoi(str);

	long nTotalRecordCount = 0;
	while(TRUE)
	{
		if (nFromYear > nToYear) break;
		else if (nFromYear == nToYear)
		{
			if (nFromMonth > nToMonth) break;
			else if (nFromMonth == nToMonth)
			{
				if (nFromDay > nToDay) break;
				else if (nFromDay == nToDay)
				{
					if (nFromHour > nToHour) break;
//					else if (nFromHour == nToHour)
//					{
//						if (nFromMinute > nToMinute) break;
//					}
				}
			}
		}

		bRet = FALSE;

		if (m_nShortQuery)
		{
			strSearchPath.Format(_T("%s%4d\\%02d\\%02d\\%02d.txt"), SAVE_SHORTDATA_PATH, 
																		nFromYear, nFromMonth, nFromDay, nFromHour);
		}
		else
		{
			strSearchPath.Format(_T("%s%4d\\%02d\\%02d\\%02d.txt"), SAVE_DATA_PATH, 
																		nFromYear, nFromMonth, nFromDay, nFromHour);
		}

		if (finder.FindFile(strSearchPath))
		{
			bRet = fpIn.Open(strSearchPath, CFile::modeRead|CFile::typeText);
			if (!bRet) continue;

			do 
			{
				nResult = fpIn.ReadString(strText);
				if (nResult == NULL) 
					break;

				str = WithoutSpaceToString(strText.Mid(0, 20));
				nResult = _tcscmp(str, m_pMainFrm->m_Search.m_strModelName);
				if (nResult!=0 && _tcscmp(m_pMainFrm->m_Search.m_strModelName, _T(""))) continue;

				if (m_pMainFrm->m_Search.m_bGood && !m_pMainFrm->m_Search.m_bBad)
				{
					str = strText.Mid(39, 1);
					if (str != _T("0")) continue;
				}

				if (!m_pMainFrm->m_Search.m_bGood && m_pMainFrm->m_Search.m_bBad)
				{
					str = strText.Mid(39, 1);
					if (str != _T("1")) continue;
				}
    	       
				str.Format(_T("%s%s"), strText.Mid(23, 6), strText.Mid(30, 4));
				nMinute = _ttoi(str);
				if (nMinute<nFromTime || nToTime<nMinute)
					continue;

				strOutText = strText + _T("\n");
				fpOut.WriteString(strOutText);

				if (nTotalRecordCount==0)
				{
					nTemp = 50;
					m_nHeadCnt = 0;
					while(nTemp<strOutText.GetLength()-5)
					{
						if (m_nHeadCnt>=GOM_MAX_NUM) break;

						strTemp = strOutText.Mid(nTemp, 10);
						_tcscpy_s(m_strHeadTitle[m_nHeadCnt], strTemp);
						m_nHeadCnt++;
						nTemp = 50+m_nHeadCnt*23;
					}
				}

				nTotalRecordCount++;
			} while(TRUE);

		    if (bRet) fpIn.Close();
		}

//		nFromMinute++;
//		if (nFromMinute>59)
//		{
		nFromMinute = 0;
		nFromHour++;
		if (nFromHour > 23)
		{
			nFromHour = 0;
			nFromDay++;
			if (nFromDay > 31)
			{
				nFromDay = 1;
				nFromMonth++;
				if (nFromMonth > 12)
				{
					nFromMonth = 1;
					nFromYear++;
				}
			}
		}
    }

	finder.Close();
	fpOut.Close();

	return nTotalRecordCount;
}


void CDlgQuery::SetListData(long nPage)
{
	BOOL bRet;
	long  i, nRow, nColumn, nResult, nCount, nTemp, nDec;
	CString strFileName, strLineText, str;
	CStdioFile file;
	int	 nInitCount;

	strFileName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
	bRet = file.Open(strFileName, CFile::modeRead|CFile::typeText);
	if (!bRet) return;

	if (nPage>1)
		m_btnPrev.EnableWindow(TRUE);
	else
		m_btnPrev.EnableWindow(FALSE);

	if (nPage<m_nTotalPage) 
		m_btnNext.EnableWindow(TRUE);
	else
		m_btnNext.EnableWindow(FALSE);

	str.Format(_T("%d / %d"), nPage, m_nTotalPage);
	m_panPage.SetCaption(str);
	nInitCount = _LOG_MAX_COUNT_ * (nPage - 1);

	nRow = 1;
	nCount = 0;

	m_listSearch.SetRows(1);
	m_listSearch.ShowWindow(SW_HIDE);

	if (m_pPageFilePtr!=NULL)
	{
		file.Seek(m_pPageFilePtr[nPage-1], SEEK_SET);
		do 
		{
			nResult = file.ReadString(strLineText);
			if (nResult == NULL) 
				break;

			nRow++;
			m_listSearch.SetRows(nRow);
			m_listSearch.SetRow(nRow-1);

			nColumn = 0;
			m_listSearch.SetCol(nColumn);
			str.Format(_T("%d"), nCount+1);
			m_listSearch.SetText(str);

    		nColumn++;
			str = strLineText.Mid(0, 20);
			m_listSearch.SetCol(nColumn);
			m_listSearch.SetText(str);

    		nColumn++;
			m_listSearch.SetCol(nColumn);
			str = strLineText.Mid(21, 8);
			m_listSearch.SetText(str);

    		nColumn++;
			m_listSearch.SetCol(nColumn);
			str = strLineText.Mid(30, 6);
			m_listSearch.SetText(str);

			for(i=0; i<m_nHeadCnt; i++)
			{
 				nColumn++;
				nTemp = 64+i*23;
				if (nTemp<strLineText.GetLength()-5)
				{
					m_listSearch.SetCol(nColumn);
					str = strLineText.Mid(63+i*23, 1);
					m_listSearch.SetCellBackColor(RGB(255, 255, 255));

					nDec = _ttoi(str);
					if (nDec == 1) m_listSearch.SetCellBackColor(RGB(255, 0, 0));
					if (nDec == 2) m_listSearch.SetCellBackColor(RGB(255, 255, 0));

					str = strLineText.Mid(64+i*23, 8);
					m_listSearch.SetText(str);
				}
			}

			nCount++;
			if (nCount >= _LOG_MAX_COUNT_) 
			{
				m_pPageFilePtr[nPage] = file.GetPosition();
				break;
			}
		} while(TRUE);

		m_listSearch.ShowWindow(SW_SHOW);
	}

	file.Close();
}

void CDlgQuery::DblclkListSearch()
{
	if(m_nImagePathIndex == -1) return; 

//	strCommand	+=	strPath;
	long nResult = WinExec((LPCSTR)(char *)("c:\\windows\\system32\\mspaint.exe"), SW_SHOW);
	if (nResult==ERROR_FILE_NOT_FOUND)
		int a=0;
}

void CDlgQuery::OnBtnPrev() 
{
	if (m_nPage > 1) m_nPage--;
	SetListData(m_nPage);
}

void CDlgQuery::OnBtnNext() 
{
	if (m_nPage< m_nTotalPage) m_nPage++;
	SetListData(m_nPage);	
}

void CDlgQuery::OnCheckGood() 
{
	m_pMainFrm->m_Search.m_bGood = m_btnCheckGood.GetCheck();
}

void CDlgQuery::OnCheckBad() 
{
	m_pMainFrm->m_Search.m_bBad = m_btnCheckBad.GetCheck();
}

void CDlgQuery::OnDeltaposSpinFromYy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panFromYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panFromYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<2012)  nNumber = 2012;
	else if(nNumber>2030)  nNumber = 2030;

    m_pMainFrm->m_Search.m_tmFromYY = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panFromYY.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinFromMm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panFromMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panFromMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>12)  nNumber = 12;

    m_pMainFrm->m_Search.m_tmFromMM = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panFromMM.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinFromDd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panFromDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panFromDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>31)  nNumber = 31;

    m_pMainFrm->m_Search.m_tmFromDD = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panFromDD.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinFromTt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panFromTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panFromTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>24)  nNumber = 24;

    m_pMainFrm->m_Search.m_tmFromTT = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panFromTT.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinToYy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panToYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panToYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if(nNumber<2012)  nNumber = 2012;
	else if(nNumber>2030)  nNumber = 2030;

    m_pMainFrm->m_Search.m_tmToYY = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panToYY.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinToMm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panToMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panToMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>12)  nNumber = 12;

    m_pMainFrm->m_Search.m_tmToMM = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panToMM.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinToDd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panToDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panToDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>31)  nNumber = 31;

    m_pMainFrm->m_Search.m_tmToDD = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panToDD.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinToTt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panToTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panToTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>24)  nNumber = 24;

    m_pMainFrm->m_Search.m_tmToTT = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panToTT.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinFromMt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panFromMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panFromMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>60)  nNumber = 60;

    m_pMainFrm->m_Search.m_tmFromMT = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panFromMT.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnDeltaposSpinToMt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= _ttoi(m_panToMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= _ttoi(m_panToMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>60)  nNumber = 60;

    m_pMainFrm->m_Search.m_tmToMT = nNumber;
	str.Format(_T("%d"), nNumber);
	m_panToMT.SetCaption(str);

	*pResult = 0;
}

void CDlgQuery::OnBtnExcelData() 
{
	BOOL bRet;
	long i, nRow, nResult, nTemp;
	TCHAR szbuf[128];
	FILE  *fp;
	SYSTEMTIME sysLocalTime;
	CStdioFile file;
	CString strPathName, strLineText, str, strT;

	BeginWaitCursor();

	if (m_nTotalRecordCount>0) 
	{
		strPathName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
		bRet = file.Open(strPathName, CFile::modeRead|CFile::typeText);
		if (!bRet) return;

		GetLocalTime(&sysLocalTime);

//		strPathName = m_pMainFrm->m_Setting.m_strSelDriver;
		strPathName = SAVE_EXCEL_PATH;
		str.Format(_T("%04d\\%02d\\%02d\\%02d\\"), sysLocalTime.wYear, sysLocalTime.wMonth, sysLocalTime.wDay, sysLocalTime.wHour);
		strPathName += str;

		CreateDirectoryFullPath(strPathName);

		if (m_nShortQuery)
			_stprintf_s(szbuf, _T("%sShort_%02d.csv"), strPathName, sysLocalTime.wMinute);
		else
			_stprintf_s(szbuf, _T("%sVision_%02d.csv"), strPathName, sysLocalTime.wMinute);

		_tfopen_s(&fp, szbuf, _T("w+"));

		strT = _T("LOT ID,YY:MM:DD HH:MM:SS,GOOD/BAD,COUNT,");
		for(i=0; i<m_nHeadCnt; i++)
		{
			m_nBadCnt[i] = 0;
			str.Format(_T("%s,"), m_strHeadTitle[i]);
			strT += str;
		}

		if (fp)
			_ftprintf(fp, _T("%s\n"), strT);

		nRow = 1;
		do 
		{
			nResult = file.ReadString(strLineText);
			if (nResult == NULL) 
				break;

			strT = _T("");
			str.Format(_T("%s,"), strLineText.Mid(0, 20));
			strT += str;

			str.Format(_T("%s %s,"), strLineText.Mid(21, 8), strLineText.Mid(30, 7));
			strT += str;

			str = strLineText.Mid(39, 1);
			if (str == _T("0")) strT += _T("OK,");
			else strT += _T("NG,");

			str.Format(_T("%s,"), strLineText.Mid(40, 10));
			strT += str;

			for(i=0; i<m_nHeadCnt; i++)
			{
				nTemp = 64+i*23;
				if (nTemp<strLineText.GetLength()-5)
				{
					str.Format(_T("%s,"), strLineText.Mid(64+i*23, 8));
					strT += str;
				}

				str = strLineText.Mid(63+i*23, 1);
				nTemp = _ttoi(str);
				if (nTemp==1)
				{
					if (!m_pMainFrm->m_Search.m_bGood && m_pMainFrm->m_Search.m_bBad)
						m_nBadCnt[i]++;
				}
			}

			nRow++;
			if (fp)
			    _ftprintf(fp, _T("%s\n"), strT);

		} while(TRUE);

		if (!m_pMainFrm->m_Search.m_bGood && m_pMainFrm->m_Search.m_bBad)
		{
			if (fp)
				_ftprintf(fp, _T("\n\n"));

			strT = _T(" , , ITEM TITLE,");
			for(i=0; i<m_nHeadCnt; i++)
			{
				str.Format(_T("%s,"), m_strHeadTitle[i]);
				strT += str;
			}

			if (fp)
				_ftprintf(fp, _T("%s\n"), strT);		

			strT = _T(" , , BAD  COUNT,");
			for(i=0; i<m_nHeadCnt; i++)
			{
				str.Format(_T("%d,"), m_nBadCnt[i]);
				strT += str;
			}

			if (fp)
				_ftprintf(fp, _T("%s\n"), strT);
		}

		if (fp)
		    fclose(fp);
	}

	GetDlgItem(IDC_BTN_EXCEL_DATA)->EnableWindow(FALSE);
	EndWaitCursor();
}

void CDlgQuery::OnBnClickedBtnLoad()
{
	BOOL bFlag, bDec, bTotalDec;
	long i, nLength, nPos, nCnt;
	CStdioFile fpOut;
	float fV;
	TCHAR szbuf[1000];
	TCHAR *pCh;
	CString strText, str, strT, strL;
	FILE  *fp;
	CString strFileName;
	
	static TCHAR BASED_CODE szFilter[] = _T("Text Files (*.txt)|*.txt|");
	CFileDialog dlg(true, _T("*.txt"), NULL , OFN_HIDEREADONLY, szFilter, NULL);
	if (dlg.DoModal() ==IDOK)
	{
		strFileName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
		bFlag = fpOut.Open(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
		if (!bFlag) return;

		_tprintf_s(szbuf, _T("%s"), dlg.GetPathName());
		_tfopen_s(&fp, szbuf, _T("r"));
		if (fp)
		{
			_fgetts(szbuf, 1000, fp);
			
			strText.Format(_T("%s"), szbuf);
			nLength = strText.GetLength();

			m_nTotalRecordCount = 0;
			m_nHeadCnt = 0;
			nPos = 0;

			for(i=0; i<nLength; i++)
			{
				if (szbuf[i] == '\n') 
				{
					_tcscpy_s(m_strHeadTitle[m_nHeadCnt], strText.Mid(nPos, i-nPos));
					m_nHeadCnt++;
					break;
				}

				if (szbuf[i] == '\t') 
				{
					_tcscpy_s(m_strHeadTitle[m_nHeadCnt], strText.Mid(nPos, i-nPos));
					m_nHeadCnt++;
					nPos = i;
					continue;
				}
			}

			do 
			{
			    pCh = _fgetts(szbuf, 1000, fp);
				if (pCh==NULL) 
					break;

				strText.Format(_T("%s"), szbuf);
			
				nLength = strText.GetLength();
				nPos = 0;
				nCnt = 0;
				strT = _T("");
				bTotalDec = FALSE;
				for(i=0; i<nLength; i++)
				{
					if (szbuf[i] == '\n') 
					{
						fV = (float)_ttof(strText.Mid(nPos, i-nPos));
						bDec = FALSE;
						if ((long)fV == 0) bDec = TRUE;
						if (bDec) bTotalDec = TRUE;

						str.Format(_T("%10s %1d %1d %6.2f "), m_strHeadTitle[nCnt], 0, bDec, fV);
					    strT += str;
						strT += _T("\n");

						strL.Format(_T("%20s %04d%02d%02d %02d%02d%02d 0 %1d"), _T("DefaultModel"), 2012, 8, 22, 4, 4, 4, bTotalDec);
						strL += strT;

						fpOut.WriteString(strL);
						m_nTotalRecordCount++;
						break;
					}

					if (szbuf[i] == '\t') 
					{
						fV = (float)_ttof(strText.Mid(nPos, i-nPos));
						bDec = FALSE;
						if ((long)fV == 0) bDec = TRUE;
						if (bDec) bTotalDec = TRUE;

						str.Format(_T("%10s %1d %1d %6.2f "), m_strHeadTitle[nCnt], 0, bDec, fV);
					    strT += str;

						nPos = i;
						nCnt++;
						continue;
					}
				}
			} while(TRUE);

			fclose(fp);	    
		}

		fpOut.Close();

		m_nPage	= 1;
		m_nTotalPage = m_nTotalRecordCount /_LOG_MAX_COUNT_ + 1;

		if (m_pPageFilePtr) delete m_pPageFilePtr;
		m_pPageFilePtr = new ULONGLONG[m_nTotalPage];
		for(i=0; i<m_nTotalPage; i++)
			m_pPageFilePtr[i] = 0;

		InitListHead();
		SetListData(m_nPage);

		if (m_nTotalRecordCount>0)
    		GetDlgItem(IDC_BTN_EXCEL_DATA)->EnableWindow(TRUE);
	}
}

void CDlgQuery::ClickListSearch()
{
	BOOL bRet, bFlag;
	long i, nRow, nCol, nCnt;
	long  nResult;
	float fLSL, fUSL, fTemp;
	float *fDataX, *fDataY;
	CString strFileName, strLineText, str, strT;
	CStdioFile file;
	INSP_RESULT_DATA *pRes0, *pRes1;

	pRes0 = (INSP_RESULT_DATA *)NAppDll_GetResultData(0);
	pRes1 = (INSP_RESULT_DATA *)NAppDll_GetResultData(1);

	BeginWaitCursor();

	strFileName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
	bRet = file.Open(strFileName, CFile::modeRead|CFile::typeText);
	if (!bRet || m_nTotalRecordCount<=0) 
		goto goto_end;

	fDataX = new float[m_nTotalRecordCount];
	fDataY = new float[m_nTotalRecordCount];

	nRow = m_listSearch.GetRow();
	nCol = m_listSearch.GetCol()-4;

	fLSL = 0.0f;
	fUSL = 0.0f;
	bFlag = FALSE;

	strT = WithoutSpaceToString((CString)m_strHeadTitle[nCol]);
	if (m_nShortQuery==0)
	{
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			if (!_tcscmp(strT, pRes0->m_strTitle[i]))
			{
				fLSL = pRes0->m_dLowLimit[i];
				fUSL = pRes0->m_dHighLimit[i];

				bFlag = TRUE;
				break;
			}
				
			if (!_tcscmp(strT, pRes1->m_strTitle[i]))
			{
				fLSL = pRes1->m_dLowLimit[i];
				fUSL = pRes1->m_dHighLimit[i];

				bFlag = TRUE;
				break;
			}

			if (!_tcscmp(strT, pRes0->m_strLinkTitle[i]))
			{
				fLSL = pRes0->m_dLinkLowLimit[i];
				fUSL = pRes0->m_dLinkHighLimit[i];

				bFlag = TRUE;
				break;
			}
				
			if (!_tcscmp(strT, pRes1->m_strLinkTitle[i]))
			{
				fLSL = pRes1->m_dLinkLowLimit[i];
				fUSL = pRes1->m_dLinkHighLimit[i];

				bFlag = TRUE;
				break;
			}
		}
	}
	else
	{
		fLSL = m_pMainFrm->m_Setting.m_fContactCurr;
		fUSL = m_pMainFrm->m_Setting.m_fLimitCurr;
		bFlag = TRUE;
	}
	
	if (bFlag)
	{
		nCnt = 0;
		if (m_nTotalRecordCount>10 && nRow==1 && nCol>=0 && nCol<m_nHeadCnt)
		{
			do 
			{
				nResult = file.ReadString(strLineText);
				if (nResult == NULL) 
					break;

				str = strLineText.Mid(64+nCol*23, 8);
				fTemp = (float)_ttof(str);
				if (fLSL<=fTemp && fTemp<=fUSL)
				{
					fDataX[nCnt] = (float)nCnt;
					fDataY[nCnt] = fTemp;
					nCnt++;
				}
			} while(TRUE);
		}

		if (nCnt>0)
		{
			if (m_nShortQuery==0)
			{
				NPlotDll_InitialData(DEF_GAUSE_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_GAUSE_PLOT_NUM+m_nShortQuery, (LPCTSTR)strT);
				NPlotDll_SetPlotMode(DEF_LIST_PLOT_NUM+m_nShortQuery, PLOT_MODE_GAUSIAN);
				NPlotDll_SetEnableVertRefLine(DEF_GAUSE_PLOT_NUM+m_nShortQuery, TRUE);
				NPlotDll_SetGausianData(DEF_GAUSE_PLOT_NUM+m_nShortQuery, fLSL, fUSL, nCnt, fDataX, fDataY);

				fTemp = (fUSL-fLSL) * 0.2f;
				NPlotDll_InitialData(DEF_LIST_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_LIST_PLOT_NUM+m_nShortQuery, (LPCTSTR)strT);
				NPlotDll_SetPlotMode(DEF_LIST_PLOT_NUM+m_nShortQuery, PLOT_MODE_XY);
				NPlotDll_SetRangeX(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt);
				NPlotDll_SetRangeLY(DEF_LIST_PLOT_NUM+m_nShortQuery, fLSL-fTemp, fUSL+fTemp);
				NPlotDll_SetData(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt, fDataX, fDataY);
			}
			else
			{
				NPlotDll_InitialData(DEF_GAUSE_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_GAUSE_PLOT_NUM+m_nShortQuery, (LPCTSTR)strT);
				NPlotDll_SetPlotMode(DEF_LIST_PLOT_NUM+m_nShortQuery, PLOT_MODE_GAUSIAN);
				NPlotDll_SetEnableVertRefLine(DEF_GAUSE_PLOT_NUM+m_nShortQuery, TRUE);
				NPlotDll_SetGausianData(DEF_GAUSE_PLOT_NUM+m_nShortQuery, fLSL, fUSL, nCnt, fDataX, fDataY);

				NPlotDll_InitialData(DEF_LIST_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_LIST_PLOT_NUM+m_nShortQuery, (LPCTSTR)strT);
				NPlotDll_SetPlotMode(DEF_LIST_PLOT_NUM+m_nShortQuery, PLOT_MODE_XY);
				NPlotDll_SetRangeX(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt);
				NPlotDll_SetRangeLY(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, 1.0f);
				NPlotDll_SetData(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt, fDataX, fDataY);
			}
		}
	}

	delete fDataX;
	delete fDataY;

	file.Close();

goto_end:
	EndWaitCursor();
}

void CDlgQuery::OnBnClickedBtnExport()
{
	BOOL bRet, bFlag;
	long i, j, nCnt;
	long  nResult;
	float fLSL, fUSL, fTemp;
	float *fDataX, *fDataY;
	SYSTEMTIME sysLocalTime;
	CString strPathName, strFileName, strLineText, str, strT;
	CStdioFile file;
	INSP_RESULT_DATA *pRes0, *pRes1;

	pRes0 = (INSP_RESULT_DATA *)NAppDll_GetResultData(0);
	pRes1 = (INSP_RESULT_DATA *)NAppDll_GetResultData(1);

	BeginWaitCursor();

	GetLocalTime(&sysLocalTime);

//	strPathName = m_pMainFrm->m_Setting.m_strSelDriver;
	strPathName = SAVE_PLOT_PATH;
	str.Format(_T("%04d\\%02d\\%02d\\%02d\\"), sysLocalTime.wYear, sysLocalTime.wMonth, sysLocalTime.wDay, sysLocalTime.wHour);
	strPathName += str;
	CreateDirectoryFullPath(strPathName);

    for(i=0; i<m_nHeadCnt; i++)
	{
		strFileName.Format(_T("%stemp.dat"), SAVE_DATA_PATH);
		bRet = file.Open(strFileName, CFile::modeRead|CFile::typeText);
		if (!bRet || m_nTotalRecordCount<=0) 
			goto goto_end;

		fDataX = new float[m_nTotalRecordCount];
		fDataY = new float[m_nTotalRecordCount];

		fLSL = 0.0f;
		fUSL = 0.0f;
		bFlag = FALSE;

		str = WithoutSpaceToString((CString)m_strHeadTitle[i]);
		if (m_nShortQuery==0)
		{
			for(j=0; j<GOM_MAX_NUM; j++)
			{
				if (!_tcscmp(str, pRes0->m_strTitle[j]))
				{
					fLSL = pRes0->m_dLowLimit[j];
					fUSL = pRes0->m_dHighLimit[j];

					bFlag = TRUE;
					break;
				}

				if (!_tcscmp(str, pRes1->m_strTitle[j]))
				{
					fLSL = pRes1->m_dLowLimit[j];
					fUSL = pRes1->m_dHighLimit[j];

					bFlag = TRUE;
					break;
				}

				if (!_tcscmp(str, pRes0->m_strLinkTitle[j]))
				{
					fLSL = pRes0->m_dLinkLowLimit[j];
					fUSL = pRes0->m_dLinkHighLimit[j];

					bFlag = TRUE;
					break;
				}
				
				if (!_tcscmp(str, pRes1->m_strLinkTitle[j]))
				{
					fLSL = pRes1->m_dLinkLowLimit[j];
					fUSL = pRes1->m_dLinkHighLimit[j];

					bFlag = TRUE;
					break;
				}
			}
		}
		else
		{
			fLSL = m_pMainFrm->m_Setting.m_fContactCurr;
			fUSL = m_pMainFrm->m_Setting.m_fLimitCurr;
			bFlag = TRUE;
		}

		if (bFlag)
		{
			nCnt = 0;
			do 
			{
				nResult = file.ReadString(strLineText);
				if (nResult == NULL) 
					break;

				str = strLineText.Mid(64+i*23, 8);
				fTemp = (float)_ttof(str);
				if (fLSL<=fTemp && fTemp<=fUSL)
				{
					fDataX[nCnt] = (float)nCnt;
					fDataY[nCnt] = fTemp;
				    nCnt++;
				}
			} while(TRUE);
			

			if (nCnt>10)
			{
				NPlotDll_InitialData(DEF_GAUSE_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_GAUSE_PLOT_NUM+m_nShortQuery, (LPCTSTR)str);
				NPlotDll_SetPlotMode(DEF_GAUSE_PLOT_NUM+m_nShortQuery, PLOT_MODE_GAUSIAN);
				NPlotDll_SetEnableVertRefLine(DEF_GAUSE_PLOT_NUM+m_nShortQuery, TRUE);
				NPlotDll_SetGausianData(DEF_GAUSE_PLOT_NUM+m_nShortQuery, fLSL, fUSL, nCnt, fDataX, fDataY);

				str = WithoutSpaceToString((CString)m_strHeadTitle[i]);
				strFileName = strPathName + _T("GauseP_") + str + _T(".bmp");
				NPlotDll_SaveImageData(DEF_GAUSE_PLOT_NUM+m_nShortQuery, strFileName);

				fTemp = (fUSL-fLSL) * 0.2f;
				NPlotDll_InitialData(DEF_LIST_PLOT_NUM+m_nShortQuery);
				NPlotDll_SetCaptionX(DEF_LIST_PLOT_NUM+m_nShortQuery, (LPCTSTR)str);
				NPlotDll_SetPlotMode(DEF_LIST_PLOT_NUM+m_nShortQuery, PLOT_MODE_XY);
				NPlotDll_SetRangeX(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt);
				NPlotDll_SetRangeLY(DEF_LIST_PLOT_NUM+m_nShortQuery, (fLSL-fTemp), (fUSL+fTemp));
				NPlotDll_SetData(DEF_LIST_PLOT_NUM+m_nShortQuery, 0, nCnt, fDataX, fDataY);

				strFileName = strPathName + _T("ListP_") + str + _T(".bmp");
				NPlotDll_SaveImageData(DEF_LIST_PLOT_NUM+m_nShortQuery, strFileName);
			}
		}

		delete fDataX;
		delete fDataY;

		file.Close();
	}

goto_end:
	EndWaitCursor();
}
		


void CDlgQuery::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);
	// TODO: Add your message handler code here
}
