// DlgExecAuto.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgExecAuto.h"
#include "MainFrm.h"
#include "math.h"
#include "NAppDll.h"
#include "NGuiDll.h"
#include "NVisionDll.h"
#include "NPioControlDll.h"
#include "afxdialogex.h"

//#import "NComOmronDll.tlb" no_namespace named_guids

extern CMainFrame *m_pMainFrm;

void CALLBACK NAppDll_CallBackEvent();
NAppDll_CallDATA    NAppDll_Data;

// CDlgExecAuto 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgExecAuto, CDialogEx)

CDlgExecAuto::CDlgExecAuto(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExecAuto::IDD, pParent)
{
	InitializeCriticalSection(&m_csSync);

	m_nCntSendDIO = 0;
	m_nCntReceiveDIO = 0;
    m_nErrResponseCnt = 0;
	m_bOpenComm = FALSE;
}

CDlgExecAuto::~CDlgExecAuto()
{
	delete m_pDlgBadImage;

	DeleteCriticalSection(&m_csSync);
}

void CDlgExecAuto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_CAM, m_tabCAM);
	DDX_Control(pDX, IDC_TAB_IMAGE_CAM, m_tabImgCAM);	
	DDX_Control(pDX, IDC_TAB_IMAGE, m_tabImage);
	DDX_Control(pDX, IDC_ACTEASYIF, m_ActEasyIF);
}

BEGIN_MESSAGE_MAP(CDlgExecAuto, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CAM, &CDlgExecAuto::OnSelchangeTabCam)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE, &CDlgExecAuto::OnSelchangeTabImage)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgExecAuto::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgExecAuto::OnBnClickedButton2)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE_CAM, &CDlgExecAuto::OnSelchangeTabImageCam)
	ON_BN_CLICKED(IDC_BTN_STOP_RNR, &CDlgExecAuto::OnBnClickedBtnStopRnr)
	ON_BN_CLICKED(IDC_BTN_START_RNR, &CDlgExecAuto::OnBnClickedBtnStartRnr)
	ON_BN_CLICKED(IDC_BTN_TRIGGER_POS, &CDlgExecAuto::OnBnClickedBtnTriggerPos)
	ON_STN_CLICKED(IDC_STATIC_LIST, &CDlgExecAuto::OnStnClickedStaticList)
END_MESSAGE_MAP()

// CDlgExecAuto 메시지 처리기입니다.
BOOL CDlgExecAuto::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	// 버튼, 비전, 카메라, 기타 Gauge R&R 테스트 컨트롤 초기화 
	InitButtonControls();
    InitCameraControls();
	InitEtcControls();
	// 버튼, 비전, 카메라, 기타 Gauge R&R 테스트 컨트롤 초기화 

	UpdateModelName();
	SetTransLanguage(m_pMainFrm->m_Setting.m_nSelLanguage);

	// 불량영상 저장 THREAD 초기화, SetEvent로 처리됨 
	m_hEventExport = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadExport = AfxBeginThread(ThreadDataExport, this, THREAD_PRIORITY_LOWEST);
	if (m_pThreadExport)
		m_pThreadExport->m_bAutoDelete = TRUE;
	// 불량영상 저장 THREAD 초기화, SetEvent로 처리됨 

	// EPC 보정값 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 
	/*
	m_hEventMelsec = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadMelsec = AfxBeginThread(ThreadMelsecComm, this, THREAD_PRIORITY_LOWEST);
	if (m_pThreadMelsec)
		m_pThreadMelsec->m_bAutoDelete = TRUE;
		*/
	// EPC 보정값 전송 THREAD 초기화, SetEvent로 처리됨 

	// 실시간 측정 데이타 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 
	m_hEventSendData = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThreadSendData = AfxBeginThread(ThreadSendDataToPLC, this, THREAD_PRIORITY_BELOW_NORMAL);
	if (m_pThreadSendData)
		m_pThreadSendData->m_bAutoDelete = TRUE;
	// 실시간 측정 데이타 전송 THREAD 초기화(PC->PLC), SetEvent로 처리됨 

	memset(m_nUnWinderInfo, 0x0000, sizeof(short)*UNW_INFO_END);


	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgExecAuto::InitButtonControls()
{
	CString str;

	// LIST_ID 리스트 DLL초기화 
	NUiListDll_Open(LIST_MAIN_ID, (long)GetDlgItem(IDC_STATIC_LIST));
	NUiListDll_Open(LIST_INFO_ID, (long)GetDlgItem(IDC_STATIC_INFO));
	// LIST_ID 리스트 DLL초기화 

	// 불량영상 출력 화면 생성 
	m_pDlgBadImage = new CDlgBadImage((CWnd*)GetDlgItem(IDC_STATIC_IMAGE));
	m_pDlgBadImage->Create(IDD_DIALOG_BAD_IMAGE, GetDlgItem(IDC_STATIC_IMAGE));
	m_pDlgBadImage->ShowWindow(SW_SHOW);
	// 불량영상 출력 화면 생성 

	// PLC통신 ETHERNET 초기화 
#if _MELSEC
	long nRet;
	try
	{
     	m_ActEasyIF.put_ActLogicalStationNumber(0);	// Exec set-property method 
		nRet = m_ActEasyIF.Open();	// Open method exec
		if (nRet==0)
		    m_bOpenComm = TRUE;
	}
	catch (...)
	{
        m_pMainFrm->WriteTextMessage(DEF_MSG_PLC_ERROR);
	}
#endif
	// PLC통신 ETHERNET 초기화 	
}

void CDlgExecAuto::InitEtcControls()
{
	// GAUGE R&R 데이타를 추출를 자동 실행할 경우 버튼 활성화
#if _GAUGE_RNR
	GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START_RNR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP_RNR)->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START_RNR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP_RNR)->EnableWindow(FALSE);
#endif
	// GAUGE R&R 데이타를 추출를 자동 실행할 경우 버튼 활성화
}

void CDlgExecAuto::InitCameraControls()
{
	long i;

	BeginWaitCursor();

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		// 비전 출력 화면의 PICTURE BOX를 윈도우에 배치
		GetDlgItem(IDC_STATIC_CAM1+i)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
		// 비전 출력 화면의 PICTURE BOX를 윈도우에 배치

		// FRAME의 크기와 영상의 ZOOM으로 DLL를 초기화 한다.
		//NAppDll_SetFrameSize(i, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		//NAppDll_SetZoomScale(i, m_pMainFrm->m_Setting.m_fCamScale[i]);
	    NAppDll_Open((long)GetDlgItem(IDC_STATIC_CAM1+i), i, IMAGE_WIDTH, IMAGE_HEIGHT);
		NAppDll_SetFrameSize(i, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		// FRAME의 크기와 영상의 ZOOM으로 DLL를 초기화 한다.

		// 실시간 검사모드가 아닌 상태로 초기화 
		NAppDll_RealTimeMode(i, FALSE);

		// 비전 DLL의 CALLBACK EVENT함수와 데이타 구조를 전달함		// 비전 DLL의 CALLBACK EVENT함수와 데이타 구조를 전달함
		NAppDll_SetEvent(i, NAppDll_CallBackEvent, (long)&NAppDll_Data);

		// 현재 언어 선택 설정값을 보냄 
		NAppDll_SetTransLanguage(i, m_pMainFrm->m_Setting.m_nSelLanguage);
	}

	// 현재 모델명으로 모델 데이타를 읽어온다.
	m_pMainFrm->LoadModelData();

	// 비전 윈도우를 출력함
	ShowVisionWindow(0, TRUE);
	
	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		// 비전 DLL의 결과 데이타를 초기화하고 결과 데이틀 구조체로 읽어온다.
		NAppDll_InitResultData(i);
		m_pMainFrm->m_pRes[i] = (INSP_RESULT_DATA *)NAppDll_GetResultData(i);

		// 초기 비전의 ZOOM SCALE를 초기화하고 적용함
		m_pMainFrm->m_Setting.m_fCamScale[i] = DEF_INIT_ZOOM_SCALE;
		NAppDll_SetZoomScale(i, m_pMainFrm->m_Setting.m_fCamScale[i]);
	}

    for(i=0; i<CAM_MAX_NUM; i++)
	{
		m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
	}

	// CAMERA TAB 위치 설정, 방향 PICTURE BOX위치 설정
	GetDlgItem(IDC_TAB_CAM)->SetWindowPos(NULL, 1, FRAME_HEIGHT-10, 350, 40, SWP_SHOWWINDOW);
	GetDlgItem(IDC_STATIC_FORWARD_DIR)->SetWindowPos(NULL, 350, FRAME_HEIGHT+2, FRAME_WIDTH-350, 30, SWP_SHOWWINDOW);
	// CAMERA TAB 위치 설정, 방향 PICTURE BOX위치 설정
	
	// CAMERA TAB 상부 TAB으로 지정 0:상부, 1:하부, 2:상/하부, 3:불량영상
	GetSelectedTab(0);
	// CAMERA TAB 상부 TAB으로 지정 0:상부, 1:하부, 2:상/하부, 3:불량영상
	
	// 검사 결과 리스트 초기화 
	InitListControls();
	// 검사 결과 리스트 초기화 

	// 카운트 데이타 
	InitCountData();
	// 카운트 데이타 

	m_pMainFrm->m_CAM.SetExposureTime(0, DEF_EXPOSURE_TIME);
	m_pMainFrm->m_CAM.SetExposureTime(1, DEF_EXPOSURE_TIME);

	m_pMainFrm->ExecMessageHandleAll();

	EndWaitCursor();
}

void CDlgExecAuto::SetTransLanguage(long nValue)
{
	// 초기 상부 카메라 TAB을 선택
	GetSelectedTab(0);
	// 초기 상부 카메라 TAB을 선택

	// 초기 상부 카메라 TAB을 선택
	GetSelectedSubTab();

	InitListControls();
}

