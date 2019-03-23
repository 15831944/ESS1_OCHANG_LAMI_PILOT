// Vision.cpp : implementation file
//
#include "stdafx.h"
#include "math.h"
#include "ModuleLink.h"
#include "NVisionDll.h"
#include "NPatternDll.h"
//#include "NMultiFuncDll.h"
#include "NBlobDll.h"
#include <omp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleLink
CModuleLink::CModuleLink()
{
	m_nImageWidth = IMAGE_WIDTH;
	m_nImageHeight = IMAGE_HEIGHT;
	m_pProcImage = NULL;
	m_fObjectEdgeY = 0;

	m_bCompens = FALSE;
	m_strSaveFilePath = _T("");

	NBlobDll_Open();
}

CModuleLink::~CModuleLink()
{
	NBlobDll_Close();

	delete m_pProcImage;
}

void CModuleLink::SetImageRawData(LPBYTE fmImage, long nWidth, long nHeight)
{
    m_pRawImage  = fmImage;
	
	m_nImageWidth = nWidth;
	m_nImageHeight= nHeight;

	if (m_pProcImage) delete m_pProcImage;
	m_pProcImage = new unsigned char[nWidth*nHeight];
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      전체 검사 모듈      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
long CModuleLink::ExecuteInspection()
{
	long nRet;
	CString str; 

	// 결과 데이타 초기화
	InitializeInspData();
//	m_MutualLink.InitializeMutualData();
	// 결과 데이타 초기화

	// 위치 보정 
	float dPX, dPY, dPT, dDX, dDY, dDT;
    CompensatePosition(0, &dPX, &dPY, &dPT, &dDX, &dDY, &dDT);
	CompensatePosition(1, &dPX, &dPY, &dPT, &dDX, &dDY, &dDT);
	// 위치 보정

	// 전처리 프로세스  
	//PreProcessSpecialData();
	// 전처리 프로세스  

	// Measurement Object 검사 
    InspectMeasurement();
	InspectMeasurement2();
	// Measurement Object 검사 

	// Measurement Object 검사 
    //InspectStraightLine();
	// Measurement Object 검사 


	// Measurement Object 검사 
    InspectPatternMatch();
	// Measurement Object 검사 

	//InspectTabFolding();
	//InspectBlobAnalysis();

	// Measurement Object 검사 
    //InspectEtc();
	// Measurement Object 검사 

	// Link Object 검사 
    m_MutualLink.InspectMutualLinkObjects();//
	// Link Object 검사 

	// Laminate Fine CAL 처리 
#if _DUO_CELL
//	if (m_pData->m_bRealTimeMode==1)
//	    FineLaminateCal();		

//	CalcOffsetData();
#else
//	if (m_pData->m_bRealTimeMode)
//	    FineLaminateOneCell();
#endif
	// Laminate Fine CAL 처리 

	// 최종 검사 결과 판정
    nRet = ProcessResultDecision();
	memcpy(&m_pResB, &m_pResData, sizeof(INSP_RESULT_DATA));

    return nRet;
}

BOOL CModuleLink::ExecuteInspectionROI(long nIndex)
{
	BOOL bRet = FALSE;
	long nGOMType, nType;

	if (_tcscmp(m_pData->m_GOM[nIndex].m_strTitle, _T("")))
	{
		nType = m_pData->m_INSP[nIndex].m_nInspectType;
		nGOMType = m_pData->m_GOM[nIndex].m_nGOMType;

		switch(nType)
		{
			case INSP_MEASURE_SIDE_ONE :
			case INSP_MEASURE_SIDE_TWO :
			case INSP_SLINE :
			case INSP_MISMATCH :
				bRet = InspectMeasurementROI(nIndex);
				break;
			case INSP_PATTERN_MATCH :
				bRet = InspectPatternMatchROI(0, nIndex);
				break;
			case INSP_GRAY_VALUE :
				InspectAverGrayValueROI(nIndex);
				break;
			case INSP_TAB_FOLDING :
				InspectTabFoldingROI(nIndex);
				break;
		}
	}

    return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      전체 검사 모듈      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    결과 데이타 초기화    ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void CModuleLink::InitializeInspData()
{
	long i, j;

	m_fMDatumA = 0.0f;
	m_fMDatumT = 0.0f;
	m_fObjectEdgeY = 0;
	m_pResData.m_bDecision = FALSE;
	m_pResData.m_bStatus = FALSE;
	m_pResData.m_bFDecision = FALSE;
	m_pResData.m_bRDecision = FALSE;
	m_pResData.m_bFDataDec = FALSE;
	m_pResData.m_bRDataDec = FALSE;
	m_pResData.m_dOffsetEPC = 0.0f;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData.m_strTitle[i], _T(""));
		m_pResData.m_bDecisionROI[i] = FALSE;
		m_pResData.m_bLocROI[i] = 0;
		m_pResData.m_nInspType[i] = 0;
		m_pResData.m_nPlotNum[i] = 0;
		m_pResData.m_dValue[i] = 0.0f;
		m_pResData.m_dLowLimit[i] = 0.0f;
		m_pResData.m_dHighLimit[i] = 0.0f;

#if _NG_DISP
		m_pResData.m_dPosRoiX1[i] = m_pData->m_GOM[i].m_ptPoint1.x;
		m_pResData.m_dPosRoiY1[i] = m_pData->m_GOM[i].m_ptPoint1.y;
		m_pResData.m_dPosRoiX2[i] = m_pData->m_GOM[i].m_ptPoint4.x;
		m_pResData.m_dPosRoiY2[i] = m_pData->m_GOM[i].m_ptPoint4.y;

	    m_pResData.m_nSampleNum[i] = 0;
		for(j=0; j<SAMPLE_MAX_NUM; j++)
		{
			m_pResData.m_ptFirstEdge[i][j].x = 0.0f;
			m_pResData.m_ptSecondEdge[i][j].y = 0.0f;
		}
#endif

        InitializeInspDataROI(i);
	}

	for(i=0; i<LOCATE_MAX_NUM; i++)	
	{
        m_pData->m_Teach.m_bPreDecision = FALSE;
		m_pData->m_Teach.m_LOC[i].m_bDecision = FALSE;
	}

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData.m_strLinkTitle[i], _T(""));
		m_pResData.m_bLinkDecROI[i] = FALSE;
		m_pResData.m_bLinkLocROI[i] = 0;
		m_pResData.m_nLinkType[i] = 0;
		m_pResData.m_nLinkPlotNum[i] = 0;
		m_pResData.m_dLinkValue[i] = 0.0f;
		m_pResData.m_dLinkLowLimit[i] = 0.0f;
		m_pResData.m_dLinkHighLimit[i] = 0.0f;

		m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
		m_pData->m_Teach.m_LINK[i].m_dCX = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dCY = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dValue = 0.0f;
		m_pData->m_Teach.m_LINK[i].m_dValueS = 0;
	}

    m_pData->m_bDecision = FALSE;                // 결과 판정
}

