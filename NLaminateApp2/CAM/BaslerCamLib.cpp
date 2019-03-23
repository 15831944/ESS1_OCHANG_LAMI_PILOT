// BaslerCamLib.cpp : implementation file
//
#include "stdafx.h"
#include "BaslerCamLib.h"
#include "..\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame    *m_pMainFrm;
/////////////////////////////////////////////////////////////////////////////
// CBaslerCamLib.
char m_szLCamSerial[10][256] = {"21156568", "21114582"};
char m_szRCamSerial[10][256] = {"21156565", "21114583"};

CBaslerCamLib::CBaslerCamLib()
{
#ifdef _BASLER_CAM
	unsigned int i;
    PylonInitialize(); 
	PylonEnumerateDevices(&m_nCamNum);

    for(i=0; i<10; i++)
		m_hDevice[i] = NULL;

	m_pGrabImage = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];
#endif
}

CBaslerCamLib::~CBaslerCamLib()
{
#ifdef _BASLER_CAM
    PylonTerminate(); 
#endif

	delete m_pGrabImage;
}

BOOL CBaslerCamLib::FindCamera()
{
	unsigned int i, j;
	PYLON_DEVICE_HANDLE hDevice;          /* Handle for the pylon device. */
	PylonDeviceInfo_t   hDeviceInfo;

	for(i=0; i<m_nCamNum; i++)
	{
		PylonCreateDeviceByIndex(i, &hDevice);
		if (hDevice>0)
		{
			PylonDeviceGetDeviceInfo(hDevice, &hDeviceInfo);					
			for(j=0; j<m_nCamNum; j++)
			{
				if (atoi(hDeviceInfo.SerialNumber) == atoi(m_szLCamSerial[j]))
				{
					m_hDevice[j] = hDevice;
					m_hDeviceInfo[j] = hDeviceInfo;
				}

				/*
				if (hDeviceInfo.SerialNumber == m_szRCamSerial[j])
				{
					m_hDevice[1] = hDevice;
					m_hDeviceInfo[1] = hDeviceInfo;
				}*/
			}
		}
	}

	return FALSE;
}

BOOL CBaslerCamLib::Open(int nCam)
{
	BOOL bRetCode=TRUE;
	int nRet;
    char buf[256];
	size_t size;

	if (m_hDevice[nCam]>0)
	{
		bRetCode = FALSE;
		nRet = PylonDeviceOpen(m_hDevice[nCam], PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM);

        // 카메라의 모델 이름, serialnumber, interface 확인
		sprintf_s(buf, "%s", "piA2400-17gm");
        size = sizeof(buf);
		nRet = PylonDeviceFeatureToString(m_hDevice[nCam], "DeviceModelName", buf, &size);
		if(nRet != 0)
			bRetCode = TRUE;		

		sprintf_s(buf, "%s", m_szSerial[nCam]);
		size = sizeof(buf);	
		if (strcmp(m_hDeviceInfo[nCam].DeviceClass, "BaslerGigE") == 0)
		{
			nRet = PylonDeviceFeatureToString(m_hDevice[nCam], "DeviceID", buf, &size);  //GigE
			if(nRet != 0)
			   bRetCode = TRUE;
		}
		
		nRet = PylonDeviceFeatureIsAvailable(m_hDevice[nCam], "EnumEntry_PixelFormat_Mono8");
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "PixelFormat", "Mono8" );
		nRet = PylonDeviceFeatureIsAvailable(m_hDevice[nCam], "EnumEntry_TriggerSelector_AcquisitionStart");
		if (nRet)
		{
			nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerSelector", "AcquisitionStart");
			nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerMode", "Off");
		}
	
		nRet = PylonDeviceFeatureIsAvailable(m_hDevice[nCam], "EnumEntry_TriggerSelector_FrameStart");
		if (nRet)
		{
			nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerSelector", "FrameStart");
     		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerMode", "Off");
		}
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam] ,"AcquisitionMode", "SingleFrame" );   


		nRet = PylonDeviceFeatureIsAvailable(m_hDevice[nCam], "GainRaw");
		if(nRet)
			nRet = PylonDeviceSetIntegerFeature(m_hDevice[nCam], "GainRaw", 0 );

		SetExposureTime(nCam, 10000);
		SetGain(nCam, 110);
	}

	return bRetCode;
}

void CBaslerCamLib::Close(int nCamNum)
{
	PylonDeviceClose(m_hDevice[nCamNum]);
	PylonDestroyDevice(m_hDevice[nCamNum]);
}

void CBaslerCamLib::SetExposureTime(int nCam, double fValue)
{
	if (fValue<4) fValue = 4;
	if (fValue>10000) fValue = 10000;

#ifdef _BASLER_CAM
	PylonDeviceFeatureFromString(m_hDevice[nCam], "ExposureMode", "Timed");
	PylonDeviceFeatureFromString(m_hDevice[nCam], "ExposureAuto", "Off");
	PylonDeviceSetIntegerFeature(m_hDevice[nCam], "ExposureTimeRaw",(int)fValue );	
	//	res = PylonDeviceSetIntegerFeature(hDevice[nCam], "ExposureTimeRaw", (int)fValue );
#endif
}

void CBaslerCamLib::ExecuteUserLoad(int nCamIndex)
{
 //   PylonDeviceExecuteCommandFeature(m_hDevice[nCamIndex], "UserSetLoad" );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   UserSet Save
/*   
   함수 설명   : Execute UserSet Save
   Parameter   : [In]카메라 인덱스                  
   return      : x
*/
void CBaslerCamLib::ExecuteUserSave(int nCamIndex)
{
 //   PylonDeviceExecuteCommandFeature(m_hDevice[nCamIndex], "UserSetSave" );
}

void CBaslerCamLib::SetGain(int nCamIndex, int nGain)
{
#ifdef _BASLER_CAM
	_Bool isAvail; 

	if (nGain<100) nGain =100;
	if (nGain>1000) nGain =1000;

	isAvail = PylonDeviceFeatureIsAvailable(m_hDevice[nCamIndex], "GainRaw");
	if(isAvail)
	{
		PylonDeviceSetIntegerFeature(m_hDevice[nCamIndex], "GainRaw", nGain );
	}
#endif
}

void CBaslerCamLib::SetGrabMode(int nCam, long nMode)
{
#ifdef _BASLER_CAM
	long nRet;

	if (nMode==0)
	{
    	nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerSelector", "FrameStart");
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerMode", "Off");        //트리거모드 켜고 끄기
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam] ,"AcquisitionMode", "SingleFrame" );   
	}
	else
	{
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerSelector", "FrameStart");
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerMode", "On");         // 트리거모드 켜고 끄기
		nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "TriggerSource", "Line1"); // 트리거모드중 소프트웨어 선택
    	nRet = PylonDeviceFeatureFromString(m_hDevice[nCam], "AcquisitionMode", "SingleFrame" );
	}
#endif
}

BOOL CBaslerCamLib::GetImageBySoftTrigger(int nCam, LPBYTE pImage, int nWidth, int nHeight)
{
	int nRet = FALSE;

#ifdef _BASLER_CAM
    _Bool bufferReady;
	PylonGrabResult_t grabResult;           /* Return value of pylon methods. */ 
	int32_t nSize;

	if (m_hDevice[nCam])
	{
		nSize = nWidth*nHeight;
		nRet = PylonDeviceGrabSingleFrame(m_hDevice[nCam], 0, pImage, nSize, &grabResult, &bufferReady, 50000);
		if (grabResult.Status != Grabbed)
			nRet = TRUE;
	}
#endif

	return nRet;
}
