// NPatternDll.h : NPatternDll DLL의 기본 헤더 파일입니다.
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

// CNPatternDllApp
// 이 클래스의 구현을 보려면 NPatternDll.cpp를 참조하십시오.
//
extern "C" _declspec(dllexport) void    NPatternDll_LoadPatternFromFile(LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NPatternDll_SavePatternToFile(LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NPatternDll_DeletePattern();
extern "C" _declspec(dllexport) void    NPatternDll_DeletePatternAll();
extern "C" _declspec(dllexport) LPBYTE  NPatternDll_GetPatternPtr(long *pPatWidth, long *pPatHeight);
extern "C" _declspec(dllexport) void    NPatternDll_RegistPattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nLowThresh, long nHighThresh, float fSAngle, float fEAngle);
extern "C" _declspec(dllexport) BOOL    NPatternDll_RotateImage(LPBYTE pImage, LPBYTE pBuffer, long x1, long y1, long x2, long y2, long nWidth, long nHeight, float dAngle);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindPatternWithID(long nID, LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nSourWidth, long nSourHeight,  float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindShapePattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight,
												                     LPBYTE fmPatImage,  long nPatWidth, long nPatHeight, float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) BOOL    NPatternDll_FindPattern(LPBYTE fmSourImage, long x1, long y1, long x2, long y2, long nSourWidth, long nSourHeight, LPBYTE fmPatImage,  long nPatWidth, long nPatHeight, float dAcceptScore, float *dPosX, float *dPosY, float *dPatScore);
extern "C" _declspec(dllexport) void    NPatternDll_ProcCannyEdge(LPBYTE pSourImage, LPBYTE pDestImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThreshLow, long nThreshHigh);

class CNPatternDllApp : public CWinApp
{
public:
	CNPatternDllApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
