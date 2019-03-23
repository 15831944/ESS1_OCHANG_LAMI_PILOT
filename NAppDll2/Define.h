#if !defined(_DEFINE_H)
#define _DEFINE_H

#define VISION_SETUP_VERSION         _T(" NVisionSetup VER 3.52 (19.02.25) for ESS ")

#pragma comment(lib, "../../VLib/General/NVisionDll.lib")

#define CAL_UP_FILE_PATH             _T("..\\CAL_UP.dat")
#define CAL_DN_FILE_PATH             _T("..\\CAL_DN.dat")
#define SYS_UP_FILE_PATH             _T("..\\SYS_UP.dat")
#define SYS_DN_FILE_PATH             _T("..\\SYS_DN.dat")
#define OFFSET_DATA_FILE             _T("..\\ConfigData.txt")

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////              비전 상수 정의            /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#define _DUO_CELL                      1
#define _NG_DISP                       1

#define DEF_INIT_ZOOM_SCALE		   0.175f
#define DEF_MINI_ZOOM_SCALE         0.10f

#define DEF_TILTING_ALG                0
#define DEF_TURNING_VALUE           0.05f
#define DEF_PIXEL_SCALE              0.0f
#define DEF_AVER_CAL_THRESH            0
#define DEF_MISMATCH_ERROR_SIZE      0.7f

#define IMAGE_WIDTH				     640
#define IMAGE_HEIGHT			     480
#define FRAME_WIDTH				     932
#define FRAME_HEIGHT			     850

#define IMAGE_BAND                     1
#define IMAGE_DEPTH                    8

#define IMAGE_LEVEL                  256
#define BLACK_LEVEL                    0
#define MEDIUM_LEVEL                 128
#define WHITE_LEVEL                  255
#define MAXIMUM_ANGLE                360

///////////////////////////// 수학 함수 정의
#define PI             3.141592653589793
#define RAD                 (PI / 180.0)
#define DEG                 (180.0 / PI)
///////////////////////////// 수학 함수 정의

#define NONE_TYPE                      0
#define SPARE_MAX_NUM                 10
#define GOM_MAX_NUM                  100
#define SAMPLE_MAX_NUM                20
#define EDGE_MAX_NUM                5000
#define TEMP_MAX_NUM                5120
#define MAX_PATTERN_NUM               10
#define LOCATE_MAX_NUM                20
#define PROCESS_MAX_NUM               20
#define LINK_MAX_NUM                  50
#define DEF_LIGHT_NUM                 10
#define DEF_LIGHT_VALUE              200
#define CAM_MAX_NUM                    4
#define DEFECT_RECT_NUM               20
#define DEF_TAB_FOLDING_RECT          50

#define DEF_CONT_EXIT_SETUP            0
#define DEF_CONT_LIGHT                 1
#define DEF_CONT_SAVE_SETUP            2

typedef struct 
{
	float x;
	float y;
} POINT2;

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////          비전 검사 구조체 정의         /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////            설정 형태 정의              /////////////////////////
#define RECKER_CENTER                 1
#define RECKER_LEFT_TOP               2
#define RECKER_RIGHT_TOP              3
#define RECKER_LEFT_BOT               4
#define RECKER_RIGHT_BOT              5
#define RECKER_MID_TOP                6
#define RECKER_MID_LEFT               7
#define RECKER_MID_RIGHT              8
#define RECKER_MID_BOT                9

// Overlay Type 정의
typedef enum 
{
	GOM_LINE=200,
	GOM_RECT,
	GOM_CIRCLE,
	GOM_CROSS,
	GOM_TEXT,
	GOM_CIRCLE2,
	GOM_MASK_RECT
};
// Overlay Type 정의

// Edge Detect 방법 정의
typedef enum
{
    SCAN_PROJECT_FT = 300,              // Projection Profile 신호  
	SCAN_PROJECT_DFDT,                  // Projection Profile 신호 미분 F'(t)
	SCAN_PROFILE_FT,                    // Profile 신호
	SCAN_PROFILE_DFDT                   // Profile 신호 F'(t)
};
// Edge Detect 방법 정의

