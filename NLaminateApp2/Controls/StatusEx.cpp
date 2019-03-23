// StaticEx.cpp : implementation file
//

#include "stdafx.h"
#include "StatusEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusEx

CStatusEx::CStatusEx()
{
	m_pParent = NULL;
	m_nMsg = WM_USER;

	m_logPen.lopnStyle				= PS_SOLID;
	m_logPen.lopnWidth.x			= 1;
	m_logPen.lopnWidth.y			= 1;
	m_logPen.lopnColor				= RGB(255,0,0);

	m_logBrush.lbStyle				= BS_SOLID;
	m_logBrush.lbColor				= RGB(0, 255, 0);
	m_logBrush.lbHatch				= HS_HORIZONTAL;

	m_nFontStyle					= 2;
	m_nFontSize						= 16;
	m_nFontThickness				= 0;
	m_stFontName					= "MicrFontExt";


	// By default the control is not blinking
	m_bTextBlink = FALSE;
	m_nTextBlinkStep = 0;
	m_bBkBlink = FALSE;
	m_nBkBlinkStep = 0;
	m_nTimerId = 0;

	// Set default foreground text
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	// Set default foreground text (when blinking)
	m_crBlinkTextColors[0] = m_crTextColor;
	m_crBlinkTextColors[1] = m_crTextColor;

	// Set default background text
	m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	// Set default background text (when blinking)
	m_crBlinkBkColors[0] = m_crBkColor;
	m_crBlinkBkColors[1] = m_crBkColor;

	// Set default background brush
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	// Set default background brush (when blinking)
	m_brBlinkBkgnd[0].CreateSolidBrush(m_crBkColor);
	m_brBlinkBkgnd[1].CreateSolidBrush(m_crBkColor);


	m_ftFont.CreateFont(m_nFontSize,
		0,
		0,
		0,
		FW_BOLD,	//nIsBold, 
		1,			//bIsItalic, 
		0,			//bIsUnderline, 
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_SWISS,
		m_stFontName);
}

CStatusEx::~CStatusEx()
{
	m_brBkgnd.DeleteObject();
	m_ftFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CStatusEx, CStatic)
	//{{AFX_MSG_MAP(CStatusEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
ON_WM_ERASEBKGND()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusEx message handlers


void CStatusEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	GetClientRect(&m_rectClient);

	CDC	m_dcPlot;//Device context for plotting merged objects on image
	CBitmap m_bitmapPlot;//Shape of the display memory
	CBitmap *m_bitmapOldPlot;//Shape of the display memory
	
	GetClientRect(&m_rectClient);
	m_dcPlot.CreateCompatibleDC(&dc);
	m_bitmapPlot.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
	m_bitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot);

	CFont *pFontOld;
	pFontOld = m_dcPlot.SelectObject(&m_ftFont);

	m_dcPlot.BitBlt(m_rectClient.left,m_rectClient.top,m_rectClient.Width(),m_rectClient.Height(),&dc,0,0,SRCCOPY);

	m_dcPlot.SetBkColor(TRANSPARENT);
	m_dcPlot.SetBkMode(TRANSPARENT);

	CRgn rgnA;
	VERIFY(rgnA.CreateRoundRectRgn(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom, 16, 16));

	CBrush brA;

	if (m_bBkBlink==TRUE) {VERIFY(brA.CreateSolidBrush(m_crBlinkBkColors[m_nBkBlinkStep]));}
	else {VERIFY(brA.CreateSolidBrush(m_crBkColor));}

	VERIFY(m_dcPlot.FillRgn(&rgnA, &brA));

	m_dcPlot.SetTextColor(m_crTextColor);
	m_dcPlot.DrawText(m_stText, m_rectClient, DT_SINGLELINE | DT_NOCLIP | DT_EXPANDTABS | DT_LEFT | DT_VCENTER);
	m_dcPlot.SetTextAlign(TA_CENTER);
	m_dcPlot.SelectObject(pFontOld);

	dc.BitBlt(m_rectClient.left,m_rectClient.top,m_rectClient.Width(),m_rectClient.Height(),&m_dcPlot,0,0,SRCCOPY);

	DeleteObject(m_bitmapPlot.m_hObject);
	m_dcPlot.SelectObject(m_bitmapOldPlot);
	m_dcPlot.DeleteDC();
}

HBRUSH CStatusEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Set foreground color
	// If control is blinking (text)
	if (m_bTextBlink == TRUE)
	{
		pDC->SetTextColor(m_crBlinkTextColors[m_nTextBlinkStep]);
	}
	else
	{
		pDC->SetTextColor(m_crTextColor);
	}

	// Set background color & brush
	// If control is blinking (background)
	if (m_bBkBlink == TRUE)
	{
		pDC->SetBkColor(m_crBlinkBkColors[m_nBkBlinkStep]);
		return (HBRUSH)m_brBlinkBkgnd[m_nBkBlinkStep];
	}
	else
	{
		// If control is not blinking (background)
		pDC->SetBkColor(m_crBkColor);
	}
	// Return a non-NULL brush if the parent's handler should not be called
    return (HBRUSH)m_brBkgnd;
}

void CStatusEx::EnableNotify(CWnd* pParent, UINT nMsg)
{
	m_pParent = pParent;
	m_nMsg = nMsg;
}

