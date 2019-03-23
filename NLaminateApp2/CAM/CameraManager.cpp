// CameraManager.cpp : implementation file
//

#include "stdafx.h"
#include "CameraManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHECK( errc ) if ( GENAPI_E_OK != errc ) printErrorAndExit( errc )
/* Register the callback function. */
//void _stdcall removalCallbackFunction(PYLON_DEVICE_HANDLE hDevice );
CCameraManager *pCameraManager;

/////////////////////////////////////////////////////////////////////////////

/* Register the callback function. */
void _stdcall removalCallbackFunction(PYLON_DEVICE_HANDLE hDevice )
{
		if(hDevice==pCameraManager->hDevice[0])
		{
			pCameraManager->m_bCameraDisConnectFlag[0] = false;
			pCameraManager->m_bStopLiveThread[0]=false;							
			pCameraManager->hDevice[0] = NULL; 									
		}
		if(hDevice==pCameraManager->hDevice[1])
		{
		pCameraManager->m_bCameraDisConnectFlag[1] = false;
		pCameraManager->m_bStopLiveThread[1]=false;							
		pCameraManager->hDevice[1] = NULL; 									
		}

}


UINT LiveThreadCam0(LPVOID pParam)
{        
    PylonStreamGrabberGetWaitObject( pCameraManager->hGrabber[0], &pCameraManager->hWait[0] );
	PylonDeviceFeatureFromString( pCameraManager->hDevice[0], "AcquisitionMode", "Continuous" );
    for (int i = 0; i < NUM_BUFFERS; ++i )
    {
        PylonStreamGrabberRegisterBuffer( pCameraManager->hGrabber[0], pCameraManager->pImageGetbuf[0][i], pCameraManager->m_iCMPayloadSize[0],  &pCameraManager->bufHandles[i] );        
        PylonStreamGrabberQueueBuffer( pCameraManager->hGrabber[0], pCameraManager->bufHandles[i], (void*) i  );
    }
	
	PylonDeviceExecuteCommandFeature( pCameraManager->hDevice[0], "AcquisitionStart");
    _Bool  isReady;        
    PylonGrabResult_t         grabResult;     
   	int bufferIndex;   
    pCameraManager->m_bCaptureEnd[0] = false;
    while(pCameraManager->m_bStopLiveThread[0])  
	{
		   PylonWaitObjectWait( pCameraManager->hWait[0], 1000, &isReady );
           PylonStreamGrabberRetrieveResult( pCameraManager->hGrabber[0], &grabResult, &isReady );
		   if(isReady)
		   {
			   if ( grabResult.Status == Grabbed  )
			   {
				   
				   bufferIndex = (int) grabResult.Context;
				   pCameraManager->m_bCaptureEnd[0] = true;	 // 캡쳐 end flag 변경												      				   
			   }		
			   else
			   {
				   TRACE("Grab Fail");
			   }
			   if(pCameraManager->m_bStopFlag[0]==false)
			   {
					pCameraManager->m_bStopLiveThread[0]=false;							   
			   }
		   }
		   PylonStreamGrabberQueueBuffer( pCameraManager->hGrabber[0], grabResult.hBuffer, (void*) bufferIndex );
	}
	if(pCameraManager->hDevice[0]!=NULL)
	{
		// Grab Finish
		PylonDeviceExecuteCommandFeature( pCameraManager->hDevice[0], "AcquisitionStop");
		PylonStreamGrabberCancelGrab(  pCameraManager->hGrabber[0] );
		do 
		{
			PylonStreamGrabberRetrieveResult( pCameraManager->hGrabber[0], &grabResult, &isReady );
		} while ( isReady );


		for (i = 0; i < NUM_BUFFERS; i++ )   
		{
			PylonStreamGrabberDeregisterBuffer(  pCameraManager->hGrabber[0], pCameraManager->bufHandles[i] );              
		}
	}

	return 0;
}
UINT LiveThreadCam1(LPVOID pParam)
{        
    PylonStreamGrabberGetWaitObject( pCameraManager->hGrabber[1], &pCameraManager->hWait[1] );
	PylonDeviceFeatureFromString( pCameraManager->hDevice[1], "AcquisitionMode", "Continuous" );
    for (int i = 0; i < NUM_BUFFERS; ++i )
    {
        PylonStreamGrabberRegisterBuffer( pCameraManager->hGrabber[1], pCameraManager->pImageGetbuf[1][i], pCameraManager->m_iCMPayloadSize[1],  &pCameraManager->bufHandles[i] );        
        PylonStreamGrabberQueueBuffer( pCameraManager->hGrabber[1], pCameraManager->bufHandles[i], (void*) i  );
    }
	
	PylonDeviceExecuteCommandFeature( pCameraManager->hDevice[1], "AcquisitionStart");
    _Bool  isReady;        
    PylonGrabResult_t         grabResult;     
   	int bufferIndex;   
    pCameraManager->m_bCaptureEnd[1] = false;
    while(pCameraManager->m_bStopLiveThread[1])  
	{
		   PylonWaitObjectWait( pCameraManager->hWait[1], 1000, &isReady );
           PylonStreamGrabberRetrieveResult( pCameraManager->hGrabber[1], &grabResult, &isReady );
		   if(isReady)
		   {
			   if ( grabResult.Status == Grabbed  )
			   {
				   
				   bufferIndex = (int) grabResult.Context;
				   pCameraManager->m_bCaptureEnd[1] = true;	 // 캡쳐 end flag 변경												      				   
			   }		
			   else
			   {
				   TRACE("Grab Fail");
			   }
			   if(pCameraManager->m_bStopFlag[1]==false)
			   {
					pCameraManager->m_bStopLiveThread[1]=false;							   
			   }
		   }
		   PylonStreamGrabberQueueBuffer( pCameraManager->hGrabber[1], grabResult.hBuffer, (void*) bufferIndex );
	}
	if(pCameraManager->hDevice[1]!=NULL)
	{
		// Grab Finish
		PylonDeviceExecuteCommandFeature( pCameraManager->hDevice[1], "AcquisitionStop");
		PylonStreamGrabberCancelGrab(  pCameraManager->hGrabber[1] );
		do 
		{
			PylonStreamGrabberRetrieveResult( pCameraManager->hGrabber[1], &grabResult, &isReady );
		} while ( isReady );


		for (i = 0; i < NUM_BUFFERS; i++ )   
		{
			PylonStreamGrabberDeregisterBuffer(  pCameraManager->hGrabber[1], pCameraManager->bufHandles[i] );              
		}
	}

	return 0;
}
// CCameraManager

IMPLEMENT_DYNCREATE(CCameraManager, CView)

CCameraManager::CCameraManager()
{
	PylonInitialize(); 
	pCameraManager = this;		
}

CCameraManager::~CCameraManager()
{

	PylonTerminate();
}


BEGIN_MESSAGE_MAP(CCameraManager, CView)
	//{{AFX_MSG_MAP(CCameraManager)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraManager drawing

void CCameraManager::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCameraManager diagnostics

#ifdef _DEBUG
void CCameraManager::AssertValid() const
{
	CView::AssertValid();
}

