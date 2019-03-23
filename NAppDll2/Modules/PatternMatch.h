#if !defined(AFX_PATTERNMATCH_H__A13BB271_DD61_48EE_A2EE_A8826C6994BD__INCLUDED_)
#define AFX_PATTERNMATCH_H__A13BB271_DD61_48EE_A2EE_A8826C6994BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternMatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatternMatch window
#ifndef	BYTE
typedef	unsigned char	BYTE;
#endif

#ifndef	PBYTE
typedef BYTE*	PBYTE;
#endif

#define MAX_PATTERN_NUM 10
#define	MAX_LEVEL		10	// maximum possible pyramid level when building pattern
#define	MIN_PATSIDE		8	// minumum pattern image side length
#define	ORG_SCORE		0.5
							// compression-related parameter when building pattern.
							// compression(leveling) stops when compressed pattern's
							// matching score with original goes below this value
							// valid range is (0.0~1.0)
							// increase this to perform exact match
							// decrease this to speed up	

#define MAXCANDIDATENUMBER 10   /* 후보의 수는 50개를 초과할수 없다. */
#define PYRAMIDLEVEL       4    /* 바닥포함하여 4단계가있음 */

typedef struct tagPATIMG*	PATIMG;
typedef struct tagSUBINFO*	SUBINFO;
typedef struct tagPATMAT*	PATMAT;

struct tagPATIMG 
{
	PBYTE	bits;
	int	memw,width,height;
	float	dAngle;	// degree
	// below : pattern only
	float	patS,patSS;
	int		patLen;
	PBYTE	patMask;
};

struct tagPATMAT 
{
	float	hotOffX,hotOffY;
	int	bUseSrc;
	int	nLevel;
	PATIMG	image[MAX_LEVEL];
};

struct tagSUBINFO
{
	int	level;
	int	x,y;
	float	sl,st,sr,sb,sm;
	float	degree, da;	// for rotation
	float	su, sd;	// for rotation
};

typedef struct {
	BOOL m_bRegist;

	long m_nWidth;
	long m_nHeight;

	LPBYTE m_pImage;
} REGISTER_PATTERN;

class CPatternMatch : public CStatic
{
// Construction
public:
	CPatternMatch();

	void CopyPattern(long pPatLib);
	void RegisterPattern(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch);
	void DeletePattern(long nID);
	void DeletePatternAll();
	void SaveImage(CString strFileName, LPBYTE pImage, long nWidth, long nHeight);
	long FileLoadPattern(CString strFileName);
	void FileSavePattern(CString strFileName);
	long FindPatternMatching(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch, 
		                                                   float dMinScore, float *dPosX, float *dPosY, float *dPatScore);
    BOOL FindPatternMatching(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nPitch, 
		                     LPBYTE fmPattern, long nWidth, long nHeight, float dMinScore, float *dPosX, float *dPosY, float *dPatScore);



// Operations
public:
	long   m_nPatternCnt;
	REGISTER_PATTERN m_Pat[MAX_PATTERN_NUM];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatternMatch)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPatternMatch();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPatternMatch)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

