// NMultiFuncDll.h : main header file for the NMultiFuncDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNMultiFuncDllApp
// See NMultiFuncDll.cpp for the implementation of this class
//

class CNMultiFuncDllApp : public CWinApp
{
public:
	CNMultiFuncDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern "C" _declspec(dllexport) void NMultiFuncDll_Trans90ConvertImage(LPBYTE pSourImage, long nSourWidth, long nSourHeight, LPBYTE pDestImage, long nDestWidth, long nDestHeight, long nType);
