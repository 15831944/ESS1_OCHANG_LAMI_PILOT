// OverlayRectGOM.cpp : implementation file
//
#include "stdafx.h"
#include "math.h"
#include "OverlayRectGOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayRectGOM

COverlayRectGOM::COverlayRectGOM()
{
}

COverlayRectGOM::~COverlayRectGOM()
{
}


BEGIN_MESSAGE_MAP(COverlayRectGOM, CStatic)
	//{{AFX_MSG_MAP(COverlayRectGOM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayRectGOM message handlers
BOOL COverlayRectGOM::DrawOverlayRect(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData)
{
	long nType;
	COLORREF nColor;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	float dScale;
	CPoint ptMouse;
	CRect rcRect;
	CString str;
	CFont *pNewFont, *pOldFont;

	m_pGOM = &pData->m_GOM[nIndex];
	m_pINSP = &pData->m_INSP[nIndex];
	nType = m_pINSP->m_nInspectType;
	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	pNewFont = new CFont;
	pNewFont->CreateFont(14, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    								   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
    m_FuncEtc.GetOffsetAdjustment(pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	if (nType == INSP_CAMERA_CAL)
	{
		ptInsp1 = m_pGOM->m_ptPoint1;
		ptInsp2 = m_pGOM->m_ptPoint2;
		ptInsp3 = m_pGOM->m_ptPoint3;
		ptInsp4 = m_pGOM->m_ptPoint4;
	}

	ptInsp1.x = (ptInsp1.x - rcRect.left)/dScale;
	ptInsp1.y = (ptInsp1.y - rcRect.top)/dScale;
	ptInsp2.x = (ptInsp2.x - rcRect.left)/dScale;
	ptInsp2.y = (ptInsp2.y - rcRect.top)/dScale;
	ptInsp3.x = (ptInsp3.x - rcRect.left)/dScale;
	ptInsp3.y = (ptInsp3.y - rcRect.top)/dScale;
	ptInsp4.x = (ptInsp4.x - rcRect.left)/dScale;
	ptInsp4.y = (ptInsp4.y - rcRect.top)/dScale;

	if (m_pGOM->m_bSelectedROI)
	{
		nColor = m_pGOM->m_nSelectedColor;
		m_ImageBMP.DrawLine4(pDC, ptInsp1, ptInsp2, ptInsp3, ptInsp4, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptInsp1.x, (long)ptInsp1.y, 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptInsp2.x, (long)ptInsp2.y, 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptInsp3.x, (long)ptInsp3.y, 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptInsp4.x, (long)ptInsp4.y, 5, m_pGOM->m_nThick, nColor);

		m_ImageBMP.DrawFillCircle(pDC, (long)((ptInsp1.x+ptInsp2.x)/2.0f), (long)((ptInsp1.y+ptInsp2.y)/2.0f), 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)((ptInsp1.x+ptInsp3.x)/2.0f), (long)((ptInsp1.y+ptInsp3.y)/2.0f), 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)((ptInsp2.x+ptInsp4.x)/2.0f), (long)((ptInsp2.y+ptInsp4.y)/2.0f), 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)((ptInsp3.x+ptInsp4.x)/2.0f), (long)((ptInsp3.y+ptInsp4.y)/2.0f), 5, m_pGOM->m_nThick, nColor);

		if (nType==INSP_PATTERN_MATCH || nType==INSP_BLOB_ANALYSIS)
			m_ImageBMP.DrawCross(pDC, (long)(ptInsp1.x+ptInsp4.x)/2, (long)(ptInsp1.y+ptInsp4.y)/2, 5, 1, nColor);
	}
	else
	{
		if (m_pINSP->m_bLocROI) {
			if(nType==INSP_MEASURE_SIDE_ONE) nColor = RGB(0, 0, 200); 
			else nColor = RGB(0, 150, 255); 
		}
		else {
			if(nType==INSP_MEASURE_SIDE_ONE) nColor = RGB(0, 100, 0);
			else nColor = RGB(150, 255, 0);

		}

		if (m_pINSP->m_bInspMode && m_pINSP->m_bDecision) {
			if(nType==INSP_MEASURE_SIDE_ONE) m_ImageBMP.DrawLine4(pDC, ptInsp1, ptInsp2, ptInsp3, ptInsp4, m_pGOM->m_nThick, RGB(100, 0, 0));
			else m_ImageBMP.DrawLine4(pDC, ptInsp1, ptInsp2, ptInsp3, ptInsp4, m_pGOM->m_nThick, RGB(255, 0, 0));
		}
		else
        	m_ImageBMP.DrawLine4(pDC, ptInsp1, ptInsp2, ptInsp3, ptInsp4, m_pGOM->m_nThick, nColor);
	}
	
	if (m_pGOM->m_bShowTitle)
	{
		m_ImageBMP.DrawText(pDC, (long)ptInsp3.x, (long)ptInsp3.y, m_pGOM->m_strTitle, 1, nColor);
//		if ((ptInsp4.x-ptInsp1.x)>(ptInsp4.y-ptInsp1.y))
//          m_ImageBMP.DrawText(pDC, (long)(ptInsp1.x+ptInsp4.x)/2, (long)ptInsp1.y, m_pGOM->m_strTitle, 1, nColor);
//		else
//			m_ImageBMP.DrawText(pDC, (long)ptInsp1.x, (long)(ptInsp1.y+ptInsp4.y)/2, m_pGOM->m_strTitle, 1, nColor);
	}
	
	pDC->SelectObject(pOldFont);
	delete pNewFont;

	return TRUE;
}

