// ModelData.cpp : implementation file
//

#include "stdafx.h"
#include "ModelData.h"
#include "math.h"
#include "NVisionDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelData
CModelData::CModelData()
{
    _tcscpy_s(m_pData.m_strTitle, _T(""));
	m_pData.m_nImageWidth = IMAGE_WIDTH;
	m_pData.m_nImageHeight = IMAGE_HEIGHT;

	InitModelData();
}

CModelData::~CModelData()
{
	CloseModelData();
}

/////////////////////////////////////////////////////////////////////////////
// CModelData message handlers
void CModelData::InitModelData()
{
	long i, j;

	m_pData.m_bRealTimeMode = FALSE;          // 현재 검사 모드 (검사시 적용)
   	m_pData.m_bSetupMode =FALSE;
    m_pData.m_bDecision = FALSE;          // 결과 판정

   	m_pData.m_dSourX = 0.0f;              // 위치 보정을 위한 X좌표(티칭시 자동 적용)
	m_pData.m_dSourY = 0.0f;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
	m_pData.m_dSourT = 0.0f;              // 각도 보정을 위한 T각도(티칭시 자동 적용)

	m_pData.m_dDeltaX = 0.0f;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData.m_dDeltaY = 0.0f;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData.m_dDeltaT = 0.0f;             // 각도 보정을 위한 T Offset(검사시 적용)

  	m_pData.m_dSourX2 = 0.0f;              // 위치 보정을 위한 X좌표(티칭시 자동 적용)
	m_pData.m_dSourY2 = 0.0f;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
	m_pData.m_dSourT2 = 0.0f;              // 각도 보정을 위한 T각도(티칭시 자동 적용)

	m_pData.m_dDeltaX2 = 0.0f;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData.m_dDeltaY2 = 0.0f;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData.m_dDeltaT2 = 0.0f;             // 각도 보정을 위한 T Offset(검사시 적용)

	m_pData.m_Enhence.m_bUseEnhence = FALSE;          // Mapping 적용 유무
	m_pData.m_Enhence.m_nContrastLow = BLACK_LEVEL;        // Low Position 지정
	m_pData.m_Enhence.m_nContrastHigh = WHITE_LEVEL;       // High Position 지정
	m_pData.m_Enhence.m_nBrightness = 150;         // Brightness Value 지정
	for(i=0; i<DEF_LIGHT_NUM; i++)
		m_pData.m_Enhence.m_LightV[i] = DEF_LIGHT_VALUE;         // Brightness Value 지정

	m_pData.m_IMG.m_nOldMouseX = 0;
	m_pData.m_IMG.m_nOldMouseY = 0;
	m_pData.m_IMG.m_fZoomScale = DEF_INIT_ZOOM_SCALE;
	m_pData.m_IMG.m_fmRect = CRect(0, 0, 0, 0);
	m_pData.m_IMG.m_ptMouse = CPoint(0, 0);

	m_pData.m_CAL.m_dScaleFactor = 1.0f;
	m_pData.m_CAL.m_dScaleX = 1.0f;
	m_pData.m_CAL.m_dScaleY = 1.0f;
	m_pData.m_CAL.m_dFacA = 0.0f;
	m_pData.m_CAL.m_dFacB = 0.0f;
	m_pData.m_CAL.m_dFacC = 0.0f;
	m_pData.m_CAL.m_nAverBright = 0;

	m_pData.m_CAL.m_dScaleLT_X = 1.0f;
	m_pData.m_CAL.m_dScaleLB_X = 1.0f;
	m_pData.m_CAL.m_dScaleRT_X = 1.0f;
	m_pData.m_CAL.m_dScaleRB_X = 1.0f;
	m_pData.m_CAL.m_dScaleL_Y = 1.0f;
	m_pData.m_CAL.m_dScaleR_Y = 1.0f;

	m_pData.m_CAL.m_dRealT_X = 95.9f;
	m_pData.m_CAL.m_dRealB_X = 95.9f;
	m_pData.m_CAL.m_dReal_Y = 144.0f;

	m_pData.m_Teach.m_nPreNum = 0;
	m_pData.m_Teach.m_nLocNum = 0;
	m_pData.m_Teach.m_nLinkNum = 0;
	m_pData.m_Teach.m_fOffsetX = 0;
	m_pData.m_Teach.m_fOffsetY = 0;

	m_pData.m_Teach.m_bPreDecision = FALSE;
	m_pData.m_Teach.m_bLocDecision = FALSE;
	m_pData.m_Teach.m_bLinkDecision = FALSE;
	m_pData.m_Teach.m_bEntire = FALSE;
    m_pData.m_Teach.m_bDispAllROI = TRUE;

	for(i=0; i<IMAGE_LEVEL; i++)
    	m_pData.m_Enhence.m_nLUT[i] = i;   // LUT 초기화

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pData.m_Scan[i].m_nScanType = SCAN_PROJECT_FT;
        m_pData.m_Scan[i].m_nFirstThresh = BLACK_LEVEL;    // First Threshold 설정치
		m_pData.m_Scan[i].m_nSecondThresh= WHITE_LEVEL;       // Second Threshold 설정치
        m_pData.m_Scan[i].m_nFirstScanDirType = DIR_LEFT_RIGHT;   // 스캔 방향 설정
	    m_pData.m_Scan[i].m_nSecondScanDirType= DIR_RIGHT_LEFT;  // 스캔 방향 설정
	    m_pData.m_Scan[i].m_nFirstPolDirType  = POL_BLACK_WHITE;    // First Polarity 방향 설정
	    m_pData.m_Scan[i].m_nSecondPolDirType = POL_BLACK_WHITE;   // Second Polarity 방향 설정
	    m_pData.m_Scan[i].m_dFirstA = 0.0f;
	    m_pData.m_Scan[i].m_dFirstB = 0.0f;
	    m_pData.m_Scan[i].m_dFirstT = 0.0f;
	    m_pData.m_Scan[i].m_dSecondA = 0.0f;
	    m_pData.m_Scan[i].m_dSecondB = 0.0f;
	    m_pData.m_Scan[i].m_dSecondT = 0.0f;
	    m_pData.m_Scan[i].m_nFirstNum = 0;                // SCAN EDGE 데이타 수 
	    m_pData.m_Scan[i].m_nSecondNum = 0;                // SCAN EDGE 데이타 수 

		_tcscpy_s(m_pData.m_GOM[i].m_strTitle, _T(""));       // ROI Label Title 
	    m_pData.m_GOM[i].m_nGOMType = GOM_RECT;            // OBJECT TYPE (점, 직선, 사각, 원, 타원 등) 
	    m_pData.m_GOM[i].m_nThick = 1L;              // OBJECT 두께 
		m_pData.m_GOM[i].m_bExecShowROI = TRUE;
		m_pData.m_GOM[i].m_bEjectROI = FALSE;
	    m_pData.m_GOM[i].m_bShowROI = TRUE;            // OBJECT그리기(Show/Hide)
	    m_pData.m_GOM[i].m_bShowTitle = TRUE;          // OBJECT TITLE(Show/Hide)
	    m_pData.m_GOM[i].m_bSelectedROI=FALSE;        // 마우스가 선택한 OBJECT(ON/OFF)
	    m_pData.m_GOM[i].m_ptPoint1.x = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint1.y = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint2.x = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint2.y = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint3.x = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint3.y = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint4.x = 0.0f;              // 영역의 첫번째 좌표 
	    m_pData.m_GOM[i].m_ptPoint4.y = 0.0f;              // 영역의 첫번째 좌표 
		for(j=0; j<SPARE_MAX_NUM; j++) 
		{
			m_pData.m_GOM[i].m_ptStart[j].x = 0.0f;
			m_pData.m_GOM[i].m_ptStart[j].y = 0.0f;
			m_pData.m_GOM[i].m_ptEnd[j].x = 0.0f;
			m_pData.m_GOM[i].m_ptEnd[j].y = 0.0f;
		}
	    m_pData.m_GOM[i].m_dCX = 0.0f;                 // 영역의 중심 좌표(X)
	    m_pData.m_GOM[i].m_dCY = 0.0f;                 // 영역의 중심 좌표(Y)
	    m_pData.m_GOM[i].m_dAngle = 0.0f;              // 기울어진 각도 
	    m_pData.m_GOM[i].m_dInRadius = 0.0f;           // 내부 반지름
	    m_pData.m_GOM[i].m_dOutRadius;                 // 밖의 반지름 
	    m_pData.m_GOM[i].m_nNormalColor = RGB(0, 255, 0);        // 보통 상태의 OBJECT COLOR
	    m_pData.m_GOM[i].m_nSelectedColor = RGB(255, 0, 0);      // 선택된 상태의 OBJECT COLOR// ROI 표시 
	}

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pData.m_INSP[i].m_bInspMode = FALSE;               // 해당 ROI 검사
		m_pData.m_INSP[i].m_bDecision = FALSE;               // 해당 ROI 판정 (결과값) : TRUE, FALSE / -1 : 사용하지 않음
		m_pData.m_INSP[i].m_nInspectType = INSP_MEASURE_SIDE_ONE;   // 위치 보정 형태
		m_pData.m_INSP[i].m_bLocROI = 0;
		m_pData.m_INSP[i].m_bScanMethod = 0;

		m_pData.m_INSP[i].m_nBlobOption = 1;
		m_pData.m_INSP[i].m_bBlobFeret = FALSE;
		m_pData.m_INSP[i].m_bOverlapRate = FALSE;
		m_pData.m_INSP[i].m_nBlobThresh = IMAGE_LEVEL/2;     // 문턱치
		m_pData.m_INSP[i].m_dBlobMinArea = 0.0f;             // Blob의 최소면적
		m_pData.m_INSP[i].m_dSumV = 0;                       // Blob의 최대면적
		m_pData.m_INSP[i].m_nBlobPolarity = POL_BLACK;       // Blob의 적용 POL

		m_pData.m_INSP[i].m_nPatScore = 70L;                 // 패턴 매칭율 
		m_pData.m_INSP[i].m_nPatSearchRate = 150L;           // 패턴의 사이즈에서 서치 영역을 계산 비율  
		m_pData.m_INSP[i].m_rcPatSearchROI = CRect(0, 0, 0, 0);      // Search Area
		m_pData.m_INSP[i].m_nPatWidth = 0L;                  // 패턴 가로 사이즈 
		m_pData.m_INSP[i].m_nPatHeight = 0L;                 // 패턴 세로 사이즈 
		m_pData.m_INSP[i].m_nPatNum=1;
		m_pData.m_INSP[i].m_nOutputNo=0;
		m_pData.m_INSP[i].m_nPlotSelNum=-1;
		m_pData.m_INSP[i].m_nSampleNum=0;

		for(j=0; j<MAX_PATTERN_NUM; j++)
    		m_pData.m_INSP[i].m_pPat[j] = NULL;              // Pattern 저장 
    
		m_pData.m_INSP[i].m_dTeachValue = 0.0f;
		m_pData.m_INSP[i].m_dSensity = 20;
		m_pData.m_INSP[i].m_nMaxThr  = 255;
		m_pData.m_INSP[i].m_nMinThr  = 0;
		m_pData.m_INSP[i].m_dStandLowLimit = 0.0f;      // 기준값 Low  Limit(SETUP)
		m_pData.m_INSP[i].m_dStandHighLimit = 0.0f;     // 기준값 High Limit(SETUP)
		m_pData.m_INSP[i].m_dStdSpec = 0.0f;      // 기준값 Low  Limit(SETUP)
		m_pData.m_INSP[i].m_dToleSpec = 0.0f;      // 기준값 Low  Limit(SETUP)
		m_pData.m_INSP[i].m_dOffsetV = 0.0f;     // 기준값 High Limit(SETUP)
		m_pData.m_INSP[i].m_dCX = 0.0f;                 // Object X 중심 
		m_pData.m_INSP[i].m_dCY = 0.0f;                 // Object Y 중심 
		m_pData.m_INSP[i].m_dRadius = 0.0f;
		m_pData.m_INSP[i].m_ptFirstMeas1.x = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptFirstMeas1.y = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptFirstMeas2.x = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptFirstMeas2.y = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptSecondMeas1.x = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptSecondMeas1.y = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptSecondMeas2.x = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_ptSecondMeas2.y = 0.0f;    // First  측정 위치 구함
		m_pData.m_INSP[i].m_dAnyValue = 0.0f;           // 임의 결과값 
		m_pData.m_INSP[i].m_nDefectCnt = 0;
		m_pData.m_INSP[i].m_nTGrayV = 0;
		for(j=0; j<DEFECT_RECT_NUM; j++)
   	        m_pData.m_INSP[i].m_rcPosFind[j] = CRect(0, 0, 0, 0);
	}

	for(i=0; i<PROCESS_MAX_NUM; i++)
	{
		_tcscpy_s(m_pData.m_Teach.m_PRE[i].m_strTitle, _T(""));       // 검사 Label Title
	    m_pData.m_Teach.m_PRE[i].m_bDecision = FALSE;
	    m_pData.m_Teach.m_PRE[i].m_bSel1 = FALSE;
	    m_pData.m_Teach.m_PRE[i].m_bSel2 = FALSE;
        m_pData.m_Teach.m_PRE[i].m_nIndex1 = -1;
        m_pData.m_Teach.m_PRE[i].m_nIndex2 = -1;
	    m_pData.m_Teach.m_PRE[i].m_dCX = (float)0.0;
	    m_pData.m_Teach.m_PRE[i].m_dCY = (float)0.0;
	    m_pData.m_Teach.m_PRE[i].m_dStandLowLimit = (float)0.0;
	    m_pData.m_Teach.m_PRE[i].m_dStandHighLimit = (float)0.0;
    }

	for(i=0; i<LOCATE_MAX_NUM; i++)
	{
		_tcscpy_s(m_pData.m_Teach.m_LOC[i].m_strTitle, _T(""));       // 검사 Label Title
	    m_pData.m_Teach.m_LOC[i].m_bDecision = FALSE;
	    m_pData.m_Teach.m_LOC[i].m_bSel1 = FALSE;
 	    m_pData.m_Teach.m_LOC[i].m_bSel2 = FALSE;
		m_pData.m_Teach.m_LOC[i].m_bLinkExec = TRUE;
		m_pData.m_Teach.m_LOC[i].m_bEjectROI = FALSE;
        m_pData.m_Teach.m_LOC[i].m_nIndex1 = -1;
        m_pData.m_Teach.m_LOC[i].m_nIndex2 = -1;
	    m_pData.m_Teach.m_LOC[i].m_dCX = (float)0.0;
	    m_pData.m_Teach.m_LOC[i].m_dCY = (float)0.0;
	    m_pData.m_Teach.m_LOC[i].m_dStandLowLimit = (float)0.0;
	    m_pData.m_Teach.m_LOC[i].m_dStandHighLimit = (float)0.0;
	    m_pData.m_Teach.m_LOC[i].m_dStdSpec = (float)0.0;
		m_pData.m_Teach.m_LOC[i].m_dToleSpec = (float)0.0;
	    m_pData.m_Teach.m_LOC[i].m_dOffsetV = (float)0.0;
    }

	for(i=0; i<CAM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pData.m_Teach.m_CAM[i].m_strTitle, _T(""));       // 검사 Label Title
	    m_pData.m_Teach.m_CAM[i].m_bDecision = FALSE;
	    m_pData.m_Teach.m_CAM[i].m_bSel1 = FALSE;
 	    m_pData.m_Teach.m_CAM[i].m_bSel2 = FALSE;
		m_pData.m_Teach.m_CAM[i].m_bLinkExec = TRUE;
		m_pData.m_Teach.m_CAM[i].m_bEjectROI = FALSE;
        m_pData.m_Teach.m_CAM[i].m_nIndex1 = -1;
        m_pData.m_Teach.m_CAM[i].m_nIndex2 = -1;
	    m_pData.m_Teach.m_CAM[i].m_dCX = (float)0.0;
	    m_pData.m_Teach.m_CAM[i].m_dCY = (float)0.0;
	    m_pData.m_Teach.m_CAM[i].m_dStandLowLimit = (float)0.0;
	    m_pData.m_Teach.m_CAM[i].m_dStandHighLimit = (float)0.0;
	    m_pData.m_Teach.m_CAM[i].m_dStdSpec = (float)0.0;
		m_pData.m_Teach.m_CAM[i].m_dToleSpec = (float)0.0;
	    m_pData.m_Teach.m_CAM[i].m_dOffsetV = (float)0.0;
    }

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		_tcscpy_s(m_pData.m_Teach.m_LINK[i].m_strTitle, _T(""));       // 검사 Label Title
	    m_pData.m_Teach.m_LINK[i].m_bDecision = FALSE;
	    m_pData.m_Teach.m_LINK[i].m_bSel1 = FALSE;
	    m_pData.m_Teach.m_LINK[i].m_bSel2 = FALSE;
		m_pData.m_Teach.m_LINK[i].m_bLinkExec = TRUE;
		m_pData.m_Teach.m_LINK[i].m_bEjectROI = FALSE;
        m_pData.m_Teach.m_LINK[i].m_nIndex1 = -1;
        m_pData.m_Teach.m_LINK[i].m_nIndex2 = -1;
	    m_pData.m_Teach.m_LINK[i].m_dCX = (float)0.0;
	    m_pData.m_Teach.m_LINK[i].m_dCY = (float)0.0;
	    m_pData.m_Teach.m_LINK[i].m_dStandLowLimit = (float)0.0;
	    m_pData.m_Teach.m_LINK[i].m_dStandHighLimit = (float)0.0;
 	    m_pData.m_Teach.m_LINK[i].m_dStdSpec = (float)0.0;
		m_pData.m_Teach.m_LINK[i].m_dToleSpec = (float)0.0;
	    m_pData.m_Teach.m_LINK[i].m_dOffsetV = (float)0.0;
		m_pData.m_Teach.m_LINK[i].m_dValueA = (float)0.0;;
	}

	memcpy(&m_pBackData, &m_pData, sizeof(SETUP_MODEL_DATA));
}

