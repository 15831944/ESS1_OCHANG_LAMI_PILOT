#if !defined(AFX_NMATROXLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_)
#define AFX_NMATROXLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NVisionLib.h : header file
//
#if _MIL_SETUP
#include "mil.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// NVisionLib window
#define  TIMER_MAX_NUM    30
#define  DCF_FILE         "D:\\APPL\\PHONE\\EXE\\VCC-G20X30T1_sync.dcf"
//#define  DCF_FILE         "D:\\OCX\\IK53V_646x494Con.dcf"

class CNMatroxLib
{
// Construction
public:
	CNMatroxLib();

// Attributes
public:
	long       m_nType;
	BOOL       m_bOpen;
	long       m_nCh;

	double     m_dSetTime[TIMER_MAX_NUM];
	double     m_dEndTime[TIMER_MAX_NUM];
    char       m_strCameraFile[256];
	double     Mil_timer;

#if _MIL_SETUP
    MIL_ID MilApplication,  //Mpplication identifier.  
		   MilSystem[2],       //System identifier.       
		   MilDigitizer[2],    //Digitizer identifier.   
		   MilImage[2],        //Image buffer identifier.
		   ProcessImage[2],
		   BufferImage,
		   SubImage,
		   GrabImage,
		   DisplayImage;
#endif
		 
// Operations
public:
	BOOL  Open(long nType, long nwidth, long nHeight);
	BOOL  Close();

	BOOL  Grab(LPBYTE pImage);
	BOOL  Grab(long nCh, LPBYTE pImage);
	BOOL  SetSelectChannel(long nCh);
	BOOL  SaveImageToFile(LPCTSTR filename, LPBYTE fmImage, long nWidth, long nHeight);
    BOOL  LoadImageFromFile(LPCTSTR filename, unsigned char *Frame, long nWidth, long nHeight);

    BOOL  BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
    BOOL  SmoothProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
	BOOL  OpenProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter);
	BOOL  CloseProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter);
    BOOL  DilateProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter);
    BOOL  ErodeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long iter);
    BOOL  SharpnessProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL  SobelEdgeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
    BOOL  EdgeDetectProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
    BOOL  BarCodeRead(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nType, long nNum, char *strCode);
    long  ImageRotation(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float angle);
    void  SaveJpegToFile(CString strPathName, LPBYTE pImage, int nWidth, int nHeight);
    void  MakeBMPHeader(BITMAPINFOHEADER *pbmiHdr, BITMAPFILEHEADER *pbmfHdr, long width, long height);

	double GetTimer(long nID);
	void   ResetTimer(long nID);

// Implementation
public:
	virtual ~CNMatroxLib();

	// Generated message map functions
protected:

};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_NMATROXLIB_H__2D7C05CE_1778_4B47_B70B_6D9EE1BD00BC__INCLUDED_)