// INSPECT TYPE 정의
typedef enum
{
	INSP_MEASURE_SIDE_ONE=400,          // 일 방향 EDGE 측정(프로젝트 방법)
	INSP_MEASURE_SIDE_TWO,              // 두 방향 EDGE 측정(프로젝트 방법)
	INSP_SLINE,                         // 직선성 검사
	INSP_MISMATCH,
	INSP_DATUM_B,
	INSP_DATUM_C,
	INSP_DEFECT_VARI,                   // Defect 바리 검사
	INSP_MARK,
	INSP_PATTERN_MATCH,                 // 패턴 매칭 
	INSP_CAMERA_CAL,
	INSP_BLOB_ANALYSIS,                 // BLOB 해석 
	INSP_MEASURE_CIRCLE,                   // 원형 검사
	INSP_BLOB_CIRCLE,
	INSP_DEFECT_NORMAL,                 // Defect 검사 Type  
	INSP_TAB_FOLDING,
	INSP_GRAY_VALUE,                    // Gray Value값 검사 Type 
	INSP_SHARP_VALUE,
	INSP_IS_OBJECT,
	INSP_SCALE_FACTOR
};
// INSPECT TYPE 정의

// TEACHING TYPE 정의
typedef enum
{
	TEACH_NONE = 500,                  // 검사 형태가 없음  
	TEACH_DIST_PNT2PNT,                // 두 점의 거리 측정 TYPE
	TEACH_DIST_PNT2LINE,
	TEACH_DIST_LINE2LINE,              // 두 라인의 거리 측정 TYPE
	TEACH_DIST_1STLINE2LINE,
	TEACH_DIST_2STLINE2LINE,
	TEACH_NADIST_LINE2LINE,            // 절대값이 아닌 값을 거리로 표시 하기 위한 Type
	TEACH_DIFF_GAP2GAP,
	TEACH_DIST_CIRCLE2LINE,
	TEACH_ANG_PNT2PNT,                 // 두 점의 각도 측정 Type  
	TEACH_ANG_LINE4PNT,                // 4점으로 만드는 2라인의 각도 측정 Type 
	TEACH_ANG_LINE2LINE,               // 두 라인의 각도 측정 TYPE
	TEACH_ANG_3PNT,                    // 세 점의 각도 측정 TYPE
	TEACH_DIFF_PNT2PNT,                // 두 점의 OFFSET 측정 TYPE 
	TEACH_CIR_ROI2ROI
};
// TEACHING TYPE 정의 

// Dir Type 정의
typedef enum
{
	DIR_LEFT_RIGHT=600,			    	// 진행방향 Left->Right 스캔 측정  
	DIR_RIGHT_LEFT,						// 진행방향 Right->Left 스캔 측정  
	DIR_TOP_BOTTOM,						// 진행방향 TOP->BOTTOM 스캔 측정
	DIR_BOTTOM_TOP,						// 진행방향 BOTTOM->TOP 스캔 측정
	DIR_INSIDE_OUTSIDE,                 // 진행방향 내부->외부 스캔 측정(원)
	DIR_OUTSIDE_INSIDE                  // 진행방향 외부->내부 스캔 측정(원)
};
// Dir Type 정의

// POLARITY Type 정의
typedef enum
{
	POL_WHITE=700,						// 흰색
	POL_BLACK,							// 검은색
	POL_WHITE_BLACK,					// 흰색에서 검은색으로 변할 때의 검사
	POL_BLACK_WHITE 					// 검은색에서 흰색으로 변할 때의 검사
};
// POLARITY Type 정의

// Edge Detect 방법 정의
typedef enum
{
    MORPH_ERODE = 900,              // Projection Profile 신호  
	MORPH_DILATE,                   // Projection Profile 신호 미분 F'(t)
	MORPH_OPEN,                     // Profile 신호 
	MORPH_CLOSE                     // Profile 신호 F'(t) 
};
// Edge Detect 방법 정의

/////////////////////////////  SYSTEM DATA 관련 구조체 //////////////////////////////
typedef struct
{
	long		 m_nExposureTime;         // Brightness Value 지정
	long         m_LightV[DEF_LIGHT_NUM];              // 조명값 

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} SYSTEM_DATA;
/////////////////////////////  SYSTEM DATA 관련 구조체 //////////////////////////////