BOOL COverlayRectGOM::SetSelectedRect(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, long *nDist)
{
	BOOL bRet=FALSE;
	POINT2  pt0, pt1, pt2, ptMid12, ptMid13, ptMid24, ptMid34, vtMouse;
	CPoint ptMouse;
	CRect rcRect;
	long nBound;
	float dScale;

	m_pGOM = &pData->m_GOM[nIndex];
	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);

	vtMouse.x = (rtMouse.x + rcRect.left/dScale) * dScale;
	vtMouse.y = (rtMouse.y + rcRect.top/dScale) * dScale;

	ptMid12.x = (m_pGOM->m_ptPoint1.x+m_pGOM->m_ptPoint2.x)/2.0f;
	ptMid12.y = (m_pGOM->m_ptPoint1.y+m_pGOM->m_ptPoint2.y)/2.0f;
	ptMid13.x = (m_pGOM->m_ptPoint1.x+m_pGOM->m_ptPoint3.x)/2.0f;
	ptMid13.y = (m_pGOM->m_ptPoint1.y+m_pGOM->m_ptPoint3.y)/2.0f;
	ptMid24.x = (m_pGOM->m_ptPoint2.x+m_pGOM->m_ptPoint4.x)/2.0f;
	ptMid24.y = (m_pGOM->m_ptPoint2.y+m_pGOM->m_ptPoint4.y)/2.0f;
	ptMid34.x = (m_pGOM->m_ptPoint3.x+m_pGOM->m_ptPoint4.x)/2.0f;
	ptMid34.y = (m_pGOM->m_ptPoint3.y+m_pGOM->m_ptPoint4.y)/2.0f;

    pt1.x = m_pGOM->m_ptPoint1.x;
	if (m_pGOM->m_ptPoint1.x>m_pGOM->m_ptPoint3.x) 
		pt1.x=m_pGOM->m_ptPoint3.x;

    pt1.y = m_pGOM->m_ptPoint1.y;
	if (m_pGOM->m_ptPoint1.y>m_pGOM->m_ptPoint2.y) 
		pt1.y=m_pGOM->m_ptPoint2.y;

    pt2.x = m_pGOM->m_ptPoint2.x;
	if (m_pGOM->m_ptPoint2.x<m_pGOM->m_ptPoint4.x) 
		pt2.x=m_pGOM->m_ptPoint4.x;

    pt2.y = m_pGOM->m_ptPoint3.y;
	if (m_pGOM->m_ptPoint3.y<m_pGOM->m_ptPoint4.y) 
		pt2.y=m_pGOM->m_ptPoint4.y;

	if (pt1.x<vtMouse.x && vtMouse.x<pt2.x && pt1.y<vtMouse.y && vtMouse.y<pt2.y)
	{
		pt0.x = (float)((pt1.x+pt2.x)/2.0);
		pt0.y = (float)((pt1.y+pt2.y)/2.0);

		*nDist = (long)m_libMath.GetDistance2D(vtMouse, pt0);
		bRet = RECKER_CENTER;
	}

	if (m_pGOM->m_ptPoint1.x-nBound<vtMouse.x && vtMouse.x<m_pGOM->m_ptPoint1.x+nBound &&
        m_pGOM->m_ptPoint1.y-nBound<vtMouse.y && vtMouse.y<m_pGOM->m_ptPoint1.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, m_pGOM->m_ptPoint1);
		bRet = RECKER_LEFT_TOP;
	}
	if (m_pGOM->m_ptPoint2.x-nBound<vtMouse.x && vtMouse.x<m_pGOM->m_ptPoint2.x+nBound &&
        m_pGOM->m_ptPoint2.y-nBound<vtMouse.y && vtMouse.y<m_pGOM->m_ptPoint2.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, m_pGOM->m_ptPoint2);
		bRet = RECKER_RIGHT_TOP;
	}
	if (m_pGOM->m_ptPoint3.x-nBound<vtMouse.x && vtMouse.x<m_pGOM->m_ptPoint3.x+nBound &&
        m_pGOM->m_ptPoint3.y-nBound<vtMouse.y && vtMouse.y<m_pGOM->m_ptPoint3.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, m_pGOM->m_ptPoint3);
		bRet = RECKER_LEFT_BOT;
	}
	if (m_pGOM->m_ptPoint4.x-nBound<vtMouse.x && vtMouse.x<m_pGOM->m_ptPoint4.x+nBound &&
        m_pGOM->m_ptPoint4.y-nBound<vtMouse.y && vtMouse.y<m_pGOM->m_ptPoint4.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, m_pGOM->m_ptPoint4);
		bRet = RECKER_RIGHT_BOT;
	}


	if (ptMid12.x-nBound<vtMouse.x && vtMouse.x<ptMid12.x+nBound &&
        ptMid12.y-nBound<vtMouse.y && vtMouse.y<ptMid12.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, ptMid12);
		bRet = RECKER_MID_TOP;
	}
	if (ptMid13.x-nBound<vtMouse.x && vtMouse.x<ptMid13.x+nBound &&
        ptMid13.y-nBound<vtMouse.y && vtMouse.y<ptMid13.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, ptMid13);
		bRet = RECKER_MID_LEFT;
	}
	if (ptMid24.x-nBound<vtMouse.x && vtMouse.x<ptMid24.x+nBound &&
        ptMid24.y-nBound<vtMouse.y && vtMouse.y<ptMid24.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, ptMid24);
		bRet = RECKER_MID_RIGHT;
	}
	if (ptMid34.x-nBound<vtMouse.x && vtMouse.x<ptMid34.x+nBound &&
        ptMid34.y-nBound<vtMouse.y && vtMouse.y<ptMid34.y+nBound)
	{
		*nDist = (long)m_libMath.GetDistance2D(vtMouse, ptMid34);
		bRet = RECKER_MID_BOT;
	}

	return bRet;
}