void CModelData::CloseModelData()
{
	long i, j;

	for(i=0; i<GOM_MAX_NUM; i++)
	for(j=0; j<MAX_PATTERN_NUM; j++)
	{
		if (m_pData.m_INSP[i].m_pPat[j]) {
			delete m_pData.m_INSP[i].m_pPat[j];
			m_pData.m_INSP[i].m_pPat[j] = NULL;
		}

		if (m_pBackData.m_INSP[i].m_pPat[j]) {
			delete m_pBackData.m_INSP[i].m_pPat[j];
			m_pBackData.m_INSP[i].m_pPat[j] = NULL;
		}
	}
}

void CModelData::CreateModelData()
{
	CloseModelData();
    InitModelData();
}

BOOL CModelData::LoadModelData(CString strFileName) 
{
	BOOL  bDel, bRet = FALSE;
	long  i, j, nPatWidth, nPatHeight, nPatNum, nType;
	CFile f;
	CString str;

	CreateModelData();
	if (f.Open(strFileName, CFile::modeRead)) 
	{
		f.Read(&m_pBackData, sizeof(SETUP_MODEL_DATA));
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			str.Format(_T("%s"), m_pBackData.m_GOM[i].m_strTitle);
			nType = m_pBackData.m_GOM[i].m_nGOMType;

			bDel = FALSE;
			if (str != _T("") && nType<GOM_LINE)
			{
				bDel = TRUE;
				_tcscpy_s(m_pBackData.m_GOM[i].m_strTitle, _T(""));
			}

			nPatWidth = m_pBackData.m_INSP[i].m_nPatWidth; 
			nPatHeight = m_pBackData.m_INSP[i].m_nPatHeight;  
			nPatNum = m_pBackData.m_INSP[i].m_nPatNum;

			for(j=0; j<nPatNum; j++)
			{
				m_pBackData.m_INSP[i].m_pPat[j] = NULL;
				if (nPatWidth>0 && nPatHeight>0)
				{
					m_pBackData.m_INSP[i].m_pPat[j] = new unsigned char[nPatWidth*nPatHeight];
    				f.Read(m_pBackData.m_INSP[i].m_pPat[j], sizeof(unsigned char)*nPatWidth*nPatHeight);  

					if (bDel)
					{
						delete m_pBackData.m_INSP[i].m_pPat[j];
						m_pBackData.m_INSP[i].m_pPat[j] = NULL;
					}
				}
			}

			if (bDel)
			{
				nPatWidth = 0;
				nPatHeight = 0;
				nPatNum = 0;
			}
		}

		f.Close();

		if (m_pBackData.m_dSourX2<0) m_pBackData.m_dSourX2=0;
		if (m_pBackData.m_dSourY2<0) m_pBackData.m_dSourY2=0;
		if (m_pBackData.m_dDeltaX2<-1000) m_pBackData.m_dDeltaX2=0;
		if (m_pBackData.m_dDeltaY2<-1000) m_pBackData.m_dDeltaY2=0;
		if (m_pBackData.m_dDeltaT2<-360) m_pBackData.m_dDeltaT2=0;

		if (m_pBackData.m_dSourX2>m_pBackData.m_nImageWidth) m_pBackData.m_dSourX2=0;
		if (m_pBackData.m_dSourY2>m_pBackData.m_nImageHeight) m_pBackData.m_dSourY2=0;
		if (m_pBackData.m_dDeltaX2>m_pBackData.m_nImageWidth) m_pBackData.m_dDeltaX2=0;
		if (m_pBackData.m_dDeltaY2>m_pBackData.m_nImageHeight) m_pBackData.m_dDeltaY2=0;
		if (m_pBackData.m_dDeltaT2>360) m_pBackData.m_dDeltaT2=0;

		CopyModelData(&m_pBackData, &m_pData);
	}

	return TRUE;
}

