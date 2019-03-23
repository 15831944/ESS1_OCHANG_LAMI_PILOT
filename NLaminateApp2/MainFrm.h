
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "NLaminateAppView.h"
#include "CAM\EuresisCamLib.h"
#include "Controls\\ClockTimer.h"
//#include "OmronPlcM.h"

class CMainFrame : public CFrameWnd
{
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	HWND           m_hSplash;

//	CPci7230Daq    m_Pci7230Daq;
	CNLaminateAppView *m_pView;
//	CBaslerCamLib   m_CAM;
	CEuresisCamLib m_CAM;
	CRITICAL_SECTION	m_csSync;
	CClockTimer     m_ctrlTimer;
	//COmronPlcM     m_OmronPlc;

	BOOL           m_bGaugeRnR;
	BOOL           m_bGrab;
	BOOL           m_bLive;
	BOOL           m_bExecRealTime;
	BOOL           m_bShortTest;
	BOOL           m_bAppExec;
	BOOL           m_bCriticalF;
	BOOL           m_bOnData2PLC;
	BOOL           m_GrabCAM[CAM_MAX_NUM];
	BOOL           m_bStatusCOM[2];
	BOOL           m_bOnConnect;
	BOOL           m_nDecF;
	BOOL           m_nDecR;

	long           m_nPageNo;
	long           m_nSelectCAM;
	long           m_nZoomScale;
	float          m_fSendDataCUT;
	float          m_fSumDataEPC;
	float          m_fOldTEOffset;
	float          m_fFrontShort;
	float          m_fRearShort;
	float          m_fSendDataEPC;

	long           m_nDaqIndex;
	long           m_nCountDataEPC;
	bool           m_bCOMM;
	long           m_nRecipeNum;

	CString        m_strFilePath;
	CString        m_strFileType;
	CString        m_strModelType;
	CString        m_strLotID;

	float          m_fZoomScale;
	CPoint         m_ptMouse;
	CPoint         m_ptOldMouse;
	CRect          m_fmRect;

	SYSTEMTIME	   m_logStartTime;
	SYSTEMTIME	   m_logEndTime;
	double         m_dEllipsedUmac;
	double         m_dStartTime;
	double         m_dEndTime;

	SYSTEMTIME	   m_sysNewTime, m_sysOldTime;

	SETTING_DATA   m_Setting;
	SEARCH_DATA    m_Search;
	ETC_DATA       m_Etc;
	PRODUCT_DATA   m_ProdData;
	INSP_RESULT_DATA *m_pRes[VIEW_MAX_NUM];

	CString		   m_strUnW_Up_Barcode;
	CString		   m_strUnW_Dn_Barcode;
	long		   m_nUpperPos;
	long		   m_nDownPos;
	


// 작업입니다.
public:
	void           Grab();
	void           Live();
	BOOL           LoadSysValueFromFile();
    BOOL           SaveSysValueToFile();
	BOOL           LoadModelData();
    BOOL           SaveModelData();
	void		   WriteTextModelName(CString str);
	void		   WriteTextStatus(CString str);
	void		   WriteTextMessage(CString str);
	void		   WriteTextShortMessage(CString str);
	void           SetCheckLED(long nCAM, BOOL bOnOff);
	void		   ExitProgram();
	void           LoadImage(CString strFileName);
	void           SaveImage(CString strFileName);
	void           AsSaveImage(CString strFileName);
    void		   SaveRealTimeData();
	int			   ExecMessageHandleAll();
	int			   ExecMessageHandleOne();
    BOOL           SendStringForWrite(CString strSend);
	void		   WriteStringToFile_Csv_Day();
    void           WriteStringToFile();
    void           WriteLogToFile(char *string);
    void           DeleteDataPath();
	void           TransOriginToBadImage(long nCAM, long pRes, LPBYTE pImage);
	void           CopyResultData(INSP_RESULT_DATA *pDest, INSP_RESULT_DATA *pSour);
	CString        GetModelName(long nOrderNum);
	void           ExecFinalCuttingOffset(long nSelExec, BOOL *bRetCUT, float *fFinalOS, BOOL *bRetT2B, float *fT2BOS, BOOL *bRetTE, float *fTEOS);
	void           SendResult2UMAC(BOOL bFDec, BOOL bRDec, BOOL bFTab, BOOL bRTab, BOOL bMisMatch, float fCutOS, BOOL bRetEPC, float fOffsetEPC);
	void           SendDataToPLC();
	long           GetIndexTitleROI(INSP_RESULT_DATA* pRes, CString strTitle);
	long           GetIndexTitleLinkROI(INSP_RESULT_DATA* pRes, CString strTitle);
	BOOL           CopyRealTimeImage(long nCAM, LPBYTE pImage);
	void           CopyRealTimeMelsec();
	void           GetRecipeStringFromEthernet();
	void           UpdateBadImage();

	BOOL		   NPio_BitRead(long nBit);
	BOOL		   NPio_BitReadABS(long nBit);
	long		   NPio_Read();
	void		   NPio_BitWrite(long nBit, BOOL bSig);
	void		   NPio_Write(long nData);
	void		   NPio_Close();
	BOOL		   NPio_Open(long nPort, long nBaud, long nSize, long nParity, long nStop);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar     m_wndStatusBar;

	OFFSET_DATA    m_OffsetD[OFFSET_MAX_NUM];

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