////////////////////////////// Overlay ROI 관련 구조체 //////////////////////////////
typedef struct 
{
	TCHAR        m_strTitle[100];       // ROI Label Title 

	long         m_nGOMType;            // OBJECT TYPE (점, 직선, 사각, 원, 타원 등) 
	long         m_nThick;              // OBJECT 두께 

	BOOL         m_bExecShowROI;        // 실행중 항목을 보이게 함 
	BOOL         m_bEjectROI;          
	BOOL         m_bShowROI;            // OBJECT그리기(Show/Hide)
	BOOL         m_bShowTitle;          // OBJECT TITLE(Show/Hide)
	BOOL         m_bSelectedROI;        // 마우스가 선택한 OBJECT(ON/OFF)

	POINT2       m_ptPoint1;            // 영역의 첫번째 좌표 
	POINT2       m_ptPoint2;            // 영역의 첫번째 좌표 
	POINT2       m_ptPoint3;            // 영역의 첫번째 좌표 
	POINT2       m_ptPoint4;            // 영역의 두번째 좌표 

	POINT2       m_ptStart[SPARE_MAX_NUM];// 영역의 첫번째 좌표 
	POINT2       m_ptEnd[SPARE_MAX_NUM];  // 영역의 두번째 좌표 

	float        m_dCX;                 // 영역의 중심 좌표(X)
	float        m_dCY;                 // 영역의 중심 좌표(Y)
	float        m_dAngle;              // 기울어진 각도 
	float		 m_dInRadius;           // 내부 반지름
	float        m_dOutRadius;          // 밖의 반지름 

	COLORREF	 m_nNormalColor;        // 보통 상태의 OBJECT COLOR
	COLORREF	 m_nSelectedColor;      // 선택된 상태의 OBJECT COLOR
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} GOM_ATTR;
////////////////////////////// Overlay ROI 관련 구조체 //////////////////////////////

///////////////////////////   SCAN EDGE 데이타 관련 구조체   //////////////////////////
typedef struct
{
	long         m_nScanType;           // EDGE DETECTIN TYPE

	long         m_nSlope;
	long		 m_nFirstThresh;        // First Threshold 설정치
	long		 m_nSecondThresh;       // Second Threshold 설정치

    long		 m_nFirstScanDirType;   // 스캔 방향 설정
	long		 m_nSecondScanDirType;  // 스캔 방향 설정
	long		 m_nFirstPolDirType;    // First Polarity 방향 설정
	long		 m_nSecondPolDirType;   // Second Polarity 방향 설정

	float        m_dFirstA;
	float        m_dFirstB;
	float        m_dFirstT;

	float        m_dSecondA;
	float        m_dSecondB;
	float        m_dSecondT;

	long         m_nFirstNum;                 // SCAN EDGE 데이타 수 
	POINT2       m_ptFirstMin;
	POINT2       m_ptFirstMax;
	bool         m_bFirstDec[EDGE_MAX_NUM];
	float		 m_dFirstPX[EDGE_MAX_NUM];    // 데이타 X 위치
	float		 m_dFirstPY[EDGE_MAX_NUM];    // 데이타 Y 위치 

	long         m_nSecondNum;                // SCAN EDGE 데이타 수 
	POINT2       m_ptSecondMin;
	POINT2       m_ptSecondMax;
	bool         m_bSecondDec[EDGE_MAX_NUM];
	float		 m_dSecondPX[EDGE_MAX_NUM];                 // 데이타 X 위치
	float		 m_dSecondPY[EDGE_MAX_NUM];                 // 데이타 Y 위치 

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} SCAN_ATTR;
//////////////////////////   SCAN EDGE 데이타 관련 구조체  //////////////////////////