void CCameraManager::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/*
   함수 설명   : 시스템에 연결된 카메라 찾기 ( find all cameras in Network)
   Parameter   : [Out] 카메라 이름(camera nme),[Out] Serial number,[Out] InterFace,  [Out]연결된 카메라 수(camera number)
   return      : bool
*/
bool CCameraManager::FindCamera(int *nCamNumber)
{
    char buf[256], szCamSerialNumber[256], szInterfacName[256];

	res = PylonEnumerateDevices( &numDevices );
	if(res!=0)
	{
		CHECK(res);
		return false;
	}
	else
	{
		*nCamNumber = numDevices;      //  시스템에 연결된 카메라 수
		if(numDevices == 0 )
		{
			return false;
			//PylonTerminate();   
		}
		for( int i=0; i<(int)numDevices; i++)
		{
			res = PylonCreateDeviceByIndex( i, &hDevice[i] );   
			if( res!=0)
			{
				CHECK(res);				
			}
			else
			{
				res = PylonDeviceOpen( hDevice[i], PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );
				if(res!=0)
				{
               
				   CHECK(res);				

				}
				else
				{	
					// 카메라의 모델 이름, serialnumber, interface 확인
					sprintf(buf, "%s", "acA640-100gm");
                    size_t siz = sizeof(buf);
					res = PylonDeviceFeatureToString( hDevice[i], "DeviceModelName", buf, &siz);
					if(res!=0)
					{
						CHECK(res);				
					}

					sprintf(szCamSerialNumber, "%s", "21110439");
					siz = sizeof(szCamSerialNumber);	
                                  
					res = PylonDeviceGetDeviceInfo( hDevice[i], &hDeviceInfo[i]);					
					if(res!=0)
					{
						CHECK(res);				
					}
					if (strcmp(hDeviceInfo[i].DeviceClass, "BaslerGigE") == 0)
					{
						res = PylonDeviceFeatureToString( hDevice[i], "DeviceID", szCamSerialNumber,&siz);  //GigE
						if(res!=0)
						{
							CHECK(res);				
						}
					}
			
					sprintf(szCamSerialNumber, "%s", "Areascan");
					siz = sizeof(szInterfacName);
					res = PylonDeviceFeatureToString( hDevice[i], "DeviceScanType", szInterfacName,&siz );
					if(res!=0)
					{
						CHECK(res);				
					}
					res = PylonDeviceClose( hDevice[i] );
					if(res!=0)
					{
						CHECK(res);				
					}
					res = PylonDestroyDevice ( hDevice[i] ); 
					if(res!=0)
					{
						CHECK(res);				
					}
				}
			}
		}
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

/*
   함수 설명   : Camera Open & Stream grabber Open
   Parameter   : [In]카메라 인덱스 (camera index), [In] 카메라 position (camera position)
   return      : bool
   Camera 추가시에는 case 문 확장 가능 
*/
bool CCameraManager::OpenCamera(int nCamIndex, int nPosition)
{
		switch(nPosition)
		{
		   case 1:
			   res = PylonCreateDeviceByIndex( 0, &hDevice[nCamIndex] );  
			   break;
		   case 2:
			   res = PylonCreateDeviceByIndex( 1, &hDevice[nCamIndex] );  
			   break;
		}
		if(res!=0)
		{
			CHECK(res);	
			return false;
		}
        res = PylonDeviceOpen( hDevice[nCamIndex], PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );
		if(res!=0)
		{
			CHECK(res);	
			return false;
		}


		/* Register the callback function. */
//		res = PylonDeviceRegisterRemovalCallback( hDevice[nCamIndex], removalCallbackFunction, &hCb[nCamIndex] );
//		if(res!=0)
//		{
//		   CHECK(res);
//		   return false;
//		}
	    size_t nStreams;   
        res = PylonDeviceGetNumStreamGrabberChannels( hDevice[nCamIndex], &nStreams );       
		if(res!=0)
		{
			CHECK(res);	
			return false;
		}
		else
		{
			if ( nStreams < 1 )
			{
				TRACE("The transport layer doesn't support image streams\n");
				PylonTerminate();
				exit(EXIT_FAILURE);
				return false;
			}
		}
		res = PylonDeviceGetStreamGrabber( hDevice[nCamIndex], 0, &hGrabber[nCamIndex] );		
		if(res!=0)
		{
			CHECK(res);	
			return false;
		}
		res = PylonStreamGrabberOpen( hGrabber[nCamIndex] );
		if(res!=0)
		{
			CHECK(res);	
			return false;
		}
		return true;
}
/* 
	함수 설명   : Camera & Stream grabber close
	Parameter   : [In]카메라 인덱스
	return      : bool   
*/
bool CCameraManager::CloseCamera(int nCamIndex)
{
	res = PylonStreamGrabberClose( hGrabber[nCamIndex] );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
//	res = PylonDeviceDeregisterRemovalCallback( hDevice[nCamIndex] , hCb[nCamIndex] );
//	if(res!=0)
//	{
//			CHECK(res);	
//			return false;
//	}

	res = PylonDeviceClose( hDevice[nCamIndex] );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}

	res = PylonDestroyDevice ( hDevice[nCamIndex] ); 
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}

	return true;
}
/*
   함수 설명   : 카메라 연결 및 버퍼 alloc를 한다.
   Parameter   : [In] 카메라 인덱스, [In] noffsetx, [In] noffsety,[In] nwidth, [In] nheight
   return      : bool      
*/
bool CCameraManager::CameraConnect(int nCamIndex, int nOffsetX, int nOffsetY, int nWidth, int nHeight)
{
	/*
    SetOffsetX(nCamIndex,nOffsetX);
	SetOffsetY(nCamIndex,nOffsetY);*/
	SetHeight(nCamIndex, nHeight);
	SetWidth(nCamIndex, nWidth);

    m_iCM_reSizeWidth[nCamIndex] = (((nWidth*8)+31)/32*4);  // width 4byte 배수계산
	if (strcmp(hDeviceInfo[nCamIndex].DeviceClass, "BaslerGigE") == 0)
	{
        res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "GevSCPSPacketSize", 7000 );  // GigE
	}

/*
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}*/

	res = PylonDeviceGetIntegerFeatureInt32( hDevice[nCamIndex], "PayloadSize", &m_iCMPayloadSize[nCamIndex]);
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
	res = PylonStreamGrabberSetMaxNumBuffer( hGrabber[nCamIndex], NUM_BUFFERS );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
	res = PylonStreamGrabberSetMaxBufferSize( hGrabber[nCamIndex], m_iCMPayloadSize[nCamIndex] );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
	res = PylonStreamGrabberPrepareGrab( hGrabber[nCamIndex] );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
	m_bCameraDisConnectFlag[nCamIndex] = true;
    return true;
}