void CDlgExecAuto::CloseControls()
{
	long i;

	// 해당 THREAD에 EVENT를 보내고 THREAD를 종료한다. 
	SetEvent(m_hEventSendData);
	SetEvent(m_hEventExport);
	//SetEvent(m_hEventMelsec);
	// 해당 THREAD에 EVENT를 보내고 THREAD를 종료한다. 

	// PLC의 ETHERNET를 Close합니다. 
#if _MELSEC
	m_ActEasyIF.Close();
#endif
	// PLC의 ETHERNET를 Close합니다. 

	// 비전 DLL를 종료함
	for(i=0; i<VIEW_MAX_NUM; i++)
	    NAppDll_Close(i);
	// 비전 DLL를 종료함

	// LIST_ID 리스트 DLL를 종료합니다.
	NUiListDll_Close(LIST_MAIN_ID);
	NUiListDll_Close(LIST_INFO_ID);
	// LIST_ID 리스트 DLL를 종료합니다.

	Sleep(100);
}

void CDlgExecAuto::ShowVisionWindow(long nCam, BOOL bShow)
{
	EnterCriticalSection(&m_csSync);

	// 각종 CONTROL의 위치를 배치함
	GetDlgItem(IDC_STATIC_CAM1)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_STATIC_CAM2)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_TAB_IMAGE)->SetWindowPos(NULL, 1, FRAME_HEIGHT-35, FRAME_WIDTH, 31, SWP_HIDEWINDOW);
	GetDlgItem(IDC_STATIC_IMAGE)->SetWindowPos(NULL, 1, 32, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_HIDEWINDOW);
	GetDlgItem(IDC_TAB_IMAGE_CAM)->SetWindowPos(NULL, 1, 8, FRAME_WIDTH, 30, SWP_HIDEWINDOW);
	// 각종 CONTROL의 위치를 배치함

	if (nCam<CAM_MAX_NUM)
	{
		// 상부 또는 하부 TAB이 선택될 경우 IDC_STATIC_CAM1의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
		if (bShow)
			GetDlgItem(IDC_STATIC_CAM1+nCam)->SetWindowPos(NULL, 1, 1, FRAME_WIDTH, FRAME_HEIGHT, SWP_SHOWWINDOW);

		NAppDll_SetFrameSize(nCam, FRAME_WIDTH, FRAME_HEIGHT, 1, 8);
		//m_pMainFrm->m_Setting.m_fCamScale[nCam] = NAppDll_GetZoomScale(nCam);
		NAppDll_SetZoomScale(nCam, m_pMainFrm->m_Setting.m_fCamScale[nCam]);
		// 상부 또는 하부 TAB이 선택될 경우 IDC_STATIC_CAM1의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
	}
	else if (nCam==VIEW_MAX_NUM)
	{
		// 상/하부 TAB이 선택될 경우 두 PICTURE BOX의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
		if (bShow)
		{
			GetDlgItem(IDC_STATIC_CAM1)->SetWindowPos(NULL, DEF_V1_SMALL_LEFT, DEF_V1_SMALL_TOP, DEF_V1_SMALL_WIDTH, DEF_V1_SMALL_HEIGHT, SWP_SHOWWINDOW);
			GetDlgItem(IDC_STATIC_CAM2)->SetWindowPos(NULL, DEF_V2_SMALL_LEFT, DEF_V2_SMALL_TOP, DEF_V2_SMALL_WIDTH, DEF_V2_SMALL_HEIGHT, SWP_SHOWWINDOW);
	    }

		NAppDll_SetFrameSize(CAM_UP, DEF_V1_SMALL_WIDTH, DEF_V1_SMALL_HEIGHT, 1, 8);
		NAppDll_SetFrameSize(CAM_DN, DEF_V2_SMALL_WIDTH, DEF_V2_SMALL_HEIGHT, 1, 8);
		NAppDll_SetZoomScale(CAM_UP, DEF_MINI_ZOOM_SCALE);
		NAppDll_SetZoomScale(CAM_DN, DEF_MINI_ZOOM_SCALE);
		// 상/하부 TAB이 선택될 경우 두 PICTURE BOX의 위치와 크기를 배치하고 ZOOM 스케일을 적용함 
	}
	else
	{
		// 불량영상 TAB이 선택될 경우 CONTROL의 위치와 크기를 지정함
		GetDlgItem(IDC_TAB_IMAGE_CAM)->SetWindowPos(NULL, 1, 8, FRAME_WIDTH+2, 30, SWP_SHOWWINDOW);
		GetDlgItem(IDC_TAB_IMAGE)->SetWindowPos(NULL, 1, FRAME_HEIGHT-52, FRAME_WIDTH+2, 35, SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_IMAGE)->SetWindowPos(NULL, 1, 35, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_SHOWWINDOW);
		// 불량영상 TAB이 선택될 경우 CONTROL의 위치와 크기를 지정함
	}

	LeaveCriticalSection(&m_csSync);
}

long CDlgExecAuto::GetSelectedTab(long nSel)
{
	long i, nLngSel;
    HICON hIcon[5];
	
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	nSel = m_tabCAM.GetCurSel();
	if (nSel<0) nSel=0;

	// 현재 선택된 탭만 LED_ON ICON표시
	for(i=0; i<4; i++)
		hIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
	hIcon[nSel] = AfxGetApp()->LoadIcon(IDI_ICON_LED_ON);

	m_imgListSel.DeleteImageList();
	m_imgListSel.Create(16, 16, TRUE, 3, 3);

	for(i=0; i<4; i++)
		m_imgListSel.Add(hIcon[i]);
	// 현재 선택된 탭만 LED_ON ICON표시 

	// 탭의 갯수를 추가하고 명칭과 ICON를 표시함
	m_tabCAM.DeleteAllItems();
	m_tabCAM.SetImageList(&m_imgListSel);
	m_tabCAM.SetMinTabWidth(80);

	TC_ITEM item;
    item.mask = TCIF_IMAGE|TCIF_TEXT;

	if (CAM_MAX_NUM==1)
	{
		item.iImage = 0;
		if (nLngSel==0) item.pszText = _T(" 상부 ");
		else if (nLngSel==1) item.pszText = _T(" UP ");
		else item.pszText = _T(" 上部 ");
		m_tabCAM.InsertItem(0, &item);

		item.iImage = 1;
		if (nLngSel==0) item.pszText = _T(" 불량영상 ");
		else if (nLngSel==1) item.pszText = _T(" NG IMAGE ");
		else item.pszText = _T(" 不良图像 ");
		m_tabCAM.InsertItem(1, &item);
	}

	if (CAM_MAX_NUM==2)
	{
		item.iImage = 0;
		if (nLngSel==0) item.pszText = _T(" 상부 ");
		else if (nLngSel==1) item.pszText = _T(" UP ");
		else item.pszText = _T(" 上部 ");
		m_tabCAM.InsertItem(0, &item);

		item.iImage = 1;
		if (nLngSel==0) item.pszText = _T(" 하부 ");
		else if (nLngSel==1) item.pszText = _T(" DN ");
		else item.pszText = _T(" 下部 ");
		m_tabCAM.InsertItem(1, &item);

		item.iImage = 2;
		if (nLngSel==0) item.pszText = _T(" 상/하부 ");
		else if (nLngSel==1) item.pszText = _T(" UP/DN ");
		else item.pszText = _T(" 上/下部 ");
		m_tabCAM.InsertItem(2, &item);

		item.iImage = 3;
		if (nLngSel==0) item.pszText = _T(" 불량영상 ");
		else if (nLngSel==1) item.pszText = _T(" NG IMAGE ");
		else item.pszText = _T(" 不良图像 ");
		m_tabCAM.InsertItem(3, &item);
	}
	// 탭의 갯수를 추가하고 명칭과 ICON를 표시함

	// USER가 선택한 TAB버튼을 눌리게 한다. 
	m_tabCAM.SetCurSel(nSel);
	// USER가 선택한 TAB버튼을 눌리게 한다. 

	return nSel;
}

void CDlgExecAuto::OnSelchangeTabCam(NMHDR *pNMHDR, LRESULT *pResult)
{
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	GetSelectedSubTab();
	*pResult = 0;
}	

void CDlgExecAuto::GetSelectedSubTab()
{
	long nLngSel, nSel, nCamSel; 
	long i; 

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	nSel = m_tabCAM.GetCurSel();
	if (nSel<0) nSel=0;

	// 실시간 검사중이 아닐 경우 S/W 트리거 모드로 전환 
	if (!m_pMainFrm->m_bExecRealTime)
	{
		m_pMainFrm->m_CAM.SetActivateTrigger(m_pMainFrm->m_nSelectCAM, FALSE);
		Sleep(30);
		m_pMainFrm->m_CAM.SetTriggerMode(nSel, 1);
		Sleep(1);
	}
	// 실시간 검사중이 아닐 경우 S/W 트리거 모드로 전환 

	// 탭 버튼 변경 0:상부, 1:하부, 2:상/하부, 3:불량영상 화면으로 전환
	m_pMainFrm->m_nSelectCAM = nSel;
	GetSelectedTab(nSel);

	ShowVisionWindow(nSel, TRUE);
	// 탭 버튼 변경 0:상부, 1:하부, 2:상/하부, 3:불량영상 화면으로 전환

	// 불량영상 탭 선택시 명칭 표시 
	if (nSel==3 || (CAM_MAX_NUM==1 && nSel==CAM_MAX_NUM))
	{
		m_tabImage.DeleteAllItems();
		m_tabImage.SetMinTabWidth(105);

		TC_ITEM item;
		item.mask = TCIF_IMAGE|TCIF_TEXT;

		for(i=0; i<DEF_QUEUE_MAX_NUM; i++)
		{
			item.iImage = i;
			if (i==0) item.pszText = _T("  1 ");
			if (i==1) item.pszText = _T("  2 ");
			if (i==2) item.pszText = _T("  3 ");
			if (i==3) item.pszText = _T("  4 ");
			if (i==4) item.pszText = _T("  5 ");
			if (i==5) item.pszText = _T("  6 ");
			if (i==6) item.pszText = _T("  7 ");
			if (i==7) item.pszText = _T("  8 ");
			if (i==8) item.pszText = _T("  9 ");
			if (i==9) item.pszText = _T(" 10 ");
#if 0
			if (i==10) item.pszText = _T(" 11 ");
			if (i==11) item.pszText = _T(" 12 ");
			if (i==12) item.pszText = _T(" 13 ");
			if (i==13) item.pszText = _T(" 14 ");
			if (i==14) item.pszText = _T(" 15 ");  
#endif // 0

			m_tabImage.InsertItem(i, &item);
		}

		m_tabImgCAM.DeleteAllItems();
		m_tabImgCAM.SetMinTabWidth(80);

		if (nLngSel==0) item.pszText = _T(" 상부 ");
		else if (nLngSel==1) item.pszText = _T(" UP ");
		else item.pszText = _T(" 上部 ");
		m_tabImgCAM.InsertItem(0, &item);

		if (CAM_MAX_NUM == VIEW_MAX_NUM)
		{
			if (nLngSel==0) item.pszText = _T(" 하부 ");
			else if (nLngSel==1) item.pszText = _T(" DN ");
			else item.pszText = _T(" 下部 ");
			m_tabImgCAM.InsertItem(1, &item);
		}

		// 현재 선택한 불량영상 탭에 대한 불량 영상 출력
		nCamSel = m_tabImgCAM.GetCurSel();
	    nSel = m_tabImage.GetCurSel();
    	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSel);
		// 현재 선택한 불량영상 탭에 대한 불량 영상 출력
	}
	// 불량영상 탭 선택시 명칭 표시 
}	

