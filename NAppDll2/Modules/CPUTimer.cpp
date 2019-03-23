// CCPUTimer.cpp: implementation of the CCPUTimer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CPUTimer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCPUTimer::CCPUTimer()
{
	start = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	//cpu_clock=_cpu_clock;
	m_bRun = FALSE;
}

CCPUTimer::~CCPUTimer()
{

}

void CCPUTimer::Start(void)
{
//	start = GetTickCount();
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
	m_bRun = TRUE;
}

long CCPUTimer::GetSecond()
{
	double dTime;
	dTime = GetTime();
	long nValue = (long) (dTime/1000.0 + 0.5);
	return nValue;
}
void CCPUTimer::End(void)
{
//	end = GetTickCount();
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	m_bRun = FALSE;
}

BOOL CCPUTimer::IsRun()
{
	return m_bRun;
}

double CCPUTimer::GetTime()
{
	__int64 nTime;
	double msec;
	QueryPerformanceCounter((LARGE_INTEGER*)&nTime);
	nTime = nTime - start;
	msec=(double)nTime/freq;
	return  msec;
}