BOOL COverlayRectGOM::SetMoveRect(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, POINT2 ptDelta, 
								  POINT2 *ptTarg1, POINT2 *ptTarg2, POINT2 *ptTarg3, POINT2 *ptTarg4)
{
	BOOL bRet = FALSE;
	float dAngle, dScale, fDistW, fDistH; 
	POINT2 ptTemp1, ptTemp2, ptTemp3, ptTemp4, vtMouse;
	POINT2 ptMid12, ptMid13, ptMid24, ptMid34;
	CPoint ptMouse;
	CRect rcRect;
	long nBound, nSelType;

	m_pGOM = &pData->m_GOM[nIndex];
	m_pINSP = &pData->m_INSP[nIndex];

	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);
	nSelType = m_pGOM->m_bSelectedROI;

	vtMouse.x = (rtMouse.x + rcRect.left/dScale) * dScale;
	vtMouse.y = (rtMouse.y + rcRect.top/dScale) * dScale;

	ptDelta.x *= dScale;
	ptDelta.y *= dScale;

	dAngle = m_pGOM->m_dAngle;

	ptTemp1 = m_pGOM->m_ptPoint1;
	ptTemp2 = m_pGOM->m_ptPoint2;
	ptTemp3 = m_pGOM->m_ptPoint3;
	ptTemp4 = m_pGOM->m_ptPoint4;

	ptTarg1->x = ptTemp1.x;
	ptTarg1->y = ptTemp1.y;
	ptTarg2->x = ptTemp2.x;
	ptTarg2->y = ptTemp2.y;
	ptTarg3->x = ptTemp3.x;
	ptTarg3->y = ptTemp3.y;
	ptTarg4->x = ptTemp4.x;
	ptTarg4->y = ptTemp4.y;

	ptMid12.x = (ptTemp1.x+ptTemp2.x)/2.0f;
	ptMid12.y = (ptTemp1.y+ptTemp2.y)/2.0f;
	ptMid13.x = (ptTemp1.x+ptTemp3.x)/2.0f;
	ptMid13.y = (ptTemp1.y+ptTemp3.y)/2.0f;
	ptMid24.x = (ptTemp2.x+ptTemp4.x)/2.0f;
	ptMid24.y = (ptTemp2.y+ptTemp4.y)/2.0f;
	ptMid34.x = (ptTemp3.x+ptTemp4.x)/2.0f;
	ptMid34.y = (ptTemp3.y+ptTemp4.y)/2.0f;

	switch(nSelType)
	{
		case RECKER_MID_TOP :
			bRet = TRUE;
			ptMid12.x += ptDelta.x;
			ptMid12.y += ptDelta.y;

			m_libMath.GetCrossPointDotLine(ptMid12, ptTemp1, ptTemp3, ptTarg1);
			m_libMath.GetCrossPointDotLine(ptMid12, ptTemp2, ptTemp4, ptTarg2);

		    if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
		case RECKER_MID_LEFT :
			bRet = TRUE;
			ptMid13.x += ptDelta.x;
			ptMid13.y += ptDelta.y;

			m_libMath.GetCrossPointDotLine(ptMid13, ptTemp1, ptTemp2, ptTarg1);
			m_libMath.GetCrossPointDotLine(ptMid13, ptTemp3, ptTemp4, ptTarg3);

		    if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
		case RECKER_MID_RIGHT :
			bRet = TRUE;
			ptMid24.x += ptDelta.x;
			ptMid24.y += ptDelta.y;

			m_libMath.GetCrossPointDotLine(ptMid24, ptTemp1, ptTemp2, ptTarg2);
			m_libMath.GetCrossPointDotLine(ptMid24, ptTemp3, ptTemp4, ptTarg4);

		    if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
        case RECKER_MID_BOT :
			bRet = TRUE;
			ptMid34.x += ptDelta.x;
			ptMid34.y += ptDelta.y;

			m_libMath.GetCrossPointDotLine(ptMid34, ptTemp1, ptTemp3, ptTarg3);
			m_libMath.GetCrossPointDotLine(ptMid34, ptTemp2, ptTemp4, ptTarg4);

			if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
        case RECKER_LEFT_TOP :
			bRet = TRUE;
			ptTemp1.x += (float)ptDelta.x;
			ptTemp1.y += (float)ptDelta.y;
			ptTemp2.y += (float)ptDelta.y;
			ptTemp3.x += (float)ptDelta.x;

			m_libMath.GetCrossPointDotWithAngle(dAngle, ptTemp1, ptTemp4, &ptTemp2);
			m_libMath.GetCrossPointDotWithAngle(dAngle+90, ptTemp1, ptTemp4, &ptTemp3);

			if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
        case RECKER_RIGHT_TOP :
			bRet = TRUE;
			ptTemp2.x += (float)ptDelta.x;
			ptTemp2.y += (float)ptDelta.y;
			ptTemp1.y += (float)ptDelta.y;
			ptTemp4.x += (float)ptDelta.x;

			m_libMath.GetCrossPointDotWithAngle(dAngle, ptTemp2, ptTemp3, &ptTemp1);
			m_libMath.GetCrossPointDotWithAngle(dAngle+90, ptTemp2, ptTemp3, &ptTemp4);

			if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
        case RECKER_LEFT_BOT :
			bRet = TRUE;
			ptTemp3.x += (float)ptDelta.x;
			ptTemp3.y += (float)ptDelta.y;
			ptTemp1.x += (float)ptDelta.x;
			ptTemp4.y += (float)ptDelta.y;

			m_libMath.GetCrossPointDotWithAngle(dAngle, ptTemp3, ptTemp2, &ptTemp4);
			m_libMath.GetCrossPointDotWithAngle(dAngle+90, ptTemp3, ptTemp2, &ptTemp1);
			
			if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
        case RECKER_RIGHT_BOT :
			bRet = TRUE;
			ptTemp4.x += (float)ptDelta.x;
			ptTemp4.y += (float)ptDelta.y;
			ptTemp2.x += (float)ptDelta.x;
			ptTemp3.y += (float)ptDelta.y;

			m_libMath.GetCrossPointDotWithAngle(dAngle, ptTemp4, ptTemp1, &ptTemp3);
			m_libMath.GetCrossPointDotWithAngle(dAngle+90, ptTemp4, ptTemp1, &ptTemp2);

			if (m_pINSP->m_nInspectType==INSP_PATTERN_MATCH)
	         	return false;
			break;
		case RECKER_CENTER :
		    bRet = TRUE;
		    ptTemp1.x += ptDelta.x;
			ptTemp1.y += ptDelta.y;
			ptTemp2.x += ptDelta.x;
			ptTemp2.y += ptDelta.y;
			ptTemp3.x += ptDelta.x;
			ptTemp3.y += ptDelta.y;
			ptTemp4.x += ptDelta.x;
			ptTemp4.y += ptDelta.y;
			break;
	}

	if (0<nSelType && nSelType<RECKER_MID_TOP)
	{
		ptTarg1->x = ptTemp1.x;
 		ptTarg1->y = ptTemp1.y;
		ptTarg2->x = ptTemp2.x;
 		ptTarg2->y = ptTemp2.y;

		ptTarg3->x = ptTemp3.x;
 		ptTarg3->y = ptTemp3.y;
		ptTarg4->x = ptTemp4.x;
 		ptTarg4->y = ptTemp4.y;
	}

	fDistW = m_libMath.GetDistance2D(ptTarg1->x, ptTarg1->y, ptTarg2->x, ptTarg2->y);
	fDistH = m_libMath.GetDistance2D(ptTarg1->x, ptTarg1->y, ptTarg3->x, ptTarg3->y);
	if (fDistW<nBound*2 || fDistH<nBound*2)
	{
		ptTarg1->x = m_pGOM->m_ptPoint1.x;
		ptTarg1->y = m_pGOM->m_ptPoint1.y;
		ptTarg2->x = m_pGOM->m_ptPoint2.x;
		ptTarg2->y = m_pGOM->m_ptPoint2.y;
		ptTarg3->x = m_pGOM->m_ptPoint3.x;
		ptTarg3->y = m_pGOM->m_ptPoint3.y;
		ptTarg4->x = m_pGOM->m_ptPoint4.x;
		ptTarg4->y = m_pGOM->m_ptPoint4.y;
	}

	if (dAngle==0.0f)
	{
		ptTarg2->x = ptTarg4->x;
		ptTarg2->y = ptTarg1->y;
		ptTarg3->x = ptTarg1->x;
		ptTarg3->y = ptTarg4->y;
	}

	return bRet;
}
