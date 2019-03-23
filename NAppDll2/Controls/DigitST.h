/////////////////////////////////////////////////////////////////////////////
// DigitST.h : header file
/////////////////////////////////////////////////////////////////////////////
#ifndef _DIGITST_H
#define _DIGITST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Min number of digits we can display
#define ST_MIN_PRECISION	1
// Max number of digits we can display (it can be increased)
#define ST_MAX_PRECISION	10
// Space left around the bitmap for border
#define BORDER_SPACE		3

/////////////////////////////////////////////////////////////////////////////
class CDigitST : public CStatic
{
public:
	BOOL	m_nInt;
public:
	CDigitST();
	virtual ~CDigitST();
    enum {	ST_LEFT			= 0x01, 
			ST_TOP			= 0x02,
			ST_RIGHT		= 0x04,
			ST_BOTTOM		= 0x08};

private:
	CBitmap m_bmDigit;
	int m_nWidth;
	int m_nHeight;

	int m_nPrecision;
	double m_dblValue;

	BOOL m_bZeroPadding;
	DWORD m_dwResize;

private:
	void Resize();
	void PrepareString(char* szDest);
	void DrawDigits(CDC* pDC);

public:
	BOOL   SetStyle(UINT nBitmapId, int nPrecision);
	
	void   SetValue(double dblValue, BOOL bRepaint = TRUE);
	double GetValue();

	void   SetPrecision(int nPrecision, BOOL bResize = TRUE);
	int    GetPrecision();

	void   SetResize(DWORD dwResize, BOOL bResize = TRUE);
	DWORD  GetResize();

	void   SetZeroPadding(BOOL bPad = FALSE, BOOL bRepaint = TRUE);
	BOOL   GetZeroPadding();

	void   Inc(BOOL bRepaint = TRUE);
	void   Dec(BOOL bRepaint = TRUE);

	static const char* GetVersionC();
	static const short GetVersionI();

public:
	//{{AFX_VIRTUAL(CDigitST)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDigitST)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
#endif