/*
	함수 설명   : cameradisconnect
	Parameter   : [In]카메라 인덱스 
	return      : bool
*/
bool CCameraManager::DisConnectCamera(int nCamIndex)
{
	res = PylonStreamGrabberFinishGrab( hGrabber[nCamIndex] );
	if(res!=0)
	{
			CHECK(res);	
			return false;
	}
	return true;

}
/**********************************************************************************/
/*							Analog  Controls								      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Gain Auto
/*
	함수 설명   : Get Gain Auto
	Parameter   : [In] int nCamIndex , [Out] char *szGainAuto
	               Off, Once, Continuous
	return      : x         
*/
void CCameraManager::GetGainAuto(int nCamIndex, char *szGainAuto)   
{
	_Bool isAvail;      
    isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "GainAuto");
	if(isAvail)
	{
		char buf[50];
		size_t siz = sizeof(buf);
		res = PylonDeviceFeatureToString( hDevice[nCamIndex], "GainAuto", buf ,&siz);
		if(res!=0)
		{
			CHECK(res);	
				
		}
		else
		{
		    lstrcpy(szGainAuto,buf);
		}
	}
	else
	{
		//TRACE("The GainAuto feature do not support!!");
		AfxMessageBox("The GainAuto feature do not support!!");
	}
}

/*
	함수 설명   : Set Gain Auto
	Parameter   : [In]int nCamIndex , [In] CString strGainAuto
	              Off, Once, Continuous
	return      : x         
*/
void CCameraManager::SetGainAuto(int nCamIndex, CString strGainAuto)
{
	CString temp;
	_Bool isAvail;      
	if(strGainAuto == "Off")
	{
		temp= "EnumEntry_GainAuto_Off";
	}
	else  if(strGainAuto == "Once")
	{
		temp= "EnumEntry_GainAuto_Once";
	}
	else
	{
		temp= "EnumEntry_GainAuto_Continuous";
	}
    isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], temp);
	if(isAvail)
	{
		res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "GainAuto", strGainAuto);
		if(res!=0)
		{
			CHECK(res);					
		}
	}
	else
	{
		//TRACE("The GainAuto feature do not support!!");
		AfxMessageBox("The GainAuto feature do not support!!");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Gain Selector
/*
	함수 설명   : Get Gain Selector
	Parameter   : [In] int nCamIndex , [Out] char *szGainSelector
	              All , Tap1 , Tap2
	return      : x         
*/
void CCameraManager::GetGainSelector(int nCamIndex, char *szGainSelector) 
{
	char buf[50];
	size_t siz = sizeof(buf);
	
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "GainSelector");
	if(isAvail)
	{
		res = PylonDeviceFeatureToString( hDevice[nCamIndex], "GainSelector", buf ,&siz);
		if(res!=0)
		{
			CHECK(res);				
		}
		lstrcpy(szGainSelector,buf);	
	}
	else
	{
		//TRACE("The Gain Selector feature do not support!!");
		AfxMessageBox("The Gain Selector feature do not support!!");
	}
}
/*
	함수 설명   : Get Gain Selector
	Parameter   : [In] int nCamIndex , [In] CString strGainSelector
	              All , Tap1 , Tap2
	return      : x         
*/
void CCameraManager::SetGainSelector(int nCamIndex, CString strGainSelector)
{
	CString temp;
	_Bool isAvail; 
	if(strGainSelector=="All")
	{
		temp = "EnumEntry_GainSelector_All";
	}
	else if(strGainSelector=="Tap1")
	{
		temp = "EnumEntry_GainSelector_Tap1";
	}
	else if(strGainSelector=="Tap2")
	{
        temp = "EnumEntry_GainSelector_Tap2";
	}

	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], temp);
	if(isAvail)
	{
		res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "GainSelector", strGainSelector);
		if(res!=0)
		{
			CHECK(res);				
		}
	}
	else
	{
		//TRACE("The Gain Selector feature do not support!!");
		AfxMessageBox("The Gain Selector feature do not support!!");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    Gain Raw
/*
	함수 설명   : Get Gain Raw
	Parameter   : [In] int nCamIndex , [Out] int *nMinGain, int *nGain , int *nMaxGain	             
	return      : x         
*/
void CCameraManager::GetGain(int nCamIndex, int *nMinGain, int *nGain , int *nMaxGain)
{
	int64_t ntemp;
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "GainRaw");
	if(isAvail)
	{
		res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "GainRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);			
		}
		*nGain = (int)ntemp;

		res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "GainRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);					
		}
		*nMinGain = (int)ntemp;

		res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "GainRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);					
		}
		*nMaxGain = (int)ntemp;
	}
	else
	{
		//TRACE("The GainRaw feature do not support!!");
		AfxMessageBox("The GainRaw feature do not support!!");
	}
}
/*
	함수 설명   : Set Gain Raw
	Parameter   : [In] int nCamIndex , [In] int nGain
	return      : x         
*/
void CCameraManager::SetGain(int nCamIndex, int nGain)
{
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "GainRaw");
	if(isAvail)
	{
		res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "GainRaw", nGain );
		if(res!=0)
		{
			CHECK(res);				
		}
	}
	else
	{
		//TRACE("The GainRaw feature do not support!!");
		AfxMessageBox("The GainRaw feature do not support!!");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Black Level Selector
/*
	함수 설명   : Get Black Level Selector
	Parameter   : [In] int nCamIndex , [Out] char *szBlackLevelSelector
	              All , Tap1 , Tap2
	return      : x         
*/
void CCameraManager::GetBlackLevelSelector(int nCamIndex, char *szBlackLevelSelector)  
{
	char buf[50];
	size_t siz = sizeof(buf);
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BlackLevelSelector");
	if(isAvail)
	{
		res = PylonDeviceFeatureToString( hDevice[nCamIndex], "BlackLevelSelector", buf ,&siz);
		if(res!=0)
		{
			CHECK(res);	
				
		}
		lstrcpy(szBlackLevelSelector,buf);
	}
	else
	{
		//TRACE("The BlackLevelSelector feature do not support!!");
		AfxMessageBox("The BlackLevelSelector feature do not support!!");
	}
}
/*
	함수 설명   : Set Black Level Selector
	Parameter   : [In] int nCamIndex , [In] CString strBlackLevelSelector
	              All , Tap1 , Tap2
	return      : x         
*/
void CCameraManager::SetBlackLevelSelector(int nCamIndex, CString strBlackLevelSelector)
{
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BlackLevelSelector");
	if(isAvail)
	{
		res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "BlackLevelSelector", strBlackLevelSelector);
		if(res!=0)
		{
			CHECK(res);					
		}
	}
	else
	{
		//TRACE("The BlackLevelSelector feature do not support!!");
		AfxMessageBox("The BlackLevelSelector feature do not support!!");
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Black Level Raw

/*
	함수 설명   : Get Black Level Raw
	Parameter   : [In] int nCamIndex , [Out] int *nMinBlackLevel, int *nBlackLevel , int *nMaxBlackLevel	             
	return      : x         
*/
void CCameraManager::GetBlackLevel(int nCamIndex, int *nMinBlackLevel, int *nBlackLevel , int *nMaxBlackLevel)
{
	int64_t ntemp;
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BlackLevelRaw");
	if(isAvail)
	{
		res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "BlackLevelRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);				
		}
		*nBlackLevel = (int)ntemp;

		res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "BlackLevelRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);				
		}
		*nMinBlackLevel = (int)ntemp;

		res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "BlackLevelRaw", &ntemp );	
		if(res!=0)
		{
			CHECK(res);			
		}
		*nMaxBlackLevel = (int)ntemp;
	}
	else
	{
		//TRACE("The BlackLevelRaw feature do not support!!");
		AfxMessageBox("The BlackLevelRaw feature do not support!!");

	}
}
/*
	함수 설명   : Set Black Level Raw
	Parameter   : [In] int nCamIndex , [In] int nBlackLevel	             
	return      : x         
*/
void CCameraManager::SetBlackLevel(int nCamIndex, int nBlackLevel)
{
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BlackLevelRaw");
	if(isAvail)
	{
		res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "BlackLevelRaw", nBlackLevel );
		if(res!=0)
		{
			CHECK(res);			
		}
	}
	else
	{
		//TRACE("The BlackLevelRaw feature do not support!!");
		AfxMessageBox("The BlackLevelRaw feature do not support!!");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Balance White Auto
/*
	함수 설명   : Get Balance White Auto
	Parameter   : [In] int nCamIndex , [Out] char *szWhiteBalAuto	      
	              Off , Once
	return      : x         
*/
void CCameraManager::GetWhiteBalAuto(int nCamIndex, char *szWhiteBalAuto) 
{
	char buf[50];
	size_t siz = sizeof(buf);
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BalanceWhiteAuto");
	if(isAvail)
	{
		res = PylonDeviceFeatureToString( hDevice[nCamIndex], "BalanceWhiteAuto", buf ,&siz);
		if(res!=0)
		{
			CHECK(res);				
		}
		lstrcpy(szWhiteBalAuto,buf);
	}
	else
	{
		//TRACE("The BalanceWhiteAuto feature do not support!!");
		AfxMessageBox("The BalanceWhiteAuto feature do not support!!");
	}
}
/*
	함수 설명   : Set Balance White Auto
	Parameter   : [In] int nCamIndex , [In] CString strWhiteBalAuto            
	              Off , Once
	return      : x         
*/
void CCameraManager::SetWhiteBalAuto(int nCamIndex, CString  strWhiteBalAuto)
{
	_Bool isAvail; 
	isAvail = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "BalanceWhiteAuto");
	if(isAvail)
	{
		res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "BalanceWhiteAuto", strWhiteBalAuto);
		if(res!=0)
		{
			CHECK(res);				
		}
	}
	else
	{
		//TRACE("The BalanceWhiteAuto feature do not support!!");
		AfxMessageBox("The BalanceWhiteAuto feature do not support!!");

	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Balance Ratio Selector

/*
	함수 설명   : Get ColorBalance Selector
	Parameter   : [In] int nCamIndex , [Out] char *szColorBalSelector	      
	              Red , Green , Blue
	return      : x         
*/
void CCameraManager::GetColorBalSelector(int nCamIndex, char *szColorBalSelector) 
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "BalanceRatioSelector", buf ,&siz);
	if(res!=0)
	{
		CHECK(res);				
	}
	lstrcpy(szColorBalSelector,buf);
}
/*
	함수 설명   : Set ColorBalance Selector
	Parameter   : [In] int nCamIndex , [In] CString  strColorBalSelector	      
	              Red , Green , Blue
	return      : x         
*/
void CCameraManager::SetColorBalSelector(int nCamIndex, CString  strColorBalSelector)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "BalanceRatioSelector", strColorBalSelector);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Balance Ratio (Raw)
/*
	함수 설명   : Get Color Raw
	Parameter   : [In] int nCamIndex , [Out] int *nMinColorRaw, int *nColorRaw, int *nMaxColorRaw	             
	return      : x         
*/
void CCameraManager::GetColorRaw(int nCamIndex, int *nMinColorRaw, int *nColorRaw, int *nMaxColorRaw)
{
	int64_t ntemp;
	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "BalanceRatioRaw", &ntemp );	
	*nColorRaw = (int)ntemp;

	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "BalanceRatioRaw", &ntemp );	
	*nMinColorRaw = (int)ntemp;

	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "BalanceRatioRaw", &ntemp );	
	*nMaxColorRaw = (int)ntemp;
}
/*
	함수 설명   : Set Color Raw
	Parameter   : [In] int nCamIndex , [In] int nColorRaw	             
	return      : x         
*/
void CCameraManager::SetColorRaw(int nCamIndex, int nColorRaw)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "BalanceRatioRaw", nColorRaw );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Gamma Enable

