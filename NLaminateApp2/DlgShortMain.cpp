// DlgShortMain.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgShortMain.h"
#include "NPioControlDll.h"
#include "MainFrm.h"
#include "math.h"
#include "NPlotDll.h"
#include "NGuiDll.h"
#include "afxdialogex.h"

// CDlgShortMain dialog
void CALLBACK  NPlotDll_TestShortEvent();
extern CMainFrame    *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgShortMain, CDialogEx)

CDlgShortMain::CDlgShortMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShortMain::IDD, pParent)
{
	m_nListIndexPoint = 0;
	nTotalOkCou = 0;
	nTotalNgCou = 0;
	nChargeCou = 0;
	nContactCou = 0;
	nHighCou = 0;
	nLowCou = 0;
	nPoCou = 0;
	nEtcCou = 0;
	nTotalCou = 0;
	nOk_Ratio = 0;

	nTotalNgCou1 = 0;
	nChargeCou1 = 0;
	nContactCou1 = 0;
	nHighCou1 = 0;
	nLowCou1 = 0;
	nPoCou1 = 0;
	nEtcCou1 = 0;
}

CDlgShortMain::~CDlgShortMain()
{
	SetEvent(m_hEventCOM);
	NPlotDll_Close(DEF_SHORT_PLOT_NUM);

	NUiListDll_Close(LIST_SHORT_ID);
}

void CDlgShortMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST_SHORT, m_listTestShort);
	DDX_Control(pDX, IDC_ISEVENSEGMEN_OK_COU, cTotalOkCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENT_NG_COU, cTotalNgCou);
	DDX_Control(pDX, IDC_ISEVEN_CHARGE_NG, cChargeCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX2, cContactCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX3, cHighCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX4, cLowCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX5, cPoCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX10, cEtcCou);
	DDX_Control(pDX, IDC_ISEVENSEGMEN_TOTAL_OK_COU, cTotalCou);

	DDX_Control(pDX, IDC_IPIECHARTX_SHORT_NG_POSITION, cShort_Ng_PChart_Com);
	DDX_Control(pDX, IDC_ILABELX12, cGoodRatio);
	DDX_Control(pDX, IDC_ISEVENSEGMENT_NG_COU3, cRearNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENT_NG_COU2, cFrontNgCou);
	DDX_Control(pDX, IDC_ISEVEN_CHARGE_NG2, cChargeNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX6, cContactNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX7, cHighNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX8, cLowNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX9, cPdNgCou);
	DDX_Control(pDX, IDC_ISEVENSEGMENTINTEGERX11, cEtcNgCou);
}

BEGIN_MESSAGE_MAP(CDlgShortMain, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgShortMain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgShortMain::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgShortMain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgShortMain::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgShortMain::OnBnClickedButton6)
END_MESSAGE_MAP()

BOOL CDlgShortMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	InitControls();
	InitListControls();
	InitIndexShort();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgShortMain message handlers
