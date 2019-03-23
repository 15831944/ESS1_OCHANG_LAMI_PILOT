#if !defined(AFX_CAMERAMANAGER_H__061680CF_C5CC_4189_91EE_2C984710D593__INCLUDED_)
#define AFX_CAMERAMANAGER_H__061680CF_C5CC_4189_91EE_2C984710D593__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraManager.h : header file
//
#include <pylonc/PylonC.h>

/////////////////////////////////////////////////////////////////////////////
// CCameraManager view

class CCameraManager : public CView
{
public:
	CCameraManager();           // protected constructor used by dynamic creation
	virtual ~CCameraManager();
	DECLARE_DYNCREATE(CCameraManager)

// Attributes
public:
	
// Operations
public:

	unsigned int                numDevices;      // 카메라 수 
	GENAPIC_RESULT              res;
	PYLON_DEVICE_HANDLE         hDevice[CAM_NUM];                  /* Handle for the pylon device */
	PYLON_STREAMGRABBER_HANDLE  hGrabber[CAM_NUM];
	PYLON_STREAMBUFFER_HANDLE   bufHandles[NUM_BUFFERS];           /* Handles for the buffers */
	PYLON_WAITOBJECT_HANDLE     hWait[CAM_NUM];                    /* Handle used for waiting for a grab to be finished */
    PylonDeviceInfo_t           hDeviceInfo[CAM_NUM];

    PYLON_DEVICECALLBACK_HANDLE hCb[CAM_NUM];		               /* Required for deregistering the callback. */

    NODEMAP_HANDLE              hNodemap[CAM_NUM];   
    NODE_HANDLE                 hNode[CAM_NUM];      

	 CString   m_strCM_ImageForamt[CAM_NUM];           // Image format	

 	 int64_t  m_iCM_OffsetX[CAM_NUM];
	 int64_t  m_iCM_MinOffsetX[CAM_NUM];
	 int64_t  m_iCM_MaxOffsetX[CAM_NUM];

	 int64_t  m_iCM_OffsetY[CAM_NUM];
	 int64_t  m_iCM_MinOffsetY[CAM_NUM];
	 int64_t  m_iCM_MaxOffsetY[CAM_NUM];

	 int64_t  m_iCM_Width[CAM_NUM];
	 int64_t  m_iCM_MinWidth[CAM_NUM];
	 int64_t  m_iCM_MaxWidth[CAM_NUM];

	 int64_t  m_iCM_Height[CAM_NUM];
	 int64_t  m_iCM_MinHeight[CAM_NUM];
	 int64_t  m_iCM_MaxHeight[CAM_NUM];
	 int64_t  m_iCM_reSizeWidth[CAM_NUM];              // width -> 4Byte 배수로 계산 

	 int32_t  m_iCMPayloadSize[CAM_NUM];

     void  *pImageGetbuf[CAM_NUM][NUM_BUFFERS];        // Image buffer

	 bool  m_bStopLiveThread[CAM_NUM];
	 bool  m_bCaptureEnd[CAM_NUM];
	 bool  m_bStopFlag[CAM_NUM];

	 bool  m_bCameraDisConnectFlag[CAM_NUM];
	  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraManager)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
public:	   
		/*
			함수 설명   : 시스템에 연결된 카메라 찾기 ( find all cameras in Network)
			Parameter   : [Out]카메라 이름(camera nme), [Out]연결된 카메라 수(camera number)
			return      : bool
		*/
        bool FindCamera(int *nCamNumber);
		/*
			함수 설명   : Camera Open & Stream grabber Open
			Parameter   : [In]카메라 인덱스 (camera index), [In]카메라 position (camera position)
			return      : bool	
		*/	    
	    bool OpenCamera(int nCamIndex, int nPosition);
		/* 
			함수 설명   : Camera & Stream grabber close
			Parameter   : [In]카메라 인덱스
			return      :  bool   
		*/
		bool CloseCamera(int nCamIndex);
		/* 
			함수 설명   : Camera Connect  
			Parameter   : [In]카메라 인덱스 , [In] int nOffsetX, int nOffsetY, int nWidth, int nHeight
			return      :  bool   
		*/
      	bool CameraConnect(int nCamIndex, int nOffsetX, int nOffsetY, int nWidth, int nHeight);
		/* 
			함수 설명   : Camera DisConnect  
			Parameter   : [In]카메라 인덱스
			return      :  bool   
		*/
		bool DisConnectCamera(int nCamIndex);


        void GrabSingle(int nCamIndex,void *pImagebuf);
		void GrabLive(int nCamIndex,void *pImagebuf);
		void StopGrabLive(int nCamIndex,bool bStop);

		bool CheckCaptureEnd(int nCamIndex);
		void ReadEnd(int nCamIndex);

