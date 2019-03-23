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

// ī�޶�#1, #2 ����ȹ�� �Ϸ�� CALLBACK EVENT �Լ� ����
void    WINAPI     GlobalCallback1(PMCSIGNALINFO SigInfo);
void    WINAPI     GlobalCallback2(PMCSIGNALINFO SigInfo);
// ī�޶�#1, #2 ����ȹ�� �Ϸ�� CALLBACK EVENT �Լ� ����

/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib
CEuresisCamLib::CEuresisCamLib()
{
	// Euresis ����ȹ�� ���� �ʱ�ȭ �� �޸� �Ҵ�/������ �ʱ�ȭ
#if _EURESIS_CAM
	m_nStatus = McOpenDriver(NULL);

	InitBoard1();
    InitBoard2();

	m_pImageT = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];

	long i;

	for(i=0; i<CAM_MAX_NUM; i++)
	    m_pImage[i] = NULL;
#endif
	// Euresis ����ȹ�� ���� �ʱ�ȭ �� �޸� �Ҵ�/������ �ʱ�ȭ
}

void CEuresisCamLib::InitBoard1()
{
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
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

	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology2_MONO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "M");

	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_1, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM5M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_1);
    McSetParamInt(m_Channel_1, MC_DriverIndex, 0);
	McSetParamStr(m_Channel_1, MC_Connector, "A");
	
	// ī�޶� ���� ������ ����
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

    // ī�޶� #1�� ����ȹ�� �Ϸ�� GlobalCallback1 �Լ� ����
    McRegisterCallback(m_Channel_1, GlobalCallback1, this);
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
}

void CEuresisCamLib::InitBoard2()
{
	// Euresis ����ȹ�� ���� ī�޶� #2 �Ķ���� ����
#if _CAM25M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    //McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology_MONO_DECA);	
	McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology2_MONO);	

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// ī�޶� ���� ������ ����
    McSetParamStr(m_Channel_2, MC_CamFile, CAMERA_DCF_FILE);
#endif

#if _CAM12M
	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    McSetParamInt(MC_BOARD + 1, MC_BoardTopology, MC_BoardTopology2_MONO);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel_2);
	McSetParamInt(m_Channel_2, MC_DriverIndex, 1);
	McSetParamStr(m_Channel_2, MC_Connector, "M");
	
	// ī�޶� ���� ������ ����
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

    // ī�޶� #2�� ����ȹ�� �Ϸ�� GlobalCallback1 �Լ� ����
    McRegisterCallback(m_Channel_2, GlobalCallback2, this);
	// Euresis ����ȹ�� ���� ī�޶� #1 �Ķ���� ����
}

CEuresisCamLib::~CEuresisCamLib()
{
	// ���� ���α׷� ����� ����ȹ�� ���� ���� �� �޸� ������ ����
#if _EURESIS_CAM
    // Set the channel to IDLE before deleting it
    McSetParamInt(m_Channel_1, MC_ChannelState, MC_ChannelState_IDLE);
	McSetParamInt(m_Channel_2, MC_ChannelState, MC_ChannelState_IDLE);

    // Delete the channel
    McDelete(m_Channel_1);
	McDelete(m_Channel_2);

	if (m_pImageT) delete m_pImageT;
#endif
	// ���� ���α׷� ����� ����ȹ�� ���� ���� �� �޸� ������ ����
}

void CEuresisCamLib::SetExposureTime(long nCamID, long nBright)
{
	// ExposureTime ���� �Լ�
	if (nBright<100) nBright = 100;
	if (nBright>1000) nBright = 1000;
	nBright = DEF_EXPOSURE_TIME;

	if (nCamID)
	    McSetParamInt(m_Channel_2, MC_Expose_us, nBright);
	else
		McSetParamInt(m_Channel_1, MC_Expose_us, nBright);
	// ExposureTime ���� �Լ�

	Sleep(1);
}

// HW, SW TRIGGER MODE ��ȯ �Լ�(nCamID:ī�޶� ����, bHWTrigger==0:S/W Trigger, bHWTrigger==1:H/W Trigger)
// ��������
void CEuresisCamLib::SetTriggerMode(long nCamID, BOOL bTrigger)
{
	// TriggerMode ��ȯ �Լ�
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
	// TriggerMode ��ȯ �Լ�

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
	// ī�޶� #1�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback1(SigInfo);
    }
#endif
	// ī�޶� #1�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
}

void WINAPI GlobalCallback2(PMCSIGNALINFO SigInfo)
{
	// ī�޶� #2�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
#if _EURESIS_CAM
    if (SigInfo && SigInfo->Context)
    {
        CEuresisCamLib* pCamLib = (CEuresisCamLib*) SigInfo->Context;
        pCamLib->Callback2(SigInfo);
    }
#endif
	// ī�޶� #2�� ������ ȹ�� �Ϸ�Ǹ� EVENT�� �Լ��� ������ �� 
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
					// ����DLL�� ������ ����
					NAppDll_SetImagePtr(0, (long)pImage);
					// ����DLL�� ������ ����

					NAppDll_RealTimeMode(0, FALSE);
	    			m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					NAppDll_RealTimeMode(0, TRUE);

					m_pMainFrm->m_bGaugeRnR = FALSE;
					return;
				}
				// Gauge R&R TEST

		    	if (m_pMainFrm->m_GrabCAM[CAM_UP])
				{
					// ������ �������� ȹ��� LOG����
					sprintf_s(szbuf, " Double Grab(CAM=1)");
					m_pMainFrm->WriteLogToFile(szbuf);
					return;
					// ������ �������� ȹ��� LOG����
				}
				else
				{
					// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��
					m_pMainFrm->m_bCriticalF = TRUE;
					m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
					// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��

					// ���������� ���� ȹ�� FLAG "TRUE" ����ð�, �˻�ð� TIMER RESET��
					m_pMainFrm->m_GrabCAM[CAM_UP] = TRUE;
					GetLocalTime(&m_pMainFrm->m_logStartTime);

					m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_TOTAL_INSP);
					sprintf_s(g_LOG.m_sLogTitle1, "GRAB_CAM,%d", 1);
					sprintf_s(szbuf, " Normal Grab(CAM=1)");
					m_pMainFrm->WriteLogToFile(szbuf);
					// ���������� ���� ȹ�� FLAG "TRUE" ����ð�, �˻�ð� TIMER RESET��
					