void CDlgExecAuto::OnSelchangeTabImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	long nCamSel, nSelPnt;

	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	// 불량영상 탭을 선택시 선택한 불량 영상을 출력함(최대 15개)
	nCamSel = m_tabImgCAM.GetCurSel();
	nSelPnt = m_tabImage.GetCurSel();

//	SetEvent(m_hEventExport);
	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);
	// 불량영상 탭을 선택시 선택한 불량 영상을 출력함(최대 15개)

	*pResult = 0;
}

void CDlgExecAuto::OnSelchangeTabImageCam(NMHDR *pNMHDR, LRESULT *pResult)
{
	long nCamSel, nSelPnt;

	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);
	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) return;

	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함
	nCamSel = m_tabImgCAM.GetCurSel();
	nSelPnt = m_tabImage.GetCurSel();

//	SetEvent(m_hEventExport);
	m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);
	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함

	*pResult = 0;
}

void CDlgExecAuto::InitListControls()
{
	long i, j, nLngSel, nRow;
	CString str, strT, strUpDn;
	INSP_RESULT_DATA  *pRes;

	// 현재 선택 언어를 읽어오고 LIST_MAIN_ID 리스트를 RESET CLEAR함
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	NUiListDll_ClearCellAll(LIST_MAIN_ID);
	// 현재 선택 언어를 읽어오고 LIST_MAIN_ID 리스트를 RESET CLEAR함

	// LIST_MAIN_ID 리스트의 구조 초기화 TITLE 표시
#if _DUO_CELL
	NUiListDll_SetRows(LIST_MAIN_ID, 50);
	NUiListDll_SetRowHeight(LIST_MAIN_ID, 18);
	NUiListDll_SetFontSize(LIST_MAIN_ID, 14);
	NUiListDll_SetCols(LIST_MAIN_ID, 4);
	NUiListDll_SetEnableEdit(LIST_MAIN_ID, FALSE);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 0, 115);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 1, 105);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 2, 55);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 3, 55);
	NUiListDll_SetTitleColor(LIST_MAIN_ID, RGB(200, 200, 200));
	NUiListDll_SetBackColor(LIST_MAIN_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_MAIN_ID, RGB(200, 200, 200));

	if (nLngSel==0)
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(상/하부)명칭"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("범위.(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" 전 "));
		NUiListDll_SetText(LIST_MAIN_ID, 3, 0, _T(" 후 "));
	}
	else if (nLngSel==1)
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(UP/DN) TITLE"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("Spec.(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" FRONT "));
		NUiListDll_SetText(LIST_MAIN_ID, 3, 0, _T(" REAR "));
	}
	else 
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(上/下部)名称"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("规格(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" 前面 "));
		NUiListDll_SetText(LIST_MAIN_ID, 3, 0, _T(" 后面 "));
	}
#else
	NUiListDll_SetRows(LIST_MAIN_ID, 30);
	NUiListDll_SetRowHeight(0, 25);
	NUiListDll_SetFontSize(LIST_MAIN_ID, 17);
	NUiListDll_SetCols(LIST_MAIN_ID, 3);
	NUiListDll_SetEnableEdit(LIST_MAIN_ID, FALSE);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 0, 130);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 1, 100);
	NUiListDll_SetColWidth(LIST_MAIN_ID, 2, 100);
	NUiListDll_SetTitleColor(LIST_MAIN_ID, RGB(200, 200, 200));
	NUiListDll_SetBackColor(LIST_MAIN_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_MAIN_ID, RGB(200, 200, 200));

	if (nLngSel==0)
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(상/하부)명칭"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("스펙.(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" 측정치 "));
	}
	else if (nLngSel==1)
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(UP/DN) TITLE"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("Spec.(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" MEASUREMENT "));
	}
	else
	{
		NUiListDll_SetText(LIST_MAIN_ID, 0, 0, _T("(上/下部)名称"));
		NUiListDll_SetText(LIST_MAIN_ID, 1, 0, _T("规格(L~H)"));
		NUiListDll_SetText(LIST_MAIN_ID, 2, 0, _T(" 测定 "));
	}
#endif
	// LIST_MAIN_ID 리스트의 구조 초기화 TITLE 표시

	nRow = 0;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 검사결과를 구조체로 읽어와 변수에 저장함 
    	pRes = (INSP_RESULT_DATA *)NAppDll_GetResultData(i);
		// 검사결과를 구조체로 읽어와 변수에 저장함 

		if (i==0)
		{
			if (nLngSel==0) strUpDn = _T("(상부)");
			else if (nLngSel==1) strUpDn = _T("(UP)");
			else strUpDn = _T("(上部)");
		}
		else
		{
			if (nLngSel==0) strUpDn = _T("(하부)");
			else if (nLngSel==1) strUpDn = _T("(DN)");
			else strUpDn = _T("(下部)");
		}

		if (!m_pMainFrm->m_Setting.m_bExecInsp[i]) continue;
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			// 명칭이 없거나, 보조 검사항목(한방향, Datum_B, C)은 Skip함(REAR 검사항목은 SKIP함) 
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE ||
//				m_pRes[i]->m_nInspType[j]==INSP_SLINE ||
				pRes->m_nInspType[j]==INSP_DATUM_C ||
				pRes->m_nInspType[j]==INSP_DATUM_B) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_bLocROI[j]) continue;
			// 명칭이 없거나, 보조 검사항목(한방향, Datum_B, C)은 Skip함(REAR 검사항목은 SKIP함) 

			nRow++;

			strT.Format(_T("%02d_%s"), nRow,pRes->m_strTitle[j]);
			if (strT.GetLength() > 10) 
				strT = strT.Left(10);

			str = strUpDn + strT;
			NUiListDll_SetText(LIST_MAIN_ID, 0, nRow, str);

			// 스펙, 양품색상 표시 
	    	str.Format(_T("%5.2f~%5.2f"), pRes->m_dLowLimit[j], pRes->m_dHighLimit[j]);
            NUiListDll_SetText(LIST_MAIN_ID, 1, nRow, str);
			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(170, 255, 170));
			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(200, 200, 255));		
			// 스펙, 양품색상 표시 
		}

		// 두개의 검사항목이 한개로 연결된 검사항목 처리   
		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			// 검사 명칭이 없거나 REAR검사항목은 SKIP함 
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (pRes->m_bLinkLocROI[j]) continue;
			// 검사 명칭이 없거나 REAR검사항목은 SKIP함 

			nRow++;

			strT.Format(_T("%02d_%s"), nRow, pRes->m_strLinkTitle[j]);
			if (strT.GetLength() > 10) 
				strT = strT.Left(10);

			str = strUpDn + strT;
			NUiListDll_SetText(LIST_MAIN_ID, 0, nRow, str);

			// 스펙, 양품색상 표시 
	    	str.Format(_T("%5.2f~%5.2f"), pRes->m_dLinkLowLimit[j], pRes->m_dLinkHighLimit[j]);
            NUiListDll_SetText(LIST_MAIN_ID, 1, nRow, str);
			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(170, 255, 170));
			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(200, 200, 255));
			// 스펙, 양품색상 표시 
		}
	}
	
	// 최종 ROW의 갯수를 설정한다.
	NUiListDll_SetRow(LIST_MAIN_ID, nRow);
	// 최종 ROW의 갯수를 설정한다.
	
	// LIST_MAIN_ID 리스트를 Invalidate한다.
	NUiListDll_Invalidate(LIST_MAIN_ID);
	// LIST_MAIN_ID 리스트를 Invalidate한다.

}