void CDlgShortMain::InitControls()
{
	WINDOWPLACEMENT wp;
	RECT rc;

	GetDlgItem(IDC_STATIC_SHORT_PLOT)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
	GetDlgItem(IDC_STATIC_SHORT_PLOT)->SetWindowPos(NULL, rc.left, rc.top, 1018, PLOT_MAX_HEIGHT-395, SWP_SHOWWINDOW);

	NPlotDll_Open(DEF_SHORT_PLOT_NUM, (long)GetDlgItem(IDC_STATIC_SHORT_PLOT));
	NPlotDll_LoadSetupData(DEF_SHORT_PLOT_NUM, DEF_SHORTPLOT_PATHNAME);
	NPlotDll_InitialData(DEF_SHORT_PLOT_NUM);
	NPlotDll_SetMaxDataNum(DEF_SHORT_PLOT_NUM, PLOT_MAX_DATA_NUM);
	NPlotDll_SetEnableHoriRefLine(DEF_SHORT_PLOT_NUM, TRUE);
	NPlotDll_SetRangeX(DEF_SHORT_PLOT_NUM, 0, PLOT_MAX_DATA_NUM);
	NPlotDll_SetRangeLY(DEF_SHORT_PLOT_NUM, 0, 1);
	NPlotDll_SetPosHoriRefLine(DEF_SHORT_PLOT_NUM, TRUE, m_pMainFrm->m_Setting.m_fContactCurr, m_pMainFrm->m_Setting.m_fLimitCurr);
	NPlotDll_SetEvent(DEF_SHORT_PLOT_NUM, NPlotDll_TestShortEvent,  (long)&NPlotDll_Data);

	NUiListDll_Open(LIST_SHORT_ID, (long)GetDlgItem(IDC_STATIC_LIST_SHORT));

    //{BEGIN-COM : Serial 통신 초기화 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// 만약에 Serial 통신을 사용하지 않으려면  아래부분을 막고 Serial 통신버튼의
	// Property에서 Visible을 UnCheck 하면된다.
	/*
	m_ctrlCOM[0].SetSettings(_T("19200, n, 8, 1")); // 먼저 데이타를 설정하고
	m_ctrlCOM[0].SetCommPort((short)m_pMainFrm->m_Setting.m_nCOM1);
	m_ctrlCOM[0].SetInputMode(1);                //Binary
	m_ctrlCOM[0].SetPortOpen(TRUE);              // COM 포트를 연다.
    m_pMainFrm->m_bStatusCOM[0] = TRUE;
	
	m_ctrlCOM[1].SetSettings(_T("19200, n, 8, 1")); // 먼저 데이타를 설정하고
	m_ctrlCOM[1].SetCommPort((short)m_pMainFrm->m_Setting.m_nCOM2);
	m_ctrlCOM[1].SetInputMode(1);                //Binary
	m_ctrlCOM[1].SetPortOpen(TRUE);              // COM 포트를 연다.
	m_pMainFrm->m_bStatusCOM[1] = TRUE;
*/
    //}END-COM : Serial 통신 초기화 끝 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

/*
void CDlgShortMain::InitListControls()
{
	long nLngSel;
	LVCOLUMN vCol;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	///////////////////////////////////////////////////////
	vCol.mask=LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;
	vCol.fmt= LVCFMT_CENTER;
	
	m_listTestShort.DeleteAllItems();

	vCol.cx=70;
	if (nLngSel==0) vCol.pszText = _T(" 冉荐 ");
	else if (nLngSel==1) vCol.pszText = _T(" NUM. ");
	else vCol.pszText = _T(" 猞 ");
	m_listTestShort.InsertColumn(0, &vCol);

	vCol.cx=80;
	if (nLngSel==0) vCol.pszText = _T(" 菊 ");
	else if (nLngSel==1) vCol.pszText = _T(" FRONT ");
	else vCol.pszText = _T(" 铖 ");
	m_listTestShort.InsertColumn(1, &vCol);

	vCol.cx=80;
	if (nLngSel==0) vCol.pszText = _T(" 第 ");
	else if (nLngSel==1) vCol.pszText = _T(" REAR ");
	else vCol.pszText = _T("  ");	
	m_listTestShort.InsertColumn(2, &vCol);

	m_listTestShort.SetExtendedStyle(LVS_EX_GRIDLINES);
	UpdateData(false);
}
*/

void CDlgShortMain::InitListControls()
{
	long nLngSel;
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	NUiListDll_SetRows(LIST_SHORT_ID, 40);
	NUiListDll_SetRow(LIST_SHORT_ID, 40);
	NUiListDll_SetCols(LIST_SHORT_ID, 3);
	NUiListDll_SetRowHeight(LIST_SHORT_ID, 22);
	NUiListDll_SetTitleColor(LIST_SHORT_ID, RGB(180, 180, 180));
	NUiListDll_SetBackColor(LIST_SHORT_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_SHORT_ID, RGB(200, 200, 200));
	NUiListDll_SetFontSize(LIST_SHORT_ID, 16);
	NUiListDll_SetEnableEdit(LIST_SHORT_ID, FALSE);

	NUiListDll_SetColWidth(LIST_SHORT_ID, 0, 82);
	NUiListDll_SetColWidth(LIST_SHORT_ID, 1, 80);
	NUiListDll_SetColWidth(LIST_SHORT_ID, 2, 80);

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" 횟수 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" NUM. "));
	else NUiListDll_SetText(LIST_SHORT_ID, 0, 0, _T(" 数量 "));

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" 전 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" FRONT "));
	else NUiListDll_SetText(LIST_SHORT_ID, 1, 0, _T(" 前面 "));

	if (nLngSel==0) NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" 후 "));
	else if (nLngSel==1) NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" REAR "));
	else NUiListDll_SetText(LIST_SHORT_ID, 2, 0, _T(" 后面 "));	
	
    NUiListDll_Invalidate(LIST_SHORT_ID);

	m_nListIndexPoint = 0;

	nTotalOkCou = 0;
	nTotalNgCou = 0;
	nChargeCou = 0;
	nContactCou = 0;
	nHighCou = 0;
	nLowCou = 0;
	nPoCou = 0;
	nEtcCou = 0;
	nTotalCou = 0;
	nOk_Ratio = 0;

	cTotalCou.put_Value(nTotalCou);
	cTotalOkCou.put_Value(nTotalOkCou);  	
	cTotalNgCou.put_Value(nTotalNgCou);
	cGoodRatio.put_Caption(_T("0"));

	cChargeCou.put_Value(nChargeCou); 
	cContactCou.put_Value(nContactCou); 
	cHighCou.put_Value(nHighCou); 
	cLowCou.put_Value(nLowCou); 	
	cPoCou.put_Value(nPoCou);
	cEtcCou.put_Value(nEtcCou);

	cShort_Ng_PChart_Com.ClearList();
	cShort_Ng_PChart_Com.put_TitleText(_T(""));
	
	cShort_Ng_PChart_Com.AddItem(_T("Charge NG"),     CHARGE_COLOR, 1);
	cShort_Ng_PChart_Com.AddItem(_T("Contact NG"),     CONTACT_COLOR, 1);
	cShort_Ng_PChart_Com.AddItem(_T("High NG"),     HIGH_COLOR, 1);
	cShort_Ng_PChart_Com.AddItem(_T("Low NG"),     LOW_COLOR, 1);
	cShort_Ng_PChart_Com.AddItem(_T("PD NG"),			PO_COLOR, 1);

	cShort_Ng_PChart_Com.SetItemValue(CHARGE_FAIL-1, 0);
	cShort_Ng_PChart_Com.SetItemValue(CONTACT_FAIL-1, 0);
	cShort_Ng_PChart_Com.SetItemValue(HIGH_OVERLOAD-1, 0);
	cShort_Ng_PChart_Com.SetItemValue(LOW_OVERLOAD-1, 0);
	cShort_Ng_PChart_Com.SetItemValue(PD_FAIL-1, 0);	
}

