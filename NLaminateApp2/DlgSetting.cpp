// DlgSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "MainFrm.h"
#include "DlgSetting.h"
#include "NAppDll.h"
#include "NGuiDll.h"
#include "NPioControlDll.h"
#include "afxdialogex.h"

extern  CMainFrame *m_pMainFrm;

// CDlgSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSetting, CDialogEx)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting::IDD, pParent)
{

}

CDlgSetting::~CDlgSetting()
{
	NUiListDll_Close(LIST_SETUP_ID);
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgSetting)
	DDX_Control(pDX, IDC_CHECK_EXEC_CAM1, m_btnExecCam1);
	DDX_Control(pDX, IDC_CHECK_EXEC_CAM2, m_btnExecCam2);
	DDX_Control(pDX, IDC_CHECK_OKSAVE_CAM1,m_btnOkSaveCam1);
	DDX_Control(pDX, IDC_CHECK_OKSAVE_CAM2, m_btnOkSaveCam2);
	DDX_Control(pDX, IDC_CHECK_NGSAVE_CAM1,m_btnNgSaveCam1);
	DDX_Control(pDX, IDC_CHECK_NGSAVE_CAM2, m_btnNgSaveCam2);
	DDX_Control(pDX, IDC_CHECK_CUT_OFFSET, m_btnCutOffset);
	DDX_Control(pDX, IDC_CHECK_TE_OFFSET, m_btnEPCOffset);
	DDX_Control(pDX, IDC_CHECK_T2B_OFFSET, m_btnT2BOffset);
	DDX_Control(pDX, IDC_CHECK_DATA_TRANS, m_btnDataTrans);
	DDX_Control(pDX, IDC_CHECK_ALIGN_UP, m_btnAlignUP);
	DDX_Control(pDX, IDC_CHECK_ALIGN_DN, m_btnAlignDN);
	DDX_Control(pDX, IDC_CHECK_SW_GRAB, m_btnSWGrab);	
	DDX_Control(pDX, IDC_CHECK_WAY_LEFT_TO_RIGHT, m_btnWayLefttoRight);
	DDX_Control(pDX, IDC_CHECK_KOREA, m_btnKorea);
	DDX_Control(pDX, IDC_CHECK_ENGLISH, m_btnEnglish);
	DDX_Control(pDX, IDC_CHECK_CHINESS, m_btnChiness);
	DDX_Control(pDX, IDC_EDIT_PERIOD, m_editPeriod);
	DDX_Control(pDX, IDC_EDIT_DRIVER, m_editDriver);
	DDX_Control(pDX, IDC_EDIT_SKIP_CNT, m_editSkipCnt);
	DDX_Control(pDX, IDC_EDIT_DELAY_T, m_editDelayTime);
	DDX_Control(pDX, IDC_EDIT_DAQ_UNIT, m_editDaqCnt);
	DDX_Control(pDX, IDC_EDIT_UP_SAVE_TYPE, m_editUpSaveType);
	DDX_Control(pDX, IDC_EDIT_DN_SAVE_TYPE, m_editDnSaveType);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);	
	DDX_Control(pDX, IDC_SSPANEL_SPEC, m_panSpec);
	//}}AFX_DATA_MAP

	long i;
	for(i=0; i<16; i++)
	{
		DDX_Control(pDX, IDC_BTN_IN0+i, m_btnIN[i]);
		DDX_Control(pDX, IDC_CHECK_OUT0+i, m_btnOUT[i]);
	}	
	DDX_Control(pDX, IDC_STATIC_SETUP, m_staticSetup);
}

BEGIN_MESSAGE_MAP(CDlgSetting, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_OUT0, &CDlgSetting::OnBnClickedCheckOut0)
	ON_BN_CLICKED(IDC_CHECK_OUT1, &CDlgSetting::OnBnClickedCheckOut1)
	ON_BN_CLICKED(IDC_CHECK_OUT2, &CDlgSetting::OnBnClickedCheckOut2)
	ON_BN_CLICKED(IDC_CHECK_OUT3, &CDlgSetting::OnBnClickedCheckOut3)
	ON_BN_CLICKED(IDC_CHECK_OUT4, &CDlgSetting::OnBnClickedCheckOut4)
	ON_BN_CLICKED(IDC_CHECK_OUT5, &CDlgSetting::OnBnClickedCheckOut5)
	ON_BN_CLICKED(IDC_CHECK_OUT6, &CDlgSetting::OnBnClickedCheckOut6)
	ON_BN_CLICKED(IDC_CHECK_OUT7, &CDlgSetting::OnBnClickedCheckOut7)
	ON_BN_CLICKED(IDC_CHECK_OUT8, &CDlgSetting::OnBnClickedCheckOut8)
	ON_BN_CLICKED(IDC_CHECK_OUT9, &CDlgSetting::OnBnClickedCheckOut9)
	ON_BN_CLICKED(IDC_CHECK_OUT10, &CDlgSetting::OnBnClickedCheckOut10)
	ON_BN_CLICKED(IDC_CHECK_OUT11, &CDlgSetting::OnBnClickedCheckOut11)
	ON_BN_CLICKED(IDC_CHECK_OUT12, &CDlgSetting::OnBnClickedCheckOut12)
	ON_BN_CLICKED(IDC_CHECK_OUT13, &CDlgSetting::OnBnClickedCheckOut13)
	ON_BN_CLICKED(IDC_CHECK_OUT14, &CDlgSetting::OnBnClickedCheckOut14)
	ON_BN_CLICKED(IDC_CHECK_OUT15, &CDlgSetting::OnBnClickedCheckOut15)
	ON_BN_CLICKED(IDC_BTN_EXEC_CAM1, &CDlgSetting::OnBnClickedBtnExecCam1)
	ON_BN_CLICKED(IDC_BTN_OKSAVE_CAM1, &CDlgSetting::OnBnClickedBtnOksaveCam1)
	ON_BN_CLICKED(IDC_BTN_NGSAVE_CAM1, &CDlgSetting::OnBnClickedBtnNgsaveCam1)
	ON_BN_CLICKED(IDC_BTN_EXEC_CAM2, &CDlgSetting::OnBnClickedBtnExecCam2)
	ON_BN_CLICKED(IDC_BTN_OKSAVE_CAM2, &CDlgSetting::OnBnClickedBtnOksaveCam2)
	ON_BN_CLICKED(IDC_BTN_NGSAVE_CAM2, &CDlgSetting::OnBnClickedBtnNgsaveCam2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DRIVE, &CDlgSetting::OnDeltaposSpinDrive)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PERIOD, &CDlgSetting::OnDeltaposSpinPeriod)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgSetting::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_CHECK_EXEC_CAM1, &CDlgSetting::OnBnClickedCheckExecCam1)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_KOREA, &CDlgSetting::OnBnClickedCheckKorea)
	ON_BN_CLICKED(IDC_CHECK_ENGLISH, &CDlgSetting::OnBnClickedCheckEnglish)
	ON_BN_CLICKED(IDC_CHECK_CHINESS, &CDlgSetting::OnBnClickedCheckChiness)
	ON_BN_CLICKED(IDC_CHECK_EXEC_CAM2, &CDlgSetting::OnBnClickedCheckExecCam2)
	ON_BN_CLICKED(IDC_CHECK_EXEC_OFFSET, &CDlgSetting::OnBnClickedCheckExecOffset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UP_SAVE_TYPE, &CDlgSetting::OnDeltaposSpinUpSaveType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DN_SAVE_TYPE, &CDlgSetting::OnDeltaposSpinDnSaveType)
	ON_BN_CLICKED(IDC_CHECK_ALIGN_UP, &CDlgSetting::OnBnClickedCheckAlignUp)
	ON_BN_CLICKED(IDC_CHECK_ALIGN_DN, &CDlgSetting::OnBnClickedCheckAlignDn)
	ON_BN_CLICKED(IDC_BTN_IN0, &CDlgSetting::OnBnClickedBtnIn0)
	ON_BN_CLICKED(IDC_CHECK_CUT_OFFSET, &CDlgSetting::OnBnClickedCheckCutOffset)