void CModuleLink::InitializeInspDataROI(long nIndex)
{
	POINT2 pt;

	m_pData->m_bDecision = FALSE;

	pt.x = 0.0f;
	pt.y = 0.0f;

	m_pData->m_dDeltaX = (float)0.0;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData->m_dDeltaY = (float)0.0;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData->m_dDeltaT = (float)0.0;             // 각도 보정을 위한 T Offset(검사시 적용)

	m_pData->m_dDeltaX2 = (float)0.0;             // 위치 보정을 위한 X Offset(검사시 적용)
	m_pData->m_dDeltaY2 = (float)0.0;             // 위치 보정을 위한 Y Offset(검사시 적용)
	m_pData->m_dDeltaT2 = (float)0.0;             // 각도 보정을 위한 T Offset(검사시 적용)

	m_pData->m_Scan[nIndex].m_dFirstA = (float)0.0;
	m_pData->m_Scan[nIndex].m_dFirstB = (float)0.0;
	m_pData->m_Scan[nIndex].m_dFirstT = (float)0.0;

	m_pData->m_Scan[nIndex].m_dSecondA = (float)0.0;
	m_pData->m_Scan[nIndex].m_dSecondB = (float)0.0;
	m_pData->m_Scan[nIndex].m_dSecondT = (float)0.0;

	m_pData->m_Scan[nIndex].m_nFirstNum = 0;                 // SCAN EDGE 데이타 수 
	m_pData->m_Scan[nIndex].m_nSecondNum = 0;                // SCAN EDGE 데이타 수 

	m_pData->m_Scan[nIndex].m_ptFirstMin = pt;
	m_pData->m_Scan[nIndex].m_ptFirstMax = pt;
	m_pData->m_Scan[nIndex].m_ptSecondMin = pt;
	m_pData->m_Scan[nIndex].m_ptSecondMax = pt;

	m_pData->m_INSP[nIndex].m_bInspMode = TRUE;            // 해당 ROI 검사
	m_pData->m_INSP[nIndex].m_bDecision = FALSE;           // 해당 ROI 판정 (결과값) : TRUE, FALSE / -1 : 사용하지 않음
    m_pData->m_INSP[nIndex].m_dCX = 0.0f;
    m_pData->m_INSP[nIndex].m_dCY = 0.0f;
    m_pData->m_INSP[nIndex].m_dRadius = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas1.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas1.y = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas2.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptFirstMeas2.y = 0.0f;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1.x = 0.0f;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1.y = 0.0f;   
	m_pData->m_INSP[nIndex].m_ptSecondMeas2.x = 0.0f;   
	m_pData->m_INSP[nIndex].m_ptSecondMeas2.y = 0.0f;

	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt;

    m_pData->m_INSP[nIndex].m_dAnyValue = 0.0f;
	m_pData->m_INSP[nIndex].m_dSumV = 0.0f;
	m_pData->m_INSP[nIndex].m_nDefectCnt = 0;
	m_pData->m_INSP[nIndex].m_dSumV = 0.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    결과 데이타 초기화    ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         위치 보정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
/*
BOOL CModuleLink::CompensatePosition(long nLoc, float *dPX, float *dPY, float *dPT, float *dDX, float *dDY, float *dDT)
{
	BOOL bRet=TRUE;
	POINT2 ptROI;
    long i, nCntP, nCntE, nType, nPos, nDir, nLocT;
	float fTop, fLeft, dAngle, dTemp;
	float dA1[2], dA2[2], dB1[2], dB2[2], fCX[7], fCY[7];
	float fTargX, fTargY, fMidY;
	POINT2 ptT1[2], ptT2[2], pt1, pt2, pt3, pt4;

	*dPX = 0.0f;
	*dPY = 0.0f;
	*dDX = 0.0f;
	*dDY = 0.0f;
	*dDT = 0.0f;
		
	fTop = -1;
	fLeft = -1;
	
	fTargX = 0.0f;
	fTargY = 0.0f;
	fMidY = m_nImageHeight/2.0f;

	nCntP = 0;
	nCntE = 0;

	for(i=0; i<2; i++)
	{
		dA1[i] = 0;
		dA2[i] = 0;
		dB1[i] = 0;
		dB2[i] = 0;
		fCX[i] = 0;
		fCY[i] = 0;
	}

    for(i=0; i<m_pData->m_Teach.m_nLocNum; i++)
	{
		nPos = m_pData->m_Teach.m_LOC[i].m_nIndex1;
		if (nPos>=0)
		{
			nType = m_pData->m_INSP[nPos].m_nInspectType;
			nDir = m_pData->m_Scan[nPos].m_nSecondScanDirType;
			nLocT = m_pData->m_INSP[nPos].m_bLocROI;

			ptROI.x = (m_pData->m_GOM[nPos].m_ptPoint1.x + m_pData->m_GOM[nPos].m_ptPoint4.x)/2.0f;
			ptROI.y = (m_pData->m_GOM[nPos].m_ptPoint1.y + m_pData->m_GOM[nPos].m_ptPoint4.y)/2.0f;
			
			if ((nType==INSP_MEASURE_SIDE_ONE|| nType==INSP_MEASURE_SIDE_TWO) && nLocT==nLoc)
			{
		   		InspectMeasurementROI(nPos);
				
				if (nDir==DIR_LEFT_RIGHT || nDir==DIR_RIGHT_LEFT)
				{
					if (ptROI.y < fMidY) 
					{
						ptT1[0] = ptROI;
						dA1[0] = m_pData->m_Scan[nPos].m_dSecondA;
						dB1[0] = m_pData->m_Scan[nPos].m_dSecondB;
					}
					else 
					{
						ptT2[0] = ptROI;
						dA2[0] = m_pData->m_Scan[nPos].m_dSecondA;
						dB2[0] = m_pData->m_Scan[nPos].m_dSecondB;
					}
				}
				else
				{
					if (ptROI.y < fMidY) 
					{
						ptT1[1] = ptROI;
						dA1[1] = m_pData->m_Scan[nPos].m_dSecondA;
						dB1[1] = m_pData->m_Scan[nPos].m_dSecondB;
					}
					else 
					{
						ptT2[1] = ptROI;
						dA2[1] = m_pData->m_Scan[nPos].m_dSecondA;
						dB2[1] = m_pData->m_Scan[nPos].m_dSecondB;
					}
				}
			}
		}
	}
	
	if ((dA1[0]!=0 || dB1[0]!=0) && (dA1[1]!=0 || dB1[1]!=0) && (dA2[0]!=0 || dB2[0]!=0) && (dA2[1]!=0 || dB2[1]!=0))
	{
		pt1.x = dA1[0]*ptT1[0].y + dB1[0];
		pt1.y = ptT1[0].y;
		pt2.y = dA1[1]*pt1.x + dB1[1];
		pt2.x = dA1[0]*pt2.y + dB1[0];

		pt3.x = dA2[0]*ptT2[0].y + dB2[0];
		pt3.y = ptT2[0].y;
		pt4.y = dA2[1]*pt3.x + dB2[1];
		pt4.x = dA2[0]*pt4.y + dB2[0];

		m_libVis.GetAngle(pt2.x, pt2.y, pt4.x, pt4.y, &dAngle);

    	*dPX = (fCX[0]+fCX[1])/2.0f;
		*dPY = (fCY[0]+fCY[1])/2.0f;
		*dPT = dAngle;

		*dPX = fTargX = (pt2.x+pt4.x) / 2.0f;
		*dPY = fTargY = (pt2.y+pt4.y) / 2.0f;
		
	    if (nLoc==0)
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX);
			*dDY = (float)(fTargY - m_pData->m_dSourY);
			*dDT = dAngle - m_pData->m_dSourT;

			m_pData->m_dDeltaX = *dDX;
			m_pData->m_dDeltaY = *dDY;
			m_pData->m_dDeltaT = *dDT;
		}
		else
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX2);
			*dDY = (float)(fTargY - m_pData->m_dSourY2);
			*dDT = dAngle - m_pData->m_dSourT2;

			m_pData->m_dDeltaX2 = *dDX;
			m_pData->m_dDeltaY2 = *dDY;
			m_pData->m_dDeltaT2 = *dDT;
		}

		m_fObjectEdgeY = pt2.x;
		bRet = FALSE;
	}
	

	m_pData->m_Teach.m_bLocDecision = bRet;

	return bRet;
}
*/


BOOL CModuleLink::CompensatePosition(long nLoc, float *dPX, float *dPY, float *dPT, float *dDX, float *dDY, float *dDT)
{
	BOOL bRet=TRUE;
    long i, nCntP, nCntE, nType, nPos, nDir, nLocT;
	float fTop, fLeft, dAngle;
	float dPosX[7], dPosY[7], fCX[7], fCY[7];
	float fTargX, fTargY, fTemp;

	*dPX = 0.0f;
	*dPY = 0.0f;
	*dDX = 0.0f;
	*dDY = 0.0f;
	*dDT = 0.0f;
		
	fTop = -1;
	fLeft = -1;
	
	fTargX = 0.0f;
	fTargY = 0.0f;

	nCntP=0;
	nCntE = 0;

    for(i=0; i<m_pData->m_Teach.m_nLocNum; i++)
	{
		nPos = m_pData->m_Teach.m_LOC[i].m_nIndex1;
		if (nPos>=0)
		{
			nType = m_pData->m_INSP[nPos].m_nInspectType;
			nDir = m_pData->m_Scan[nPos].m_nSecondScanDirType;
			nLocT = m_pData->m_INSP[nPos].m_bLocROI;

			if (nType==INSP_PATTERN_MATCH && nLocT==nLoc)
			{
		   		bRet = InspectPatternMatchROI(1, nPos);
				if (bRet) break;

				fCX[nCntP] = m_pData->m_GOM[nPos].m_dCX;
				fCY[nCntP] = m_pData->m_GOM[nPos].m_dCY;

				dPosX[nCntP] = m_pData->m_INSP[nPos].m_dCX;
				dPosY[nCntP] = m_pData->m_INSP[nPos].m_dCY;
				nCntP++;
			}
			
			if (nType==INSP_MEASURE_SIDE_ONE && nLocT==nLoc)
			{
		   		InspectMeasurementROI(nPos);

				if (nDir==DIR_TOP_BOTTOM || nDir==DIR_BOTTOM_TOP)
				{
					//if (m_pData->m_INSP[nPos].m_ptFirstMeas1.y<m_pData->m_INSP[nPos].m_ptSecondMeas1.y)
					//	fTemp = m_pData->m_INSP[nPos].m_ptFirstMeas1.y;
					//else
						fTemp = m_pData->m_INSP[nPos].m_ptSecondMeas1.y;

					fTop = fTemp;
					nCntE++;
				}
				else
				{
					//if (m_pData->m_INSP[nPos].m_ptFirstMeas1.x<m_pData->m_INSP[nPos].m_ptSecondMeas1.x)
					//	fTemp = m_pData->m_INSP[nPos].m_ptFirstMeas1.x;
					//else
						fTemp = m_pData->m_INSP[nPos].m_ptSecondMeas1.x;

					fLeft = fTemp;
					nCntE++;
				}
			}
		}
	}

	if (nCntP>=2)
	{
		m_libVis.GetAngle(dPosX[0], dPosY[0], dPosX[1], dPosY[1], &dAngle);

    	*dPX = (fCX[0]+fCX[1])/2.0f;
		*dPY = (fCY[0]+fCY[1])/2.0f;
		*dPT = dAngle;

		fTargX = (dPosX[0]+dPosX[1]) / 2.0f;
		fTargY = (dPosY[0]+dPosY[1]) / 2.0f;
		
	    if (nLoc==0)
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX);
			*dDY = (float)(fTargY - m_pData->m_dSourY);
			*dDT = dAngle - m_pData->m_dSourT;

			m_pData->m_dDeltaX = *dDX;
			m_pData->m_dDeltaY = *dDY;
			m_pData->m_dDeltaT = *dDT;
		}
		else
		{
			*dDX = (float)(fTargX - m_pData->m_dSourX2);
			*dDY = (float)(fTargY - m_pData->m_dSourY2);
			*dDT = dAngle - m_pData->m_dSourT2;

			m_pData->m_dDeltaX2 = *dDX;
			m_pData->m_dDeltaY2 = *dDY;
			m_pData->m_dDeltaT2 = *dDT;
		}

		bRet = FALSE;
	}

	if (nCntE>=2 && fLeft>0 && fTop>0)
	{
		*dPX = fLeft;
		*dPY = fTop;
		*dPT = 0;

	    if (nLoc==0)
		{
			*dDX = (float)(fLeft - m_pData->m_dSourX);
			*dDY = (float)(fTop - m_pData->m_dSourY);
			//*dDT = dAngle - m_pData->m_dSourT;

			m_pData->m_dDeltaX = *dDX;
			m_pData->m_dDeltaY = *dDY;
			m_pData->m_dDeltaT = 0;
		}
		else
		{
			*dDX = (float)(fLeft - m_pData->m_dSourX2);
			*dDY = (float)(fTop - m_pData->m_dSourY2);
			//*dDT = dAngle - m_pData->m_dSourT2;

			m_pData->m_dDeltaX2 = *dDX;
			m_pData->m_dDeltaY2 = *dDY;
			m_pData->m_dDeltaT2 = 0;
		}

		m_fObjectEdgeY = fTop;
		bRet = FALSE;
	}

	m_pData->m_Teach.m_bLocDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         위치 보정        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////        전처리 함수        ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void CModuleLink::PreProcessSpecialData()
{
	BOOL bRet;
	long i, nType;
	float dA;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nType==INSP_DATUM_B)
		{
		    bRet = InspectStraightLineROI(i);
			dA = m_pData->m_Scan[i].m_dSecondA;

			m_fMDatumA = m_libMath.GetPerpendicularM(dA);
			m_fMDatumT = m_pData->m_Scan[i].m_dSecondT;
			break;
		}		
    }
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         측정 검사        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectMeasurement()
{
	long i, nGOMType, nType;
	BOOL bRet = FALSE;

	m_fObjectEdgeY = 0.0f;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nGOMType==GOM_RECT && 
		   (nType==INSP_MEASURE_SIDE_ONE || nType==INSP_MEASURE_SIDE_TWO))
		    bRet = InspectMeasurementROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectMeasurement2()
{
	long i, nGOMType, nType;
	BOOL bRet = FALSE;

	m_fObjectEdgeY = 0.0f;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nGOMType==GOM_RECT && nType==INSP_MISMATCH)
		    bRet = InspectMeasurementROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectMeasurementROI(long nIndex)
{
	BOOL  bRet=FALSE, bRear;
	long  i, j, nCnt1, nCnt2, nH, nMid, nInvalidCnt, nMisMatchL, nTop2, nBot2;
	long  nInspType, nFirstPol, nSecondPol, nFirstDir, nSampleNum;
	long  nSecondDir, nFirstThresh, nSecondThresh, nScanType, nMaxThr, nMinThr;;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	POINT2 ptT1, ptT2, ptT3, ptT4, ptPos1, ptPos2;
	POINT2 pt1, pt2, pt3, pt4, ptCP;
	POINT2 ptCross1, ptCross2, ptCross3, ptCross4;
	float fLowL, fHighL, dStdSpec, fF, fS, dNoTiltValue, fRate, fSize;
	float dA1, dB1, dA2, dB2;
	float a, b, c, d, t, dScale, dValue, fDiff, dOffsetV, fTransV, fMax, fBadValue;
	float ARR_VALUE[50];
	float *pEdgeX, *pEdgeY;
	float *m_p1EdgeX;
	float *m_p1EdgeY;
	float *m_p2EdgeX;
	float *m_p2EdgeY;
	CString str, strTitle;

	// 검사용 MODE로 전환
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환

    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

	nInspType = m_pData->m_INSP[nIndex].m_nInspectType;
	nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
	nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
	nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
	nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
	nFirstThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;
	nSecondThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;

	nMaxThr = (long)m_pData->m_INSP[nIndex].m_nMaxThr;
	nMinThr = (long)m_pData->m_INSP[nIndex].m_nMinThr;

	nScanType = SCAN_PROJECT_DFDT;//m_pData->m_Scan[nIndex].m_nScanType;
	nSampleNum = m_pData->m_INSP[nIndex].m_nSampleNum;
	bRear = m_pData->m_INSP[nIndex].m_bLocROI;
	dOffsetV = m_pData->m_INSP[nIndex].m_dOffsetV;
	fRate = m_pData->m_INSP[nIndex].m_bOverlapRate/10.0f;
	
	if (nSampleNum<1) 
		nSampleNum=1;

	nH = m_nImageHeight/2;
	nMid = (long)(ptInsp1.y + ptInsp4.y)/2;

	dValue = 0.0f;
	fTransV = 0.0f;
	dNoTiltValue = 0.0f;

	fLowL = m_pData->m_INSP[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_INSP[nIndex].m_dStandHighLimit;
	dStdSpec = (fLowL+fHighL)/2;

	str.Format(_T("%s"), m_pData->m_GOM[nIndex].m_strTitle);
	strTitle = WithoutSpaceToString(str);

	m_pData->m_Scan[nIndex].m_nFirstNum = nSampleNum;
	m_pData->m_Scan[nIndex].m_nSecondNum = nSampleNum;

	if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
	{
		if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
		if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
		if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
		if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;
	}
	else
	{
		if (bRear) dScale = m_pCAL->m_dScaleR_Y;
		else  dScale = m_pCAL->m_dScaleL_Y;
	}

	m_p1EdgeX = new float[EDGE_MAX_NUM];
	m_p1EdgeY = new float[EDGE_MAX_NUM];
	m_p2EdgeX = new float[EDGE_MAX_NUM];
	m_p2EdgeY = new float[EDGE_MAX_NUM];

	pEdgeX = new float[EDGE_MAX_NUM];
	pEdgeY = new float[EDGE_MAX_NUM];

	if (nInspType==INSP_MEASURE_SIDE_ONE)
	{
		nInvalidCnt=0;
		m_n1EdgeCnt = 0;

		if (nSampleNum>1) 
		{
			for(i=0; i<nSampleNum; i++)
			{
				ptT1 = ptInsp1;
				ptT2 = ptInsp2;
				ptT3 = ptInsp3;
				ptT4 = ptInsp4;

				m_pData->m_Scan[nIndex].m_dSecondPX[i] = 0.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[i] = 0.0f;

				if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
				{
					fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.y;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.y;
					}
					else
					{
						fF = ptInsp1.y + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

					ptT1.y = fF;
					ptT2.y = fF;
					ptT3.y = fS;
					ptT4.y = fS;
				}
				else
				{
					fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.x;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.x;
					}
					else
					{
						fF = ptInsp1.x + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

					ptT1.x = fF;
					ptT2.x = fS;
					ptT3.x = fF;
					ptT4.x = fS;
				}
					
				nInvalidCnt=0;
				nCnt1 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nSecondDir, nSecondPol, nSecondThresh, nMaxThr, nMinThr, &ptPos1, &ptPos2, pEdgeX, pEdgeY, &dA1, &dB1);
				if (nCnt1>0)
				{
					for(j=0; j<nCnt1; j++)
					{
						m_p1EdgeX[m_n1EdgeCnt] = pEdgeX[j];
						m_p1EdgeY[m_n1EdgeCnt] = pEdgeY[j];
						if (m_n1EdgeCnt>=EDGE_MAX_NUM)
							break;

						m_n1EdgeCnt++;
					}

					m_pData->m_Scan[nIndex].m_dSecondPX[i] = (ptPos1.x+ptPos2.x) / 2.0f;
				    m_pData->m_Scan[nIndex].m_dSecondPY[i] = (ptPos1.y+ptPos2.y) / 2.0f;
				}
			}

			if (m_n1EdgeCnt>5)
			{
				if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
				{
					m_libVis.LineFitting(m_n1EdgeCnt, m_p1EdgeY, m_p1EdgeX, (int)(m_n1EdgeCnt*0.1), 2, &t, &dA1, &dB1);
					m_pData->m_Scan[nIndex].m_dSecondA = dA1; 
					m_pData->m_Scan[nIndex].m_dSecondB = dB1; 

					pt1.x = dA1*ptInsp1.y + dB1;
					pt1.y = ptInsp1.y;
					pt2.x = dA1*ptInsp3.y + dB1;
					pt2.y = ptInsp3.y;
				}
				else
				{
					m_libVis.LineFitting(m_n1EdgeCnt, m_p1EdgeX, m_p1EdgeY, (int)(m_n1EdgeCnt*0.1), 2, &t, &dA1, &dB1);
					m_pData->m_Scan[nIndex].m_dSecondA = dA1; 
					m_pData->m_Scan[nIndex].m_dSecondB = dB1; 

					pt1.x = ptInsp1.x;
					pt1.y = dA1*ptInsp1.x + dB1;
					pt2.x = ptInsp2.x;
					pt2.y = dA1*ptInsp2.x + dB1;
				}

				if (dA1!=0 || dB1!=0)
				{
					m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt1;
					m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt2;
				}
			}

			m_pData->m_Scan[nIndex].m_nSecondNum = nSampleNum;
		}
		else
		{
    		nCnt1 = GetPosEdgeDetection(ptInsp1, ptInsp2, ptInsp3, ptInsp4, nInspType, nScanType, nSecondDir, nSecondPol, nSecondThresh, nMaxThr, nMinThr, &ptPos1, &ptPos2, pEdgeX, pEdgeY, &dA1, &dB1);
			if (nCnt1>0)
			{
				m_pData->m_Scan[nIndex].m_dSecondA = dA1; 
				m_pData->m_Scan[nIndex].m_dSecondB = dB1; 
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = ptPos2;

				m_pData->m_Scan[nIndex].m_dSecondPX[0] = (ptPos1.x+ptPos2.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[0] = (ptPos1.y+ptPos2.y)/2.0f;
			}
		}

		m_pData->m_Scan[nIndex].m_nSecondNum = nSampleNum;
	}

	if (nInspType==INSP_MEASURE_SIDE_TWO || nInspType==INSP_MISMATCH)
	{
		nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
		nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
		nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
		nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;

		fBadValue = 0.0f;
		m_fCalPixelDist = 0.0;
		m_n1EdgeCnt = 0;
		m_n2EdgeCnt = 0;
		fMax = 0.0f;

		for(i=0; i<nSampleNum; i++)
		{
			dValue = 0.0f;
			ARR_VALUE[i] = 0.0f;

			m_pData->m_Scan[nIndex].m_dFirstPX[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dFirstPY[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dSecondPX[i] = 0.0f;
			m_pData->m_Scan[nIndex].m_dSecondPY[i] = 0.0f;

			ptT1 = ptInsp1;
			ptT2 = ptInsp2;
			ptT3 = ptInsp3;
			ptT4 = ptInsp4;

			if (nSampleNum>1) 
			{
				ptT1 = ptInsp1;
				ptT2 = ptInsp2;
				ptT3 = ptInsp3;
				ptT4 = ptInsp4;

				m_pData->m_Scan[nIndex].m_dSecondPX[i] = 0.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[i] = 0.0f;

				if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
				{
					fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.y;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.y;
					}
					else
					{
						fF = ptInsp1.y + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

					ptT1.y = fF;
					ptT2.y = fF;
					ptT3.y = fS;
					ptT4.y = fS;
				}
				else
				{
					fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
					if (i==0)
					{
						fF = ptInsp1.x;
						fS = fF+fSize + fSize*fRate*2.0f;
					}
					else if (i==nSampleNum-1)
					{
						fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
						fS = ptInsp4.x;
					}
					else
					{
						fF = ptInsp1.x + i*fSize - fSize*fRate;
						fS = fF+fSize + fSize*fRate*2.0f;
					}

					if (fF<10) fF = 10;
					if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

					ptT1.x = fF;
					ptT2.x = fS;
					ptT3.x = fF;
					ptT4.x = fS;
				}

				m_libMath.GetCrossPoint2Line(ptT1, ptT2, ptT3, ptT4, &ptCP);
			}

			nInvalidCnt=0;
			nCnt1 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nFirstDir, nFirstPol, nFirstThresh, nMaxThr, nMinThr, &ptPos1, &ptPos2, pEdgeX, pEdgeY, &dA1, &dB1);
			if (nCnt1>0)
			{
				for(j=0; j<nCnt1; j++)
				{
					m_p1EdgeX[m_n1EdgeCnt] = pEdgeX[j];
					m_p1EdgeY[m_n1EdgeCnt] = pEdgeY[j];
					if (m_n1EdgeCnt>=EDGE_MAX_NUM)
						break;

					m_n1EdgeCnt++;
				}

				m_pData->m_INSP[nIndex].m_ptFirstMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptFirstMeas2 = ptPos2;
			}
		
			nFirstPol = m_pData->m_Scan[nIndex].m_nFirstPolDirType;
			nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
			nFirstDir = m_pData->m_Scan[nIndex].m_nFirstScanDirType;
			nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;

			nInvalidCnt=0;
			nCnt2 = GetPosEdgeDetection(ptT1, ptT2, ptT3, ptT4, nInspType, nScanType, nSecondDir, nSecondPol, nSecondThresh, nMaxThr, nMinThr, &ptPos1, &ptPos2, pEdgeX, pEdgeY, &dA2, &dB2);
			if (nCnt2>0)
			{
				for(j=0; j<nCnt2; j++)
				{
					m_p2EdgeX[m_n2EdgeCnt] = pEdgeX[j];
					m_p2EdgeY[m_n2EdgeCnt] = pEdgeY[j];
					if (m_n2EdgeCnt>=EDGE_MAX_NUM)
						break;

					m_n2EdgeCnt++;
				}
				
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = ptPos1;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = ptPos2;
			}

			if (nCnt1>0 && nCnt2>0)
			{
				if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
				{
					pt1.x = dA1*ptT1.y + dB1;
					pt1.y = ptT1.y;
					pt2.x = dA1*ptT3.y + dB1;
					pt2.y = ptT3.y;

					pt3.x = dA2*ptT2.y + dB2;
					pt3.y = ptT2.y;
					pt4.x = dA2*ptT4.y + dB2;
					pt4.y = ptT4.y;

					dNoTiltValue = (fabs(pt1.x-pt3.x)+fabs(pt2.x-pt4.x))/2.0f;
				}
				else
				{
					pt1.y = dA1*ptT1.x + dB1;
					pt1.x = ptT1.x;
					pt2.y = dA1*ptT2.x + dB1;
					pt2.x = ptT2.x;

					pt3.y = dA2*ptT3.x + dB2;
					pt3.x = ptT3.x;
					pt4.y = dA2*ptT4.x + dB2;
					pt4.x = ptT4.x;

					dNoTiltValue = (fabs(pt1.y-pt3.y)+fabs(pt2.y-pt4.y))/2.0f;
				}

				m_libMath.GetCrossPointDotLine(pt1, pt3, pt4, &ptCross1);
				m_libMath.GetCrossPointDotLine(pt2, pt3, pt4, &ptCross2);
				m_libMath.GetCrossPointDotLine(pt3, pt1, pt2, &ptCross3);
				m_libMath.GetCrossPointDotLine(pt4, pt1, pt2, &ptCross4);

				a = (float)m_libMath.GetDistance2D(pt1, ptCross1);
				b = (float)m_libMath.GetDistance2D(pt2, ptCross2);
				c = (float)m_libMath.GetDistance2D(pt3, ptCross3);
				d = (float)m_libMath.GetDistance2D(pt4, ptCross4);

				dValue = (a+b+c+d) / 4.0f;
			
	#if !DEF_TILTING_ALG
				//if (strTitle.Find(_T("A"))<0)
	    		dValue = dNoTiltValue;
	#endif

				m_pData->m_INSP[nIndex].m_ptFirstMeas1 = pt1;
				m_pData->m_INSP[nIndex].m_ptFirstMeas2 = pt2;
				m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt3;
				m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt4;

				m_pData->m_Scan[nIndex].m_dFirstPX[i] = (pt1.x+pt2.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dFirstPY[i] = (pt1.y+pt2.y)/2.0f; 
				m_pData->m_Scan[nIndex].m_dSecondPX[i] = (pt3.x+pt4.x)/2.0f;
				m_pData->m_Scan[nIndex].m_dSecondPY[i] = (pt3.y+pt4.y)/2.0f; 

				m_fCalPixelDist = dValue;
				dValue = (dValue * dScale);
				dValue += dOffsetV;
				fDiff = (float)fabs(dStdSpec - dValue);
			}

			if (fLowL<=dValue && dValue<=fHighL)
				ARR_VALUE[i] = dValue;
			else
			{
				if (fDiff>fMax)
				{
					fMax = fDiff;
					fBadValue = dValue;
				}
				bRet = TRUE;
			}
		}

		/*
		m_pData->m_Scan[nIndex].m_dFirstA = 0;
		m_pData->m_Scan[nIndex].m_dFirstB = 0;

		if (m_n1EdgeCnt>5)
		{
			if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
			{
				m_libVis.LineFitting(m_n1EdgeCnt, m_p1EdgeY, m_p1EdgeX, (int)(m_n1EdgeCnt*0.1), 2, &t, &dA1, &dB1);
				m_pData->m_Scan[nIndex].m_dFirstA = dA1; 
		        m_pData->m_Scan[nIndex].m_dFirstB = dB1; 

				pt1.y = dA1*ptInsp1.x + dB1;
				pt1.x = ptInsp1.x;
				pt2.y = dA1*ptInsp2.x + dB1;
				pt2.x = ptInsp2.x;
			}
			else
			{
				m_libVis.LineFitting(m_n1EdgeCnt, m_p1EdgeX, m_p1EdgeY, (int)(m_n1EdgeCnt*0.1), 2, &t, &dA1, &dB1);
				m_pData->m_Scan[nIndex].m_dFirstA = dA1; 
		        m_pData->m_Scan[nIndex].m_dFirstB = dB1; 

				pt1.x = ptInsp1.x;
				pt1.y = dA1*ptInsp1.x + dB1;
				pt2.x = ptInsp2.x;
				pt2.y = dA1*ptInsp2.x + dB1;
			}
		}

		m_pData->m_Scan[nIndex].m_dSecondA = 0;
		m_pData->m_Scan[nIndex].m_dSecondB = 0;

		if (m_n2EdgeCnt>5)
		{
			if (nFirstDir==DIR_LEFT_RIGHT || nFirstDir==DIR_RIGHT_LEFT)
			{
				m_libVis.LineFitting(m_n2EdgeCnt, m_p2EdgeY, m_p2EdgeX, (int)(m_n2EdgeCnt*0.1), 2, &t, &dA2, &dB2);
				m_pData->m_Scan[nIndex].m_dSecondA = dA2; 
		        m_pData->m_Scan[nIndex].m_dSecondB = dB2; 

				pt3.y = dA2*ptInsp1.x + dB2;
				pt3.x = ptInsp1.x;
				pt4.y = dA2*ptInsp2.x + dB2;
				pt4.x = ptInsp2.x;
			}
			else
			{
				m_libVis.LineFitting(m_n2EdgeCnt, m_p2EdgeX, m_p2EdgeY, (int)(m_n2EdgeCnt*0.1), 2, &t, &dA2, &dB2);
				m_pData->m_Scan[nIndex].m_dSecondA = dA2; 
		        m_pData->m_Scan[nIndex].m_dSecondB = dB2; 

				pt3.x = ptInsp1.x;
				pt3.y = dA2*ptInsp1.x + dB2;
				pt4.x = ptInsp2.x;
				pt4.y = dA2*ptInsp2.x + dB2;
			}
		}
		*/

		nMid = nSampleNum/2;
		if (bRet)
			dValue = fBadValue;
		else
			dValue = ARR_VALUE[nMid];

		fTransV = 0.0f;

//		if (m_pData->m_bRealTimeMode)
//		{
			if (fDiff>0.03f && dValue>0.06f)
			{
				if (dValue<fLowL) fTransV = dValue + 0.03f;
				if (dValue>fHighL) fTransV = dValue - 0.03f;
			}
			if (fDiff>0.1 && dValue>0.1)
			{
				if (dValue<fLowL)  fTransV = dValue + 0.06f;
				if (dValue>fHighL) fTransV = dValue - 0.06f;
			}
//		}	

		if (dValue<fLowL)  fTransV = dValue+DEF_TURNING_VALUE;
		if (dValue>fHighL) fTransV = dValue-DEF_TURNING_VALUE;

		if (dValue<0)  dValue = 0;
		if (fTransV<0) fTransV = 0;
	}

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	bRet = FALSE;
	if (fLowL>dValue || dValue>fHighL)
		bRet = TRUE;
	
	if (m_pData->m_bRealTimeMode==1)
	{
		bRet = FALSE;
		if (fLowL<=dValue && dValue<=fHighL)
			m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
		else if (fLowL<=fTransV && fTransV<=fHighL)
			m_pData->m_INSP[nIndex].m_dAnyValue = fTransV;
		else
			bRet = TRUE;
	}

	if (nInspType==INSP_MISMATCH)
	{
		long nTop, nBot, nLeft, nRight, nDec;
		long nGrayV, nRefV, nTGrayV, nApplyV;
		
		nRefV = GetGrayValueItem(nIndex, 1);
		nTGrayV = m_pData->m_INSP[nIndex].m_nTGrayV;
		nDec = m_pData->m_INSP[nIndex].m_bDecision;

		nApplyV = 0;
		if (BLACK_LEVEL<nRefV && nRefV<=WHITE_LEVEL && BLACK_LEVEL<nTGrayV && nTGrayV<=WHITE_LEVEL)
			nApplyV = (long)((nRefV - nTGrayV) * 0.5f);
		
		pt1.x = m_pData->m_INSP[nIndex].m_ptFirstMeas1.x;
		pt1.y = m_pData->m_INSP[nIndex].m_ptFirstMeas1.y;
		pt2.x = m_pData->m_INSP[nIndex].m_ptSecondMeas2.x;
		pt2.y = m_pData->m_INSP[nIndex].m_ptSecondMeas2.y;

		if (pt1.x>pt2.x) 
		{
			nLeft = (long)pt2.x;
			nRight = (long)pt1.x;
		}
		else
		{
			nLeft = (long)pt1.x;
			nRight = (long)pt2.x;
		}

		if (pt1.y>pt2.y) 
		{
			nTop = (long)pt2.y+1;
			nBot = (long)pt1.y-1;

			nBot2 = (long)pt1.y-2;
			nTop2 = (long)(nBot2 - fHighL/dScale - 2);			
		}
		else
		{
			nTop = (long)pt1.y+1;
			nBot = (long)pt2.y-1;

			nTop2 = (long)pt1.y+2;
			nBot2 = (long)(nTop2 + fHighL/dScale + 2);
		}

		if (nTop2<10) nTop2 = 10;
		if (nTop2>=m_nImageHeight-10) nTop2 = m_nImageHeight-10;
		if (nBot2<10) nBot2 = 10;
		if (nBot2>=m_nImageHeight-10) nBot2 = m_nImageHeight-10;
		
		nMisMatchL = (m_pData->m_Scan[nIndex].m_nSecondThresh + nApplyV);
		if (nMisMatchL>=WHITE_LEVEL)
			nMisMatchL = WHITE_LEVEL-1;
		if (nMisMatchL<BLACK_LEVEL)
			nMisMatchL = BLACK_LEVEL+1;

		m_pData->m_Scan[nIndex].m_nFirstThresh = nMisMatchL;

		//nGrayV2 = m_libVis.GetAveBrightness2(m_pRawImage, nLeft, nTop2, nRight, nBot2, m_nImageWidth);
		if (dValue>fHighL)// || nGrayV2>nMisMatchL)
		{
			nGrayV = m_libVis.GetAveBrightness(m_pRawImage, nLeft, nTop, nRight, nBot, m_nImageWidth);
			if (nGrayV<nMisMatchL)// && nGrayV2<nMisMatchL)
			{
				pt1 = m_pData->m_INSP[nIndex].m_ptFirstMeas1; 
				pt2 = m_pData->m_INSP[nIndex].m_ptFirstMeas2;
				pt3 = m_pData->m_INSP[nIndex].m_ptSecondMeas1;
				pt4 = m_pData->m_INSP[nIndex].m_ptSecondMeas2;

				if (nSecondDir==DIR_TOP_BOTTOM)
				{
					if (pt3.y>pt1.y)
					{
						m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt1;
						m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt2;
					}
					else
					{
						m_pData->m_INSP[nIndex].m_ptFirstMeas1 = pt3;
						m_pData->m_INSP[nIndex].m_ptFirstMeas2 = pt4;
					}
				}
				else
				{
					if (pt3.y>pt1.y)
					{
						m_pData->m_INSP[nIndex].m_ptFirstMeas1 = pt3;
						m_pData->m_INSP[nIndex].m_ptFirstMeas2 = pt4;
					}
					else
					{
						m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt1;
						m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt2;
					}
				}

				dValue = 0.0f;
				bRet = FALSE;
			}
			else
			{
				//if (dValue<=fHighL && nGrayV2>nMisMatchL)
				//	m_pData->m_INSP[nIndex].m_dAnyValue = (float)nGrayV2;

				bRet = TRUE;
			}
		}
	}

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	delete pEdgeX;
	delete pEdgeY;

	delete m_p1EdgeX;
	delete m_p1EdgeY;
	delete m_p2EdgeX;
	delete m_p2EdgeY;

	return bRet;
}


long CModuleLink::GetPosEdgeDetection(POINT2 ptInsp1, POINT2 ptInsp2, POINT2 ptInsp3, POINT2 ptInsp4, long nInspType, long nType, long nDir, long nPolality, long nThresh, 
									  long nMaxThr, long nMinThr, POINT2 *ptPos1, POINT2 *ptPos2, float *pEdgeX, float *pEdgeY, float *dA, float *dB)
{
	BOOL  bRet=TRUE;
    long  nCnt, nPos=-1;
	long  ProfileData[TEMP_MAX_NUM];
	float t;
	float fEdgePos;

	// Profile Data구하기
    nCnt=0;
	ptPos1->x = 0.0f;
	ptPos1->y = 0.0f;
	ptPos2->x = 0.0f;
	ptPos2->y = 0.0f;

	*dA = 0;
	*dB = 0;

	if (nType ==SCAN_PROJECT_DFDT)
	{
//		 nThresh = abs(nThresh-MEDIUM_LEVEL);
        if (nDir==DIR_LEFT_RIGHT || nDir==DIR_RIGHT_LEFT) 
		{
			m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);

			if (nInspType==INSP_MISMATCH)
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt2(ProfileData, (long)ptInsp1.x, (long)ptInsp4.x, nDir, nPolality, nThresh);
			else
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.x, (long)ptInsp4.x, nDir, nPolality, nThresh, nMaxThr, nMinThr);

			
			if (fEdgePos>10 && fEdgePos<m_nImageWidth-10)
			{
				//if (nInspType==INSP_MISMATCH)
    	            nCnt = GetLineAverEdgePos2(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, pEdgeX, pEdgeY);
				//else
	            //    nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, pEdgeX, pEdgeY);
	

				if (nCnt>5)
				{
					m_libVis.LineFitting(nCnt, pEdgeY, pEdgeX, (int)(nCnt*0.1), 2, &t, dA, dB);

					ptPos1->x = (*dA)*ptInsp1.y + (*dB);
					ptPos1->y = ptInsp1.y;
					ptPos2->x = (*dA)*ptInsp4.y + (*dB);
					ptPos2->y = ptInsp4.y;
				}
			}				 
		}
		else
		{
			/*
			nMid = (long)(ptInsp1.y+ptInsp4.y)/2;
			if (nInspType==INSP_MEASURE_SIDE_ONE && nDir==DIR_TOP_BOTTOM && nPolality==POL_BLACK_WHITE && nMid>m_nImageHeight/2)
			{
				nDir = DIR_BOTTOM_TOP;
				nPolality = POL_WHITE_BLACK;
				nTempThresh = 20;
				
				m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nDir, nPolality, nTempThresh);
				if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			    {
					nTemp = (long)(fEdgePos-5);
			    	fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)nTemp, nDir, nPolality, nThresh);
				}
			}
			else
			{
		*/
			m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
			if (nInspType==INSP_MISMATCH)
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt2(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nDir, nPolality, nThresh);
			else
				fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nDir, nPolality, nThresh, nMaxThr, nMinThr);

			
			if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			{
			//	if (nInspType==INSP_MISMATCH)
                    nCnt = GetLineAverEdgePos2(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, pEdgeX, pEdgeY);
			//	else
            //        nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nDir, nPolality, &fEdgePos, pEdgeX, pEdgeY);
	

	      		if (nCnt>5)
				{
					m_libVis.LineFitting(nCnt, pEdgeX, pEdgeY, (int)(nCnt*0.1), 2, &t, dA, dB);

					ptPos1->y = (*dA)*ptInsp1.x + (*dB);
					ptPos1->x = ptInsp1.x;
					ptPos2->y = (*dA)*ptInsp4.x + (*dB);
					ptPos2->x = ptInsp4.x;
				}
			}
		}
	}

	return nCnt;
}

