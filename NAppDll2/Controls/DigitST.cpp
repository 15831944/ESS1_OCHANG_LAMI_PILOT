/////////////////////////////////////////////////////////////////////////////
// DigitST.cpp
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DigitST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDigitST, CStatic)
	//{{AFX_MSG_MAP(CDigitST)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CDigitST::CDigitST()
{
	m_nPrecision = 2;
	m_dblValue = 0.0;
	m_bZeroPadding = FALSE;
	m_dwResize = ST_RIGHT | ST_BOTTOM;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nInt = FALSE;
}

CDigitST::~CDigitST()
{
}

/////////////////////////////////////////////////////////////////////////////
void CDigitST::OnPaint() 
{
	PAINTSTRUCT lpPaint;

	BeginPaint(&lpPaint);

	CWindowDC dc(this);

	if (m_bmDigit.m_hObject) DrawDigits(&dc);

	EndPaint(&lpPaint);
	
}

/////////////////////////////////////////////////////////////////////////////
BOOL CDigitST::SetStyle(UINT nBitmapId, int nPrecision)
{
	BITMAP bm;
	BOOL bRet;
	m_bmDigit.Detach();
	bRet = m_bmDigit.LoadBitmap(nBitmapId);
	if (bRet == TRUE)
	{
		m_bmDigit.GetBitmap(&bm);
		m_nWidth = (int)bm.bmWidth / 13;
		m_nHeight = bm.bmHeight;
		SetPrecision(nPrecision);
	}

	return bRet;
}

void CDigitST::SetValue(double dblValue, BOOL bRepaint)
{
	m_dblValue = dblValue;
	if (bRepaint == TRUE) Invalidate();
}

double CDigitST::GetValue()
{
	return m_dblValue;
}

void CDigitST::SetPrecision(int nPrecision, BOOL bResize)
{
	if (nPrecision >= ST_MIN_PRECISION && nPrecision <= ST_MAX_PRECISION)
		m_nPrecision = nPrecision;
	if (bResize == TRUE) Resize();
}

int CDigitST::GetPrecision()
{
	return m_nPrecision;
}

void CDigitST::SetResize(DWORD dwResize, BOOL bResize)
{
	m_dwResize = dwResize;
	if (bResize == TRUE) Resize();
}

DWORD CDigitST::GetResize()
{
	return m_dwResize;
}

void CDigitST::SetZeroPadding(BOOL bPad, BOOL bRepaint)
{
	m_bZeroPadding = bPad;
	if (bRepaint == TRUE) Invalidate();
}

BOOL CDigitST::GetZeroPadding()
{
	return m_bZeroPadding;
}

void CDigitST::Inc(BOOL bRepaint)
{
	SetValue(GetValue() + 1, bRepaint);
}

void CDigitST::Dec(BOOL bRepaint)
{
	SetValue(GetValue() - 1, bRepaint);
}

const char* CDigitST::GetVersionC()
{
  return "1.0";
}

const short CDigitST::GetVersionI()
{
  return 10;
}

void CDigitST::DrawDigits(CDC* pDC)
{
	char szValue[ST_MAX_PRECISION+1];

	int nLoop;
	int destX;
	CRect rectCtrl;
	int nAsciiChar;
	CBitmap* pOldBitmap;

    CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	pOldBitmap = memDC.SelectObject(&m_bmDigit);
	
	GetClientRect(rectCtrl);

	destX = BORDER_SPACE;

	PrepareString(szValue);

	for (nLoop = 0; nLoop < m_nPrecision; nLoop++)
	{
		if (m_bZeroPadding == TRUE && m_dblValue >= 0)
			nAsciiChar = 0;
		else
			nAsciiChar = 10;

		if (szValue[nLoop] >= '0' && szValue[nLoop] <= '9')
			nAsciiChar = szValue[nLoop] - 48;

		if (szValue[nLoop] == '-') nAsciiChar = 11;

		if (szValue[nLoop] == '.') nAsciiChar = 12;

		pDC->BitBlt(destX, BORDER_SPACE, m_nWidth, m_nHeight, &memDC, 0+(m_nWidth*nAsciiChar), 0, SRCCOPY);
		destX += m_nWidth;
	}

	memDC.SelectObject(pOldBitmap);
}

void CDigitST::Resize()
{
	CRect rectCtrl;

	Invalidate();

	GetWindowRect(rectCtrl);
	GetParent()->ScreenToClient(rectCtrl);

	if (m_dwResize & ST_RIGHT)
		rectCtrl.right = rectCtrl.left + (m_nWidth * m_nPrecision)+(BORDER_SPACE*2);
	else
		rectCtrl.left = rectCtrl.right - (m_nWidth * m_nPrecision)-(BORDER_SPACE*2);

	if (m_dwResize & ST_BOTTOM)
		rectCtrl.bottom = rectCtrl.top + m_nHeight+(BORDER_SPACE*2);
	else
		rectCtrl.top = rectCtrl.bottom - m_nHeight-(BORDER_SPACE*2);

	MoveWindow(rectCtrl);
}

void CDigitST::PrepareString(char* szDest)
{
	char szStr[ST_MAX_PRECISION+1];	

	double dblValue = (m_dblValue - (int)m_dblValue);

	if(m_nInt == FALSE)
	{
		printf(szStr, "%10.1Lf", m_dblValue);
	}
	else
	{
		sprintf(szStr, "%10.0Lf", m_dblValue);
	}
//	if(dblValue == 0.0 && m_nInt == FALSE)
//	{
//		sprintf(szStr, "%10.0Lf", m_dblValue);
//	}
//	else
//	{
//		sprintf(szStr, "%10.1Lf", m_dblValue);
//	}

	sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
}


#undef ST_MIN_PRECISION
#undef ST_MAX_PRECISION
#undef BORDER_SPACE
