

// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "NPioControlDll.h"
#include "NAppDll.h"
#include "AXD.h"
#include "AXL.h"
#include "mmsystem.h"
#include "math.h"
#include "NVisionDll.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void    __stdcall EventFuncCallProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2); 
void    __stdcall OnDIOInterruptCallback (long lActiveNo, DWORD uFlag);

CMainFrame  *m_pMainFrm;

#define TIMER_DEL_DATA     3
#define INDEX_MODEL_PAIN   0 
#define INDEX_STATUS_PAIN  1
#define INDEX_MESSAGE_PAIN 2 
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//static UINT indicators[] =
//{
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL
//};

// CMainFrame 생성/소멸
CMainFrame::CMainFrame()
{
	CNLaminateAppApp* pApp = (CNLaminateAppApp *)AfxGetApp();
	m_hSplash = pApp->GetSplashHwnd();

	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_LOADING_FILE, 0);
	m_pMainFrm = this;
	
	long i, j, k;

	// 각종 변수 초기화 
	m_strFilePath = "";
	m_strFileType = "";
	m_nSelectCAM = 0;
	m_nZoomScale = 1;
	m_bAppExec = TRUE;
	m_bGaugeRnR = FALSE;
	m_bExecRealTime = FALSE; 
#if SHORTAGE_USE
	m_bShortTest = FALSE;  
#endif // SHORTAGE_USE

	m_bCOMM = FALSE;
	m_bCriticalF = FALSE;
	m_bOnData2PLC = FALSE;
	m_nDaqIndex = 0;
	m_nCountDataEPC = 0;
	m_fSumDataEPC = 0.0f;
	m_bStatusCOM[0] = FALSE;
	m_bStatusCOM[1] = FALSE;

	m_fZoomScale = DEF_INIT_ZOOM_SCALE;
	m_ptMouse = CPoint(0, 0);
	m_ptOldMouse = CPoint(0, 0);
	m_fmRect = CRect(0, 0, 0, 0);
	// 각종 변수 초기화 

    // SETTING 구조체 변수 초기화 
	_tcscpy_s(m_Setting.m_strModelName, _T("DefaultModel"));
	_tcscpy_s(m_Setting.m_strPassWord, _T(""));
	_tcscpy_s(m_Setting.m_strSelDriver, _T("D:\\"));

	m_Setting.m_nDataPeriod	=	30;
	m_Setting.m_nSaveTypeImage 	=	0;
	m_Setting.m_nPortNo = 1;
	m_Setting.m_nCOM1 = 1;
	m_Setting.m_nCOM2 = 2;
    m_Setting.m_bExecCutOffset = FALSE;
	m_Setting.m_bExecEPCOffset = FALSE;
	m_Setting.m_bExecT2BOffset = FALSE;
	m_Setting.m_bExecDataTrans = FALSE;
	m_Setting.m_bSWGrab = FALSE;
//	m_Setting.m_bWayLefttoRight = FALSE;
	m_Setting.m_nSkipCnt = 5;
	m_Setting.m_nDaqUnitCnt = 20;
	m_Setting.m_nDelayTime=100;

	m_Setting.m_fTestVolt = 200.0f;
	m_Setting.m_fLimitCurr = 0.05f;
	m_Setting.m_fStartDelayTime = 0.01f;
	m_Setting.m_fRampingTime = 0.03f;
	m_Setting.m_fHoldingTime = 0.02f;
	m_Setting.m_fContactCurr = 0.0f;

	m_Setting.m_sysOldTime.wDay = 0;
	m_Setting.m_sysOldTime.wHour = 0;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		m_Setting.m_bExecInsp[i] = TRUE;
		m_Setting.m_bOKImageSave[i] =	FALSE;
		m_Setting.m_bNGImageSave[i] =	FALSE;
		m_Setting.m_bSaveImageType[i] = 0;
		m_Setting.m_nExposureTime[i] = DEF_EXPOSURE_TIME;
		m_GrabCAM[i] = FALSE;
	}

	for(i=0; i<DEF_LIGHT_CHANNEL_NUM; i++)
		m_Setting.m_nLightV[i] = 0;

	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		m_Setting.m_OrderNum[i] = 0;
		_tcscpy_s(m_Setting.m_ModelName[i], _T(""));
	}	
	// SETTING 구조체 변수 초기화 

	// ETC 구조체 변수 초기화 
	m_Etc.m_nTotalInspCount = 0;
	m_Etc.m_nTotalGoodCount = 0;
	m_Etc.m_nTotalBadCount = 0;
	m_Etc.m_nUpBadCount = 0;
	m_Etc.m_nDnBadCount = 0;
	m_Etc.m_nUpFBadCount = 0;	
	m_Etc.m_nUpFBadCount = 0;
	m_Etc.m_nDnRBadCount = 0;	
	m_Etc.m_nDnRBadCount = 0;
	m_Etc.m_nPriorityNum = 0;
	// ETC 구조체 변수 초기화 
	
	// SEARCH 구조체 변수 초기화 
	m_Search.m_bCam1 = FALSE;
	m_Search.m_bCam2 = FALSE;
	m_Search.m_bGood = FALSE;
	m_Search.m_bBad = FALSE;
	m_Search.m_bPeriod = FALSE;
	m_Search.m_bModel = FALSE;
	m_Search.m_bInspItem = FALSE;
    m_Search.m_tmFromYY = 2012;
	m_Search.m_tmFromMM = 1;
	m_Search.m_tmFromDD = 1;
	m_Search.m_tmFromTT = 1;
	m_Search.m_tmFromMT = 1;
	m_Search.m_tmToYY = 2012;
	m_Search.m_tmToMM = 1;
	m_Search.m_tmToDD = 1;
	m_Search.m_tmToTT = 1;
	m_Search.m_tmToMT = 1;
	_tcscpy_s(m_Search.m_strModelName, _T(""));
	// SEARCH 구조체 변수 초기화 

	// 실시간 생산관련 구조체 메모리 할당 및 초기화
	m_ProdData.m_nTotalCnt = 0;
	m_ProdData.m_nIndexPnt = 0;

	for(i=0; i<CAM_MAX_NUM; i++)
	for(j=0; j<GOM_MAX_NUM; j++)
	{
		m_Etc.m_CntROI[i][j] = 0;
		m_Etc.m_CntLink[i][j] = 0;
	
		m_ProdData.m_InspType[i][j] = 0;
		m_ProdData.m_bDec[i][j] = new BOOL[PROD_MAXDATA_NUM];
		m_ProdData.m_fValue[i][j] = new float[PROD_MAXDATA_NUM];
		m_ProdData.m_fValueS[i][j] = new float[PROD_MAXDATA_NUM];
		for(k=0; k<PROD_MAXDATA_NUM; k++)
		{
			m_ProdData.m_bDec[i][j][k] = 0;
		    m_ProdData.m_fValue[i][j][k] = 0;
			m_ProdData.m_fValueS[i][j][k] = 0;
		}
	}
	// 실시간 생산관련 구조체 메모리 할당 및 초기화

	// 영상, 로그, ㄸEXCEL, 모델 등 폴더 생성
	CString strPath = (CString)m_Setting.m_strSelDriver;
	CreateDirectoryFullPath(strPath+(CString)SAVE_IMAGE_PATH);
	CreateDirectoryFullPath((CString)SAVE_DATA_PATH);
	CreateDirectoryFullPath((CString)SAVE_DATA_PATH_CSV);
	CreateDirectoryFullPath((CString)SAVE_EXCEL_PATH);
	CreateDirectoryFullPath((CString)MODEL_FILE_PATH);
	// 영상, 로그, ㄸEXCEL, 모델 등 폴더 생성

	m_strUnW_Up_Barcode = _T("");
	m_strUnW_Dn_Barcode = _T("");
	m_nUpperPos = 0;
	m_nDownPos  = 0;

}

CMainFrame::~CMainFrame()
{
	long i, j;

	DeleteCriticalSection(&m_csSync);

	// 실시간 생산관련 구조체 메모리 해제 
	for(i=0; i<CAM_MAX_NUM; i++)
	for(j=0; j<GOM_MAX_NUM; j++)
	{
		delete m_ProdData.m_bDec[i][j];
	    delete m_ProdData.m_fValue[i][j];
		delete m_ProdData.m_fValueS[i][j];
	}	
	// 실시간 생산관련 구조체 메모리 해제 
}

void CMainFrame::ExitProgram()
{
	OnClose();
}

void CMainFrame::OnClose()
{
	int nRet;
	CString strT;

	m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
	m_CAM.SetTriggerMode(m_nSelectCAM, 1);

	if (m_bExecRealTime)
	{
		// 실시간 검사중에는 프로그램 종료가 않됨 
		m_pView->SelectDialog(DEF_BUTTON_AUTO);
		return;
		// 실시간 검사중에는 프로그램 종료가 않됨 
	}
	else
	{
		// 프로그램 종료시 메세지 화면 출력 
		if (m_Setting.m_nSelLanguage==0)
			strT = _T(" 프로그램을 종료하시겠습니까 ? ");
		else if (m_Setting.m_nSelLanguage==1)
			strT = _T(" Do you really want to exit program ? ");
		else 
			strT = _T(" 此按钮的功能为，终止系统 ? ");

		nRet = MessageBox(strT, _T(" Message Box "), MB_ICONQUESTION|MB_YESNO);
    	if (IDNO==nRet) 
		{
			m_pView->SelectDialog(DEF_BUTTON_AUTO);
//			m_pView->m_pDlgTestShort->SelectDialog(DEF_BUTTON_SHORT_MAIN);
			return;
		}
		// 프로그램 종료시 메세지 화면 출력
	}

	// 프로그램 종료 FLAG를 "FALSE"로 하고 종료 LOG를 저장함
	m_bAppExec = FALSE;
#if SHORTAGE_USE
	m_bShortTest = FALSE;  
#endif // SHORTAGE_USE


    char szbuf[100];
	sprintf_s(szbuf, " Terminate program ... ");
	WriteLogToFile(szbuf);
	// 프로그램 종료 FLAG를 "FALSE"로 하고 종료 LOG를 저장함

    // 영상획득 보드를 종료한다.
#if _BASLER_CAM
	long i;
	for(i=0; i<CAM_MAX_NUM; i++)
    	m_CAM.Close(i);
#endif
	// 영상획득 보드를 종료한다.

    //m_OmronPlc.ClosePLC();

	// 각종 구조체의 메모리를 해제한다. 
	CloseGlobalData();
	// 각종 구조체의 메모리를 해제한다. 

	// 시스템 설정 파라미터를 저장함
	SaveSysValueToFile();
	// 시스템 설정 파라미터를 저장함

	// 각종 DLL의 모듈을 종료한다.
	//	NUmacDll_Close();
	m_pView->m_pDlgExecAuto->CloseControls();
	//NComPort_Close(DEF_REMOTE_LIGHT_COMPORT);
	NComPort_Close(m_pMainFrm->m_Setting.m_nCOM1);
	NComPort_Close(m_pMainFrm->m_Setting.m_nCOM2);

	NPio_Close();
	NVisionDll_Close();
	// 각종 DLL의 모듈을 종료한다.
	
	CFrameWnd::OnClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	long i;
	long *pLightV;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// SPLASH 윈도우를 실행함 (ID_INITIAL_CONTROL)
	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_INITIAL_CONTROL, 0);
	// SPLASH 윈도우를 실행함 (ID_INITIAL_CONTROL)

	InitializeCriticalSection(&m_csSync);

	/*
	m_DlgMainBar.Create(this, IDD_DIALOG_MAINBAR, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER, 0);
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // Fehler beim Erzeugen
	}
	*/

	// 영상획득 보드 초기화