#define PIXEL_INTER_SCAN      3   // shsong 인접하고 있는 엣지가 잡힐때 이부문을 3정도로 조절할 필요가 있다. MEB는 3임.
                                  // 결국에 3이 되면 라래의 GetLineAverEdgePos2와 완전동일한 루틴임.
long CModuleLink::GetLineAverEdgePos(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nPol, float *fEdge, float *EDGE_X, float *EDGE_Y)
{
	long i, j;
	long nCnt, nEdge, nMax, nDiff, nPos;
	float a, b, c, Temp_A, Temp_B, fEdgePos;
	double dSum;

	nPos = (long)(*fEdge);
	nCnt = 0;
	dSum = 0.0f;
	fEdgePos = 0.0f;

	if (nDir==DIR_LEFT_RIGHT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN; j<=nPos+PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[i*nWidth + (j+1)]-pImage[i*nWidth + (j-1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN; j<=nPos+PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[i*nWidth + (j-1)]-pImage[i*nWidth + (j+1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_RIGHT_LEFT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN; j>=nPos-PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[i*nWidth + (j-1)]-pImage[i*nWidth + (j+1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN; j>=nPos-PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[i*nWidth + (j+1)]-pImage[i*nWidth + (j-1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_TOP_BOTTOM)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN; j<=nPos+PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[(j+1)*nWidth + i]-pImage[(j-1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN; j<=nPos+PIXEL_INTER_SCAN; j++)
				{
					nDiff = pImage[(j-1)*nWidth + i]-pImage[(j+1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_BOTTOM_TOP)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN; j>=nPos-PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[(j-1)*nWidth + i]-pImage[(j+1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN; j>=nPos-PIXEL_INTER_SCAN; j--)
				{
					nDiff = pImage[(j+1)*nWidth + i]-pImage[(j-1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	*fEdge = 0;
	if (nCnt>0)
		*fEdge = (float)(dSum/nCnt);

	return nCnt;
}

#define PIXEL_INTER_SCAN2      3
long CModuleLink::GetLineAverEdgePos2(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nDir, long nPol, float *fEdge, float *EDGE_X, float *EDGE_Y)
{
	long i, j;
	long nCnt, nEdge, nMax, nDiff, nPos;
	float a, b, c, Temp_A, Temp_B, fEdgePos;
	double dSum;

	nPos = (long)(*fEdge);
	nCnt = 0;
	dSum = 0.0f;
	fEdgePos = 0.0f;

	if (nDir==DIR_LEFT_RIGHT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN2; j<=nPos+PIXEL_INTER_SCAN2; j++)
				{
					nDiff = pImage[i*nWidth + (j+1)]-pImage[i*nWidth + (j-1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN2; j<=nPos+PIXEL_INTER_SCAN2; j++)
				{
					nDiff = pImage[i*nWidth + (j-1)]-pImage[i*nWidth + (j+1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_RIGHT_LEFT)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN2; j>=nPos-PIXEL_INTER_SCAN2; j--)
				{
					nDiff = pImage[i*nWidth + (j-1)]-pImage[i*nWidth + (j+1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=y1; i<=y2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN2; j>=nPos-PIXEL_INTER_SCAN2; j--)
				{
					nDiff = pImage[i*nWidth + (j+1)]-pImage[i*nWidth + (j-1)];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nWidth-10)
				{
					a = pImage[i*nWidth + (nEdge+2)]; 
					b = pImage[i*nWidth + (nEdge-2)];  
					c = pImage[i*nWidth + nEdge];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[i*nWidth + (nEdge+3)]; 
					b = pImage[i*nWidth + (nEdge-1)];
					c = pImage[i*nWidth + (nEdge+1)];
					Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = fEdgePos;
					EDGE_Y[nCnt] = (float)i;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_TOP_BOTTOM)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN2; j<=nPos+PIXEL_INTER_SCAN2; j++)
				{
					nDiff = pImage[(j+1)*nWidth + i]-pImage[(j-1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos-PIXEL_INTER_SCAN2; j<=nPos+PIXEL_INTER_SCAN2; j++)
				{
					nDiff = pImage[(j-1)*nWidth + i]-pImage[(j+1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	if (nDir==DIR_BOTTOM_TOP)
	{
		if (nPol==POL_BLACK_WHITE)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN2; j>=nPos-PIXEL_INTER_SCAN2; j--)
				{
					nDiff = pImage[(j-1)*nWidth + i]-pImage[(j+1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}

		if (nPol==POL_WHITE_BLACK)
		{
			for(i=x1; i<=x2; i++)
			{
				nMax = 5;
				nEdge = -1;
				for(j=nPos+PIXEL_INTER_SCAN2; j>=nPos-PIXEL_INTER_SCAN2; j--)
				{
					nDiff = pImage[(j+1)*nWidth + i]-pImage[(j-1)*nWidth + i];
					if (nDiff>nMax) 
					{
						nEdge = j;
						nMax = nDiff;
					}
				}

				if (nEdge>10 && nEdge<nHeight-10)
				{
					a = pImage[(nEdge+2)*nWidth + i];
					b = pImage[(nEdge-2)*nWidth + i];
					c = pImage[nEdge*nWidth + i];
					Temp_A = (float)fabs((double)(a+b-2*c));

					a = pImage[(nEdge+3)*nWidth + i];
					b = pImage[(nEdge-1)*nWidth + i];
					c = pImage[(nEdge+1)*nWidth + i];
    				Temp_B = (float)fabs((double)(a+b-2*c));

					if ((Temp_A+Temp_B) != 0)
						fEdgePos = (float)nEdge + Temp_A/(Temp_A+Temp_B);	
					else
						fEdgePos = (float)nEdge;

					EDGE_X[nCnt] = (float)i;
					EDGE_Y[nCnt] = fEdgePos;

					dSum += fEdgePos;
					nCnt++;
				}
			}
		}
	}

	*fEdge = 0;
	if (nCnt>0)
		*fEdge = (float)(dSum/nCnt);

	return nCnt;
}

#define TAG_MIN_LIMIT   50
BOOL CModuleLink::GetTapePosision(LPBYTE pImage, long x1, long y1, long x2, long y2, long nWidth, long nHeight, long nThresh, float *fF, float *fS)
{
	BOOL bRet=FALSE;
	long i, j, nCnt, nDiff;
	long PROJ[5000];

	*fF = 0.0f;
	*fS = 0.0f;

	for(i=x1; i<=x2; i++)
	{
		PROJ[i] = 0;
		nCnt = 0;
		for(j=y1; j<y2; j++)
		{
			PROJ[i] += pImage[j*nWidth + i];
			nCnt++;
		}

		if (nCnt>0)
		    PROJ[i] /= nCnt;
	}

	for(i=x1+3; i<x2-3; i++)
	{
		nDiff = abs(PROJ[i+1]-PROJ[i-1]);
		if (nDiff>nThresh)
		{
			*fF = (float)i;
			bRet = TRUE;
			break;
		}
	}

	/*
	for(i=x2-3; i>x1+3; i--)
	{
		nDiff = abs(PROJ[i+1]-PROJ[i-1]);
		if (nDiff>nThresh)
		{
			*fS = (float)i;
			bRet = TRUE;
			break;
		}
	}
	*/

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         측정 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         직선 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectStraightLine()
{
	long i, nGOMType, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (INSP_SLINE<=nType && nType<=INSP_DATUM_C))
		    bRet = InspectStraightLineROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectStraightLineROI(long nIndex)
{
#if 0
	BOOL  bRet=FALSE, bRear;
	long  nInspType, nSecondPol;
	long  nSecondDir, nSecondThresh;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	POINT2 pt1, pt2, ptMinT, ptMaxT, ptTemp, ptCross;
	float fLowL, fHighL, fMidL, fF, fS, dTiltValue, fAvr;
	float t, dA, dB;
	float dScale, dValue, dOffsetV, fTransV;
	float dMinV, dMaxV, fRate;
	CString str, strTitle;
	long  i, j, nPos=-1;
	long  nCnt, nScanCnt, nH, nMid, nSampleNum, nTemp;
	long  ProfileData[TEMP_MAX_NUM];
	float fSum, fSize, fEdgePos;
	float EDGE_X[TEMP_MAX_NUM], EDGE_Y[TEMP_MAX_NUM], PX[TEMP_MAX_NUM], PY[TEMP_MAX_NUM];

	// 검사용 MODE로 전환
	m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환

	m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);

	nInspType = m_pData->m_INSP[nIndex].m_nInspectType;
	nSecondPol = m_pData->m_Scan[nIndex].m_nSecondPolDirType;
	nSecondDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
	nSecondThresh = (long)m_pData->m_INSP[nIndex].m_dSensity;
	bRear = m_pData->m_INSP[nIndex].m_bLocROI;
	dOffsetV = m_pData->m_INSP[nIndex].m_dOffsetV;
	fRate = m_pData->m_INSP[nIndex].m_bOverlapRate/10.0f;

	nSampleNum = (long)m_pData->m_INSP[nIndex].m_nSampleNum;
	if (nSampleNum<1) 
		nSampleNum=1;

	dValue = 0.0f;
	fTransV = 0.0f;
	dTiltValue = 0.0f;

	fLowL = m_pData->m_INSP[nIndex].m_dStandLowLimit;
	fHighL = m_pData->m_INSP[nIndex].m_dStandHighLimit;
	fMidL = (fLowL+fHighL)/2;

	str.Format(_T("%s"), m_pData->m_GOM[nIndex].m_strTitle);
	strTitle = WithoutSpaceToString(str);

	fSize = 1;
	nH = m_nImageHeight/2;
	nMid = (long)(ptInsp1.y + ptInsp4.y)/2;

	if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT)
	{
		if (bRear && nMid<nH)   dScale = m_pCAL->m_dScaleRT_X;
		if (bRear && nMid>=nH)  dScale = m_pCAL->m_dScaleRB_X;
		if (!bRear && nMid<nH)  dScale = m_pCAL->m_dScaleLT_X;
		if (!bRear && nMid>=nH) dScale = m_pCAL->m_dScaleLB_X;

		fSize = (float)(ptInsp4.y-ptInsp1.y)/(float)nSampleNum;
	}
	else
	{
		if (bRear) dScale = m_pCAL->m_dScaleR_Y;
		else  dScale = m_pCAL->m_dScaleL_Y;

		fSize = (float)(ptInsp4.x-ptInsp1.x)/(float)nSampleNum;
	}

	ptMinT.x = 100000;
	ptMinT.y = 100000;
	ptMaxT.x = 0;
	ptMaxT.y = 0;

	nScanCnt = 0;
	nTemp = 0;

	m_pData->m_Scan[nIndex].m_nFirstNum = 0;
	m_pData->m_Scan[nIndex].m_nSecondNum = 0;

	if (nSecondDir==DIR_LEFT_RIGHT || nSecondDir==DIR_RIGHT_LEFT) 
	{
		for(i=0; i<nSampleNum; i++)
		{
			if (i==0)
			{
				fF = ptInsp1.y;
				fS = fF+fSize + fSize*fRate*2.0f;
			}
			else if (i==nSampleNum-1)
			{
				fF = ptInsp4.y-fSize - fSize*fRate*2.0f;
				fS = ptInsp4.y;
			}
			else
			{
				fF = ptInsp1.y + i*fSize - fSize*fRate;
				fS = fF+fSize + fSize*fRate*2.0f;
			}

			if (fF<10) fF = 10;
			if (fS>m_nImageHeight-10) fS = (float)(m_nImageHeight-10);

			m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)fF, (long)ptInsp4.x, (long)fS, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.x, (long)ptInsp4.x, nSecondDir, nSecondPol, nSecondThresh);
			if (fEdgePos>10 && fEdgePos<m_nImageWidth-10)
			{
				nCnt = GetLineAverEdgePos(m_pRawImage, (long)ptInsp1.x, (long)fF, (long)ptInsp4.x, (long)fS, m_nImageWidth, m_nImageHeight, nSecondDir, nSecondPol, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					fSum = 0.0f;
					for(j=0; j<nCnt; j++)
					{
						PX[nScanCnt] = EDGE_X[j];
						PY[nScanCnt] = EDGE_Y[j];
						fSum += EDGE_X[j];
						nScanCnt++;
					}

					fAvr = fSum/nCnt;
					if (fAvr<ptMinT.x) 
					{
						ptMinT.y = (fF+fS)/2.0f;
						ptMinT.x = fAvr;
					}

					if (fAvr>ptMaxT.x) 
					{
						ptMaxT.y = (fF+fS)/2.0f;
						ptMaxT.x = fAvr;
					}

					m_pData->m_Scan[nIndex].m_dSecondPX[nTemp] = fAvr;
					m_pData->m_Scan[nIndex].m_dSecondPY[nTemp] = (fF+fS)/2.0f;
					nTemp++;
				}
			}
		}

		if (nScanCnt>0 && nTemp>0)
		{
			m_pData->m_Scan[nIndex].m_ptSecondMin = ptMinT;
			m_pData->m_Scan[nIndex].m_ptSecondMax = ptMaxT;

			if (nInspType==INSP_DATUM_B || nInspType==INSP_DATUM_C) 
			{
				m_libVis.LineFitting1(nScanCnt, PX, PY, (int)(nScanCnt*0.2f), 2, &t, &dA, &dB);
				if (nInspType==INSP_DATUM_C) 
				{
					dA = m_fMDatumA;
					t = m_fMDatumT;
				}

				if (t==0)
				{
					if (nSecondDir==DIR_LEFT_RIGHT)
						ptTemp = ptMinT;
					else
						ptTemp = ptMaxT;

					pt1.x = ptTemp.x;
					pt2.x = ptTemp.x;
					pt1.y = ptInsp1.y;
					pt2.y = ptInsp3.y;
				}
				else
				{
					ptMinT.x = 10000;
					ptMaxT.x = 0;

					for(i=0; i<nTemp; i++)
					{
						ptTemp.x = m_pData->m_Scan[nIndex].m_dSecondPX[i];
						ptTemp.y = m_pData->m_Scan[nIndex].m_dSecondPY[i];

						pt1.x = (ptInsp1.y-ptTemp.y + dA*ptTemp.x)/dA;
						pt1.y = ptInsp1.y;

						if (pt1.x<ptMinT.x) ptMinT=pt1;
						if (pt1.x>ptMaxT.x) ptMaxT=pt1;						 
					}

					if (nSecondDir==DIR_LEFT_RIGHT)
						ptTemp = ptMinT;
					else
						ptTemp = ptMaxT;

					pt1.x = (ptInsp1.y-ptTemp.y + dA*ptTemp.x)/dA;
					pt1.y = ptInsp1.y;
					pt2.x = (ptInsp3.y-ptTemp.y + dA*ptTemp.x)/dA;
					pt2.y = ptInsp3.y;
				}
			}
			else
			{
				m_libVis.LineFitting(nScanCnt, PY, PX, (int)(nScanCnt*0.2f), 2, &t, &dA, &dB);

				pt1.x = dA*ptInsp1.y + dB;
				pt1.y = ptInsp1.y;
				pt2.x = dA*ptInsp3.y + dB;
				pt2.y = ptInsp3.y;
			}

			// 직선도 계산 
			ptMinT.x = 10000;
			ptMaxT.x = 0;
			dMinV = 0;
			dMaxV = 0;

			for(i=0; i<nTemp; i++)
			{
				ptTemp.x =  m_pData->m_Scan[nIndex].m_dSecondPX[i];
				ptTemp.y =  m_pData->m_Scan[nIndex].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(ptTemp, pt1, pt2, &ptCross);
				fS = (float)m_libMath.GetDistance2D(ptTemp, ptCross);

				fF = ptTemp.x - ptCross.x;
				if (fF<ptMinT.x) 
				{
					ptMinT.x = fF;
					dMinV = fS;
				}

				if (fF>ptMaxT.x)
				{
					ptMaxT.x = fF;
					dMaxV = fS;
				}
			}

			dValue = (dMinV + dMaxV)*dScale;
			// 직선도 계산 
		}
	}
	else
	{
		for(i=0; i<nSampleNum; i++)
		{
			if (i==0)
			{
				fF = ptInsp1.x;
				fS = fF+fSize + fSize*fRate*2.0f;
			}
			else if (i==nSampleNum-1)
			{
				fF = ptInsp4.x-fSize - fSize*fRate*2.0f;
				fS = ptInsp4.x;
			}
			else
			{
				fF = ptInsp1.x + i*fSize - fSize*fRate;
				fS = fF+fSize + fSize*fRate*2.0f;
			}

			if (fF<10) fF = 10;
			if (fS>m_nImageWidth-10) fS = (float)(m_nImageWidth-10);

			m_libVis.GetProjectFtY(m_pRawImage, (long)fF, (long)ptInsp1.y, (long)fS, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProfileData);
			fEdgePos = m_libVis.GetPosDetectEdge_DfDt(ProfileData, (long)ptInsp1.y, (long)ptInsp4.y, nSecondDir, nSecondPol, nSecondThresh);
			if (fEdgePos>10 && fEdgePos<m_nImageHeight-10)
			{
				nCnt = GetLineAverEdgePos(m_pRawImage, (long)fF, (long)ptInsp1.y, (long)fS, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, nSecondDir, nSecondPol, &fEdgePos, EDGE_X, EDGE_Y);
				if (nCnt>0)
				{
					fSum = 0.0f;
					for(j=0; j<nCnt; j++)
					{
						PX[nScanCnt] = EDGE_X[j];
						PY[nScanCnt] = EDGE_Y[j];
						fSum += EDGE_Y[j];
						nScanCnt++;
					}

					fAvr = fSum/nCnt;
					if (fAvr<ptMinT.y) 
					{
						ptMinT.x = (fF+fS)/2.0f;
						ptMinT.y = fAvr;
					}

					if (fAvr>ptMaxT.y) 
					{
						ptMaxT.x = (fF+fS)/2.0f;
						ptMaxT.y = fAvr;
					}

					m_pData->m_Scan[nIndex].m_dSecondPX[nTemp] = (fF+fS)/2.0f;
					m_pData->m_Scan[nIndex].m_dSecondPY[nTemp] = fAvr; 
					nTemp++;
				}
			}
		}

		if (nScanCnt>0 && nTemp>0)
		{
			m_pData->m_Scan[nIndex].m_ptSecondMin = ptMinT;
			m_pData->m_Scan[nIndex].m_ptSecondMax = ptMaxT;

			if (nInspType==INSP_DATUM_B || nInspType==INSP_DATUM_C) 
			{
				m_libVis.LineFitting(nScanCnt, PX, PY, (int)(nScanCnt*0.2), 2, &t, &dA, &dB);
				if (nInspType==INSP_DATUM_C) 
				{
					dA = m_fMDatumA;
					t = m_fMDatumT;
				}

				if (t==0)
				{
					if (nSecondDir==DIR_TOP_BOTTOM)
						ptTemp = ptMaxT;
					else
						ptTemp = ptMinT;

					pt1.x = ptInsp1.x;
					pt2.x = ptInsp2.x;
					pt1.y = ptTemp.y;
					pt2.y = ptTemp.y;
				}
				else
				{
					ptMinT.y = 10000;
					ptMaxT.y = 0;

					for(i=0; i<nTemp; i++)
					{
						ptTemp.x = m_pData->m_Scan[nIndex].m_dSecondPX[i];
						ptTemp.y = m_pData->m_Scan[nIndex].m_dSecondPY[i];

						pt1.y = dA*(ptInsp1.x-ptTemp.x) + ptTemp.y;
						pt1.x = ptInsp1.x;

						if (pt1.y<ptMinT.y) ptMinT=pt1;
						if (pt1.y>ptMaxT.y) ptMaxT=pt1;						 
					}

					if (nSecondDir==DIR_TOP_BOTTOM)
						ptTemp = ptMaxT;
					else
						ptTemp = ptMinT;

					pt1.y = dA*(ptInsp1.x-ptTemp.x) + ptTemp.y;
					pt1.x = ptInsp1.x;
					pt2.y = dA*(ptInsp2.x-ptTemp.x) + ptTemp.y;
					pt2.x = ptInsp2.x;
				}
			}
			else
			{
				m_libVis.LineFitting(nScanCnt, PX, PY, (int)(nScanCnt*0.2), 2, &t, &dA, &dB);

				pt1.y = dA*ptInsp1.x + dB;
				pt1.x = ptInsp1.x;
				pt2.y = dA*ptInsp2.x + dB;
				pt2.x = ptInsp2.x;
			}

			// 직선도 계산 
			ptMinT.y = 0;
			ptMaxT.y = 0;
			dMinV = 0;
			dMaxV = 0;

			for(i=0; i<nTemp; i++)
			{
				ptTemp.x =  m_pData->m_Scan[nIndex].m_dSecondPX[i];
				ptTemp.y =  m_pData->m_Scan[nIndex].m_dSecondPY[i];

				m_libMath.GetCrossPointDotLine(ptTemp, pt1, pt2, &ptCross);
				fS = (float)m_libMath.GetDistance2D(ptTemp, ptCross);

				fF = ptTemp.y - ptCross.y;
				if (fF<ptMinT.y) 
				{
					ptMinT.y = fF;
					dMinV = fS;
				}

				if (fF>ptMaxT.y)
				{
					ptMaxT.y = fF;
					dMaxV = fS;
				}
			}

			dValue = (dMinV + dMaxV)*dScale;
			// 직선도 계산 
		}
	}

	m_pData->m_Scan[nIndex].m_nSecondNum = nTemp;
	m_pData->m_Scan[nIndex].m_dSecondA = dA; 
	m_pData->m_Scan[nIndex].m_dSecondB = dB; 
	m_pData->m_Scan[nIndex].m_dSecondT = t;

	m_pData->m_INSP[nIndex].m_ptSecondMeas1 = pt1;
	m_pData->m_INSP[nIndex].m_ptSecondMeas2 = pt2;

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;

	/*
	if (nInspType==INSP_STRAIGHTNESS)
	{
	if (dValue<fLowL || fHighL<dValue)
	m_pData->m_INSP[nIndex].m_bDecision = TRUE;
	}
	*/

#endif // 0

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         직선 검사        /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      면적 해석 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectBlobAnalysis()
{
	long i, nType;
	BOOL bRet=FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_BLOB_ANALYSIS)) 
		    bRet = InspectBlobAnalysisROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectBlobAnalysisROI(long nIndex)
{
	BOOL  bFeret, bRet=FALSE;
    long  i, j, x, y, nMinArea, nCnt, nType, nColor;
	long  nPolarity, nOption, nBlobCnt, nSensity;
	long nHalfW, nHalfH, nSign;
	float dMax, dValue;
	BLOB_STRUCT *pBS;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
    LPBYTE fmImage;
//	long *ProjY, *ProjY_T, *ProjX_T, *ProjX;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	nType = m_pData->m_INSP[nIndex].m_nInspectType;
	nMinArea = (long)m_pData->m_INSP[nIndex].m_dStandLowLimit;
	nSensity = m_pData->m_INSP[nIndex].m_nBlobThresh;
	nOption = m_pData->m_INSP[nIndex].m_nBlobOption;
	bFeret = m_pData->m_INSP[nIndex].m_bBlobFeret;
	nPolarity = m_pData->m_INSP[nIndex].m_nBlobPolarity;

	m_pData->m_INSP[nIndex].m_dCX = 0.0f;
	m_pData->m_INSP[nIndex].m_dCY = 0.0f;
	m_pData->m_INSP[nIndex].m_dAnyValue = 0.0f;

	m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	if (ptInsp1.x<0) ptInsp1.x=0;
	if (ptInsp1.y<0) ptInsp1.y=0;
	if (ptInsp4.x<0) ptInsp4.x=0;
	if (ptInsp4.y<0) ptInsp4.y=0;

	nHalfW = (long)(ptInsp4.x-ptInsp1.x)/2;
	nHalfH = (long)(ptInsp4.y-ptInsp1.y)/2;

 	fmImage = new unsigned char[nHalfH * nHalfW];
#if 0
	ProjY_T = new long[nHalfH];
	ProjY = new long[nHalfH];
	ProjX_T = new long[nHalfW];
	ProjX = new long[nHalfW];  
#endif // 0

		
	for(i=0; i<nHalfH; i++)
	{
		y = i*2 + (long)ptInsp1.y;
		if (y>=m_nImageHeight) 
			continue;

		for(j=0; j<nHalfW; j++)
		{
			x = j*2 + (long)ptInsp1.x;
			if (x<m_nImageWidth)
		    	fmImage[i*nHalfW + j] = m_pRawImage[y*m_nImageWidth + x];
		}
	}
 		
#if 0
	m_libVis.GetProjectFtX(fmImage,  0, 0, nHalfW-1, nHalfH-1, nHalfW, nHalfH, ProjX_T);
	for(i=2; i<nHalfW-2; i++)
		ProjX[i] = (ProjX_T[i-2]+ProjX_T[i-1]+ProjX_T[i]+ProjX_T[i+1]+ProjX_T[i+2]) / 5;

	m_libVis.GetProjectFtY(fmImage,  0, 0, nHalfW-1, nHalfH-1, nHalfW, nHalfH, ProjY_T);
	for(i=2; i<nHalfH-2; i++)
		ProjY[i] = (ProjY_T[i-2]+ProjY_T[i-1]+ProjY_T[i]+ProjY_T[i+1]+ProjY_T[i+2]) / 5;
#endif // 0

	if (nPolarity==POL_WHITE) 
	{
		nColor = WHITE_LEVEL;
		nSign = 1;
	}
	else 
	{
		nColor = BLACK_LEVEL;
		nSign = -1;
	}  

	for(i=2; i<nHalfH-2; i++)
    for(j=2; j<nHalfW-2; j++)
	{
#if 0
		if (ProjX[j]>ProjY[i]) 
			nThresh = ProjX[j] + (nSensity*nSign);
		else 
			nThresh = ProjY[i] + (nSensity*nSign);  
#endif // 0


		if (fmImage[i*nHalfW + j] > nSensity) 
			fmImage[i*nHalfW + j] = WHITE_LEVEL;
		else 
			fmImage[i*nHalfW + j] = BLACK_LEVEL;
	}

	//m_libVis.ImageBinarize(fmImage, fmImage, 0, 0, nHalfW-1, nHalfH-1, nHalfW, nHalfH, nThresh);
	pBS = (BLOB_STRUCT *)NBlobDll_BlobAnalysis(fmImage, 2, 2, nHalfW-3, nHalfH-3, nHalfW, nHalfH, 10, 90000000, nColor);

	nBlobCnt = NBlobDll_GetBlobCount();

	dMax = 0.0f;
	dValue = 0.0f;
	nCnt = 0;
	for(i=0; i<nBlobCnt; i++)
	{
		if (pBS[i].m_fArea>dMax) 
			dMax = pBS[i].m_fArea;

		if (pBS[i].m_fArea>nMinArea) 
		{
			if (nCnt<DEFECT_RECT_NUM)
			{
				m_pData->m_INSP[nIndex].m_rcPosFind[nCnt].left = (long)(pBS[i].m_fTopX*2 + ptInsp1.x);
				m_pData->m_INSP[nIndex].m_rcPosFind[nCnt].top = (long)(pBS[i].m_fTopY*2 + ptInsp1.y);
				m_pData->m_INSP[nIndex].m_rcPosFind[nCnt].right = (long)(pBS[i].m_fBotX*2 + ptInsp1.x);
				m_pData->m_INSP[nIndex].m_rcPosFind[nCnt].bottom = (long)(pBS[i].m_fBotY*2 + ptInsp1.y);
				nCnt++;
			}

			bRet = TRUE;
		}
	}

#if 0
	delete ProjY_T;
	delete ProjX_T;
	delete ProjY;
#endif // 0
	delete fmImage;

	if (dMax<=0) dMax = 0.0f;
	if (dMax>=1000) dMax = 999.0f;

	m_pData->m_INSP[nIndex].m_nDefectCnt = nCnt;
	m_pData->m_INSP[nIndex].m_dAnyValue = dMax;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;
	
	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      면적 해석 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      패턴 매칭 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
#define DEF_INSP_THICK    10
BOOL CModuleLink::InspectPatternMatch()
{
	long i, nType;
 	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && nType==INSP_PATTERN_MATCH) 
		    bRet = InspectPatternMatchROI(0, i);
	}

	return bRet;
}

BOOL CModuleLink::InspectPatternMatchROI(long nType, long nIndex)
{
	BOOL   bPatRet=TRUE, bRet=TRUE;
	LPBYTE pPAT, pHalfPAT, pHalfImage, pPatSepa, pPatPol;
	long   i, j, nPatWidth, nSideWidth, nSideHeight, nInspType;
	long   nPatNum, nPatHeight, nRate, nHalfWidth, nHalfHeight;
	long nHalfPWidth, nHalfPHeight, nCntX, nCntY, nPx, nPy;
	float  dPosX, dPosY, dScore1, dScore2, dMaxScore, dPatScore;
	CRect  rcSearchRect, rcBounds;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	dPatScore = m_pData->m_INSP[nIndex].m_dStandLowLimit;//(float)m_pData->m_INSP[nIndex].m_nPatScore;
	nPatWidth = m_pData->m_INSP[nIndex].m_nPatWidth;
	nPatHeight = m_pData->m_INSP[nIndex].m_nPatHeight;
	nRate = m_pData->m_INSP[nIndex].m_nPatSearchRate;
	nPatNum = m_pData->m_INSP[nIndex].m_nPatNum;
	nInspType = m_pData->m_INSP[nIndex].m_nBlobOption;
	m_pData->m_GOM[nIndex].m_strTitle;

	// 위치보정
	ptInsp1 = m_pData->m_GOM[nIndex].m_ptPoint1;
	ptInsp2 = m_pData->m_GOM[nIndex].m_ptPoint2;
	ptInsp3 = m_pData->m_GOM[nIndex].m_ptPoint3;
	ptInsp4 = m_pData->m_GOM[nIndex].m_ptPoint4;
	if (nType==0)
	{
        m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
		nRate = 170;
	}
	else
	{
		if (nRate>400) nRate=400;
		if (nRate<200) nRate=200;
	}
	// 위치보정

	nSideWidth  = (long)(nPatWidth/2 * nRate/100.0);
	nSideHeight = (long)(nPatHeight/2* nRate/100.0);

	rcSearchRect.left = (long)(ptInsp1.x+ptInsp4.x)/2 - nSideWidth;
	rcSearchRect.top =  (long)(ptInsp1.y+ptInsp4.y)/2 - nSideHeight;
	rcSearchRect.right = (long)(ptInsp1.x+ptInsp4.x)/2 + nSideWidth;
	rcSearchRect.bottom = (long)(ptInsp1.y+ptInsp4.y)/2 + nSideHeight;

	if (rcSearchRect.left<0) rcSearchRect.left = 0;
	if (rcSearchRect.top <0) rcSearchRect.top = 0;
	if (rcSearchRect.right >=m_nImageWidth)  rcSearchRect.right = m_nImageWidth-1;
	if (rcSearchRect.bottom>=m_nImageHeight) rcSearchRect.bottom= m_nImageHeight-1;

	m_pData->m_GOM[nIndex].m_dCX = (ptInsp1.x+ptInsp4.x)/2;
	m_pData->m_GOM[nIndex].m_dCY = (ptInsp1.y+ptInsp4.y)/2;
	dScore1 = 0.0f;

	if (nType==1)
	{
		pPAT = m_pData->m_INSP[nIndex].m_pPat[0];

		nHalfPWidth = nPatWidth/2 + 1;
		nHalfPHeight = nPatHeight/2 + 1;
		nPx = nHalfPWidth/2;
		nPy = nHalfPHeight/2;

		pHalfPAT = new unsigned char[nHalfPWidth*nHalfPHeight];
		pPatSepa = new unsigned char[nHalfPWidth*nHalfPHeight];
		memset(pPatSepa, 0, nHalfPWidth*nHalfPHeight);

		nHalfWidth = (rcSearchRect.right-rcSearchRect.left)/2 + 1;
		nHalfHeight = (rcSearchRect.bottom-rcSearchRect.top)/2 + 1;
		pHalfImage = new unsigned char[nHalfWidth*nHalfHeight];
		
		for(i=rcSearchRect.top, nCntY = 0; i<rcSearchRect.bottom; i+=2, nCntY++)
		for(j=rcSearchRect.left, nCntX=0; j<rcSearchRect.right; j+=2, nCntX++)
			pHalfImage[nCntY*nHalfWidth + nCntX] = m_pRawImage[i*m_nImageWidth + j];

		for(i=0, nCntY=0; i<nPatHeight; i+=2, nCntY++)
		for(j=0, nCntX=0; j<nPatWidth; j+=2, nCntX++)
			pHalfPAT[nCntY*nHalfPWidth + nCntX] = pPAT[i*nPatWidth + j];
		
		if (nInspType==0)
		{
			for(i=0; i<nHalfPHeight; i++)
			for(j=0; j<nHalfPWidth; j++)
				pPatSepa[i*nHalfPWidth + j] = pHalfPAT[i*nHalfPWidth + j];
		}
		if (nInspType==1)
		{
			for(i=0; i<nHalfPHeight; i++)
			for(j=0; j<nHalfPWidth; j++)
			{
				if (i<nPy || j<nPx) continue;
				pPatSepa[i*nHalfPWidth + j] = pHalfPAT[i*nHalfPWidth + j];
			}
		}
		if (nInspType==2)
		{
			for(i=0; i<nHalfPHeight; i++)
			for(j=0; j<nHalfPWidth; j++)
			{
				if (i>nPy || j<nPx) continue;
				pPatSepa[i*nHalfPWidth + j] = pHalfPAT[i*nHalfPWidth + j];
			}
		}
		if (nInspType==3)
		{
			for(i=0; i<nHalfPHeight; i++)
			for(j=0; j<nHalfPWidth; j++)
			{
				if (i<nPy || j>nPx) continue;
				pPatSepa[i*nHalfPWidth + j] = pHalfPAT[i*nHalfPWidth + j];
			}
		}
		if (nInspType==4)
		{
			for(i=0; i<nHalfPHeight; i++)
			for(j=0; j<nHalfPWidth; j++)
			{
				if (i>nPy || j>nPx) continue;
				pPatSepa[i*nHalfPWidth + j] = pHalfPAT[i*nHalfPWidth + j];
			}
		}
		
		dMaxScore = 0.0f;
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindShapePattern(pHalfImage, 0, 0, nHalfWidth-1, nHalfHeight-1,
	            								nHalfWidth, nHalfHeight, pPatSepa, nHalfPWidth, nHalfPHeight, 20, &dPosX, &dPosY, &dScore1);
			if (!bPatRet)
			{
				dMaxScore = dScore1;

				m_pData->m_GOM[nIndex].m_dCX;
				m_pData->m_GOM[nIndex].m_dCY;
				m_pData->m_INSP[nIndex].m_dCX = rcSearchRect.left + dPosX*2;
				m_pData->m_INSP[nIndex].m_dCY = rcSearchRect.top + dPosY*2;
				bRet = FALSE;
			}
		}

		delete pHalfPAT;
		delete pHalfImage;
		delete pPatSepa;
	}
	else
	{
		pPAT = m_pData->m_INSP[nIndex].m_pPat[0];

		nPx = nPatWidth/2;
		nPy = nPatHeight/2;
		dMaxScore = 0.0f;
	
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindPattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
												m_nImageWidth, m_nImageHeight, pPAT, nPatWidth, nPatHeight, 40, &dPosX, &dPosY, &dScore1);
			if (!bPatRet && dScore1>dMaxScore)
			{
				dMaxScore = dScore1;

				m_pData->m_INSP[nIndex].m_dCX = dPosX;
				m_pData->m_INSP[nIndex].m_dCY = dPosY;
			}

			if (dScore1>dPatScore) 
				bRet = FALSE;
		}

		if (nInspType<=0)
			goto goto_end;

		pPatSepa = new unsigned char[nPatWidth*nPatHeight];
		pPatPol = new unsigned char[nPatWidth*nPatHeight];
		memset(pPatSepa, 0, nPatWidth*nPatHeight);
		memset(pPatPol, 0, nPatWidth*nPatHeight);
		
		rcBounds.left = nPx-DEF_INSP_THICK;
		rcBounds.right = nPx+DEF_INSP_THICK;
		rcBounds.top = nPy-DEF_INSP_THICK;
		rcBounds.bottom = nPy+DEF_INSP_THICK;

		if (nInspType==1)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<rcBounds.top || j<rcBounds.left) continue;
				if (i>rcBounds.bottom && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<nPy+3 || j<nPx+3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==2)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>rcBounds.bottom || j<rcBounds.left) continue;
				if (i<rcBounds.top && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>nPy-3 || j<nPx+3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==3)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<rcBounds.top || j>rcBounds.right) continue;
				if (i>rcBounds.bottom && j>rcBounds.right) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i<nPy+3 || j>nPx-3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}
		if (nInspType==4)
		{
			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>rcBounds.bottom || j>rcBounds.right) continue;
				if (i<rcBounds.top && j<rcBounds.left) continue;

				pPatSepa[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}

			for(i=0; i<nPatHeight; i++)
			for(j=0; j<nPatWidth; j++)
			{
				if (i>nPy-3 || j>nPx-3) continue;
				pPatPol[i*nPatWidth + j] = pPAT[i*nPatWidth + j];
			}
		}


		bRet=TRUE;
		dMaxScore = 0.0f;
		dScore1 = 0;
		dScore2 =0;
		if (pPAT && nPatWidth>0 && nPatHeight>0)
		{
			bPatRet = NPatternDll_FindShapePattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
											    	m_nImageWidth, m_nImageHeight, pPatSepa, nPatWidth, nPatHeight, 20, &dPosX, &dPosY, &dScore1);
			if (!bPatRet && dScore1>50)
			{
				m_pData->m_INSP[nIndex].m_dCX = dPosX;
				m_pData->m_INSP[nIndex].m_dCY = dPosY;
			}

			bPatRet = NPatternDll_FindShapePattern(m_pRawImage, rcSearchRect.left, rcSearchRect.top, rcSearchRect.right, rcSearchRect.bottom,
											    	m_nImageWidth, m_nImageHeight, pPatPol, nPatWidth, nPatHeight, 20, &dPosX, &dPosY, &dScore2);
			if (dScore1>=dPatScore && dScore2>=dPatScore)
				bRet = FALSE;

			if (dScore1<dScore2)
				dMaxScore = dScore1;
			else
				dMaxScore = dScore2;
		}

//		m_libVis.SaveBitmapImage(_T("c:\\strFileName.bmp"), pPatSepa, nPatWidth, nPatHeight);
		delete pPatSepa;
		delete pPatPol;
	}

goto_end:
	m_pData->m_INSP[nIndex].m_dAnyValue = dMaxScore;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      패턴 매칭 검사      ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


BOOL CModuleLink::InspectEtc()
{
	long i, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_GRAY_VALUE))
		    bRet = InspectAverGrayValueROI(i);
	}

	return bRet;
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         평균밝기         ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectAverGrayValueROI(long nIndex)
{
	BOOL bRet = FALSE;
	float dValue;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	// 위치보정
    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	// 위치보정

	dValue = (float)m_libVis.GetAveBrightness(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth);

	if (dValue<m_pData->m_INSP[nIndex].m_dStandLowLimit || 
		dValue>m_pData->m_INSP[nIndex].m_dStandHighLimit) bRet=TRUE;

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         평균밝기         ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////       TAB FOLDING        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CModuleLink::InspectTabFolding()
{
	long i, nType;
	BOOL bRet = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && (nType==INSP_TAB_FOLDING))
		    bRet = InspectTabFoldingROI(i);
	}

	return bRet;
}

BOOL CModuleLink::InspectTabFoldingROI(long nIndex)
{
	BOOL bRet=FALSE;
	long nSign, nSize, nDiff, nPol, nType, nSlope, nCenX, nCenY;
	long i, j, nSum, nCnt;
	float dValue;
	CRect rcRect;
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	long *ProjX, *ProjY;

	// 검사용 MODE로 전환 
    m_pData->m_INSP[nIndex].m_bInspMode = TRUE;
	// 검사용 MODE로 전환 

	// 위치보정
    m_FuncEtc.GetOffsetAdjustment(m_pData, nIndex, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	// 위치보정

	nType = m_pData->m_INSP[nIndex].m_nBlobOption;
	nSlope = (long)m_pData->m_INSP[nIndex].m_dSensity;
	nPol = m_pData->m_INSP[nIndex].m_nBlobPolarity;
	nSize = m_pData->m_INSP[nIndex].m_bBlobFeret;

	ProjX = new long[m_nImageWidth];
	ProjY = new long[m_nImageHeight];

	if (nType==0) 
	{
		m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)(ptInsp4.y-DEF_TAB_FOLDING_RECT), (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjX);
		m_libVis.GetProjectFtY(m_pRawImage, (long)(ptInsp4.x-DEF_TAB_FOLDING_RECT), (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjY);
	}
	else if (nType==1) 
	{
		m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)(ptInsp1.y+DEF_TAB_FOLDING_RECT), m_nImageWidth, m_nImageHeight, ProjX);
		m_libVis.GetProjectFtY(m_pRawImage, (long)(ptInsp4.x-DEF_TAB_FOLDING_RECT), (long)ptInsp1.y, (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjY);
	}
	else if (nType==2) 
	{
		m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)(ptInsp4.y-DEF_TAB_FOLDING_RECT), (long)ptInsp4.x, (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjX);
		m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)(ptInsp1.x+DEF_TAB_FOLDING_RECT), (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjY);
	}
	else if (nType==3) 
	{
		m_libVis.GetProjectFtX(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)ptInsp4.x, (long)(ptInsp4.y-DEF_TAB_FOLDING_RECT), m_nImageWidth, m_nImageHeight, ProjX);
		m_libVis.GetProjectFtY(m_pRawImage, (long)ptInsp1.x, (long)ptInsp1.y, (long)(ptInsp1.x+DEF_TAB_FOLDING_RECT), (long)ptInsp4.y, m_nImageWidth, m_nImageHeight, ProjY);
	}

	nCenX = 0;
	nCenY = 0;

	nSign = 1;
	if (nPol) nSign = -1;

	if (nType==0)
	{
		for(i=(long)ptInsp1.x+2; i<(long)ptInsp4.x-2; i++)
		{
			nDiff = (ProjX[i-2]-ProjX[i+2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenX = i+5;
				break;
			}
		}

		for(i=(long)ptInsp1.y+2; i<(long)ptInsp4.y-2; i++)
		{
			nDiff = (ProjY[i-2]-ProjY[i+2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenY = i+5;
				break;
			}
		}

		m_pData->m_INSP[nIndex].m_rcPosFind[0].left = nCenX;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].top = nCenY;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].right = nCenX+nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].bottom = nCenY+nSize;
		m_pData->m_INSP[nIndex].m_nDefectCnt = 1;
	}

	if (nType==1)
	{
		for(i=(long)ptInsp1.x+2; i<(long)ptInsp4.x-2; i++)
		{
			nDiff = (ProjX[i-2]-ProjX[i+2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenX = i+5;
				break;
			}
		}

		for(i=(long)ptInsp4.y-2; i>(long)ptInsp1.y+2; i--)
		{
			nDiff = (ProjY[i+2]-ProjY[i-2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenY = i-5;
				break;
			}
		}

		m_pData->m_INSP[nIndex].m_rcPosFind[0].left = nCenX;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].top = nCenY-nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].right = nCenX+nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].bottom = nCenY;
		m_pData->m_INSP[nIndex].m_nDefectCnt = 1;
	}

	if (nType==2)
	{
		for(i=(long)ptInsp4.x-2; i>(long)ptInsp1.x+2; i--)
		{
			nDiff = (ProjX[i+2]-ProjX[i-2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenX = i-5;
				break;
			}
		}

		for(i=(long)ptInsp1.y+2; i<(long)ptInsp4.y-2; i++)
		{
			nDiff = (ProjY[i-2]-ProjY[i+2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenY = i+5;
				break;
			}
		}

		m_pData->m_INSP[nIndex].m_rcPosFind[0].left = nCenX-nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].top = nCenY;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].right = nCenX;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].bottom = nCenY+nSize;
		m_pData->m_INSP[nIndex].m_nDefectCnt = 1;
	}

	if (nType==3)
	{
		for(i=(long)ptInsp4.x-2; i>(long)ptInsp1.x+2; i--)
		{
			nDiff = (ProjX[i+2]-ProjX[i-2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenX = i-5;
				break;
			}
		}

		for(i=(long)ptInsp4.y-2; i>(long)ptInsp1.y+2; i--)
		{
			nDiff = (ProjY[i+2]-ProjY[i-2]) * nSign;
			if (nDiff>nSlope) 
			{
				nCenY = i-5;
				break;
			}
		}

		m_pData->m_INSP[nIndex].m_rcPosFind[0].left = nCenX-nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].top = nCenY-nSize;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].right = nCenX;
		m_pData->m_INSP[nIndex].m_rcPosFind[0].bottom = nCenY;
		m_pData->m_INSP[nIndex].m_nDefectCnt = 1;
	}
		
	rcRect = m_pData->m_INSP[nIndex].m_rcPosFind[0];

	nSum = 0;
	nCnt = 0;
	for(i=rcRect.top; i<=rcRect.bottom; i++)
    for(j=rcRect.left; j<=rcRect.right; j++)
	{
		nCnt++;
		if (m_pRawImage[i*m_nImageWidth + j] > 100) 
			nSum++;
	}

	//dValue = m_libVis.GetAveBrightness(m_pRawImage, rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, m_nImageWidth);
	if (nCnt>0)
	    dValue = nSum*100.0f/nCnt;

	if (dValue<m_pData->m_INSP[nIndex].m_dStandLowLimit || 
		dValue>m_pData->m_INSP[nIndex].m_dStandHighLimit) bRet=TRUE;

	if (dValue<=0) dValue = 0.0f;
	if (dValue>=1000) dValue = 999.0f;

	m_pData->m_INSP[nIndex].m_dAnyValue = dValue;
	m_pData->m_INSP[nIndex].m_bDecision = bRet;
		
	delete ProjX;
	delete ProjY;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////       TAB FOLDING        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         결과 판정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
long CModuleLink::GetIndexTitleROI(LPCTSTR strTitle)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, strTitle) || !m_pData->m_INSP[i].m_bLocROI) continue;

		nPos = i;
		break;
	}

	return nPos;
}