BOOL CModelData::SaveModelData(CString strFileName) 
{
	BOOL  bRet = FALSE;
	CString strPathName;
	TCHAR szbuf[128];
	long  i, j;
	long  nLen, nPatNum, nPatWidth, nPatHeight;
	CFile f;

    strPathName = _T("");
	_tcscpy_s(szbuf, _T(""));

	nLen = strFileName.GetLength();
	if (nLen>8)
	{
	    strPathName = strFileName.Left(nLen-8);
    	CreateDirectoryFullPath(strPathName);
	}

	if (f.Open(strFileName, CFile::modeCreate|CFile::modeWrite)) 
	{
		f.Write(&m_pBackData, sizeof(SETUP_MODEL_DATA));
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			nPatWidth = m_pBackData.m_INSP[i].m_nPatWidth; 
			nPatHeight = m_pBackData.m_INSP[i].m_nPatHeight;  
			nPatNum = m_pBackData.m_INSP[i].m_nPatNum;

			for(j=0; j<nPatNum; j++)
			{
				if (nPatWidth>0 && nPatHeight>0)
    				f.Write(m_pBackData.m_INSP[i].m_pPat[j], sizeof(unsigned char)*nPatWidth*nPatHeight);
			}
		}

		f.Close();
	}

	return TRUE;
}

void CModelData::SetSelectedROI(CString strTitle)
{
	long i, nPos=-1;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
		{
			m_pData.m_GOM[i].m_bShowROI = TRUE;
			m_pData.m_GOM[i].m_bSelectedROI = TRUE;
		}
		else
			m_pData.m_GOM[i].m_bSelectedROI = FALSE;
	}
}

void CModelData::DeleteSelectedROI()
{
	long i, j, k;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (m_pData.m_GOM[i].m_bSelectedROI)
		{
			m_pData.m_INSP[i].m_nInspectType = INSP_MEASURE_SIDE_ONE;

            _tcscpy_s(m_pData.m_GOM[i].m_strTitle, _T(""));
            m_pData.m_GOM[i].m_nGOMType = NONE_TYPE;
			m_pData.m_GOM[i].m_bSelectedROI = FALSE;
			m_pData.m_GOM[i].m_bEjectROI = FALSE;

			j=0;
			while(j<LOCATE_MAX_NUM)
			{
				if (i==m_pData.m_Teach.m_LOC[j].m_nIndex1 || 
					i==m_pData.m_Teach.m_LOC[j].m_nIndex2) 
				{
					for(k=j; k<LOCATE_MAX_NUM-1; k++)
				    	memcpy(&m_pData.m_Teach.m_LOC[k], &m_pData.m_Teach.m_LOC[k+1], sizeof(TEACH_ATTR));

					m_pData.m_Teach.m_nLocNum--;
					continue;
				}
				j++;
			}

			j=0;
			while(j<LINK_MAX_NUM)
			{
				if (i==m_pData.m_Teach.m_LINK[j].m_nIndex1 || 
					i==m_pData.m_Teach.m_LINK[j].m_nIndex2) 
				{
					for(k=j; k<LINK_MAX_NUM-1; k++)
				    	memcpy(&m_pData.m_Teach.m_LINK[k], &m_pData.m_Teach.m_LINK[k+1], sizeof(TEACH_ATTR));

					m_pData.m_Teach.m_nLinkNum--;
					continue;
				}
				j++;
			}

			break;
		}
	}
}

