#if !defined(AFX_EURESISCAMLIB_H__F4C60418_DC43_4C48_92F4_1EB05AAE867A__INCLUDED_)
#define AFX_EURESISCAMLIB_H__F4C60418_DC43_4C48_92F4_1EB05AAE867A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EuresisCamLib.h : header file
//

#include "..\..\..\VLib\HW\Grabber\Euresys\MultiCam\Include\multicam.h"

/////////////////////////////////////////////////////////////////////////////
// CEuresisCamLib window
class CEuresisCamLib 
{
// Construction
public:
	CEuresisCamLib();

	void     InitBoard1();
	void     InitBoard2();
	void     SetTriggerMode(long nCamID, BOOL bHWTrigger);
	void     SetActivateTrigger(long nCamID, BOOL bFlag);
	void     SetExposureTime(long nCamID, long nBright);
    void     SetImagePtr(long nCam, LPBYTE pImage);
	void     Callback1(PMCSIGNALINFO SigInfo);
	void     Callback2(PMCSIGNALINFO SigInfo);

// Attributes
public:
	LPVOID      m_pCurrent;
	LPBYTE      m_pImageT;
	LPBYTE      m_pImage[CAM_MAX_NUM];

	MCHANDLE    m_Channel_1;
	MCHANDLE    m_Channel_2;
	MCSTATUS    m_nStatus;

	INT32       m_SizeX;
	INT32       m_SizeY;
    INT32    	m_BufferPitch;

// Operations
public:


// Implementation
public:
	virtual ~CEuresisCamLib();

	// Generated message map functions
protected:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EURESISCAMLIB_H__F4C60418_DC43_4C48_92F4_1EB05AAE867A__INCLUDED_)
