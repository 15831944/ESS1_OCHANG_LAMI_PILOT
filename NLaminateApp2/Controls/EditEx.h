#if !defined(AFX_EDITEX_H__5D3BB2DD_9B3B_11D4_B4AE_00C026DD30D4__INCLUDED_)
#define AFX_EDITEX_H__5D3BB2DD_9B3B_11D4_B4AE_00C026DD30D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();

// Attributes
public:
	CRect m_rectClient;
	CString m_stText;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

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

	void SetTextOut(CString txt);
	CString GetText() {return m_stText;}
	double GetTextValue()
	{
		TCHAR csValue[256];
		GetWindowText(csValue, 256);
		return ::_ttof(csValue);
	}

	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
//	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

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

	CString m_stFontName;
	int m_nFontSize;
	int m_nFontStyle;
	int m_nFontThickness;
	CFont m_ftFont; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__5D3BB2DD_9B3B_11D4_B4AE_00C026DD30D4__INCLUDED_)