#if _BASLER_CAM
	m_CAM.FindCamera();
   	m_CAM.Open(0);
   	m_CAM.Open(1);
#endif
	// 영상획득 보드 초기화

	// 시스템 설정 파라미터를 로드함
	LoadSysValueFromFile();
	// 시스템 설정 파라미터를 로드함

	NVisionDll_Open(0, IMAGE_WIDTH, IMAGE_HEIGHT);

	// 각종 구조체의 메모리를 할당한다. 
	InitGlobalData();
	// 각종 구조체의 메모리를 할당한다. 

	// UMAC 초기화
#if _UMAC
	try
	{
		NUmacDll_Open(0);
	}
	catch (...)
	{
        WriteTextMessage(DEF_MSG_UMAC_ERROR);
	}
#endif
	// UMAC 초기화

#if _OMRON
	// Open PMac
	m_bOnConnect = m_OmronPlc.OpenPLC();
#endif

#if SHORTAGE_USE
	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화 
	NComPort_Open(m_pMainFrm->m_Setting.m_nCOM1, 19200, 8, 0, 1);
	NComPort_Open(m_pMainFrm->m_Setting.m_nCOM2, 19200, 8, 0, 1);
	// SHORT RS232C 포트 초기화 및 REMOTE LIGHT RS232C포트 초기화   
#endif // 

	
	// 조명 RS232C포트 초기화 & 초기 조명값 RS232C 통신 적용
	//NComPort_Open(DEF_REMOTE_LIGHT_COMPORT, 19200, 8, 0, 0);
	// 조명 RS232C포트 초기화 & 초기 조명값 RS232C 통신 적용

	NPio_Open(5, 19200, 8, 0, 0); // 실제 아진 I/O  보드만 오픈한다.
	NPio_Read();

	pLightV = new long[4];
	for(i=0; i<4; i++)
		pLightV[i] = m_Setting.m_nLightV[i];
	NComPort_SetLight(DEF_REMOTE_LIGHT_COMPORT, 1, pLightV);

	Sleep(100);

	for(i=0; i<4; i++)
		pLightV[i] = m_Setting.m_nLightV[i+4];
	NComPort_SetLight(DEF_REMOTE_LIGHT_COMPORT, 5, pLightV);
	delete pLightV;
	// 조명 RS232C포트 초기화 & 초기 조명값 RS232C 통신 적용

	// SPLASH 윈도우를 실행함 (UM_SPLASH_INIT)
 	::SendMessage(m_pMainFrm->m_hSplash, UM_SPLASH_INIT, ID_INITIAL_FINISH, 0);
	// SPLASH 윈도우를 실행함 (UM_SPLASH_INIT)

	// 비전 READY는 "FALSE"로 초기화
	NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
	// 비전 READY는 "FALSE"로 초기화

	// 현재 시간 설정
	GetLocalTime(&m_sysNewTime);
	GetLocalTime(&m_pMainFrm->m_logStartTime);
	// 현재 시간 설정

	// 프로그램 시작하는 로그를 저장함
    char szbuf[100];
	sprintf_s(szbuf, " Load program ... ");
	WriteLogToFile(szbuf);
	// 프로그램 시작하는 로그를 저장함
	
	// TIMER 1000:PLC, UMAC모니터링, 2000:0.5초에 한 번씩 비전 통신 TOGGLE신호를 보냄
	SetTimer(1000, 3000, NULL);
	SetTimer(2000, 500, NULL);

	// TIMER 1000:PLC, UMAC모니터링, 2000:0.5초에 한 번씩 비전 통신 TOGGLE신호를 보냄	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//  the CREATESTRUCT cs
	cs.x = 1280;
	cs.y = 0;
	cs.cx = APP_WIDTH;
	cs.cy = APP_HEIGHT;
//	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	cs.style = WS_VISIBLE|WS_POPUP|WS_BORDER;
	
	CMenu* pMenu = new CMenu;
	pMenu->Attach(cs.hMenu);
	pMenu->DestroyMenu();
	cs.hMenu = NULL;	
	delete pMenu;
	pMenu = NULL;

	// SPLASH 윈도우를 실행함 (ID_INITIAL_FINISH)
 	::SendMessage(m_hSplash, UM_SPLASH_INIT, ID_INITIAL_FINISH, 0);
	// SPLASH 윈도우를 실행함 (ID_INITIAL_FINISH)

	return TRUE;
}

// CMainFrame 진단
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////         STATUS BAR DISPLAY MODULE          /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetCheckLED(long nCAM, BOOL bOnOff)
{
	// 카메라 영상획득 상태 LED
	m_pView->m_pDlgSubBar->SetCheckLED(nCAM, bOnOff);
}

void CMainFrame::WriteTextMessage(CString str)
{
	// 상태 메세지 정보(str)를 표시
	m_pView->m_pDlgSubBar->WriteTextMessage(str);
}

void CMainFrame::WriteTextShortMessage(CString str)
{
	// 상태 메세지 정보(str)를 표시
#if SHORTAGE_USE
	m_pView->m_pDlgTestShort->WriteTextMessage(str);  
#endif // SHORTAGE_USE

}
// CMainFrame 메시지 처리기

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////         IMAGE INPUT/OUTPUT MODULE          /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::Grab()
{
	// GRAB 영상을 촬상함 S/W 트리거 사용
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
		Sleep(30);
		m_CAM.SetTriggerMode(m_nSelectCAM, 1);
		m_CAM.SetActivateTrigger(m_nSelectCAM, TRUE);
	}
}

// LIVE 영상을 촬상함 
void CMainFrame::Live() 
{
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		m_CAM.SetActivateTrigger(m_nSelectCAM, FALSE);
		Sleep(30);
		m_CAM.SetTriggerMode(m_nSelectCAM, 0);
	}
}

void CMainFrame::LoadImage(CString strFileName)
{
	LPBYTE pImage;
	char szbuf[256];
	CString str;

	// strFileName 의 영상을 로드함 ".JPG", ".BMP" TYPE가능함
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
	    str = strFileName.Right(4);
		if (str == ".jpg" || str == ".JPG")
		{
			pImage = (LPBYTE)NAppDll_GetImagePtr(m_nSelectCAM);
			StringToChar(strFileName, szbuf);
			NVisionDll_LoadImageFromFile(szbuf, pImage, IMAGE_WIDTH, IMAGE_HEIGHT, 1, 8);
		}
		else
			NAppDll_LoadImageData(m_nSelectCAM, strFileName);

		NAppDll_InvalidateView(m_nSelectCAM);
	}
	// strFileName 의 영상을 로드함 ".JPG", ".BMP" TYPE가능함
}

void CMainFrame::SaveImage(CString strFileName)
{
	LPBYTE pImage;
	char szbuf[256];

	//m_ctrlTimer.SetClockTimer(0);

	//for(int i=0; i<10; i++)
	//{
	// 화면에 출력된 영상을 외부장치로 저장함
	if (!m_bExecRealTime && m_nSelectCAM<CAM_MAX_NUM) 
	{
		pImage = (LPBYTE)NAppDll_GetImagePtr(m_nSelectCAM);
		StringToChar(strFileName, szbuf);

       	NVisionDll_SaveImageToFile(szbuf, pImage, IMAGE_WIDTH, IMAGE_HEIGHT, 1, 8);
		//NAppDll_InvalidateView(m_nSelectCAM);
	}
	// 화면에 출력된 영상을 외부장치로 저장함
	//}
	/*
	double dTime = m_ctrlTimer.GetClockTimer(0);
	CString str;
	str.Format(_T("%5.3f"), dTime);
	AfxMessageBox(str);
	*/
}
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////          IMAGE INPUT/OUTPUT MODULE         /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////        SYSTEM/MODEL DATA SAVE MODULE       /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadModelData()
{
	BOOL bFlag, bRet[CAM_MAX_NUM];
	CString filename;
	long i;
	CFile f;

    bFlag = TRUE;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName, i);
		bRet[i] = NAppDll_LoadModelData(i, filename);
		if (!bRet[i]) bFlag = FALSE;
		NAppDll_SetZoomScale(i, m_Setting.m_fCamScale[i]);
	}

	return bFlag;
}

