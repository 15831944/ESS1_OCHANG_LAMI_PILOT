// NAppDll.h : NAppDll DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

// CNAppDllApp
// 이 클래스의 구현을 보려면 NAppDll.cpp를 참조하십시오.
//
extern "C" _declspec(dllexport) void    NAppDll_Open(long pWnd, long nCamID, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void    NAppDll_Close(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetEvent(long nCAM, LPVOID lpCallBack, long pCallData);

extern "C" _declspec(dllexport) void	NAppDll_CreateModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL	NAppDll_LoadModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) BOOL	NAppDll_SaveModelData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) long	NAppDll_GetModelData(long nCamID);

extern "C" _declspec(dllexport) void    NAppDll_GetImageSize(long nCamID, long *nWidth, long *nHeight);
extern "C" _declspec(dllexport) void	NAppDll_SetImageSize(long nCamID, long nWidth, long nHeight);
extern "C" _declspec(dllexport) void	NAppDll_SetFrameSize(long nCamID, long nWidth, long nHeight, long nBand, long nDepth);

extern "C" _declspec(dllexport) void	NAppDll_SaveImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void	NAppDll_AsSaveImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void	NAppDll_LoadImageData(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) void    NAppDll_SetEtcFilePath(long nCamID, LPCTSTR strFilePath);
extern "C" _declspec(dllexport) long	NAppDll_GetImagePtr(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetImagePtr(long nCamID, long pImage);

extern "C" _declspec(dllexport) void	NAppDll_InitResultData(long nCamID);
extern "C" _declspec(dllexport) long	NAppDll_GetResultData(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetSpecData(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetFileName(long nCamID, LPCTSTR strFileName);
extern "C" _declspec(dllexport) long	NAppDll_ExecuteInspection(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_SetTransLanguage(long nCamID, long nValue);

extern "C" _declspec(dllexport) void	NAppDll_SetRemoteLight(long nCamID, BOOL bLight);
extern "C" _declspec(dllexport) long	NAppDll_GetBrightnessValue(long nCamID, long nCh);
extern "C" _declspec(dllexport) void	NAppDll_InvalidateView(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_ActiveSetupDialog(long nCamID);
extern "C" _declspec(dllexport) void	NAppDll_SetZoomScale(long nCamID, float fScale);
extern "C" _declspec(dllexport) float	NAppDll_GetZoomScale(long nCamID);
extern "C" _declspec(dllexport) void    NAppDll_RealTimeMode(long nCamID, BOOL bRealTime);

extern "C" _declspec(dllexport) void    NAppDll_SetPressKey(long nCamID, long nCtrlKey, long nKey);

class CNAppDllApp : public CWinApp
{
public:
	CNAppDllApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