void CDlgShortMain::UpdateListControls(long bDecF, long nInxF, float fF, long bDecR, long nInxR, float fR)
{
	long i, nRow, nCnt;
	CString strList[3];
	SYSTEMTIME	lpSystemTime;
	CString str;

	m_pMainFrm->m_Setting.m_nTotalShortCount++;

	GetLocalTime(&lpSystemTime);

	m_IndexData[m_nListIndexPoint] = m_pMainFrm->m_Setting.m_nTotalShortCount;
	m_FrontData[m_nListIndexPoint] = fF;
	m_RearData[m_nListIndexPoint] = fR;
	m_FrontDec[m_nListIndexPoint] = bDecF;
	m_RearDec[m_nListIndexPoint] = bDecR;

	nRow = 1;
	nCnt = 0;
	i=m_nListIndexPoint;
	if (i>=DEF_SHORTLIST_MAX_NUM)
		i = 0;

	while(nCnt<DEF_SHORTLIST_MAX_NUM)
	{
	    if (m_IndexData[i]>=0) 
		{
			str.Format(_T("%6d"), m_IndexData[i]);
			NUiListDll_SetText(LIST_SHORT_ID, 0, nRow, str);

	        if (m_pMainFrm->m_Setting.m_nCOM1)
			{
				str.Format(_T("%5.4f"), m_FrontData[i]);
				NUiListDll_SetText(LIST_SHORT_ID, 1, nRow, str);
				if(m_FrontDec[i] == SHORT_PASS)
					NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, RGB(200, 255, 200)); 
				else 
				{
					if (m_FrontDec[i] == CHARGE_FAIL)
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, CHARGE_COLOR); 				
					else if (m_FrontDec[i] == CONTACT_FAIL) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, CONTACT_COLOR); 
					else if (m_FrontDec[i] == HIGH_OVERLOAD) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, HIGH_COLOR); 	
					else if (m_FrontDec[i] == LOW_OVERLOAD) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, LOW_COLOR); 
					else if (m_FrontDec[i] == PD_FAIL) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, PO_COLOR); 
					else 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 1, nRow, ETC_COLOR); //ETC ERROR
				}
			}

            if (m_pMainFrm->m_Setting.m_nCOM2)
			{
				str.Format(_T("%5.4f"), m_RearData[i]);
				NUiListDll_SetText(LIST_SHORT_ID, 2, nRow, str);
				if (m_RearDec[i] == SHORT_PASS)
					NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, RGB(200, 255, 200));
				else 
				{
					if (m_RearDec[i] == CHARGE_FAIL)
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, CHARGE_COLOR);
					else if (m_RearDec[i] == CONTACT_FAIL) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, CONTACT_COLOR); 
					else if (m_RearDec[i] == HIGH_OVERLOAD) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, HIGH_COLOR); 
					else if (m_RearDec[i] == LOW_OVERLOAD) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, LOW_COLOR);
					else if (m_RearDec[i] == PD_FAIL) 
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, PO_COLOR); 
					else
						NUiListDll_SetCellBackColor(LIST_SHORT_ID, 2, nRow, ETC_COLOR);
				}
			}

			nRow++;
		}

		i++;
		if (i>=DEF_SHORTLIST_MAX_NUM)
			i=0;

		nCnt++;
	}

	m_nListIndexPoint++;
	if (m_nListIndexPoint>=DEF_SHORTLIST_MAX_NUM)
		m_nListIndexPoint = 0;

	nRow++;
	NUiListDll_SetRow(LIST_SHORT_ID, nRow);
	NUiListDll_Invalidate(LIST_SHORT_ID);
}