BOOL CMainFrame::SaveModelData()
{
	BOOL bFlag, bRet[CAM_MAX_NUM];
	CString filename, pathName;
	long i;
	CFile f;

	// 모델 저장 루틴
	pathName.Format(_T("%s%s\\"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName);
	CreateDirectoryFullPath(pathName);

    bFlag = TRUE;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		filename.Format(_T("%s%s\\Cam%d.dat"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName, i);
		bRet[i] = NAppDll_SaveModelData(i, filename);
		if (!bRet[i]) bFlag = FALSE;
	}
	// 모델 저장 루틴

	return TRUE;
}

BOOL CMainFrame::LoadSysValueFromFile()
{
	CFile f;
	CString filename;

	// 시스템 파라미터 읽어옴
	if (f.Open((CString)SETTING_DATA_FILE, CFile::modeRead)) 
	{
		f.Read(&m_Setting, sizeof(SETTING_DATA));
		f.Read(&m_Search, sizeof(SEARCH_DATA));
		f.Close();
	}

//	filename.Format(_T("%s%s\\Etc.dat"), MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	if (f.Open((CString)ETC_DATA_FILE, CFile::modeRead)) 
	{
		f.Read(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}

	if (m_Setting.m_nDataPeriod>90) 
		m_Setting.m_nDataPeriod = 90;
	// 시스템 파라미터 읽어옴

	return TRUE;
}

BOOL CMainFrame::SaveSysValueToFile()
{
	CFile f;
	CString pathName, filename;

	// 시스템 파라미터를 저장함
	//pathName.Format(_T("%s%s\\"), (CString)MODEL_FILE_PATH, (CString)m_Setting.m_strModelName);
	//CreateDirectoryFullPath(pathName);

	if (f.Open((CString)SETTING_DATA_FILE, CFile::modeCreate|CFile::modeWrite)) 
	{
		f.Write(&m_Setting, sizeof(SETTING_DATA));
		f.Write(&m_Search, sizeof(SEARCH_DATA));
		f.Close();
	}
	else
		m_Setting.m_nOldHour = -1;

//	filename.Format("%s%s\\Etc.dat", MODEL_FILE_PATH, m_pMainFrm->m_Setting.m_strModelName);
	if (f.Open((CString)ETC_DATA_FILE, CFile::modeCreate|CFile::modeWrite)) 
	{
		f.Write(&m_pMainFrm->m_Etc, sizeof(ETC_DATA));
		f.Close();
	}
	// 시스템 파라미터를 저장함

	return TRUE;
}

void CMainFrame::SaveRealTimeData()
{
	BOOL bDec, bFlag;
	long nHead, nTail, nCAM, bDecF, bDecR;
	CString strPathName, str, strText;
	LPBYTE pImage;
	char szbuf[256];
//	int j=0;

	// 실시간 검사시 양/불량 영상저장
	nHead = g_DataRT.m_nImgHeadPnt;
	nTail = g_DataRT.m_nImgTailPnt;
	
	while(nTail != nHead)
	{
		nCAM = (unsigned long)g_DataRT.m_ImgCam[nTail];
		bDec = (BOOL)g_DataRT.m_ImgRes[nTail].m_bDecision;
		bDecF = (BOOL)g_DataRT.m_ImgRes[nTail].m_bFDecision;
		bDecR = (BOOL)g_DataRT.m_ImgRes[nTail].m_bRDecision;
		pImage = g_pImage[nTail];

		strPathName = m_Setting.m_strSelDriver;
		strPathName += SAVE_IMAGE_PATH;

		if (nCAM == CAM_UP)
			str.Format(_T("%4d%02d%02d\\%02d\\UP\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay, m_sysNewTime.wHour);
		else
			str.Format(_T("%4d%02d%02d\\%02d\\DN\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay, m_sysNewTime.wHour);
		strPathName += str;
	
		if (bDec) 
			strPathName += _T("NG\\");
		else 
			strPathName += _T("OK\\");
		 
#if 0
		for(j=0;j<GOM_MAX_NUM;j++)
			if ( !_tcscmp(g_DataRT.m_ImgRes[nTail].m_strTitle[j], _T("UP_TAB"))  )
				if(g_DataRT.m_ImgRes[nTail].m_dValue[j] < g_DataRT.m_ImgRes[nTail].m_dLowLimit[j])  
#endif // 0				
			
		bFlag = TRUE;
		StringToChar(strPathName, szbuf);

		//sprintf_s(szbuf2, " %s Save Image To HDD (START) ", szbuf);
		//m_pMainFrm->WriteLogToFile(szbuf2);

		CreateDirectoryFullPath(strPathName);

		str.Format(_T("%02d%02d_%d.jpg"), m_sysNewTime.wMinute, m_sysNewTime.wSecond, g_DataRT.m_ImgIndexP[nTail]);
	   	strPathName += str;
		StringToChar(strPathName, szbuf);

		//sprintf_s(szbuf2, " %s Save Image (MID) ", szbuf);
	    //m_pMainFrm->WriteLogToFile(szbuf2);

		if (m_Setting.m_bSaveImageType[nCAM])
		{
			// 양품 영상저장(축소된 원본 영상)
        	NVisionDll_SaveImageToFile(szbuf, pImage, IMAGE_WIDTH, IMAGE_HEIGHT, 1, 8);
		}
		else
		{
			// 불량 영상저장(축소 화면 캡춰된 영상)
			m_pView->m_pDlgExecAuto->m_pDlgBadImage->SetSaveImageFile(strPathName);
			m_pView->m_pDlgExecAuto->m_pDlgBadImage->UpdateFrameBadImage(nCAM, 0);
		}
		
		nTail++;
		if (nTail>=DEF_QUEUE_MAX_NUM)
			nTail = 0;
	}

	g_DataRT.m_nImgTailPnt = nTail;
	// 실시간 검사시 양/불량 영상저장
}

BOOL CMainFrame::CopyRealTimeImage(long nCAM, LPBYTE pImage)
{
	BOOL bRet=FALSE;
	long nHead;
	float fRate, fRateW, fRateH;
	LPBYTE pImageBuff;
	char szbuf[125];

	// 실시간 검사중 불량 영상을 큐 메모리에 저장함 
	fRateW = (float)IMAGE_WIDTH/(float)SMALL_IMAGE_WIDTH;
	fRateH = (float)IMAGE_HEIGHT/(float)SMALL_IMAGE_HEIGHT;
	if (fRateW>fRateH) fRate = fRateW;
	else fRate = fRateH;
	
	if ((m_Setting.m_bOKImageSave[nCAM] && !m_pRes[nCAM]->m_bDecision) ||
        (m_Setting.m_bNGImageSave[nCAM] && m_pRes[nCAM]->m_bDecision))
	{
		nHead = g_DataRT.m_nImgHeadPnt;

		g_DataRT.m_ImgCam[nHead] = nCAM;
		g_DataRT.m_nImgIndex++;
		g_DataRT.m_ImgIndexP[nHead] = m_Etc.m_nTotalInspCount+1;//g_DataRT.m_nImgIndex;

		//CopyResultData(&g_DataRT.m_ImgRes[nHead], m_pRes[nCAM]);
		memcpy(&g_DataRT.m_ImgRes[nHead], (INSP_RESULT_DATA *)m_pRes[nCAM], sizeof(INSP_RESULT_DATA));
		memcpy(g_pImage[nHead], pImage, IMAGE_WIDTH*IMAGE_HEIGHT);

		///////////    Test   ///////////
		sprintf_s(szbuf, "Copy : HEAD=%d TAIL=%d CAM=%d COUNT=%d", nHead, g_DataRT.m_nImgTailPnt, nCAM, g_DataRT.m_ImgIndexP[nHead]);
		//WriteLogToFile(szbuf);
		///////////    Test   ///////////
		
		nHead++;
		if (nHead>=DEF_QUEUE_MAX_NUM) 
			nHead = 0;

		g_DataRT.m_nImgHeadPnt = nHead;		
		bRet = TRUE;

		nHead = g_DataRT.m_nBadIndex[nCAM];
		pImageBuff = g_pBadImage[nCAM][nHead];

		memcpy(pImageBuff, pImage, IMAGE_WIDTH*IMAGE_HEIGHT);
		memcpy(&g_DataRT.m_BadRes[nCAM][nHead], (INSP_RESULT_DATA *)m_pMainFrm->m_pRes[nCAM], sizeof(INSP_RESULT_DATA));
		/*
		for(i=0; i<SMALL_IMAGE_HEIGHT; i++)
		{
			y = (long)(i*fRate);
			if (y>=IMAGE_HEIGHT) break;

			for(j=0; j<SMALL_IMAGE_WIDTH; j++)
			{
				x = (long)(j*fRate);
				if (x<IMAGE_WIDTH) 
				    pImageBuff[i*SMALL_IMAGE_WIDTH + j] = pImage[y*IMAGE_WIDTH + x];
			}
		}
		*/

		nHead++;
		if (nHead>=DEF_QUEUE_MAX_NUM)
			nHead=0;

		g_DataRT.m_nBadIndex[nCAM] = nHead;		
		bRet = TRUE;
	}
	// 실시간 검사중 불량 영상을 큐 메모리에 저장함 

	return bRet;
}

void CMainFrame::CopyRealTimeMelsec()
{
	BOOL bRet=FALSE;
	long i, nHead;

	// 상위에 데이타를 전송할 때 측정 데이타를 큐 메모리에 저장함 
	nHead = g_DataRT.m_nMelHeadPnt;

	for(i=0; i<CAM_MAX_NUM; i++)
    	CopyResultData(&g_DataRT.m_MelRes[i][nHead], m_pMainFrm->m_pRes[i]);

	nHead++;
	if (nHead>=DEF_QUEUE_MAX_NUM) 
		nHead = 0;

	g_DataRT.m_nMelHeadPnt = nHead;		
	// 상위에 데이타를 전송할 때 측정 데이타를 큐 메모리에 저장함 
}

void CMainFrame::CopyResultData(INSP_RESULT_DATA *pDest, INSP_RESULT_DATA *pSour)
{
	long i;

	// 실시간 검사 데이타를 복사함 
	pDest->m_bDecision = pSour->m_bDecision;

	pDest->m_bMisMatchDecF = pSour->m_bMisMatchDecF;
	pDest->m_bMisMatchDecR = pSour->m_bMisMatchDecR;
	pDest->m_dSensorDataF = pSour->m_dSensorDataF;
	pDest->m_dOffsetEPC = pSour->m_dOffsetEPC;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(pDest->m_strTitle[i], 10, pSour->m_strTitle[i]);
		pDest->m_bDecisionROI[i] = pSour->m_bDecisionROI[i];
		pDest->m_bLocROI[i] = pSour->m_bLocROI[i];
		pDest->m_bExecROI[i] = pSour->m_bExecROI[i];
		pDest->m_bEjectROI[i] = pSour->m_bEjectROI[i];
		pDest->m_nInspType[i] = pSour->m_nInspType[i];
		pDest->m_dValue[i] = pSour->m_dValue[i];
		pDest->m_dLowLimit[i] = pSour->m_dLowLimit[i];
		pDest->m_dStdSpec[i] = pSour->m_dStdSpec[i];
		pDest->m_dHighLimit[i] = pSour->m_dHighLimit[i];
	}

	pDest->m_nLinkNum = pSour->m_nLinkNum;
	for(i=0; i<pSour->m_nLinkNum; i++)
	{
		_tcscpy_s(pDest->m_strLinkTitle[i], 10, pSour->m_strLinkTitle[i]);
		pDest->m_bLinkDecROI[i] = pSour->m_bLinkDecROI[i];
		pDest->m_bLinkExecROI[i] = pSour->m_bLinkExecROI[i];
		pDest->m_bLinkEjectROI[i] = pSour->m_bLinkEjectROI[i];
		pDest->m_bLinkLocROI[i] = pSour->m_bLinkLocROI[i];
		pDest->m_nLinkType[i] = pSour->m_nLinkType[i];
		pDest->m_dLinkValue[i] = pSour->m_dLinkValue[i];
		pDest->m_dLinkLowLimit[i] = pSour->m_dLinkLowLimit[i];
		pDest->m_dLinkStdSpec[i] = pSour->m_dLinkStdSpec[i];
		pDest->m_dLinkHighLimit[i] = pSour->m_dLinkHighLimit[i];
	}
	// 실시간 검사 데이타를 복사함 
}

CString CMainFrame::GetModelName(long nOrderNum)
{
	long i;
	CString strModelName;
	
	// 검사ROI의 위치에서 모델명을 리턴함 
	strModelName = "";
	for(i=0; i<MODEL_MAX_NUM; i++)
	{
		if (nOrderNum==m_pMainFrm->m_Setting.m_OrderNum[i])
		{
			strModelName.Format(_T("%s"), (CString)m_Setting.m_ModelName[i]);
			break;
		}
	}
	// 검사ROI의 위치에서 모델명을 리턴함 

	return strModelName;
}

long CMainFrame::GetIndexTitleROI(INSP_RESULT_DATA* pRes, CString strTitle)
{
	long i, nPos;

	// 한 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함
	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp((TCHAR *)pRes->m_strTitle[i], strTitle) || !pRes->m_bLocROI[i]) continue;

		nPos = i;
		break;
	}
	// 한 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함

	return nPos;
}

long CMainFrame::GetIndexTitleLinkROI(INSP_RESULT_DATA* pRes, CString strTitle)
{
	long i, nPos;

	// 두 개의 검사항목에 구한하여 결과 구조체에서 strTitle과 부합하는 위치의 인텍스를 리턴함
	nPos = -1;
	for(i=0; i<pRes->m_nLinkNum; i++)
	{
		if (_tcscmp((TCHAR *)pRes->m_strLinkTitle[i], strTitle) || !pRes->m_bLinkLocROI[i]) continue;

		nPos = i;
		break;
	}

	return nPos;
}

void CMainFrame::ExecFinalCuttingOffset(long nSelExec, BOOL *bRetCUT, float *fOffCUT, BOOL *bRetSpare, float *fOffSpare, BOOL *bRetEPC, float *fOffEPC)
{
	BOOL bFlag;
	long i, nCamID;
	long nR1, nR2, nR3, nR4, nF1, nF2, nF3, nF4;
	float fT1, fT2;
	float fSumA1, fSumA2, dAvrA1, dAvrA2, fV, dMid;
	float fSumT2B, fSumTE, fAvrT2B, fAvrTE;
	long nCnt, nTotalCnt, nSkipCnt, nMaxCnt;
	CString strT, strTitle1, strTitle2, strTitle3, strTitle4;

	bFlag = FALSE;

	*bRetCUT = 0;
	*fOffCUT = 0.0f;

	*bRetSpare = 0;
	*fOffSpare = 0.0f;
	*bRetEPC = 0;
	*fOffEPC = 0.0f;

	if (nSelExec==1 || nSelExec==3) nCamID = CAM_UP;
	else if (nSelExec==2) nCamID = CAM_DN;
	else return;

	nCamID = m_Setting.m_nSelUpDn;
	nSkipCnt = m_Setting.m_nSkipCnt;
	nMaxCnt = nSkipCnt + m_Setting.m_nDaqUnitCnt;

	nF1 = nF2 = nF3 = nF4 = -1;
	nR1 = nR2 = nR3 = nR4 = -1;

	if (m_Setting.m_bExecCutOffset)
	{
		if (nCamID==CAM_UP) strTitle1 = "A1U";
		else strTitle1 = "A1L";

		if (nCamID==CAM_UP) strTitle2 = "A2U";
		else strTitle2 = "A2L";

		if (nCamID==CAM_UP) strTitle3 = "A3U";
		else strTitle3 = "A3L";

		if (nCamID==CAM_UP) strTitle4 = "A4U";
		else strTitle4 = "A4L";

		for(i=0; i<GOM_MAX_NUM; i++)
		{
			if (m_pRes[nCamID]->m_bLocROI[i]) continue;

			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle1)) nF1 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle2)) nF2 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle3)) nF3 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle4)) nF4 = i;
		}

