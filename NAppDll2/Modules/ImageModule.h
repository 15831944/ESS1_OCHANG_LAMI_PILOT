#if !defined(AFX_IMAGEMODULE_H__1E34E4AA_79ED_4CFA_9BB2_22AE12CC83E1__INCLUDED_)
#define AFX_IMAGEMODULE_H__1E34E4AA_79ED_4CFA_9BB2_22AE12CC83E1__INCLUDED_

#include "Cdib.h"
//#include "OverlayLineGOM.h"
#include "OverlayRectGOM.h"
#include "OverlayCircleGOM.h"
//#include "OverlayMultiRectGOM.h"
//#include "OverlayTitleGOM.h"
#include "OverlayResultGOM.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageModule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageModule window
class CImageModule 
{
// Construction
public:
	CImageModule();
   ~CImageModule();
// Attributes

   	void    SetModelData(SETUP_MODEL_DATA *pData);

   	//////////////////////////////////////////////////////////
    //                   Image Infomation                   
    ////////////////////////////////////////////////////////// 
	BOOL    GetCurrentImage();
    void    SetImageSize(long nWidth, long nHeight);
	void    GetImageSize(long *nWidth, long *nHeight);
    void    SetFrameSize(long nWidth, long nHeight, long nBand, long nDepth);

	CString GetImageFileName();
    void    SetImageFileName(CString strImageFileName);
	long    GetCurrImageColor();
	long    GetCurrImageHeight();
	long    GetCurrImageWidth();
    void    SetCurrentImage(long nPos);
	void    WriteGrayLevelPos(long x, long y, long *nPosX, long *nPosY, long *nLevel);
    void    DisplayImageToFrame(LPBYTE pImage);
    void	InitImageMemory(long nWidth, long nHeight);
    void	InitFrameMemory(long nWidth, long nHeight, long nBand, long nDepth);
    void    SetZoomScale(float fScale);
	float	GetZoomScale();
	//////////////////////////////////////////////////////////
	//                  File Handling Function
	////////////////////////////////////////////////////////// 
    BOOL    OpenImageFile(LPCTSTR lpszPathName);
	BOOL    SaveImageFile(LPCTSTR lpszPathName);
	BOOL    SaveAsImageFile(LPCTSTR lpszPathName);
	BOOL    SaveAsRawImageData(LPCTSTR lpszPathName, unsigned char *fm);

	//////////////////////////////////////////////////////////
	//                   Overlay Function
	////////////////////////////////////////////////////////// 
    BOOL    InvalidateView(CDC *pDC);

	BOOL    SetSelectedROI(POINT2 nStartPoint);
	void    SetMoveROI(POINT2 nStartPoint, POINT2 nDeltaLength);

	BOOL	DrawLineROI(LPCTSTR strTitle, long x1, long y1, long x2, long y2, long nThick, COLORREF nColor);
	BOOL	DrawCrossROI(LPCTSTR strTitle, long x, long y, long nSize, long nThick, COLORREF nColor);
	BOOL	DrawCircleROI(LPCTSTR strTitle, long x, long y, long nRadius, long nThick, COLORREF nColor);
	BOOL	DrawRectROI(LPCTSTR strTitle, long x1, long y1, long x2, long y2, long nThick, COLORREF nColor);
	BOOL	DrawTextROI(LPCTSTR strTitle, long x, long y, COLORREF nColor);
	BOOL	DrawEllipseROI(LPCTSTR strTitle, long cx, long cy, float la, float lb, float dAngle, int nThick, COLORREF nColor);

	BOOL	GetLineROI(LPCTSTR strTitle, long *x1, long *y1, long *x2, long *y2, long *nThick, COLORREF *nColor);
	BOOL	GetCrossROI(LPCTSTR strTitle, long *x, long *y, long *nSize, long *nThick, COLORREF *nColor);
	BOOL	GetCircleROI(LPCTSTR strTitle, long *x, long *y, long *nRadius, long *nThick, COLORREF *nColor);
    BOOL    GetRectROI(LPCTSTR strTitle, long *x1, long *y1, long *x2, long *y2, long *nThick, COLORREF *nColor);
	BOOL	GetEllipseROI(LPCTSTR strTitle, long *cx, long *cy, float *la, float *lb, float *dAngle, int *nThick, COLORREF *nColor);
    BOOL    GetTextROI(LPCTSTR strTitle, long *x, long *y, COLORREF *nColor);

public:
 	LPBYTE  m_pRawImage;
//    LPBYTE  m_pDispImage;

	CDib   *m_fileDib;
    CDib   *m_dispDib;
	CRITICAL_SECTION	m_csSync;

	long    m_nFrameWidth;
	long    m_nFrameHeight;

	long    m_nImageWidth; 			// BMP file의 폭
	long    m_nImageHeight;    		// BMP file의 높이

	BOOL    m_bAllocFrame;
	BOOL    m_bDispFrame;

// Implementation
protected:
	long    GetMousePosMinDist(CPoint pt0, CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4);
	BOOL    GetModifyInsertPos(LPCTSTR strTitle);

	// Generated message map functions
protected:
    SETUP_MODEL_DATA *m_pData;

//	COverlayMultiRectGOM m_OverlayMultiRectGOM;
//	COverlayLineGOM   m_OverlayLineGOM;
	COverlayRectGOM   m_OverlayRectGOM;
	COverlayCircleGOM m_OverlayCircleGOM;
//	COverlayTitleGOM  m_OverlayTitleGOM;
	COverlayResultGOM m_OverlayResultGOM;

	CString   m_strImageFileName;
	CString   m_strGrayLevel;
	long      m_nImagePitch;	        // BMP file의 Pitch
	long      m_nImageColor;            // Image Bit Count
	long      m_nImageSize;              
	long      m_nInspItemCount;
	long      m_nMarginFillColor;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMODULE_H__1E34E4AA_79ED_4CFA_9BB2_22AE12CC83E1__INCLUDED_)
