// DlgSliderBarCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "NAppDll.h"
#include "DlgSliderBarCtrl.h"
#include "afxdialogex.h"
// DlgSliderBarCtrl dialog

IMPLEMENT_DYNAMIC(CDlgSliderBarCtrl, CDialogEx)

CDlgSliderBarCtrl::CDlgSliderBarCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSliderBarCtrl::IDD, pParent)
{
	long i;

	m_pWnd = pParent;
	m_pDC = NULL;
	m_nMouseSel = -1;
	for(i=0; i<SLIDE_MAX_NUM; i++)
	    m_fSlidePos[i] = 0;
}

CDlgSliderBarCtrl::~CDlgSliderBarCtrl()
{
	if (m_pDC) ReleaseDC(m_pDC);
}

void CDlgSliderBarCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgSliderBarCtrl, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgSliderBarCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pWnd->GetClientRect(&m_rcBound);
	m_pDC = m_pWnd->GetDC();

	m_fWarningLowR = (float)m_rcBound.right;
	m_fWarningHighR = (float)m_rcBound.right;
	m_nWarningColor = RGB(255, 170, 170);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSliderBarCtrl::SetScrollRange(long nSlidNum, float fMinR, float fMaxR)
{
	m_nSlideMaxNum = nSlidNum;
	m_fMinR = fMinR;
	m_fMaxR = fMaxR;
}

void CDlgSliderBarCtrl::SetNormalRange(long nSlidNum, float fLowPos, float fHighPos, COLORREF nColor)
{

}

void CDlgSliderBarCtrl::SetWarningRange(long nSlidNum, float fLowPos, float fHighPos, COLORREF nColor)
{
	m_nSlideMaxNum = nSlidNum;
	m_fWarningLowR = fLowPos;
	m_fWarningHighR = fHighPos;
	m_nWarningColor = nColor;
}

void CDlgSliderBarCtrl::SetScrollPos(long nSlide, float fSlidePos)
{ 
	m_fSlidePos[nSlide] = m_rcBound.Width()*(fSlidePos-m_fMinR)/(m_fMaxR-m_fMinR); 
}

float CDlgSliderBarCtrl::GetScrollPos(long nSlide)
{
	return (float)(m_fMinR + m_fSlidePos[nSlide]*(m_fMaxR-m_fMinR)/m_rcBound.Width());
}

void CDlgSliderBarCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	long i;
	m_nMouseSel = -1;

	for(i=0; i<m_nSlideMaxNum; i++)
	{
    	if (m_fSlidePos[i]-10<point.x && point.x<m_fSlidePos[i]+10)
		{
			m_nMouseSel = i;
			break;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CDlgSliderBarCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_nMouseSel>=0)
		{
		    m_fSlidePos[m_nMouseSel] = (float)point.x;
		}

		InvalidateRect(m_rcBound);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CDlgSliderBarCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonUp(nFlags, point);
}

void CDlgSliderBarCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	long i;
	CRect rcRect;
	CString str;
	CBrush *brushBColor;
	
	brushBColor = new CBrush(RGB(200, 200, 200));
 	m_pDC->FillRect(m_rcBound, brushBColor);
	delete brushBColor;

	brushBColor = new CBrush(m_nWarningColor);
	rcRect = m_rcBound;
	rcRect.left = (long)(m_rcBound.Width()*(m_fWarningLowR-m_fMinR)/(m_fMaxR-m_fMinR)); 
	rcRect.right = (long)(m_rcBound.Width()*(m_fWarningHighR-m_fMinR)/(m_fMaxR-m_fMinR)); 
 	m_pDC->FillRect(rcRect, brushBColor);
	delete brushBColor;

	brushBColor = new CBrush(RGB(0, 150, 0));
	for(i=0; i<m_nSlideMaxNum; i++)
	{
		rcRect.left = (long)m_fSlidePos[i]-7;
		rcRect.top = (long)m_rcBound.top;
		rcRect.right = (long)m_fSlidePos[i]+7;
		rcRect.bottom = (long)m_rcBound.bottom;

		m_pDC->FillRect(rcRect, brushBColor);

		str.Format(_T("%d"), (long)GetScrollPos(i));
		if ((long)m_fSlidePos[i]<m_rcBound.Width()/2)
		    m_pDC->TextOut((long)m_fSlidePos[i]+7, m_rcBound.top, str);
		else
			m_pDC->TextOut((long)m_fSlidePos[i]-32, m_rcBound.top, str);
	}

	delete brushBColor;
}

void CDlgSliderBarCtrl::DrawFillCircle(CDC *pDC, long x, long y, long nRadius, COLORREF nColor)
{
	CBrush *pNewBrush, *pOldBrush;
	CRect  rcRect;

	pNewBrush = new CBrush(nColor);
	pOldBrush = pDC->SelectObject(pNewBrush);

	rcRect.bottom = y+nRadius;
    rcRect.top = y-nRadius;
	rcRect.left = x-nRadius;
	rcRect.right = x+nRadius;

	pDC->Ellipse(rcRect);

	pDC->SelectObject(pOldBrush);
	delete pNewBrush;
}

