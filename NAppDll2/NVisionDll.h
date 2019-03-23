// NVisionDll.h : NVisionDll DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CNVisionDllApp
// 이 클래스의 구현을 보려면 NVisionDll.cpp를 참조하십시오.
//

extern "C" _declspec(dllexport) BOOL    NVisionDll_Open(long nType, long nWidth, long nHeight);
extern "C" _declspec(dllexport) BOOL    NVisionDll_Close();
extern "C" _declspec(dllexport) void    NVisionDll_SetCameraFile(LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL    NVisionDll_SetSelectChannel(long nCh);
extern "C" _declspec(dllexport) BOOL    NVisionDll_Grab(LPBYTE fmImage);
extern "C" _declspec(dllexport) BOOL    NVisionDll_GrabCH(long nCh, LPBYTE fmImage);
extern "C" _declspec(dllexport) BOOL    NVisionDll_LoadImageFromFile(char *strPathName, unsigned char *fmImage, long nWidth, long nHeight);
extern "C" _declspec(dllexport) BOOL    NVisionDll_SaveImageToFile(char *strPathName, LPBYTE fmImage, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void    NVisionDll_ResetTimer(long nID);
extern "C" _declspec(dllexport) double  NVisionDll_GetTimer(long nID);

extern "C" _declspec(dllexport) BOOL    NVisionDll_SmoothProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_BinarizeProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
extern "C" _declspec(dllexport) BOOL    NVisionDll_SobelEdgeProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
extern "C" _declspec(dllexport) BOOL    NVisionDll_OpenBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_CloseBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_ErodeBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_DilateBinaryProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_OpenGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_CloseGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_ErodeGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);
extern "C" _declspec(dllexport) BOOL    NVisionDll_DilateGrayProc(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nIter);

extern "C" _declspec(dllexport) BOOL    NVisionDll_ImageRotation(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dAngle);
extern "C" _declspec(dllexport) float   NVisionDll_FindAngleProj(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dSAngle, float dEAngle);
extern "C" _declspec(dllexport) float   NVisionDll_GetSharpnessValue(LPBYTE pSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
extern "C" _declspec(dllexport) BOOL    NVisionDll_CodeRead(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nType, long nNum, char *strCode);
extern "C" _declspec(dllexport) void    NVisionDll_ZoomingImage(LPBYTE pSour, LPBYTE pDest, float fDispX, float fDispY, long nIWidth, long nIHeight, long nFWidth, long nFHeight, float fScale);
extern "C" _declspec(dllexport) LPBYTE  NVisionDll_OpenTransTiltToRect(POINT2 pt1, POINT2 pt2, POINT2 pt3, POINT2 pt4, long *nWidth, long *nHeight);
extern "C" _declspec(dllexport) void    NVisionDll_ExecTransTiltToRect(LPBYTE pSour, POINT2 pt1, POINT2 pt2, POINT2 pt3, POINT2 pt4, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void    NVisionDll_CloseTransTiltToRect();
extern "C" _declspec(dllexport) void    NVisionDll_GetProjectFtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjX);
extern "C" _declspec(dllexport) void    NVisionDll_GetProjectFtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long *ProjY);
extern "C" _declspec(dllexport) void    NVisionDll_GetProjectDfDtX(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverDist, long *ProjX);
extern "C" _declspec(dllexport) void    NVisionDll_GetProjectDfDtY(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nNaverDist, long *ProjY);

extern "C" _declspec(dllexport) void    NVisionDll_LoadCalData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NVisionDll_SaveCalData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL    NVisionDll_SetWarping(long nCamID, LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, long nPol, long nRow, long nCol, float dGridSize);
extern "C" _declspec(dllexport) BOOL    NVisionDll_SetWarping2(long nCamID, long nBlobCnt, double *fCenX, double *fCenY, long nWidth, long nHeight, long nRow, long nCol, float dGridSize);
extern "C" _declspec(dllexport) BOOL    NVisionDll_TransWarpingImage(long nCamID, LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight);
extern "C" _declspec(dllexport) BOOL    NVisionDll_TransWarpingPos(long nCamID, float i, float j, float *x, float *y, long nWidth, long nHeight);
extern "C" _declspec(dllexport) float   NVisionDll_GetScaleFactor(long nCamID);

class CNVisionDllApp : public CWinApp
{
public:
	CNVisionDllApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
