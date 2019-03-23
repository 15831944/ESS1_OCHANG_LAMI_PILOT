// EuresisCamLib.cpp : implementation file
//
#include "stdafx.h"
#include "EuresisCamLib.h"
#include "..\NAppDll.h"
#include "..\NMultiFuncDll.h"
#include "..\MainFrm.h"   

#define  _CAM25M               1
//#define  _CAM12M             1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  CMainFrame *m_pMainFrm;

// 카메라#1, #2 영상획득 완료시 CALLBACK EVENT 함수 선언
void    WINAPI     GlobalCallback1(PMCSIGNALINFO SigInfo);
void    WINAPI     GlobalCallback2(PMCSIGNALINFO SigInfo);
// 카메라#1, #2 영상획득 완료시 CALLBACK EVENT 함수 선언

/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib
CEuresisCamLib::CEuresisCamLib()
{
	// Euresis 영상획득 보드 초기화 및 메모리 할당/포인터 초기화
#if _EURESIS_CAM
	m_nStatus = McOpenDriver(NULL);

	InitBoard1();
    InitBoard2();

	m_pImageT = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];

	long i;

	for(i=0; i<CAM_MAX_NUM; i++)
	    m_pImage[i] = NULL;
#endif
	// Euresis 영상획득 보드 초기화 및 메모리 할당/포인터 초기화
}

void CEuresisCamLib::InitBoard1()
{
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
#if _OCHANG_NO3
	McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology2_MONO);    	
#else	
	McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	
#endif
    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology2_MONO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "A");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

    // Choose the camera expose duration
    McSetParamInt(m_Channel_1, MC_Expose_us, 300);
   	McSetParamInt(m_Channel_1, MC_TrigLine, MC_TrigLine_IIN1);

    // Choose the number of images to acquire
    McSetParamInt(m_Channel_1, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel_1, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel_1, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel_1, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel_1, MC_SurfaceCount, 3);

    // Enable MultiCam signals
    McSetParamInt(m_Channel_1, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel_1, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // 카메라 #1의 영상획득 완료시 GlobalCallback1 함수 실행
    McRegisterCallback(m_Channel_1, GlobalCallback1, this);
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
}

void CEuresisCamLib::InitBoard2()
{
	// Euresis 영상획득 보드 카메라 #2 파라미터 설정
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    //McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	
	McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology2_MONO);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology2_MONO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// 카메라 설정 파일을 적용
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_2, MC_Connector, "B");
	// Choose the video standard
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

    // Choose the camera expose duration
    McSetParamInt(m_Channel_2, MC_Expose_us, 300);
   	McSetParamInt(m_Channel_2, MC_TrigLine, MC_TrigLine_IIN1);

    // Choose the number of images to acquire
    McSetParamInt(m_Channel_2, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel_2, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel_2, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel_2, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel_2, MC_SurfaceCount, 3);

    // Enable MultiCam signals
    McSetParamInt(m_Channel_2, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel_2, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // 카메라 #2의 영상획득 완료시 GlobalCallback1 함수 실행
    McRegisterCallback(m_Channel_2, GlobalCallback2, this);
	// Euresis 영상획득 보드 카메라 #1 파라미터 설정
}

CEuresisCamLib::~CEuresisCamLib()
{
	// 응용 프로그램 종료시 영상획득 보드 제거 및 메모리 포인터 제거
#if _EURESIS_CAM
    // Set the channel to IDLE before deleting it
    McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_IDLE);

    // Delete the channel
    McDelete(m_Channel_1);
	McDelete(m_Channel_2);

	if (m_pImageT) delete m_pImageT;
#endif
	// 응용 프로그램 종료시 영상획득 보드 제거 및 메모리 포인터 제거
}

void CEuresisCamLib::SetExposureTime(long nCamID, long nBright)
{
	// ExposureTime 설정 함수
	if (nBright<100) nBright = 100;
	if (nBright>1000) nBright = 1000;
	nBright = DEF_EXPOSURE_TIME;

	if (nCamID)
	    McSetParamInt(m_Channel_2, MC_Expose_us, nBright);
	else
		McSetParamInt(m_Channel_1, MC_Expose_us, nBright);
	// ExposureTime 설정 함수

	Sleep(1);
}