void CDlgShortMain::InitIndexShort()
{
	long i;

	m_nFirstIndex[0] = 0;
	m_nFirstIndex[1] = 0;
	m_nRearIndex[0] = 0;
	m_nRearIndex[1] = 0;

    m_strRemainT[0] = _T(""); 
	m_strRemainT[1] = _T(""); 

	for(i=0; i<SHORT_BUFFER_MAX_NUM; i++)
	{
		m_nGoodDec[0][i] = 0;
		m_nGoodDec[1][i] = 0;

		m_nVoltV[0][i] = 0;
		m_nVoltV[1][i] = 0;

		m_nCurrV[0][i] = 0;
		m_nCurrV[1][i] = 0;

		m_nNumID[0][i] = -1;
		m_nNumID[1][i] = -1;
	}

	m_nListIndexPoint = 0;
	for(i=0; i<DEF_SHORTLIST_MAX_NUM; i++)
	{
		m_IndexData[i] = -1;
		m_FrontData[i] = -1;
		m_RearData[i] = -1;
		m_FrontDec[i] = TRUE;
    	m_RearDec[i] = TRUE;
	}
}

void CDlgShortMain::InitShortProc()
{
	long i;

	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화 
	NComPort_Close(m_pMainFrm->m_Setting.m_nCOM1);
	NComPort_Close(m_pMainFrm->m_Setting.m_nCOM2);
	Sleep(100);

	NComPort_Open(m_pMainFrm->m_Setting.m_nCOM1, 19200, 8, 0, 1);
	NComPort_Open(m_pMainFrm->m_Setting.m_nCOM2, 19200, 8, 0, 1);
	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화 

	InitIndexShort();
	InitListControls();

	m_bOnRS232C = FALSE;	

	for(i=0; i<5; i++)
	{
		NComPort_BufferClear(m_pMainFrm->m_Setting.m_nCOM1);
		NComPort_BufferClear(m_pMainFrm->m_Setting.m_nCOM2);
	}

	m_hEventCOM = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadCOM = AfxBeginThread(ThreadShortCOM, this, THREAD_PRIORITY_NORMAL);
	if (m_pThreadCOM)
		m_pThreadCOM->m_bAutoDelete = TRUE;
}

void CDlgShortMain::CloseShortProc()
{

}

void CDlgShortMain::SetEventCOM()
{
	SetEvent(m_hEventCOM);
	//m_pMainFrm->WriteLogToFile(" Input Short Read "); 
}

UINT CDlgShortMain::ThreadShortCOM(LPVOID pParam)
{
	CDlgShortMain *pView = (CDlgShortMain *)pParam;
   	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	static bool bFlag = FALSE;

	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventCOM, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

#if _RS232C_LVS_TESTER
		if (bFlag)
		{ 
		    pView->GetRS232CData();
		}
#endif

#if _RS232C_CHROMA_TESTER || _RS232C_CHROMA_TESTER2
	    pView->GetRS232CData();
#endif
		
		bFlag = !bFlag;
        ResetEvent(pView->m_hEventCOM);
	}

	CloseHandle(pView->m_hEventCOM);
	AfxEndThread(100);

	return 0;
}

#if _RS232C_CHROMA_TESTER2

