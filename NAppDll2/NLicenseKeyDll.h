// NLicenseKeyDll.h : main header file for the NLicenseKeyDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CNLicenseKeyDllApp
// See NLicenseKeyDll.cpp for the implementation of this class

extern "C" _declspec(dllexport) BOOL IsLicenseKey(LPCTSTR strLockType);


class CNLicenseKeyDllApp : public CWinApp
{
public:
	CNLicenseKeyDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
