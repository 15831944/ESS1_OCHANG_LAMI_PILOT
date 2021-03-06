// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
	m_pParent = NULL;
	m_nMsg = WM_USER;

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

	m_nFontStyle					= 2;
	m_nFontSize						= 10;
	m_nFontThickness				= 0;
	m_stFontName					= "ARIAL";

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

CEditEx::~CEditEx()
{
	m_ftFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers
/*
void CEditEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC	m_dcPlot;//Device context for plotting merged objects on image
	CBitmap m_bitmapPlot;//Shape of the display memory
	CBitmap *m_bitmapOldPlot;//Shape of the display memory
	
	GetClientRect(&m_rectClient);
	m_dcPlot.CreateCompatibleDC(&dc);
	m_bitmapPlot.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
	m_bitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot);

	m_dcPlot.SetBkColor(TRANSPARENT);
	m_dcPlot.SetBkMode(TRANSPARENT);

	m_dcPlot.FillSolidRect(m_rectClient, m_crBkColor);

	CFont *pFontOld;
	pFontOld = m_dcPlot.SelectObject(&m_ftFont);

	GetWindowText(m_stText);
	m_dcPlot.SetTextColor(m_crTextColor);
	m_dcPlot.DrawText(m_stText, m_rectClient, DT_SINGLELINE|DT_NOCLIP|DT_EXPANDTABS|DT_LEFT|DT_VCENTER);
	m_dcPlot.SetTextAlign(TA_CENTER);
	m_dcPlot.SelectObject(pFontOld);

	dc.BitBlt(m_rectClient.left,m_rectClient.top,m_rectClient.Width(),m_rectClient.Height(),&m_dcPlot,0,0,SRCCOPY);

	DeleteObject(m_bitmapPlot.m_hObject);
	m_dcPlot.SelectObject(m_bitmapOldPlot);
	m_dcPlot.DeleteDC();
}
*/
HBRUSH CEditEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Set foreground color
	// If control is blinking (text)
	if (m_bTextBlink == TRUE)
	{
		pDC->SetTextColor(m_crBlinkTextColors[m_nTextBlinkStep]);
		pDC->SetTextAlign(TA_LEFT);
	}
	else
	{
		pDC->SetTextColor(m_crTextColor);
		pDC->SetTextAlign(TA_LEFT);
	}

	// Set background color & brush
	// If control is blinking (background)
	if (m_bBkBlink == TRUE)
	{
		pDC->SetBkColor(m_crBlinkBkColors[m_nBkBlinkStep]);
		return (HBRUSH)m_brBlinkBkgnd[m_nBkBlinkStep];
	}
	// If control is not blinking (background)
	pDC->SetBkColor(m_crBkColor);

	// Return a non-NULL brush if the parent's handler should not be called
    return (HBRUSH)m_brBkgnd;
}

void CEditEx::SetTextColor(COLORREF crTextColor)
{
	if (crTextColor != 0xffffffff)
	{
		m_crTextColor = crTextColor;
	}
	else
	{
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	}

	Invalidate();
}

COLORREF CEditEx::GetTextColor()
{
	return m_crTextColor;
}

void CEditEx::SetBkColor(COLORREF crBkColor)
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

	Invalidate();
}

COLORREF CEditEx::GetBkColor()
{
	return m_crBkColor;
}

void CEditEx::OnTimer(UINT nIDEvent) 
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
			if (m_pParent != NULL && (m_nBkBlinkStep == 1 || m_nTextBlinkStep == 1)) 
				m_pParent->PostMessage(m_nMsg, GetDlgCtrlID(), 0);
		}
	}
	else
		CEdit::OnTimer(nIDEvent);
}

void CEditEx::OnDestroy() 
{
	CEdit::OnDestroy();
	
	// Destroy timer (if any)
	if (m_nTimerId > 0) KillTimer(m_nTimerId);

	// Destroy resources
    m_brBkgnd.DeleteObject();
    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[1].DeleteObject();
}

void CEditEx::SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2)
{
	// Set new blink text colors
	m_crBlinkTextColors[0] = crBlinkTextColor1;
	m_crBlinkTextColors[1] = crBlinkTextColor2;
} // End of SetBlinkTextColors


void CEditEx::StartTextBlink(BOOL bStart, UINT nElapse)
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


void CEditEx::SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2)
{
	m_crBlinkBkColors[0] = crBlinkBkColor1;
	m_crBlinkBkColors[1] = crBlinkBkColor2;

    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[0].CreateSolidBrush(m_crBlinkBkColors[0]);
    m_brBlinkBkgnd[1].DeleteObject();
    m_brBlinkBkgnd[1].CreateSolidBrush(m_crBlinkBkColors[1]);

	Invalidate();
}

void CEditEx::SetBkBlink(BOOL bStart, UINT nElapse)
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

void CEditEx::SetTextOut(CString txt)
{
	m_stText.Empty();
	m_stText = txt;

	SetWindowText(txt);

	Invalidate(FALSE);
}

void CEditEx::EnableNotify(CWnd* pParent, UINT nMsg)
{
	m_pParent = pParent;
	m_nMsg = nMsg;
}

void CEditEx::SetFontName(CString fntName)
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
		VARIABLE_PITCH | FF_SWISS,
		m_stFontName);

	SetFont(&m_ftFont);
	Invalidate();
}

CString CEditEx::GetFontName()
{
	return m_stFontName;
}

void CEditEx::SetFontSize(int nSize)
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
		VARIABLE_PITCH | FF_SWISS,
		m_stFontName);

	SetFont(&m_ftFont);
	Invalidate();
}

int CEditEx::GetFontSize()
{
	return m_nFontSize;
}