long CModuleLink::GetIndexROI(BOOL bLoc, LPCTSTR strTitle)
{
	long i, nPos;

	nPos = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, strTitle) || !bLoc) continue;

		nPos = i;
		break;
	}

	return nPos;
}

long CModuleLink::GetIndexTitleLinkROI(LPCTSTR strTitle)
{
	long i, nPos, nSel, nNum, nLocROI;

	nPos = -1;
	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		if (nSel>=0)
		{
			nLocROI = m_pData->m_INSP[nSel].m_bLocROI;
		    if (_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, strTitle)	|| 
				!m_pData->m_INSP[nSel].m_bLocROI) continue;

			nPos = i;
		    break;
		}
	}

	return nPos;
}

long CModuleLink::GetGrayValueItem(long nIndex, long nMode)
{
	long i, j, nSel, nRate, nCntL, nItem, nGray, nCnt, nGrayAvr, nType;
	long nThresh;
	long nLeft, nRight, nSumV;
	long HISTO[IMAGE_LEVEL];
	POINT2 ptInsp1, ptInsp2, ptInsp3, ptInsp4;
	CString str;

	nSel = (long)m_pData->m_INSP[nIndex].m_bOverlapRate-1;
	nType = (long)m_pData->m_INSP[nIndex].m_nInspectType;
	nRate = (m_pData->m_INSP[nIndex].m_bBlobFeret+1) * 10;

	if (nSel<0 || nType!=INSP_MISMATCH) 
		return 0;

	nCnt = 0;
	nItem = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		str.Format(_T("%s"), m_pData->m_GOM[i].m_strTitle);
		if (str == _T("A1L") || str == _T("A2L") || str == _T("A3L") || str == _T("A4L"))
		{
			if (m_pData->m_INSP[i].m_bLocROI)
			{
				if (nSel == nCnt) 
				{
					nItem = i;
					break;
				}
				nCnt++;
			}
			else
			{
				if (nSel == nCnt) 
				{
					nItem = i;
					break;
				}
				nCnt++;
			}
		}
	}
	
	

	nCnt = 0;
	nGray = 0;
	nGrayAvr = 0;
	if (nItem>=0)
	{
		if (nMode==1 && m_pData->m_INSP[nItem].m_bDecision)
			return nGrayAvr;

		m_FuncEtc.GetOffsetAdjustment(m_pData, nItem, &ptInsp1, &ptInsp2, &ptInsp3, &ptInsp4);
	
		nLeft = (long)(m_pData->m_INSP[nItem].m_ptFirstMeas1.x+2);
		nRight = (long)(m_pData->m_INSP[nItem].m_ptSecondMeas1.x-2);

		for(i=0; i<IMAGE_LEVEL; i++)
			HISTO[i] = 0;

		if (nLeft>0 && nLeft<m_nImageWidth && nRight>0 && nRight<m_nImageWidth)
		{
			for(i=(long)ptInsp1.y; i<(long)ptInsp4.y; i++)
			for(j=nLeft; j<nRight; j++)
			{
				nGray = m_pRawImage[i*m_nImageWidth + j];
				HISTO[nGray]++;
				nCnt++;
			}

			nCntL = (long)(nCnt * nRate/100.0f);
			nSumV = 0;
			nGray = 0;
			nThresh = 0;
			for(i=IMAGE_LEVEL-1; i>0; i--)
			{
				nSumV += HISTO[i];
				if (nSumV>nCntL)
				{
					nThresh = i;
					break;
				}
			}

			nSumV = 0;
			nCnt = 0;
			for(i=(long)ptInsp1.y; i<(long)ptInsp4.y; i++)
			for(j=nLeft; j<nRight; j++)
			{
				nGray = m_pRawImage[i*m_nImageWidth + j];
				if (nGray>=nThresh)
				{
					nSumV += m_pRawImage[i*m_nImageWidth + j];
					nCnt++;
				}
			}
		}
	}

	if (nCnt>0)
	    nGrayAvr = nSumV/nCnt;

	return nGrayAvr;
}