void CModelData::DeleteROI(LPCTSTR strTitle)
{
	long i;
	if (strTitle == _T("")) return;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
		{
            _tcscpy_s(m_pData.m_GOM[i].m_strTitle, _T(""));
			m_pData.m_GOM[i].m_bSelectedROI = FALSE;
			m_pData.m_GOM[i].m_bEjectROI = FALSE;
			m_pData.m_INSP[i].m_nInspectType = INSP_MEASURE_SIDE_ONE;
		}
	}
}

void CModelData::DeleteAllROI()
{
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pData.m_GOM[i].m_strTitle, _T(""));
		m_pData.m_GOM[i].m_nGOMType = NONE_TYPE;
  		m_pData.m_GOM[i].m_bSelectedROI = FALSE;
		m_pData.m_GOM[i].m_bEjectROI = FALSE;
		m_pData.m_INSP[i].m_nInspectType = INSP_MEASURE_SIDE_ONE;
    }
}

BOOL CModelData::CopyFrontToRear()
{
	BOOL bRet, bF, bR;
	long i, nPos;
	
	bF = FALSE;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")) && !m_pData.m_INSP[i].m_bLocROI)
		{
			bF = !m_pData.m_INSP[i].m_bLocROI;
			break;
		}
	}

	bR = FALSE;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")) && m_pData.m_INSP[i].m_bLocROI)
		{
			bR = m_pData.m_INSP[i].m_bLocROI;
			break;
		}
	}

	bRet = FALSE;
	if (bF && bR) 
	{
		bRet = TRUE;
		return bRet;
	}

	if (bF)
	{
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			if (_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")) && !m_pData.m_INSP[i].m_bLocROI)
			{
				nPos = GetEmptyROI();
				if (nPos<0) 
				{
					bRet = TRUE;
					return bRet;
				}

				AppendCopyROI(nPos, i);
				m_pData.m_INSP[nPos].m_bLocROI = TRUE;
			}
		}
	}
	else
	{
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			if (_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")) && m_pData.m_INSP[i].m_bLocROI)
			{
				nPos = GetEmptyROI();
				if (nPos<0) 
				{
					bRet = TRUE;
					return bRet;
				}

				AppendCopyROI(nPos, i);
				m_pData.m_INSP[nPos].m_bLocROI = FALSE;
			}
		}
	}

	return bRet;
}

void CModelData::AppendCopyROI(long nDest, long nSour)
{
	LPBYTE pPat;
	long i, nWidth, nHeight, nPatNum;

	for(i=0; i<MAX_PATTERN_NUM; i++)
	{
		if (m_pData.m_INSP[nDest].m_pPat[i])
		{
			delete m_pData.m_INSP[nDest].m_pPat[i];
			m_pData.m_INSP[nDest].m_pPat[i] = NULL;
		}
	}

	memcpy(&m_pData.m_GOM[nDest], &m_pData.m_GOM[nSour], sizeof(GOM_ATTR));
	memcpy(&m_pData.m_Scan[nDest], &m_pData.m_Scan[nSour], sizeof(SCAN_ATTR));
	memcpy(&m_pData.m_INSP[nDest], &m_pData.m_INSP[nSour], sizeof(INSPECT_DATA));

	nWidth = m_pData.m_INSP[nSour].m_nPatWidth;
	nHeight = m_pData.m_INSP[nSour].m_nPatHeight;
	nPatNum = m_pData.m_INSP[nSour].m_nPatNum;

	for(i=0; i<nPatNum; i++)
	{
		pPat = m_pData.m_INSP[nSour].m_pPat[i];
		if (nWidth>0 && nHeight>0 && pPat>0)
		{
			m_pData.m_INSP[nDest].m_pPat[i] = new unsigned char[nWidth*nHeight];
			memcpy(m_pData.m_INSP[nDest].m_pPat[i], pPat, sizeof(unsigned char)*nWidth*nHeight);
		}
	}
}

long CModelData::GetEmptyROI()
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")))
		{
            nPos = i;
			break;
		}
	}

	return nPos;
}

void CModelData::SetLimitData(LPCTSTR strTitle, float fLowL, float fHighL)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle)) continue;

		m_pData.m_INSP[i].m_dStandLowLimit = fLowL;
		m_pData.m_INSP[i].m_dStandHighLimit = fHighL;
		m_pBackData.m_INSP[i].m_dStandLowLimit = fLowL;
		m_pBackData.m_INSP[i].m_dStandHighLimit = fHighL;
	}
}

void CModelData::SetLinkLimitData(LPCTSTR strTitle, float fLowL, float fHighL)
{
	long i, nNum;

	nNum = m_pData.m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		if (_tcscmp(m_pData.m_Teach.m_LINK[i].m_strTitle, strTitle)) continue;

		m_pData.m_Teach.m_LINK[i].m_dStandLowLimit = fLowL;
		m_pData.m_Teach.m_LINK[i].m_dStandHighLimit = fHighL;
		m_pBackData.m_Teach.m_LINK[i].m_dStandLowLimit = fLowL;
		m_pBackData.m_Teach.m_LINK[i].m_dStandHighLimit = fHighL;
	}
}

void CModelData::SetMoveROI(long nDir, BOOL bLoc)
{
	long i;
	POINT2 pt1, pt2, pt3, pt4;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		pt1 = m_pData.m_GOM[i].m_ptPoint1;
		pt2 = m_pData.m_GOM[i].m_ptPoint2;
		pt3 = m_pData.m_GOM[i].m_ptPoint3;
		pt4 = m_pData.m_GOM[i].m_ptPoint4;

		if (m_pData.m_INSP[i].m_bLocROI == bLoc)
		{
			if (nDir==0) 
			{
				pt1.x-=5;
				pt2.x-=5;
				pt3.x-=5;
				pt4.x-=5;
			}
			if (nDir==1) 
			{
				pt1.x+=5;
				pt2.x+=5;
				pt3.x+=5;
				pt4.x+=5;
			}
			if (nDir==2) 
			{
				pt1.y-=5;
				pt2.y-=5;
				pt3.y-=5;
				pt4.y-=5;
			}
			if (nDir==3) 
			{
				pt1.y+=5;
				pt2.y+=5;
				pt3.y+=5;
				pt4.y+=5;
			}

			if (pt1.x>10 && pt2.x<m_pData.m_nImageWidth-10 &&
				pt3.x>10 && pt4.x<m_pData.m_nImageWidth-10 &&
				pt1.y>10 && pt3.y<m_pData.m_nImageHeight-10 && 
				pt2.y>10 && pt4.y<m_pData.m_nImageHeight-10)
			{
				m_pData.m_GOM[i].m_ptPoint1 = pt1;
				m_pData.m_GOM[i].m_ptPoint2 = pt2;
				m_pData.m_GOM[i].m_ptPoint3 = pt3;
				m_pData.m_GOM[i].m_ptPoint4 = pt4;
			}	 
		}
	}
}