void CDlgExecAuto::InitCountData()
{
	long nLngSel;

	// 현재 선택 언어 
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	// LIST_COUNT_ID 리스트의 구조 초기화 TITLE 표시
	NUiListDll_ClearCellAll(LIST_INFO_ID);
	NUiListDll_SetRows(LIST_INFO_ID, 6);
	NUiListDll_SetRow(LIST_INFO_ID, 6);
	NUiListDll_SetRowHeight(LIST_INFO_ID, 24);
	NUiListDll_SetFontSize(LIST_INFO_ID, 18);
	NUiListDll_SetCols(LIST_INFO_ID, 2);
	NUiListDll_SetEnableTitle(LIST_INFO_ID, FALSE);
	NUiListDll_SetColWidth(LIST_INFO_ID, 0, 140);
	NUiListDll_SetColWidth(LIST_INFO_ID, 1, 300);
	NUiListDll_SetBackColor(LIST_INFO_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_INFO_ID, RGB(200, 200, 200));

	NUiListDll_SetText(LIST_INFO_ID, 0, 0, _T("MODEL NAME"));
	NUiListDll_SetText(LIST_INFO_ID, 0, 1, _T("LOT ID"));
	NUiListDll_SetText(LIST_INFO_ID, 0, 2, _T("INSP. CNT."));
	NUiListDll_SetText(LIST_INFO_ID, 0, 3, _T("NG CNT TOTAL"));
	NUiListDll_SetText(LIST_INFO_ID, 0, 4, _T("NG CNT(UP/DN)"));
	NUiListDll_SetText(LIST_INFO_ID, 0, 5, _T("YIELD"));
	NUiListDll_Invalidate(LIST_INFO_ID);
	// LIST_COUNT_ID 리스트의 구조 초기화 TITLE 표시

	UpdateCountData();
}

void CDlgExecAuto::UpdateCountData()
{
	CString str;
	long nGoodCnt;
	float dTemp=0.0f;

	str.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	NUiListDll_SetText(LIST_INFO_ID, 1, 0, str);

	str.Format(_T("%s"), m_pMainFrm->m_strLotID);
	NUiListDll_SetText(LIST_INFO_ID, 1, 1, str);

	if (m_pMainFrm->m_Etc.m_nTotalInspCount>=0)
	{
		nGoodCnt = m_pMainFrm->m_Etc.m_nTotalInspCount - m_pMainFrm->m_Etc.m_nTotalBadCount;
		if (m_pMainFrm->m_Etc.m_nTotalInspCount>0)
	    	dTemp = (float)nGoodCnt/m_pMainFrm->m_Etc.m_nTotalInspCount;

		// 검사수량 표시
		str.Format(_T("%d"), m_pMainFrm->m_Etc.m_nTotalInspCount);
		NUiListDll_SetText(LIST_INFO_ID, 1, 2, str);

		// 양품 수량 표시
		//str.Format(_T("%d"), nGoodCnt);
		//NUiListDll_SetText(LIST_INFO_ID, 1, 2, str);

		// 불량 수량 표시
		str.Format(_T("%d"), m_pMainFrm->m_Etc.m_nTotalBadCount);
		NUiListDll_SetText(LIST_INFO_ID, 1, 3, str);
	
		str.Format(_T("%d (%2.2f) / %d (%2.2f)"), m_pMainFrm->m_Etc.m_nUpBadCount, 100.0*(float)m_pMainFrm->m_Etc.m_nUpBadCount/m_pMainFrm->m_Etc.m_nTotalInspCount, 
			m_pMainFrm->m_Etc.m_nDnBadCount, 100.0*(float)m_pMainFrm->m_Etc.m_nDnBadCount/m_pMainFrm->m_Etc.m_nTotalInspCount);
		NUiListDll_SetText(LIST_INFO_ID, 1, 4, str);
		
		// 수율 표시
		str.Format(_T("%3.2f"), dTemp*100.0f);
		NUiListDll_SetText(LIST_INFO_ID, 1, 5, str);
	}

	NUiListDll_Invalidate(LIST_INFO_ID);
}

void CDlgExecAuto::UpdateResultList()
{
	long i, j, nLngSel, nRow;
	long nIndex;
	CString str, strT, strUpDn;
	INSP_RESULT_DATA  *pRes;

	// 현재 선택 언어 
	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	// 현재 선택 언어

	// 카메라별, 검사항목로 검사 결과를 리스트에 표시함
	nIndex = 0;
	nRow = 0;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 검사결과를 구조체로 읽어와 변수에 저장함 
		pRes = m_pMainFrm->m_pRes[i];
		// 검사결과를 구조체로 읽어와 변수에 저장함 

		if (i==0)
		{
			if (nLngSel==0) strUpDn = _T("(상부)");
			else if (nLngSel==1) strUpDn = _T("(UP)");
			else strUpDn = _T("(上部)");
		}
		else
		{
			if (nLngSel==0) strUpDn = _T("(하부)");
			else if (nLngSel==1) strUpDn = _T("(DN)");
			else strUpDn = _T("(下部)");
		}

		if (!m_pMainFrm->m_Setting.m_bExecInsp[i]) continue;
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			// 명칭이 없거나, 보조 검사항목(한방향, Datum_B, C)은 Skip함(REAR 검사항목은 SKIP함) 
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE ||
//				m_pRes[i]->m_nInspType[j]==INSP_SLINE || 
				pRes->m_nInspType[j]==INSP_DATUM_C ||
				pRes->m_nInspType[j]==INSP_DATUM_B) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_bLocROI[j]) continue;
			// 명칭이 없거나, 보조 검사항목(한방향, Datum_B, C)은 Skip함(REAR 검사항목은 SKIP함) 

			nRow++;

			// 언어에 따라 검사명칭 표시 
			strT.Format(_T("%02d_%s"), nRow, pRes->m_strTitle[j]);
			if (strT.GetLength() > 7) 
				strT = strT.Left(7);

			str = strUpDn + strT;
			NUiListDll_SetText(LIST_MAIN_ID, 0, nRow, str);
			// 언어에 따라 검사명칭 표시 

			// 스펙, 측정치, 양/불 표시 
	    	str.Format(_T("%5.2f~%5.2f"), pRes->m_dLowLimit[j], pRes->m_dHighLimit[j]);
            NUiListDll_SetText(LIST_MAIN_ID, 1, nRow, str);

			str.Format(_T("%5.2f"), pRes->m_dValue[j]);
            NUiListDll_SetText(LIST_MAIN_ID, 2, nRow, str);

			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(170, 255, 170));
			if (pRes->m_bDecisionROI[j]) 
				NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(255, 170, 170));
			// 스펙, 측정치, 양/불 표시 

			// 검사항목에 일치하는 REAR CELL를 찾아서 측정치, 양/불 표시 
#if _DUO_CELL
			nIndex = m_pMainFrm->GetIndexTitleROI(pRes, (LPCTSTR)pRes->m_strTitle[j]);
			if (nIndex>=0)
			{
				str.Format(_T("%5.2f"), pRes->m_dValue[nIndex]);
				NUiListDll_SetText(LIST_MAIN_ID, 3, nRow, str);

				NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(200, 200, 255));
				if (pRes->m_bDecisionROI[nIndex]) 
					NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(255, 170, 170));
			}
#endif
			// 검사항목에 일치하는 REAR CELL를 찾아서 측정치, 양/불 표시 

		}

		// 두개의 검사항목이 한개로 연결된 검사항목 처리   
		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			// 검사 명칭이 없거나 REAR검사항목은 SKIP함 
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (pRes->m_bLinkLocROI[j]) continue;
			// 검사 명칭이 없거나 REAR검사항목은 SKIP함 

			nRow++;

			strT.Format(_T("%02d_%s"), nRow, pRes->m_strLinkTitle[j]);
			if (strT.GetLength() > 7) 
				strT = strT.Left(7);

			str = strUpDn + strT;
			NUiListDll_SetText(LIST_MAIN_ID, 0, nRow, str);
			// 언어에 따라 검사명칭 표시 

			// 스펙, 측정치, 양/불 표시
	    	str.Format(_T("%5.2f~%5.2f"), pRes->m_dLinkLowLimit[j], pRes->m_dLinkHighLimit[j]);
            NUiListDll_SetText(LIST_MAIN_ID, 1, nRow, str);

//			if (m_pRes[i]->m_nLinkType[j] == TEACH_SLINE_SURFACE)
//			{
//				str.Format(_T("%5.3f | %5.3f"), m_pRes[i]->m_dLinkValue[j], m_pRes[i]->m_dLinkValueS[j]);
//				NUiListDll_SetText(LIST_MAIN_ID, 2, nRow, str);
//			}
//			else
//			{
		    str.Format(_T("%5.2f"), pRes->m_dLinkValue[j]);
			NUiListDll_SetText(LIST_MAIN_ID, 2, nRow, str);
//			}
			
			NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(170, 255, 170));
			if (pRes->m_bLinkDecROI[j] && pRes->m_nLinkType) 
				NUiListDll_SetCellBackColor(LIST_MAIN_ID, 2, nRow, RGB(255, 170, 170));
			// 스펙, 측정치, 양/불 표시

			// 검사항목에 일치하는 REAR CELL를 찾아서 측정치, 양/불 표시 
#if _DUO_CELL
			nIndex = m_pMainFrm->GetIndexTitleLinkROI(pRes, (LPCTSTR)pRes->m_strLinkTitle[j]);
			if (nIndex>=0)
			{
/*				if (m_pRes[i]->m_nLinkType[j] == TEACH_SLINE_SURFACE)
				{
					str.Format(_T("%5.3f | %5.3f"), m_pRes[i]->m_dLinkValue[j], m_pRes[i]->m_dLinkValueS[j]);
					NUiListDll_SetText(LIST_MAIN_ID, 3, nRow, str);
				}
				else
				{*/
				str.Format(_T("%5.2f"), pRes->m_dLinkValue[nIndex]);
				NUiListDll_SetText(LIST_MAIN_ID, 3, nRow, str);
//				}

				NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(200, 200, 255));
				if (pRes->m_bLinkDecROI[nIndex]) 
					NUiListDll_SetCellBackColor(LIST_MAIN_ID, 3, nRow, RGB(255, 170, 170));
			}
#endif
			// 검사항목에 일치하는 REAR CELL를 찾아서 측정치, 양/불 표시 
		}
	}

	// 최종 ROW의 갯수를 설정한다.
	NUiListDll_SetRow(LIST_MAIN_ID, nRow);
	// 최종 ROW의 갯수를 설정한다.

	// LIST_MAIN_ID 리스트를 Invalidate한다.
	NUiListDll_Invalidate(LIST_MAIN_ID);
	// LIST_MAIN_ID 리스트를 Invalidate한다.
}

