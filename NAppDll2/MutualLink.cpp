// MutualLink.cpp : implementation file
//

#include "stdafx.h"
#include "MutualLink.h"
#include "math.h"
#include "NVisionDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMutualLink
CMutualLink::CMutualLink()
{
	m_bCompens = FALSE;
}

CMutualLink::~CMutualLink()
{

}

void CMutualLink::SetImageRawData(LPBYTE fmImage, long nWidth, long nHeight)
{
    m_pRawImage  = fmImage;
	m_nImageWidth = nWidth;
	m_nImageHeight= nHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LINK 항목 초기화       //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void CMutualLink::InitializeMutualData()
{
	long i;

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
		m_pData->m_Teach.m_LINK[i].m_dCX = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dCY = 0.0f;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LINK 항목 초기화       //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////        LINK 항목 검사        //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CMutualLink::InspectMutualLinkObjects()
{
	long i, nCnt, nType;
	BOOL bRet = FALSE;

	nCnt = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nCnt; i++)
	{
		nType = m_pData->m_Teach.m_LINK[i].m_nLinkType;

		if (nType == TEACH_DIST_LINE2LINE)
			bRet = InspectLine2LineDist(i);
		if (nType == TEACH_DIST_1STLINE2LINE)
			bRet = Inspect1STLINE2LineDist(i);
		if (nType == TEACH_DIST_2STLINE2LINE)
			bRet = Inspect2STLINE2LineDist(i);
		if (nType == TEACH_NADIST_LINE2LINE)
			bRet = InspectLine2LineNADist(i);
	}

	return bRet;
}

BOOL CMutualLink::InspectPoint2LineDist(long nIndex)
{
	BOOL bRet = FALSE;
	float a, b, dValue;
	float fLowL, fHighL, dScale;
	long nCnt, bRear, POS[3];
	POINT2 pt1, pt2;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;

	dValue = 999.0f;
	a = 0.0f;
	b = 0.0f;
	pt1.x = 0;
	pt1.y = 0;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_PATTERN_MATCH)
		{
			pt1.x = m_pData->m_INSP[POS[nCnt]].m_dCX;
			pt1.y = m_pData->m_INSP[POS[nCnt]].m_dCY;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			a = m_pData->m_Scan[POS[nCnt]].m_dSecondA;
			b = m_pData->m_Scan[POS[nCnt]].m_dSecondB;
		}

		nCnt++;
	}

	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_PATTERN_MATCH)
		{
			pt1.x = m_pData->m_INSP[POS[nCnt]].m_dCX;
			pt1.x = m_pData->m_INSP[POS[nCnt]].m_dCY;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			a = m_pData->m_Scan[POS[nCnt]].m_dSecondA;
			b = m_pData->m_Scan[POS[nCnt]].m_dSecondB;
		}

		nCnt++;
	}

	if (bRear) dScale = m_pCAL->m_dScaleR_Y;
	else  dScale = m_pCAL->m_dScaleL_Y;

	if (nCnt>=2 && (a!=0 || b!=0) && pt1.x>0 && pt1.y>0)
	{
		m_libMath.GetCrossPointLineEquWithDot(a, b, pt1, &pt2);
		dValue = (float)m_libMath.GetDistance2D(pt1, pt2);
		dValue *= dScale;
	}
	
	m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;

	bRet = FALSE;
	m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
	m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

	if (dValue<fLowL || fHighL<dValue)
	{
		bRet = TRUE;
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}