void CModelData::SetSpecData(INSP_RESULT_DATA *pResData)
{
	long i;
	CString str, strT;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pBackData.m_GOM[i].m_bExecShowROI = pResData->m_bExecROI[i];
		m_pBackData.m_GOM[i].m_bEjectROI = pResData->m_bEjectROI[i];
		m_pBackData.m_INSP[i].m_dAnyValue = pResData->m_dValue[i];
		m_pBackData.m_INSP[i].m_nPlotSelNum = pResData->m_nPlotNum[i];
		m_pBackData.m_INSP[i].m_nOutputNo = pResData->m_nOutNo[i];
		m_pBackData.m_INSP[i].m_bDecision = pResData->m_bDecisionROI[i];
		m_pBackData.m_INSP[i].m_dStdSpec = pResData->m_dStdSpec[i];
		m_pBackData.m_INSP[i].m_dToleSpec = pResData->m_dToleSpec[i];
		m_pBackData.m_INSP[i].m_dStandLowLimit = pResData->m_dLowLimit[i];
		m_pBackData.m_INSP[i].m_dStandHighLimit = pResData->m_dHighLimit[i];
		m_pBackData.m_INSP[i].m_nPatScore = (long)pResData->m_dLowLimit[i];
	}

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		m_pBackData.m_Teach.m_LINK[i].m_bLinkExec = pResData->m_bLinkExecROI[i];
		m_pBackData.m_Teach.m_LINK[i].m_bEjectROI = pResData->m_bLinkEjectROI[i];
		m_pBackData.m_Teach.m_LINK[i].m_dValue = pResData->m_dLinkValue[i];
		m_pBackData.m_Teach.m_LINK[i].m_nPlotSelNum = pResData->m_nLinkPlotNum[i];
		m_pBackData.m_Teach.m_LINK[i].m_nOutputNo = pResData->m_nLinkOutNo[i];
		m_pBackData.m_Teach.m_LINK[i].m_bDecision = pResData->m_bLinkDecROI[i];
		m_pBackData.m_Teach.m_LINK[i].m_dStdSpec = pResData->m_dLinkStdSpec[i];
		m_pBackData.m_Teach.m_LINK[i].m_dToleSpec = pResData->m_dLinkToleSpec[i];
		m_pBackData.m_Teach.m_LINK[i].m_dStandLowLimit = pResData->m_dLinkLowLimit[i];
		m_pBackData.m_Teach.m_LINK[i].m_dStandHighLimit = pResData->m_dLinkHighLimit[i];
	}

	m_pBackData.m_bRealTimeMode = m_pData.m_bRealTimeMode;
	CopyModelData(&m_pBackData, &m_pData);
}

void CModelData::SetRealTimeMode(BOOL bRealTime)
{
	m_pData.m_bRealTimeMode = bRealTime;
}

CString CModelData::GetTitleSelectedROI()
{
	long i;
	CString  strTitle;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (m_pData.m_GOM[i].m_bSelectedROI)
		{
            strTitle = m_pData.m_GOM[i].m_strTitle;
			break;
		}
	}

	return strTitle;
}

long CModelData::GetIndexSelectedROI()
{
	long i, nPos=-1;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (m_pData.m_GOM[i].m_bSelectedROI)
		{
            nPos = i;
			break;
		}
	}

	return nPos;
}

void CModelData::GetRegionROI(long nSel, long *x1, long *y1, long *x2, long *y2)
{
	*x1 = (long)m_pData.m_GOM[nSel].m_ptPoint1.x;
	*y1 = (long)m_pData.m_GOM[nSel].m_ptPoint1.y;
	*x2 = (long)m_pData.m_GOM[nSel].m_ptPoint4.x;
	*y2 = (long)m_pData.m_GOM[nSel].m_ptPoint4.y;
}

CString CModelData::GetSelectedTitle()
{
	CString strTitle;
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (m_pData.m_GOM[i].m_bSelectedROI)
		{
            strTitle = m_pData.m_GOM[i].m_strTitle;
			break;
		}
	}

	return strTitle;
}

long CModelData::GetIndexROI(CString strTitle)
{
	long i, nPos=-1;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
		{
            nPos = i;
			break;
		}
	}

	return nPos;
}

long CModelData::GetNextROI()
{
	BOOL bFlag;
	long i, j, nSel=1;
	CString str;

	for(i=1; i<=GOM_MAX_NUM; i++)
	{
		str.Format(_T("%d"), i);
		bFlag = TRUE;
		for(j=0; j<GOM_MAX_NUM; j++)
		{
    		if (!_tcscmp(m_pData.m_GOM[j].m_strTitle, str))
			{
				bFlag = FALSE;
				break;
			}
		}

		if (bFlag) 
		{
			nSel = i;
			break;
		}
	}

	return nSel;
}

void CModelData::ShowROI(LPCTSTR strTitle, BOOL bShow)
{
	long i;

	if (strTitle == _T("")) return;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
            m_pData.m_GOM[i].m_bShowROI = bShow;
	}
}

void CModelData::ShowAllROI(BOOL bShow)
{
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
        m_pData.m_GOM[i].m_bShowROI = bShow;
}

void CModelData::ShowTitleROI(LPCTSTR strTitle, BOOL bShow)
{
	long i;

	if (strTitle == _T("")) return;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
		{
            m_pData.m_GOM[i].m_bShowTitle = bShow;
			break;
		}
	}
}

void CModelData::SetRepositionROI(float dSX, float dSY, float dDX, float dDY, float dDT, long nImageWidth, long nImageHeight)
{
	long i;
	GOM_ATTR *pGOM;
	CPoint ptStart, ptEnd;
	POINT2 ptDot1, ptDot2, ptDot3, ptDot4;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, _T(""))) continue;
		pGOM = &m_pData.m_GOM[i];

		ptDot1 = pGOM->m_ptPoint1;
		ptDot2 = pGOM->m_ptPoint2;
		ptDot3 = pGOM->m_ptPoint3;
		ptDot4 = pGOM->m_ptPoint4;

		ptDot1.x = (float)(dSX + (pGOM->m_ptPoint1.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint1.y-dSY) * sin(dDT*RAD));
		ptDot1.y = (float)(dSY + (pGOM->m_ptPoint1.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint1.x-dSX) * sin(dDT*RAD));
		ptDot2.x = (float)(dSX + (pGOM->m_ptPoint2.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint2.y-dSY) * sin(dDT*RAD));
		ptDot2.y = (float)(dSY + (pGOM->m_ptPoint2.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint2.x-dSX) * sin(dDT*RAD));
		ptDot3.x = (float)(dSX + (pGOM->m_ptPoint3.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint3.y-dSY) * sin(dDT*RAD));
		ptDot3.y = (float)(dSY + (pGOM->m_ptPoint3.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint3.x-dSX) * sin(dDT*RAD));
		ptDot4.x = (float)(dSX + (pGOM->m_ptPoint4.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint4.y-dSY) * sin(dDT*RAD));
		ptDot4.y = (float)(dSY + (pGOM->m_ptPoint4.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint4.x-dSX) * sin(dDT*RAD));

		pGOM->m_ptPoint1.x = ptDot1.x + dDX;
		pGOM->m_ptPoint1.y = ptDot1.y + dDY;
		pGOM->m_ptPoint2.x = ptDot2.x + dDX;
		pGOM->m_ptPoint2.y = ptDot2.y + dDY;
		pGOM->m_ptPoint3.x = ptDot3.x + dDX;
		pGOM->m_ptPoint3.y = ptDot3.y + dDY;
		pGOM->m_ptPoint4.x = ptDot4.x + dDX;
		pGOM->m_ptPoint4.y = ptDot4.y + dDY;

		if (pGOM->m_ptPoint1.x < 0) pGOM->m_ptPoint1.x = 0;
		if (pGOM->m_ptPoint1.y < 0) pGOM->m_ptPoint1.y = 0;
		if (pGOM->m_ptPoint2.x < 0) pGOM->m_ptPoint2.x = 0;
		if (pGOM->m_ptPoint2.y < 0) pGOM->m_ptPoint2.y = 0;
		if (pGOM->m_ptPoint3.x < 0) pGOM->m_ptPoint3.x = 0;
		if (pGOM->m_ptPoint3.y < 0) pGOM->m_ptPoint3.y = 0;
		if (pGOM->m_ptPoint4.x < 0) pGOM->m_ptPoint4.x = 0;
		if (pGOM->m_ptPoint4.y < 0) pGOM->m_ptPoint4.y = 0;

		if (pGOM->m_ptPoint1.x>=nImageWidth)  pGOM->m_ptPoint1.x = (float)(nImageWidth-1);
		if (pGOM->m_ptPoint1.y>=nImageHeight) pGOM->m_ptPoint1.y = (float)(nImageHeight-1);
		if (pGOM->m_ptPoint2.x>=nImageWidth)  pGOM->m_ptPoint2.x = (float)(nImageWidth-1);
		if (pGOM->m_ptPoint2.y>=nImageHeight) pGOM->m_ptPoint2.y = (float)(nImageHeight-1);
		if (pGOM->m_ptPoint3.x>=nImageWidth)  pGOM->m_ptPoint3.x = (float)(nImageWidth-1);
		if (pGOM->m_ptPoint3.y>=nImageHeight) pGOM->m_ptPoint3.y = (float)(nImageHeight-1);
		if (pGOM->m_ptPoint4.x>=nImageWidth)  pGOM->m_ptPoint4.x = (float)(nImageWidth-1);
		if (pGOM->m_ptPoint4.y>=nImageHeight) pGOM->m_ptPoint4.y = (float)(nImageHeight-1);
	}
}