END_MESSAGE_MAP()

// CDlgSetting 메시지 처리기입니다.
BOOL CDlgSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	NUiListDll_Open(LIST_SETUP_ID, (long)GetDlgItem(IDC_STATIC_SPEC));
		
	InitButtonControls();
	InitSetupControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSetting::SetTransLanguage(long nValue)
{
	InitSetupControls();
	
	if (nValue==0) m_btnApply.SetWindowText(_T("저장"));
	else if (nValue==1) m_btnApply.SetWindowText(_T("SAVE"));
	else m_btnApply.SetWindowText(_T("貯藏"));

	m_panSpec.SetWindowText(_T("SPEC. SETUP"));
	/*
	if (nValue==0) m_panSpec.SetCaption(_T("스펙 설정"));
	else if (nValue==1) m_panSpec.SetWindowText(_T("SPEC. SETUP"));
	else m_panSpec.SetWindowText(_T("範圍 設定"));
	*/

	if (nValue==0) m_editUpSaveType.SetTextOut(_T("축소"));
	else if (nValue==1) m_editUpSaveType.SetTextOut(_T("Zoom Out"));
	else m_editUpSaveType.SetTextOut(_T("缩小"));

	if (m_pMainFrm->m_Setting.m_bSaveImageType[CAM_UP])
	{
		if (nValue==0) m_editUpSaveType.SetTextOut(_T("1:1 원본"));
		else if (nValue==1) m_editUpSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editUpSaveType.SetTextOut(_T("1:1 原本"));
	}

	if (nValue==0) m_editDnSaveType.SetTextOut(_T("축소"));
	else if (nValue==1) m_editDnSaveType.SetTextOut(_T("Zoom Out"));
	else m_editDnSaveType.SetTextOut(_T("缩小"));

	if (m_pMainFrm->m_Setting.m_bSaveImageType[CAM_DN])
	{
		if (nValue==0) m_editDnSaveType.SetTextOut(_T("1:1 원본"));
		else if (nValue==1) m_editDnSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editDnSaveType.SetTextOut(_T("1:1 原本"));
	}
}

