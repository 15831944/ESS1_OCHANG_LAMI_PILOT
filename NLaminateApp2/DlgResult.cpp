// DlgResult.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgResult.h"
#include "NAppDll.h"
#include "NPlotDll.h"
#include "NGuiDll.h"
#include "MainFrm.h"
#include "math.h"
#include "afxdialogex.h"

// CDlgResult 대화 상자입니다.
extern CMainFrame     *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgResult, CDialogEx)

CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResult::IDD, pParent)
{

}

CDlgResult::~CDlgResult()
{
	NUiListDll_Close(LIST_RESULT_ID);
}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CDlgResult, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CDlgResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	NUiListDll_Open(LIST_RESULT_ID, (long)GetDlgItem(IDC_STATIC_LIST));
	ResetProductData();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgResult::SetTransLanguage(long nValue)
{
	InitListControls();
}

void CDlgResult::InitListControls()
{
	long i, j, nRow, nLngSel;
	CString str;
	INSP_RESULT_DATA  *pRes;

	NUiListDll_ClearCellAll(LIST_RESULT_ID);

	NUiListDll_SetRows(LIST_RESULT_ID, 50);
	NUiListDll_SetCols(LIST_RESULT_ID, 13);
	NUiListDll_SetRowHeight(LIST_RESULT_ID, 17);
	NUiListDll_SetTitleColor(LIST_RESULT_ID, RGB(180, 180, 180));
	NUiListDll_SetBackColor(LIST_RESULT_ID, RGB(255, 255, 255));
	NUiListDll_SetGridLineColor(LIST_RESULT_ID, RGB(200, 200, 200));
	NUiListDll_SetFontSize(LIST_RESULT_ID, 16);
	NUiListDll_SetEnableEdit(LIST_RESULT_ID, FALSE);

	NUiListDll_SetColWidth(LIST_RESULT_ID, 0, 140);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 1, 120);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 2, 70);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 3, 120);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 4, 80);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 5, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 6, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 7, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 8, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 9, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 10, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 11, 90);
	NUiListDll_SetColWidth(LIST_RESULT_ID, 12, 90);

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	if (nLngSel==0) 
	{
		NUiListDll_SetText(LIST_RESULT_ID, 0, 0, _T(" 판정 / 검사항목 "));
		NUiListDll_SetText(LIST_RESULT_ID, 1, 0, _T(" 카메라# "));
		NUiListDll_SetText(LIST_RESULT_ID, 2, 0, _T(" 전/후 "));
		NUiListDll_SetText(LIST_RESULT_ID, 3, 0, _T(" 스펙 "));
		NUiListDll_SetText(LIST_RESULT_ID, 4, 0, _T(" 측정치 "));
		NUiListDll_SetText(LIST_RESULT_ID, 5, 0, _T(" 불량수 "));
		NUiListDll_SetText(LIST_RESULT_ID, 6, 0, _T(" 수율 "));
		NUiListDll_SetText(LIST_RESULT_ID, 7, 0, _T(" 평균치 "));
		NUiListDll_SetText(LIST_RESULT_ID, 8, 0, _T(" 표준편차 "));
		NUiListDll_SetText(LIST_RESULT_ID, 9, 0, _T(" 최소값 "));
		NUiListDll_SetText(LIST_RESULT_ID, 10, 0, _T(" 최대값 "));
		NUiListDll_SetText(LIST_RESULT_ID, 11, 0, _T(" CP "));
		NUiListDll_SetText(LIST_RESULT_ID, 12, 0, _T(" CPK "));
	}
	else if (nLngSel==1) 
	{
		NUiListDll_SetText(LIST_RESULT_ID, 0, 0, _T(" DEC. / TITLE "));
		NUiListDll_SetText(LIST_RESULT_ID, 1, 0, _T(" CAMERA# "));
		NUiListDll_SetText(LIST_RESULT_ID, 2, 0, _T(" F/R "));
		NUiListDll_SetText(LIST_RESULT_ID, 3, 0, _T(" Spec. "));
		NUiListDll_SetText(LIST_RESULT_ID, 4, 0, _T(" Result. "));
		NUiListDll_SetText(LIST_RESULT_ID, 5, 0, _T(" Bad Cnt. "));
		NUiListDll_SetText(LIST_RESULT_ID, 6, 0, _T(" Yield "));
		NUiListDll_SetText(LIST_RESULT_ID, 7, 0, _T(" Average "));
		NUiListDll_SetText(LIST_RESULT_ID, 8, 0, _T(" STD. DEV. "));
		NUiListDll_SetText(LIST_RESULT_ID, 9, 0, _T(" Min Value "));
		NUiListDll_SetText(LIST_RESULT_ID, 10, 0, _T(" Max Value "));
		NUiListDll_SetText(LIST_RESULT_ID, 11, 0, _T(" CP "));
		NUiListDll_SetText(LIST_RESULT_ID, 12, 0, _T(" CPK "));
	}
	else
	{
		NUiListDll_SetText(LIST_RESULT_ID, 0, 0, _T(" 判定 / 檢査项目 "));
		NUiListDll_SetText(LIST_RESULT_ID, 1, 0, _T(" 位置# "));
		NUiListDll_SetText(LIST_RESULT_ID, 2, 0, _T(" 前面/后面 "));
		NUiListDll_SetText(LIST_RESULT_ID, 3, 0, _T(" 规格 "));
		NUiListDll_SetText(LIST_RESULT_ID, 4, 0, _T(" 测量值 "));
		NUiListDll_SetText(LIST_RESULT_ID, 5, 0, _T(" 不良数量 "));
		NUiListDll_SetText(LIST_RESULT_ID, 6, 0, _T(" 产量 "));
		NUiListDll_SetText(LIST_RESULT_ID, 7, 0, _T(" 平均值 "));
		NUiListDll_SetText(LIST_RESULT_ID, 8, 0, _T(" 平均偏差 "));
		NUiListDll_SetText(LIST_RESULT_ID, 9, 0, _T(" 最小值 "));
		NUiListDll_SetText(LIST_RESULT_ID, 10, 0, _T(" 最大值 "));
		NUiListDll_SetText(LIST_RESULT_ID, 11, 0, _T(" CP "));
		NUiListDll_SetText(LIST_RESULT_ID, 12, 0, _T(" CPK "));
	}

	nRow = 1;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		pRes = m_pMainFrm->m_pRes[i];
		//if (!m_pMainFrm->m_Setting.m_bExecInsp[i]) continue;

		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_bLocROI[j]) continue;

			str.Format(_T("   %s"), pRes->m_strTitle[j]);
            NUiListDll_SetText(LIST_RESULT_ID, 0, nRow, str);

			nRow++;
		}
		
		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (pRes->m_bLinkLocROI[j]) continue;

			str.Format(_T("   %s"), pRes->m_strLinkTitle[j]);
            NUiListDll_SetText(LIST_RESULT_ID, 0, nRow, str);

			nRow++;
		}

		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (!pRes->m_bLocROI[j]) continue;

			str.Format(_T("   %s"), pRes->m_strTitle[j]);
            NUiListDll_SetText(LIST_RESULT_ID, 0, nRow, str);

			nRow++;
		}

		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (!pRes->m_bLinkLocROI[j]) continue;

			str.Format(_T("   %s"), pRes->m_strLinkTitle[j]);
            NUiListDll_SetText(LIST_RESULT_ID, 0, nRow, str);

			nRow++;
		}
	}

	for(i=nRow; i<50; i++)
		NUiListDll_SetText(LIST_RESULT_ID, 0, i, _T(""));

	NUiListDll_SetRow(LIST_RESULT_ID, nRow);
	NUiListDll_Invalidate(LIST_RESULT_ID);

	UpdateData(false);
}