void CDlgShortMain::GetRS232CData()
{
	BOOL bRetF, bRetR;
	long nRearIndex0, nRearIndex1;
	float fV0, fI0, fV1, fI1;
	float fOhm0, fOhm1;
	char szbufF[500], szbufR[500];
	CString strT, str, strF, strR, strName;
	
	fOhm0 = 0;
	fOhm1 = 0;
	fV0 = 0;
	fV1 = 0;
	fI0 = 0;
	fI1 = 0;

	bRetF = 0;
	bRetR = 0;
	nRearIndex0 = 0;
	nRearIndex1 = 0;

	strcpy_s(szbufF, "");
	strcpy_s(szbufR, "");

    if (m_pMainFrm->m_Setting.m_nCOM1)
	{
		bRetF = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM1, szbufF);
		if (bRetF)
		{
			strF = CharToString(szbufF);
			//strF = strF.Left(bRetF);
			GetStringFromText(0, strF);

			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(0, TRUE);
		}
		else
			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(0, FALSE);
	}

    if (m_pMainFrm->m_Setting.m_nCOM2)
	{
		bRetR = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM2, szbufR);
		if (bRetR)
		{
			strR = CharToString(szbufR);
			//strR = strR.Left(bRetR);
			GetStringFromText(1, strR);

			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(1, TRUE);
		}
		else
			m_pMainFrm->m_pView->m_pDlgTestShort->SetCommStatus(1, FALSE);
	}	

	if (m_pMainFrm->m_Setting.m_nCOM1)
	{
		fV0 = (float)m_nVoltV[0][0];
		fI0 = (float)m_nCurrV[0][0]/10000.0f;

		if (fabs(fI0)<0.00001f) 
		{
			fI0 = 0.00001f;
			fOhm0 = 0.0f; 
		}
		else fOhm0 = fV0/fI0;

		WriteShortVToFile(0, m_nGoodDec[0][0], fI0);		

		if (m_nGoodDec[0][0] == SHORT_PASS)	   {	nTotalOkCou++;  	}	
		else { 
			if (m_nGoodDec[0][0] == CHARGE_FAIL)   {	nChargeCou++; nTotalNgCou++;  	}			
			else if (m_nGoodDec[0][0] == CONTACT_FAIL)  {	nContactCou++; nTotalNgCou++; 	}	 
			else if (m_nGoodDec[0][0] == HIGH_OVERLOAD) {	nHighCou++; nTotalNgCou++; 	}		
			else if (m_nGoodDec[0][0] == LOW_OVERLOAD)  {	nLowCou++; nTotalNgCou++;	}	
			else if (m_nGoodDec[0][0] == PD_FAIL)       {	nPoCou++;  nTotalNgCou++;  	}	
			else if (m_nGoodDec[0][0] == ETC_ERROR)     {	nEtcCou++; 	nTotalNgCou++;					}	
			else {	nEtcCou++; nTotalNgCou++;  	}
		}
	}

	if (m_pMainFrm->m_Setting.m_nCOM2)
	{
		fV1 = (float)m_nVoltV[1][0];
		fI1 = (float)m_nCurrV[1][0]/10000.0f;

		if (fabs(fI1)<0.00001f)
		{
			fI1 = 0.00001f;
			fOhm1 = 0.0f;
		}
		else fOhm1 = fV1/fI1;

		WriteShortVToFile(1, m_nGoodDec[1][0], fI1);	

		if (m_nGoodDec[1][0] == SHORT_PASS)	   {	nTotalOkCou++;  	}	
		else {
			if (m_nGoodDec[1][0] == CHARGE_FAIL)		{	nChargeCou1++; nTotalNgCou1++;  	}			
			else if (m_nGoodDec[1][0] == CONTACT_FAIL)  {	nContactCou1++; nTotalNgCou1++; 	}	 
			else if (m_nGoodDec[1][0] == HIGH_OVERLOAD) {	nHighCou1++; nTotalNgCou1++; 	}		
			else if (m_nGoodDec[1][0] == LOW_OVERLOAD)  {	nLowCou1++; nTotalNgCou1++;	}	
			else if (m_nGoodDec[1][0] == PD_FAIL)       {	nPoCou1++; nTotalNgCou1++;  	}	
			else if (m_nGoodDec[1][0] == ETC_ERROR)     {	nEtcCou1++; nTotalNgCou1++;  	}	
			else {	nEtcCou1++; nTotalNgCou1++;  	}	
		}
	}

	cChargeCou.put_Value(nChargeCou);
	cContactCou.put_Value(nContactCou);
	cHighCou.put_Value(nHighCou);
	cLowCou.put_Value(nLowCou);
	cPoCou.put_Value(nPoCou);
	cEtcCou.put_Value(nEtcCou);

	cChargeNgCou.put_Value(nChargeCou1);
	cContactNgCou.put_Value(nContactCou1);
	cHighNgCou.put_Value(nHighCou1);
	cLowNgCou.put_Value(nLowCou1);
	cPdNgCou.put_Value(nPoCou1);
	cEtcNgCou.put_Value(nEtcCou1);

	cFrontNgCou.put_Value(nTotalNgCou);
	cRearNgCou.put_Value(nTotalNgCou1);

	cTotalNgCou.put_Value(nTotalNgCou+nTotalNgCou1);

	cTotalOkCou.put_Value(nTotalOkCou);
	nTotalCou = nTotalOkCou + nTotalNgCou + nTotalNgCou1;
	cTotalCou.put_Value(nTotalCou);

	CString strtmp = _T("");
	strtmp.Format(_T("%0.2f"), (float)((float)nTotalOkCou/(float)nTotalCou) * 100.0);

	cShort_Ng_PChart_Com.SetItemValue(CHARGE_FAIL-1, nChargeCou+nChargeCou1);
	cShort_Ng_PChart_Com.SetItemValue(CONTACT_FAIL-1, nContactCou+nContactCou1);
	cShort_Ng_PChart_Com.SetItemValue(HIGH_OVERLOAD-1, nHighCou+nHighCou1);
	cShort_Ng_PChart_Com.SetItemValue(LOW_OVERLOAD-1, nLowCou+nLowCou1);
	cShort_Ng_PChart_Com.SetItemValue(PD_FAIL-1, nPoCou+nPoCou1);

	cGoodRatio.put_Caption(strtmp);
		
	m_pMainFrm->m_fFrontShort = fI1;
	m_pMainFrm->m_fRearShort  = fI0;