BOOL CModelData::GetMidPointLocation(float *dPosX, float *dPosY, float *dPosT)
{
	BOOL   bRet=TRUE;
	POINT2 ptLoc[4];
	float  dSumX, dSumY;
	long   i, nPos, nNum;

	*dPosX = 0.0f;
	*dPosY = 0.0f;
	*dPosT = 0.0f;

	nNum = 0;
	dSumX = 0.0f;
	dSumY = 0.0f;
	for(i=0; i<m_pData.m_Teach.m_nLocNum; i++)
	{
		if (!m_pData.m_Teach.m_LOC[i].m_bSel1)
		{
			nPos = m_pData.m_Teach.m_LOC[i].m_nIndex1;
			if (m_pData.m_INSP[nPos].m_nInspectType==INSP_MEASURE_CIRCLE)
			{
				ptLoc[i].x = m_pData.m_GOM[nPos].m_dCX;
				ptLoc[i].y = m_pData.m_GOM[nPos].m_dCY;
			}
			else
			{
				ptLoc[i].x = (m_pData.m_GOM[nPos].m_ptPoint1.x+m_pData.m_GOM[nPos].m_ptPoint4.x)/2;
				ptLoc[i].y = (m_pData.m_GOM[nPos].m_ptPoint1.y+m_pData.m_GOM[nPos].m_ptPoint4.y)/2;
			}

			dSumX += ptLoc[i].x;
			dSumY += ptLoc[i].y;
			nNum++;
		}
	}

	if (nNum>0)
	{
		*dPosX = (dSumX/nNum);
		*dPosY = (dSumY/nNum);

		if (nNum==2)
		{
			m_libVis.GetAngle(ptLoc[0].x, ptLoc[0].y, ptLoc[1].x, ptLoc[1].y, dPosT);
			if (fabs(*dPosT)>45.0f)
				m_libVis.GetAngle(ptLoc[0].y, ptLoc[0].x, ptLoc[1].y, ptLoc[1].x, dPosT);
		}

		bRet = FALSE;
	}

	return bRet;
}

long CModelData::RegisterPattern(long nIndexROI, LPBYTE pImage, long x1, long y1, long x2, long y2, long nImageWidth, long nImageHeight, long nPatNum, long *nWidth, long *nHeight)
{
	LPBYTE pBuffer, pPAT=NULL;
	long i, j, k, nGOMType;
	long nPatWidth, nPatHeight;
	double dAngle;
	CPoint pt1, pt2;

	pBuffer = new unsigned char[nImageWidth*nImageHeight];

	nGOMType = m_pData.m_GOM[nIndexROI].m_nGOMType;
	if (nIndexROI>=0 && nGOMType==GOM_RECT)
	{
		nPatWidth = (long)(x2 - x1 + 1);
		nPatHeight = (long)(y2 - y1 + 1);

		pt1.x = x1 - 30;
		pt1.y = y1 - 30;
		pt2.x = x2 + 30;
		pt2.y = y2 + 30;
		if (pt1.x<0) pt1.x = 0;  
		if (pt1.y<0) pt1.y = 0;  
		if (pt2.x>=nImageWidth)  pt2.x = nImageWidth-1;  
		if (pt2.y>=nImageHeight) pt2.y = nImageHeight-1;  

		for(i=0; i<MAX_PATTERN_NUM; i++)
		{
			if (m_pData.m_INSP[nIndexROI].m_pPat[i])
			{
				delete m_pData.m_INSP[nIndexROI].m_pPat[i];
				m_pData.m_INSP[nIndexROI].m_pPat[i] = NULL;
			}
		}

		for(k=0; k<nPatNum; k++)
		{
			pPAT = NULL;
			if (nPatWidth>0 && nPatHeight>0)
			{
				m_pData.m_INSP[nIndexROI].m_pPat[k] = new unsigned char[nPatWidth*nPatHeight];
				pPAT = m_pData.m_INSP[nIndexROI].m_pPat[k];
			}

			if (k==1) dAngle = -1.5;
			else if (k==2) dAngle = 1.5;
			else if (k==3) dAngle = -3.0;
			else if (k==4) dAngle = 3.0;
			else dAngle = 0;

			NVisionDll_ImageRotation(pImage, pBuffer, pt1.x, pt1.y, pt2.x, pt2.y, nImageWidth, nImageHeight, (float)dAngle);

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
				pPAT[i*nPatWidth + j] = pBuffer[(i+y1)*nImageWidth + (j+x1)];
		}

		pPAT = m_pData.m_INSP[nIndexROI].m_pPat[0];

		*nWidth = nPatWidth;
		*nHeight = nPatHeight;
	}

	delete pBuffer;
	return (long)pPAT;
}

long CModelData::RegisterBLOB(long nIndexROI, LPBYTE pImage, long x1, long y1, long x2, long y2, long nImageWidth, long nImageHeight, long nPol, long nThresh, long *nWidth, long *nHeight)
{
	LPBYTE pPAT=NULL;
	long  i, j, nGOMType, nColor;
	long  nGray, nPatWidth, nPatHeight;
	float dCX, dCY, dInR, dOutR; 
	float a, b, dDist;

	*nWidth = 0;
	*nHeight = 0;
	nGOMType = m_pData.m_GOM[nIndexROI].m_nGOMType;

	if (nIndexROI<0) return (long)pPAT;
	if (nGOMType==GOM_CIRCLE2)
	{
		dCX = m_pData.m_GOM[nIndexROI].m_dCX;
		dCY = m_pData.m_GOM[nIndexROI].m_dCY;
		dInR = m_pData.m_GOM[nIndexROI].m_dInRadius;
		dOutR = m_pData.m_GOM[nIndexROI].m_dOutRadius;

		if (nPol==POL_WHITE) nColor = 0;
		else nColor = 255;

		x1 = (long)(dCX-dOutR);
		y1 = (long)(dCY-dOutR);
		x2 = (long)(dCX+dOutR);
		y2 = (long)(dCY+dOutR);

		if (x1<0) x1 = 0;
		if (y1<0) y1 = 0;
		if (x2>=nImageWidth)  x2 = nImageWidth-1;
		if (y2>=nImageHeight) y2 = nImageHeight-1;
	}

	nPatWidth = (long)(x2 - x1 + 1);
	nPatHeight = (long)(y2 - y1 + 1);

	for(i=0; i<MAX_PATTERN_NUM; i++)
	{
		if (m_pData.m_INSP[nIndexROI].m_pPat[i])
		{
			delete m_pData.m_INSP[nIndexROI].m_pPat[i];
			m_pData.m_INSP[nIndexROI].m_pPat[i] = NULL;
		}
	}

	pPAT = NULL;
	if (nPatWidth>0 && nPatHeight>0)
	{
		m_pData.m_INSP[nIndexROI].m_pPat[0] = new unsigned char[nPatWidth*nPatHeight];
		pPAT = m_pData.m_INSP[nIndexROI].m_pPat[0];
	}

	*nWidth = nPatWidth;
	*nHeight = nPatHeight;

	if (nGOMType==GOM_RECT)
	{
		for(i=0; i<nPatHeight; i++)
		for(j=0; j<nPatWidth; j++)
		{
			nGray = pImage[(i+y1)*nImageWidth + (j+x1)];
			if (nGray>nThresh) 
				pPAT[i*nPatWidth + j] = WHITE_LEVEL;
			else 
				pPAT[i*nPatWidth + j] = BLACK_LEVEL;
		}
	}

	if (nGOMType==GOM_CIRCLE2)
	{
		for(i=0; i<nPatHeight; i++)
		for(j=0; j<nPatWidth; j++)
		{
			a = (i+y1)-dCY;
			b = (j+x1)-dCX;
			dDist = (float)sqrt(a*a + b*b);

			nGray = pImage[(i+y1)*nImageWidth + (j+x1)];
			if (dDist>=dInR && dDist<=dOutR)
			{
				if (nGray>nThresh)
					pPAT[i*nPatWidth + j] = WHITE_LEVEL;
				else 
					pPAT[i*nPatWidth + j] = BLACK_LEVEL;
			}
			else
				pPAT[i*nPatWidth + j] = (unsigned char)nColor;
		}
	}

	return (long)pPAT;
}