// HW, SW TRIGGER MODE 전환 함수(nCamID:카메라 선택, bHWTrigger==0:S/W Trigger, bHWTrigger==1:H/W Trigger)
// 수정요함
void CEuresisCamLib::SetTriggerMode(long nCamID, BOOL bTrigger)
{
	// TriggerMode 변환 함수
#if _EURESIS_CAM
	if (nCamID==0)
	{
		if (bTrigger==1)
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_SOFT);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_SOFT);
		}
		else if(bTrigger==2)
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_HARD);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_HARD);   
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
//			McSetParamInt(m_Channel_1, MC_TrigFilter,  MC_TrigFilter_STRONG);
		}
		else
		{
			McSetParamInt(m_Channel_1, MC_TrigMode, MC_TrigMode_IMMEDIATE);
			McSetParamInt(m_Channel_1, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
		}
	}
	else
	{
		if (bTrigger==1)
        {
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_SOFT);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_SOFT);
		}
		else if (bTrigger==2)
		{
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_HARD);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_HARD);  
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
//			McSetParamInt(m_Channel_2, MC_TrigFilter,  MC_TrigFilter_STRONG);
		}
		else
		{
			McSetParamInt(m_Channel_2, MC_TrigMode, MC_TrigMode_IMMEDIATE);
			McSetParamInt(m_Channel_2, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
		}
	}
#endif
	// TriggerMode 변환 함수

	Sleep(1);
}

void CEuresisCamLib::SetActivateTrigger(long nCamID, BOOL bFlag)
{
#if _EURESIS_CAM
	if (nCamID==0)
	{
		if (bFlag)
		{
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_ACTIVE);
			McSetParamInt(m_Channel_1, MC_ForceTrig, MC_ForceTrig_TRIG);
		}
		else
			McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_IDLE);
	}
	else
	{
		if (bFlag)
		{
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_ACTIVE);
			McSetParamInt(m_Channel_2, MC_ForceTrig, MC_ForceTrig_TRIG);
		}
		else
			McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_IDLE);
	}
#endif

	Sleep(1);
}

/////////////////////////////////////////////////////////////////////////////
// GlobalCallback
void WINAPI GlobalCallback1(PMCSIGNALINFO SigInfo)
{
	// 카메라 #1의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback1(SigInfo);
    }
#endif
	// 카메라 #1의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
}

void WINAPI GlobalCallback2(PMCSIGNALINFO SigInfo)
{
	// 카메라 #2의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback2(SigInfo);
    }
#endif
	// 카메라 #2의 영상이 획득 완료되면 EVENT로 함수가 실행이 됨 
}

/////////////////////////////////////////////////////////////////////////////
void CEuresisCamLib::Callback1(PMCSIGNALINFO SigInfo)
{
#if _EURESIS_CAM
	LPBYTE pImage;
	long nWidth, nHeight;
	char szbuf[125];

	nWidth = 0;
	nHeight = 0;

//	if (m_pImage[0]==NULL) return;

    switch(SigInfo->Signal)
    {
        case MC_SIG_SURFACE_PROCESSING:
            McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&m_pCurrent); 

			pImage = (LPBYTE)m_pCurrent;
            if (m_pMainFrm->m_bExecRealTime)
			{
				// Gauge R&R TEST
				if (m_pMainFrm->m_bGaugeRnR)
				{
					// 비전DLL에 영상을 전달
					NAppDll_SetImagePtr(0, (long)pImage);
					// 비전DLL에 영상을 전달

					NAppDll_RealTimeMode(0, FALSE);
	    			m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					NAppDll_RealTimeMode(0, TRUE);

					m_pMainFrm->m_bGaugeRnR = FALSE;
					return;
				}
				// Gauge R&R TEST

		    	if (m_pMainFrm->m_GrabCAM[CAM_UP])
				{
					// 영상이 이중으로 획득시 LOG저장
					sprintf_s(szbuf, " Double Grab(CAM=1)");
					m_pMainFrm->WriteLogToFile(szbuf);
					return;
					// 영상이 이중으로 획득시 LOG저장
				}
				else
				{
					// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함
					m_pMainFrm->m_bCriticalF = TRUE;
					m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
					// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함

					// 정상적으로 영상 획득 FLAG "TRUE" 현재시간, 검사시간 TIMER RESET함
					m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
					GetLocalTime(&m_pMainFrm->m_logStartTime);

					m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_TOTAL_INSP);
					sprintf_s(g_LOG.m_sLogTitle1, "GRAB_CAM,%d", 1);
					sprintf_s(szbuf, " Normal Grab(CAM=1)");
					m_pMainFrm->WriteLogToFile(szbuf);
					// 정상적으로 영상 획득 FLAG "TRUE" 현재시간, 검사시간 TIMER RESET함
					
#if _CAM25M
					// 비전DLL에 영상을 전달
					NAppDll_SetImagePtr(0, (long)pImage);
					if (CAM_MAX_NUM==1)
					    m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					// 비전DLL에 영상을 전달
#endif

#if _CAM5M
					nWidth = IMAGE_HEIGHT;
					nHeight = IMAGE_WIDTH;

					// 영상을 90도 회전하여 영상을 비전DLL에 영상을 전달
					NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 1);
					NAppDll_SetImagePtr(0, (long)m_pImageT);
					// 영상을 90도 회전하여 영상을 비전DLL에 영상을 전달
#endif
					// 상태 표시 LED UPDATE
					m_pMainFrm->SetCheckLED(CAM_UP, TRUE);
					m_pMainFrm->SetCheckLED(CAM_DN, FALSE);
					// 상태 표시 LED UPDATE

					m_pMainFrm->m_bCriticalF = FALSE;
    				//m_pMainFrm->ExecMessageHandleOne();
	    		}
			}
			else
			{
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
#if _CAM25M
				NAppDll_SetImagePtr(0, (long)pImage);
#endif

#if _CAM5M
				nWidth = IMAGE_HEIGHT;
				nHeight = IMAGE_WIDTH;

				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 1);
				NAppDll_SetImagePtr(0, (long)m_pImageT);
