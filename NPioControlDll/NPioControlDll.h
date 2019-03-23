// NPioControlDll.h : main header file for the NPioControlDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNPioControlDllApp
// See NPioControlDll.cpp for the implementation of this class
//
extern "C" _declspec(dllexport) BOOL    NPioControlDll_Open(long nPort, long nBaud, long nSize, long nParity, long nStop);
extern "C" _declspec(dllexport) void    NPioControlDll_Close();
extern "C" _declspec(dllexport) void    NPioControlDll_Write(long nData);
extern "C" _declspec(dllexport) void    NPioControlDll_BitWrite(long nBit, BOOL bSig);
extern "C" _declspec(dllexport) long    NPioControlDll_Read();
extern "C" _declspec(dllexport) BOOL    NPioControlDll_BitRead(long nBit);
extern "C" _declspec(dllexport) BOOL    NPioControlDll_BitReadABS(long nBit);
extern "C" _declspec(dllexport) void    NPioControlDll_SetLight(long nCh, long nBright);
extern "C" _declspec(dllexport) void    NPioControlDll_Exit();

extern "C" _declspec(dllexport) BOOL    NUmacDll_Open(long nDevice);
extern "C" _declspec(dllexport) void    NUmacDll_Close();
extern "C" _declspec(dllexport) double  NUmacDll_GetDoubleValue(char ch, long nAddr);
extern "C" _declspec(dllexport) int     NUmacDll_GetIntValue(char ch, long nAddr);
extern "C" _declspec(dllexport) void    NUmacDll_SetDoubleValue(char ch, long nAddr, double dValue);
extern "C" _declspec(dllexport) void    NUmacDll_SetIntValue(char ch, long nAddr, short int nValue);

extern "C" _declspec(dllexport) BOOL    NComPort_Open(long nPort, long nBaud, long nSize, long nParity, long nStop);
extern "C" _declspec(dllexport) BOOL    NComPort_Read(long nPort, char *szbuf);
extern "C" _declspec(dllexport) BOOL    NComPort_Write(long nPort, char chComm, char *szbuf);
extern "C" _declspec(dllexport) BOOL    NComPort_Mode(long nPort, char chMode);
extern "C" _declspec(dllexport) BOOL    NComPort_ClearCount(long nPort);
extern "C" _declspec(dllexport) void    NComPort_Close(long nPort);
extern "C" _declspec(dllexport) BOOL    NComPort_GetVersion(long nPort);
extern "C" _declspec(dllexport) BOOL    NComPort_BufferClear(long nPort);
extern "C" _declspec(dllexport) void    NComPort_SetLight(long nPort, long nCh, long *nBright);

class CNPioControlDllApp : public CWinApp
{
public:
	CNPioControlDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