#if _DUO_CELL
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			if (!m_pRes[nCamID]->m_bLocROI[i]) continue;

			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle1)) nR1 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle2)) nR2 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle3)) nR3 = i;
			if (!_tcscmp((CString)m_pRes[nCamID]->m_strTitle[i], strTitle4)) nR4 = i;
		}
#endif
	}

	if (!m_pRes[nCamID]->m_bFDecision)
	{
		m_OffsetD[m_nDaqIndex].m_A1 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A2 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A3 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A4 = 0.0f;

		if (nF1>=0) m_OffsetD[m_nDaqIndex].m_A1 = m_pRes[nCamID]->m_dValue[nF1];
		if (nF2>=0) m_OffsetD[m_nDaqIndex].m_A2 = m_pRes[nCamID]->m_dValue[nF2];
		if (nF3>=0) m_OffsetD[m_nDaqIndex].m_A3 = m_pRes[nCamID]->m_dValue[nF3];
		if (nF4>=0) m_OffsetD[m_nDaqIndex].m_A4 = m_pRes[nCamID]->m_dValue[nF4];

		m_nDaqIndex++;
		if (m_nDaqIndex>=nMaxCnt)
		{
			m_nDaqIndex = nSkipCnt;
			bFlag = TRUE;
		}
	}
	
#if _DUO_CELL
	if (!m_pRes[nCamID]->m_bRDecision)
	{
		m_OffsetD[m_nDaqIndex].m_A1 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A2 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A3 = 0.0f;
		m_OffsetD[m_nDaqIndex].m_A4 = 0.0f;

		if (nR1>=0) m_OffsetD[m_nDaqIndex].m_A1 = m_pRes[nCamID]->m_dValue[nR1];
		if (nR2>=0) m_OffsetD[m_nDaqIndex].m_A2 = m_pRes[nCamID]->m_dValue[nR2];
		if (nR3>=0) m_OffsetD[m_nDaqIndex].m_A3 = m_pRes[nCamID]->m_dValue[nR3];
		if (nR4>=0) m_OffsetD[m_nDaqIndex].m_A4 = m_pRes[nCamID]->m_dValue[nR4];

		m_nDaqIndex++;
		if (m_nDaqIndex>=nMaxCnt)
		{
			m_nDaqIndex = nSkipCnt;
			bFlag = TRUE;
		}
	}
#endif
	
	if (bFlag)
	{
		fSumA1 = 0;
		fSumA2 = 0;

		fSumT2B = 0;
		fSumTE = 0;

		dAvrA1 = 0;
		dAvrA2 = 0;
		fAvrT2B = 0;
		fAvrTE = 0;

		nTotalCnt = 0;
		for(i=nSkipCnt; i<nMaxCnt; i++)
		{
			fT1 = 0.0f;
			nCnt = 0;
			if (m_OffsetD[i].m_A1>0) nCnt++;
			if (m_OffsetD[i].m_A2>0) nCnt++;

			if (nCnt>0) 
				fT1 = (m_OffsetD[i].m_A1 + m_OffsetD[i].m_A2)/nCnt;

			fT2 = 0.0f;
			nCnt = 0;
			if (m_OffsetD[i].m_A3>0) nCnt++;
			if (m_OffsetD[i].m_A4>0) nCnt++;

			if (nCnt>0) 
				fT2 = (m_OffsetD[i].m_A3 + m_OffsetD[i].m_A4)/nCnt;

			fSumA1 += fT1;
			fSumA2 += fT2;
			nTotalCnt++;
		}

		if (nTotalCnt>0)
		{
			dAvrA1 = fSumA1/nCnt;
			dAvrA2 = fSumA2/nCnt;

			dMid = (dAvrA1+dAvrA2)/2;
			fV = ((dAvrA1 - dMid) + (dMid-dAvrA2))/2;

			fV = -1*fV;
			if (fabs(fV)<1.0f)
			{
				*bRetCUT = 2;
				*fOffCUT = fV;
				m_fSendDataCUT = fV;
	    	}
		}
	}
}

void CMainFrame::SendResult2UMAC(BOOL bFDec, BOOL bRDec, BOOL bFTabDec, BOOL bRTabDec, BOOL bMisMatch, float fCutOS, BOOL bRetEPC, float fOffsetEPC)
{
	long nInt=0, nErrCnt=0;
	double dTime;

	m_ctrlTimer.SetClockTimer(1);

#if _UMAC
	///////////////////////////////////////////////////////////
	//////////   양/불 출력을 UMAC ETHERNET으로 송부    //////////
	///////////////////////////////////////////////////////////
	if (bFDec)
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_FRONT, 1);
	else
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_FRONT, 2);

	if (bRDec)
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_REAR, 1);	
	else
		NUmacDll_SetIntValue('P', UMAC_OUT_REJECT_REAR, 2);	