/*
	함수 설명   : Get Gamma Enable
	Parameter   : [In] int nCamIndex , [Out] bool *bGammaEnable	             
	              0(false) , 1(true)
	return      : x         
*/
void CCameraManager::GetGammaEnable(int nCamIndex, bool *bGammaEnable) 
{
    _Bool bTemp;
	res = PylonDeviceGetBooleanFeature(hDevice[nCamIndex], "GammaEnable", &bTemp);	
	if(bTemp==TRUE)	
        *bGammaEnable = true;
	else
		*bGammaEnable = false;		 
}
/*
	함수 설명   : Set Gamma Enable
	Parameter   : [In] int nCamIndex , [In] bool bGammaEnable	             
	              0(false) , 1(true)
	return      : x         
*/
void CCameraManager::SetGammaEnable(int nCamIndex, bool bGammaEnable)
{
	res = PylonDeviceSetBooleanFeature(hDevice[nCamIndex], "GammaEnable",bGammaEnable);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Gamma

/*
	함수 설명   : Get Gamma 
	Parameter   : [In] int nCamIndex , [Out] double *dMinGammaEnable, double *dGammaEnable, double *dMaxGammaEnable	             	          
	return      : x         
*/
void CCameraManager::GetGamma(int nCamIndex, double *dMinGammaEnable, double *dGammaEnable, double *dMaxGammaEnable)
{
	res = PylonDeviceGetFloatFeature( hDevice[nCamIndex], "Gamma", dGammaEnable );	
	res = PylonDeviceGetFloatFeatureMin( hDevice[nCamIndex], "Gamma", dMinGammaEnable );	
	res = PylonDeviceGetFloatFeatureMax( hDevice[nCamIndex], "Gamma", dMaxGammaEnable );	
}
/*
	함수 설명   : Set Gamma 
	Parameter   : [In] int nCamIndex , [In] double dGammaEnable	             	          
	return      : x         
*/
void CCameraManager::SetGamma(int nCamIndex, double dGammaEnable)
{
	 res = PylonDeviceSetFloatFeature( hDevice[nCamIndex], "Gamma", dGammaEnable );	
}

/**********************************************************************************/
/*							Image Format Controls  							      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Pixel Format
/*
	함수 설명   : Get Pixel Format 
	Parameter   : [In]카메라 인덱스 [Out] char *szImgFormat 
	              Mono8 , Mono 16 , Mono12Packed ,BayerBG8 , BayerRG8 ,BayerGB8 , YUV422Packed , BayerBG16
	return      :  bool         
*/
void CCameraManager::GetImgFormat(int nCamIndex, char *szImgFormat)
{
	char buf[50];
	size_t siz = sizeof(buf);	
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "PixelFormat", buf ,&siz);
	lstrcpy(szImgFormat,buf);
}
/*
	함수 설명   : Set Pixel Format 
	Parameter   : [In]카메라 인덱스 [In] CString strImageFormat 
	              Mono8 , Mono 16 , Mono12Packed ,BayerBG8 , BayerRG8 ,BayerGB8 , YUV422Packed , BayerBG16
	return      :  bool         
*/
void CCameraManager::SetImgFormat(int nCamIndex, CString strImageFormat)
{
	PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "EnumEntry_PixelFormat_Mono8");
	res = PylonDeviceFeatureFromString(hDevice[nCamIndex], "PixelFormat", "Mono8" );
	if(res!=0)
	{
	    CHECK(res);
	}

	res = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "EnumEntry_TriggerSelector_AcquisitionStart");
	if(res!=0)
	{
		res = PylonDeviceFeatureFromString(hDevice[nCamIndex], "TriggerSelector", "AcquisitionStart");
		res = PylonDeviceFeatureFromString(hDevice[nCamIndex], "TriggerMode", "Off");
	}

	res = PylonDeviceFeatureIsAvailable(hDevice[nCamIndex], "EnumEntry_TriggerSelector_FrameStart");
	if(res!=0)
		res = PylonDeviceFeatureFromString(hDevice[nCamIndex], "TriggerSelector", "FrameStart");
}


