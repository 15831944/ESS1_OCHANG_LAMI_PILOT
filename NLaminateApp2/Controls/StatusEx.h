#ifndef _STATUSEX_H
#define _STATUSEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusEx window


#define RESERVED_MAP_NUM			512


class CStatusEx : public CStatic
{
// Construction
public:
	CStatusEx();

private:
	CRect m_rectClient;
	CBrush m_brushBack;//Color of brush to paint brush

	LOGPEN m_logPen;
	LOGBRUSH m_logBrush;

public:
	CString m_stText;
	CString m_stFontName;

	int m_nFontSize;
	int m_nFontStyle;
	int m_nFontThickness;
	
	CFont m_ftFont; 

	CBitmap m_Bmp;

// Operations
public:
	void MouseMove(UINT nFlags, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatusEx();

	virtual void SetTextColor(COLORREF crTextColor = 0xffffffff);
	virtual COLORREF GetTextColor();

	virtual void SetBkColor(COLORREF crBkColor = 0xffffffff);
	virtual COLORREF GetBkColor();

	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = 1000);

	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);
	void SetBkBlink(BOOL bStart = TRUE, UINT nElapse = 1000);

	virtual void SetFontName(CString fntName);
	virtual CString GetFontName();

	void SetFontSize(int nSize);
	int GetFontSize();

	void SetFontThickness(int nSize);
	int GetFontThickness();

	void SetTextOut(CString txt);
	CString GetText() {return m_stText;}

private:
	UINT m_nTimerId;

	COLORREF m_crTextColor;
	COLORREF m_crBlinkTextColors[2];
	BOOL m_bTextBlink;
	int m_nTextBlinkStep;

	COLORREF m_crBkColor;
	COLORREF m_crBlinkBkColors[2];
	BOOL m_bBkBlink;
	int m_nBkBlinkStep;

	CBrush m_brBkgnd;
	CBrush m_brBlinkBkgnd[2];

	CWnd* m_pParent;
	UINT m_nMsg;

protected:
	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

	// Generated message map functions
protected:
	//{{AFX_MSG(CStatusEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#endif//_STATUSEX_H