void CStatusEx::SetTextColor(COLORREF crTextColor)
{
	if (crTextColor != 0xffffffff)
	{
		m_crTextColor = crTextColor;
	}
	else
	{
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	}

	Invalidate(FALSE);
}

COLORREF CStatusEx::GetTextColor()
{
	return m_crTextColor;
}

void CStatusEx::SetBkColor(COLORREF crBkColor)
{
	if (crBkColor != 0xffffffff)
	{
		m_crBkColor = crBkColor;
	}
	else
	{
		m_crBkColor = ::GetSysColor(COLOR_BTNFACE);
	}

	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	Invalidate(FALSE);
}

COLORREF CStatusEx::GetBkColor()
{
	return m_crBkColor;
}

void CStatusEx::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimerId)	
	{
		// If control is blinking (text) switch its color
		if (m_bTextBlink == TRUE) m_nTextBlinkStep = !m_nTextBlinkStep;

		// If control is blinking (background) switch its color
		if (m_bBkBlink == TRUE) m_nBkBlinkStep = !m_nBkBlinkStep;

		// If there is any blinking in action then repaint the control
		// and send the notification message (if any)
		if (m_bBkBlink == TRUE || m_bTextBlink == TRUE) 
		{
			Invalidate();
			// Send notification message only on rising blink
		}
	}
	else
		CStatic::OnTimer(nIDEvent);
}

void CStatusEx::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// Destroy timer (if any)
	if (m_nTimerId > 0) KillTimer(m_nTimerId);

	// Destroy resources
    m_brBkgnd.DeleteObject();
    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[1].DeleteObject();
}

void CStatusEx::SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2)
{
	// Set new blink text colors
	m_crBlinkTextColors[0] = crBlinkTextColor1;
	m_crBlinkTextColors[1] = crBlinkTextColor2;

} // End of SetBlinkTextColors


void CStatusEx::StartTextBlink(BOOL bStart, UINT nElapse)
{
	// Destroy any previous timer
	if (m_nTimerId > 0)	
	{
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bTextBlink = bStart;
	m_nTextBlinkStep = 0;

	if(m_bTextBlink == TRUE) 
		m_nTimerId = (UINT)SetTimer(1, nElapse, NULL); 
}


void CStatusEx::SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2)
{
	m_crBlinkBkColors[0] = crBlinkBkColor1;
	m_crBlinkBkColors[1] = crBlinkBkColor2;

    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[0].CreateSolidBrush(m_crBlinkBkColors[0]);
    m_brBlinkBkgnd[1].DeleteObject();
    m_brBlinkBkgnd[1].CreateSolidBrush(m_crBlinkBkColors[1]);

	Invalidate(FALSE);
}

void CStatusEx::SetBkBlink(BOOL bStart, UINT nElapse)
{
	// Destroy any previous timer
	if (m_nTimerId > 0)	
	{
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bBkBlink = bStart;
	m_nBkBlinkStep = 0;

	// Start timer
	if (m_bBkBlink == TRUE) 
		m_nTimerId = (UINT)SetTimer(1, nElapse, NULL);
}

void CStatusEx::SetTextOut(CString txt)
{
	CTime cTime = CTime::GetCurrentTime();
//	m_stText.Format(_T(" %4d/%02d/%02d/%02d/%02d/%02d : %s"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond(), txt);
	m_stText.Format(_T(" %s"), txt);

	Invalidate(FALSE);
}

void CStatusEx::SetFontName(CString fntName)
{
	m_stFontName.Empty();
	m_stFontName = fntName;
	m_ftFont.DeleteObject();
	m_ftFont.CreateFont(m_nFontSize,
		0,
		0,
		0,
		FW_BOLD,	//nIsBold, 
		1,			//bIsItalic, 
		0,			//bIsUnderline, 
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_stFontName);

	SetFont(&m_ftFont);
	Invalidate(FALSE);
}

CString CStatusEx::GetFontName()
{
	return m_stFontName;
}

void CStatusEx::SetFontSize(int nSize)
{
	m_nFontSize = -nSize;
	m_ftFont.DeleteObject();
	m_ftFont.CreateFont(m_nFontSize,
		0,
		0,
		0,
		FW_BOLD,	//nIsBold, 
		1,			//bIsItalic, 
		0,			//bIsUnderline, 
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_stFontName);

	SetFont(&m_ftFont);
	Invalidate(FALSE);
}

int CStatusEx::GetFontSize()
{
	return m_nFontSize;
}

void CStatusEx::SetFontThickness(int nSize)
{
	if(nSize>9) return;
	m_nFontThickness = nSize*100;
	m_ftFont.DeleteObject();
	m_ftFont.CreateFont(m_nFontSize,
		0,
		0,
		0,
		FW_BOLD,//nIsBold, 
		1, //bIsItalic, 
		0, //bIsUnderline, 
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_stFontName);

	SetFont(&m_ftFont);
	Invalidate(FALSE);
}

int CStatusEx::GetFontThickness()
{
	m_nFontSize/=100;
	Invalidate(FALSE);

	return m_nFontSize;
}

void CStatusEx::MouseMove(UINT nFlags, CPoint point)
{
}

void CStatusEx::OnMouseMove(UINT nFlags, CPoint point)
{
	(GetParent()->SendMessage)( WM_MOUSEMOVE, nFlags, point.y*0x10000+point.x  );

//	CStatic::OnMouseMove(nFlags, point);
}