void CDlgExecAuto::UpdateModelName()
{
	CString str;

	str.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	NUiListDll_SetText(LIST_INFO_ID, 1, 0, str);

	str.Format(_T("%s"), m_pMainFrm->m_strLotID);
	NUiListDll_SetText(LIST_INFO_ID, 1, 1, str);

	NUiListDll_Invalidate(LIST_INFO_ID);
}

void CDlgExecAuto::InitRealTimeProc()
{
	long i;
	char szbuf[100];

	// MainFrm.cpp에서 UMAC통신, PLC통신 상태 모니터링을 실시간중에는 OFF함
	m_pMainFrm->KillTimer(1000);
	// MainFrm.cpp에서 UMAC통신, PLC통신 상태 모니터링을 실시간중에는 OFF함

	sprintf_s(szbuf, " Start RUN ... ");
	m_pMainFrm->WriteLogToFile(szbuf);

#if _MELSEC
	//m_ActEasyIF.SetDevice(PLC_OUT_EPC_DATA, (long)0);
#endif

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		// 실사간 검사 모드로 전환하고 결과 데이타를 초기화함
		NAppDll_RealTimeMode(i, TRUE);
		NAppDll_InitResultData(i);
		// 실사간 검사 모드로 전환하고 결과 데이타를 초기화함
	
		// S/W 트리거 모드를 H/W 트리거 모드로 전환  
		if (!m_pMainFrm->m_Setting.m_bSWGrab)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			Sleep(30);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 2);
		}
		else
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			Sleep(30);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		}
		// S/W 트리거 모드를 H/W 트리거 모드로 전환  
	}

	// 변수 초기화 
	// 실시간 운영 모드 "TRUE"
	m_pMainFrm->m_bGaugeRnR = FALSE;
	m_pMainFrm->m_bExecRealTime = TRUE;
	m_pMainFrm->m_fSendDataCUT = 0.0f;
	m_pMainFrm->m_fOldTEOffset = 0.0f;
	m_pMainFrm->m_fFrontShort = 0.0f;
	m_pMainFrm->m_fRearShort = 0.0f;
	m_pMainFrm->m_nDaqIndex = 0;
	// 변수 초기화 


	strcpy_s(g_LOG.m_sLogTitle1, "");
	strcpy_s(g_LOG.m_sLogTitle2, "");
	strcpy_s(g_LOG.m_sLogTitle3, "");
	strcpy_s(g_LOG.m_sLogTitle4, "");
	strcpy_s(g_LOG.m_sLogTitle5, "");
	strcpy_s(g_LOG.m_sLogTitle6, "");
	strcpy_s(g_LOG.m_sLogTitle7, "");
	strcpy_s(g_LOG.m_sLogTitle8, "");
	strcpy_s(g_LOG.m_sLogTitle9, "");
	strcpy_s(g_LOG.m_sLogTitle10, "");
	strcpy_s(g_LOG.m_sLogTitle11, "");
	strcpy_s(g_LOG.m_sLogTitle12, "");
	strcpy_s(g_LOG.m_sLogTitle13, "");
	strcpy_s(g_LOG.m_sLogTitle14, "");
	strcpy_s(g_LOG.m_sLogTitle15, "");
	strcpy_s(g_LOG.m_sLogTitle16, "");


	// DIO 상태 초기화 
	// READY PORT "ON"
    m_pMainFrm->NPio_BitWrite(PIO_OUT_READY_PORT, TRUE);
	m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_OK_PORT, FALSE);
	m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_NG_PORT, FALSE);
	m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_MISMATCH_PORT, FALSE);
    m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_OK_PORT, FALSE);
	m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_NG_PORT, FALSE);
	m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_MISMATCH_PORT, FALSE);
	// DIO 상태 초기화 

	// LED 상태 초기화 
	m_pMainFrm->SetCheckLED(0, FALSE);
	m_pMainFrm->SetCheckLED(1, FALSE);
	m_pMainFrm->SetCheckLED(2, FALSE);
	// LED 상태 초기화 

	// 검사 결과 리스트 초기화 
	InitListControls();
	// 검사 결과 리스트 초기화 

	m_pMainFrm->m_bGaugeRnR = FALSE;
	
	//m_pMainFrm->m_pView->m_pDlgExecAuto->SetTimer(1000, 300, NULL);
}

void CDlgExecAuto::CloseRealTimeProc()
{
	long i;
	char szbuf[100];

	// MainFrm.cpp에서 UMAC통신, PLC통신 상태를 주기적으로 모니터링
	//m_pMainFrm->SetTimer(1000, 3000, NULL);
	// MainFrm.cpp에서 UMAC통신, PLC통신 상태를 주기적으로 모니터링

	sprintf_s(szbuf, " Stop RUN ! ");
	m_pMainFrm->WriteLogToFile(szbuf);
	
	// READY PORT "OFF"
    m_pMainFrm->NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
	Sleep(500);
	// READY PORT "OFF"

	// 실시간 운영 모드 "FALSE"
	m_pMainFrm->m_bExecRealTime = FALSE;
	
	// H/W트리거 모드를 S/W트리거 모드로 변경  
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		NAppDll_RealTimeMode(i, FALSE);
		if (!m_pMainFrm->m_Setting.m_bSWGrab)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(i, FALSE);
			m_pMainFrm->m_CAM.SetTriggerMode(i, 1);
		}
	}
	// H/W트리거 모드를 S/W트리거 모드로 변경  
}

BOOL CDlgExecAuto::InspectProgressGrab(long lParam)
//LONG CDlgAuto::OnInspectProgressGrab(UINT wParam, LONG lParam)
{
	CString str, strT;
	BOOL bExecCAM[VIEW_MAX_NUM];
	char szbuf[600];
	long i, nSelExec;
	double dTime;
	UINT nCAM = lParam;
	LPBYTE pImage;
	BOOL bRet, bRetF, bRetR, bFDec, bRDec, bExec, bFTab, bRTab;
	BOOL bMisMatch, bRetEPC;
	float fCutOS, fOffEPC;

	GetLocalTime(&m_pMainFrm->m_sysNewTime);
	
	//if (!m_pMainFrm->m_bExecRealTime && !m_pMainFrm->m_bGaugeRnR)
	if (m_pMainFrm->m_bExecRealTime && !m_pMainFrm->m_bGaugeRnR)
	{
		bExecCAM[CAM_UP] = m_pMainFrm->m_Setting.m_bExecInsp[CAM_UP];
		//bExecCAM[CAM_DN] = m_pMainFrm->m_Setting.m_bExecInsp[CAM_DN];

		//  상/하부 개별 검사 실행
		m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

		NAppDll_ExecuteInspection(nCAM);
		m_pMainFrm->m_pRes[nCAM] = (INSP_RESULT_DATA *)NAppDll_GetResultData(nCAM);

		dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);

		if (nCAM==CAM_UP) sprintf_s(g_LOG.m_sLogTitle3, "INSP(CAM=%d T=%5.3f)", 1, dTime);
		if (nCAM==CAM_DN) sprintf_s(g_LOG.m_sLogTitle4, "INSP(CAM=%d T=%5.3f)", 2, dTime);
		//  상/하부 개별 검사 실행


		//  불량 영상 저장 및 저장 THREAD실행 
		pImage = (LPBYTE)NAppDll_GetImagePtr(nCAM);
		if (bExecCAM[CAM_UP] && nCAM==CAM_UP)
		{
			bRet = m_pMainFrm->CopyRealTimeImage(CAM_UP, pImage);     
			if (bRet) 
				SetEvent(m_hEventExport);
			m_pMainFrm->SetCheckLED(CAM_UP, TRUE);
		}


		if (bExecCAM[CAM_DN] && nCAM==CAM_DN)
		{
			bRet = m_pMainFrm->CopyRealTimeImage(CAM_DN, pImage);
			if (bRet) 
				SetEvent(m_hEventExport);

			m_pMainFrm->SetCheckLED(CAM_UP, FALSE);
			m_pMainFrm->SetCheckLED(CAM_DN, TRUE);
		}  


		//  불량 영상 저장 및 저장 THREAD실행 


		nSelExec = 0;
		if (bExecCAM[CAM_UP] && !bExecCAM[CAM_DN])
			nSelExec = 1;
		else if (!bExecCAM[CAM_UP] && bExecCAM[CAM_DN])
			nSelExec = 2;
		else if (bExecCAM[CAM_UP] && bExecCAM[CAM_DN])
			nSelExec = 3;

		bExec = FALSE; 
		if (m_pMainFrm->m_GrabCAM[CAM_UP] && m_pMainFrm->m_GrabCAM[CAM_DN] && nCAM==CAM_DN)
			bExec = TRUE;

		//bExec = TRUE;
		if (bExec)
		{
			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

			bFDec = FALSE;
			bRDec = FALSE;
			bFTab = FALSE;
			bRTab = FALSE;

			//  상부불량 카운트 수량 계산 및 상부 전체 검사항목 불량 결과 판정
			if (nSelExec==1 || nSelExec==3)
			{
				if (m_pMainFrm->m_pRes[CAM_UP]->m_bFDecision) 
				{
					m_pMainFrm->m_Etc.m_nUpBadCount++;
					bFDec = TRUE;				
				}

#if _DUO_CELL
				if (m_pMainFrm->m_pRes[CAM_UP]->m_bRDecision) 
				{
					m_pMainFrm->m_Etc.m_nUpBadCount++;
					bRDec = TRUE;
				}
#endif
			}
			//  상부불량 카운트 수량 계산 및 상부 전체 검사항목 불량 결과 판정
			
			//  하부불량 카운트 수량 계산 및 하부 전체 검사항목 불량 결과 판정

			if (nSelExec==2 || nSelExec==3)
			{
				if (m_pMainFrm->m_pRes[CAM_DN]->m_bFDecision)
				{
					m_pMainFrm->m_Etc.m_nDnBadCount++;
					bFDec = TRUE;
				}

#if _DUO_CELL
				if (m_pMainFrm->m_pRes[CAM_DN]->m_bRDecision) 
				{
					m_pMainFrm->m_Etc.m_nDnBadCount++;
					bRDec = TRUE;
				}
#endif
			}  

			//  하부불량 카운트 수량 계산 및 하부 전체 검사항목 불량 결과 판정

			//  Final Cutter의 위치(A1, A2, A3, A4)를 보정하기 위한 보정값 계산 후 UMAC으로 전송 
			//m_pMainFrm->ExecFinalCuttingOffset(nSelExec, &bRetCUT, &fCutOS, &bRetT2B, &fT2BOS, &bRetEPC, &fOffEPC);
			fOffEPC = 0;
			bRetEPC = 0;
			fCutOS = 0;
			bMisMatch = FALSE;

            m_pMainFrm->SendResult2UMAC(bFDec, bRDec, bFTab, bRTab, bMisMatch, fCutOS, bRetEPC, fOffEPC);
			//  Final Cutter의 위치(A1, A2, A3, A4)를 보정하기 위한 보정값 계산 후 UMAC으로 전송 

			bRetF = 1;
			bRetR = 1;

			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle8, "PROC1(T=%5.3f)", dTime);

			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);
			//  EPC 보정값 적용 PC->PLC로 전송 THREAD 실행 
			if (bRetEPC)
			{
			    m_pMainFrm->m_fSendDataEPC = fOffEPC;
//				SetEvent(m_hEventMelsec);
			}
			//  EPC 보정값 적용 PC->PLC로 전송 THREAD 실행 

			//  검사결과 데이타 PC->PLC로 전송 THREAD 실행 
			if (m_pMainFrm->m_Setting.m_bExecDataTrans)
			{
				m_pMainFrm->CopyRealTimeMelsec();
				SetEvent(m_hEventSendData);
			}
			//  검사결과 데이타 PC->PLC로 전송 THREAD 실행 
			//  UNWINDER정보를 읽어와 전역변수에 저장한다. 
			
			//  UNWINDER정보를 읽어와 전역변수에 저장한다.  
			//  검사결과 데이타 리스트 출력 및 양/불/수율 카운트 UPDATE(MAIN 화면)
			UpdateResultList();
			UpdateCountData();
			//  검사결과 데이타 리스트 출력 및 양/불/수율 카운트 UPDATE(MAIN 화면)


			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle9, "PROC2(T=%5.3f)", dTime);

			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);

			//  상/하부 검사결과 데이타 PLOTTING UPDATE
			for(i=0; i<PLOT_PAGE_MAX_NUM; i++)
			    m_pMainFrm->m_pView->m_pDlgPlot->UpdatePlotData(i);
			//  상/하부 검사결과 데이타 PLOTTING UPDATE

			//  검사결과 데이타 리스트 출력 및 검사항목별 불량수량, CP, CPK, 수율 등 UPDATE(RESULT 화면)
			//  최근 500개 데이타를 환형 QUEUE구조를 사용하여 CP, CPK를 계산함 
			if (m_pMainFrm->m_ProdData.m_nTotalCnt<PROD_MAXDATA_NUM)
				m_pMainFrm->m_ProdData.m_nTotalCnt++;

			m_pMainFrm->m_pView->m_pDlgResult->UpdateResultList();
			
			m_pMainFrm->m_ProdData.m_nIndexPnt++;
			if (m_pMainFrm->m_ProdData.m_nIndexPnt>=PROD_MAXDATA_NUM)
				m_pMainFrm->m_ProdData.m_nIndexPnt = 0;
			//  검사결과 데이타 리스트 출력 및 검사항목별 불량수량, CP, CPK, 수율 등 UPDATE(RESULT 화면)

			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle10, "PROC3(T=%5.3f)", dTime);
			
			//  검사결과 데이타를 TEXT파일로 년월일시분 단위로 저장
			m_pMainFrm->m_ctrlTimer.SetClockTimer(0);