/**********************************************************************************/
/*							AOI  Control									      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Width
/*  
   함수 설명   : Get image width
   Parameter   : [In]카메라 인덱스 , [Out] int *nMinWidth, int *nWidth , int *nMaxWidth
   return      : x
*/
void CCameraManager::GetWidth(int nCamIndex,int *nMinWidth, int *nWidth , int *nMaxWidth)
{
	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "Width", &m_iCM_Width[nCamIndex] );	
	*nWidth = (int)m_iCM_Width[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "Width", &m_iCM_MinWidth[nCamIndex] );	
	*nMinWidth = (int)m_iCM_MinWidth[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "Width", &m_iCM_MaxWidth[nCamIndex] );	
	*nMaxWidth = (int)m_iCM_MaxWidth[nCamIndex];
}
/*  
   함수 설명   : Set image width
   Parameter   : [In]카메라 인덱스 , [In] int nWidth
   return      : x
*/
void CCameraManager::SetWidth(int nCamIndex,int nWidth)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "Width", nWidth );
	m_iCM_Width[nCamIndex] = nWidth;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Height
/*  
   함수 설명   : Get image height 
   Parameter   : [In]카메라 인덱스 , [Out] int *nMinHeight, int *nHeight, int *nMaxHeight
   return      : x
*/
void CCameraManager::GetHeight(int nCamIndex, int *nMinHeight, int *nHeight, int *nMaxHeight)
{
	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "Height", &m_iCM_Height[nCamIndex] );	
	*nHeight = (int)m_iCM_Height[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "Height", &m_iCM_MinHeight[nCamIndex] );	
	*nMinHeight = (int)m_iCM_MinHeight[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "Height", &m_iCM_MaxHeight[nCamIndex] );	
	*nMaxHeight = (int)m_iCM_MaxHeight[nCamIndex];
}
/*  
   함수 설명   : Set image height 
   Parameter   : [In]카메라 인덱스 , [In] int nHeight
   return      : x
*/
void CCameraManager::SetHeight(int nCamIndex, int nHeight)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "Height", nHeight );
	m_iCM_Height[nCamIndex] = nHeight;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Y Offset
/*  
   함수 설명   : Get image offset y  
   Parameter   : [In]카메라 인덱스 , [Out] int *nMinOffsety, int *nOffsety, int *nMaxOffsety
   return      : x
*/
void CCameraManager::GetOffsetY(int nCamIndex,int *nMinOffsety, int *nOffsety, int *nMaxOffsety)
{
	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "OffsetY", &m_iCM_OffsetY[nCamIndex] );	
	*nOffsety = (int)m_iCM_OffsetY[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "OffsetY", &m_iCM_MinOffsetY[nCamIndex] );	
	*nMinOffsety = (int)m_iCM_MinOffsetY[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "OffsetY", &m_iCM_MaxOffsetY[nCamIndex] );	
	*nMaxOffsety = (int)m_iCM_MaxOffsetY[nCamIndex];
}
/*  
   함수 설명   : Set image offset y  
   Parameter   : [In]카메라 인덱스 , [In] int nOffsety
   return      : x
*/
void CCameraManager::SetOffsetY(int nCamIndex,int nOffsety)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "OffsetY", nOffsety );
	m_iCM_OffsetY[nCamIndex] = nOffsety;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   X Offset
/*   
   함수 설명   : Get image offset x 
   Parameter   : [In]카메라 인덱스 , [Out] int *nMinOffsetx, int *nOffsetx, int *nMaxOffsetx
   return      : x
*/
void CCameraManager::GetOffsetX(int nCamIndex,int *nMinOffsetx, int *nOffsetx, int *nMaxOffsetx)
{
	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "OffsetX", &m_iCM_OffsetX[nCamIndex] );
	*nOffsetx = (int)m_iCM_OffsetX[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "OffsetX", &m_iCM_MinOffsetX[nCamIndex] );
	*nMinOffsetx = (int)m_iCM_MinOffsetX[nCamIndex];

	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "OffsetX", &m_iCM_MaxOffsetX[nCamIndex] );
	*nMaxOffsetx = (int)m_iCM_MaxOffsetX[nCamIndex];
}
/*   
   함수 설명   : Set image offset x  
   Parameter   : [In]카메라 인덱스 , [In] int nOffsetx
   return      : x
*/
void CCameraManager::SetOffsetX(int nCamIndex,int nOffsetx)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "OffsetX", nOffsetx );
	m_iCM_OffsetX[nCamIndex] = nOffsetx;
}

/**********************************************************************************/
/*						Acquisition Controls								      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  TriggerSelector
/*   
   함수 설명   : Get Trigger Selector
   Parameter   : [In]카메라 인덱스 , [Out] char *szTriggerSelector
                 AcquisitionStart
   return      : x
*/
void CCameraManager::GetTriggerSelector(int nCamIndex, char *szTriggerSelector)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "TriggerSelector", buf ,&siz);
	lstrcpy(szTriggerSelector,buf);
}
/*   
   함수 설명   : Set Trigger Selector
   Parameter   : [In]카메라 인덱스 , [In] CString  strTriggerSelector
                 AcquisitionStart 만 이용 가능
   return      : x
*/
void CCameraManager::SetTriggerSelector(int nCamIndex, CString  strTriggerSelector)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "TriggerSelector", strTriggerSelector);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Trigger Mode

