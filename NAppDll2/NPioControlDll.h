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

class CNPioControlDllApp : public CWinApp
{
public:
	CNPioControlDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