//			m_pMainFrm->WriteStringToFile();           // 상위보고 쓰레드쪽으로 이동함
			//  검사결과 데이타를 TEXT파일로 년월일시분 단위로 저장
			m_pMainFrm->WriteStringToFile_Csv_Day();  // shsong 추가된 사항. // 상위보고 쓰레드쪽으로 이동함

			//  검사 프로세스 시간계산 및 로그 데이타 저장
			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
			sprintf_s(g_LOG.m_sLogTitle11, "PROC4(T=%5.3f)", dTime);

			GetLocalTime(&m_pMainFrm->m_logEndTime);
			dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_TOTAL_INSP);

			sprintf_s(szbuf, ",EllipsedTime, %7.3f,CNT=%d, %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", dTime, m_pMainFrm->m_Etc.m_nTotalInspCount, g_LOG.m_sLogTitle1, g_LOG.m_sLogTitle2, g_LOG.m_sLogTitle3, g_LOG.m_sLogTitle4, 
				                    g_LOG.m_sLogTitle5, g_LOG.m_sLogTitle7, g_LOG.m_sLogTitle8, g_LOG.m_sLogTitle9, g_LOG.m_sLogTitle10, g_LOG.m_sLogTitle11,
									g_LOG.m_sLogTitle12, g_LOG.m_sLogTitle13, g_LOG.m_sLogTitle14, g_LOG.m_sLogTitle15, g_LOG.m_sLogTitle16);
			m_pMainFrm->WriteLogToFile((char *)szbuf);
			//  검사 프로세스 시간계산 및 로그 데이타 저장

			//  각종 정보 메세지 표시 형태로 출력
			if (bFDec || bRDec)
			{
    			m_pMainFrm->SetCheckLED(2, TRUE);
			    str.Format(_T("U_UW_P:%1d,Up_B:%10s,D_UW_P:%1d,Dn_B:%10s,T:%4.3fS"), m_pMainFrm->m_nUpperPos, m_pMainFrm->m_strUnW_Up_Barcode, 
					m_pMainFrm->m_nDownPos, m_pMainFrm->m_strUnW_Dn_Barcode, dTime);
			}
			else
			{
				m_pMainFrm->SetCheckLED(2, FALSE);
			    str.Format(_T("U_UW_P:%1d,Up_B:%10s,D_UW_P:%1d,Dn_B:%10s,T:%4.3fS"), m_pMainFrm->m_nUpperPos, m_pMainFrm->m_strUnW_Up_Barcode, 
					m_pMainFrm->m_nDownPos, m_pMainFrm->m_strUnW_Dn_Barcode, dTime);
			}

			//str.Format(_T("Ellipsed Time:%5.3f Sec. "), dTime);
			m_pMainFrm->WriteTextMessage(str);
			//  각종 정보 메세지 표시 형태로 출력

			//  GRAB FLAG UPDATE      
			m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
			m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;
			//  GRAB FLAG UPDATE			
		}
		else
		{
			if (nCAM==CAM_DN)
			{
//				m_pMainFrm->m_pView->m_pDlgExecAuto->SendResult2UMAC(1, 1, 0, 0.0f, 0, 0.0f);
				sprintf_s(szbuf, "DG_CAM=%d  CAM1(X), CAM2(O) ", 2);
			    m_pMainFrm->WriteLogToFile(szbuf);
			}
		}
	}
	else
	{
		//  매뉴얼로 검사할 때 처리되는 부분으로 검사후 PLOTTING & 항목별 리스트 출력 
		m_pMainFrm->m_ctrlTimer.SetClockTimer(1);
	    	GetUnWinderInfo(); // shsong
		//  상/하부 검사결과 데이타 PLOTTING UPDATE
		m_pMainFrm->m_pRes[nCAM] = (INSP_RESULT_DATA *)NAppDll_ExecuteInspection(nCAM);
		for(i=0; i<PLOT_PAGE_MAX_NUM; i++)
			m_pMainFrm->m_pView->m_pDlgPlot->UpdatePlotData(i);
		//  상/하부 검사결과 데이타 PLOTTING UPDATE

		UpdateResultList();
		//  매뉴얼로 검사할 때 처리되는 부분으로 검사후 PLOTTING & 항목별 리스트 출력 

		//  GAUGE R&R 테스트시 사용하는 것으로 검사결과 LOT ID가 TEST로 저장됨  
		if (m_pMainFrm->m_bGaugeRnR)
            m_pMainFrm->WriteStringToFile();
	   //  GAUGE R&R 테스트시 사용하는 것으로 검사결과 LOT ID가 TEST로 저장됨  

	   //  시간계산 후 화면에 표시
		dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(1);

		//str.Format(_T("%5.3f"), dTime);
		str.Format(_T("Up_UW_P:%1d,Up_BC:%10s,Dn_UW_P:%1d,Dn_BC:%10s,E-T:%4.3fS"), m_pMainFrm->m_nUpperPos, m_pMainFrm->m_strUnW_Up_Barcode, 
					m_pMainFrm->m_nDownPos, m_pMainFrm->m_strUnW_Dn_Barcode, dTime);
		m_pMainFrm->WriteTextMessage(str);
		 //  시간계산 후 화면에 표시
	}

	m_pMainFrm->ExecMessageHandleAll();

	return 1;
}

UINT CDlgExecAuto::ThreadDataExport(LPVOID pParam)
{
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;
  	long nRet=0;
//	long nCamSel, nSelPnt;

	// 불량 영상이 있으면 EVENT가 발생하여 영상을 하드디스크로 저장함 
	// 메인 운영 처리 프로세스와 별개로 CPU 병렬처리가 가능함
	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventExport, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;
		pView->GetUnWinderInfo(); // shsong
        m_pMainFrm->SaveRealTimeData();
		
//		nCamSel = pView->m_tabImgCAM.GetCurSel();
//	    nSelPnt = pView->m_tabImage.GetCurSel();
//    	pView->m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);

		ResetEvent(pView->m_hEventExport);
	}
	// 불량 영상이 있으면 EVENT가 발생하여 영상을 하드디스크로 저장함 
	
	CloseHandle(pView->m_hEventExport);
	AfxEndThread(100);

	return 0;
}