//
//	m_pMainFrm->m_nFrontDec = m_nGoodDec[0][0];
//	m_pMainFrm->m_nRearDec  = m_nGoodDec[1][0];

	UpdateListControls(m_nGoodDec[0][nRearIndex0], m_nNumID[0][0], fI0, m_nGoodDec[1][0], m_nNumID[1][0], fI1);

	m_nVoltV[0][nRearIndex0] = 0;
	m_nCurrV[0][nRearIndex0] = 0;
	m_nNumID[0][nRearIndex0] = -1;

	m_nVoltV[1][nRearIndex1] = 0;
	m_nCurrV[1][nRearIndex1] = 0;
	m_nNumID[1][nRearIndex1] = -1;
		
	if (m_pMainFrm->m_Setting.m_nCOM1)
    	NPlotDll_AppendStickData(DEF_SHORT_PLOT_NUM, 0, 0, fI0);
    if (m_pMainFrm->m_Setting.m_nCOM2)
		NPlotDll_AppendStickData(DEF_SHORT_PLOT_NUM, 1, 0, fI1);

	NPlotDll_Invalidate(DEF_SHORT_PLOT_NUM);

	nRearIndex0++;
	if (nRearIndex0>=SHORT_BUFFER_MAX_NUM)
		nRearIndex0 = 0;
	m_nRearIndex[0] = nRearIndex0;


	str.Format(_T(" F : %s , R : %s "), strF, strR);
	m_pMainFrm->WriteTextShortMessage(str);

	/*
	char szbuf[256];
	StringToChar(str, szbuf);
	m_pMainFrm->WriteLogToFile(szbuf);	
	*/

	//m_bOnRS232C = FALSE;
	//m_bOnRS232C = !m_bOnRS232C;

//	return 1;
}

//8.19752e+01,-1.00000e+00,-1.00000e+00,72
//1.99024e+02,0.00000e+00,1.60749e-02,128
#endif

void CDlgShortMain::GetStringFromText(long nPort, CString strText)
{
	long i, nPos, nTemp;
	BOOL bFlag;
	CString strT;

	nPos = 0;
	bFlag = FALSE;
	int cou = 0;
	//////////////////////////////// 로그 저장 ////////////////////////
	char szbuf[256];
	StringToChar(strText, szbuf);
	m_pMainFrm->WriteLogToFile(szbuf);
	///////////////////////////////////////////////////////////////////

	strT = strText.Mid(24, 11);

	nTemp = (long)(_ttof(strT) * 10000);
	m_nCurrV[nPort][0] = nTemp;

	strText.Remove(_T(' '));
	//strT = strText.Right(3); 

	for(i = 0; i < strText.GetLength(); i++) 
	{
		if(strText.GetAt(i) == ',') 
			cou++;
		if((strText.GetAt(i) == ',') && cou == 3)
			break; 
	}	

	strT = strText.Mid(i+1, 3);

	if (strT.Find(_T("12")) != -1) 
		m_nGoodDec[nPort][0] = SHORT_PASS;
	else 
	{
		if (strT.Find(_T("72")) != -1) 
			m_nGoodDec[nPort][0] = CHARGE_FAIL;		// 72(48)  Charge/Discharge Fail
		else if(strT.Find(_T("96")) != -1)      
			m_nGoodDec[nPort][0] = HIGH_OVERLOAD;	// 96(60)  High/Overload
		else  if(strT.Find(_T("80")) != -1)      
			m_nGoodDec[nPort][0] = LOW_OVERLOAD;	// 80	  Low
		else  if(strT.Find(_T("68")) != -1)       
			m_nGoodDec[nPort][0] = CONTACT_FAIL;	// 68(44)  Contack Fail
		else  if(strT.Find(_T("66")) != -1)       
			m_nGoodDec[nPort][0] = PD_FAIL;			// 66(42)  PD Fail
		else     
		{
			m_nGoodDec[nPort][0] = ETC_ERROR;		// unknown 				
		}
	}

}


long CDlgShortMain::GetIndexRearSide(long nIndexF)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<SHORT_BUFFER_MAX_NUM; i++)
	{
		if (m_nNumID[1][i] == nIndexF) 
		{
			nPos = i;
			break;
		}
	}

	return nPos;
}