/*   
   함수 설명   : Get Trigger Mode
   Parameter   : [In]카메라 인덱스 , [Out] char *szTriggerMode
                 Off , On
   return      : x
*/
void CCameraManager::GetTriggerMode(int nCamIndex, char *szTriggerMode)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "TriggerMode", buf ,&siz);
	lstrcpy(szTriggerMode,buf);
}
/*   
   함수 설명   : Set Trigger Mode
   Parameter   : [In]카메라 인덱스 , [In] CString strTriggerMode
                 Off , On
   return      : x
*/
void CCameraManager::SetTriggerMode(int nCamIndex, CString strTriggerMode)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "TriggerMode", strTriggerMode);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Generate Software Trigger
/*   
   함수 설명   : Execute SoftTrigger
   Parameter   : [In]카메라 인덱스                  
   return      : x
*/
void CCameraManager::ExecuteSoftTrigger(int nCamIndex)
{
	res = PylonDeviceExecuteCommandFeature(  hDevice[nCamIndex], "TriggerSoftware" );    //TriggerSource
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Trigger Source
/*   
   함수 설명   : Get Trigger Source
   Parameter   : [In]카메라 인덱스 , [Out] char *szTriggerSource
                 Line1 ,Line2 , Software
   return      : x
*/
void CCameraManager::GetTriggerSource(int nCamIndex, char *szTriggerSource)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "TriggerSource", buf ,&siz);
	lstrcpy(szTriggerSource,buf);
}
/*   
   함수 설명   : Set Trigger Source
   Parameter   : [In]카메라 인덱스 , [In]  CString strTriggerSource
                 Line1 ,Line2 , Software
   return      : x
*/
void CCameraManager::SetTriggerSource(int nCamIndex, CString strTriggerSource)
{
    res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "TriggerSource", strTriggerSource);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Trigger Activation