void CModelData::CopyROI(long nSelectedROI)
{
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, _T(""))) 
		{
			m_pData.m_GOM[nSelectedROI].m_bSelectedROI = FALSE;
            m_pData.m_GOM[i] = m_pData.m_GOM[nSelectedROI];
			m_pData.m_Scan[i] = m_pData.m_Scan[nSelectedROI];

		    m_pData.m_Scan[i].m_nFirstNum = 0;
            m_pData.m_Scan[i].m_nSecondNum = 0;

			m_pData.m_INSP[i].m_bInspMode = m_pData.m_INSP[nSelectedROI].m_bInspMode;           // 해당 ROI 검사
			m_pData.m_INSP[i].m_bDecision = m_pData.m_INSP[nSelectedROI].m_bDecision;           // 해당 ROI 검사
			m_pData.m_INSP[i].m_nInspectType = m_pData.m_INSP[nSelectedROI].m_nInspectType;     // 해당 ROI 검사
			m_pData.m_INSP[i].m_nPatWidth = 0;
			m_pData.m_INSP[i].m_nPatHeight = 0;
            m_pData.m_INSP[i].m_nPatNum = 0;
			
			InitPosROI(i, m_pData.m_GOM[nSelectedROI].m_nGOMType);
			break;
		}
	}
}

void CModelData::CopyModelData(SETUP_MODEL_DATA *pSour, SETUP_MODEL_DATA *pDest)
{
	long i;

	memcpy(pDest->m_strTitle, pSour->m_strTitle, sizeof(WCHAR)*100);       // 검사면의 타이틀 (SETUP)
	pDest->m_bRealTimeMode = pSour->m_bRealTimeMode;
	pDest->m_bSetupMode = pSour->m_bSetupMode;
    pDest->m_bDecision = pSour->m_bDecision;

    pDest->m_dSourY = pSour->m_dSourY;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourX = pSour->m_dSourX;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourY = pSour->m_dSourY;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourT = pSour->m_dSourT;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaX = pSour->m_dDeltaX;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaY = pSour->m_dDeltaY;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaT = pSour->m_dDeltaT;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)

	pDest->m_dSourY2 = pSour->m_dSourY2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourX2 = pSour->m_dSourX2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourY2 = pSour->m_dSourY2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dSourT2 = pSour->m_dSourT2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaX2 = pSour->m_dDeltaX2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaY2 = pSour->m_dDeltaY2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
    pDest->m_dDeltaT2 = pSour->m_dDeltaT2;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)

	memcpy(&pDest->m_Enhence, &pSour->m_Enhence, sizeof(ENHENCE_DATA));
	memcpy(&pDest->m_IMG, &pSour->m_IMG, sizeof(IMAGE_DATA));
	memcpy(&pDest->m_CAL, &pSour->m_CAL, sizeof(CAL_DATA));
	memcpy(&pDest->m_Teach, &pSour->m_Teach, sizeof(TEACH_DATA));

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		pSour->m_GOM[i].m_bSelectedROI = FALSE;
		pSour->m_INSP[i].m_bInspMode = FALSE;

        CopySelectedROI(i, pSour, pDest);
	}
}

void CModelData::CopySelectedROI(long nSelectedROI, SETUP_MODEL_DATA *pSour, SETUP_MODEL_DATA *pDest)
{
	LPBYTE pPat;
	long i, nWidth, nHeight, nPatNum;

	for(i=0; i<MAX_PATTERN_NUM; i++)
	{
		if (pDest->m_INSP[nSelectedROI].m_pPat[i])
		{
			delete pDest->m_INSP[nSelectedROI].m_pPat[i];
			pDest->m_INSP[nSelectedROI].m_pPat[i] = NULL;
		}
	}

	memcpy(&pDest->m_GOM[nSelectedROI], &pSour->m_GOM[nSelectedROI], sizeof(GOM_ATTR));
	memcpy(&pDest->m_Scan[nSelectedROI], &pSour->m_Scan[nSelectedROI], sizeof(SCAN_ATTR));
	memcpy(&pDest->m_INSP[nSelectedROI], &pSour->m_INSP[nSelectedROI], sizeof(INSPECT_DATA));

	nWidth = pSour->m_INSP[nSelectedROI].m_nPatWidth;
	nHeight = pSour->m_INSP[nSelectedROI].m_nPatHeight;
	nPatNum = pSour->m_INSP[nSelectedROI].m_nPatNum;

	for(i=0; i<MAX_PATTERN_NUM; i++)
	    pDest->m_INSP[nSelectedROI].m_pPat[i] = NULL;

	for(i=0; i<nPatNum; i++)
	{
		pPat = pSour->m_INSP[nSelectedROI].m_pPat[i];
		if (nWidth>0 && nHeight>0 && pPat>0)
		{
			pDest->m_INSP[nSelectedROI].m_pPat[i] = new unsigned char[nWidth*nHeight];
			memcpy(pDest->m_INSP[nSelectedROI].m_pPat[i], pPat, sizeof(unsigned char)*nWidth*nHeight);
		}
	}

//	pDest->m_Scan[nSelectedROI].m_nSecondScanDirType;
}

long CModelData::AddROI(LPCTSTR strTitle, long nGOMType, long nInspType, long nThick, COLORREF nColor)
{
    long nIndex=-1;

	if (nGOMType<GOM_LINE || nGOMType>GOM_MASK_RECT) return nIndex;

   	nIndex = GetModifyInsertPos(strTitle);
	if (nIndex == -1) return nIndex;

	_tcscpy_s(m_pData.m_GOM[nIndex].m_strTitle, strTitle);
	m_pData.m_GOM[nIndex].m_nGOMType = nGOMType;
	m_pData.m_GOM[nIndex].m_nThick = nThick;
	m_pData.m_GOM[nIndex].m_nNormalColor = nColor;
	m_pData.m_GOM[nIndex].m_bExecShowROI = TRUE;
	m_pData.m_GOM[nIndex].m_bEjectROI = TRUE;
	m_pData.m_GOM[nIndex].m_bSelectedROI = TRUE;
    m_pData.m_GOM[nIndex].m_bShowROI = TRUE;
	m_pData.m_GOM[nIndex].m_bShowTitle = TRUE;
	m_pData.m_INSP[nIndex].m_nSampleNum = 1;
    m_pData.m_INSP[nIndex].m_nInspectType = nInspType;

	InitPosROI(nIndex, nGOMType);
	return nIndex;
}