void CDlgShortMain::WriteShortVToFile(BOOL bRear, long bDec, float fV)
{
	FILE  *fp;
	TCHAR   szbuf[1200];
	CString strPathName, strDataPath, str, strT;
	SYSTEMTIME	lpSystemTime;

	GetLocalTime(&lpSystemTime);

	strDataPath = SAVE_SHORTDATA_PATH;

	str.Format(_T("%04d\\%02d\\%02d\\"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	strDataPath += str;

	_stprintf_s(szbuf, _T("%s%02d.txt"), strDataPath, lpSystemTime.wHour);
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
	                  m_pMainFrm->m_strLotID, lpSystemTime.wYear, 
					  lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);

	bRear = FALSE;
	str.Format(_T("%1d %1d"), bRear, bDec);
	strT += str;

	str.Format(_T("%20s %1d %1d %6.4f "), _T("S.T"), 0, bDec, fV);
	strT += str;

	if (m_pMainFrm->m_Setting.m_nOldShortHour != lpSystemTime.wHour)
	{
		CreateDirectoryFullPath(strDataPath);
		_tfopen_s(&fp, szbuf, _T("w+"));

		m_pMainFrm->m_Setting.m_nOldShortHour = lpSystemTime.wHour;
		m_pMainFrm->SaveSysValueToFile();
	}
	else
	{
		_tfopen_s(&fp, szbuf, _T("a+"));
		if (!fp)
		{
			CreateDirectoryFullPath(strDataPath);
		    _tfopen_s(&fp, szbuf, _T("w+"));
		}
	}

	if (fp)
	{
		_ftprintf_s(fp, _T("%s\n"), strT);
    	fclose(fp);
	}

	m_pMainFrm->m_Setting.m_nOldShortHour = lpSystemTime.wHour;
}

void CDlgShortMain::SetShortTestCondition()
{
	char szbuf[100];

	if (g_PortCOM[0])
	{
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'S');

		sprintf_s(szbuf, "%04d", (long)m_pMainFrm->m_Setting.m_fTestVolt);							//전압설정 200
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'A', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fLimitCurr*1000+0.5));//*1000		//제한전류 5
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'B', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fStartDelayTime*100+0.5));//*100	//0 
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'C', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fRampingTime*100+0.5));//*100		//Ramping, 3
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'D', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fHoldingTime*100+0.5));//*100		//Holding 2
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'E', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fContactCurr*1000+0.5));//*1000		//접촉불량 0
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM1, 'F', szbuf);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM1, 'R');
	}

	if (g_PortCOM[1])
	{
		///////PLC에서 한번에 2개를 동시에 콘트롤 하면 아래 수정
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'S');

		sprintf_s(szbuf, "%04d", (long)m_pMainFrm->m_Setting.m_fTestVolt);					//테스트 볼트(2제거)
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'A', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fLimitCurr*1000+0.5));		//리미트커런트(2제거) *1000
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'B', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fStartDelayTime*100+0.5));	//딜레이타임(2제거) *100
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'C', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fRampingTime*100+0.5));		
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'D', szbuf);

		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fHoldingTime*100+0.5));		
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'E', szbuf);
	
		sprintf_s(szbuf, "%04d", (long)(m_pMainFrm->m_Setting.m_fContactCurr*1000+0.5));		//콘텍트(2제거) *1000
		NComPort_Write(m_pMainFrm->m_Setting.m_nCOM2, 'F', szbuf);
		NComPort_Mode(m_pMainFrm->m_Setting.m_nCOM2, 'R');
	}
}

void CALLBACK NPlotDll_TestShortEvent()
{

}