// 2Point 검출과 1Point검출의 조합에서 2Point에서 첫번째 감지와 연결
BOOL CMutualLink::Inspect1STLINE2LineDist(long nIndex)
{
	BOOL bRet = FALSE;
	float a, b, w, fCenV, dValue;
	float fLowL, fHighL, dOffsetV, dScale, dStdSpec, fTransV, fDiff;
	long nDir, nMid, nH, nCnt, bRear, POS[3];
//	POINT2 pt1, pt2;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	dValue = 999.0f;
	fTransV = 0.0f;
	a = 0.0f;
	b = 0.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[nCnt]].m_ptPoint1.y + m_pData->m_GOM[POS[nCnt]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nH = IMAGE_HEIGHT/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_TWO)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
				if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
				if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
				if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
				if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.x;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.y;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}

			fCenV = a;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
					if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
					if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
					if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
					if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
					
					w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;			
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}
		}

		nCnt++;
	}

	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[nCnt]].m_ptPoint1.y + m_pData->m_GOM[POS[nCnt]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nH = IMAGE_HEIGHT/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_TWO)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
				if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
				if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
				if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
				if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.x;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.y;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}

			fCenV = a;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
					if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
					if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
					if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
					if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
					
					w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;			
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}
		}

		nCnt++;
	}

//	if (bRear) dScale = m_pCAL->m_dScaleR_Y;
//	else  dScale = m_pCAL->m_dScaleL_Y;

	if (nCnt>=2 && (a!=0 || b!=0 || w!=0))
	{
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;
		dValue = fabs(fCenV - w); 
		dValue *= dScale;
		dValue += dOffsetV;
	}


	fDiff = (float)fabs(dStdSpec - dValue);

	if (fDiff>0.03 && dValue>0.06)
	{
		if (dValue<fLowL)  fTransV = dValue + 0.03f;
		if (dValue>fHighL) fTransV = dValue - 0.03f;
	}
	if (fDiff>0.1 && dValue>0.1)
	{
		if (dValue<fLowL)  fTransV = dValue + 0.06f;
		if (dValue>fHighL) fTransV = dValue - 0.06f;
	}  

	if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
	if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;

	if (dValue<0.0f) dValue=0.0f;
	if (fTransV<0.0f) fTransV=0.0f;

	m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;

	m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
	m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

	if (fLowL<=dValue && dValue<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	else if (fLowL<=fTransV && fTransV<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = fTransV;
	else
	{
		bRet = TRUE;
		if(m_pData->m_INSP[POS[0]].m_nInspectType == INSP_MEASURE_SIDE_ONE)  
			m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		if(m_pData->m_INSP[POS[1]].m_nInspectType == INSP_MEASURE_SIDE_ONE)  
			m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}

// 2Point 검출과 1Point검출의 조합에서 2Point에서 두번째 감지와 연결
BOOL CMutualLink::Inspect2STLINE2LineDist(long nIndex)
{
	BOOL bRet = FALSE;
	float a, b, w, fCenV, dValue;
	float fLowL, fHighL, dOffsetV, dScale, dStdSpec, fTransV, fDiff;
	long nDir, nMid, nH, nCnt, bRear, POS[3];
	//POINT2 pt1, pt2;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	dValue = 999.0f;
	fTransV = 0.0f;
	a = 0.0f;
	b = 0.0f;
//	pt1.x = 0;
//	pt1.y = 0;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[nCnt]].m_ptPoint1.y + m_pData->m_GOM[POS[nCnt]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nH = IMAGE_HEIGHT/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_TWO)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
				if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
				if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
				if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
				if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.x;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.y;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}

			fCenV = b;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
					if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
					if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
					if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
					if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
					
					w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;			
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}
		}

		nCnt++;
	}

	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		bRear = m_pData->m_INSP[POS[nCnt]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[nCnt]].m_ptPoint1.y + m_pData->m_GOM[POS[nCnt]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nH = IMAGE_HEIGHT/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_TWO)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
				if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
				if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
				if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
				if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.x;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				a = m_pData->m_INSP[POS[nCnt]].m_ptFirstMeas1.y;
				b = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}

			fCenV = b;
		}

		if (m_pData->m_INSP[POS[nCnt]].m_nInspectType == INSP_MEASURE_SIDE_ONE)
		{
			if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
			{
					if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
					if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
					if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
					if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
					
					w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.x;			
			}
			else {
				if (bRear) dScale = m_pCAL->m_dScaleR_Y;
				else  dScale = m_pCAL->m_dScaleL_Y;
				w = m_pData->m_INSP[POS[nCnt]].m_ptSecondMeas1.y;
			}
		}

		nCnt++;
	}

