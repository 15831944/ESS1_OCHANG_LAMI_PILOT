// NBlobDll.h : NBlobDll DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

// CNBlobDllApp
// 이 클래스의 구현을 보려면 NBlobDll.cpp를 참조하십시오.
//
#define  BLOB_MAX_NUM             100000

typedef struct 
{
	float   m_fArea;
	float   m_fCircle;

	float   m_fTopX;
	float   m_fTopY;
	float   m_fBotX;
	float   m_fBotY;

	float   m_fCenX;
	float   m_fCenY;

	float   m_fMinFeret;
	float   m_fMaxFeret;

	float   m_fPerimeter;
	float   m_fAngle;
	float   m_fCompact;

} BLOB_STRUCT;

/////////////////////////////////////////////////////////////////////////////
// CNPatternDllApp
// See NPatternDll.cpp for the implementation of this class
//

extern "C" _declspec(dllexport) void  NBlobDll_Open();
extern "C" _declspec(dllexport) void  NBlobDll_Close();
extern "C" _declspec(dllexport) long  NBlobDll_FindAutoThreshold(LPBYTE fm, long x1, long y1, long x2, long y2, long pitch);
extern "C" _declspec(dllexport) BOOL  NBlobDll_BinarizeProc(LPBYTE fmSourImage, LPBYTE fmTargImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh);
extern "C" _declspec(dllexport) BLOB_STRUCT*   NBlobDll_BlobAnalysis(unsigned char *pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nMinArea, long nMaxArea, long nColor);
extern "C" _declspec(dllexport) long  NBlobDll_GetBlobCount();


class CNBlobDllApp : public CWinApp
{
public:
	CNBlobDllApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