void CDlgSetting::InitButtonControls()
{
	long i, nLngSel;
	CString str;

	m_btnExecCam1.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnExecCam1.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnExecCam1.DrawBorder(FALSE);
	m_btnExecCam1.DrawTransparent();

	m_btnExecCam2.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnExecCam2.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnExecCam2.DrawBorder(FALSE);
	m_btnExecCam2.DrawTransparent();

	m_btnDataTrans.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnDataTrans.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnDataTrans.DrawBorder(FALSE);
	m_btnDataTrans.DrawTransparent();	

	m_btnSWGrab.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnSWGrab.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnSWGrab.DrawBorder(FALSE);
	m_btnSWGrab.DrawTransparent();  

	m_btnCutOffset.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnCutOffset.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnCutOffset.DrawBorder(FALSE);
	m_btnCutOffset.DrawTransparent();	

	m_btnEPCOffset.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnEPCOffset.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnEPCOffset.DrawBorder(FALSE);
	m_btnEPCOffset.DrawTransparent();	

	m_btnT2BOffset.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnT2BOffset.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnT2BOffset.DrawBorder(FALSE);
	m_btnT2BOffset.DrawTransparent();	

	m_btnAlignUP.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnAlignUP.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnAlignUP.DrawBorder(FALSE);
	m_btnAlignUP.DrawTransparent();

	m_btnAlignDN.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnAlignDN.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnAlignDN.DrawBorder(FALSE);
	m_btnAlignDN.DrawTransparent();

	m_btnOkSaveCam1.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnOkSaveCam1.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnOkSaveCam1.DrawBorder(FALSE);
	m_btnOkSaveCam1.DrawTransparent();

	m_btnOkSaveCam2.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnOkSaveCam2.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnOkSaveCam2.DrawBorder(FALSE);
	m_btnOkSaveCam2.DrawTransparent();

	m_btnNgSaveCam1.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnNgSaveCam1.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnNgSaveCam1.DrawBorder(FALSE);
	m_btnNgSaveCam1.DrawTransparent();

	m_btnNgSaveCam2.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnNgSaveCam2.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnNgSaveCam2.DrawBorder(FALSE);
	m_btnNgSaveCam2.DrawTransparent();

	m_btnKorea.SetIcon(IDI_ICON_LARGE_ON, IDI_ICON_LARGE_OFF);
	m_btnKorea.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnKorea.DrawBorder(FALSE);
	m_btnKorea.DrawTransparent();

	m_btnEnglish.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnEnglish.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnEnglish.DrawBorder(FALSE);
	m_btnEnglish.DrawTransparent();

	m_btnChiness.SetIcon(IDI_ICON_LARGE_ON,IDI_ICON_LARGE_OFF);
	m_btnChiness.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnChiness.DrawBorder(FALSE);
	m_btnChiness.DrawTransparent();

	m_btnWayLefttoRight.SetIcon(IDI_ICON_LED_ON, IDI_ICON_LED_OFF);
	m_btnWayLefttoRight.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	m_btnWayLefttoRight.DrawBorder(FALSE);
	m_btnWayLefttoRight.DrawTransparent();  



	for(i=0; i<DEF_DIO_PORTNUM; i++)
	{
		str.Format(_T("      %02d"), i);
		m_btnIN[i].SetIcon(IDI_ICON_LARGE_OFF);
		m_btnIN[i].SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_btnIN[i].DrawBorder(FALSE);
		m_btnIN[i].SetWindowText(str);
		m_btnIN[i].DrawTransparent();
		
		m_btnOUT[i].SetIcon(IDI_ICON_LARGE_ON, IDI_ICON_LARGE_OFF);
		m_btnOUT[i].SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_btnOUT[i].DrawBorder(FALSE);
		m_btnOUT[i].SetWindowText(str);
		m_btnOUT[i].DrawTransparent();
	}

	m_editPeriod.SetBkColor(RGB(255, 255, 255));
	m_editPeriod.SetFontSize(16);

	m_editDriver.SetBkColor(RGB(255, 255, 255));
	m_editDriver.SetFontSize(16);

	m_editSkipCnt.SetBkColor(RGB(255, 255, 255));
	m_editSkipCnt.SetFontSize(16);

	m_editDaqCnt.SetBkColor(RGB(255, 255, 255));
	m_editDaqCnt.SetFontSize(16);

	m_editUpSaveType.SetBkColor(RGB(255, 255, 255));
	m_editUpSaveType.SetFontSize(15);

	m_editDnSaveType.SetBkColor(RGB(255, 255, 255));
	m_editDnSaveType.SetFontSize(15);

	m_editDelayTime.SetBkColor(RGB(255, 255, 255));
	m_editDelayTime.SetFontSize(15);

	m_btnApply.SetIcon(IDI_ICON_SAVE);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnApply.DrawTransparent();

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	if (nLngSel==0) m_btnApply.SetWindowText(_T("저장"));
	else if (nLngSel==1) m_btnApply.SetWindowText(_T("SAVE"));
	else m_btnApply.SetWindowText(_T("貯藏"));
}