UINT CDlgExecAuto::ThreadSendDataToPLC(LPVOID pParam)
{
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;
  	long nRet=0;

	// 검사 결과 데이타 전송 EVENT가 발생하면 PC->PLC로 보내줌
	// 메인 운영 처리 프로세스와 별개로 CPU 병렬처리가 가능함
	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventSendData, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

		if (m_pMainFrm->m_bOnData2PLC)
			Sleep(50);

		if (!m_pMainFrm->m_bOnData2PLC)
		{
			m_pMainFrm->m_bOnData2PLC = TRUE;
			m_pMainFrm->WriteStringToFile();
			//  검사결과 데이타를 TEXT파일로 년월일시분 단위로 저장

			//m_pMainFrm->WriteStringToFile_Csv_Day();  // shsong 추가된 사항.

	        m_pMainFrm->SendDataToPLC();
			m_pMainFrm->m_bOnData2PLC = FALSE;
		}

		ResetEvent(pView->m_hEventSendData);
	}
	// 검사 결과 데이타 전송 EVENT가 발생하면 PC->PLC로 보내줌
	
	CloseHandle(pView->m_hEventSendData);
	AfxEndThread(100);

	return 0;
}

UINT CDlgExecAuto::ThreadMelsecComm(LPVOID pParam)
{
	/*
	CDlgExecAuto *pView = (CDlgExecAuto *)pParam;
	char szbuf[125];
  	long nRet=0;

	// EPC 보정값 전송 EVENT가 발생하면 PC->PLC로 보정값을 보내줌
	// PC->PLC로 데이타 전송시 60ms소요 이후 1초를 Delay하고 다시 0으로 초기화
	while(TRUE)
	{
    	WaitForSingleObject(pView->m_hEventMelsec, INFINITE);
		if (!m_pMainFrm->m_bAppExec) break;

		nRet = pView->m_ActEasyIF.SetDevice(PLC_OUT_EPC_DATA, (long)(m_pMainFrm->m_fSendDataEPC*10000));
		if (nRet==0)
		{
//			m_pMainFrm->NPio_BitWrite(PIO_OUT_EPC_PORT, TRUE);
			Sleep(1000);
//			m_pMainFrm->NPio_BitWrite(PIO_OUT_EPC_PORT, FALSE);
			nRet = pView->m_ActEasyIF.SetDevice(PLC_OUT_EPC_DATA, (long)0);

			m_pMainFrm->m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);	
			sprintf_s(szbuf, "Align V=%3.2f", m_pMainFrm->m_fSendDataEPC);
		    m_pMainFrm->WriteLogToFile(szbuf);
		}

		ResetEvent(pView->m_hEventMelsec);
	}
	// EPC 보정값 전송 EVENT가 발생하면 PC->PLC로 보정값을 보내줌
	
	CloseHandle(pView->m_hEventMelsec);
	AfxEndThread(100);
	*/
	return 0;
}

// 제어기 최대 밝기는 500으로 고정
void CALLBACK NAppDll_CallBackEvent()
{
	long i;
	long *pLightV;

	pLightV = new long[4];

	switch(NAppDll_Data.m_bContNo)
	{
		// 비전설정의 조명이 변경되면 CALLBACK EVENT가 발생 REMOTE 조명제어를 위해 4CH씩 한번에 RS232C로 전송 
		// 최대 밝기값은 255LEVEL로 변환  
		case DEF_CONT_LIGHT :
			if (NAppDll_Data.m_nCamID==0)
			{
				m_pMainFrm->m_Setting.m_nExposureTime[CAM_UP] = NAppDll_Data.m_nExposureTime;
				m_pMainFrm->m_CAM.SetExposureTime(CAM_UP, m_pMainFrm->m_Setting.m_nExposureTime[CAM_UP]);

				for(i=0; i<4; i++) 
				{
					pLightV[i] = NAppDll_Data.m_LightV[i];
					m_pMainFrm->m_Setting.m_nLightV[i] = pLightV[i];
				}

				NComPort_SetLight(DEF_REMOTE_LIGHT_COMPORT, 1, pLightV);
			}
			else
			{
				m_pMainFrm->m_Setting.m_nExposureTime[CAM_DN] = NAppDll_Data.m_nExposureTime;
				m_pMainFrm->m_CAM.SetExposureTime(CAM_DN, m_pMainFrm->m_Setting.m_nExposureTime[CAM_DN]);

				for(i=4; i<8; i++) 
				{
					pLightV[i-4] = NAppDll_Data.m_LightV[i];
					m_pMainFrm->m_Setting.m_nLightV[i] = pLightV[i-4];
				}

				NComPort_SetLight(DEF_REMOTE_LIGHT_COMPORT, 5, pLightV);
			}
		    break;

		// 비전설정의 조명이 변경되면 CALLBACK EVENT가 발생 REMOTE 조명제어를 위해 4CH씩 한번에 RS232C로 전송 
		// 비전설정 화면 종료시 CALLBACK EVENT가 발생하여 실행함
		// 검사항목 리스트를 초기화함
	    case DEF_CONT_EXIT_SETUP :
			m_pMainFrm->m_pView->m_pDlgExecAuto->InitListControls();
			break;
		// 비전설정 화면 종료시 CALLBACK EVENT가 발생하여 실행함
	}	

	delete pLightV;
}

void CDlgExecAuto::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
//		InitListControls();
//		UpdateResultList();
	}
}