void CDlgResult::UpdateResultList()
{
	long i, j, nOrderNum;
	INSP_RESULT_DATA  *pRes;
	
	nOrderNum = 0;
	m_nRow = 1;
	for(i=0; i<CAM_MAX_NUM; i++)
	{
		pRes = m_pMainFrm->m_pRes[i];
		//if (!m_pMainFrm->m_Setting.m_bExecInsp[i]) continue;

		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_bLocROI[j]) continue;

			if (nOrderNum<GOM_MAX_NUM)
			{
     			WriteProductData(i,  FALSE, nOrderNum, j);
				nOrderNum++;
			}
		}

		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (pRes->m_bLinkLocROI[j]) continue;

			if (nOrderNum<GOM_MAX_NUM)
			{
				WriteProductData(i,  TRUE, nOrderNum, j);
				nOrderNum++;
			}
		}

		for(j=0; j<GOM_MAX_NUM; j++)
		{
			if (pRes->m_nInspType[j]==INSP_MEASURE_SIDE_ONE) continue;
			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (!pRes->m_bLocROI[j]) continue;
						
			if (nOrderNum<GOM_MAX_NUM)
			{
				WriteProductData(i, FALSE, nOrderNum, j);
	    		nOrderNum++;
			}
		}

		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (!pRes->m_bLinkLocROI[j]) continue;

			if (nOrderNum<GOM_MAX_NUM)
			{
				WriteProductData(i, TRUE, nOrderNum, j);
				nOrderNum++;
			}
		}
	}

	NUiListDll_Invalidate(LIST_RESULT_ID);
}

