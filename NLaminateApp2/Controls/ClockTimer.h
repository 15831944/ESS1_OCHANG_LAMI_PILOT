#if !defined(AFX_CLOCKTIMER_H__FAEAAFEB_76F6_433C_8A9F_53EA7CA1B9A0__INCLUDED_)
#define AFX_CLOCKTIMER_H__FAEAAFEB_76F6_433C_8A9F_53EA7CA1B9A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClockTimer.h : header file
//

#define TIMER_MAX_NUM         50

/////////////////////////////////////////////////////////////////////////////
// CClockTimer window

class CClockTimer : public CStatic
{
// Construction
public:
	CClockTimer();
    void       SetClockTimer(long id);
    double     GetClockTimer(long id);

// Attributes
public:
	double     m_dSetTime[TIMER_MAX_NUM];
	double     m_dEndTime[TIMER_MAX_NUM];

	LARGE_INTEGER m_nStartTick[TIMER_MAX_NUM];
	LARGE_INTEGER m_nEndTick[TIMER_MAX_NUM];
	LARGE_INTEGER m_nPreqTick[TIMER_MAX_NUM];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockTimer)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClockTimer();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClockTimer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKTIMER_H__FAEAAFEB_76F6_433C_8A9F_53EA7CA1B9A0__INCLUDED_)