void CModelData::InitPosROI(long nIndex, long nType)
{
	long nTemp, nW;

	nTemp = (long)((m_pData.m_nImageHeight + m_pData.m_nImageWidth)/2.0);
	nW = (long)(100*nTemp/1000.0);

	switch(nType)
	{
		case GOM_LINE :
		case GOM_RECT :
		{
			 m_pData.m_GOM[nIndex].m_dAngle = 0.0f;
			 m_pData.m_GOM[nIndex].m_ptPoint1.x = (float)(m_pData.m_nImageWidth/2);
			 m_pData.m_GOM[nIndex].m_ptPoint1.y = (float)(m_pData.m_nImageHeight/2);

			 m_pData.m_GOM[nIndex].m_ptPoint2.x = (float)(m_pData.m_nImageWidth/2+nW);
			 m_pData.m_GOM[nIndex].m_ptPoint2.y = (float)(m_pData.m_nImageHeight/2);

			 m_pData.m_GOM[nIndex].m_ptPoint3.x = (float)(m_pData.m_nImageWidth/2);
			 m_pData.m_GOM[nIndex].m_ptPoint3.y = (float)(m_pData.m_nImageHeight/2+nW);

			 m_pData.m_GOM[nIndex].m_ptPoint4.x = (float)(m_pData.m_nImageWidth/2+nW);
			 m_pData.m_GOM[nIndex].m_ptPoint4.y = (float)(m_pData.m_nImageHeight/2+nW);
			 break;
		}
	
		{
			 m_pData.m_GOM[nIndex].m_dCX = (float)(m_pData.m_nImageWidth/2);
			 m_pData.m_GOM[nIndex].m_dCY = (float)(m_pData.m_nImageHeight/2);
			 m_pData.m_GOM[nIndex].m_dInRadius = (float)nW;
			 break;
		}
		case GOM_CROSS :
		{
			 m_pData.m_GOM[nIndex].m_dCX = (float)200;
			 m_pData.m_GOM[nIndex].m_dCY = (float)200;
			 m_pData.m_GOM[nIndex].m_dInRadius = (float)15;
			 break;
		}
		case GOM_TEXT :
		{
			 m_pData.m_GOM[nIndex].m_ptPoint1.x = 100;
			 m_pData.m_GOM[nIndex].m_ptPoint1.y = 100;
			 break;
		}
		case GOM_CIRCLE2:
		{
		     m_pData.m_GOM[nIndex].m_dCX = 150;
			 m_pData.m_GOM[nIndex].m_dCY = 150;
             m_pData.m_GOM[nIndex].m_dInRadius = 50;
             m_pData.m_GOM[nIndex].m_dOutRadius = 80;
			 break;
		}
		case GOM_MASK_RECT :
		{
			 m_pData.m_GOM[nIndex].m_dAngle = 0.0f;
			 m_pData.m_GOM[nIndex].m_ptPoint1.x = 100;
			 m_pData.m_GOM[nIndex].m_ptPoint1.y = 100;

			 m_pData.m_GOM[nIndex].m_ptPoint2.x = 170;
			 m_pData.m_GOM[nIndex].m_ptPoint2.y = 100;

			 m_pData.m_GOM[nIndex].m_ptPoint3.x = 100;
			 m_pData.m_GOM[nIndex].m_ptPoint3.y = 170;

			 m_pData.m_GOM[nIndex].m_ptPoint4.x = 170;
			 m_pData.m_GOM[nIndex].m_ptPoint4.y = 170;
			 break;
		}
	}
}

BOOL CModelData::SetTransRectROI(long nSelectedROI, float dAngle)
{
	BOOL bRet=FALSE;
	POINT2 ptRect[4], ptMid;
	float dTemp;
	GOM_ATTR  *pGOM = &m_pData.m_GOM[nSelectedROI];

	dTemp = pGOM->m_dAngle-dAngle;
	ptMid.x = (pGOM->m_ptPoint1.x+pGOM->m_ptPoint2.x+pGOM->m_ptPoint3.x+pGOM->m_ptPoint4.x) / 4;
	ptMid.y = (pGOM->m_ptPoint1.y+pGOM->m_ptPoint2.y+pGOM->m_ptPoint3.y+pGOM->m_ptPoint4.y) / 4;

	ptRect[0].x = (float)(ptMid.x + (pGOM->m_ptPoint1.x-ptMid.x) * cos(dTemp * RAD) + (pGOM->m_ptPoint1.y-ptMid.y) * sin(dTemp * RAD));
	ptRect[0].y = (float)(ptMid.y + (pGOM->m_ptPoint1.y-ptMid.y) * cos(dTemp * RAD) - (pGOM->m_ptPoint1.x-ptMid.x) * sin(dTemp * RAD));
	ptRect[3].x = (float)(ptMid.x + (pGOM->m_ptPoint4.x-ptMid.x) * cos(dTemp * RAD) + (pGOM->m_ptPoint4.y-ptMid.y) * sin(dTemp * RAD));
	ptRect[3].y = (float)(ptMid.y + (pGOM->m_ptPoint4.y-ptMid.y) * cos(dTemp * RAD) - (pGOM->m_ptPoint4.x-ptMid.x) * sin(dTemp * RAD));

	m_libMath.GetCrossPointDotWithAngle(dAngle, ptRect[0], ptRect[3], &ptRect[1]);
	m_libMath.GetCrossPointDotWithAngle(dAngle+90, ptRect[0], ptRect[3], &ptRect[2]);

	if (ptRect[0].x+6<ptRect[1].x && ptRect[0].y+6<ptRect[2].y &&
		ptRect[2].x+6<ptRect[3].x && ptRect[1].y+6<ptRect[3].y)
	{
		pGOM->m_ptPoint1 = ptRect[0];
		pGOM->m_ptPoint2 = ptRect[1];
		pGOM->m_ptPoint3 = ptRect[2];
		pGOM->m_ptPoint4 = ptRect[3];

		pGOM->m_dAngle = dAngle;
		bRet = TRUE;
	}

	if (dAngle==0.0f)
	{
		pGOM->m_ptPoint2.x = ptRect[3].x;
		pGOM->m_ptPoint2.y = ptRect[0].y;

		pGOM->m_ptPoint3.x = ptRect[0].x;
		pGOM->m_ptPoint3.y = ptRect[3].y;
	}

	return bRet;
}

BOOL CModelData::GetModifyInsertPos(LPCTSTR strTitle)
{
	bool bModifyFlag, bInsertFlag;
	long i, nIndex;
	long nModifyPos, nInsertPos;

	if (strTitle == _T("")) return FALSE;

	bModifyFlag = false;
	bInsertFlag = false;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, strTitle))
		{
			bModifyFlag = true;
			nModifyPos = i;
			break;
		}

		if (!bInsertFlag)
		{
			if (!_tcscmp(m_pData.m_GOM[i].m_strTitle, _T("")))
			{
		    	nInsertPos = i;
				bInsertFlag = true;
			}			
		}
	}

	nIndex = -1;
	if (bModifyFlag) nIndex = nModifyPos;
	else if (bInsertFlag) nIndex = nInsertPos;

	return nIndex;
}

void CModelData::SetPressKey(long nCtrlKey, long nKey)
{
	long nROI;
	float x1, y1, x2, y2, fStepX, fStepY;

	nROI = GetIndexSelectedROI();
	if (nROI>=0)
	{
		fStepX = 0.0f;
		fStepY = 0.0f;

		if (nKey==VK_LEFT)  fStepX = -2.0f;
		if (nKey==VK_RIGHT) fStepX = 2.0f;
		if (nKey==VK_UP)    fStepY = -2.0f;
		if (nKey==VK_DOWN)  fStepY = 2.0f;

		if (nCtrlKey)
		{
			if (m_pData.m_INSP[nROI].m_nInspectType == INSP_PATTERN_MATCH)
				return;

			m_pData.m_GOM[nROI].m_ptPoint2.x += fStepX;
			m_pData.m_GOM[nROI].m_ptPoint4.x += fStepX;
			m_pData.m_GOM[nROI].m_ptPoint3.y += fStepY;
			m_pData.m_GOM[nROI].m_ptPoint4.y += fStepY;

			x1 = m_pData.m_GOM[nROI].m_ptPoint1.x;
			x2 = m_pData.m_GOM[nROI].m_ptPoint4.x;
			y1 = m_pData.m_GOM[nROI].m_ptPoint1.y;
			y2 = m_pData.m_GOM[nROI].m_ptPoint4.y;

			if (x2-x1<50)
			{
				m_pData.m_GOM[nROI].m_ptPoint2.x = x1+50;
				m_pData.m_GOM[nROI].m_ptPoint4.x = x1+50;
			}
		
			if (y2-y1<50)
			{
				m_pData.m_GOM[nROI].m_ptPoint3.y = y1+50;
				m_pData.m_GOM[nROI].m_ptPoint4.y = y1+50;
			}
		}
		else
		{
			m_pData.m_GOM[nROI].m_ptPoint1.x += fStepX;
			m_pData.m_GOM[nROI].m_ptPoint2.x += fStepX;
			m_pData.m_GOM[nROI].m_ptPoint3.x += fStepX;
			m_pData.m_GOM[nROI].m_ptPoint4.x += fStepX;

			m_pData.m_GOM[nROI].m_ptPoint1.y += fStepY;
			m_pData.m_GOM[nROI].m_ptPoint2.y += fStepY;
			m_pData.m_GOM[nROI].m_ptPoint3.y += fStepY;
			m_pData.m_GOM[nROI].m_ptPoint4.y += fStepY;
		}
	}
}
