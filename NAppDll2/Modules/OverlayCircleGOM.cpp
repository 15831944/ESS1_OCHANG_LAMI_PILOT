// OverlayCircleGOM.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "OverlayCircleGOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayCircleGOM
COverlayCircleGOM::COverlayCircleGOM()
{
}

COverlayCircleGOM::~COverlayCircleGOM()
{
}

BEGIN_MESSAGE_MAP(COverlayCircleGOM, CStatic)
	//{{AFX_MSG_MAP(COverlayCircleGOM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayCircleGOM message handlers
void COverlayCircleGOM::DrawOverlayCircle(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData)
{
	CPoint ptMouse, ptRight;
	POINT2 ptCir, vtCir;
	CRect rcRect;
	long nInRadius, nColor;
	float dScale;

	m_pGOM = &pData->m_GOM[nIndex];
	m_pINSP = &pData->m_INSP[nIndex];
	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;
	dScale = 1.0f/pData->m_IMG.m_fZoomScale;

    // 위치보정 
	m_FuncEtc.GetTransPosision(pData->m_dSourX, pData->m_dSourY, pData->m_dDeltaX, pData->m_dDeltaY, pData->m_dDeltaT,
			                   m_pGOM->m_dCX, m_pGOM->m_dCY, &ptCir);
    // 위치보정 

	nInRadius = (long)(m_pGOM->m_dInRadius/dScale);

	vtCir.x = (float)((ptCir.x - rcRect.left)/dScale);
	vtCir.y = (float)((ptCir.y - rcRect.top)/dScale);

	ptRight.x = (long)(((ptCir.x + m_pGOM->m_dInRadius) - rcRect.left)/dScale);
	ptRight.y = (long)((ptCir.y  - rcRect.top)/dScale);

	if (m_pGOM->m_bSelectedROI)
	{
		nColor = m_pGOM->m_nSelectedColor;
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nInRadius, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptRight.x, (long)ptRight.y, 5, m_pGOM->m_nThick, nColor);
    }
	else
	{
		nColor = m_pGOM->m_nNormalColor;
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nInRadius, m_pGOM->m_nThick, nColor);
	}

	if (m_pGOM->m_bShowTitle)
	{
		if (m_pGOM->m_bSelectedROI)
            m_ImageBMP.DrawText(pDC, (long)vtCir.x, (long)vtCir.y, m_pGOM->m_strTitle, 1, nColor);
        else
            m_ImageBMP.DrawText(pDC, (long)vtCir.x, (long)vtCir.y, m_pGOM->m_strTitle, 1, nColor);
	}
}

void COverlayCircleGOM::DrawOverlayCircle2(CDC *pDC, long nIndex, SETUP_MODEL_DATA *pData)
{
	CPoint ptInRight, ptOutRight, ptMouse;
	POINT2 ptCir, vtCir;
	CRect rcRect;
	long nInRadius, nOutRadius, nColor;
	float dScale;

	m_pGOM = &pData->m_GOM[nIndex];
	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;

    // 위치보정 
	m_FuncEtc.GetTransPosision(pData->m_dSourX, pData->m_dSourY, pData->m_dDeltaX, pData->m_dDeltaY, pData->m_dDeltaT,
			                   m_pGOM->m_dCX, m_pGOM->m_dCY, &ptCir);
    // 위치보정 

	vtCir.x = (float)((ptCir.x - rcRect.left)/dScale);
	vtCir.y = (float)((ptCir.y - rcRect.top)/dScale);

	ptInRight.x = (long)(((ptCir.x + m_pGOM->m_dInRadius) - rcRect.left)/dScale);
	ptInRight.y = (long)((ptCir.y - rcRect.top)/dScale);
	ptOutRight.x = (long)(((ptCir.x + m_pGOM->m_dOutRadius) - rcRect.left)/dScale);
	ptOutRight.y = (long)((ptCir.y - rcRect.top)/dScale);

	nInRadius = (long)(m_pGOM->m_dInRadius/dScale);
	nOutRadius = (long)(m_pGOM->m_dOutRadius/dScale);

	if (m_pGOM->m_bSelectedROI)
	{
		nColor = m_pGOM->m_nSelectedColor;
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nInRadius, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nOutRadius, m_pGOM->m_nThick, nColor);

		m_ImageBMP.DrawFillCircle(pDC, (long)ptInRight.x, (long)ptInRight.y, 5, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawFillCircle(pDC, (long)ptOutRight.x, (long)ptOutRight.y, 5, m_pGOM->m_nThick, nColor);
	}
	else
	{
		nColor = m_pGOM->m_nNormalColor;
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nInRadius, m_pGOM->m_nThick, nColor);
		m_ImageBMP.DrawEdgeCircle(pDC, (long)vtCir.x, (long)vtCir.y, (long)nOutRadius, m_pGOM->m_nThick, nColor);
	}

	if (m_pGOM->m_bShowTitle)
	{
		if (m_pGOM->m_bSelectedROI)
            m_ImageBMP.DrawText(pDC, (long)vtCir.x, (long)vtCir.y, m_pGOM->m_strTitle, 1, nColor);
        else
            m_ImageBMP.DrawText(pDC, (long)vtCir.x, (long)vtCir.y, m_pGOM->m_strTitle, 1, nColor);
	}
}