void CDlgSetting::InitSetupControls()
{
	long i, j;
	long nRow, nLngSel;
	CString str;
	INSP_RESULT_DATA  *pRes;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	NUiListDll_ClearCellAll(LIST_SETUP_ID);

	NUiListDll_SetRows(LIST_SETUP_ID, 45);
	NUiListDll_SetCols(LIST_SETUP_ID, 8);
	NUiListDll_SetRowHeight(LIST_SETUP_ID, 20);
	NUiListDll_SetTitleColor(LIST_SETUP_ID, RGB(200, 200, 200));
	NUiListDll_SetBackColor(LIST_SETUP_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_SETUP_ID, RGB(200, 200, 200));
	NUiListDll_SetFontSize(LIST_SETUP_ID, 15);
	NUiListDll_SetEnableEdit(LIST_SETUP_ID, TRUE);
	NUiListDll_SetColEnableEdit(LIST_SETUP_ID, 2, TRUE);
	NUiListDll_SetPrecisionNum(LIST_SETUP_ID, 2, 2);
	NUiListDll_SetColEnableEdit(LIST_SETUP_ID, 3, TRUE);
	NUiListDll_SetPrecisionNum(LIST_SETUP_ID, 3, 2);
	NUiListDll_SetColEnableEdit(LIST_SETUP_ID, 4, TRUE);
	NUiListDll_SetPrecisionNum(LIST_SETUP_ID, 4, 0);
//	NUiListDll_SetColEnableEdit(LIST_SETUP_ID, 5, TRUE);
//	NUiListDll_SetPrecisionNum(LIST_SETUP_ID, 5, 0);
	NUiListDll_SetColEnableEdit(LIST_SETUP_ID, 5, TRUE);
	NUiListDll_SetPrecisionNum(LIST_SETUP_ID, 5, 0);
	NUiListDll_SetColEnableCheck(LIST_SETUP_ID, 6, TRUE);
	NUiListDll_SetColEnableCheck(LIST_SETUP_ID, 7, TRUE);

	NUiListDll_SetColWidth(LIST_SETUP_ID, 0, 110);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 1, 100);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 2, 70);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 3, 70);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 4, 50);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 5, 40);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 6, 65);
	NUiListDll_SetColWidth(LIST_SETUP_ID, 7, 65);

	if (nLngSel==0)
	{
		NUiListDll_SetText(LIST_SETUP_ID, 0, 0, _T("카메라(상/하부)"));
		NUiListDll_SetText(LIST_SETUP_ID, 1, 0, _T("검사항목"));
		NUiListDll_SetText(LIST_SETUP_ID, 2, 0, _T("하한선"));
		NUiListDll_SetText(LIST_SETUP_ID, 3, 0, _T("상한선"));
		NUiListDll_SetText(LIST_SETUP_ID, 4, 0, _T("그래프"));
		NUiListDll_SetText(LIST_SETUP_ID, 5, 0, _T("출력"));
		NUiListDll_SetText(LIST_SETUP_ID, 6, 0, _T("검사유무"));
		NUiListDll_SetText(LIST_SETUP_ID, 7, 0, _T("불량배출"));
	}
	else if (nLngSel==1)
	{
		NUiListDll_SetText(LIST_SETUP_ID, 0, 0, _T("CAM(UP/DN)"));
		NUiListDll_SetText(LIST_SETUP_ID, 1, 0, _T("TITLE"));
		NUiListDll_SetText(LIST_SETUP_ID, 2, 0, _T("LSL"));
		NUiListDll_SetText(LIST_SETUP_ID, 3, 0, _T("USL"));
		NUiListDll_SetText(LIST_SETUP_ID, 4, 0, _T("PLOT"));
		NUiListDll_SetText(LIST_SETUP_ID, 5, 0, _T("OUT"));
		NUiListDll_SetText(LIST_SETUP_ID, 6, 0, _T("INSP"));
		NUiListDll_SetText(LIST_SETUP_ID, 7, 0, _T("Ejection"));
	}
	else
	{
		NUiListDll_SetText(LIST_SETUP_ID, 0, 0, _T("撮影(上/下部)"));
		NUiListDll_SetText(LIST_SETUP_ID, 1, 0, _T("檢査名稱"));
		NUiListDll_SetText(LIST_SETUP_ID, 2, 0, _T("下限線"));
		NUiListDll_SetText(LIST_SETUP_ID, 3, 0, _T("上限線"));
		NUiListDll_SetText(LIST_SETUP_ID, 4, 0, _T("圖形"));
		NUiListDll_SetText(LIST_SETUP_ID, 5, 0, _T("出力"));
		NUiListDll_SetText(LIST_SETUP_ID, 6, 0, _T("檢査與否"));
		NUiListDll_SetText(LIST_SETUP_ID, 7, 0, _T("不良排出"));
	}

	nRow = 0;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
    	pRes = (INSP_RESULT_DATA *)NAppDll_GetResultData(i);
	
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE ||
//				pRes->m_nInspType[j]==INSP_SLINE ||
				pRes->m_nInspType[j]==INSP_DATUM_C ||
				pRes->m_nInspType[j]==INSP_DATUM_B) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_bLocROI[j]) continue;

			nRow++;

			if (i==0) str.Format(_T("CAM%d(UP)"), i+1);
			else str.Format(_T("CAM%d(DN)"), i+1);
			NUiListDll_SetText(LIST_SETUP_ID, 0, nRow, str);

			str.Format(_T("%s"), pRes->m_strTitle[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 1, nRow, str);

			str.Format(_T("%5.2f"), pRes->m_dLowLimit[j]);
            NUiListDll_SetText(LIST_SETUP_ID, 2, nRow, str);

			str.Format(_T("%5.2f"), pRes->m_dHighLimit[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 3, nRow, str);

			/*
			if (pRes->m_nPlotPos[j]<1 || 2<pRes->m_nPlotPos[j])
				pRes->m_nPlotPos[j] = i+1;
			str.Format(_T("%d"), pRes->m_nPlotPos[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 4, nRow, str);
			*/

			if (pRes->m_nPlotNum[j]<0)
				pRes->m_nPlotNum[j] = 0;
			str.Format(_T("%d"), pRes->m_nPlotNum[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 4, nRow, str);

			str.Format(_T("%d"), pRes->m_nOutNo[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 5, nRow, str);

			NUiListDll_SetCheck(LIST_SETUP_ID, 6, nRow, pRes->m_bExecROI[j]);
			NUiListDll_SetCheck(LIST_SETUP_ID, 7, nRow, pRes->m_bEjectROI[j]);

			m_CAM[nRow] = i;
		}

		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (pRes->m_bLinkLocROI[j]) continue;

			nRow++;

			if (i==0) str.Format(_T("CAM%d(UP)"), i+1);
			else str.Format(_T("CAM%d(DN)"), i+1);
			NUiListDll_SetText(LIST_SETUP_ID, 0, nRow, str);

			str.Format(_T("%s"), pRes->m_strLinkTitle[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 1, nRow, str);

			str.Format(_T("%5.2f"), pRes->m_dLinkLowLimit[j]);
            NUiListDll_SetText(LIST_SETUP_ID, 2, nRow, str);

			str.Format(_T("%5.2f"), pRes->m_dLinkHighLimit[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 3, nRow, str);

			/*
			if (pRes->m_nLinkPlotPos[j]<1 || 2<pRes->m_nLinkPlotPos[j])
				pRes->m_nLinkPlotPos[j] = i+1;
			str.Format(_T("%d"), pRes->m_nLinkPlotPos[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 4, nRow, str);
			*/

			if (pRes->m_nLinkPlotNum[j]<0)
				pRes->m_nLinkPlotNum[j] = 0;
			str.Format(_T("%d"), pRes->m_nLinkPlotNum[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 4, nRow, str);

			str.Format(_T("%d"), pRes->m_nLinkOutNo[j]);
			NUiListDll_SetText(LIST_SETUP_ID, 5, nRow, str);

			NUiListDll_SetCheck(LIST_SETUP_ID, 6, nRow, pRes->m_bLinkExecROI[j]);
			NUiListDll_SetCheck(LIST_SETUP_ID, 7, nRow, pRes->m_bLinkEjectROI[j]);

			m_CAM[nRow] = i;
		}
	}

	NUiListDll_SetRow(LIST_SETUP_ID, nRow);
	NUiListDll_Invalidate(LIST_SETUP_ID);
}

void CDlgSetting::UpdateControlsData()
{
	long nLngSel;
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	str.Format(_T("%s"), m_pMainFrm->m_Setting.m_strSelDriver);
	m_editDriver.SetTextOut(str);
	str.Format(_T("%d"), m_pMainFrm->m_Setting.m_nDataPeriod);
	m_editPeriod.SetTextOut(str);

	str.Format(_T("%d"), m_pMainFrm->m_Setting.m_nSkipCnt);
	m_editSkipCnt.SetTextOut(str);
	str.Format(_T("%d"), m_pMainFrm->m_Setting.m_nDaqUnitCnt);
	m_editDaqCnt.SetTextOut(str);

	str.Format(_T("%d"), m_pMainFrm->m_Setting.m_nDelayTime);
	m_editDelayTime.SetTextOut(str);

	if (nLngSel==0) m_editUpSaveType.SetTextOut(_T("축소"));
	else if (nLngSel==1) m_editUpSaveType.SetTextOut(_T("Zoom Out"));
	else m_editUpSaveType.SetTextOut(_T("縮小"));

	if (m_pMainFrm->m_Setting.m_bSaveImageType[CAM_UP])
	{
		if (nLngSel==0) m_editUpSaveType.SetTextOut(_T("1:1 원본"));
		else if (nLngSel==1) m_editUpSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editUpSaveType.SetTextOut(_T("1:1 原本"));
	}

	if (nLngSel==0) m_editDnSaveType.SetTextOut(_T("축소"));
	else if (nLngSel==1) m_editDnSaveType.SetTextOut(_T("Zoom Out"));
	else m_editDnSaveType.SetTextOut(_T("縮小"));

	if (m_pMainFrm->m_Setting.m_bSaveImageType[CAM_DN])
	{
		if (nLngSel==0) m_editDnSaveType.SetTextOut(_T("1:1 원본"));
		else if (nLngSel==1) m_editDnSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editDnSaveType.SetTextOut(_T("1:1 原本"));
	}

	m_btnExecCam1.SetCheck(m_pMainFrm->m_Setting.m_bExecInsp[CAM_UP]);
	m_btnOkSaveCam1.SetCheck(m_pMainFrm->m_Setting.m_bOKImageSave[CAM_UP]);
	m_btnNgSaveCam1.SetCheck(m_pMainFrm->m_Setting.m_bNGImageSave[CAM_UP]);
	m_btnSWGrab.SetCheck(m_pMainFrm->m_Setting.m_bSWGrab);

	//m_btnWayLefttoRight.SetCheck(m_pMainFrm->m_Setting.m_bWayLefttoRight);

	m_btnExecCam2.SetCheck(m_pMainFrm->m_Setting.m_bExecInsp[CAM_DN]);
	m_btnOkSaveCam2.SetCheck(m_pMainFrm->m_Setting.m_bOKImageSave[CAM_DN]);
	m_btnNgSaveCam2.SetCheck(m_pMainFrm->m_Setting.m_bNGImageSave[CAM_DN]);

	m_btnCutOffset.SetCheck(m_pMainFrm->m_Setting.m_bExecCutOffset);
	m_btnEPCOffset.SetCheck(m_pMainFrm->m_Setting.m_bExecEPCOffset);
	m_btnT2BOffset.SetCheck(m_pMainFrm->m_Setting.m_bExecT2BOffset);	
	m_btnDataTrans.SetCheck(m_pMainFrm->m_Setting.m_bExecDataTrans);	

	if (m_pMainFrm->m_Setting.m_nSelUpDn)
		m_btnAlignDN.SetCheck(TRUE);
	else
		m_btnAlignUP.SetCheck(TRUE);

	m_btnKorea.SetCheck(FALSE);
	m_btnEnglish.SetCheck(FALSE);
	m_btnChiness.SetCheck(FALSE);

	m_bitmapLang.DeleteObject();
	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	{
		m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP0);
		m_staticSetup.SetBitmap(m_bitmapLang);
		m_btnKorea.SetCheck(TRUE);
	}
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
	{
		m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP1);
		m_staticSetup.SetBitmap(m_bitmapLang);
		m_btnEnglish.SetCheck(TRUE);
	}
	else
	{
		m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP2);
		m_staticSetup.SetBitmap(m_bitmapLang);
		m_btnChiness.SetCheck(TRUE);
	}

	UpdateData(false);

	InitSetupControls();
}

