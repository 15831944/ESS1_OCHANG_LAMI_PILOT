#if !defined(AFX_BASLERCAMLIB_H__91DDECCC_24FF_4B59_991A_3886AAB3080E__INCLUDED_)
#define AFX_BASLERCAMLIB_H__91DDECCC_24FF_4B59_991A_3886AAB3080E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaslerCamLib.h : header file
//
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <pylonc/PylonC.h>

/////////////////////////////////////////////////////////////////////////////
// CBaslerCamLib window

class CBaslerCamLib 
{
// Construction
public:
	CBaslerCamLib();

// Attributes
public:
	BOOL     FindCamera();
	BOOL     Open(int nCam);
	void     Close(int nCamNum);
	void     SetExposureTime(int nCamNum, double fValue);
    BOOL     GetImageBySoftTrigger(int nCamNum, LPBYTE pImage, int nWidth, int nHeight);
    void     SetGrabMode(int nCam, long nMode);
    void     ExecuteUserLoad(int nCamIndex);
    void     ExecuteUserSave(int nCamIndex);
    void     SetGain(int nCamIndex, int nGain);

// Operations
public:
	PYLON_DEVICE_HANDLE     m_hDevice[10];          /* Handle for the pylon device. */
	PylonDeviceInfo_t       m_hDeviceInfo[10];
	CString  m_szSerial[10];

// Implementation
public:
	virtual ~CBaslerCamLib();

	// Generated message map functions
protected:
	LPBYTE   m_pGrabImage;
	unsigned int   m_nCamNum;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASLERCAMLIB_H__91DDECCC_24FF_4B59_991A_3886AAB3080E__INCLUDED_)