/*   
   함수 설명   : Get Trigger Activation
   Parameter   : [In]카메라 인덱스 , [Out] char *szTriggerActive
                 RisingEdge ,FallingEdge
   return      : x
*/
void CCameraManager::GetTriggerActivation(int nCamIndex, char *szTriggerActive)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "TriggerActivation", buf ,&siz);
	lstrcpy(szTriggerActive,buf);
}
/*   
   함수 설명   : Set Trigger Activation
   Parameter   : [In]카메라 인덱스 , [In] CString strTriggerActive
                 RisingEdge ,FallingEdge
   return      : x
*/
void CCameraManager::SetTriggerActivation(int nCamIndex, CString strTriggerActive)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "TriggerActivation", strTriggerActive);	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Exposure Mode
/*   
   함수 설명   : Get Exposure Mode
   Parameter   : [In]카메라 인덱스 , [Out] char *szExpMode
                 Timed ,TriggerWidth
   return      : x
*/
void CCameraManager::GetExposureMode(int nCamIndex, char *szExpMode)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "ExposureMode", buf ,&siz);
	lstrcpy(szExpMode,buf);	
}
/*   
   함수 설명   : Set Exposure Mode
   Parameter   : [In]카메라 인덱스 , [In] CString strExpMode
                 Timed ,TriggerWidth
   return      : x
*/
void CCameraManager::SetExposureMode(int nCamIndex, CString strExpMode)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "ExposureMode", strExpMode);	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Exposure Auto
/*   
   함수 설명   : Get Exposure Auto
   Parameter   : [In]카메라 인덱스 , [Out] char *szExpAuto
                 Off ,Once , Continuous
   return      : x
*/
void CCameraManager::GetExposureAuto(int nCamIndex, char *szExpAuto)   
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "ExposureAuto", buf ,&siz);
	lstrcpy(szExpAuto,buf);
}
/*   
   함수 설명   : Set Exposure Auto
   Parameter   : [In]카메라 인덱스 , [In] CString strExpAuto
                 Off ,Once , Continuous
   return      : x
*/
void CCameraManager::SetExposureAuto(int nCamIndex, CString strExpAuto)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "ExposureAuto", strExpAuto);	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Exposure Time Abs
/*   
   함수 설명   : Get Exposure Time Abs
   Parameter   : [In]카메라 인덱스 , [Out] double *dMinExpAbs, double *dExpAbs, double *dMaxExpAbs                
   return      : x
*/
void CCameraManager::GetExposureTimeAbs(int nCamIndex, double *dMinExpAbs, double *dExpAbs, double *dMaxExpAbs)
{
	res = PylonDeviceGetFloatFeature( hDevice[nCamIndex], "ExposureTimeAbs", dExpAbs );	
	res = PylonDeviceGetFloatFeatureMin( hDevice[nCamIndex], "ExposureTimeAbs", dMinExpAbs );	
	res = PylonDeviceGetFloatFeatureMax( hDevice[nCamIndex], "ExposureTimeAbs", dMaxExpAbs );	
}
/*   
   함수 설명   : Set Exposure Time Abs
   Parameter   : [In]카메라 인덱스 , [In] double dExpAbs               
   return      : x
*/
void CCameraManager::SetExposureTimeAbs(int nCamIndex, double dExpAbs)
{
	 res = PylonDeviceSetFloatFeature( hDevice[nCamIndex], "ExposureTimeAbs", dExpAbs );	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Exposure Time Base
/*   
   함수 설명   : Get Exposure Time Base
   Parameter   : [In]카메라 인덱스 , [Out] double *dMinExpTBase, double *dExpTBase, double *dMaxExpTBase                
   return      : x
*/
void CCameraManager::GetExposureTimeBase(int nCamIndex, double *dMinExpTBase, double *dExpTBase, double *dMaxExpTBase)
{
	res = PylonDeviceGetFloatFeature( hDevice[nCamIndex], "ExposureTimeBase", dExpTBase );	
	res = PylonDeviceGetFloatFeatureMin( hDevice[nCamIndex], "ExposureTimeBase", dMinExpTBase );	
	res = PylonDeviceGetFloatFeatureMax( hDevice[nCamIndex], "ExposureTimeBase", dMaxExpTBase );	
}

/*   
   함수 설명   : Set Exposure Time Base
   Parameter   : [In]카메라 인덱스 , [In] double dExpTBase               
   return      : x
*/
void CCameraManager::SetExposureTimeBase(int nCamIndex, double dExpTBase)
{
	res = PylonDeviceSetFloatFeature( hDevice[nCamIndex], "ExposureTimeBase", dExpTBase );	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Exposure Time Raw
/*   
   함수 설명   : Get Exposure Time Raw
   Parameter   : [In]카메라 인덱스 , [Out] int *nMinExpRaw, int *nExpRaw, int *nMaxExpRaw                
   return      : x
*/
void CCameraManager::GetExposureTimeRaw(int nCamIndex, int *nMinExpRaw, int *nExpRaw, int *nMaxExpRaw)
{
	int64_t ntemp;

	res = PylonDeviceGetIntegerFeature( hDevice[nCamIndex], "ExposureTimeRaw", &ntemp );	
    *nExpRaw = (int)ntemp;
	res = PylonDeviceGetIntegerFeatureMin( hDevice[nCamIndex], "ExposureTimeRaw", &ntemp );	
    *nMinExpRaw = (int)ntemp;
	res = PylonDeviceGetIntegerFeatureMax( hDevice[nCamIndex], "ExposureTimeRaw", &ntemp );	
    *nMaxExpRaw = (int)ntemp;
}
/*   
   함수 설명   : Set Exposure Time Raw
   Parameter   : [In]카메라 인덱스 , [In] int nExpRaw               
   return      : x
*/
void CCameraManager::SetExposureTimeRaw(int nCamIndex, int nExpRaw)
{
	res = PylonDeviceSetIntegerFeature( hDevice[nCamIndex], "ExposureTimeRaw",nExpRaw );	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Enable Acquisition Frame Rate
/*   
   함수 설명   : Get Enable Acquisition Frame Rate
   Parameter   : [In]카메라 인덱스 , [Out] bool *bEnableFps                
   return      : x
*/
void CCameraManager::GetEnableFps(int nCamIndex, bool *bEnableFps)
{
    _Bool bTemp;
	res = PylonDeviceGetBooleanFeature(hDevice[nCamIndex], "AcquisitionFrameRateEnable", &bTemp);
	if(bTemp==TRUE)
        *bEnableFps = true;
	else
		*bEnableFps = false;
}
/*   
   함수 설명   : Set Enable Acquisition Frame Rate
   Parameter   : [In]카메라 인덱스 , [Out] bool bEnableFps              
   return      : x
*/
void CCameraManager::SetEnableFps(int nCamIndex, bool bEnableFps)
{
	res = PylonDeviceSetBooleanFeature(hDevice[nCamIndex], "AcquisitionFrameRateEnable", bEnableFps);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Frame Rate

/*   
   함수 설명   : Get Frame Rate 
   Parameter   : [In]카메라 인덱스 , [Out] double *dMinFps, double *dFps, double *dMaxFps                
   return      : x
*/
void CCameraManager::GetFrameRate(int nCamIndex, double *dMinFps, double *dFps, double *dMaxFps)
{
	res = PylonDeviceGetFloatFeature( hDevice[nCamIndex], "AcquisitionFrameRateAbs", dFps );	
	res = PylonDeviceGetFloatFeatureMin( hDevice[nCamIndex], "AcquisitionFrameRateAbs", dMinFps );	
	res = PylonDeviceGetFloatFeatureMax( hDevice[nCamIndex], "AcquisitionFrameRateAbs", dMaxFps );	
}

/*   
   함수 설명   : Set Frame Rate 
   Parameter   : [In]카메라 인덱스 , [In] double dFps                
   return      : x
*/
void CCameraManager::SetFrameRate(int nCamIndex, double dFps)
{
	res = PylonDeviceSetFloatFeature( hDevice[nCamIndex], "AcquisitionFrameRateAbs", dFps );	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Result Frame Rate
/*   
   함수 설명   : Get Result Frame Rate 
   Parameter   : [In]카메라 인덱스 , [Out] double *dResultFps                
   return      : x
*/
void CCameraManager::GetResultFrameRate(int nCamIndex,  double *dResultFps)
{
	res = PylonDeviceGetFloatFeature( hDevice[nCamIndex], "ResultingFrameRateAbs", dResultFps );	
}

/**********************************************************************************/
/*						Digital I/O Controls								      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Line Selector
/*   
   함수 설명   : Get I/O Line Selector 
   Parameter   : [In]카메라 인덱스 , [Out] char* szIOLineSelector                
                 Line1, Line2 , Out1 , Out2, Out3, Out4
   return      : x
*/
void CCameraManager::GetIOLineSelector(int nCamIndex,  char* szIOLineSelector)        
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "LineSelector", buf ,&siz);
	lstrcpy(szIOLineSelector,buf);
}
/*   
   함수 설명   : Set I/O Line Selector 
   Parameter   : [In]카메라 인덱스 , [Out] char* szIOLineSelector                
                 Line1, Line2 , Out1 , Out2, Out3, Out4
   return      : x
*/
void CCameraManager::SetIOLineSelector(int nCamIndex,  CString strIOLineSelector)
{
    res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "LineSelector", strIOLineSelector);	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Line Selector
void CCameraManager::GetIOLineMode(int nCamIndex,  char* szIOLineMode)        // Input, Output
{
}
void CCameraManager::SetIOLineMode(int nCamIndex,  CString strIOLineMode)
{
}
void CCameraManager::GetIOLineFormat(int nCamIndex,  char* szIOLineFormat)     // OptoCoupled
{
}
void CCameraManager::SetIOLineFormat(int nCamIndex,  CString strIOLineFormat)
{
}
void CCameraManager::GetIOLineSource(int nCamIndex,  char* szIOLineSource)      // ExposureActive , TimerActive ,UserOutput , TriggerReady , SerialTx , AcquisitionTriggerReady
{
}
void CCameraManager::SetIOLineSource(int nCamIndex,  CString strIOLineSource)
{
}
void CCameraManager::GetIOLineDebouncertime(int nCamIndex,  double* dIODebouncerTime)    // Input 만 사용
{
}
void CCameraManager::SetIOLineDebouncertime(int nCamIndex,  double dIODebouncerTime)     // Input 만 사용
{
}
void CCameraManager::GetIOLineInverter(int nCamIndex, bool* bIOLineInverter)     // Output 만 이용
{
}
void CCameraManager::SetIOLineInverter(int nCamIndex, bool bIOLineInverter)      // Output 만 이용
{
}
void CCameraManager::GetIOLineStatus(int nCamIndex, bool* bIOLineStatus)         // Output 만 이용
{
}
void CCameraManager::SetIOLineStatus(int nCamIndex, bool bIOLineStatus)          // Output 만 이용
{
}
void CCameraManager::GetIOUserOutputSelector(int nCamIndex, char* szIOUserOutput)    // UserOutput1 ~ 4
{
}
void CCameraManager::SetIOUserOutputSelector(int nCamIndex, CString strIOUserOutput)
{
}
void CCameraManager::GetIOUserOuputValue(int nCamIndex, bool* bIOUserOV)
{
}
void CCameraManager::SetIOUserOuputValue(int nCamIndex, bool bIOUserOV)
{
}
void CCameraManager::GetIOUserOuputValueAll(int nCamIndex, int* nIOUserOVall)
{
}
void CCameraManager::SetIOUserOuputValueAll(int nCamIndex, int nIOUserOVall)
{
}  

/**********************************************************************************/
/*						Configuration Sets									      */
/**********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Configuration Set Selector

/*   
   함수 설명   : Get Configuration Set Selector 
   Parameter   : [In]카메라 인덱스 , [Out] char* szConSetSelector               
                 Default  ,UserSet1 , UserSet2 ,UserSet3
   return      : x
*/
void CCameraManager::GetConfigrationSetSelector(int nCamIndex, char* szConSetSelector)
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "UserSetSelector", buf ,&siz);
	lstrcpy(szConSetSelector,buf);
}
/*   
   함수 설명   : Set Configuration Set Selector 
   Parameter   : [In]카메라 인덱스 , [Out] CString strConSetSelector               
                 Default  ,UserSet1 , UserSet2 ,UserSet3
   return      : x
*/
void CCameraManager::SetConfigrationSetSelector(int nCamIndex, CString strConSetSelector)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "UserSetSelector", strConSetSelector);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   UserSet Load
/*   
   함수 설명   : Execute UserSet Load 
   Parameter   : [In]카메라 인덱스                  
   return      : x
*/