//	if (bRetCUT==2)
//		NUmacDll_SetDoubleValue('P', UMAC_OUT_FINAL_CUT, fCutOS);
//	if (bRetEPC)
//		NUmacDll_SetDoubleValue('P', UMAC_OUT_EPC, fOffsetEPC);

	m_pMainFrm->NPio_BitWrite(PIO_OUT_READY_PORT, TRUE);
#else
	if (bFDec) NPio_BitWrite(PIO_OUT_FRONT_NG_PORT, TRUE);
	else NPio_BitWrite(PIO_OUT_FRONT_OK_PORT, TRUE);

	if (bRDec) NPio_BitWrite(PIO_OUT_REAR_NG_PORT, TRUE);
	else NPio_BitWrite(PIO_OUT_REAR_OK_PORT, TRUE);

	timeSetEvent(100, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_PIO_INSP_CLEAR, TIME_ONESHOT);
#endif
	
	dTime = m_ctrlTimer.GetClockTimer(1);

	// REJECT OUT(BAD)
    sprintf_s(g_LOG.m_sLogTitle5, "SEND2UMAC(RES=%d F=%d R=%d T=%5.3f)", nInt, bFDec, bRDec, dTime);

	m_Etc.m_nTotalInspCount++;
	if (bFDec) m_Etc.m_nTotalBadCount++;

#if _DUO_CELL
	m_Etc.m_nTotalInspCount++;
	if (bRDec) m_Etc.m_nTotalBadCount++;
#endif
}

void CMainFrame::WriteStringToFile()
{
	CString strModelID, strLotID;

	// GAUGE R&R 테스트시 LOT ID는  "R_R"로 저장됨
	strModelID.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	strLotID = m_pMainFrm->m_strLotID;

	/*
	if (m_bGaugeRnR)
	{
		strLotID = _T("R_R");
		GetLocalTime(&m_sysNewTime);
	}
	*/

	// GAUGE R&R 테스트시 LOT ID는  "TEST"로 저장됨

	//EnterCriticalSection(&m_csSync);
	long i, j, k;
	double dTime;
	BOOL bDec;
	FILE  *fp;
	TCHAR   szbuf[1200];
	CString STR_ROI[CAM_MAX_NUM][GOM_MAX_NUM], STR_LINK[CAM_MAX_NUM][GOM_MAX_NUM];
	CString strPathName, strDataPath, strTitle, str, strT;
	INSP_RESULT_DATA *pResData[CAM_MAX_NUM];
	
	for(i=0; i<CAM_MAX_NUM; i++)
	    pResData[i] = (INSP_RESULT_DATA *)m_pRes[i];

	strDataPath = SAVE_DATA_PATH;
	str.Format(_T("%04d\\%02d\\%02d\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay);
	strDataPath += str;
	_stprintf_s(szbuf, _T("%s%02d.txt"), strDataPath, m_sysNewTime.wHour);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	m_pMainFrm->m_ctrlTimer.SetClockTimer(1);
	if (m_sysOldTime.wHour != m_sysNewTime.wHour)
	{
		CreateDirectoryFullPath(strDataPath);
		_tfopen_s(&fp, szbuf, _T("w+"));
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

	dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(1);
	sprintf_s(g_LOG.m_sLogTitle12, "W_PROC1(T=%5.3f)", dTime);


#if _DUO_CELL
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d %1d %9d"), 1, (long)(pResData[CAM_UP]->m_bRDataDec), m_Etc.m_nTotalInspCount-1);
	strT += str;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			STR_ROI[i][j] = "";

			if (pResData[i]->m_nInspType[j] == INSP_MEASURE_SIDE_ONE) continue;
			if (_tcscmp(pResData[i]->m_strTitle[j], _T("")) && pResData[i]->m_bLocROI[j])
			{
				str.Format(_T("%7.3f"), pResData[i]->m_dValue[j]);
				if (str.GetLength()>7)
					pResData[i]->m_dValue[j] = 0.0f;

				bDec = pResData[i]->m_bDecisionROI[j];
				if (bDec && !pResData[i]->m_bEjectROI[j])
					bDec = 2;

				if (!pResData[i]->m_bExecROI[j]) 
					bDec = 0;

				str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strTitle[j], i, (long)bDec, pResData[i]->m_dValue[j]);
				strT += str;

				STR_ROI[i][j].Format(_T("%s"), pResData[i]->m_strTitle[j]); 
			}
		}
		
		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			STR_LINK[i][j] = "";

			if (_tcscmp(pResData[i]->m_strLinkTitle[j], _T("")) && pResData[i]->m_bLinkLocROI[j])
			{
				str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[j]);
				if (str.GetLength()>7)
					pResData[i]->m_dLinkValue[j] = 0.0f;

				bDec = pResData[i]->m_bLinkDecROI[j];
				if (bDec && !pResData[i]->m_bLinkEjectROI[j])
					bDec = 2;

				if (!pResData[i]->m_bLinkExecROI[j]) 
					bDec = 0;

				str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValue[j]);
				strT += str;

				STR_LINK[i][j].Format(_T("%s"), pResData[i]->m_strLinkTitle[j]); 
			}
		}
	}

	str.Format(_T("%10s %1d %10s %1d "), m_strUnW_Up_Barcode, m_nUpperPos, m_strUnW_Dn_Barcode, m_nDownPos);
	strT += str;

	if (fp)
		_ftprintf(fp, _T("%s\n"), strT);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d %1d %9d"), 0, (long)(pResData[CAM_UP]->m_bFDataDec), m_Etc.m_nTotalInspCount);
	strT += str;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			strTitle = STR_ROI[i][j];
			if (strTitle == "") continue;

			for(k=0; k<GOM_MAX_NUM; k++)
			{
				if (_tcscmp((CString)pResData[i]->m_strTitle[k], strTitle) || pResData[i]->m_bLocROI[k])
					continue;

				str.Format(_T("%7.3f"), pResData[i]->m_dValue[k]);
				if (str.GetLength()>7)
					pResData[i]->m_dValue[k] = 0.0f;

				bDec = pResData[i]->m_bDecisionROI[k];
				if (bDec && !pResData[i]->m_bEjectROI[k])
					bDec = 2;

				if (!pResData[i]->m_bExecROI[k]) 
					bDec = 0;

				str.Format(_T("%10s %1d %1d %7.3f "), strTitle, i, (long)bDec, pResData[i]->m_dValue[k]);
				strT += str;

				break;
			}
		}

		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			strTitle = STR_LINK[i][j];
			if (strTitle == "") continue;

			for(k=0; k<pResData[i]->m_nLinkNum; k++)
			{
				if (_tcscmp((CString)pResData[i]->m_strLinkTitle[k], strTitle) || pResData[i]->m_bLinkLocROI[k])
					continue;

				str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[k]);
				if (str.GetLength()>7)
					pResData[i]->m_dLinkValue[k] = 0.0f;

				bDec = pResData[i]->m_bLinkDecROI[k];
				if (bDec && !pResData[i]->m_bLinkEjectROI[k])
					bDec = 2;

				if (!pResData[i]->m_bLinkExecROI[k]) 
					bDec = 0;

				str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[k], i, (long)bDec, pResData[i]->m_dLinkValue[k]);
				strT += str;

				/*
				if (pResData[i]->m_nLinkType[k] == 	TEACH_SLINE_SURFACE)
				{
					str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[k], i, (long)bDec, pResData[i]->m_dLinkValueS[k]);
				    strT += str;
				}
				*/
			}
		}
	}
#else
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d %1d %9d"), 0, (long)bFDec, m_Etc.m_nTotalInspCount);
	strT += str;

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (!_tcscmp((CString)pResData[i]->m_strTitle[j], _T("")) || pResData[i]->m_bLocROI[j]) 
				continue;

			str.Format(_T("%7.3f"), pResData[i]->m_dValue[j]);
			if (str.GetLength()>7)
				pResData[i]->m_dValue[j] = 0.0f;

			bDec = pResData[i]->m_bDecisionROI[j];
			if (bDec && !pResData[i]->m_bEjectROI[j])
				bDec = 2;

			if (!pResData[i]->m_bExecROI[j]) 
				bDec = 0;

			str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strTitle[j], i, (long)bDec, pResData[i]->m_dValue[j]);
			strT += str;
		}

		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			if (!_tcscmp((CString)pResData[i]->m_strLinkTitle[j], _T("")) || pResData[i]->m_bLinkLocROI[j])
				continue;

			str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[j]);
			if (str.GetLength()>7)
				pResData[i]->m_dLinkValue[j] = 0.0f;

			bDec = pResData[i]->m_bLinkDecROI[j];
			if (bDec && !pResData[i]->m_bLinkEjectROI[j])
				bDec = 2;

			if (!pResData[i]->m_bLinkExecROI[j]) 
				bDec = 0;

			str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValue[j]);
			strT += str;

			/*
			if (pResData[i]->m_nLinkType[j] == 	TEACH_SLINE_SURFACE)
			{
				str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValueS[j]);
				strT += str;
			}
			*/
		}
	}
#endif
	str.Format(_T("%10s %1d %10s %1d "), m_strUnW_Up_Barcode, m_nUpperPos, m_strUnW_Dn_Barcode, m_nDownPos);
	strT += str;
	if (fp)
	{
		_ftprintf(fp, _T("%s\n"), strT);
		fclose(fp);
	}

	m_sysOldTime = m_sysNewTime;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//LeaveCriticalSection(&m_csSync);
}

