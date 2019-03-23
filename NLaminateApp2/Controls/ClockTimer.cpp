// ClockTimer.cpp : implementation file
//
#include "stdafx.h"
#include "ClockTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockTimer
CClockTimer::CClockTimer()
{

}

CClockTimer::~CClockTimer()
{

}

BEGIN_MESSAGE_MAP(CClockTimer, CStatic)
	//{{AFX_MSG_MAP(CClockTimer)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClockTimer message handlers
void CClockTimer::SetClockTimer(long id)
{
//	m_dSetTime[id] = GetTickCount();
	QueryPerformanceCounter(&m_nStartTick[id]) ;
}

double CClockTimer::GetClockTimer(long id)
{
	double dTime;
//    m_dEndTime[id] = GetTickCount();
//    return (m_dEndTime[id] - m_dSetTime[id]) / 1000.0;

	QueryPerformanceCounter(&m_nEndTick[id]);
	QueryPerformanceFrequency(&m_nPreqTick[id]);
    dTime = ((double)(m_nEndTick[id].QuadPart - m_nStartTick[id].QuadPart)) / ((double)m_nPreqTick[id].QuadPart);

	return dTime;
}