void CModuleLink::FineLaminateCal()
{
	long i, nROI, nNum, nSel, nLocROI;
	float fPrevV, fNextV, fDiff, fTemp, fL, fH, fMidL, fMidH, fRate;
	long nCnt;
	CString str, strTitle;

	fRate = 0.15f;

	nCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (m_pData->m_INSP[i].m_bDecision) nCnt++;
	}

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) && !m_pData->m_INSP[i].m_bLocROI) 
		{
			if (m_pData->m_INSP[i].m_nInspectType == INSP_MEASURE_SIDE_ONE || 
				m_pData->m_INSP[i].m_nInspectType == INSP_CAMERA_CAL ||
				m_pData->m_INSP[i].m_nInspectType == INSP_BLOB_ANALYSIS)
				continue;

			fL = m_pData->m_INSP[i].m_dStandLowLimit;
			fH = m_pData->m_INSP[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f - 0.03f;
			fMidH = (fL+fH)/2.0f + 0.03f;

			nROI = GetIndexTitleROI(m_pData->m_GOM[i].m_strTitle);
			if (nROI>=0)
			{
				////////////////////////// Fine ALG. 추가 
				fPrevV = m_pResB.m_dValue[nROI];
				fNextV = m_pData->m_INSP[i].m_dAnyValue;

//				fPrevV = m_pData->m_INSP[i].m_dAnyValue;
//				fNextV = m_pData->m_INSP[nROI].m_dAnyValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;
								
				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;

						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += 0.03f;
				if (fNextV>fMidH) fNextV -= 0.03f;

				m_pData->m_INSP[i].m_dAnyValue = fNextV;
				m_pData->m_INSP[i].m_bDecision = FALSE;
//				m_pData->m_INSP[nROI].m_dAnyValue = fNextV;
//				m_pData->m_INSP[nROI].m_bDecision = FALSE;
				////////////////////////// Fine ALG. 추가 
				
				fPrevV = m_pData->m_INSP[i].m_dAnyValue;
				fNextV = m_pData->m_INSP[nROI].m_dAnyValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;

						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += 0.03f;
				if (fNextV>fMidH) fNextV -= 0.03f;

//				m_pData->m_INSP[i].m_dAnyValue = fNextV;
//				m_pData->m_INSP[i].m_bDecision = FALSE;
				m_pData->m_INSP[nROI].m_dAnyValue = fNextV;
				m_pData->m_INSP[nROI].m_bDecision = FALSE;
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		if (nSel>=0 && nSel<GOM_MAX_NUM)
		{
			nLocROI = m_pData->m_INSP[nSel].m_bLocROI;
		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T("")) || nLocROI) continue;

			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			nROI = GetIndexTitleLinkROI(str);

			if (nROI>=0 && nROI<LINK_MAX_NUM)
			{
				fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
				fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;
				fMidL = (fL+fH)/2.0f - 0.03f;
				fMidH = (fL+fH)/2.0f + 0.03f;

				// STC, STA Fine ALG. 추가
				fPrevV = m_pResB.m_dLinkValue[nROI];
				fNextV = m_pData->m_Teach.m_LINK[i].m_dValue;
//				fPrevV = m_pData->m_Teach.m_LINK[i].m_dValue;
//				fNextV = m_pData->m_Teach.m_LINK[nROI].m_dValue;

				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
					}
				}
				
				if (fNextV<fMidL) fNextV += 0.03f;
				if (fNextV>fMidH) fNextV -= 0.03f;

				m_pData->m_Teach.m_LINK[i].m_dValue = fNextV;
				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
//				m_pData->m_Teach.m_LINK[nROI].m_dValue = fNextV;
//				m_pData->m_Teach.m_LINK[nROI].m_bDecision = FALSE;
				// STC, STA Fine ALG. 추가

				fPrevV = m_pData->m_Teach.m_LINK[i].m_dValue;
				fNextV = m_pData->m_Teach.m_LINK[nROI].m_dValue;
				if (fPrevV<fL || fPrevV>fH || fNextV<fL || fNextV>fH)
					continue;

				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*fRate;
				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
			        	fTemp = fDiff*0.01f;
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
					}
				}
				
				if (fNextV<fMidL) fNextV += 0.03f;
				if (fNextV>fMidH) fNextV -= 0.03f;