/**********************************************************************************/
/*							Analog  Controls								      */
/**********************************************************************************/
       void GetGainAuto(int nCamImdex, char *szGainAuto);           // Off, Once, Continuous
	   void SetGainAuto(int nCamImdex, CString strGainAuto);

	   void GetGainSelector(int nCamIndex, char *szGainSelector);  // All , Tap1 , Tap2
	   void SetGainSelector(int nCamIndex, CString strGainSelector);

	   void GetGain(int nCamIndex, int *nMinGain, int *nGain , int *nMaxGain);
	   void SetGain(int nCamIndex, int nGain);

	   void GetBlackLevelSelector(int nCamIndex, char *szBlackLevelSelector);  // All , Tap1 , Tap2
	   void SetBlackLevelSelector(int nCamIndex, CString strBlackLevelSelector);

	   void GetBlackLevel(int nCamIndex, int *nMinBlackLevel, int *nBlackLevel , int *nMaxBlackLevel);
	   void SetBlackLevel(int nCamIndex, int nBlackLevel);

	   void GetWhiteBalAuto(int nCamIndex, char *szWhiteBalAuto);               // Off , Once
	   void SetWhiteBalAuto(int nCamIndex, CString  strWhiteBalAuto);

	   void GetColorBalSelector(int nCamIndex, char *szColorBalSelector);        // Red , Green , Blue
	   void SetColorBalSelector(int nCamIndex, CString  strColorBalSelector);

	   void GetColorRaw(int nCamIndex, int *nMinColorRaw, int *nColorRaw, int *nMaxColorRaw); 
	   void SetColorRaw(int nCamIndex, int nColorRaw);

	   void GetGammaEnable(int nCamIndex, bool *bGammaEnable);                   // 0(false) , 1(true)
	   void SetGammaEnable(int nCamIndex, bool bGammaEnable);

	   void GetGamma(int nCamIndex, double *dMinGammaEnable, double *dGammaEnable, double *dMaxGammaEnable); 
	   void SetGamma(int nCamIndex, double dGammaEnable);

/**********************************************************************************/
/*							Image Format  Controls							      */
/**********************************************************************************/
	
		void GetImgFormat(int nCamIndex,char *szImgFormat);             //Mono8 , Mono 16 , Mono12Packed ,BayerBG8 , BayerRG8 ,BayerGB8 , YUV422Packed , BayerBG16 
		void SetImgFormat(int nCamIndex,CString strImageFormat);        //Mono8 , Mono 16 , Mono12Packed ,BayerBG8 , BayerRG8 ,BayerGB8 , YUV422Packed , BayerBG16

/**********************************************************************************/
/*							AOI  Controls									      */
/**********************************************************************************/
		void GetWidth(int nCamIndex,int *nMinWidth, int *nWidth , int *nMaxWidth);
		void GetHeight(int nCamIndex, int *nMinHeight, int *nHeight, int *nMaxHeight);
		void GetOffsetX(int nCamIndex,int *nMinOffsetx, int *nOffsetx, int *nMaxOffsetx);
		void GetOffsetY(int nCamIndex,int *nMinOffsety, int *nOffsety, int *nMaxOffsety);
		
		void SetWidth(int nCamIndex,int nWidth);
		void SetHeight(int nCamIndex, int nHeight);
		void SetOffsetX(int nCamIndex,int nOffsetx);
		void SetOffsetY(int nCamIndex,int nOffsety);

