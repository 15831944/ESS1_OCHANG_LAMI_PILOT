// CCPUTimer.h: interface for the CCPUTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCPUTimer_H__816DDD41_1BE5_11D4_835D_00105A83EED2__INCLUDED_)
#define AFX_CCPUTimer_H__816DDD41_1BE5_11D4_835D_00105A83EED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCPUTimer
{
public:
	__int64 start,end, freq;
//	long start,end;
	BOOL	m_bRun;
public:
	//CCPUTimer(double cpu_clock);
	CCPUTimer();
	virtual ~CCPUTimer();
	BOOL IsRun();

	long GetSecond();
	void Start(void);
	void End(void);
	double GetTime();
};

#endif // !defined(AFX_CCPUTimer_H__816DDD41_1BE5_11D4_835D_00105A83EED2__INCLUDED_)
