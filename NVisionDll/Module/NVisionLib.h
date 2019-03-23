#if !defined(AFX_NVISIONLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_)
#define AFX_NVISIONLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NVisionLib.h : header file
//
#define USE_JPEG_TURBO_LIB 

#include "jpeglib.h"
#include <setjmp.h>

#define USE_INSPECT_OPENCV 

#include "opencv.hpp"

using namespace cv;
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// NVisionLib window
#define  TIMER_MAX_NUM    30
//#define  DCF_FILE         "D:\\OCX\\IK53V_646x494Con.dcf"

class CNVisionLib
{
// Construction
public:
	CNVisionLib();

// Attributes
public:
	double      m_dSetTime[TIMER_MAX_NUM];
	double      m_dEndTime[TIMER_MAX_NUM];
    char        m_strCameraFile[256];

// Operations
public:
	BOOL	Open(long nType, long nwidth, long nHeight);
	BOOL	Close();

	BOOL	SaveImageToFile(LPCTSTR filename, LPBYTE fmImage, long nWidth, long nHeight);
    BOOL	LoadImageFromFile(LPCTSTR filename, unsigned char *Frame, long nWidth, long nHeight);

    BOOL	BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
    BOOL	SmoothProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
    BOOL	SharpnessProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL	SobelEdgeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL	EdgeDetectProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
    long	ImageRotation(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float angle);
    void	SaveJpegToFile(CString strPathName, LPBYTE pImage, int nWidth, int nHeight);
    void	MakeBMPHeader(BITMAPINFOHEADER *pbmiHdr, BITMAPFILEHEADER *pbmfHdr, long width, long height);
    float	GetSharpnessValue(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    void	ZoomingImage(LPBYTE pSour, LPBYTE pDest, float fDispX, float fDispY, long nIWidth, long nIHeight, long nFWidth, long nFHeight, float fScale);
    void    GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX);
    void    GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY);
    void    GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX);
    void    GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY);
    void    GetProfileFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX);
    void    GetProfileFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY);
    void    GetProfileDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjX);
    void    GetProfileDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverPixel, long *ProjY);
	
	double	GetTimer(long nID);
	void	ResetTimer(long nID);

public:
	////////////////////////////////////////////////////////
	// OPENCV3.x 관련 함수 정의      by shsong            //
	////////////////////////////////////////////////////////
	BYTE* matToBytes(Mat image);
	Mat   bytesToMat(BYTE* bytes, int width, int height);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// jpeg turbo lib 사용
	///////////////////////////////////////////////////////
	struct my_error_mgr {
		struct jpeg_error_mgr pub;    /* "public" fields */
		jmp_buf setjmp_buffer;        /* for return to caller */
	};

	typedef struct my_error_mgr *my_error_ptr;

	void read_BMPJPEG_file(char *filename,  LPBYTE fmImage, int image_width, int image_height);
	void write_JPEG_file (char *filename,  LPBYTE fmImage, int image_width, int image_height, int quality);
	void write_BMP_file  (char *filename,  LPBYTE fmImage, int image_width, int image_height);
	void my_error_exit (j_common_ptr cinfo);
	///////////////////////////////////////////////////////

// Implementation
public:
	virtual ~CNVisionLib();

	// Generated message map functions
protected:

};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_NVISIONLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_)