BOOL COverlayCircleGOM::SetSelectedCircle(SETUP_MODEL_DATA *pData, long nIndex, POINT2 ptMouse, long *nDist)
{
	long nTempDist, nBound;
	float dScale;
	BOOL bRet=FALSE;
	POINT2 pt, ptTarg[2];
	m_pGOM = &pData->m_GOM[nIndex];

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);

    ptTarg[0].x = (m_pGOM->m_dCX-m_pGOM->m_dInRadius);
	ptTarg[0].y = m_pGOM->m_dCY;
  
	ptTarg[1].x = (m_pGOM->m_dCX+m_pGOM->m_dInRadius);
	ptTarg[1].y = m_pGOM->m_dCY;

	pt.x = m_pGOM->m_dCX;
	pt.y = m_pGOM->m_dCY;

	m_pGOM->m_bSelectedROI = FALSE;
    nTempDist = (long)m_libMath.GetDistance2D(pt, ptMouse);
	if (nTempDist<m_pGOM->m_dInRadius+nBound)
	{
		*nDist = GetMousePosMinDist(2, ptMouse, &ptTarg[0]);
		bRet = TRUE;
	}

	return bRet;
}

BOOL COverlayCircleGOM::SetSelectedCircle2(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, long *nDist)
{
	long nTempDist, nBound;
	BOOL bRet=FALSE;
	CPoint ptMouse;
	CRect rcRect;
	POINT2 pt, vtMouse, ptTarg[4];
	float dScale;
	m_pGOM = &pData->m_GOM[nIndex];

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);

	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	vtMouse.x = (rtMouse.x + rcRect.left/dScale) * dScale;
	vtMouse.y = (rtMouse.y + rcRect.top/dScale) * dScale;

    ptTarg[0].x = m_pGOM->m_dCX - m_pGOM->m_dInRadius;
	ptTarg[0].y = m_pGOM->m_dCY;
  
	ptTarg[1].x = m_pGOM->m_dCX + m_pGOM->m_dInRadius;
	ptTarg[1].y = m_pGOM->m_dCY;

    ptTarg[2].x = m_pGOM->m_dCX - m_pGOM->m_dOutRadius;
	ptTarg[2].y = m_pGOM->m_dCY;
  
	ptTarg[3].x = m_pGOM->m_dCX + m_pGOM->m_dOutRadius;
	ptTarg[3].y = m_pGOM->m_dCY;

	pt.x = m_pGOM->m_dCX;
	pt.y = m_pGOM->m_dCY;

	m_pGOM->m_bSelectedROI = FALSE;
    nTempDist = (long)m_libMath.GetDistance2D(pt, vtMouse);
	if (nTempDist<m_pGOM->m_dOutRadius+nBound)
	{
		*nDist = GetMousePosMinDist(4, vtMouse, &ptTarg[0]);
		bRet = TRUE;
	}

	return bRet;
}

