// FunctionEtc.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "FunctionEtc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunctionEtc

CFunctionEtc::CFunctionEtc()
{
}

CFunctionEtc::~CFunctionEtc()
{
}


BEGIN_MESSAGE_MAP(CFunctionEtc, CStatic)
	//{{AFX_MSG_MAP(CFunctionEtc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionEtc message handlers
void CFunctionEtc::GetOffsetAdjustment(SETUP_MODEL_DATA *pData, long nIndex, POINT2 *ptT1, POINT2 *ptT2, POINT2 *ptT3, POINT2 *ptT4)
{
	long nWidth, nHeight;
	float dSX, dSY, dDX, dDY, dDT;
	POINT2 ptDot1, ptDot2, ptDot3, ptDot4;
	GOM_ATTR *pGOM = &pData->m_GOM[nIndex];

	nWidth = pData->m_nImageWidth;
	nHeight = pData->m_nImageHeight;

	ptDot1 = pGOM->m_ptPoint1;
	ptDot2 = pGOM->m_ptPoint2;
	ptDot3 = pGOM->m_ptPoint3;
	ptDot4 = pGOM->m_ptPoint4;

	ptT1->x = ptDot1.x;
	ptT1->y = ptDot1.y;
	ptT2->x = ptDot2.x;
	ptT2->y = ptDot2.y;

	ptT3->x = ptDot3.x;
	ptT3->y = ptDot3.y;
	ptT4->x = ptDot4.x;
	ptT4->y = ptDot4.y;

	if (!pData->m_bRealTimeMode) return;

	if (pData->m_INSP[nIndex].m_bLocROI==0)
	{
		dSX = pData->m_dSourX;
		dSY = pData->m_dSourY;
		dDX = pData->m_dDeltaX;
		dDY = pData->m_dDeltaY;
		dDT = pData->m_dDeltaT;
	}
	else
	{
		dSX = pData->m_dSourX2;
		dSY = pData->m_dSourY2;
		dDX = pData->m_dDeltaX2;
		dDY = pData->m_dDeltaY2;
		dDT = pData->m_dDeltaT2;
	}

	ptDot1.x = (float)(dSX + (pGOM->m_ptPoint1.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint1.y-dSY) * sin(dDT*RAD));
	ptDot1.y = (float)(dSY + (pGOM->m_ptPoint1.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint1.x-dSX) * sin(dDT*RAD));
	ptDot2.x = (float)(dSX + (pGOM->m_ptPoint2.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint2.y-dSY) * sin(dDT*RAD));
	ptDot2.y = (float)(dSY + (pGOM->m_ptPoint2.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint2.x-dSX) * sin(dDT*RAD));
	ptDot3.x = (float)(dSX + (pGOM->m_ptPoint3.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint3.y-dSY) * sin(dDT*RAD));
	ptDot3.y = (float)(dSY + (pGOM->m_ptPoint3.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint3.x-dSX) * sin(dDT*RAD));
	ptDot4.x = (float)(dSX + (pGOM->m_ptPoint4.x-dSX) * cos(dDT*RAD) + (pGOM->m_ptPoint4.y-dSY) * sin(dDT*RAD));
	ptDot4.y = (float)(dSY + (pGOM->m_ptPoint4.y-dSY) * cos(dDT*RAD) - (pGOM->m_ptPoint4.x-dSX) * sin(dDT*RAD));

	ptT1->x = ptDot1.x + dDX;
	ptT1->y = ptDot1.y + dDY;
	ptT2->x = ptDot2.x + dDX;
	ptT2->y = ptDot2.y + dDY;
	ptT3->x = ptDot3.x + dDX;
	ptT3->y = ptDot3.y + dDY;
	ptT4->x = ptDot4.x + dDX;
	ptT4->y = ptDot4.y + dDY;

	if (ptT1->x < 0) ptT1->x = 0;
	if (ptT1->y < 0) ptT1->y = 0;
	if (ptT2->x < 0) ptT2->x = 0;
	if (ptT2->y < 0) ptT2->y = 0;
	if (ptT3->x < 0) ptT3->x = 0;
	if (ptT3->y < 0) ptT3->y = 0;
	if (ptT4->x < 0) ptT4->x = 0;
	if (ptT4->y < 0) ptT4->y = 0;

	if (ptT1->x>=nWidth)  ptT1->x = (float)(nWidth-1);
	if (ptT1->y>=nHeight) ptT1->y = (float)(nHeight-1);
	if (ptT2->x>=nWidth)  ptT2->x = (float)(nWidth-1);
	if (ptT2->y>=nHeight) ptT2->y = (float)(nHeight-1);
	if (ptT3->x>=nWidth)  ptT3->x = (float)(nWidth-1);
	if (ptT3->y>=nHeight) ptT3->y = (float)(nHeight-1);
	if (ptT4->x>=nWidth)  ptT4->x = (float)(nWidth-1);
	if (ptT4->y>=nHeight) ptT4->y = (float)(nHeight-1);
}

void CFunctionEtc::GetTransPosision(float dSX, float dSY, float dDX, float dDY, float dDT, float dCX, float dCY, POINT2 *ptT)
{
	ptT->x = (float)(dSX + (dCX-dSX) * cos(dDT*RAD) + (dCY-dSY) * sin(dDT*RAD));
	ptT->y = (float)(dSY + (dCY-dSY) * cos(dDT*RAD) - (dCX-dSX) * sin(dDT*RAD));

	ptT->x += dDX;
	ptT->y += dDY;
}
