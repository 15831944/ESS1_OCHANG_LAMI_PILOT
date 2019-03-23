// DlgSearch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgSearch.h"
#include "MainFrm.h"
#include "afxdialogex.h"
#include "NPlotDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame    *m_pMainFrm;
#define _LOG_MAX_COUNT_ 200

long    m_nHeadCnt;
char    m_strHeadTitle[GOM_MAX_NUM][100];

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch dialog

CDlgSearch::CDlgSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSearch::IDD, pParent)
{
	m_nPage	= 1;
	m_nTotalPage = 1;
	m_nTotalRecordCount = 0;
	m_nHeadCnt = 0;
	m_nImagePathIndex = -1;
	m_pPageFilePtr = NULL;
}

CDlgSearch::~CDlgSearch()
{
	if (m_pPageFilePtr) delete m_pPageFilePtr;

	NPlotDll_Close(DEF_GAUSE_PLOT_NUM);
}

void CDlgSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgSearch)
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
	DDX_Control(pDX, IDC_SSPANEL_PAGE, m_panPage);
	DDX_Control(pDX, IDC_CHECK_SEARCH_CAM1, m_btnCheckCam1);
	DDX_Control(pDX, IDC_CHECK_SEARCH_CAM2, m_btnCheckCam2);	
	DDX_Control(pDX, IDC_COMBO_MODELNAME, m_comboModelName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSearch, CDialogEx)
	//{{AFX_MSG_MAP(CDlgSearch)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgSearch::OnBtnSearch)
	ON_LBN_DBLCLK(IDC_LIST_SEARCH, &CDlgSearch::OnDblclkListSearch)
	ON_BN_CLICKED(IDC_BTN_EXCEL_DATA, &CDlgSearch::OnBtnExcelData)
	ON_BN_CLICKED(IDC_BTN_PREV, &CDlgSearch::OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CDlgSearch::OnBtnNext)
	ON_BN_CLICKED(IDC_CHECK_GOOD, &CDlgSearch::OnCheckGood)
	ON_BN_CLICKED(IDC_CHECK_BAD, &CDlgSearch::OnCheckBad)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_YY, &CDlgSearch::OnDeltaposSpinFromYy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_MM, &CDlgSearch::OnDeltaposSpinFromMm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_DD, &CDlgSearch::OnDeltaposSpinFromDd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_TT, &CDlgSearch::OnDeltaposSpinFromTt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_YY, &CDlgSearch::OnDeltaposSpinToYy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_MM, &CDlgSearch::OnDeltaposSpinToMm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_DD, &CDlgSearch::OnDeltaposSpinToDd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_TT, &CDlgSearch::OnDeltaposSpinToTt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FROM_MT, &CDlgSearch::OnDeltaposSpinFromMt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TO_MT, &CDlgSearch::OnDeltaposSpinToMt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSearch message handlers