void CMainFrame::WriteStringToFile_Csv_Day()
{
	CString strModelID, strLotID;

	// GAUGE R&R 테스트시 LOT ID는  "R_R"로 저장됨
	strModelID.Format(_T("%s"), m_pMainFrm->m_Setting.m_strModelName);
	strLotID = m_pMainFrm->m_strLotID;

	/*
	if (m_bGaugeRnR)
	{
		strLotID = _T("R_R");
		GetLocalTime(&m_sysNewTime);
	}
	*/

	// GAUGE R&R 테스트시 LOT ID는  "TEST"로 저장됨

	//EnterCriticalSection(&m_csSync);
	long i, j, k;
	double dTime;
	BOOL bDec;
	FILE  *fp;
	TCHAR   szbuf[1200];
	CString STR_ROI[CAM_MAX_NUM][GOM_MAX_NUM], STR_LINK[CAM_MAX_NUM][GOM_MAX_NUM];
	CString strPathName, strDataPath, strTitle, str, strT;
	INSP_RESULT_DATA *pResData[CAM_MAX_NUM];
	
	for(i=0; i<CAM_MAX_NUM; i++)
	    pResData[i] = (INSP_RESULT_DATA *)m_pRes[i];

	str.Format(_T("\\%04d\\%02d\\"), m_sysNewTime.wYear, m_sysNewTime.wMonth);
	strDataPath += str;
	_stprintf_s(szbuf, _T("%s%2d.csv"), strDataPath, m_sysNewTime.wDay);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	if (m_sysOldTime.wDay!=m_sysNewTime.wDay)
	{
		CreateDirectoryFullPath(strDataPath);
		_tfopen_s(&fp, szbuf, _T("w+"));
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

	dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(1);
	sprintf_s(g_LOG.m_sLogTitle12, "W_PROC1(T=%5.3f)", dTime);


#if _DUO_CELL
	strT.Format(_T("%20s, %04d%02d%02d %02d%02d%02d, "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d, %1d, %9d, "), 1, (long)(pResData[CAM_UP]->m_bRDataDec), m_Etc.m_nTotalInspCount-1);
	strT += str;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			STR_ROI[i][j] = "";

			if (pResData[i]->m_nInspType[j] == INSP_MEASURE_SIDE_ONE) continue;
			if (_tcscmp(pResData[i]->m_strTitle[j], _T("")) && pResData[i]->m_bLocROI[j])
			{
				str.Format(_T("%7.3f"), pResData[i]->m_dValue[j]);
				if (str.GetLength()>7)
					pResData[i]->m_dValue[j] = 0.0f;

				bDec = pResData[i]->m_bDecisionROI[j];
				if (bDec && !pResData[i]->m_bEjectROI[j])
					bDec = 2;

				if (!pResData[i]->m_bExecROI[j]) 
					bDec = 0;

				str.Format(_T("%10s, %1d, %1d, %7.3f, "), pResData[i]->m_strTitle[j], i, (long)bDec, pResData[i]->m_dValue[j]);
				strT += str;

				STR_ROI[i][j].Format(_T("%s"), pResData[i]->m_strTitle[j]); 
			}
		}
		
		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			STR_LINK[i][j] = "";

			if (_tcscmp(pResData[i]->m_strLinkTitle[j], _T("")) && pResData[i]->m_bLinkLocROI[j])
			{
				str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[j]);
				if (str.GetLength()>7)
					pResData[i]->m_dLinkValue[j] = 0.0f;

				bDec = pResData[i]->m_bLinkDecROI[j];
				if (bDec && !pResData[i]->m_bLinkEjectROI[j])
					bDec = 2;

				if (!pResData[i]->m_bLinkExecROI[j]) 
					bDec = 0;

				str.Format(_T("%10s, %1d, %1d, %7.3f, "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValue[j]);
				strT += str;

				STR_LINK[i][j].Format(_T("%s"), pResData[i]->m_strLinkTitle[j]); 
			}
		}
	}

	str.Format(_T("%10s, %1d, %10s, %1d, "), m_strUnW_Up_Barcode, m_nUpperPos, m_strUnW_Dn_Barcode, m_nDownPos);
	strT += str;

	if (fp)
		_ftprintf(fp, _T("%s\n"), strT);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	strT.Format(_T("%20s, %04d%02d%02d %02d%02d%02d, "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d, %1d, %9d, "), 0, (long)(pResData[CAM_UP]->m_bFDataDec), m_Etc.m_nTotalInspCount);
	strT += str;

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			strTitle = STR_ROI[i][j];
			if (strTitle == "") continue;

			for(k=0; k<GOM_MAX_NUM; k++)
			{
				if (_tcscmp((CString)pResData[i]->m_strTitle[k], strTitle) || pResData[i]->m_bLocROI[k])
					continue;

				str.Format(_T("%7.3f"), pResData[i]->m_dValue[k]);
				if (str.GetLength()>7)
					pResData[i]->m_dValue[k] = 0.0f;

				bDec = pResData[i]->m_bDecisionROI[k];
				if (bDec && !pResData[i]->m_bEjectROI[k])
					bDec = 2;

				if (!pResData[i]->m_bExecROI[k]) 
					bDec = 0;

				str.Format(_T("%10s, %1d, %1d, %7.3f, "), strTitle, i, (long)bDec, pResData[i]->m_dValue[k]);
				strT += str;

				break;
			}
		}

		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			strTitle = STR_LINK[i][j];
			if (strTitle == "") continue;

			for(k=0; k<pResData[i]->m_nLinkNum; k++)
			{
				if (_tcscmp((CString)pResData[i]->m_strLinkTitle[k], strTitle) || pResData[i]->m_bLinkLocROI[k])
					continue;

				str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[k]);
				if (str.GetLength()>7)
					pResData[i]->m_dLinkValue[k] = 0.0f;

				bDec = pResData[i]->m_bLinkDecROI[k];
				if (bDec && !pResData[i]->m_bLinkEjectROI[k])
					bDec = 2;

				if (!pResData[i]->m_bLinkExecROI[k]) 
					bDec = 0;

				str.Format(_T("%10s, %1d, %1d, %7.3f, "), pResData[i]->m_strLinkTitle[k], i, (long)bDec, pResData[i]->m_dLinkValue[k]);
				strT += str;

				/*
				if (pResData[i]->m_nLinkType[k] == 	TEACH_SLINE_SURFACE)
				{
					str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[k], i, (long)bDec, pResData[i]->m_dLinkValueS[k]);
				    strT += str;
				}
				*/
			}
		}
	}
#else
	strT.Format(_T("%20s %04d%02d%02d %02d%02d%02d "),
					strLotID, m_sysNewTime.wYear, m_sysNewTime.wMonth, m_sysNewTime.wDay,
					m_sysNewTime.wHour, m_sysNewTime.wMinute, m_sysNewTime.wSecond);
	str.Format(_T("%1d %1d %9d"), 0, (long)bFDec, m_Etc.m_nTotalInspCount);
	strT += str;

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (!_tcscmp((CString)pResData[i]->m_strTitle[j], _T("")) || pResData[i]->m_bLocROI[j]) 
				continue;

			str.Format(_T("%7.3f"), pResData[i]->m_dValue[j]);
			if (str.GetLength()>7)
				pResData[i]->m_dValue[j] = 0.0f;

			bDec = pResData[i]->m_bDecisionROI[j];
			if (bDec && !pResData[i]->m_bEjectROI[j])
				bDec = 2;

			if (!pResData[i]->m_bExecROI[j]) 
				bDec = 0;

			str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strTitle[j], i, (long)bDec, pResData[i]->m_dValue[j]);
			strT += str;
		}

		for(j=0; j<pResData[i]->m_nLinkNum; j++)
		{
			if (!_tcscmp((CString)pResData[i]->m_strLinkTitle[j], _T("")) || pResData[i]->m_bLinkLocROI[j])
				continue;

			str.Format(_T("%7.3f"), pResData[i]->m_dLinkValue[j]);
			if (str.GetLength()>7)
				pResData[i]->m_dLinkValue[j] = 0.0f;

			bDec = pResData[i]->m_bLinkDecROI[j];
			if (bDec && !pResData[i]->m_bLinkEjectROI[j])
				bDec = 2;

			if (!pResData[i]->m_bLinkExecROI[j]) 
				bDec = 0;

			str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValue[j]);
			strT += str;

			/*
			if (pResData[i]->m_nLinkType[j] == 	TEACH_SLINE_SURFACE)
			{
				str.Format(_T("%10s %1d %1d %7.3f "), pResData[i]->m_strLinkTitle[j], i, (long)bDec, pResData[i]->m_dLinkValueS[j]);
				strT += str;
			}
			*/
		}
	}
#endif
	str.Format(_T("%10s, %1d, %10s, %1d, "), m_strUnW_Up_Barcode, m_nUpperPos, m_strUnW_Dn_Barcode, m_nDownPos);
	strT += str;

	if (fp)
	{
		_ftprintf(fp, _T("%s\n"), strT);
		fclose(fp);
	}

	m_sysOldTime = m_sysNewTime;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//LeaveCriticalSection(&m_csSync);
}

static CString UP_CAM[] = {_T("A1U"), _T("A2U"), _T("A3U"), _T("A4U"), _T("A5U"), _T("A6U"), _T("T.H"), _T("TH"), _T("T.W"), _T("TW")};
static CString DN_CAM[] = {_T("A1L"), _T("A2L"), _T("A3L"), _T("A4L"), _T("A5L"), _T("A6L"), _T("T.H"), _T("TH"), _T("T.W"), _T("TW")};
static CString UP_LINK[] = {_T("STA"), _T("STC")};
static CString DN_LINK[] = {_T("STA"), _T("STC")};