//////////////////////////   TEACHING 데이타 관련 구조체   //////////////////////////
typedef struct
{
	TCHAR        m_strTitle[100];       // 검사 Label Title
	BOOL         m_bDecision;

	BOOL         m_bSel1;               // 0:Inspect Item 1:
	BOOL         m_bSel2;
	BOOL         m_bLinkExec;
	BOOL         m_bEjectROI;

	long         m_nLinkType;
    long         m_nIndex1;
	long         m_nIndex2;
	long         m_nOutputNo;
	long         m_nPlotSelNum;

	float        m_dCX;
	float        m_dCY;
	float        m_dValue;
	float        m_dValueS;

	float		 m_dStandLowLimit;       // 기준값 Low  Limit(SETUP)
	float		 m_dStandHighLimit;      // 기준값 High Limit(SETUP)
	float        m_dStdSpec;
	float        m_dToleSpec;
	float        m_dOffsetV;
	float        m_dValueA;
	
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} TEACH_ATTR;
//////////////////////////   TEACHING 데이타 관련 구조체   //////////////////////////

/////////////////////////  CALIBRATION 데이타 관련 구조체  //////////////////////////
typedef struct
{
	float        m_dScaleFactor;
	float        m_dScaleLT_X;
	float        m_dScaleLB_X;
	float        m_dScaleRT_X;
	float        m_dScaleRB_X;
	float        m_dScaleL_Y;
	float        m_dScaleR_Y;

	float        m_dRealT_X;
	float        m_dRealB_X;
	float        m_dReal_Y;
	float        m_dScaleX;             // 영상의 Scale Factor X  
	float        m_dScaleY;             // 영상의 Scale Factor Y

	double       m_dFacA;
	double       m_dFacB;
	double       m_dFacC;
	long         m_nAverBright;
} CAL_DATA;
/////////////////////////  CALIBRATION 데이타 관련 구조체  //////////////////////////

/////////////////////////////// 캘리브레이션  구조체  ///////////////////////////////
typedef struct
{
	long		 m_nOldMouseX;       
	long		 m_nOldMouseY;     

	float        m_fZoomScale;          // 영상의 Scale Factor 
	CPoint       m_ptMouse;
	CRect        m_fmRect;
} IMAGE_DATA;
///////////////////////////// 캘리브레이션  구조체 ////////////////////////////////////

/////////////////////////////  Enhencement 관련 구조체 //////////////////////////////
typedef struct
{
	BOOL		 m_bUseEnhence;         // Mapping 적용 유무

	long		 m_nContrastLow;        // Low Position 지정
	long		 m_nContrastHigh;       // High Position 지정
	long		 m_nBrightness;         // Brightness Value 지정
	long         m_LightV[DEF_LIGHT_NUM];              // 조명값 
	long		 m_nLUT[IMAGE_LEVEL];   // Contrast 지정

	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} ENHENCE_DATA;
/////////////////////////////  Enhencement 관련 구조체 //////////////////////////////

