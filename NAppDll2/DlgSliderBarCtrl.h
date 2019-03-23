#pragma once

#include "resource.h"
#define SLIDE_MAX_NUM          3
// DlgSliderBarCtrl dialog

class CDlgSliderBarCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSliderBarCtrl)

public:
	CDlgSliderBarCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSliderBarCtrl();

	void     SetEvent();
	void     DrawScrollBar();
	void     SetHandleWnd(CWnd *pWnd);
	void     SetScrollRange(long nSlidNum, float fMinR, float fMaxR);
	void     SetScrollPos(long nSlide, float fMinL);
	float    GetScrollPos(long nSlide);
	void     DrawFillCircle(CDC *pDC, long x, long y, long nRadius, COLORREF nColor);
	void     SetNormalRange(long nSlidNum, float fLowPos, float fHighPos, COLORREF nColor);
	void     SetWarningRange(long nSlidNum, float fLowPos, float fHighPos, COLORREF nColor);

// Dialog Data
	enum { IDD = IDD_DLG_SLIDEBAR_CTRL };

public:
	CWnd     *m_pWndParent;

private :
	CWnd    *m_pWnd;
	CDC     *m_pDC;
	CRect    m_rcBound;

	long     m_nSlideMaxNum;
	long     m_nMouseSel;

	float    m_fMinR;
	float    m_fMaxR;
	float    m_fSlidePos[3];

	float    m_fWarningLowR;
	float    m_fWarningHighR;
	COLORREF m_nWarningColor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