//	if (bRear) dScale = m_pCAL->m_dScaleR_Y;
//	else  dScale = m_pCAL->m_dScaleL_Y;

	if (nCnt>=2 && (a!=0 || b!=0 || w!=0))
	{
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;
		dValue = fabs(fCenV - w); 
		dValue *= dScale;
		dValue += dOffsetV;
	}

	fDiff = (float)fabs(dStdSpec - dValue);

	if (fDiff>0.03 && dValue>0.06)
	{
		if (dValue<fLowL)  fTransV = dValue + 0.03f;
		if (dValue>fHighL) fTransV = dValue - 0.03f;
	}
	if (fDiff>0.1 && dValue>0.1)
	{
		if (dValue<fLowL)  fTransV = dValue + 0.06f;
		if (dValue>fHighL) fTransV = dValue - 0.06f;
	}

	if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
	if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;

	if (dValue<0.0f) dValue=0.0f;
	if (fTransV<0.0f) fTransV=0.0f;

	m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
	m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

	if (fLowL<=dValue && dValue<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	else if (fLowL<=fTransV && fTransV<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = fTransV;
	else
	{
		bRet = TRUE;
		if(m_pData->m_INSP[POS[0]].m_nInspectType == INSP_MEASURE_SIDE_ONE)  
			m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		if(m_pData->m_INSP[POS[1]].m_nInspectType == INSP_MEASURE_SIDE_ONE)  
			m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}

#if 0
BOOL CMutualLink::InspectLine2LineAngle(long nIndex)
{
	BOOL bFlag, bRet = FALSE;
	float dValue;
	float fLowL, fHighL, d1Angle, d2Angle;
	float dA1, dB1, dA2, dB2;
	long nCnt, POS[3], DIR[3];
	POINT2 pt1, pt2, pt3, pt4;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;

	dValue = 999.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}

	bFlag = FALSE;
	if ((DIR[0]==DIR_LEFT_RIGHT || DIR[0]==DIR_RIGHT_LEFT) && 
		(DIR[1]==DIR_LEFT_RIGHT || DIR[1]==DIR_RIGHT_LEFT)) bFlag = TRUE;
	if ((DIR[0]==DIR_TOP_BOTTOM || DIR[0]==DIR_BOTTOM_TOP) && 
		(DIR[1]==DIR_TOP_BOTTOM || DIR[1]==DIR_BOTTOM_TOP)) bFlag = TRUE;

	if (nCnt>=2)
	{
		dA1 = m_pData->m_Scan[POS[0]].m_dSecondA;
		dB1 = m_pData->m_Scan[POS[0]].m_dSecondB;

		dA2 = m_pData->m_Scan[POS[1]].m_dSecondA;
		dB2 = m_pData->m_Scan[POS[1]].m_dSecondB;

		if (DIR[0] == DIR_LEFT_RIGHT || DIR[0] == DIR_RIGHT_LEFT)
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.x = dA1*ptInsp1.y + dB1;
			pt1.y = ptInsp1.y;
			pt2.x = dA1*ptInsp3.y + dB1;
			pt2.y = ptInsp3.y;
		}
		else
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.y = dA1*ptInsp1.x + dB1;
			pt1.x = ptInsp1.x;
			pt2.y = dA1*ptInsp2.x + dB1;
			pt2.x = ptInsp2.x;
		}

		if (DIR[1] == DIR_LEFT_RIGHT || DIR[1] == DIR_RIGHT_LEFT)
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.x = dA2*ptInsp1.y + dB2;
			pt3.y = ptInsp1.y;
			pt4.x = dA2*ptInsp3.y + dB2;
			pt4.y = ptInsp3.y;
		}
		else
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.y = dA2*ptInsp1.x + dB2;
			pt3.x = ptInsp1.x;
			pt4.y = dA2*ptInsp2.x + dB2;
			pt4.x = ptInsp2.x;
		}

		if ((dA1!=0.0f || dB1!=0.0f) && (dA2!=0.0f || dB2!=0.0f))
		{
			if (bFlag)
			{
				m_libVis.GetAngle(pt1.x, pt1.y, pt2.x, pt2.y, &d1Angle);
				m_libVis.GetAngle(pt3.x, pt3.y, pt4.x, pt4.y, &d2Angle);

				dValue = d1Angle-d2Angle;
			}
			else
				dValue = m_libMath.GetAngleWith2Line(pt1, pt2, pt3, pt4);

		}

		m_pData->m_INSP[POS[0]].m_dAnyValue = dValue;
		m_pData->m_INSP[POS[1]].m_dAnyValue = dValue;
	}


	m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;

	bRet = FALSE;
	m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
	m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

	if (dValue<fLowL || fHighL<dValue)
	{
		bRet = TRUE;
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}  
#endif // 0