///////////////////////////////   검사 관련 구조체  /////////////////////////////////
typedef struct
{
	BOOL		 m_bInspMode;           // 해당 ROI 검사
	BOOL		 m_bDecision;           // 해당 ROI 판정 (결과값) : TRUE, FALSE / -1 : 사용하지 않음
	BOOL         m_bLocROI;
	BOOL         m_bScanMethod;

    long		 m_nInspectType;        // 검사 형태

	BOOL         m_bOverlapRate;
	BOOL         m_bBlobFeret;          // 0 : 면적 검사만      1 : Felet 검사   
	long         m_nBlobOption;         // 0 : 유일한 최대 면적 1 : 총합 면적  
	long         m_nBlobThresh;         // 문턱치 
	float		 m_dBlobMinArea;        // Blob의 최소면적
//	float		 m_dBlobMaxArea;        // Blob의 최소면적
	long		 m_nBlobPolarity;       // Blob의 적용 POL

	long         m_nPatScore;           // 패턴 매칭율 
	long         m_nPatSearchRate;      // 패턴의 사이즈에서 서치 영역을 계산 비율  
	CRect		 m_rcPatSearchROI;      // Search Area

	long         m_nPatWidth;           // 패턴 가로 사이즈 
	long         m_nPatHeight;          // 패턴 세로 사이즈 
	long         m_nOutputNo;            // "1" 앞, "2" 뒤
	long         m_nPlotSelNum;
	long         m_nSampleNum;
	long         m_nPatNum;             // 패턴 수 
	LPBYTE		 m_pPat[MAX_PATTERN_NUM]; // Pattern 저장 

	float        m_dAnyValue;           // 임의 결과값 
	float        m_dTeachValue;         // 검사 항목에 따른 저장값  
	float        m_dSensity;            // 감도 적용 값 
	float		 m_dSumV;        // Blob의 최대면적

	float		 m_dStandLowLimit;      // 기준값 Low  Limit(SETUP)
	float		 m_dStandHighLimit;     // 기준값 High Limit(SETUP)
	float        m_dStdSpec;
	float        m_dToleSpec;
	float        m_dOffsetV;            // Object Y 중심 
	
	float        m_dCX;                 // Object X 중심 
	float        m_dCY;                 // Object Y 중심 
	float        m_dRadius;

	POINT2		 m_ptFirstMeas1;        // First  측정 위치 구함 1
	POINT2       m_ptFirstMeas2;        // First  측정 위치 구함 2
	POINT2		 m_ptSecondMeas1;       // Second 측정 위치 구함 1
	POINT2		 m_ptSecondMeas2;       // Second 측정 위치 구함 2

	long         m_nDefectCnt;
	CRect        m_rcPosFind[DEFECT_RECT_NUM];
	
	long         m_nTGrayV;
	long		 m_nMaxThr;
	long		 m_nMinThr;
	long         m_nSpare[7];	
	float        m_fSpare[SPARE_MAX_NUM];

} INSPECT_DATA;
///////////////////////////////   검사 관련 구조체     ///////////////////////////////

///////////////////////////////   TEACH 관련 구조체    ///////////////////////////////
typedef struct
{
	long         m_nPreNum;
	long         m_nLocNum;
	long         m_nCamNum;
	long         m_nLinkNum;

	BOOL         m_bPreDecision;
	BOOL         m_bLocDecision;
	BOOL         m_bLinkDecision;
	BOOL         m_bEntire;
	BOOL         m_bDispAllROI;

    TEACH_ATTR   m_PRE[PROCESS_MAX_NUM];
	TEACH_ATTR   m_LOC[LOCATE_MAX_NUM];
	TEACH_ATTR   m_CAM[CAM_MAX_NUM];
	TEACH_ATTR   m_LINK[LINK_MAX_NUM];

	float        m_fOffsetX;
	float        m_fOffsetY;
	long         m_nSpare[SPARE_MAX_NUM];
	float        m_fSpare[SPARE_MAX_NUM];
} TEACH_DATA;
///////////////////////////////   TEACH 관련 구조체    ///////////////////////////////

typedef struct
{
	TCHAR		 m_strTitle[100];       // 검사면의 타이틀 (SETUP)
	BOOL		 m_bRealTimeMode;       // 현재 검사 모드 (검사시 적용)
	BOOL         m_bSetupMode;          // Setup시 적용  
    BOOL		 m_bDecision;           // 결과 판정

   	float		 m_dSourX;              // 위치 보정을 위한 X좌표(티칭시 자동 적용)
	float		 m_dSourY;              // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
	float		 m_dSourT;              // 각도 보정을 위한 T각도(티칭시 자동 적용)

	float		 m_dDeltaX;             // 위치 보정을 위한 X Offset(검사시 적용)
	float		 m_dDeltaY;             // 위치 보정을 위한 Y Offset(검사시 적용)
	float		 m_dDeltaT;             // 각도 보정을 위한 T Offset(검사시 적용)

	long		 m_nImageWidth;         // 영상저장(Width)
	long		 m_nImageHeight;        // 영상저장(Height)
	long         m_nSpare[SPARE_MAX_NUM];

   	float		 m_dSourX2;             // 위치 보정을 위한 X좌표(티칭시 자동 적용)
	float		 m_dSourY2;             // 위치 보정을 위한 Y좌표(티칭시 자동 적용)
	float		 m_dSourT2;             // 각도 보정을 위한 T각도(티칭시 자동 적용)

	float		 m_dDeltaX2;            // 위치 보정을 위한 X Offset(검사시 적용)
	float		 m_dDeltaY2;            // 위치 보정을 위한 Y Offset(검사시 적용)
	float		 m_dDeltaT2;            // 각도 보정을 위한 T Offset(검사시 적용)

	float        m_fSpare[4];

	SCAN_ATTR    m_Scan[GOM_MAX_NUM];
	GOM_ATTR     m_GOM[GOM_MAX_NUM];
	ENHENCE_DATA m_Enhence;             // 영상 개선 구조체
	IMAGE_DATA   m_IMG;
    INSPECT_DATA m_INSP[GOM_MAX_NUM];   // 검사 구조체
	CAL_DATA     m_CAL;
	TEACH_DATA   m_Teach;
} SETUP_MODEL_DATA;