/**********************************************************************************/
/*						Acquisition Controls								      */
/**********************************************************************************/
        void GetTriggerSelector(int nCamIndex, char *szTriggerSelector);    // AcquisitionStart
		void SetTriggerSelector(int nCamIndex, CString  strTriggerSelector);

		void GetTriggerMode(int nCamIndex, char *szTriggerMode);      // Off , On
		void SetTriggerMode(int nCamIndex, CString strTriggerMode);  

        void ExecuteSoftTrigger(int nCamIndex);

		void GetTriggerSource(int nCamIndex, char *szTriggerSource);      // Line1 ,Line2 , Software
		void SetTriggerSource(int nCamIndex, CString strTriggerSource);  

		void GetTriggerActivation(int nCamIndex, char *szTriggerActive);    // RisingEdge ,FallingEdge
		void SetTriggerActivation(int nCamIndex, CString strTriggerActive);  

		void GetExposureMode(int nCamIndex, char *szExpMode);      // Timed ,TriggerWidth
		void SetExposureMode(int nCamIndex, CString strExpMode);  

		void GetExposureAuto(int nCamIndex, char *szExpAuto);      // Off ,Once , Continuous
		void SetExposureAuto(int nCamIndex, CString strExpAuto);  


		void GetExposureTimeAbs(int nCamIndex, double *dMinExpAbs, double *dExpAbs, double *dMaxExpAbs); 
		void SetExposureTimeAbs(int nCamIndex, double dExpAbs);  

		void GetExposureTimeBase(int nCamIndex, double *dMinExpTBase, double *dExpTBase, double *dMaxExpTBase); 
		void SetExposureTimeBase(int nCamIndex, double dExpTBase);  

		void GetExposureTimeRaw(int nCamIndex, int *nMinExpRaw, int *nExpRaw, int *nMaxExpRaw); 
		void SetExposureTimeRaw(int nCamIndex, int nExpRaw);  


		void GetEnableFps(int nCamIndex, bool *bEnableFps); 
		void SetEnableFps(int nCamIndex, bool bEnableFps);  

		void GetFrameRate(int nCamIndex, double *dMinFps, double *dFps, double *dMaxFps); 
		void SetFrameRate(int nCamIndex, double dFps);  

		void GetResultFrameRate(int nCamIndex,  double *dResultFps); 

/**********************************************************************************/
/*						Digital I/O Controls								      */
/**********************************************************************************/
        void GetIOLineSelector(int nCamIndex,  char* szIOLineSelector);        // Line1, Line2 , Out1 , Out2, Out3, Out4
		void SetIOLineSelector(int nCamIndex,  CString strIOLineSelector);  
        void GetIOLineMode(int nCamIndex,  char* szIOLineMode);        // Input, Output
		void SetIOLineMode(int nCamIndex,  CString strIOLineMode);  
        void GetIOLineFormat(int nCamIndex,  char* szIOLineFormat);     // OptoCoupled
		void SetIOLineFormat(int nCamIndex,  CString strIOLineFormat);  
        void GetIOLineSource(int nCamIndex,  char* szIOLineSource);      // ExposureActive , TimerActive ,UserOutput , TriggerReady , SerialTx , AcquisitionTriggerReady
		void SetIOLineSource(int nCamIndex,  CString strIOLineSource);  
        void GetIOLineDebouncertime(int nCamIndex,  double* dIODebouncerTime);    // Input 만 사용
		void SetIOLineDebouncertime(int nCamIndex,  double dIODebouncerTime);     // Input 만 사용
        void GetIOLineInverter(int nCamIndex, bool* bIOLineInverter);     // Output 만 이용
		void SetIOLineInverter(int nCamIndex, bool bIOLineInverter);      // Output 만 이용
        void GetIOLineStatus(int nCamIndex, bool* bIOLineStatus);         // Output 만 이용
		void SetIOLineStatus(int nCamIndex, bool bIOLineStatus);          // Output 만 이용
        void GetIOUserOutputSelector(int nCamIndex, char* szIOUserOutput);    // UserOutput1 ~ 4
		void SetIOUserOutputSelector(int nCamIndex, CString strIOUserOutput);   
        void GetIOUserOuputValue(int nCamIndex, bool* bIOUserOV);  
		void SetIOUserOuputValue(int nCamIndex, bool bIOUserOV);   
        void GetIOUserOuputValueAll(int nCamIndex, int* nIOUserOVall);  
		void SetIOUserOuputValueAll(int nCamIndex, int nIOUserOVall);   
   
/**********************************************************************************/
/*						Configuration Sets									      */
/**********************************************************************************/
	    void GetConfigrationSetSelector(int nCamIndex, char* szConSetSelector);     //Default  ,UserSet1 , UserSet2 ,UserSet3
		void SetConfigrationSetSelector(int nCamIndex, CString strConSetSelector);  
		void ExecuteUserLoad(int nCamIndex);
		void ExecuteUserSave(int nCamIndex);
	    void GetDefaultStartSet(int nCamIndex, char* szDefaultStartSet);      //Default  ,UserSet1 , UserSet2 ,UserSet3
		void SetDefaultStartSet(int nCamIndex, CString strDefaultStartSet);  

/**********************************************************************************/
/*						TransPort Layer										      */
/**********************************************************************************/		
		void GetHeartBeatTimeOut(int nCamIndex,int* dtime);
		void SetHeartBeatTimeOut(int nCamIndex,int dtime);
        

/**********************************************************************************/
/*						Error Description									      */
/**********************************************************************************/		
		void printErrorAndExit( GENAPIC_RESULT errc );
			
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCameraManager)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERAMANAGER_H__061680CF_C5CC_4189_91EE_2C984710D593__INCLUDED_)