#endif
	
				m_pMainFrm->m_bCriticalF = FALSE;
		    	NAppDll_InvalidateView(0);
				// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
			}

            break;
        case MC_SIG_ACQUISITION_FAILURE:
            break;
        default:
            break;
    }
#endif
}

void CEuresisCamLib::Callback2(PMCSIGNALINFO SigInfo)
{
#if 0
#if _EURESIS_CAM
	LPBYTE pImage;
	char szbuf[125];
	long nWidth, nHeight;

	nWidth = 0;
	nHeight = 0;

	//	if (m_pImage[1]) return;
	switch(SigInfo->Signal)
	{
	case MC_SIG_SURFACE_PROCESSING:
		// Update "current" surface address pointer
		McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&m_pCurrent); 

		pImage = (LPBYTE)m_pCurrent;
		if (m_pMainFrm->m_bExecRealTime)
		{
			if (m_pMainFrm->m_bGaugeRnR)
			{
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"
				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, IMAGE_WIDTH, IMAGE_HEIGHT, 0);
				NAppDll_SetImagePtr(1, (long)m_pImageT);
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"

				NAppDll_RealTimeMode(1, FALSE);
				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(1);
				NAppDll_RealTimeMode(1, TRUE);

				m_pMainFrm->m_bGaugeRnR = FALSE;
				return;
			}

			if (m_pMainFrm->m_GrabCAM[CAM_DN])
			{
				// 영상이 이중으로 획득시 FLAG초기화 및 LOG저장
				m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
				m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;

				m_pMainFrm->SendResult2UMAC(1, 1, 1, 1, 0, 0.0f, 0, 0.0f);
				sprintf_s(szbuf, "Double Grab(CAM=2)  ON INSP ... ");
				m_pMainFrm->WriteLogToFile(szbuf);
				return;
				// 영상이 이중으로 획득시 FLAG초기화 및 LOG저장-
			}
			else
			{
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// 영상이 획득되어 검사를 시작하는 FLAG로 USER버튼이 반응을 못하도록 함

				// 정상적으로 영상 획득 FLAG "TRUE" 
				m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;

				sprintf_s(g_LOG.m_sLogTitle2, "GRAB_CAM,%d", 2);
				sprintf_s(szbuf, " Normal Grab(CAM=2)");
				m_pMainFrm->WriteLogToFile(szbuf);
				// 정상적으로 영상 획득 FLAG "TRUE" 
#if _CAM25M
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"
				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, IMAGE_WIDTH, IMAGE_HEIGHT, 0);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageT);
				// 하부 카메라 영상을 위치 변환하여 그 영상을 비전DLL로 전달 "0"
#endif

#if _CAM5M
				nWidth = IMAGE_HEIGHT;
				nHeight = IMAGE_WIDTH;

				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 2);
				NAppDll_SetImagePtr(1, (long)m_pImageT);
#endif

				// 상/하부 검사를 실행
				if (CAM_MAX_NUM==2)
				{
					m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(1);
				}
				// 상/하부 검사를 실행

				// 검사중일 때 USER버튼이 반응을 못하도록 하는 FLAG를 원래로 초기화
				m_pMainFrm->m_bCriticalF = FALSE;
				// 검사중일 때 USER버튼이 반응을 못하도록 하는 FLAG를 원래로 초기화

				//m_pMainFrm->ExecMessageHandleOne();
			}
		}
		else
		{
			// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
#if _CAM25M
			NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, IMAGE_WIDTH, IMAGE_HEIGHT, 0);
			NAppDll_SetImagePtr(1, (long)m_pImageT);
#endif

#if _CAM5M
			nWidth = IMAGE_HEIGHT;
			nHeight = IMAGE_WIDTH;

			NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 2);
			NAppDll_SetImagePtr(1, (long)m_pImageT);
#endif

			m_pMainFrm->m_bCriticalF = FALSE;
			NAppDll_InvalidateView(1);
			// GRAB, LIVE영상 획득시 비전DLL에 영상을 전달하고 화면을  Invalidate함
		}

		break;
	case MC_SIG_ACQUISITION_FAILURE:
		break;
	default:
		break;
	}
#endif

#endif // 0
}

void CEuresisCamLib::SetImagePtr(long nCam, LPBYTE pImage)
{
	m_pImage[nCam] = pImage;
}
/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib message handlers