typedef struct
{
	long    m_nCamID;
	BOOL    m_bDecision;
	BOOL    m_bStatus;
	BOOL    m_bFDecision;
	BOOL    m_bRDecision;
	BOOL    m_bFDataDec;
	BOOL    m_bRDataDec;

	BOOL    m_bMisMatchDecF;
	BOOL    m_bMisMatchDecR;

	float   m_dSensorDataF;
	float   m_dSensorDataR;
	float   m_dOffsetEPC;

	TCHAR   m_strTitle[GOM_MAX_NUM][100];
	BOOL    m_bDecisionROI[GOM_MAX_NUM];
	BOOL    m_bLocROI[GOM_MAX_NUM];
	BOOL    m_bExecROI[GOM_MAX_NUM];
	BOOL    m_bEjectROI[GOM_MAX_NUM];
	long    m_nPlotNum[GOM_MAX_NUM];
	long    m_nOutNo[GOM_MAX_NUM];
    long    m_nInspType[GOM_MAX_NUM];

#if _NG_DISP
	float   m_dPosRoiX1[GOM_MAX_NUM];
	float   m_dPosRoiY1[GOM_MAX_NUM];
	float   m_dPosRoiX2[GOM_MAX_NUM];
	float   m_dPosRoiY2[GOM_MAX_NUM];

	long    m_nSampleNum[GOM_MAX_NUM];
	POINT2  m_ptFirstEdge[GOM_MAX_NUM][SAMPLE_MAX_NUM];
	POINT2  m_ptSecondEdge[GOM_MAX_NUM][SAMPLE_MAX_NUM];
#endif
	
	float   m_dValue[GOM_MAX_NUM];
	float   m_dLowLimit[GOM_MAX_NUM];
	float   m_dHighLimit[GOM_MAX_NUM];
	float   m_dStdSpec[GOM_MAX_NUM];
	float   m_dToleSpec[GOM_MAX_NUM];
	long    m_nLinkNum;
	TCHAR   m_strLinkTitle[LINK_MAX_NUM][100];
	BOOL    m_bLinkDecROI[LINK_MAX_NUM];
	BOOL    m_bLinkExecROI[LINK_MAX_NUM];
	BOOL    m_bLinkEjectROI[LINK_MAX_NUM];
	BOOL    m_bLinkLocROI[LINK_MAX_NUM];
	long    m_nLinkPlotPos[LINK_MAX_NUM];
	long    m_nLinkOutNo[LINK_MAX_NUM];
	long    m_nLinkPlotNum[LINK_MAX_NUM];
    long    m_nLinkType[LINK_MAX_NUM];
	float   m_dLinkValue[LINK_MAX_NUM];
	float   m_dLinkValueS[LINK_MAX_NUM];
	float   m_dLinkLowLimit[LINK_MAX_NUM];
	float   m_dLinkHighLimit[LINK_MAX_NUM];
	float   m_dLinkStdSpec[LINK_MAX_NUM];
	float   m_dLinkToleSpec[LINK_MAX_NUM];
} INSP_RESULT_DATA;

typedef struct
{
    long    m_nCamID;
	long    m_nPosX;
	long    m_nPosY;
	long    m_nGrayLevel;

	BOOL    m_bModify;
	BOOL    m_bContNo;
	long    m_nCh;
	long    m_nExposureTime;
	long    m_LightV[DEF_LIGHT_NUM];
} NAppDll_DATA;
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////          비전 검사 구조체 정의         /////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#endif 