void CDlgShortMain::OnTimer(UINT_PTR nIDEvent)
{
	//GetRS232CData();
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgShortMain::OnBnClickedButton3()
{

}

void CDlgShortMain::OnBnClickedButton4()
{

}



BOOL CDlgShortMain::GetTesterVersion(long nComPort)
{
	long i, nPort, nLen;
	CString str, strT, strReceive;
	BYTE szbuf[1024], szReceive[1024];
	BOOL bRet = TRUE;

	nPort = nComPort-1;
	if (nPort<0) 
		nPort=0;
	
	if (!m_pMainFrm->m_bStatusCOM[nPort])
		return bRet;

//	str.Format(_T("%x%x%x%x%x%x%x%x"), 0xAB, 0x01, 0x70, 0x01, 0x90, 0xFE);
	szbuf[0] = unsigned char(0xAB);
	szbuf[1] = unsigned char(0x01);
	szbuf[2] = unsigned char(0x70);
	szbuf[3] = unsigned char(0x01);
	szbuf[4] = unsigned char(0x90);
	szbuf[5] = unsigned char(0xFE);
	szbuf[6] = '\0';

	m_ctrlCOM[nPort].SendString(szbuf, 6);
	Sleep(5);

	strReceive = _T("");
	nLen = m_ctrlCOM[nPort].ReceiveString(szReceive);
	for(i=0; i<nLen; i++)
	{
		strT.Format(_T("%02X"), szReceive[i]);
		strReceive += strT;
	}

	if (strReceive.Find(_T("4348524F"))>0) //43C 48H 52R 4FO
	    bRet = FALSE;

	return bRet;
}

CString CDlgShortMain::ReadComPort(long nComPort, long *nLen)
{
	long i, nPort;
	CString str, strT, strReceive;
	BYTE szbuf[1024], szReceive[1024];

	strReceive = _T("");
	*nLen = 0;
	nPort = nComPort-1;
	if (nPort<0) 
		nPort=0;
	
	if (!m_pMainFrm->m_bStatusCOM[nPort])
		return 0;

//	str.Format(_T("%x%x%x%x%x%x%x%x"), 0xAB, 0x01, 0x70, 0x03, 0xB1, 0x00, 0xD7, 0x04);
	str.Format(_T("%c%c%c%c%c%c%c%c"), unsigned char(0xAB), unsigned char(0x01), unsigned char(0x70), unsigned char(0x03), 
		                               unsigned char(0xB1), unsigned char(0x00), unsigned char(0xD7), unsigned char(0x04));
	szbuf[0] = unsigned char(0xAB);
	szbuf[1] = unsigned char(0x01);
	szbuf[2] = unsigned char(0x70);
	szbuf[3] = unsigned char(0x03);
	szbuf[4] = unsigned char(0xB1);
	szbuf[5] = unsigned char(0x00);
	szbuf[6] = unsigned char(0xD7);
	szbuf[7] = unsigned char(0x04);
	szbuf[8] = '\0';

	m_ctrlCOM[nPort].SendString(szbuf, 8);
	Sleep(10);
	
	*nLen = m_ctrlCOM[nPort].ReceiveString(szReceive);
	for(i=0; i<*nLen; i++)
	{
		strT.Format(_T("%02X"), szReceive[i]);
		strReceive += strT;
	}

	return strReceive;
}


void CDlgShortMain::OnBnClickedButton2()
{
	BOOL bRetF;
	CString strF, strText;
	char szbufF[500];

	strF = _T("");
	m_pMainFrm->WriteTextShortMessage(strF);
	Sleep(200);

	bRetF = NComPort_Read(m_pMainFrm->m_Setting.m_nCOM1, szbufF);
	//strF = ReadComPort(m_pMainFrm->m_Setting.m_nCOM1, &nLen);

	strF = CharToString(szbufF);
	strF = strF.Left(bRetF);

	GetStringFromText(0, strF);

	m_pMainFrm->WriteTextShortMessage(_T("F : ") + strF);
}

void CDlgShortMain::OnBnClickedButton5()
{

}


void CDlgShortMain::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	nTotalOkCou = 0;
	nTotalNgCou = 0;
	nChargeCou = 0;
	nContactCou = 0;
	nHighCou = 0;
	nLowCou = 0;
	nPoCou = 0;
	nEtcCou = 0;
	nTotalCou = 0;
	nOk_Ratio = 0;

	nTotalNgCou1 = 0;
	nChargeCou1 = 0;
	nContactCou1 = 0;
	nHighCou1 = 0;
	nLowCou1 = 0;
	nPoCou1 = 0;
	nEtcCou1 = 0;

	cChargeCou.put_Value(nChargeCou);
	cContactCou.put_Value(nContactCou);
	cHighCou.put_Value(nHighCou);
	cLowCou.put_Value(nLowCou);
	cPoCou.put_Value(nPoCou);
	cEtcCou.put_Value(nEtcCou);

	cChargeNgCou.put_Value(nChargeCou1);
	cContactNgCou.put_Value(nContactCou1);
	cHighNgCou.put_Value(nHighCou1);
	cLowNgCou.put_Value(nLowCou1);
	cPdNgCou.put_Value(nPoCou1);
	cEtcNgCou.put_Value(nEtcCou1);

	cFrontNgCou.put_Value(nTotalNgCou);
	cRearNgCou.put_Value(nTotalNgCou1);

	cTotalNgCou.put_Value(nTotalNgCou+nTotalNgCou1);

	cTotalOkCou.put_Value(nTotalOkCou);
	nTotalCou = nTotalOkCou + nTotalNgCou + nTotalNgCou1;
	cTotalCou.put_Value(nTotalCou);

	CString strtmp = _T("0");	

	cShort_Ng_PChart_Com.SetItemValue(CHARGE_FAIL-1, nChargeCou);
	cShort_Ng_PChart_Com.SetItemValue(CONTACT_FAIL-1, nContactCou);
	cShort_Ng_PChart_Com.SetItemValue(HIGH_OVERLOAD-1, nHighCou);
	cShort_Ng_PChart_Com.SetItemValue(LOW_OVERLOAD-1, nLowCou);
	cShort_Ng_PChart_Com.SetItemValue(PD_FAIL-1, nPoCou);

	cGoodRatio.put_Caption(strtmp);
}