BOOL CDlgSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	InitButtonControls();
	UpdateControlsData();
	InitListHead();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSearch::InitButtonControls()
{
	WINDOWPLACEMENT wp;
	RECT rc;

	NPlotDll_Open(DEF_GAUSE_PLOT_NUM, (long)GetDlgItem(IDC_STATIC_GAUSE_PLOT));
	NPlotDll_LoadSetupData(DEF_GAUSE_PLOT_NUM, DEF_GAUSPLOT_PATHNAME);
	NPlotDll_SetRangeX(DEF_GAUSE_PLOT_NUM, 0.0, 100.0);

	m_btnExcelData.SetIcon(IDI_ICON_EXCEL);
	m_btnExcelData.SetTooltipText(_T("검색된 Data를 Excel파일로 저장합니다."));
	m_btnExcelData.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnExcelData.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnExcelData.DrawBorder(FALSE);
	m_btnExcelData.DrawTransparent();

	m_btnSearch.SetIcon(IDI_ICON_SEARCH);
	m_btnSearch.SetTooltipText(_T("DATA를 검색 합니다."));
	m_btnSearch.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnSearch.DrawBorder(FALSE);
	m_btnSearch.DrawTransparent();

	m_btnNext.SetIcon(IDI_ICON_NEXT);
	m_btnNext.SetTooltipText(_T("다음 화면으로 갑니다."));
	m_btnNext.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnNext.DrawBorder(FALSE);
	m_btnNext.DrawTransparent();

	m_btnPrev.SetIcon(IDI_ICON_PRIV);
	m_btnPrev.SetTooltipText(_T("이전 화면으로 갑니다."));
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

void CDlgSearch::OnBtnSearch() 
{
	CString strModelName;
	long i;
	UpdateData(true);

	m_comboModelName.GetWindowTextA(strModelName);
	if (strModelName.GetLength() > 0)
	{
		strcpy_s(m_pMainFrm->m_Search.m_strModelName, strModelName);

		m_nPage	= 1;
		m_nHeadCnt = 0;
		m_nTotalRecordCount = SearchTextDataFile();
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

void CDlgSearch::UpdateControlsData()
{
	CString str, strPathName; 
	CTime tmCurrent	= CTime::GetCurrentTime();

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

	str.Format("%d", m_pMainFrm->m_Search.m_tmFromYY);
    m_panFromYY.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmFromMM);
	m_panFromMM.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmFromDD);
	m_panFromDD.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmFromTT);
	m_panFromTT.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmFromMT);
	m_panFromMT.SetCaption(str);

	str.Format("%d", m_pMainFrm->m_Search.m_tmToYY);
    m_panToYY.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmToMM);
	m_panToMM.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmToDD);
	m_panToDD.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmToTT);
	m_panToTT.SetCaption(str);
	str.Format("%d", m_pMainFrm->m_Search.m_tmToMT);
	m_panToMT.SetCaption(str);

	UpdateData(false);
}

void CDlgSearch::InitListHead()
{
	m_nImagePathIndex = -1;
	long i, nColumn=0;
	CString strFlag;

	m_listSearch.SetRows(2);
    m_listSearch.SetRow(0);
	m_listSearch.SetCols(nColumn+1);
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(" NUM. ");
	m_listSearch.SetColWidth(nColumn, 1000);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn); 
	m_listSearch.SetText(" MODEL NAME ");
	m_listSearch.SetColWidth(nColumn, 2500);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(" YY-MM-DD ");
	m_listSearch.SetColWidth(nColumn, 1300);
	nColumn++;

	m_listSearch.SetCols(nColumn+1);	
	m_listSearch.SetCol(nColumn);
	m_listSearch.SetText(" HH:MM:SS ");
	m_listSearch.SetColWidth(nColumn, 1300);
	nColumn++;

	for(i=0; i<m_nHeadCnt; i++)
	{
		m_listSearch.SetCols(nColumn+1);
		m_listSearch.SetCol(nColumn);
		m_listSearch.SetText(m_strHeadTitle[i]);
		m_listSearch.SetColWidth(nColumn, 1500);
		nColumn++;
	}
}

