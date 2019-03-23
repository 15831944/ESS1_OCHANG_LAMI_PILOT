// OverlayResultGOM.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "OverlayResultGOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayResultGOM
COverlayResultGOM::COverlayResultGOM()
{
}

COverlayResultGOM::~COverlayResultGOM()
{
}

BEGIN_MESSAGE_MAP(COverlayResultGOM, CStatic)
	//{{AFX_MSG_MAP(COverlayResultGOM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayResultGOM message handlers
BOOL COverlayResultGOM::DrawOverlayResult(CDC *pDC, SETUP_MODEL_DATA *pData)
{
	BOOL bRes, bCameraCal;
	long i, j, nType, nDir, bLoc, nMidY1, nMidY2;
	long nInspItemCount, nCntLoc;
	float dLowLimit, dHighLimit, dValue, dScale;
	CString szbuf, strTitle, str;
	COLORREF nColor;
		
	CRect rcRect, rcCorner;
	POINT2 vtDec, ptCir, vtSt, vtEt, vtT;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CFont *pNewFont, *pOldFont;
	
	pNewFont = new CFont;
	pNewFont->CreateFont(15, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    								   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);

	rcRect = pData->m_IMG.m_fmRect;
	dScale = 1.0f/pData->m_IMG.m_fZoomScale;

	///////////////////////////////         가로 라인 긋기      /////////////////////////////////
	nMidY1 = (long)((pData->m_Teach.m_fOffsetX - rcRect.top)/dScale);
	m_ImageBMP.DrawLine(pDC, 0, nMidY1, FRAME_WIDTH, nMidY1, 1, RGB(0, 240, 240));
	nMidY2 = (long)((pData->m_Teach.m_fOffsetY - rcRect.top)/dScale);
	m_ImageBMP.DrawLine(pDC, 0, nMidY2, FRAME_WIDTH, nMidY2, 1, RGB(0, 240, 240));
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      개별 항목 검사 표시     /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	
	bCameraCal = FALSE;
	nInspItemCount = 0;
	nCntLoc = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (!_tcscmp(pData->m_GOM[i].m_strTitle, _T("")) || !pData->m_INSP[i].m_bInspMode) continue;

 		m_pGOM = &pData->m_GOM[i];
		m_pSCAN = &pData->m_Scan[i];
		m_pINSP = &pData->m_INSP[i];
		bLoc = m_pINSP->m_bLocROI;

    	// 위치보정 
		m_FuncEtc.GetOffsetAdjustment(pData, i, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
		m_FuncEtc.GetTransPosision(pData->m_dSourX, pData->m_dSourY, pData->m_dDeltaX, pData->m_dDeltaY, pData->m_dDeltaT,
			                       m_pGOM->m_dCX, m_pGOM->m_dCY, &ptCir);
      	// 위치보정

		strTitle = pData->m_GOM[i].m_strTitle;
		nType = pData->m_INSP[i].m_nInspectType;
		nDir = pData->m_Scan[i].m_nSecondScanDirType;
		dValue = pData->m_INSP[i].m_dAnyValue;
		dLowLimit = pData->m_INSP[i].m_dStandLowLimit;
		dHighLimit = pData->m_INSP[i].m_dStandHighLimit;
		bRes = pData->m_INSP[i].m_bDecision;

		if (bRes) { 
			if(nType == INSP_MEASURE_SIDE_ONE) nColor = RGB(100, 0, 0);
			else nColor = RGB(255, 0, 0);
		}
		else
		{
			if (bLoc) {
				if(nType == INSP_MEASURE_SIDE_ONE) 
					nColor = RGB(0, 0, 200);
				else nColor = RGB(0, 150, 255);
			}
			else {
				if(nType ==	INSP_MEASURE_SIDE_ONE)  
					nColor = RGB(0, 120, 0); 
				else nColor = RGB(0, 255, 0);
			}
		}

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		vtDec.x = (ptInsp4.x - rcRect.left)/dScale;
		vtDec.y = (ptInsp4.y - rcRect.top)/dScale;

		switch (nType)
		{
			case INSP_MEASURE_SIDE_ONE :
				if (m_pSCAN->m_nSecondNum>1)
				{
					for(j=0; j<m_pSCAN->m_nSecondNum; j++)
					{
						vtEt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
						vtEt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;

						if (vtEt.x>0 || vtEt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x-2, (long)vtEt.y-2, (long)vtEt.x+3, (long)vtEt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x+2, (long)vtEt.y-2, (long)vtEt.x-3, (long)vtEt.y+3, 1, nColor);
						}
					}
				}
				else
				{
					vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
					vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
					vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
					vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
				}

				break;
			case INSP_MEASURE_SIDE_TWO :
			case INSP_MISMATCH :
				if (m_pSCAN->m_nSecondNum>1)
				{
					for(j=0; j<m_pSCAN->m_nSecondNum; j++)
					{
						vtSt.x = (m_pSCAN->m_dFirstPX[j] - rcRect.left-1)/dScale;
						vtSt.y = (m_pSCAN->m_dFirstPY[j] - rcRect.top-1)/dScale;
						vtEt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
						vtEt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;
						
						if (vtSt.x>0 || vtSt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtSt.x-2, (long)vtSt.y-2, (long)vtSt.x+3, (long)vtSt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtSt.x+2, (long)vtSt.y-2, (long)vtSt.x-3, (long)vtSt.y+3, 1, nColor);
						}

						if (vtEt.x>0 || vtEt.y>0)
						{
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x-2, (long)vtEt.y-2, (long)vtEt.x+3, (long)vtEt.y+3, 1, nColor);
							m_ImageBMP.DrawLine(pDC, (long)vtEt.x+2, (long)vtEt.y-2, (long)vtEt.x-3, (long)vtEt.y+3, 1, nColor);
						}
					}
				}
				else
				{
					vtSt.x = (m_pINSP->m_ptFirstMeas1.x - rcRect.left)/dScale;
					vtSt.y = (m_pINSP->m_ptFirstMeas1.y - rcRect.top)/dScale;
					vtEt.x = (m_pINSP->m_ptFirstMeas2.x - rcRect.left)/dScale;
					vtEt.y = (m_pINSP->m_ptFirstMeas2.y - rcRect.top)/dScale;

					float ptX, ptY;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
					{
						//m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
						ptX = (vtSt.x+vtEt.x)/2.0f;
						ptY = (vtSt.y+vtEt.y)/2.0f;
						m_ImageBMP.DrawLine(pDC, (long)ptX-2, (long)ptY-2, (long)ptX+3, (long)ptY+3, 1, nColor);
						m_ImageBMP.DrawLine(pDC, (long)ptX+2, (long)ptY-2, (long)ptX-3, (long)ptY+3, 1, nColor);
					}

					vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
					vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
					vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
					vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;

					if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
					{
						//m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
						ptX = (vtSt.x+vtEt.x)/2.0f;
						ptY = (vtSt.y+vtEt.y)/2.0f;
						m_ImageBMP.DrawLine(pDC, (long)ptX-2, (long)ptY-2, (long)ptX+3, (long)ptY+3, 1, nColor);
						m_ImageBMP.DrawLine(pDC, (long)ptX+2, (long)ptY-2, (long)ptX-3, (long)ptY+3, 1, nColor);
					}
				}
				break;
			case INSP_SLINE :
				vtSt.x = (m_pINSP->m_dCX - rcRect.left-1)/dScale;
				vtSt.y = (m_pINSP->m_dCY - rcRect.top-1)/dScale;

				if (vtSt.x>0 || vtSt.y>0)
				{
					m_ImageBMP.DrawLine(pDC, (long)vtSt.x-2, (long)vtSt.y-2, (long)vtSt.x+3, (long)vtSt.y+3, 1, nColor);
					m_ImageBMP.DrawLine(pDC, (long)vtSt.x+2, (long)vtSt.y-2, (long)vtSt.x-3, (long)vtSt.y+3, 1, nColor);
				}
				break;
			case INSP_DATUM_B :
			case INSP_DATUM_C :
				vtSt.x = (m_pINSP->m_ptSecondMeas1.x - rcRect.left-1)/dScale;
				vtSt.y = (m_pINSP->m_ptSecondMeas1.y - rcRect.top-1)/dScale;
				vtEt.x = (m_pINSP->m_ptSecondMeas2.x - rcRect.left-1)/dScale;
				vtEt.y = (m_pINSP->m_ptSecondMeas2.y - rcRect.top-1)/dScale;
				if (vtSt.x>0 || vtSt.y>0 || vtEt.x>0 || vtEt.y>0)
					m_ImageBMP.DrawLine(pDC, (long)vtSt.x, (long)vtSt.y, (long)vtEt.x, (long)vtEt.y, 1, nColor);
				break;

				/*
			case INSP_STRAIGHTNESS :
				for(j=0; j<m_pSCAN->m_nSecondNum; j++)
				{
					vtSt.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left-1)/dScale;
					vtSt.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top-1)/dScale;

					if (vtSt.x>0 || vtSt.y>0)
					{
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x-2, (long)vtSt.y-2, (long)vtSt.x+3, (long)vtSt.y+3, 1, nColor);
						m_ImageBMP.DrawLine(pDC, (long)vtSt.x+2, (long)vtSt.y-2, (long)vtSt.x-3, (long)vtSt.y+3, 1, nColor);
					}
				}
				break;
			    */
			case INSP_PATTERN_MATCH :
			case INSP_CAMERA_CAL :
				if (m_pINSP->m_dCX>0 && m_pINSP->m_dCY>0)
				{
					vtSt.x = (m_pINSP->m_dCX - rcRect.left)/dScale;
					vtSt.y = (m_pINSP->m_dCY - rcRect.top)/dScale;
					m_ImageBMP.DrawCross(pDC, (long)vtSt.x-1, (long)vtSt.y-1, 5,  1, nColor);
				}
				for(j=0; j<m_pSCAN->m_nSecondNum; j++)
				{
					vtT.x = (m_pSCAN->m_dSecondPX[j] - rcRect.left)/dScale;
					vtT.y = (m_pSCAN->m_dSecondPY[j] - rcRect.top)/dScale;
					m_ImageBMP.DrawPixel(pDC, (long)vtT.x-1, (long)vtT.y-1, 1, RGB(0, 0, 255));
				}
				break;
			case INSP_TAB_FOLDING :
			case INSP_BLOB_ANALYSIS :
				for(j=0; j<m_pINSP->m_nDefectCnt; j++)
				{
					rcCorner = m_pINSP->m_rcPosFind[j];

					vtSt.x = (rcCorner.left - rcRect.left-1)/dScale;
					vtSt.y = (rcCorner.top - rcRect.top-1)/dScale;
					vtEt.x = (rcCorner.right - rcRect.left-1)/dScale;
					vtEt.y = (rcCorner.bottom - rcRect.top-1)/dScale;

					m_ImageBMP.DrawRect(pDC, (long)vtSt.x-1, (long)vtSt.y-1, (long)vtEt.x-1, (long)vtEt.y-1, 1, RGB(0, 0, 255));
				}
				break;
        }

		m_ImageBMP.DrawFillCircle(pDC, (long)vtDec.x, (long)vtDec.y, 5, 1, nColor);

		if (nType != INSP_MEASURE_SIDE_ONE)
		{
			vtT.x = (ptInsp4.x - rcRect.left)/dScale;
			vtT.y = (ptInsp4.y - rcRect.top)/dScale;

			str.Format(_T("%5.2f"), m_pINSP->m_dAnyValue);
			if (m_pINSP->m_bDecision) 
				m_ImageBMP.DrawText(pDC, (long)vtT.x, (long)vtT.y, str, 1, RGB(255, 0, 0));
			else
				m_ImageBMP.DrawText(pDC, (long)vtT.x, (long)vtT.y, str, 1, nColor);
		}

		/*
		if (pData->m_GOM[i].m_bExecShowROI)
		{
			szbuf.Format("%s : [%3.2f %3.2f]=%3.2f", strTitle, dLowLimit, dHighLimit, fabs(dValue));
			pDC->TextOut(5, 40 + nInspItemCount*16, szbuf);

    		nInspItemCount++;
		}
		*/
		
		if (nType==INSP_CAMERA_CAL && m_pINSP->m_bDecision)
			bCameraCal = TRUE;

		if (!pData->m_bRealTimeMode)
		    pData->m_INSP[i].m_bInspMode = FALSE;
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      개별 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      LINK 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	/*
	long nNum, nIndex1, nIndex2;

	nCnt = pData->m_Teach.m_nLinkNum;
	for(i=0; i<nCnt; i++)
	{
		nNum=0;
		if (pData->m_Teach.m_LINK[i].m_bSel1) nNum++;
		if (pData->m_Teach.m_LINK[i].m_bSel2) nNum++;

		if (nNum>=2) 
		{
			nType = pData->m_Teach.m_LINK[i].m_nLinkType;
			bRes = pData->m_Teach.m_LINK[i].m_bDecision;
			nIndex1 = pData->m_Teach.m_LINK[i].m_nIndex1;
			nIndex2 = pData->m_Teach.m_LINK[i].m_nIndex2;

			if (bRes) nColor = RGB(255, 0, 0);
			else      nColor = RGB(0, 255, 0);

			pDC->SetTextColor(nColor);
    		pDC->SetBkMode(TRANSPARENT);

			if (nType==TEACH_DIST_LINE2LINE)
			{
				dValue = pData->m_Teach.m_LINK[i].m_dValue;
				break;
			}
		}
	}
	*/
	
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////      LINK 항목 검사 표시     //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	pDC->SelectObject(pOldFont);
	delete pNewFont;

	if (pData->m_bRealTimeMode)
	{
		CFont *pNewFont, *pOldFont;
		pNewFont = new CFont;
		pNewFont->CreateFont(40, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
		pOldFont = pDC->SelectObject(pNewFont);
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetBkMode(TRANSPARENT);

		vtT.x = ((pData->m_nImageWidth-300) - rcRect.left)/dScale;
		vtT.y = (100 - rcRect.top)/dScale;
		if (pData->m_bDecision)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->TextOut((int)vtT.x, (int)vtT.y, _T("NG")); 
		}
		else
		{
			pDC->SetTextColor(RGB(0, 255, 0));
			pDC->TextOut((int)vtT.x, (int)vtT.y, _T("OK")); 
		}

        pDC->SelectObject(pOldFont); 
		delete pNewFont;

		pNewFont = new CFont;
		pNewFont->CreateFont(20, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
		pOldFont = pDC->SelectObject(pNewFont);
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetBkMode(TRANSPARENT);

		if (bCameraCal)
		{
			pDC->SetTextColor(RGB(255, 255, 0));
			pDC->TextOut(FRAME_WIDTH-400, 20, _T(" ERROR CAMERA CAL. ! "));
		}

	    pDC->SelectObject(pOldFont); 
		delete pNewFont;
	}

	return TRUE;
}