BOOL CMutualLink::InspectLine2LineDist(long nIndex)
{
	BOOL bRear, bFlag, bRet = FALSE;
	float dValue, dScale, dNoTiltValue;
	float fDiff, dOffsetV, dStdSpec, fTransV;
	float fLowL, fHighL;
	float dA1, dB1, dA2, dB2;
	long  nLocPos, nDir, nMid, nCnt, nH, POS[3], DIR[3];
	POINT2 pt1, pt2, pt3, pt4;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CString str;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	dValue = 0.0f;
	dNoTiltValue = 0.0f;
	fTransV = 0.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}

	bFlag = FALSE;
	if ((DIR[0]==DIR_LEFT_RIGHT || DIR[0]==DIR_RIGHT_LEFT) && 
		(DIR[1]==DIR_LEFT_RIGHT || DIR[1]==DIR_RIGHT_LEFT)) bFlag = TRUE;
	if ((DIR[0]==DIR_TOP_BOTTOM || DIR[0]==DIR_BOTTOM_TOP) && 
		(DIR[1]==DIR_TOP_BOTTOM || DIR[1]==DIR_BOTTOM_TOP)) bFlag = TRUE;

	if (nCnt>=2 && bFlag)
	{
		bRear = m_pData->m_INSP[POS[0]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[0]].m_ptPoint1.y + m_pData->m_GOM[POS[0]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[0]].m_nSecondScanDirType;
		nH = m_pData->m_nImageHeight/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		dA1 = m_pData->m_Scan[POS[0]].m_dSecondA;
		dB1 = m_pData->m_Scan[POS[0]].m_dSecondB;

		dA2 = m_pData->m_Scan[POS[1]].m_dSecondA;
		dB2 = m_pData->m_Scan[POS[1]].m_dSecondB;
		
		if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
		{
			if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
			if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
			if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
			if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.x = dA1*ptInsp1.y + dB1;
			pt1.y = ptInsp1.y;
			pt2.x = dA1*ptInsp3.y + dB1;
			pt2.y = ptInsp3.y;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.x = dA2*ptInsp1.y + dB2;
			pt3.y = ptInsp1.y;
			pt4.x = dA2*ptInsp3.y + dB2;
			pt4.y = ptInsp3.y;

			dNoTiltValue = (fabs(pt1.x-pt3.x)+fabs(pt2.x-pt4.x))/2.0f;
		}
		else
		{
			if (bRear) dScale = m_pCAL->m_dScaleR_Y;
			else  dScale = m_pCAL->m_dScaleL_Y;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.y = dA1*ptInsp1.x + dB1;
			pt1.x = ptInsp1.x;
			pt2.y = dA1*ptInsp2.x + dB1;
			pt2.x = ptInsp2.x;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.y = dA2*ptInsp1.x + dB2;
			pt3.x = ptInsp1.x;
			pt4.y = dA2*ptInsp2.x + dB2;
			pt4.x = ptInsp2.x;

			dNoTiltValue = (fabs(pt1.y-pt3.y)+fabs(pt2.y-pt4.y))/2.0f;
		}

		str.Format(_T("%s"), m_pData->m_Teach.m_LINK[nIndex].m_strTitle);
	    str = WithoutSpaceToString(str);

		m_pData->m_INSP[POS[0]].m_ptSecondMeas1 = pt1;
		m_pData->m_INSP[POS[0]].m_ptSecondMeas2 = pt2;
		m_pData->m_INSP[POS[1]].m_ptSecondMeas1 = pt3;
		m_pData->m_INSP[POS[1]].m_ptSecondMeas2 = pt4;
		
		/*
		if (str.Find(_T("TS"))<0)
		{
			m_libMath.GetCrossPointDotLine(pt1, pt3, pt4, &ptCross1);
			m_libMath.GetCrossPointDotLine(pt2, pt3, pt4, &ptCross2);
			m_libMath.GetCrossPointDotLine(pt3, pt1, pt2, &ptCross3);
			m_libMath.GetCrossPointDotLine(pt4, pt1, pt2, &ptCross4);

			a = (float)m_libMath.GetDistance2D(pt1, ptCross1);
			b = (float)m_libMath.GetDistance2D(pt2, ptCross2);
			c = (float)m_libMath.GetDistance2D(pt3, ptCross3);
			d = (float)m_libMath.GetDistance2D(pt4, ptCross4);

			dValue = (a+b+c+d) / 4.0f;
			if ((dA1==0.0f && dB1==0.0f) || (dA2==0.0f && dB2==0.0f))
				dValue = 0.0f;
		}
		else
		{
	    	dValue = dNoTiltValue;
		}
		*/

//#if !DEF_TILTING_ALG
	   	dValue = dNoTiltValue;
//#endif

		dValue = dValue * dScale;	
		dValue += dOffsetV;

		nLocPos = m_pData->m_INSP[POS[0]].m_bLocROI;
		fDiff = (float)fabs(dStdSpec - dValue);

		if (fDiff>0.03 && dValue>0.06)
		{
			if (dValue<fLowL)  fTransV = dValue + 0.03f;
			if (dValue>fHighL) fTransV = dValue - 0.03f;
		}
		if (fDiff>0.1 && dValue>0.1)
		{
			if (dValue<fLowL)  fTransV = dValue + 0.06f;
			if (dValue>fHighL) fTransV = dValue - 0.06f;
		}

		if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
		if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;
		
		m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
		m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

		if (dValue<0.0f) dValue=0.0f;
		if (fTransV<0.0f) fTransV=0.0f;

		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	}

	if (fLowL<=dValue && dValue<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	else if (fLowL<=fTransV && fTransV<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = fTransV;
	else
	{
		bRet = TRUE;
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}

BOOL CMutualLink::InspectLine2LineNADist(long nIndex)
{
	BOOL bRear, bFlag, bRet = FALSE;
	float dValue, dScale, dNoTiltValue;
	float fDiff, dOffsetV, dStdSpec, fTransV;
	float fLowL, fHighL;
	float dA1, dB1, dA2, dB2;
	long  nLocPos, nDir, nMid, nCnt, nH, POS[3], DIR[3];
	POINT2 pt1, pt2, pt3, pt4;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CString str;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	dValue = 0.0f;
	dNoTiltValue = 0.0f;
	fTransV = 0.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}

	bFlag = FALSE;
	if ((DIR[0]==DIR_LEFT_RIGHT || DIR[0]==DIR_RIGHT_LEFT) && 
		(DIR[1]==DIR_LEFT_RIGHT || DIR[1]==DIR_RIGHT_LEFT)) bFlag = TRUE;
	if ((DIR[0]==DIR_TOP_BOTTOM || DIR[0]==DIR_BOTTOM_TOP) && 
		(DIR[1]==DIR_TOP_BOTTOM || DIR[1]==DIR_BOTTOM_TOP)) bFlag = TRUE;

	if (nCnt>=2 && bFlag)
	{
		bRear = m_pData->m_INSP[POS[0]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[0]].m_ptPoint1.y + m_pData->m_GOM[POS[0]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[0]].m_nSecondScanDirType;
		nH = m_pData->m_nImageHeight/2;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;

		dA1 = m_pData->m_Scan[POS[0]].m_dSecondA;
		dB1 = m_pData->m_Scan[POS[0]].m_dSecondB;

		dA2 = m_pData->m_Scan[POS[1]].m_dSecondA;
		dB2 = m_pData->m_Scan[POS[1]].m_dSecondB;
		
		if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
		{
			if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
			if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
			if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
			if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.x = dA1*ptInsp1.y + dB1;
			pt1.y = ptInsp1.y;
			pt2.x = dA1*ptInsp3.y + dB1;
			pt2.y = ptInsp3.y;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.x = dA2*ptInsp1.y + dB2;
			pt3.y = ptInsp1.y;
			pt4.x = dA2*ptInsp3.y + dB2;
			pt4.y = ptInsp3.y;

			dNoTiltValue = ((pt1.x-pt3.x)+(pt2.x-pt4.x))/2.0f;
		}
		else
		{
			if (bRear) dScale = m_pCAL->m_dScaleR_Y;
			else  dScale = m_pCAL->m_dScaleL_Y;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt1.y = dA1*ptInsp1.x + dB1;
			pt1.x = ptInsp1.x;
			pt2.y = dA1*ptInsp2.x + dB1;
			pt2.x = ptInsp2.x;

			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
			pt3.y = dA2*ptInsp1.x + dB2;
			pt3.x = ptInsp1.x;
			pt4.y = dA2*ptInsp2.x + dB2;
			pt4.x = ptInsp2.x;

			dNoTiltValue = ((pt1.y-pt3.y)+(pt2.y-pt4.y))/2.0f;
		}

		str.Format(_T("%s"), m_pData->m_Teach.m_LINK[nIndex].m_strTitle);
	    str = WithoutSpaceToString(str);

		m_pData->m_INSP[POS[0]].m_ptSecondMeas1 = pt1;
		m_pData->m_INSP[POS[0]].m_ptSecondMeas2 = pt2;
		m_pData->m_INSP[POS[1]].m_ptSecondMeas1 = pt3;
		m_pData->m_INSP[POS[1]].m_ptSecondMeas2 = pt4;

	   	dValue = dNoTiltValue;

		dValue = dValue * dScale;	
		dValue += dOffsetV;

#if 0
		nLocPos = m_pData->m_INSP[POS[0]].m_bLocROI;
		fDiff = (float)fabs(dStdSpec - dValue);

		if (fDiff>0.03 && dValue>0.06)
		{
			if (dValue<fLowL)  fTransV = dValue + 0.03f;
			if (dValue>fHighL) fTransV = dValue - 0.03f;
		}
		if (fDiff>0.1 && dValue>0.1)
		{
			if (dValue<fLowL)  fTransV = dValue + 0.06f;
			if (dValue>fHighL) fTransV = dValue - 0.06f;
		}

		if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
		if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;  
#endif // 0

		
		m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
		m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

#if 0
		if (dValue<0.0f) dValue=0.0f;
		if (fTransV<0.0f) fTransV=0.0f;  
#endif // 0


		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	}

	if (fLowL<=dValue && dValue<=fHighL)
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
	else
	{
		bRet = TRUE;
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;

}

#if 0
BOOL CMutualLink::InspectDiffGap2Gap(long nIndex)
{
	BOOL bRet = TRUE;
	float dValue;
	float fLowL, fHighL;
	long  nCnt, POS[3];
	CString str, strTitle1, strTitle2;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	dValue = 0.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		nCnt++;
	}
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		nCnt++;
	}

	dValue = 0.0f;
	if (nCnt>=2)
	{
		dValue = m_pData->m_INSP[POS[0]].m_dAnyValue - m_pData->m_INSP[POS[1]].m_dAnyValue;
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = dValue;
		bRet = FALSE;
	}

	if (dValue<fLowL || fHighL<dValue)
	{
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
		bRet = TRUE;
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LINK 항목 검사        ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////     LINK SLINE MIN-MAX      ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CMutualLink::InspectSLineSurface(long nIndex)
{
	BOOL  bRear, bCalcF, bFlag, bDecF, bRet = FALSE;
	float dValue, dScale;
	float dOffsetV, fTransV, fMaxV;
	float fDiffMaxV, fMidV, fLowL, fHighL;
	float dDist, fMidL;
	long i, bDatumL1, bDatumL2, nType1, nType2;
	long  nInxMidP, nPos, nDir, nMid, nCnt, nNum, nH, POS[3], DIR[3];
	POINT2 pt1, pt2, pt3;
	POINT2 ptCross1;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CString str, strTitle1, strTitle2;

	fLowL = m_pData->m_Teach.m_LINK[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_Teach.m_LINK[nIndex].m_dStandHighLimit;
	fMidL = (fLowL+fHighL)/2;

	bDecF = FALSE;
	bCalcF = FALSE;
	dValue = 0.0f;
	fTransV = 0.0f;

	nCnt=0;
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel1) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex1;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}
	if (m_pData->m_Teach.m_LINK[nIndex].m_bSel2) 
	{
		POS[nCnt] = m_pData->m_Teach.m_LINK[nIndex].m_nIndex2;
		DIR[nCnt] = m_pData->m_Scan[POS[nCnt]].m_nSecondScanDirType;
		nCnt++;
	}

	bFlag = FALSE;
	if ((DIR[0]==DIR_LEFT_RIGHT || DIR[0]==DIR_RIGHT_LEFT) && 
		(DIR[1]==DIR_LEFT_RIGHT || DIR[1]==DIR_RIGHT_LEFT)) bFlag = TRUE;
	if ((DIR[0]==DIR_TOP_BOTTOM || DIR[0]==DIR_BOTTOM_TOP) && 
		(DIR[1]==DIR_TOP_BOTTOM || DIR[1]==DIR_BOTTOM_TOP)) bFlag = TRUE;

	if (nCnt>=2 && bFlag)
	{
		bRear = m_pData->m_INSP[POS[0]].m_bLocROI;
		nMid = (long)(m_pData->m_GOM[POS[0]].m_ptPoint1.y + m_pData->m_GOM[POS[0]].m_ptPoint4.y)/2;
		nDir = m_pData->m_Scan[POS[0]].m_nSecondScanDirType;
		nType1 = m_pData->m_INSP[POS[0]].m_nInspectType;
		nType2 = m_pData->m_INSP[POS[1]].m_nInspectType;
		dOffsetV = m_pData->m_Teach.m_LINK[nIndex].m_dOffsetV;
		nH = m_nImageHeight/2;

		nType1 = m_pData->m_INSP[POS[0]].m_nInspectType;
		nType2 = m_pData->m_INSP[POS[1]].m_nInspectType;

		bDatumL1 = FALSE;
		if (nType1==INSP_DATUM_B || nType1==INSP_DATUM_C) 
			bDatumL1 = TRUE;

		bDatumL2 = FALSE;
		if (nType2==INSP_DATUM_B || nType2==INSP_DATUM_C) 
			bDatumL2 = TRUE;

		if (bDatumL1 || (!bDatumL1 && !bDatumL2)) 
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[1], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

			pt2 = m_pData->m_INSP[POS[0]].m_ptSecondMeas1;
			pt3 = m_pData->m_INSP[POS[0]].m_ptSecondMeas2;

			nPos = POS[1];
			nNum = m_pData->m_Scan[nPos].m_nSecondNum;
		}
		else
		{
			m_FuncEtc.GetOffsetAdjustment(m_pData, POS[0], &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

			pt2 = m_pData->m_INSP[POS[1]].m_ptSecondMeas1;
			pt3 = m_pData->m_INSP[POS[1]].m_ptSecondMeas2;

			nPos = POS[0];
			nNum = m_pData->m_Scan[nPos].m_nSecondNum;
		}

		fMidV = 0.0f;
		fDiffMaxV = 0;
		fMaxV = 0.0f;

		nInxMidP = -1;
		if (nNum>0)
			nInxMidP = nNum/2;

		if (nDir == DIR_LEFT_RIGHT || nDir == DIR_RIGHT_LEFT)
		{
			if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
			if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
			if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
			if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

			for(i=0; i<nNum; i++)
			{
				pt1.x = m_pData->m_Scan[nPos].m_dSecondPX[i];
				pt1.y = m_pData->m_Scan[nPos].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(pt1, pt2, pt3, &ptCross1);

				dDist = (float)m_libMath.GetDistance2D(pt1, ptCross1);
				//				dDist = (dDist-dSA) * dScale;
				dDist *= dScale;
				dDist += dOffsetV;

				if (dDist<fLowL)  dDist += DEF_TURNING_VALUE;
				if (dDist>fHighL) dDist -= DEF_TURNING_VALUE;

				if (fabs(dDist-fMidL)>fabs(fDiffMaxV))
				{
					fDiffMaxV = dDist-fMidL;
					fMaxV = dDist;
				}

				if (i==nInxMidP)
					fMidV = dDist;

				m_pData->m_Scan[nPos].m_bSecondDec[i] = false;
				if (dDist<fLowL || fHighL<dDist) 
				{
					bDecF = TRUE;
					m_pData->m_Scan[nPos].m_bSecondDec[i] = true;
				}

				bCalcF = TRUE;
			}
		}
		else
		{
			if (bRear) dScale = m_pCAL->m_dScaleR_Y;
			else  dScale = m_pCAL->m_dScaleL_Y;

			for(i=0; i<nNum; i++)
			{
				pt1.x = m_pData->m_Scan[nPos].m_dSecondPX[i];
				pt1.y = m_pData->m_Scan[nPos].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(pt1, pt2, pt3, &ptCross1);

				dDist = (float)m_libMath.GetDistance2D(pt1, ptCross1);
				//              dDist = (dDist-dSA) * dScale;
				dDist *= dScale;
				dDist += dOffsetV;

				if (dDist<fLowL)  dDist += DEF_TURNING_VALUE;
				if (dDist>fHighL) dDist -= DEF_TURNING_VALUE;

				if (fabs(dDist-fMidL)>fabs(fDiffMaxV))
				{
					fDiffMaxV = dDist-fMidL;
					fMaxV = dDist;
				}

				if (i==nInxMidP)
					fMidV = dDist;

				m_pData->m_Scan[nPos].m_bSecondDec[i] = false;
				if (dDist<fLowL || fHighL<dDist) 
				{
					bDecF = TRUE;
					m_pData->m_Scan[nPos].m_bSecondDec[i] = true;
				}

				bCalcF = TRUE;
			}
		}
	}

	bRet = FALSE;
	m_pData->m_INSP[POS[0]].m_bDecision = FALSE;
	m_pData->m_INSP[POS[1]].m_bDecision = FALSE;

	if (!bCalcF)
	{
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = 0;
		m_pData->m_Teach.m_LINK[nIndex].m_dValueS = 0;

		bRet = TRUE;
		m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
		m_pData->m_INSP[POS[1]].m_bDecision = TRUE;
	}
	else
	{
		m_pData->m_Teach.m_LINK[nIndex].m_dValue = fMidV;
		m_pData->m_Teach.m_LINK[nIndex].m_dValueS = fMaxV;

		if (bDecF)
		{
			bRet = TRUE;
			m_pData->m_INSP[POS[0]].m_bDecision = TRUE;
			m_pData->m_INSP[POS[1]].m_bDecision = TRUE;

			m_pData->m_Teach.m_LINK[nIndex].m_dValue = fMaxV;
		}
	}

	m_pData->m_Teach.m_LINK[nIndex].m_bDecision = bRet;
	return bRet;
}  
#endif // 0

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////     LINK SLINE MIN-MAX      ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////        캘리브레이션      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CMutualLink::GetScaleFactor(LPBYTE m_fmR, double dReal, long *x1, long *y1, long *x2, long *y2, double *dDx, double *dDy)
{
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////        캘리브레이션      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