static bool m_bToggleFlag2 = false;
void CDlgExecAuto::OnTimer(UINT_PTR nIDEvent)
{
#if 1  // shsong 인터럽에서 직접 받는걸로 체인지 단지, 오토일때와 아닐때를 구분한다. (오토일때는 LOT만 받아들이고, 실제 모델을 불러오질 않는다.)
	if (nIDEvent==1000)
	{
		// PLC에서 DIO 0번(첫번째)로 레시피 변경 신호가 입력되면 실행함
		KillTimer(1000);
		GetRecipeUpdateProc();
		// PLC에서 DIO 0번(첫번째)로 레시피 변경 신호가 입력되면 실행함
	}  
#endif // 0  // shsong 인터럽에서 직접 받는걸로 체인지 단지, 오토일때와 아닐때를 구분한다. (오토일때는 LOT만 받아들이고, 실제 모델을 불러오질 않는다.)


	// 타이머가 안될시 직접 스레드에서 호출하고 이부분은 주석 처리
	if (nIDEvent==2000)
	{
        // PLC에서 DIO 4번(다섯번째) 로 쇼트 신호가 입력되면 실행함
		KillTimer(2000);
		//SetShortTestCondition();	//쇼트 데이터 처리 함수
        // PLC에서 DIO 4번(다섯번째) 로 쇼트 신호가 입력되면 실행함
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgExecAuto::GetRecipeUpdateProc()
{
	long i, nRet, nModelType, nRecipiNum;
	CString str, strType, strT, strModelName;
	short nShort, STR[10];

	if (m_pMainFrm->m_bOnData2PLC)
		Sleep(100);

	nRet = 999;

	//AfxMessageBox(_T("Recieved Recipe New Num"));
	if (!m_pMainFrm->m_bOnData2PLC)
	{
		m_pMainFrm->m_bOnData2PLC = TRUE;

#if _MELSEC
		// PLC에서 모델형태, 레시피 ID, 레시피 명을 입력 받는다.
		if(!m_pMainFrm->m_bExecRealTime)
		{
			nRet = m_ActEasyIF.GetDevice(PLC_IN_PROD_MODEL, &nModelType);
			if (nRet>=0)
			{
				m_pMainFrm->m_strModelType = "";
				if (nModelType==1) m_pMainFrm->m_strModelType = _T("A");
				if (nModelType==2) m_pMainFrm->m_strModelType = _T("C");
				if (nModelType==3) m_pMainFrm->m_strModelType = _T("L");
				if (nModelType==4) m_pMainFrm->m_strModelType = _T("R");

				nRet = m_ActEasyIF.GetDevice(PLC_IN_RECIPI_NUM, &nRecipiNum);
				if (nRet>=0)
				{				
					if (m_pMainFrm->m_Setting.m_bRecipeOption)
					{
						m_pMainFrm->m_nRecipeNum = nRecipiNum;
						strModelName = m_pMainFrm->GetModelName(nRecipiNum);
						if (strModelName != _T(""))
						{						
							_tcscpy_s(m_pMainFrm->m_Setting.m_strModelName, strModelName);						
							m_pMainFrm->LoadModelData();
							UpdateModelName();							
						}
					}
				}
			}
		}
				// 한 번에 문자 10개를 입력 받음 
		m_ActEasyIF.ReadDeviceBlock2(PLC_IN_RECIPI_NAME, 10, STR);

		strT = _T("");
		for(i=0; i<10; i++)
		{
			str = char(STR[i]);
			strT += str;

			nShort = ((STR[i]>>8 & 0x00ff));
			str = char(nShort);
			strT += str;
		}

		strT = WithoutSpaceToString(strT);
		if (strT==_T(""))
			m_pMainFrm->m_strLotID = _T("NONE");
		else
			m_pMainFrm->m_strLotID = strT;

		UpdateModelName();
		// 한 번에 문자 10개를 입력 받음 

		str.Format(_T(" Model Type : %s , Recipi. NUM. : %d"), m_pMainFrm->m_strModelType, nRecipiNum);
		m_pMainFrm->WriteTextMessage(str);
//			m_pMainFrm->WriteLogToFile(szbuf);

        m_nCntSendDIO = 0;
		m_nCntReceiveDIO = 0;
		m_nErrResponseCnt = 0;
		
#if 0
						else
						{
							m_pMainFrm->m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
							m_pMainFrm->WriteTextMessage(DEF_MSG_PLC_ERROR);
						}  
#endif // 0

		// PLC에서 모델형태, 레시피 ID, 레시피 명을 입력 받는다.
#endif

#if _OMRON
		char szbuf[128];

		// PLC에서 모델형태, 레시피 ID, 레시피 명을 입력 받는다.
		nRet = (long)m_pMainFrm->m_OmronPlc.ReceiveMessageFromPLC_Int(OMRON_IN_RECIPI_NUM, 0, szbuf);
		if (nRet>0)
		{
			nRecipiNum = nRet;
			if (m_pMainFrm->m_Setting.m_bRecipeOption)
			{
				m_pMainFrm->m_nRecipeNum = nRecipiNum;
				strModelName = m_pMainFrm->GetModelName(nRecipiNum);
				if (strModelName != _T(""))
				{
					_tcscpy_s(m_pMainFrm->m_Setting.m_strModelName, strModelName);
					m_pMainFrm->LoadModelData();
					UpdateModelName();
				}
			}

			// 한 번에 문자 10개를 입력 받음 
			nRet = (long)m_pMainFrm->m_OmronPlc.ReceiveMessageFromPLC(OMRON_IN_LOT_ID, 0, szbuf);
	        //strT = CharToString(szbuf);
			strT =szbuf;
			/*
			strT = _T("");
			for(i=0; i<20; i++)
			{
				str = char(szbuf[i]);
				strT += str;
			}
			*/

			//strT = WithoutSpaceToString(strT);
			if (strT==_T(""))
				m_pMainFrm->m_strLotID = _T("NONE");
			else
				m_pMainFrm->m_strLotID = strT;

			UpdateModelName();
			// 한 번에 문자 10개를 입력 받음 

			str.Format(_T(" Model Type : %s , Recipi. NUM. : %d"), m_pMainFrm->m_strModelType, nRecipiNum);
			m_pMainFrm->WriteTextMessage(str);
//			m_pMainFrm->WriteLogToFile(szbuf);

           	m_nCntSendDIO = 0;
			m_nCntReceiveDIO = 0;
			m_nErrResponseCnt = 0;
		}
		else
		{
			m_pMainFrm->m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
			m_pMainFrm->WriteTextMessage(DEF_MSG_PLC_ERROR);
		}
		// PLC에서 모델형태, 레시피 ID, 레시피 명을 입력 받는다.
#endif

		m_pMainFrm->m_bOnData2PLC = FALSE;
	}

	char szbuf[100];
	
	sprintf_s(szbuf, " Recipe change signal input ");
	m_pMainFrm->WriteLogToFile(szbuf);
}

static bool m_bToggleFlag=false;
void CDlgExecAuto::OnBnClickedButton1()
{
	/*
#if _OMRON
	int DATA[5] = {1234, 2345, 3456, 4567, 5678};
	m_pMainFrm->m_OmronPlc.SendMessageToPLC(OMRON_OUT_VISION, 0, 5, DATA);
#endif
	*/

//	m_pMainFrm->m_pView->m_pDlgTestShort->m_pDlgShortMain->GetRS232CData();
	/*
	// 상부 트리거 테스트
	NUmacDll_SetIntValue('M', UMAC_OUT_TRIGGER_CAM1, 1);
	Sleep(20);
	NUmacDll_SetIntValue('M', UMAC_OUT_TRIGGER_CAM1, 0);
	// 상부 트리거 테스트
		
	m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
	InspectProgressGrab(0);

	m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;
	InspectProgressGrab(1);
	*/

	//char szbuf[200];
	//NComPort_Read(1, szbuf);
}

long nMessageID=0;
void CDlgExecAuto::OnBnClickedButton2()
{
#if _OMRON
	char szbuf[256];
	CString strT, strTmp;
	
	//m_pMainFrm->m_ctrlTimer.SetClockTimer(0);
	//m_pMainFrm->ReceiveMessageFromPLC(OMRON_IN_LOT_ID, 0, (char *)szbuf);
	m_pMainFrm->m_OmronPlc.ReceiveMessageFromPLC(OMRON_IN_LOT_ID, 0, (char *)szbuf);
	//dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(0);
	//strTmp.Format(_T("TIME = %5.3f"), dTime);
	//AfxMessageBox(strTmp);
		
	//m_pMainFrm->ReceiveMessageFromPLC(OMRON_IN_COMM_PORT, nMessageID, (char *)szbuf);

	strT = szbuf; 
	m_pMainFrm->WriteTextMessage(strT);
#endif

	/*
	// 하부 트리거 테스트
	NUmacDll_SetIntValue('M', UMAC_OUT_TRIGGER_CAM2, 1);
	Sleep(20);
	NUmacDll_SetIntValue('M', UMAC_OUT_TRIGGER_CAM2, 0);
	// 하부 트리거 테스트
	*/
}

static double dPosLoc = 0.0;
static bool   bGaugeRnR = false;
void CDlgExecAuto::OnBnClickedBtnTriggerPos()
{
	if (m_pMainFrm->m_nSelectCAM<2)
	{
		NUmacDll_SetIntValue('P', UMAC_OUT_CAM_SEL, (short)m_pMainFrm->m_nSelectCAM);

		NUmacDll_SetDoubleValue('P', UMAC_OUT_VELLO, 30);
		NUmacDll_SetDoubleValue('P', UMAC_OUT_ACCEL, 50);
		NUmacDll_SetIntValue('P', UMAC_OUT_MOVE_TRIG, 2);

		dPosLoc = 0.0;
		bGaugeRnR = true;
		m_pMainFrm->m_bGaugeRnR = TRUE;
    }
}

void CDlgExecAuto::OnBnClickedBtnStopRnr()
{
	m_pMainFrm->m_bGaugeRnR = FALSE;
	bGaugeRnR = false;

	GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP_RNR)->EnableWindow(TRUE);
}

void CDlgExecAuto::OnBnClickedBtnStartRnr()
{
	long i;
	CString str;

	if (!bGaugeRnR)
	{
		AfxMessageBox(_T(" No trigger position ! "));
		GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(TRUE);
		return;
	}
	
	GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP_RNR)->EnableWindow(TRUE);

	i=0;
	while(i<4 && bGaugeRnR)
	{
		//////////////////////////////////////////////////////////
		if (dPosLoc==0) dPosLoc -= 5;
		else dPosLoc = -7;
	
		NUmacDll_SetDoubleValue('P', UMAC_OUT_MOVE_TARG, dPosLoc);
		NUmacDll_SetIntValue('P', UMAC_OUT_MOVE_TRIG, 1);

		str.Format(_T(" Target Pos=%5.2f Repeat=%d"), dPosLoc, i+1);
		m_pMainFrm->WriteTextMessage(str);

		Sleep(1000);
		m_pMainFrm->ExecMessageHandleAll();
		if (!bGaugeRnR) break;
		Sleep(1000);
		//////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////
		dPosLoc = 7;

		m_pMainFrm->m_bGaugeRnR = TRUE;
		NUmacDll_SetDoubleValue('P', UMAC_OUT_MOVE_TARG, dPosLoc);
		NUmacDll_SetIntValue('P', UMAC_OUT_MOVE_TRIG, 1);

		str.Format(_T(" Target Pos=%5.2f Repeat=%d"), dPosLoc, i+1);
		m_pMainFrm->WriteTextMessage(str);

		Sleep(1000);
		m_pMainFrm->ExecMessageHandleAll();
		if (!bGaugeRnR) break;
		Sleep(1000);	
		//////////////////////////////////////////////////////////

		i++;
	}

	GetDlgItem(IDC_BTN_TRIGGER_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP_RNR)->EnableWindow(FALSE);
}

void CDlgExecAuto::OnStnClickedStaticList()
{
}
void CDlgExecAuto::GetUnWinderInfo()
{
	long ret = 0;
	short nShort = 0;
	CString str = _T("");

	ret = m_ActEasyIF.ReadDeviceBlock2(PLC_IN_UNWINDER_UP_POS, UNW_INFO_END, m_nUnWinderInfo);

	m_pMainFrm->m_nUpperPos = m_nUnWinderInfo[UNW_UPPER_POS]; 
	m_pMainFrm->m_nDownPos = m_nUnWinderInfo[UNW_DOWN_POS]; 

	m_pMainFrm->m_strUnW_Up_Barcode = _T("");
	m_pMainFrm->m_strUnW_Dn_Barcode = _T("");
	
	for(int i=UNW_UPPER_BARCODE; i<UNW_UPPER_BARCODE+10; i++)
	{
		str = char(m_nUnWinderInfo[i]);
		m_pMainFrm->m_strUnW_Up_Barcode += str;

		nShort = ((m_nUnWinderInfo[i]>>8 & 0x00ff));
		str = char(nShort);
		m_pMainFrm->m_strUnW_Up_Barcode += str;
	}

	m_pMainFrm->m_strUnW_Up_Barcode = WithoutSpaceToString(m_pMainFrm->m_strUnW_Up_Barcode);
	if (m_pMainFrm->m_strUnW_Up_Barcode==_T(""))
		m_pMainFrm->m_strUnW_Up_Barcode = _T("NONE");

	str = _T("");
	nShort = 0;

	for(int i=UNW_DOWN_BARCODE; i<UNW_DOWN_BARCODE+10; i++)
	{
		str = char(m_nUnWinderInfo[i]);
		m_pMainFrm->m_strUnW_Dn_Barcode += str;

		nShort = ((m_nUnWinderInfo[i]>>8 & 0x00ff));
		str = char(nShort);
		m_pMainFrm->m_strUnW_Dn_Barcode += str;
	}

	m_pMainFrm->m_strUnW_Dn_Barcode = WithoutSpaceToString(m_pMainFrm->m_strUnW_Dn_Barcode);
	if (m_pMainFrm->m_strUnW_Dn_Barcode==_T(""))
		m_pMainFrm->m_strUnW_Dn_Barcode = _T("NONE");

}