void CMainFrame::SendDataToPLC()
//LONG CDlgExecAuto::OnSendDataToPLC(UINT wParam, LONG lParam)
{
	long i, j, k, nIndex, nRow, nRet;
	long nHead, nTail, nCnt, nPosT;
	CString strTitle[60];
	char szbuf[128];
	long  DATA[85];
	long  F_DATA[GOM_MAX_NUM], R_DATA[GOM_MAX_NUM], nTemp;
	CString strInspT[GOM_MAX_NUM], strT;
	double dTime;
	
	m_pMainFrm->m_ctrlTimer.SetClockTimer(2);

   	sprintf_s(szbuf, " Transfer insp. result data to PLC. (START) ");
//	m_pMainFrm->WriteLogToFile(szbuf);

	nHead = g_DataRT.m_nMelHeadPnt;
	nTail = g_DataRT.m_nMelTailPnt;
	
    nPosT = 0;
	nCnt = 0;
	while(nTail != nHead)
	{
    	for(i=0; i<85; i++) DATA[i] = 0;
			
		for(i=0; i<CAM_MAX_NUM; i++)
		{
			if (!m_Setting.m_bExecInsp[i]) continue;

			nPosT = 0;
			if (i==1) nPosT = 16;

			nRow = 0;
			for(j=0; j<GOM_MAX_NUM; j++)
			{
				F_DATA[nRow] = 0;
			    R_DATA[nRow] = 0;

				if (g_DataRT.m_MelRes[i][nTail].m_nInspType[j] == INSP_MEASURE_SIDE_ONE) continue;
				if (!_tcscmp((TCHAR *)g_DataRT.m_MelRes[i][nTail].m_strTitle[j], _T(""))) continue;

#if _DUO_CELL
				if (g_DataRT.m_MelRes[i][nTail].m_bLocROI[j]) continue;
#endif


				nIndex = GetIndexTitleROI(g_DataRT.m_MelRes[i], g_DataRT.m_MelRes[i][nTail].m_strTitle[j]);
				if (nIndex>=0)
				{
					F_DATA[nRow] = (long)(g_DataRT.m_MelRes[i][nTail].m_dValue[j]*100);
				    R_DATA[nRow] = (long)(g_DataRT.m_MelRes[i][nTail].m_dValue[nIndex]*100);
					strInspT[nRow].Format(_T("%s"),  g_DataRT.m_MelRes[i][nTail].m_strTitle[j]);
					nRow++;
				}
			}

			for(j=0; j<g_DataRT.m_MelRes[i][nTail].m_nLinkNum; j++)
			{
				if (!_tcscmp((TCHAR *)g_DataRT.m_MelRes[i][nTail].m_strLinkTitle[j], _T(""))) continue;

#if _DUO_CELL
				if (g_DataRT.m_MelRes[i][nTail].m_bLinkLocROI[j]) continue;
#endif

				nIndex = GetIndexTitleLinkROI(g_DataRT.m_MelRes[i], (LPCTSTR)g_DataRT.m_MelRes[i][nTail].m_strLinkTitle[j]);
				if (nIndex>=0)
				{
					F_DATA[nRow] = (long)(g_DataRT.m_MelRes[i][nTail].m_dLinkValue[j]*100);
					R_DATA[nRow] = (long)(g_DataRT.m_MelRes[i][nTail].m_dLinkValue[nIndex]*100);
					strInspT[nRow].Format(_T("%s"),  g_DataRT.m_MelRes[i][nTail].m_strLinkTitle[j]);
					nRow++;
				}
			}

			for(j=0; j<nRow-1; j++)
			for(k=j+1; k<nRow; k++)
			{
				if (_tcscmp(strInspT[j], strInspT[k])>0)
				{
					nTemp = F_DATA[j];
					F_DATA[j] = F_DATA[k];
					F_DATA[k] = nTemp;

					strT = strInspT[j];
					strInspT[j] = strInspT[k];
					strInspT[k] = strT;

					nTemp = R_DATA[j];
					R_DATA[j] = R_DATA[k];
					R_DATA[k] = nTemp;
				}
			}

			for(j=0; j<nRow; j++)
			{
				strInspT[j];
				DATA[j+nPosT] = F_DATA[j];
				DATA[j+nPosT+32] = R_DATA[j];
			}
		}

		/*
		DATA[70] = (int)(m_pMainFrm->m_fFrontShort*1000.0);
		DATA[71] = (int)(m_pMainFrm->m_fRearShort*1000.0);
		DATA[72] = (int)m_pMainFrm->m_Setting.m_fTestVolt;
		DATA[73] = (int)(m_pMainFrm->m_Setting.m_fLimitCurr*1000.0);
		DATA[74] = (int)(m_pMainFrm->m_Setting.m_fStartDelayTime*1000.0);
		DATA[75] = (int)(m_pMainFrm->m_Setting.m_fRampingTime*1000.0);
		DATA[76] = (int)(m_pMainFrm->m_Setting.m_fHoldingTime*1000.0);
		DATA[77] = (int)(m_pMainFrm->m_Setting.m_fContactCurr*1000.0);

		DATA[78] = (int)m_pMainFrm->m_Setting.m_fTestVolt;
		DATA[79] = (int)(m_pMainFrm->m_Setting.m_fLimitCurr*1000.0);
		DATA[80] = (int)(m_pMainFrm->m_Setting.m_fStartDelayTime*1000.0);
		DATA[81] = (int)(m_pMainFrm->m_Setting.m_fRampingTime*1000.0);
		DATA[82] = (int)(m_pMainFrm->m_Setting.m_fHoldingTime*1000.0);
		DATA[83] = (int)(m_pMainFrm->m_Setting.m_fContactCurr*1000.0);
		*/

		nRet = m_pView->m_pDlgExecAuto->m_ActEasyIF.WriteDeviceBlock(PLC_OUT_VISION_DATA, 85, DATA);
		if (nRet==0)
			m_pMainFrm->m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);	
		else
		{
			m_pMainFrm->m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
			//m_pMainFrm->NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
			m_pMainFrm->WriteTextMessage(DEF_MSG_PLC_ERROR);
		}

		nTail++;
		if (nTail>=DEF_QUEUE_MAX_NUM)
			nTail = 0;

		nCnt++;
		if (nCnt>=2) break;
	}

	g_DataRT.m_nMelTailPnt = nTail;

	sprintf_s(szbuf, " Transfer insp. result data to PLC.(END), RET(%d) ", (int)nRet);
	m_pMainFrm->WriteLogToFile(szbuf);

	dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(2);
	sprintf_s(g_LOG.m_sLogTitle7, "PLC_SEND(T=%5.3f)", dTime);
//	return 0;
}

