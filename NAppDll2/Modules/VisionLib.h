#if !defined(AFX_FUNCTIONETC_H__F4FD36C2_BA6B_11D5_BFD9_0000F0672E7F__INCLUDED_)
#define AFX_FUNCTIONETC_H__F4FD36C2_BA6B_11D5_BFD9_0000F0672E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunctionEtc.h : header file
//
#include "Mathematic.h"

/////////////////////////////////////////////////////////////////////////////
// CVisionLib window
#define CLOCK_MAX_NUM          10

class CVisionLib : public CStatic
{
// Construction
public:
	double m_SetTime, m_EndTime;
	CMathematic       m_libMath;

public:
	CVisionLib();

	// 자동 문턱값 구하는 함수 
    // fm : 원 영상의 메모리 
    // x1, y1, x2, y2 : 처리하고자하는 영역 
    // pitch : 영상의 WIDTH(대부분 640)
	long   FindAutoThreshold(unsigned char* fm, long x1, long y1, long x2, long y2, long pitch);
    double FindZerePos(int N, float *Pos, float *PosV);
    void   TwoCurveFitting(int N, double *Pos, double *PosV, double *a, double *b, double *c);

	// 직선 성분이 많은 영상의 경우 회전된 각도를 구하는 함수 
    // fm : 원 영상의 메모리 
    // x1, y1, x2, y2 : 처리하고자하는 영역 
    // pitch : 영상의 WIDTH(대부분 640)
	double FindImageAngle(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);
    BOOL   GetDfDtSignal(long *pFt, long nStart, long nEnd, long *pDfDt, long nPixelSide);

	// 평균 밝기값을 구하는 함수(0 ~ 255)
	// frame : 원 영상의 메모리 
	// x1, y1, x2, y2 : 처리하고자하는 영역 
	// pitch : 영상의 WIDTH(대부분 640)
	long   GetAveBrightness(unsigned char *frame, long x1, long y1, long x2, long y2, long pitch);
	long   GetAveBrightness2(unsigned char *frame, long x1, long y1, long x2, long y2, long pitch);
    BOOL   GetWhiteBlob(LPBYTE SourImage1, LPBYTE SourImage2, LPBYTE pMaskImage, long x1, long y1, long x2, long y2, long nOffset, long nWidth, long nHeight);

	float  GetSharpnessValue(unsigned char *fmImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight); // Image Width
    float  GetSharpnessValue(int dataN, float *x, float *y);
    float  GetLinerityValue(int dataN, float *x, float *y);
    float  GetDetectVariValue(int dataN, float *x, float *y);

	// 영상에서 Edge 처리 Processing 함수
	// sourImage : 원 영상 메모리
	// destImage : 결과 영상 메모리 
	// x1, y1, x2, y2 : 처리하고자하는 영상의 위치 
	// differ : 문턱값 
    long   EdgeDetect(unsigned char *sourImage, unsigned char *destImage, long x1, long y1, long x2, long y2, long differ, long nWidth, long nHeight);

	// 이진화 처리 
	// sourImage : 원 영상 메모리
	// destImage : 결과 영상 메모리 
	// x1, y1, x2, y2 : 처리하고자하는 영상의 위치 
	// threshold : 문턱값 
    void   ImageBinarize(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long threshold);
    void   BinarizeLinearThreshold(unsigned char *SourImage, unsigned char *DestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nStep, long threshold);

    long   GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX);
    long   GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY);
    long   GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX);
    long   GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY);
    long   GetProfileFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX);
    long   GetProfileFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY);
    long   GetProfileDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX);
    long   GetProfileDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY);
    float  GetAverDistance(long nType, POINT2 ptPos1, POINT2 ptPos2, POINT2 ptPos3, POINT2 ptPos4, float a1, float b1, float a2, float b2);
    BOOL   GetAngle(float dX1, float dY1, float dX2, float dY2, float *dAngle);
	float  GetPosDetectEdge_DfDt(long *pData, long nStart, long nEnd, long nDir, long nPolality, long nThresh, long nMaxThr, long nMinThr);
	float  GetPosDetectEdge_DfDt2(long *pData, long nStart, long nEnd, long nDir, long nPolality, long nThresh);
    float  GetDistance2D(float x1, float y1, float x2, float y2);

	// 실제 프로파일 분석으로 산의 갯수를 구함 
    // Data : 한 라인의 데이타 메모리 
    // nDataNum : 데이타 길이 
    // differ : Profile의 높이 폭이 이 수치보다 크면 Check함 
    long   GetPeakData(long nDataNum, long *Data, long *PEAK_X, long *PEAK_Y);

	int    LineFitting(int dataN, float *x, float *y, int removeN, float ErrorLimit, float *t, float *a, float *b);
	int    LineFitting1(int dataN, float *x, float *y, int removeN, float ErrorLimit, float *t, float *a, float *b);

	long   FindFittingCenter(int N, float *x, float *y, int nRemove, float dLimit, 
					   float *Cx, float *Cy, float *MaxRadius,float *MinRadius,float *AvgRadius);

	// 시간얻기 
    void   SetClockTimer(long nID);
	double GetClockTimer(long nID);

	void   SaveTextFile(CString strPathName, CString strText);
    BOOL   SaveBitmapImage(LPCTSTR strFileName, LPBYTE pImage, long width, long height);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionLib)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVisionLib();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVisionLib)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    long   matrixInverse(float *a,int n,float *b);

	double m_dSetTime[CLOCK_MAX_NUM];
    double m_dEndTime[CLOCK_MAX_NUM];

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONETC_H__F4FD36C2_BA6B_11D5_BFD9_0000F0672E7F__INCLUDED_)