#if _CAM25M
					// ����DLL�� ������ ����
					NAppDll_SetImagePtr(0, (long)pImage);
					if (CAM_MAX_NUM==1)
					    m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					// ����DLL�� ������ ����
#endif

#if _CAM5M
					nWidth = IMAGE_HEIGHT;
					nHeight = IMAGE_WIDTH;

					// ������ 90�� ȸ���Ͽ� ������ ����DLL�� ������ ����
					NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 1);
					NAppDll_SetImagePtr(0, (long)m_pImageT);
					// ������ 90�� ȸ���Ͽ� ������ ����DLL�� ������ ����
#endif
					// ���� ǥ�� LED UPDATE
					m_pMainFrm->SetCheckLED(CAM_UP, TRUE);
					m_pMainFrm->SetCheckLED(CAM_DN, FALSE);
					// ���� ǥ�� LED UPDATE

					m_pMainFrm->m_bCriticalF = FALSE;
    				//m_pMainFrm->ExecMessageHandleOne();
	    		}
			}
			else
			{
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
				// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"
				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, IMAGE_WIDTH, IMAGE_HEIGHT, 0);
				NAppDll_SetImagePtr(1, (long)m_pImageT);
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"

				NAppDll_RealTimeMode(1, FALSE);
				m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(1);
				NAppDll_RealTimeMode(1, TRUE);

				m_pMainFrm->m_bGaugeRnR = FALSE;
				return;
			}

			if (m_pMainFrm->m_GrabCAM[CAM_DN])
			{
				// ������ �������� ȹ��� FLAG�ʱ�ȭ �� LOG����
				m_pMainFrm->m_GrabCAM[CAM_UP] = FALSE;
				m_pMainFrm->m_GrabCAM[CAM_DN] = FALSE;

				m_pMainFrm->SendResult2UMAC(1, 1, 1, 1, 0, 0.0f, 0, 0.0f);
				sprintf_s(szbuf, "Double Grab(CAM=2)  ON INSP ... ");
				m_pMainFrm->WriteLogToFile(szbuf);
				return;
				// ������ �������� ȹ��� FLAG�ʱ�ȭ �� LOG����-
			}
			else
			{
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��
				m_pMainFrm->m_bCriticalF = TRUE;
				m_pMainFrm->m_ctrlTimer.SetClockTimer(TIMER_ON_PROCESS);
				// ������ ȹ��Ǿ� �˻縦 �����ϴ� FLAG�� USER��ư�� ������ ���ϵ��� ��

				// ���������� ���� ȹ�� FLAG "TRUE" 
				m_pMainFrm->m_GrabCAM[CAM_DN] = TRUE;

				sprintf_s(g_LOG.m_sLogTitle2, "GRAB_CAM,%d", 2);
				sprintf_s(szbuf, " Normal Grab(CAM=2)");
				m_pMainFrm->WriteLogToFile(szbuf);
				// ���������� ���� ȹ�� FLAG "TRUE" 
#if _CAM25M
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"
				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, IMAGE_WIDTH, IMAGE_HEIGHT, 0);
				NAppDll_SetImagePtr(CAM_DN, (long)m_pImageT);
				// �Ϻ� ī�޶� ������ ��ġ ��ȯ�Ͽ� �� ������ ����DLL�� ���� "0"
#endif

#if _CAM5M
				nWidth = IMAGE_HEIGHT;
				nHeight = IMAGE_WIDTH;

				NMultiFuncDll_Trans90ConvertImage(pImage, IMAGE_WIDTH, IMAGE_HEIGHT, m_pImageT, nWidth, nHeight, 2);
				NAppDll_SetImagePtr(1, (long)m_pImageT);
#endif

				// ��/�Ϻ� �˻縦 ����
				if (CAM_MAX_NUM==2)
				{
					m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(0);
					m_pMainFrm->m_pView->m_pDlgExecAuto->InspectProgressGrab(1);
				}
				// ��/�Ϻ� �˻縦 ����

				// �˻����� �� USER��ư�� ������ ���ϵ��� �ϴ� FLAG�� ������ �ʱ�ȭ
				m_pMainFrm->m_bCriticalF = FALSE;
				// �˻����� �� USER��ư�� ������ ���ϵ��� �ϴ� FLAG�� ������ �ʱ�ȭ

				//m_pMainFrm->ExecMessageHandleOne();
			}
		}
		else
		{
			// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
			// GRAB, LIVE���� ȹ��� ����DLL�� ������ �����ϰ� ȭ����  Invalidate��
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