long CDlgSearch::SearchTextDataFile()
{
	BOOL bRet;
	BOOL bFlag;
	long  nResult, nTemp;
	CString strSearchPath, strText, str;
	CString strOutText, strFileName, strTemp;
	CFileFind finder;	
	long nFromYear, nFromMonth, nFromDay, nFromHour, nFromMinute;
	long nToYear, nToMonth, nToDay, nToHour, nToMinute;
	CStdioFile fpIn, fpOut;
	
	strFileName.Format("%s%stemp.dat", m_pMainFrm->m_Setting.m_strSelDriver, SAVE_DATA_PATH);
	bFlag = fpOut.Open(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	if (!bFlag) return 0;

	nFromYear = m_pMainFrm->m_Search.m_tmFromYY;
	nFromMonth = m_pMainFrm->m_Search.m_tmFromMM;
	nFromDay = m_pMainFrm->m_Search.m_tmFromDD;
	nFromHour = m_pMainFrm->m_Search.m_tmFromTT;
	nFromMinute = m_pMainFrm->m_Search.m_tmFromMT;

	nToYear = m_pMainFrm->m_Search.m_tmToYY;
	nToMonth = m_pMainFrm->m_Search.m_tmToMM;
	nToDay = m_pMainFrm->m_Search.m_tmToDD;
	nToHour = m_pMainFrm->m_Search.m_tmToTT;
	nToMinute = m_pMainFrm->m_Search.m_tmToMT;

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
					else if (nFromHour == nToHour)
					{
						if (nFromMinute > nToMinute) break;
					}
				}
			}
		}

		bRet = FALSE;
		strSearchPath.Format("%s%s\\%4d\\%02d\\%02d\\%02d\\%02d.txt", m_pMainFrm->m_Setting.m_strSelDriver, SAVE_DATA_PATH, 
																		nFromYear, nFromMonth, nFromDay, nFromHour, nFromMinute);
		if (finder.FindFile(strSearchPath))
		bRet = fpIn.Open(strSearchPath, CFile::modeRead|CFile::typeText);

        do 
		{
			nResult = fpIn.ReadString(strText);
			if (nResult == NULL) continue;

			if (m_pMainFrm->m_Search.m_bGood && !m_pMainFrm->m_Search.m_bBad)
			{
				str = strText.Mid(37, 1);
				if (str != "0") continue;
			}

			if (!m_pMainFrm->m_Search.m_bGood && m_pMainFrm->m_Search.m_bBad)
			{
				str = strText.Mid(37, 1);
				if (str != "1") continue;
			}
    	       
			strOutText = strText + "\n";
			fpOut.WriteString(strOutText);

			if (nTotalRecordCount==0)
			{
				nTemp = 39;
				m_nHeadCnt = 0;
				while(nTemp<strOutText.GetLength()-10)
				{
					if (m_nHeadCnt>=GOM_MAX_NUM) break;

					strTemp = strOutText.Mid(nTemp, 10);
					strcpy_s(m_strHeadTitle[m_nHeadCnt], strTemp);
					m_nHeadCnt++;
					nTemp = 39+m_nHeadCnt*20;
				}
			}

			nTotalRecordCount++;
		} while(TRUE);

	    if (bRet) fpIn.Close();

		nFromMinute++;
		if (nFromMinute>59)
		{
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
    }

	finder.Close();
	fpOut.Close();

	return nTotalRecordCount;
}