//				m_pData->m_Teach.m_LINK[i].m_dValue = fPrevV;
//				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
				m_pData->m_Teach.m_LINK[nROI].m_dValue = fNextV;
				m_pData->m_Teach.m_LINK[nROI].m_bDecision = FALSE;
			}
		}
	}
}

void CModuleLink::FineLaminateOneCell()
{
	long i, nNum, nSel1, nSel2, nLocROI;
	float fPrevV, fNextV, fDiff, fTemp, fL, fMidL, fH;
	long nType;
	CString str, strTitle;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		fPrevV = 0.0f;
		fNextV = 0.0f;

		str = m_pData->m_GOM[i].m_strTitle;
		strTitle = WithoutSpaceToString(str);
		if (_tcscmp(strTitle, _T(""))) 
		{
			nType = m_pData->m_INSP[i].m_nInspectType;
			if (nType == INSP_MEASURE_SIDE_ONE || 
				nType == INSP_SLINE ||
				nType == INSP_DATUM_B || 
				nType == INSP_DATUM_C) continue;

			fL = m_pData->m_INSP[i].m_dStandLowLimit;
			fH = m_pData->m_INSP[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f;

			fPrevV = m_pResB.m_dValue[i];
			fNextV = m_pData->m_INSP[i].m_dAnyValue;
			if (fPrevV>=fL && fPrevV<=fH && fNextV>=fL && fNextV<=fH)
			{
				fDiff = (float)fabs(fNextV - fPrevV);
				fTemp = fDiff*0.10f;

				if (fPrevV>fNextV)
				{
					fPrevV = fPrevV-fTemp;
					fNextV = fNextV+fTemp;
					if (fPrevV<fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
						fTemp = fDiff*0.02f;
					 
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}
				else
				{
					fPrevV = fPrevV+fTemp;
					fNextV = fNextV-fTemp;
					if (fPrevV>fNextV)
					{
	        			fDiff = (float)fabs(fNextV - fPrevV);
						fTemp = fDiff*0.02f;
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
					}
				}

				if (fNextV<fMidL) fNextV += DEF_TURNING_VALUE;
				if (fNextV>fMidL) fNextV -= DEF_TURNING_VALUE;

				m_pData->m_INSP[i].m_dAnyValue = fNextV;
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex2;
		if (nSel1>=0 && nSel2>=0 && nSel1<GOM_MAX_NUM && nSel2<GOM_MAX_NUM)
		{
			nLocROI = m_pData->m_INSP[nSel1].m_bLocROI;
			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			strTitle = WithoutSpaceToString(str);

		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T(""))) continue;

			fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
			fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;
			fMidL = (fL+fH)/2.0f;
			nType = m_pData->m_Teach.m_LINK[i].m_nLinkType;

			if (nType == TEACH_DIFF_GAP2GAP)
			{
				fDiff = m_pData->m_INSP[nSel1].m_dAnyValue - m_pData->m_INSP[nSel2].m_dAnyValue;
				m_pData->m_Teach.m_LINK[i].m_dValue = fDiff;

				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
				if (fDiff<fL || fH<fDiff)
				{
					m_pData->m_INSP[nSel1].m_bDecision = TRUE;
					m_pData->m_INSP[nSel2].m_bDecision = TRUE;
					m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;
				}
			}
			else
			{
				fPrevV = m_pResB.m_dLinkValue[i];
				fNextV = m_pData->m_Teach.m_LINK[i].m_dValue;
				fMidL = (fL+fH)/2.0f;

				if (fPrevV>=fL && fPrevV<=fH && fNextV>=fL && fNextV<=fH)
				{
					fDiff = (float)fabs(fNextV - fPrevV);
					fTemp = fDiff*0.15f;
					if (fPrevV>fNextV)
					{
						fPrevV = fPrevV-fTemp;
						fNextV = fNextV+fTemp;
						if (fPrevV<fNextV)
						{
	        				fDiff = (float)fabs(fNextV - fPrevV);
							fTemp = fDiff*0.02f;
							fPrevV = fPrevV+fTemp;
							fNextV = fNextV-fTemp;
						}
					}
					else
					{
						fPrevV = fPrevV+fTemp;
						fNextV = fNextV-fTemp;
						if (fPrevV>fNextV)
						{
	        				fDiff = (float)fabs(fNextV - fPrevV);
							fTemp = fDiff*0.02f;
							fPrevV = fPrevV-fTemp;
							fNextV = fNextV+fTemp;
						}
					}

					if (fNextV<fMidL) fNextV += DEF_TURNING_VALUE;
					if (fNextV>fMidL) fNextV -= DEF_TURNING_VALUE;

					m_pData->m_Teach.m_LINK[i].m_dValue = fNextV;
				}
			}
		}
	}
}

void CModuleLink::CalcOffsetData()
{
	long i, j, nNum, nSel1, nSel2, nType;
	float fL, fH, fMid, fMeasV;
	CString str, strTitle;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		str = m_pData->m_GOM[i].m_strTitle;
		strTitle = WithoutSpaceToString(str);
		if (_tcscmp(strTitle, _T(""))) 
		{
			nType = m_pData->m_INSP[i].m_nInspectType;
			if (nType == INSP_MEASURE_SIDE_ONE) 
				continue;

			for(j=0; j<m_nItemMaxNum; j++)
			{
				if (!_tcscmp(strTitle, m_strItemTitle[j]))
				{
					m_pData->m_INSP[i].m_bDecision = FALSE;
					fL = m_pData->m_INSP[i].m_dStandLowLimit;
					fH = m_pData->m_INSP[i].m_dStandHighLimit;
					fMid = (fL+fH) / 2.0f;
					fMeasV = m_pData->m_INSP[i].m_dAnyValue;

//					fMeasV = fMeasV*m_fSpecV[j] / (m_fSpecV[j]+m_fOffsetV[j]);
					if (fMeasV>0)
					{
						if (fMeasV<fMid)
							fMeasV += ((fMid - fL) * m_fSpecV[j]);
						else
							fMeasV += ((fMid - fH) * m_fSpecV[j]);

						fMeasV += m_fOffsetV[j];
					}
										
					if (fMeasV<0.0f) 
						fMeasV=0.0f;

					m_pData->m_INSP[i].m_dAnyValue = fMeasV;
					if (fMeasV<fL || fH<fMeasV)
						m_pData->m_INSP[i].m_bDecision = TRUE;

					break;
				}
			}
		}
	}

	nNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
		nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex2;
		if (nSel1>=0 && nSel1<GOM_MAX_NUM && nSel2>=0 && nSel2<=GOM_MAX_NUM)
		{
			str.Format(_T("%s"), m_pData->m_Teach.m_LINK[i].m_strTitle);
			strTitle = WithoutSpaceToString(str);

		    if (!_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T(""))) continue;

			for(j=0; j<m_nItemMaxNum; j++)
			{
				if (!_tcscmp(strTitle, m_strItemTitle[j]))
				{
					m_pData->m_INSP[nSel1].m_bDecision = FALSE;
					m_pData->m_INSP[nSel2].m_bDecision = FALSE;
					m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;

					fL = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
					fH = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;
					fMid = (fL+fH) / 2.0f;
					fMeasV = m_pData->m_Teach.m_LINK[i].m_dValue;

					//fMeasV = fMeasV*m_fSpecV[j] / (m_fSpecV[j]+m_fOffsetV[j]);
					if (fMeasV>0)
					{
						if (fMeasV<fMid)
							fMeasV += ((fMid - fL) * m_fSpecV[j]);
						else
							fMeasV += ((fMid - fH) * m_fSpecV[j]);

						fMeasV += m_fOffsetV[j];
					}
										
					if (fMeasV<0.0f) 
						fMeasV=0.0f;

					m_pData->m_Teach.m_LINK[i].m_dValue = fMeasV;
					if (fMeasV<fL || fH<fMeasV)
					{
						m_pData->m_INSP[nSel1].m_bDecision = TRUE;
						m_pData->m_INSP[nSel2].m_bDecision = TRUE;
						m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;
					}

					break;
				}
			}
		}
	}
}