void CMainFrame::WriteLogToFile(char *string)
{
	FILE  *fp;
	CString str;
	char strFile[150];

	//Enter Critical Section   //
	EnterCriticalSection(&m_csSync);
	//Enter Critical Section   //

	// 로그를 저장하는 함수
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

    str.Format(_T("%s%d\\%02d\\%02d"), _T("C:\\DATABASE\\LOG\\"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	sprintf_s(strFile, "%s%d\\%02d\\%02d\\%02d.txt", LOG_DATA_FILE, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour);

	if (m_Setting.m_nOldHour != lpSystemTime.wHour)
	{
		CreateDirectoryFullPath(str);
		fopen_s(&fp, strFile, "w+");

		m_Setting.m_nOldHour = lpSystemTime.wHour;
		SaveSysValueToFile();
	}
	else
	{
		fopen_s(&fp, strFile, "a+");
		if (!fp)
		{
			CreateDirectoryFullPath(str);
			fopen_s(&fp, strFile, "w+");
		}
	}

	if (fp)
	{
		fprintf(fp, "[%02d:%02d:%03d] %s\n", lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, string);
		fclose(fp);
	}

	m_Setting.m_nOldHour = lpSystemTime.wHour;
	// 로그를 저장하는 함수

	LeaveCriticalSection(&m_csSync);
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////        SYSTEM/MODEL DATA SAVE MODULE       /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int CMainFrame::ExecMessageHandleAll()
{
	int returnValue;
	MSG Mess;

	// 윈도우의 메세지 큐에 있는 메세지를 전부 실행함
	do {
		returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
		if (returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	} while(returnValue);
	// 윈도우의 메세지 큐에 있는 메세지를 전부 실행함

	return returnValue;
}

int CMainFrame::ExecMessageHandleOne()
{
	int returnValue;
	MSG Mess;

	// 윈도우의 메세지 큐에 있는 메세지를 하나만 실행함
	returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
	if (returnValue)
	{
		::TranslateMessage(&Mess);
		::DispatchMessage (&Mess);
	}
	// 윈도우의 메세지 큐에 있는 메세지를 하나만 실행함
	
	return returnValue;
}

void CMainFrame::DeleteDataPath()
{
	CTime tmStartDay, tmEndDay;
	long nStartDay, nEndDay;
	CString strPathName, str;
	
	tmEndDay = CTime::GetCurrentTime() - CTimeSpan(m_Setting.m_nDataPeriod, 0, 0, 0);
	tmStartDay = tmEndDay - CTimeSpan(100, 0, 0, 0);
	nStartDay = _ttoi(tmStartDay.Format(_T("%Y%m%d")));
	nEndDay = _ttoi(tmEndDay.Format(_T("%Y%m%d")));

	strPathName = "";
	strPathName = m_Setting.m_strSelDriver;
	strPathName += SAVE_IMAGE_PATH;

	while(nStartDay<=nEndDay)
	{
		str.Format(_T("%s%4d%02d%02d\\"), strPathName, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

		str.Format(_T("%s%4d\\%02d\\%02d\\"), SAVE_DATA_PATH, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

		str.Format(_T("%s%4d\\%02d\\"), SAVE_DATA_PATH_CSV, tmStartDay.GetYear(), tmStartDay.GetMonth());               // shsong day단위로 저장한걸 지울때
		RemoveDir(str);

#if SHORTAGE_USE
		str.Format(_T("%s%4d\\%02d\\%02d\\"), SAVE_SHORTDATA_PATH, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);
#endif // SHORTAGE_USE

		str.Format(_T("%s%4d\\%02d\\%02d\\"), LOG_DATA_FILE, tmStartDay.GetYear(), tmStartDay.GetMonth(), tmStartDay.GetDay());
		RemoveDir(str);

		tmStartDay += CTimeSpan(1, 0, 0, 0);
   		nStartDay = _ttoi(tmStartDay.Format(_T("%Y%m%d")));
	}
}

long static m_nIndexCnt=0;
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	long nRet=0, nData=0, nInt=0;
	CString str;

	if (!m_bAppExec)
	{
		KillTimer(1000);
		KillTimer(2000);
		return;
	}

	if (nIDEvent==1000 && m_bExecRealTime==FALSE)
	{
#if _UMAC
		nInt = NUmacDll_GetIntValue('P', UMAC_IN_COMM);          
		if (nInt==2)
		{
			if(m_pView->m_pDlgMainBar !=NULL) 
				m_pView->m_pDlgMainBar->SetCommStatus(1, TRUE);
			//NPio_BitWrite(PIO_OUT_READY_PORT, TRUE);
		}
		else
		{
			if(m_pView->m_pDlgMainBar !=NULL) 
				m_pView->m_pDlgMainBar->SetCommStatus(1, FALSE);
			NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
//			WriteTextMessage(DEF_MSG_UMAC_ERROR);
		}
#endif

#if _MELSEC
		if (m_pView->m_pDlgExecAuto->m_bOpenComm)
		{
			nRet = m_pView->m_pDlgExecAuto->m_ActEasyIF.GetDevice(PLC_IN_COMM_PORT, &nData);
			//if (nRet==0 && nData>0) // PLC_IN_COMM_PORT 번지가 아무래도 잘못된듯하다. 오창 라미 1호기와 2호기가 그런듯.
			if(1) 
			{
				if(m_pView->m_pDlgMainBar !=NULL) 
					m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);
			}
			else
			{
				if(m_pView->m_pDlgMainBar !=NULL) 
				m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
				NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
	//			WriteTextMessage(DEF_MSG_PLC_ERROR);
			}
		}
#endif

#if _OMRON
		char szbuf[126];
		nRet = m_OmronPlc.ReceiveMessageFromPLC_Int(OMRON_IN_COMM_PORT, 0, (char *)szbuf);
		if (nRet>0) 
			if(m_pView->m_pDlgMainBar !=NULL)
				m_pView->m_pDlgMainBar->SetCommStatus(0, TRUE);
		else
		{
			if(m_pView->m_pDlgMainBar !=NULL)
				m_pView->m_pDlgMainBar->SetCommStatus(0, FALSE);
			NPio_BitWrite(PIO_OUT_READY_PORT, FALSE);
//			WriteTextMessage(DEF_MSG_PLC_ERROR);
		}
#endif
	}

	if (nIDEvent==2000)
	{
		NPio_BitWrite(PIO_OUT_COMM_PORT, m_bCOMM);
		m_bCOMM = !m_bCOMM;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////////////////////////////////////////
//////////                      NPIOCONTROL    FUNCTION                     ////////////
////////////////////////////////////////////////////////////////////////////////////////
static long    m_nInput=0;
static long    m_nOldInput[16];
BOOL CMainFrame::NPio_Open(long nPort, long nBaud, long nSize, long nParity, long nStop)
{
    BOOL bRet=TRUE;

#if _PCI7230
	m_Pci7230Daq.Open();
	m_Pci7230Daq.ReadPort(&m_nInput);
#endif

#if _PCI2416
	pIO_init();
	m_nInput = pGet_in(0);
#endif

#if _AJIN_EXT
	AxlOpen(0);
//	AxlInterruptDisable();

	AxlInterruptEnable();
	bRet = AxlIsOpened();
		
	AxdiInterruptSetModuleEnable(0, ENABLE);
	AxdiInterruptEdgeSetWord(0, 0, UP_EDGE, 0xFFFF);
	AxdiInterruptSetModule(0, NULL, NULL, (AXT_INTERRUPT_PROC)OnDIOInterruptCallback, NULL);
#endif

	m_nInput = 0;
	for(long i=0; i<DEF_DIO_PORTNUM; i++)
        m_nOldInput[i] = (m_nInput>>i) & 0x01;

	return bRet;
}

void CMainFrame::NPio_Close()
{
#if _PCI7230
	m_Pci7230Daq.Close();
#endif

#if _PCI2416
	pIO_end();
#endif

#if _AJIN_EXT
	AxlClose();
#endif
}

void CMainFrame::NPio_Write(long nData)
{
//	EnterCriticalSection(&m_csSync);

#if _PCI7230
	m_Pci7230Daq.WritePort(nData);
#endif

#if _PCI2416
    pSet_out(0, nData);
#endif

#if _AJIN_EXT
	AxdoWriteOutportWord(0, 0, (DWORD)nData);
#endif

//	LeaveCriticalSection(&m_csSync);
}

void CMainFrame::NPio_BitWrite(long nBit, BOOL bSig)
{
//	EnterCriticalSection(&m_csSync);

#if _PCI7230
	m_Pci7230Daq.SetWritePort(nBit, bSig);
#endif

#if _PCI2416
	if (bSig)
        pSet_out_bit(0, (short)nBit);
	else
        pReset_out_bit(0, (short)nBit);
#endif

#if _AJIN_EXT
	AxdoWriteOutportBit(0, nBit, (DWORD)bSig);
#endif

//	LeaveCriticalSection(&m_csSync);
}

long CMainFrame::NPio_Read()
{
//	EnterCriticalSection(&m_csSync);
    long nData=0;

#if _PCI7230
	m_Pci7230Daq.ReadPort(&nData);
#endif

#if _PCI2416
	nData = pGet_in(0);
#endif

#if _AJIN_EXT
	DWORD upValue;
	AxdiReadInportWord(0, 0, &upValue);
	nData = (long)upValue;
#endif

//	LeaveCriticalSection(&m_csSync);
	return nData;
}

BOOL CMainFrame::NPio_BitReadABS(long nBit)
{
//	EnterCriticalSection(&m_csSync);
    BOOL bRet=FALSE;

#if _PCI7230
	bRet = m_Pci7230Daq.GetReadPortABS(nBit);
#endif

#if _PCI2416
	bRet = (BOOL)pGet_in_bit(0, nBit);
#endif

#if _AJIN_EXT
	DWORD upValue;
	AxdiReadInportWord(0, 0, &upValue);
	m_nInput = (long)upValue;
	bRet = (m_nInput>>nBit) & 0x01;
#endif

//	LeaveCriticalSection(&m_csSync);
	return bRet;
}

BOOL CMainFrame::NPio_BitRead(long nBit)
{
//	EnterCriticalSection(&m_csSync);
	BOOL bRet=FALSE;
	long nTemp;

#if _PCI7230
	m_Pci7230Daq.ReadPort(&m_nInput);
#endif

#if _PCI2416
	m_nInput = pGet_in(0);
#endif

#if _AJIN_EXT
	DWORD upValue;
	AxdiReadInportWord(0, 0, &upValue);
	m_nInput = (long)upValue;
#endif

    nTemp = (m_nInput>>nBit) & 0x01;
    if (nTemp != m_nOldInput[nBit])
        if (nTemp) bRet = TRUE; 

	m_nOldInput[nBit] = nTemp;
//    Sleep(1);
//	LeaveCriticalSection(&m_csSync);
	return bRet;
}

bool bFirstFlag = false;
void __stdcall OnDIOInterruptCallback (long lActiveNo, DWORD uFlag)
{
	BOOL bRet;
	long nTemp;

//	nTemp = m_pMainFrm->NPio_Read();
	nTemp = (long)uFlag;

	// DIO 인터럽트로 레시피를 PLC Ethernet로 읽는다  
	bRet = (nTemp>>PIO_IN_READ_RECIPE) & 0x01;
	if (bRet && bFirstFlag)
	{
		m_pMainFrm->m_pView->m_pDlgExecAuto->SetTimer(1000, 300, NULL);
		//if(!m_pMainFrm->m_bExecRealTime) 
		//	m_pMainFrm->m_pView->m_pDlgExecAuto->GetRecipeUpdateProc();
			// DIO 인터럽트로 레시피를 PLC Ethernet로 읽는다  
	}
	// DIO 인터럽트로 SHORT 데이타를 RS-232C로 읽는다  
#if SHORTAGE_USE
	bRet = (nTemp>>PIO_IN_READ_SHORT) & 0x01;
	if (bRet && bFirstFlag)
		timeSetEvent(m_pMainFrm->m_Setting.m_nDelayTime, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_SHORT_DATA, TIME_ONESHOT);
	// DIO 인터럽트로 SHORT 데이타를 RS-232C로 읽는다  
#endif
	// DIO 인터럽트로 상부 카메라 영상을 촬상한다 
	bRet = (nTemp>>PIO_IN_READ_CAM1) & 0x01;
	if (bRet && bFirstFlag)
	{
		if (m_pMainFrm->m_Setting.m_bSWGrab && m_pMainFrm->m_bExecRealTime)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_UP, TRUE);
			timeSetEvent(30, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_GRAB_CAM_UP, TIME_ONESHOT);
		}
	}
	// DIO 인터럽트로 상부 카메라 영상을 촬상한다 

	// DIO 인터럽트로 하부 카메라 영상을 촬상한다 
#if 0
	bRet = (nTemp>>PIO_IN_READ_CAM2) & 0x01;
	if (bRet && bFirstFlag)
	{
		if (m_pMainFrm->m_Setting.m_bSWGrab && m_pMainFrm->m_bExecRealTime)
		{
			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_DN, TRUE);
			timeSetEvent(30, 1, EventFuncCallProc, (DWORD)(LPVOID)TIMEMSG_GRAB_CAM_DN, TIME_ONESHOT);
		}
	}

#endif // 0
	// DIO 인터럽트로 하부 카메라 영상을 촬상한다 

	bFirstFlag = true;
}

void CALLBACK EventFuncCallProc(UINT nTimeID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	switch (dwUser)
	{
		case TIMEMSG_GRAB_CAM_UP :
			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_UP, FALSE);
			break;
		case TIMEMSG_GRAB_CAM_DN :
//			m_pMainFrm->m_CAM.SetActivateTrigger(CAM_DN, FALSE);
			break;
		case TIMEMSG_PIO_INSP_CLEAR :
			m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_MISMATCH_PORT, FALSE);
			m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_MISMATCH_PORT, FALSE);
			m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_OK_PORT, FALSE);
			m_pMainFrm->NPio_BitWrite(PIO_OUT_FRONT_NG_PORT, FALSE);
			m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_OK_PORT, FALSE);
			m_pMainFrm->NPio_BitWrite(PIO_OUT_REAR_NG_PORT, FALSE);
			break;
		case TIMEMSG_COMM_ERROR :
			break;

#if SHORTAGE_USE
		case TIMEMSG_SHORT_DATA :
			m_pMainFrm->m_pView->m_pDlgTestShort->m_pDlgShortMain->SetEventCOM();
			break;  
#endif // SHORTAGE_USE

	}
}
////////////////////////////////////////////////////////////////////////////////////////
//////////                      NPIOCONTROL    FUNCTION                     ////////////
////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::UpdateBadImage()
{
	long nCamSel, nSelPnt;

	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함
	nCamSel = m_pView->m_pDlgExecAuto->m_tabImgCAM.GetCurSel();
	nSelPnt = m_pView->m_pDlgExecAuto->m_tabImage.GetCurSel();

	 m_pView->m_pDlgExecAuto->m_pDlgBadImage->UpdateFrameBadImage(nCamSel, nSelPnt);
	// 불량영상 화면에서 CAM탭을 선택시 guswo 불량 영상을 출력함
}