BOOL COverlayCircleGOM::SetMoveCircle(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, POINT2 ptDelta, POINT2 *ptTarg, float *dInRadius)
{
	BOOL bRet = FALSE;
	long nTempDist, nBound;
	POINT2 ptMid, ptInRight, vtMouse;
	CPoint ptMouse;
	CRect rcRect;
	float dScale;
	m_pGOM = &pData->m_GOM[nIndex];

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);

	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	vtMouse.x = (rtMouse.x + rcRect.left/dScale) * dScale;
	vtMouse.y = (rtMouse.y + rcRect.top/dScale) * dScale;

	ptMid.x = m_pGOM->m_dCX;
	ptMid.y = m_pGOM->m_dCY;

	ptInRight.x = m_pGOM->m_dCX + m_pGOM->m_dInRadius;
	ptInRight.y = m_pGOM->m_dCY;

	*dInRadius = m_pGOM->m_dInRadius;
    nTempDist = (long)m_libMath.GetDistance2D(ptInRight, vtMouse);
	if (nTempDist<nBound)
	{
		ptInRight.x += ptDelta.x;
		ptInRight.y += ptDelta.y;
        *dInRadius = (float)m_libMath.GetDistance2D(ptMid, ptInRight);
	}

    nTempDist = (long)m_libMath.GetDistance2D(ptMid, vtMouse);
	if (nTempDist<m_pGOM->m_dInRadius-nBound)
	{
		ptMid.x += ptDelta.x;
		ptMid.y += ptDelta.y;
	}

	if (*dInRadius>nBound*2)
	{
        *ptTarg = ptMid;
		bRet = TRUE;
	}

	return bRet;
}

BOOL COverlayCircleGOM::SetMoveCircle2(SETUP_MODEL_DATA *pData, long nIndex, POINT2 rtMouse, POINT2 ptDelta, POINT2 *ptTarg, float *dInRadius, float *dOutRadius)
{
	BOOL bRet = FALSE;
	long nTempDist, nBound;
	float dScale;
	CRect rcRect;
	CPoint ptMouse;
	POINT2 vtMouse, ptMid, ptInRight, ptOutRight;
	m_pGOM = &pData->m_GOM[nIndex];

	dScale = 1.0f/pData->m_IMG.m_fZoomScale;
	nBound = (long)(7*dScale);

	ptMouse = pData->m_IMG.m_ptMouse;
	rcRect = pData->m_IMG.m_fmRect;

	vtMouse.x = (rtMouse.x + rcRect.left/dScale) * dScale;
	vtMouse.y = (rtMouse.y + rcRect.top/dScale) * dScale;
    ptDelta.x *= dScale;
    ptDelta.y *= dScale;

	ptMid.x = m_pGOM->m_dCX;
	ptMid.y = m_pGOM->m_dCY;

	ptInRight.x = m_pGOM->m_dCX + m_pGOM->m_dInRadius;
	ptInRight.y = m_pGOM->m_dCY;
	ptOutRight.x = m_pGOM->m_dCX + m_pGOM->m_dOutRadius;
	ptOutRight.y = m_pGOM->m_dCY;

	*dInRadius = m_pGOM->m_dInRadius;
	*dOutRadius = m_pGOM->m_dOutRadius;

    nTempDist = (long)m_libMath.GetDistance2D(ptInRight, vtMouse);
	if (nTempDist<nBound)
	{
		ptInRight.x += ptDelta.x;
		ptInRight.y += ptDelta.y;
        *dInRadius = (float)m_libMath.GetDistance2D(ptMid, ptInRight);
	}
    nTempDist = (long)m_libMath.GetDistance2D(ptOutRight, vtMouse);
	if (nTempDist<nBound)
	{
		ptOutRight.x += ptDelta.x;
		ptOutRight.y += ptDelta.y;
        *dOutRadius = (float)m_libMath.GetDistance2D(ptMid, ptOutRight);
	}

    nTempDist = (long)m_libMath.GetDistance2D(ptMid, vtMouse);
	if (nTempDist<m_pGOM->m_dInRadius-nBound)
	{
		ptMid.x += ptDelta.x;
		ptMid.y += ptDelta.y;
	}

	if (*dInRadius>nBound*2 && (*dOutRadius-*dInRadius)>nBound*4)
	{
        *ptTarg = ptMid;
		bRet = TRUE;
	}

	return bRet;
}

long COverlayCircleGOM::GetMousePosMinDist(long nNum, POINT2 pt0, POINT2 *pt)
{
	long i;
    float a[5], b[5], dMin, dTemp;

	for(i=0; i<nNum; i++)
	{
		a[i] = (pt[i].x - pt0.x);
		b[i] = (pt[i].y - pt0.y);
    }
 
	dMin = 100000.0f;
	for(i=0; i<nNum; i++)
	{
	    dTemp = (float)sqrt(a[i]*a[i] + b[i]*b[i]);
		if (dMin > dTemp) dMin = dTemp;
	}

	return (long)dMin;
}