private:
	void	PatternInit(PBYTE (*alloc_8)(int),void (*free_8)(PBYTE));
		// has no function. (for compatibility only)

	void	PatternSetErrorDisplay(void (*showError)(LPCTSTR));
		// assigns error reporting function
		// args
		//	showError :	pointer to function, which shows error
		//			message delivered as (const char*).

	PATMAT	PatternBuildPattern(PBYTE src,int memw,
			int left,int top,int right,int bottom,
			float hotOffX, float hotOffY, int bUseSrc);
		// creates and initializes template pattern from source image
		// return
		//	pointer to pattern structure created (of type PATMAT)
		// args
		//	src :		address of (0,0) pixel of source image
		//	memw :		memory width of source image
		//	left,top,right,bottom :	region of image to register
		//	hotOffX,hotOffY :	reference (x,y) of pattern
		//				w.r.t. pattern image.
		//	bUseSrc :	nonzero is possible if source image remains
		//			unchanged while using this pattern. it is to
		//			save memory space used to copy original.
		//			if source changes, set this to be 0.

	PATMAT	PatternBuildMaskPattern(PBYTE src,int memw,
			int left,int top,int right,int bottom,
			float hotOffX,float hotOffY,
			PBYTE mask,int mpitch);
		// creates and initializes template pattern from source image
		// return
		//	pointer to pattern structure created (of type PATMAT)
		// args
		//	src :		address of (0,0) pixel of source image
		//	memw :		memory width of source image
		//	left,top,right,bottom :	region of image to register
		//	hotOffX,hotOffY :	reference (x,y) of pattern
		//				w.r.t. pattern image.
		//	mask :		address of first mask pixel for source region
		//	mpitch :	pitch of mask memory

	void	PatternFreePattern(PATMAT *ppat);
		// frees pattern structure, which is created by PatternBuildPattern
		// args
		//	ppat :		pointer  to PATMAT object, which was
		//			created by PatternBuildPattern function. *ppat becomes
		//			0 when this function ends.

	int	PatternSearch(PATMAT pat, PBYTE dst,int memw,
			int left,int top,int right,int bottom,float minScore,int pixRes,
			float *locX,float *locY,float *score);
		// executes pattern matching without rotation option
		// return
		//	nonzero if found, zero otherwise.
		// args
		//	pat :		pointer to PATMAT structure created by PatternBuildPattern.
		//	dst :		address of (0,0) pixel of destination image
		//	memw :		memory width of destination image
		//	left,top,right,bottom :	search region in destination image
		//	minScore :	minimum acceptance level used for algorithm.
		//			valid range is (0.0~1.0). set 0.5 for normal case.
		//			increase this to speed up searching.
		//			decrease this to find noisy object.
		//	pixRes	:	pixel resolution.
		//				setting value of 1 (minimum) means full searching.
		//				increasing this value speeds up searching, but
		//				makes less reliable.
		//	locX :		pointer to save result X of 'pat's hot point.
		//			set 0 to skip saving
		//	locY :		pointer to save result Y of 'pat's hot point.
		//			set 0 to skip saving
		//	score :		pointer to save result score (0.0~1.0).
		//			set 0 to skip saving

	int	PatternSearchWithAngle(PATMAT pat,PBYTE dst, int memw,
			int left,int top,int right,int bottom,float loDegree,float hiDegree,
			float minScore,int pixRes,
			float *locX,float *locY,float *degree,float *score);
		// executes pattern matching with rotation option
		// return
		//	nonzero if found, zero otherwise.
		// args
		//	pat :		pointer to PATMAT structure created by PatternBuildPattern.
		//	dst :		address of (0,0) pixel of destination image
		//	memw :		memory width of destination image
		//	left,top,right,bottom :	search region in destination image
		//	loDegree,hiDegree :	low/high degree value of search angle.
		//			the angle increases clockwise. loDegree<hiDegree must.
		//	minScore :	minimum acceptance level used for algorithm.
		//			valid range is (0.0~1.0). set 0.5 for normal case.
		//			increase this to speed up searching.
		//			decrease this to find noisy object.
		//	pixRes	:	pixel resolution.
		//				setting value of 1 (minimum) means full searching.
		//				increasing this value speeds up searching, but
		//				makes less reliable.
		//	locX :		pointer to save result X of 'pat's hot point.
		//			set 0 to skip saving
		//	locY :		pointer to save result Y of 'pat's hot point.
		//			set 0 to skip saving
		//	degree :	pointer to save result Theta of 'pat'.
		//			set 0 to skip saving
		//	score :		pointer to save result score (0.0~1.0).
		//			set 0 to skip saving

	void	PatternShowError(LPCTSTR msg);
	void	PatternMemErr();
	void	PatternShrink(PATIMG dn,PATIMG up,int bIsPat);
	float	PatternOrgMatch(PATIMG img0,PATIMG img,int level);
	float	PatternCorr(PBYTE src,int smemw,float s_s,float s_ss,int s_len, PBYTE mask,PBYTE dst,int dmemw,int w,int h);
	float	PatternDownSearch(PATIMG *dimg,PATIMG *patimg,int level,int pixLevel, float minScore,int x,int y,SUBINFO subInfo);
	float	PatternBottomSearch(PATIMG dimg,PATIMG patimg,int pixLevel,	int x,int y,SUBINFO subInfo);
	void	PatternSubPixel(PATIMG img,PATIMG patimg,SUBINFO subInfo,float *rx,float *ry);
	float	PatternCorr_Angle(PBYTE src,int smemw,int sw,int sh, float s_s,float s_ss,int s_len,PBYTE mask,
			PBYTE dst,int dmemw,int dw,int dh,int x,int y,float degree);
	float	PatternDownSearch_Angle(PATIMG *dimg,PATIMG *patimg,int level,int pixLevel,	float minScore,int x,int y,float degree,SUBINFO subInfo);
	void	PatternSubPixel_Angle(PATIMG img,PATIMG patimg,SUBINFO subInfo,	float *rx,float *ry,float *degree);
	float	PatternBottomSearch_Angle(PATIMG dimg,PATIMG patimg,int pixLevel, int x,int y,float degree,SUBINFO subInfo);
	float	PatternCos(float theta);
	float	PatternSin(float theta);



	/* BEGIN : Gray Pattern Matching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
	int     ImgAlloc_Alloc(int w, int h);
	LPBYTE  ImgAlloc_GetStart(int a);
	void    ImgAlloc_Free(LPBYTE aaa);
	float   GrayMatch(LPBYTE fstart,int fwidth,int x,int y,int w,int h,
					  LPBYTE mstart,int mwidth,int mw,int mh,float *rx,float *ry,int SUBFLAG,
					  int nCandiNumber, float m_dConfuseCorrValue, float m_dAcceptCorrValue);
	int     MatchingSubForTop(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh, int m_nCandiNumber, int ThreCorr,
						  float CandiData[][3], int *m_nFoundCandiNumber);
	int     MatchingSub(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh,float *ReturnX, float *ReturnY);
	int     MatchingForBottom(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh,float *ReturnX, float *ReturnY, float *m_dCorrValue, int WIDTH_MEM, int mwidth);
	int     MatchingForSubPixel(LPBYTE Tfm,LPBYTE Sfm, int Tw, int Th, int Sw, int Sh,float *ReturnX, float *ReturnY, float *m_dCorrValue, int WIDTH_MEM, int mwidth);
	int     FindSecondPolynomialPeak(int N, float Data[][3], float *rx, float *ry);
	int     MakePyramid(LPBYTE fmS,LPBYTE fmD,int nWidth, int nHeight, int WIDTH_MEM);
	int     MatchSort(int count, float Value[][3]);

	int     MatrixTranspose(int n, int m, float A[][9], float Ai[][9]);
	int     MatrixMultiplicant(int n1, int m1, float A[][9], int n2, int m2, float B[][9], float C[][9]);
	int     MatrixMultiplicant2(int n, int m, float A[][9], float B[], float C[]);
	int     MatrixInverse(int n, float A[][9], float Ai[][9]);

	int     matrixInverse(float *a,int n,float *b);
	/* BEGIN : Gray Pattern Matching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

private:
	LPBYTE  m_fmTemp[PYRAMIDLEVEL];     /* Template의 각 단계별 메모리 시작번지 */
	int     m_TempWidth[PYRAMIDLEVEL];  /* 각 단계별 Template Width             */
	int     m_TempHeight[PYRAMIDLEVEL]; /* 각 단계별 Template Height            */
    /* Search Area관련 데이타 */ 
	LPBYTE  m_fmSearch[PYRAMIDLEVEL];    /* Search Area의 각 단계별 메모리 시작번지 */
	int     m_SearchWidth[PYRAMIDLEVEL]; /* 각 단계별 Search Area Width             */
	int     m_SearchHeight[PYRAMIDLEVEL];/* 각 단계별 Search Area Height            */

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNMATCH_H__A13BB271_DD61_48EE_A2EE_A8826C6994BD__INCLUDED_)