void CDlgSetting::OnBnClickedBtnExecCam1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnBnClickedBtnOksaveCam1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnBnClickedBtnNgsaveCam1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnBnClickedBtnExecCam2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnBnClickedBtnOksaveCam2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnBnClickedBtnNgsaveCam2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSetting::OnDeltaposSpinDrive(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;

	str = m_editDriver.GetText();
	if(pNMUpDown->iDelta < 0)
	{
		if(str[0]<'Z') str.Replace(str[0], str[0]+1);
	}
	else if(pNMUpDown->iDelta > 0)
	{
		if(str[0]>'C') str.Replace(str[0], str[0]-1);
	}

	m_editDriver.SetTextOut(str);
	*pResult = 0;
}

void CDlgSetting::OnDeltaposSpinPeriod(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString str;
	int nNum;

	nNum = _ttoi(m_editPeriod.GetText());

	if(pNMUpDown->iDelta < 0)
		nNum-= pNMUpDown->iDelta;
	else if(pNMUpDown->iDelta > 0)
		nNum-= pNMUpDown->iDelta;
	else return;

	if (nNum<1)  nNum = 1;
	else if(nNum>90)  nNum = 90;

	str.Format(_T("%d"), nNum);
	m_editPeriod.SetTextOut(str);

	*pResult = 0;
}