void CDlgSearch::SetListData(long nPage)
{
	BOOL bRet;
	long  i, nRow, nColumn, nResult, nCount, nTemp;
	CString strFileName, strLineText, str;
	CStdioFile file;
	int	 nInitCount;

	strFileName.Format("%s%stemp.dat", m_pMainFrm->m_Setting.m_strSelDriver, SAVE_DATA_PATH);
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

	str.Format("%d / %d", nPage, m_nTotalPage);
	m_panPage.SetCaption(str);
	nInitCount = _LOG_MAX_COUNT_ * (nPage - 1);

	nRow = 1;
	nCount = 0;

	m_listSearch.SetRows(1);
	m_listSearch.ShowWindow(SW_HIDE);
	file.Seek(m_pPageFilePtr[nPage-1], SEEK_SET);

	do 
	{
		nResult = file.ReadString(strLineText);
	    if (nResult == NULL) break;

		nRow++;
		m_listSearch.SetRows(nRow);
		m_listSearch.SetRow(nRow-1);

		nColumn = 0;
		m_listSearch.SetCol(nColumn);
		str.Format("%d", nCount+1);
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
			nTemp = 60+i*20;
			if (nTemp<strLineText.GetLength()-10)
			{
				m_listSearch.SetCol(nColumn);
				str = strLineText.Mid(60+i*20, 1);
				m_listSearch.SetCellBackColor(RGB(255, 255, 255));
				if (atoi(str) != 0)
					m_listSearch.SetCellBackColor(RGB(255, 0, 0));

				str = strLineText.Mid(62+i*20, 10);
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
	file.Close();
}

void CDlgSearch::OnDblclkListSearch() 
{
	CString strCommand;

	if(m_nImagePathIndex == -1) return; 
//	if(m_nImagePathIndex-1 != //m_GridList.GetColSel()) return;
//	CString strPath	=	//m_GridList.GetText();
//	if(strPath.GetLength()==0) return;

	strCommand	=	"c:\\windows\\system32\\mspaint.exe ";
//	strCommand	+=	strPath;

	long nResult = WinExec(strCommand, SW_SHOW);
	if (nResult==ERROR_FILE_NOT_FOUND)
		int a=0;
}

void CDlgSearch::OnBtnPrev() 
{
	if (m_nPage > 1) m_nPage--;
	SetListData(m_nPage);
}

void CDlgSearch::OnBtnNext() 
{
	if (m_nPage< m_nTotalPage) m_nPage++;
	SetListData(m_nPage);	
}

void CDlgSearch::OnCheckGood() 
{
	m_pMainFrm->m_Search.m_bGood = m_btnCheckGood.GetCheck();
}

void CDlgSearch::OnCheckBad() 
{
	m_pMainFrm->m_Search.m_bBad = m_btnCheckBad.GetCheck();
}

void CDlgSearch::OnDeltaposSpinFromYy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panFromYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panFromYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<2007)  nNumber = 2007;
	else if(nNumber>2030)  nNumber = 2030;

    m_pMainFrm->m_Search.m_tmFromYY = nNumber;
	str.Format("%d", nNumber);
	m_panFromYY.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinFromMm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panFromMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panFromMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>12)  nNumber = 12;

    m_pMainFrm->m_Search.m_tmFromMM = nNumber;
	str.Format("%d", nNumber);
	m_panFromMM.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinFromDd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panFromDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panFromDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>31)  nNumber = 31;

    m_pMainFrm->m_Search.m_tmFromDD = nNumber;
	str.Format("%d", nNumber);
	m_panFromDD.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinFromTt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panFromTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panFromTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>24)  nNumber = 24;

    m_pMainFrm->m_Search.m_tmFromTT = nNumber;
	str.Format("%d", nNumber);
	m_panFromTT.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinToYy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panToYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panToYY.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if(nNumber<2007)  nNumber = 2007;
	else if(nNumber>2030)  nNumber = 2030;

    m_pMainFrm->m_Search.m_tmToYY = nNumber;
	str.Format("%d", nNumber);
	m_panToYY.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinToMm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panToMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panToMM.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>12)  nNumber = 12;

    m_pMainFrm->m_Search.m_tmToMM = nNumber;
	str.Format("%d", nNumber);
	m_panToMM.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinToDd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panToDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panToDD.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>31)  nNumber = 31;

    m_pMainFrm->m_Search.m_tmToDD = nNumber;
	str.Format("%d", nNumber);
	m_panToDD.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinToTt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panToTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panToTT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<1)  nNumber = 1;
	else if(nNumber>24)  nNumber = 24;

    m_pMainFrm->m_Search.m_tmToTT = nNumber;
	str.Format("%d", nNumber);
	m_panToTT.SetCaption(str);

	*pResult = 0;
}


void CDlgSearch::OnDeltaposSpinFromMt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panFromMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panFromMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>60)  nNumber = 60;

    m_pMainFrm->m_Search.m_tmFromMT = nNumber;
	str.Format("%d", nNumber);
	m_panFromMT.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnDeltaposSpinToMt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;
	int nNumber;

	if (pNMUpDown->iDelta < 0)
	{
		nNumber	= atoi(m_panToMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else if(pNMUpDown->iDelta > 0)
	{
		nNumber	= atoi(m_panToMT.GetCaption());
		nNumber-= pNMUpDown->iDelta;
	}
	else 
		return;

	if (nNumber<0)  nNumber = 0;
	else if(nNumber>60)  nNumber = 60;

    m_pMainFrm->m_Search.m_tmToMT = nNumber;
	str.Format("%d", nNumber);
	m_panToMT.SetCaption(str);

	*pResult = 0;
}

void CDlgSearch::OnBtnExcelData() 
{

}