long CModuleLink::ProcessResultDecision()
{
	BOOL bRet, bRet1, bRet2;
	long i, j, nSel1, nSel2, nFDec, nRDec;
	float fTempP, fLow, fHigh, dScale, fTempX, fTempY;

	fTempX = m_pData->m_Teach.m_fOffsetX;
	fTempY = m_pData->m_Teach.m_fOffsetY;

    dScale = m_pCAL->m_dScaleL_Y;
	fTempP = fabs(m_pData->m_nImageHeight-m_fObjectEdgeY)*dScale;

    m_pResData.m_dOffsetEPC = 0.0f;
    if (m_fObjectEdgeY>0.0f) 
	    m_pResData.m_dOffsetEPC = fTempP;
	
	bRet = FALSE;
	m_pResData.m_bFDataDec = FALSE;
	m_pResData.m_bRDataDec = FALSE;
	m_pResData.m_bFDecision = FALSE;
	m_pResData.m_bRDecision = FALSE;

	m_pResData.m_bMisMatchDecF = FALSE;
	m_pResData.m_bMisMatchDecR = FALSE;

	nFDec = 0;
	nRDec = 0;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		_tcscpy_s(m_pResData.m_strTitle[i], _T(""));
		m_pResData.m_bDecisionROI[i] = 0;
        m_pResData.m_nInspType[i] = 0;
		m_pResData.m_nPlotNum[i] = 0;

		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T(""))) 
		{
			_tcscpy_s(m_pResData.m_strTitle[i], m_pData->m_GOM[i].m_strTitle);
			m_pResData.m_bLocROI[i] = m_pData->m_INSP[i].m_bLocROI;
			m_pResData.m_bEjectROI[i] = m_pData->m_GOM[i].m_bEjectROI;
			m_pResData.m_bExecROI[i] = m_pData->m_GOM[i].m_bExecShowROI;
			m_pResData.m_nInspType[i] = m_pData->m_INSP[i].m_nInspectType;
			m_pResData.m_nPlotNum[i] = m_pData->m_INSP[i].m_nPlotSelNum;
			m_pResData.m_nOutNo[i] = m_pData->m_INSP[i].m_nOutputNo;
			m_pResData.m_dValue[i] = m_pData->m_INSP[i].m_dAnyValue;
			m_pResData.m_dLowLimit[i] = m_pData->m_INSP[i].m_dStandLowLimit;
			m_pResData.m_dStdSpec[i] = m_pData->m_INSP[i].m_dStdSpec;
			m_pResData.m_dToleSpec[i] = m_pData->m_INSP[i].m_dToleSpec;
			m_pResData.m_dHighLimit[i] = m_pData->m_INSP[i].m_dStandHighLimit;

#if _NG_DISP
			m_pResData.m_nSampleNum[i] = m_pData->m_Scan[i].m_nSecondNum;
			if (m_pData->m_Scan[i].m_nSecondNum>1)
			{
				for(j=0; j<m_pData->m_Scan[i].m_nSecondNum; j++)
				{
					m_pResData.m_ptFirstEdge[i][j].x = m_pData->m_Scan[i].m_dFirstPX[j];
					m_pResData.m_ptFirstEdge[i][j].y = m_pData->m_Scan[i].m_dFirstPY[j];
					m_pResData.m_ptSecondEdge[i][j].x = m_pData->m_Scan[i].m_dSecondPX[j];
					m_pResData.m_ptSecondEdge[i][j].y = m_pData->m_Scan[i].m_dSecondPY[j];
				}
			}
			else
			{
				m_pResData.m_ptFirstEdge[i][0].x = (m_pData->m_INSP[i].m_ptFirstMeas1.x + m_pData->m_INSP[i].m_ptFirstMeas2.x) / 2.0f;
				m_pResData.m_ptFirstEdge[i][0].y = (m_pData->m_INSP[i].m_ptFirstMeas1.y + m_pData->m_INSP[i].m_ptFirstMeas2.y) / 2.0f;
				m_pResData.m_ptSecondEdge[i][0].x = (m_pData->m_INSP[i].m_ptSecondMeas1.x + m_pData->m_INSP[i].m_ptSecondMeas2.x) / 2.0f;
				m_pResData.m_ptSecondEdge[i][0].y = (m_pData->m_INSP[i].m_ptSecondMeas1.y + m_pData->m_INSP[i].m_ptSecondMeas2.y) / 2.0f;
			}
						
			if (m_pResData.m_bLocROI[i])
			{
				m_pResData.m_dPosRoiX1[i] = m_pData->m_GOM[i].m_ptPoint1.x + m_pData->m_dDeltaX2;
				m_pResData.m_dPosRoiY1[i] = m_pData->m_GOM[i].m_ptPoint1.y + m_pData->m_dDeltaY2;
				m_pResData.m_dPosRoiX2[i] = m_pData->m_GOM[i].m_ptPoint4.x + m_pData->m_dDeltaX2;
				m_pResData.m_dPosRoiY2[i] = m_pData->m_GOM[i].m_ptPoint4.y + m_pData->m_dDeltaY2;
			}
			else
			{
				m_pResData.m_dPosRoiX1[i] = m_pData->m_GOM[i].m_ptPoint1.x + m_pData->m_dDeltaX;
				m_pResData.m_dPosRoiY1[i] = m_pData->m_GOM[i].m_ptPoint1.y + m_pData->m_dDeltaY;
				m_pResData.m_dPosRoiX2[i] = m_pData->m_GOM[i].m_ptPoint4.x + m_pData->m_dDeltaX;
				m_pResData.m_dPosRoiY2[i] = m_pData->m_GOM[i].m_ptPoint4.y + m_pData->m_dDeltaY;
			}
#endif

			if (m_pData->m_INSP[i].m_nInspectType == INSP_MEASURE_SIDE_ONE) continue;

			fLow = m_pData->m_INSP[i].m_dStandLowLimit;
			fHigh = m_pData->m_INSP[i].m_dStandHighLimit;
			
			m_pData->m_INSP[i].m_bDecision = FALSE;
			if (m_pResData.m_dValue[i]<fLow || fHigh<m_pResData.m_dValue[i])
				m_pData->m_INSP[i].m_bDecision = TRUE;

			m_pResData.m_bDecisionROI[i] = m_pData->m_INSP[i].m_bDecision;
			
			if (m_pData->m_INSP[i].m_nInspectType == INSP_MISMATCH) 
			{
				if (m_pResData.m_bExecROI[i] && m_pResData.m_bEjectROI[i])
				{
					if (!m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
				        m_pResData.m_bMisMatchDecF = TRUE;
					
					if (m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
					    m_pResData.m_bMisMatchDecR = TRUE;
				}
			}
			
			
			if (m_pResData.m_bExecROI[i])
			{
				if (!m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
					m_pResData.m_bFDataDec = TRUE;
				
				if (m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
					m_pResData.m_bRDataDec = TRUE;
			}
			else
			{
				m_pData->m_INSP[i].m_bDecision = TRUE;
				m_pResData.m_dValue[i] = 0.0f;
			}		 

			if (m_pResData.m_bEjectROI[i])
			{
				if (!m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
				{
					bRet = TRUE;
					nFDec = TRUE;
//					if (nFDec > m_pResData.m_nOutNo[i])
//						nFDec = m_pResData.m_nOutNo[i]; 
				}

				if (m_pResData.m_bLocROI[i] && m_pResData.m_bDecisionROI[i])
				{
					bRet = TRUE;
					nRDec = TRUE;
//					if (nRDec > m_pResData.m_nOutNo[i])
//						nRDec = m_pResData.m_nOutNo[i]; 					
				}
			}
		}
	}

	m_pResData.m_nLinkNum = m_pData->m_Teach.m_nLinkNum;
	for(i=0; i<m_pData->m_Teach.m_nLinkNum; i++)
	{
		_tcscpy_s(m_pResData.m_strLinkTitle[i], _T(""));
		m_pResData.m_nLinkPlotPos[i] = 0;
		m_pResData.m_nLinkPlotNum[i] = 0;

		bRet1 = m_pData->m_Teach.m_LINK[i].m_bSel1;
		bRet2 = m_pData->m_Teach.m_LINK[i].m_bSel2;

		if (_tcscmp(m_pData->m_Teach.m_LINK[i].m_strTitle, _T("")) && bRet1 && bRet2)
		{
			nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
			nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
			if (nSel1>=0 && nSel1<GOM_MAX_NUM && nSel2>=0 && nSel2<GOM_MAX_NUM)
			{
				_tcscpy_s(m_pResData.m_strLinkTitle[i], m_pData->m_Teach.m_LINK[i].m_strTitle);
				m_pResData.m_nLinkType[i] = m_pData->m_Teach.m_LINK[i].m_nLinkType;
				m_pResData.m_bLinkExecROI[i] = m_pData->m_Teach.m_LINK[i].m_bLinkExec;
#if 0
				if(m_pData->m_Teach.m_LINK[i].m_nLinkType == TEACH_DIST_1STLINE2LINE || m_pData->m_Teach.m_LINK[i].m_nLinkType == TEACH_DIST_2STLINE2LINE)				
					m_pResData.m_bLinkEjectROI[i] = FALSE;				
				else  
#endif // 0
				m_pResData.m_bLinkEjectROI[i] = m_pData->m_Teach.m_LINK[i].m_bEjectROI;
				
				m_pResData.m_bLinkLocROI[i] = m_pData->m_INSP[nSel1].m_bLocROI;
				m_pResData.m_nLinkOutNo[i] = m_pData->m_Teach.m_LINK[i].m_nOutputNo;
				m_pResData.m_nLinkPlotNum[i] = m_pData->m_Teach.m_LINK[i].m_nPlotSelNum;
				m_pResData.m_bLinkDecROI[i] = m_pData->m_Teach.m_LINK[i].m_bDecision;
				m_pResData.m_dLinkValue[i] = m_pData->m_Teach.m_LINK[i].m_dValue;
				m_pResData.m_dLinkValueS[i] = m_pData->m_Teach.m_LINK[i].m_dValueS;
				m_pResData.m_dLinkLowLimit[i] = m_pData->m_Teach.m_LINK[i].m_dStandLowLimit;
				m_pResData.m_dLinkStdSpec[i] = m_pData->m_Teach.m_LINK[i].m_dStdSpec;
				m_pResData.m_dLinkToleSpec[i] = m_pData->m_Teach.m_LINK[i].m_dToleSpec;
				m_pResData.m_dLinkHighLimit[i] = m_pData->m_Teach.m_LINK[i].m_dStandHighLimit;

				fLow = m_pResData.m_dLinkLowLimit[i];
				fHigh = m_pResData.m_dLinkHighLimit[i];

				m_pData->m_Teach.m_LINK[i].m_bDecision = FALSE;
				if (m_pResData.m_dLinkValue[i]<fLow || fHigh<m_pResData.m_dLinkValue[i])
				{
				    m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;
					//m_pResData.m_bDecisionROI[nSel1] = TRUE;    // shsong 
					//m_pResData.m_bDecisionROI[nSel2] = TRUE;
				}

				m_pResData.m_bLinkDecROI[i] = m_pData->m_Teach.m_LINK[i].m_bDecision;
				if (m_pResData.m_bLinkExecROI[i])
				{
					if (!m_pResData.m_bLinkLocROI[i] && m_pResData.m_bLinkDecROI[i])
						m_pResData.m_bFDataDec = TRUE;

					if (m_pResData.m_bLinkLocROI[i] && m_pResData.m_bLinkDecROI[i])
						m_pResData.m_bRDataDec = TRUE;
				}
				else
				{
					m_pData->m_Teach.m_LINK[i].m_bDecision = TRUE;
				    m_pResData.m_dLinkValue[i] = 0.0f;
				}

				if (m_pResData.m_bLinkEjectROI[i])
				{
					if (!m_pResData.m_bLinkLocROI[i] && m_pResData.m_bLinkDecROI[i])
					{
						bRet = TRUE;
						nFDec = TRUE;
//						if (nFDec > m_pResData.m_nLinkOutNo[i])
//					    	nFDec = m_pResData.m_nLinkOutNo[i]; 	
					}	
		    		if (m_pResData.m_bLinkLocROI[i] && m_pResData.m_bLinkDecROI[i])
					{
						bRet = TRUE;
						nRDec = TRUE;
//						if (nRDec > m_pResData.m_nLinkOutNo[i])
//					    	nRDec = m_pResData.m_nLinkOutNo[i]; 	
					}						
				}
			}
		}
	}

	//if (nFDec!=999)	m_pResData.m_bFDecision = nFDec;
	//if (nRDec!=999)	m_pResData.m_bRDecision = nRDec;

	m_pResData.m_bFDecision = nFDec;
	m_pResData.m_bRDecision = nRDec;

	m_pResData.m_bDecision = bRet;
	m_pData->m_bDecision = bRet;

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         결과 판정        ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