void CDlgResult::WriteProductData(long nCAM, BOOL bLink, long nOrderNum, long nROI)
{
	BOOL bDec, bDecROI, bLocPos;
	long i, nLngSel, nSel, nDataNum, nEnd, nBadCnt, nIndex, nGoodCnt, nType;
	float fMin, fMax, fDiff, fSum, fAvr, fDiv, fValue;
	float fStdDev, fCp, fCpl, fCpu, fCpk, fYield;
	float fValueS, fSumS, fAvrS, fCpS, fCpkS, fStdDevS;
	float dLowL, dHighL, dValueROI;
	CString str, strTitle;
	INSP_RESULT_DATA  *pRes;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;

	pRes = m_pMainFrm->m_pRes[nCAM];

	//////////////////////////////////////////////////////
	///////////////      RESULT DATA       ///////////////
	//////////////////////////////////////////////////////
	nDataNum = m_pMainFrm->m_ProdData.m_nTotalCnt;
	nIndex = m_pMainFrm->m_ProdData.m_nIndexPnt;
	
	if (!bLink)
	{
		nType = pRes->m_nInspType[nROI];
		dValueROI = pRes->m_dValue[nROI];
		m_pMainFrm->m_ProdData.m_bDec[nCAM][nOrderNum][nIndex] = pRes->m_bDecisionROI[nROI];
		m_pMainFrm->m_ProdData.m_fValue[nCAM][nOrderNum][nIndex] = dValueROI;

		strTitle.Format(_T("%s"), pRes->m_strTitle[nROI]);
		dLowL = pRes->m_dLowLimit[nROI];
		dHighL = pRes->m_dHighLimit[nROI];
		bDecROI = pRes->m_bDecisionROI[nROI];
		bLocPos = pRes->m_bLocROI[nROI];

		g_DataRT.m_InspCnt[nOrderNum]++;
		if (pRes->m_bDecisionROI[nROI])
		    g_DataRT.m_BadCnt[nOrderNum]++;
	}
	else
	{
		nType = pRes->m_nLinkType[nROI];
		dValueROI = pRes->m_dLinkValue[nROI];
		m_pMainFrm->m_ProdData.m_bDec[nCAM][nOrderNum][nIndex] = pRes->m_bLinkDecROI[nROI];
		m_pMainFrm->m_ProdData.m_fValue[nCAM][nOrderNum][nIndex] = dValueROI;

		/*
		if (nType == TEACH_SLINE_SURFACE)
			m_pMainFrm->m_ProdData.m_fValueS[nCAM][nOrderNum][nIndex] = pRes->m_dLinkValueS[nROI];
		*/

		strTitle.Format(_T("%s"), pRes->m_strLinkTitle[nROI]);
		dLowL = pRes->m_dLinkLowLimit[nROI];
		dHighL = pRes->m_dLinkHighLimit[nROI];
		bDecROI = pRes->m_bLinkDecROI[nROI];
		bLocPos = pRes->m_bLinkLocROI[nROI];

		g_DataRT.m_InspCnt[nOrderNum]++;
		if (pRes->m_bLinkDecROI[nROI])
		    g_DataRT.m_BadCnt[nOrderNum]++;
	}

	m_pMainFrm->m_ProdData.m_InspType[nCAM][nOrderNum] = nType;

	nEnd = PROD_MAXDATA_NUM;
	if (nDataNum<PROD_MAXDATA_NUM)
		nEnd =  nDataNum;

	fMin = 0;
	fMax = 0;
	nBadCnt = 0;
	nGoodCnt = 0;
	fSum = 0;
	fSumS = 0;
	fAvr = 0;
	fAvrS = 0;
	fStdDev = 0;
	fStdDevS = 0;
	fYield = 0;
	fCp = 0;
	fCpS = 0;
	fCpl = 0;
	fCpu = 0;
	fCpk = 0;
	fCpkS = 0;

	if (nDataNum>0)
	{
		fMin = 1000;
		fMax = -1000;

		for(i=0; i<nEnd; i++)
		{
			fValue = m_pMainFrm->m_ProdData.m_fValue[nCAM][nOrderNum][i];
			fValueS = m_pMainFrm->m_ProdData.m_fValueS[nCAM][nOrderNum][i];
			bDec = m_pMainFrm->m_ProdData.m_bDec[nCAM][nOrderNum][i];

			if (bDec) 
				nBadCnt++;
			else
			{
				fSum += fValue;
				if (fValue<fMin) fMin = fValue;
		    	if (fValue>fMax) fMax = fValue;

				/*
				if (nType == TEACH_SLINE_SURFACE)
				{
					fSumS += fValueS;
					if (fValueS<fMin) fMin = fValue;
		         	if (fValueS>fMax) fMax = fValue;
				}
				*/

				nGoodCnt++;
			}
		}
	
		if (nGoodCnt>10)
		{
		    fAvr = fSum/nGoodCnt;
			fAvrS = fSumS/nGoodCnt;

			fSum = 0.0f;
			fSumS = 0.0f;
			for(i=0; i<nEnd; i++)
			{
				bDec = m_pMainFrm->m_ProdData.m_bDec[nCAM][nOrderNum][i];
				if (!bDec)
				{
					fDiff = m_pMainFrm->m_ProdData.m_fValue[nCAM][nOrderNum][i] - fAvr;
					fSum += (fDiff*fDiff);

					/*
					if (nType == TEACH_SLINE_SURFACE)
					{
						fDiff = m_pMainFrm->m_ProdData.m_fValueS[nCAM][nOrderNum][i] - fAvrS;
						fSumS += (fDiff*fDiff);
					}
					*/
				}
			}

			//fYield = (float)(nDataNum-nBadCnt)/nDataNum*100.0f;
			nBadCnt = g_DataRT.m_BadCnt[nOrderNum];
			if (g_DataRT.m_InspCnt[nOrderNum]>0)
			    fYield = (float)(g_DataRT.m_InspCnt[nOrderNum] - nBadCnt) / g_DataRT.m_InspCnt[nOrderNum] * 100.0f;

		    fDiff = fSum/nGoodCnt;
			fStdDev = (float)sqrt(fDiff);

			fCp = 0.0f;
			fCpk = 0.0f;
			fDiv = (float)fabs(fStdDev);

			if (fabs(fDiv)>0.00001)
			{
				fCp = (dHighL-dLowL)/(6*fStdDev);
				fCpl = fabs((dLowL-fAvr)/(3*fStdDev));
				fCpu = fabs((dHighL-fAvr)/(3*fStdDev));
				fCpk = (fCpl>fCpu) ? fCpu : fCpl;
			}

			/*
			if (nType == TEACH_SLINE_SURFACE)
			{
				fDiff = fSumS/nGoodCnt;
				fStdDevS = (float)sqrt(fDiff);

				fCpS = 0.0f;
				fCpkS = 0.0f;
				fDiv = (float)fabs(fStdDevS);

				if (fabs(fDiv)>0.00001)
				{
					fCpS = (dHighL-dLowL)/(6*fStdDevS);
					fCpl = fabs((dLowL-fAvr)/(3*fStdDevS));
					fCpu = fabs((dHighL-fAvr)/(3*fStdDevS));
					fCpkS = (fCpl>fCpu) ? fCpu : fCpl;
				}
			}
			*/

		}
	}

	nSel = 1;
	if (bDecROI)
		nSel = 2;

	str.Format(_T("   %s"), strTitle);
    NUiListDll_SetText(LIST_RESULT_ID, 0, m_nRow, str);

	NUiListDll_SetCellBackColor(LIST_RESULT_ID, 0, m_nRow, RGB(170, 255, 170));
	if (bDecROI) 
		NUiListDll_SetCellBackColor(LIST_RESULT_ID, 0, m_nRow, RGB(255, 170, 170));

	str.Format(_T(" %d. "), nCAM+1);
    NUiListDll_SetText(LIST_RESULT_ID, 1, m_nRow, str);
	
	if (nLngSel==0) str = _T("전");
	else if (nLngSel==1) str = _T("FRONT");
	else str = _T("前");

	if (bLocPos)
	{
		if (nLngSel==0) str = _T("후");
		else if (nLngSel==1) str = _T("REAR");
		else str = _T("後");
	}

    NUiListDll_SetText(LIST_RESULT_ID, 2, m_nRow, str);

	str.Format(_T("%5.2f~%5.2f"), dLowL, dHighL);
    NUiListDll_SetText(LIST_RESULT_ID, 3, m_nRow, str);

	str.Format(_T("%5.2f"), dValueROI);
    NUiListDll_SetText(LIST_RESULT_ID, 4, m_nRow, str);
	
	str.Format(_T("%d"), nBadCnt);
	//str.Format(_T("%d"), g_DataRT.m_BadCnt[nOrderNum]);	
    NUiListDll_SetText(LIST_RESULT_ID, 5, m_nRow, str);

	str.Format(_T("%5.2f"), fYield);
    NUiListDll_SetText(LIST_RESULT_ID, 6, m_nRow, str);

	str.Format(_T("%5.2f"), fAvr);
//	if (nType == TEACH_SLINE_SURFACE) 
//		str.Format(_T("%5.2f | %5.2f"), fAvr, fAvrS);
    NUiListDll_SetText(LIST_RESULT_ID, 7, m_nRow, str);

	str.Format(_T("%5.2f"), fStdDev);
//	if (nType == TEACH_SLINE_SURFACE) 
//		str.Format(_T("%5.2f | %5.2f"), fStdDev, fStdDevS);
    NUiListDll_SetText(LIST_RESULT_ID, 8, m_nRow, str);

	str.Format(_T("%5.2f"), fMin);
    NUiListDll_SetText(LIST_RESULT_ID, 9, m_nRow, str);

	str.Format(_T("%5.2f"), fMax);
    NUiListDll_SetText(LIST_RESULT_ID, 10, m_nRow, str);

	str.Format(_T("%5.2f"), fCp);
//	if (nType == TEACH_SLINE_SURFACE) 
//		str.Format(_T("%5.2f | %5.2f"), fCp, fCpS);
    NUiListDll_SetText(LIST_RESULT_ID, 11, m_nRow, str);

	str.Format(_T("%5.2f"), fCpk);
//	if (nType == TEACH_SLINE_SURFACE) 
//		str.Format(_T("%5.2f | %5.2f"), fCpk, fCpkS);
    NUiListDll_SetText(LIST_RESULT_ID, 12, m_nRow, str);

	m_nRow++;
	//////////////////////////////////////////////////////
	///////////////      RESULT DATA       ///////////////
	//////////////////////////////////////////////////////
}

void CDlgResult::ResetProductData()
{
	long i, j, k;

	m_pMainFrm->m_ProdData.m_nTotalCnt = 0;
	m_pMainFrm->m_ProdData.m_nIndexPnt = 0;

	for(i=0; i<CAM_MAX_NUM; i++)
	for(j=0; j<GOM_MAX_NUM; j++)
	{
		m_pMainFrm->m_ProdData.m_InspType[i][j] = 0;
		for(k=0; k<PROD_MAXDATA_NUM; k++)
		{
			m_pMainFrm->m_ProdData.m_bDec[i][j][k] = FALSE;
			m_pMainFrm->m_ProdData.m_fValue[i][j][k] = 0;
			m_pMainFrm->m_ProdData.m_fValueS[i][j][k] = 0;
		}
	}
}

void CDlgResult::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		InitListControls();	
		UpdateResultList();
	}
	// TODO: Add your message handler code here
}