void CDlgSetting::OnBnClickedCheckOut0()
{
	if (m_btnOUT[0].GetCheck()) m_pMainFrm->NPio_BitWrite(0, TRUE);
	else m_pMainFrm->NPio_BitWrite(0, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut1()
{
	if (m_btnOUT[1].GetCheck()) m_pMainFrm->NPio_BitWrite(1, TRUE);
	else m_pMainFrm->NPio_BitWrite(1, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut2()
{
	if (m_btnOUT[2].GetCheck()) m_pMainFrm->NPio_BitWrite(2, TRUE);
	else m_pMainFrm->NPio_BitWrite(2, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut3()
{
	if (m_btnOUT[3].GetCheck())m_pMainFrm->NPio_BitWrite(3, TRUE);
	else m_pMainFrm->NPio_BitWrite(3, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut4()
{
	if (m_btnOUT[4].GetCheck())m_pMainFrm->NPio_BitWrite(4, TRUE);
	else m_pMainFrm->NPio_BitWrite(4, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut5()
{
	if (m_btnOUT[5].GetCheck())m_pMainFrm->NPio_BitWrite(5, TRUE);
	else m_pMainFrm->NPio_BitWrite(5, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut6()
{
	if (m_btnOUT[6].GetCheck())m_pMainFrm->NPio_BitWrite(6, TRUE);
	else m_pMainFrm->NPio_BitWrite(6, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut7()
{
	if (m_btnOUT[7].GetCheck())m_pMainFrm->NPio_BitWrite(7, TRUE);
	else m_pMainFrm->NPio_BitWrite(7, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut8()
{
	if (m_btnOUT[8].GetCheck())m_pMainFrm->NPio_BitWrite(8, TRUE);
	else m_pMainFrm->NPio_BitWrite(8, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut9()
{
	if (m_btnOUT[9].GetCheck())m_pMainFrm->NPio_BitWrite(9, TRUE);
	else m_pMainFrm->NPio_BitWrite(9, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut10()
{
	if (m_btnOUT[10].GetCheck())m_pMainFrm->NPio_BitWrite(10, TRUE);
	else m_pMainFrm->NPio_BitWrite(10, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut11()
{
	if (m_btnOUT[11].GetCheck())m_pMainFrm->NPio_BitWrite(11, TRUE);
	else m_pMainFrm->NPio_BitWrite(11, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut12()
{
	if (m_btnOUT[12].GetCheck())m_pMainFrm->NPio_BitWrite(12, TRUE);
	else m_pMainFrm->NPio_BitWrite(12, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut13()
{
	if (m_btnOUT[13].GetCheck())m_pMainFrm->NPio_BitWrite(13, TRUE);
	else m_pMainFrm->NPio_BitWrite(13, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut14()
{
	if (m_btnOUT[14].GetCheck())m_pMainFrm->NPio_BitWrite(14, TRUE);
	else m_pMainFrm->NPio_BitWrite(14, FALSE);
}

void CDlgSetting::OnBnClickedCheckOut15()
{
	if (m_btnOUT[15].GetCheck())m_pMainFrm->NPio_BitWrite(15, TRUE);
	else m_pMainFrm->NPio_BitWrite(15, FALSE);
}

void CDlgSetting::OnBnClickedBtnApply()
{
	long i, j, nLangSel;
	CString str, strMessage;

	UpdateData(true);

	nLangSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	if (nLangSel==0)
		strMessage = _T(" 변경된 설정 데이타로 적용하시겠습니까 ? ");
	else if (nLangSel==1)
		strMessage = _T(" Do you want apply the changed data ? ");
	else
		strMessage = _T("是否适用已变更Data数据？");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		for(j=0; j<2; j++)
		{
			ApplySpecSetup();

			str = m_editDriver.GetText();
			_tcscpy_s(m_pMainFrm->m_Setting.m_strSelDriver, str);

			m_pMainFrm->m_Setting.m_nDataPeriod = (long)m_editPeriod.GetTextValue();
			m_pMainFrm->m_Setting.m_nDaqUnitCnt = (long)m_editDaqCnt.GetTextValue();
			m_pMainFrm->m_Setting.m_nSkipCnt = (long)m_editSkipCnt.GetTextValue();

			str = m_editUpSaveType.GetText();
			m_pMainFrm->m_Setting.m_bSaveImageType[CAM_UP] = 0;
			if (str==_T("1:1 원본") || str==_T("1:1 IMAGE") || str==_T("1:1 原本")) 
				m_pMainFrm->m_Setting.m_bSaveImageType[CAM_UP] = 1;

			str = m_editDnSaveType.GetText();
			m_pMainFrm->m_Setting.m_bSaveImageType[CAM_DN] = 0;
			if (str==_T("1:1 원본") || str==_T("1:1 IMAGE") || str==_T("1:1 原本"))
				m_pMainFrm->m_Setting.m_bSaveImageType[CAM_DN] = 1;

			m_pMainFrm->m_Setting.m_bExecInsp[CAM_UP] = m_btnExecCam1.GetCheck();
			m_pMainFrm->m_Setting.m_bOKImageSave[CAM_UP] = m_btnOkSaveCam1.GetCheck();
			m_pMainFrm->m_Setting.m_bNGImageSave[CAM_UP] = m_btnNgSaveCam1.GetCheck();
			m_pMainFrm->m_Setting.m_nDelayTime = (long)m_editDelayTime.GetTextValue();

			m_pMainFrm->m_Setting.m_bExecInsp[CAM_DN] = m_btnExecCam2.GetCheck();
			m_pMainFrm->m_Setting.m_bOKImageSave[CAM_DN] = m_btnOkSaveCam2.GetCheck();
			m_pMainFrm->m_Setting.m_bNGImageSave[CAM_DN] = m_btnNgSaveCam2.GetCheck();

			m_pMainFrm->m_Setting.m_bExecCutOffset = m_btnCutOffset.GetCheck();
			m_pMainFrm->m_Setting.m_bExecEPCOffset = m_btnEPCOffset.GetCheck();
			m_pMainFrm->m_Setting.m_bExecT2BOffset = m_btnT2BOffset.GetCheck();
			m_pMainFrm->m_Setting.m_bExecDataTrans = m_btnDataTrans.GetCheck();
			m_pMainFrm->m_Setting.m_bSWGrab = m_btnSWGrab.GetCheck();

//			m_pMainFrm->m_Setting.m_bWayLefttoRight = m_btnWayLefttoRight.GetCheck();
		
			if (m_btnKorea.GetCheck()) nLangSel = 0;
			if (m_btnEnglish.GetCheck()) nLangSel = 1;
			if (m_btnChiness.GetCheck()) nLangSel = 2; 

			if (m_btnAlignUP.GetCheck()) m_pMainFrm->m_Setting.m_nSelUpDn = 0;
			else m_pMainFrm->m_Setting.m_nSelUpDn = 1;

			m_pMainFrm->m_Setting.m_nSelLanguage = nLangSel;
			m_pMainFrm->m_pView->m_pDlgMainBar->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgSubBar->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgExecAuto->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgResult->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgPlot->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgQuery->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgSetting->SetTransLanguage(nLangSel);

			/*
			m_pMainFrm->m_pView->m_pDlgTestShort->SetTransLanguage(nLangSel);
			m_pMainFrm->m_pView->m_pDlgTestShort->m_pDlgShortSetup->SetTransLanguage(nLangSel);
			*/

			for(i=0; i<CAM_MAX_NUM; i++)
			{
				NAppDll_SetTransLanguage(i, nLangSel);
			}

			m_bitmapLang.DeleteObject();
			if (nLangSel==0)
			{
				m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP0);
				m_staticSetup.SetBitmap(m_bitmapLang);
			}
			else if (nLangSel==1)
			{
				m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP1);
				m_staticSetup.SetBitmap(m_bitmapLang);
			}
			else 
			{
				m_bitmapLang.LoadBitmap(IDB_BITMAP_SETUP2);
				m_staticSetup.SetBitmap(m_bitmapLang);
			}
		}
	}

	Invalidate(false);
}

void CDlgSetting::ApplySpecSetup()
{
	BOOL bExec, bEject;
	long i, j, nPlot, nOutNo, nPlotMaxNum;
	float fStdSpec, fToleSpec;
	CString str, strT, strTitle, strMessage;
	TCHAR lpStr[100];
	INSP_RESULT_DATA  *pResCam1, *pResCam2;

	pResCam1 = (INSP_RESULT_DATA *)NAppDll_GetResultData(0);
	pResCam2 = (INSP_RESULT_DATA *)NAppDll_GetResultData(1);

	nPlotMaxNum = PLOT_PAGE_MAX_NUM*PLOT_MAX_NUM;

	for(i=1; i<GOM_MAX_NUM; i++)
	{
		NUiListDll_GetText(LIST_SETUP_ID, 1, i, lpStr);
		strTitle = WithoutSpaceToString(lpStr);
		if (!_tcscmp(strTitle, _T(""))) break;

		NUiListDll_GetText(LIST_SETUP_ID, 2, i, lpStr);
		fStdSpec = (float)_ttof(lpStr);

		NUiListDll_GetText(LIST_SETUP_ID, 3, i, lpStr);
		fToleSpec = (float)_ttof(lpStr);

		NUiListDll_GetText(LIST_SETUP_ID, 4, i, lpStr);
		nPlot = _ttoi(lpStr);
		if (nPlot>nPlotMaxNum)
			nPlot = 0;

		NUiListDll_GetText(LIST_SETUP_ID, 5, i, lpStr);
		nOutNo = _ttoi(lpStr);
		if (nOutNo<1 && nOutNo>10)
			nOutNo = 0;

		bExec = NUiListDll_GetCheck(LIST_SETUP_ID, 6, i);
		bEject = NUiListDll_GetCheck(LIST_SETUP_ID, 7, i);
		if (!bExec) 
			bEject=FALSE;

		if (m_CAM[i]==0)
		{
			for(j=0; j<GOM_MAX_NUM; j++)
			{
				if (pResCam1->m_nInspType[j]==INSP_MEASURE_SIDE_ONE ||
//				    pResCam1->m_nInspType[j]==INSP_SLINE ||
					pResCam1->m_nInspType[j]==INSP_DATUM_C ||
					pResCam1->m_nInspType[j]==INSP_DATUM_B) continue;

				strT = WithoutSpaceToString((TCHAR *)pResCam1->m_strTitle[j]);
				if (!_tcscmp(strTitle, strT))
				{
					_tcscpy_s(pResCam1->m_strTitle[j], strTitle);
					pResCam1->m_nPlotNum[j] = nPlot;
					pResCam1->m_nOutNo[j] = nOutNo;
					pResCam1->m_bExecROI[j] = bExec;
					pResCam1->m_bEjectROI[j] = bEject;
					pResCam1->m_dLowLimit[j] = fStdSpec;
					pResCam1->m_dHighLimit[j] = fToleSpec;
				}
			}
	
			for(j=0; j<pResCam1->m_nLinkNum; j++)
			{
				strT = WithoutSpaceToString((TCHAR *)pResCam1->m_strLinkTitle[j]);
				if (!_tcscmp(strTitle, strT))
				{
					_tcscpy_s(pResCam1->m_strLinkTitle[j], strTitle);
					pResCam1->m_nLinkPlotNum[j] = nPlot;
					pResCam1->m_nLinkOutNo[j] = nOutNo;
					pResCam1->m_bLinkExecROI[j] = bExec;
					pResCam1->m_bLinkEjectROI[j] = bEject;
					pResCam1->m_dLinkLowLimit[j] = fStdSpec;
					pResCam1->m_dLinkHighLimit[j] = fToleSpec;
				}
    		}
		}
		else
		{
			for(j=0; j<GOM_MAX_NUM; j++)
			{
				if (pResCam2->m_nInspType[j]==INSP_MEASURE_SIDE_ONE ||
//				    pResCam2->m_nInspType[j]==INSP_SLINE ||
					pResCam2->m_nInspType[j]==INSP_DATUM_C ||
					pResCam2->m_nInspType[j]==INSP_DATUM_B) continue;

				strT = WithoutSpaceToString((TCHAR *)pResCam2->m_strTitle[j]);
				if (!_tcscmp(strTitle, strT))
				{
					_tcscpy_s(pResCam2->m_strTitle[j], strTitle);
					pResCam2->m_nPlotNum[j] = nPlot;
					pResCam2->m_nOutNo[j] = nOutNo;
					pResCam2->m_bExecROI[j] = bExec;
					pResCam2->m_bEjectROI[j] = bEject;
					pResCam2->m_dLowLimit[j] = fStdSpec;
					pResCam2->m_dHighLimit[j] = fToleSpec;
				}
			}
	
			for(j=0; j<pResCam2->m_nLinkNum; j++)
			{
				strT = WithoutSpaceToString((TCHAR *)pResCam2->m_strLinkTitle[j]);
				if (!_tcscmp(strTitle, strT))
				{
					_tcscpy_s(pResCam2->m_strLinkTitle[j], strTitle);
					pResCam2->m_nLinkPlotNum[j] = nPlot;
					pResCam2->m_nLinkOutNo[j] = nOutNo;
					pResCam2->m_bLinkExecROI[j] = bExec;
					pResCam2->m_bLinkEjectROI[j] = bEject;
					pResCam2->m_dLinkLowLimit[j] = fStdSpec;
					pResCam2->m_dLinkHighLimit[j] = fToleSpec;
				}
			}
		}
	}

	NAppDll_SetSpecData(0);
	NAppDll_SetSpecData(1);
}

void CDlgSetting::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		UpdateControlsData();
	    InitSetupControls();

		SetTimer(1000, 200, NULL);
	}
	else
		KillTimer(1000);
}

void CDlgSetting::OnBnClickedCheckExecCam1()
{

}

void CDlgSetting::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bRet;
	long i, nTemp;

    if (nIDEvent==1000)
	{
		nTemp =m_pMainFrm->NPio_Read();
		for(i=0; i<DEF_DIO_PORTNUM; i++)
		{
		    bRet = (nTemp>>i) & 0x01;
			if (bRet)
				m_btnIN[i].SetIcon(IDI_ICON_LARGE_ON);
			else
				m_btnIN[i].SetIcon(IDI_ICON_LARGE_OFF);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgSetting::OnBnClickedCheckKorea()
{
	m_btnKorea.SetCheck(TRUE);
	m_btnEnglish.SetCheck(FALSE);
    m_btnChiness.SetCheck(FALSE);
}

void CDlgSetting::OnBnClickedCheckEnglish()
{
	m_btnKorea.SetCheck(FALSE);
	m_btnEnglish.SetCheck(TRUE);
    m_btnChiness.SetCheck(FALSE);
}

void CDlgSetting::OnBnClickedCheckChiness()
{
	m_btnKorea.SetCheck(FALSE);
	m_btnEnglish.SetCheck(FALSE);
    m_btnChiness.SetCheck(TRUE);
}

void CDlgSetting::OnBnClickedCheckExecCam2()
{
	// TODO: Add your control notification handler code here
}

void CDlgSetting::OnBnClickedCheckExecOffset()
{
	// TODO: Add your control notification handler code here
}

void CDlgSetting::OnDeltaposSpinUpSaveType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	long nLngSel, nNum=0;
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	str = m_editUpSaveType.GetText();
	if (str==_T("1:1 원본") || str==_T("1:1 IMAGE") || str==_T("1:1 原本")) nNum = 1;
	else nNum = 0;

	if(pNMUpDown->iDelta < 0)
		nNum-= pNMUpDown->iDelta;
	else if(pNMUpDown->iDelta > 0)
		nNum-= pNMUpDown->iDelta;
	else return;

	if (nNum<0)  nNum = 0;
	else if(nNum>1)  nNum = 1;

	if (nNum==0)
	{
		if (nLngSel==0) m_editUpSaveType.SetTextOut(_T("축소"));
		else if (nLngSel==1) m_editUpSaveType.SetTextOut(_T("Zoom Out"));
		else m_editUpSaveType.SetTextOut(_T("缩小"));
	}
	else
	{
		if (nLngSel==0) m_editUpSaveType.SetTextOut(_T("1:1 원본"));
		else if (nLngSel==1) m_editUpSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editUpSaveType.SetTextOut(_T("1:1 原本"));
	}

	*pResult = 0;
}

void CDlgSetting::OnDeltaposSpinDnSaveType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	long nLngSel, nNum=0;
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	str = m_editDnSaveType.GetText();
	if (str==_T("1:1 원본") || str==_T("1:1 IMAGE") || str==_T("1:1 原本")) nNum = 1;
	else nNum = 0;

	if(pNMUpDown->iDelta < 0)
		nNum-= pNMUpDown->iDelta;
	else if(pNMUpDown->iDelta > 0)
		nNum-= pNMUpDown->iDelta;
	else return;

	if (nNum<0)  nNum = 0;
	else if(nNum>1)  nNum = 1;

	if (nNum==0) 
	{
		if (nLngSel==0) m_editDnSaveType.SetTextOut(_T("축소"));
		else if (nLngSel==1) m_editDnSaveType.SetTextOut(_T("Zoom Out"));
		else m_editDnSaveType.SetTextOut(_T("缩小"));
	}
	else
	{
		if (nLngSel==0) m_editDnSaveType.SetTextOut(_T("1:1 원본"));
		else if (nLngSel==1) m_editDnSaveType.SetTextOut(_T("1:1 IMAGE"));
		else m_editDnSaveType.SetTextOut(_T("1:1 原本"));
	}

	*pResult = 0;
}

void CDlgSetting::OnBnClickedCheckAlignUp()
{
	m_btnAlignUP.SetCheck(TRUE);
	m_btnAlignDN.SetCheck(FALSE);
}

void CDlgSetting::OnBnClickedCheckAlignDn()
{
	m_btnAlignUP.SetCheck(FALSE);
	m_btnAlignDN.SetCheck(TRUE);
}

void CDlgSetting::OnBnClickedBtnIn0()
{
	// TODO: Add your control notification handler code here
}

void CDlgSetting::OnBnClickedCheckCutOffset()
{
	// TODO: Add your control notification handler code here
}