void CCameraManager::ExecuteUserLoad(int nCamIndex)
{
    res = PylonDeviceExecuteCommandFeature( hDevice[nCamIndex], "UserSetLoad" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   UserSet Save
/*   
   함수 설명   : Execute UserSet Save
   Parameter   : [In]카메라 인덱스                  
   return      : x
*/
void CCameraManager::ExecuteUserSave(int nCamIndex)
{
    res = PylonDeviceExecuteCommandFeature( hDevice[nCamIndex], "UserSetSave" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   Default Startup Set
/*   
   함수 설명   : Get Default Start Set
   Parameter   : [In]카메라 인덱스 , [Out] char* szDefaultStartSet               
                 Default  ,UserSet1 , UserSet2 ,UserSet3
   return      : x
*/
void CCameraManager::GetDefaultStartSet(int nCamIndex, char* szDefaultStartSet)
{
	char buf[50];
	size_t siz = sizeof(buf);
	res = PylonDeviceFeatureToString( hDevice[nCamIndex], "UserSetDefaultSelector", buf ,&siz);
	lstrcpy(szDefaultStartSet,buf);
}
/*   
   함수 설명   : Set Default Start Set 
   Parameter   : [In]카메라 인덱스 , [Out] CString strDefaultStartSet               
                 Default  ,UserSet1 , UserSet2 ,UserSet3
   return      : x
*/
void CCameraManager::SetDefaultStartSet(int nCamIndex, CString strDefaultStartSet)
{
	res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "UserSetDefaultSelector", strDefaultStartSet);
}
/**********************************************************************************/
/*					Single Grab & Live Grab									      */
/**********************************************************************************/

/*
	함수 이름   : GrabSingle 
	함수 설명   : One Shot image
	Parameter   : [In]카메라 인덱스 , [Out] unsigned char *pImagebuf
	return      : X
*/
void CCameraManager::GrabSingle(int nCamIndex, void *pImagebuf)
{	    
        res = PylonStreamGrabberGetWaitObject( hGrabber[nCamIndex], &hWait[nCamIndex] );       		
		res = PylonDeviceFeatureFromString( hDevice[nCamIndex], "AcquisitionMode", "SingleFrame" );   //SingleFrame
		 		
		for ( int i = 0; i < 1; i++ )
		{
			res = PylonStreamGrabberRegisterBuffer( hGrabber[nCamIndex],pImagebuf, m_iCMPayloadSize[nCamIndex],  &bufHandles[i] );
			res = PylonStreamGrabberQueueBuffer( hGrabber[nCamIndex], bufHandles[i], (void*) i );
		}

		_Bool  isReady;        
		PylonGrabResult_t         grabResult;     
		int bufferIndex;     
	    res = PylonDeviceExecuteCommandFeature( hDevice[nCamIndex], "AcquisitionStart");
		PylonWaitObjectWait( hWait[nCamIndex], 100, &isReady );

		res = PylonStreamGrabberRetrieveResult( hGrabber[nCamIndex], &grabResult, &isReady );
		bufferIndex = (int) grabResult.Context;

        if ( grabResult.Status == Grabbed  )
        {

	    	//	memcpy(pImagebuf,(unsigned char*) grabResult.pBuffer ,payloadSize);									 				
		}		

        PylonDeviceExecuteCommandFeature( hDevice[nCamIndex], "AcquisitionStop");
        PylonStreamGrabberCancelGrab(  hGrabber[nCamIndex] );
        for ( i = 0; i < 1; ++i )   
		{
            PylonStreamGrabberDeregisterBuffer(  hGrabber[nCamIndex], bufHandles[i] );              
		}		 
}
/*
	함수 이름   : GrabLive
	함수 설명   : Live Grab
	Parameter   : [In]카메라 인덱스 , [Out] unsigned char *pImagebuf
	return      : X
*/
void CCameraManager::GrabLive(int nCamIndex, void *pImagebuf)
{
	    for(int i=0; i<NUM_BUFFERS; i++)
		{
			pImageGetbuf[nCamIndex][i] = pImagebuf;
		}		
		m_bStopLiveThread[nCamIndex]=true;  // 쓰레드 정지 Flag
		m_bStopFlag[nCamIndex] = true;      // Live 정지 Flag
		switch(nCamIndex)
		{
			case 0:
				AfxBeginThread(LiveThreadCam0,NULL);
				break;
			case 1:
				AfxBeginThread(LiveThreadCam1,NULL);
				break;
		}
}

/*
	함수 이름   : Stop Grab Live
	함수 설명   : Live 정지
	Parameter   : [In]카메라 인덱스 , [In] bool bStop
	return      : X
*/   

void CCameraManager::StopGrabLive(int nCamIndex,bool bStop)
{
	    m_bStopFlag[nCamIndex]=bStop; 
}
/*
	함수 이름   : Check Grab Stats
	함수 설명   : Grab 완료 여부 확인
	Parameter   : [In]카메라 인덱스 
	return      : bool
*/   
bool CCameraManager::CheckCaptureEnd(int nCamIndex)
{
     return m_bCaptureEnd[nCamIndex];
}
/*
	함수 이름   : ReadEnd
	함수 설명   : 영상 read 확인 
	Parameter   : [In]카메라 인덱스
	return      : X
*/   
void CCameraManager::ReadEnd(int nCamIndex)
{
    	m_bCaptureEnd[nCamIndex] = false;
}
/*
	함수 이름   : HeartBeatTime
	함수 설명   : Debug 모드 BreakPoint 걸어서 코드 확인시 HeartbeatTime시간동안 코드 이동이 없는 경우 카메라 연결이 끊어짐 
	Parameter   : [In]카메라 인덱스 , [In dTime (단위는 ms)
	return      : X
*/  
void CCameraManager::SetHeartBeatTimeOut(int nCamIndex,int dtime)
{
	PylonDeviceGetTLNodeMap(hDevice[nCamIndex], &hNodemap[nCamIndex] );
	GenApiNodeMapGetNode( hNodemap[nCamIndex], "HeartbeatTimeout", &hNode[nCamIndex] );
	res = GenApiIntegerSetValue( hNode[nCamIndex], dtime );
}

/**********************************************************************************/
/*						Error Description									      */
/**********************************************************************************/		
void CCameraManager::printErrorAndExit( GENAPIC_RESULT errc )
{
    char *errMsg;
    size_t length;
	CString error;

    GenApiGetLastErrorMessage( NULL, &length );
    errMsg = (char*) malloc( length );

    GenApiGetLastErrorMessage( errMsg, &length );
    		
	
    error.Format("%s (%#08x).\n", errMsg, errc);
    free( errMsg);
	AfxMessageBox(error);

    GenApiGetLastErrorDetail( NULL, &length );
    errMsg = (char*) malloc( length );

    GenApiGetLastErrorDetail( errMsg, &length );    
	error.Format("%s\n", errMsg);
    free( errMsg);
    AfxMessageBox(error);
}
