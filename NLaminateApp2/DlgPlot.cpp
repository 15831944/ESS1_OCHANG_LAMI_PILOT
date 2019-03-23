// DlgPlot.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPlot.h"
#include "NPlotDll.h"
#include "NAppDll.h"
#include "MainFrm.h"
#include "math.h"
#include "afxdialogex.h"

extern  CMainFrame *m_pMainFrm;

// CDlgPlot 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgPlot, CDialogEx)

void CALLBACK NPlotDll_CallBackEvent();

CDlgPlot::CDlgPlot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPlot::IDD, pParent)
{
	m_bToggleSize = FALSE;
	m_nFullSizeID = 0;
	m_nCurSel = 0;
}

CDlgPlot::~CDlgPlot()
{
	long i, nPlotMaxNum;
	CString str;

	nPlotMaxNum = PLOT_PAGE_MAX_NUM*PLOT_MAX_NUM;
	for(i=0; i<nPlotMaxNum; i++)
	{
		str.Format(_T("%sRes%d.fmt"), DEF_RESULT_PATHNAME, i);
		NPlotDll_SaveStickData(i, str);

		NPlotDll_Close(i);
	}
}

void CDlgPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MULTI, m_tabMulti);
}

BEGIN_MESSAGE_MAP(CDlgPlot, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MULTI, &CDlgPlot::OnTcnSelchangeMultiTab)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CDlgPlot 메시지 처리기입니다.
BOOL CDlgPlot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(NULL, 0, 0, DEF_DIALOG_WIDTH, 1024, SWP_SHOWWINDOW);

	InitPlotControls();
	EnableFullSizePlot(0, true);
	EnableFullSizePlot(0, false);

	GetDlgItem(IDC_TAB_MULTI)->SetWindowPos(NULL, 5, 2, PLOT_MIN_WIDTH*2+17, 32, SWP_SHOWWINDOW);
 	GetSelectedTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgPlot::SetTransLanguage(long nValue)
{
	GetSelectedTab(0);
}

void CDlgPlot::InitPlotControls()
{
	long i, nPitch, nPlotNum;
	long nDiv, nMod;
	CString str;

	nPlotNum = PLOT_MAX_NUM*PLOT_PAGE_MAX_NUM;
	for(i=0; i<nPlotNum; i++)
	{
		nDiv = i/(PLOT_MAX_NUM/2);
		nMod = i%PLOT_MAX_NUM;
		if (i%2==0)
		{
			nPitch = (nMod/2) * (5+PLOT_MIN_HEIGHT);
			GetDlgItem(IDC_STATIC_UP_PLOT1+i)->SetWindowPos(NULL, 5, 27+nPitch, PLOT_MIN_WIDTH, PLOT_MIN_HEIGHT, SWP_SHOWWINDOW);
		}
		else 
		{
			nPitch = (nMod/2) * (5+PLOT_MIN_HEIGHT);
			GetDlgItem(IDC_STATIC_UP_PLOT1+i)->SetWindowPos(NULL, PLOT_MIN_WIDTH+18, 27+nPitch, PLOT_MIN_WIDTH, PLOT_MIN_HEIGHT, SWP_SHOWWINDOW);
		}

    	NPlotDll_Open(i, (long)GetDlgItem(IDC_STATIC_UP_PLOT1+i));
		NPlotDll_InitialData(i);
		NPlotDll_SetMaxDataNum(i, PLOT_MAX_DATA_NUM);
		NPlotDll_LoadSetupData(i, DEF_MINPLOT_PATHNAME);
		NPlotDll_SetEvent(i, NPlotDll_CallBackEvent,  (long)&NPlotDll_Data);

		str.Format(_T("%sRes%d.fmt"), DEF_RESULT_PATHNAME, i);
		NPlotDll_LoadStickData(i, str);

		str.Format(_T("No. %d"), i+1);
		NPlotDll_SetCaptionX(i, str);
	}
}

void CDlgPlot::ResetPlotData()
{
	long i, nPlotNum;

	nPlotNum = PLOT_MAX_NUM*PLOT_PAGE_MAX_NUM;
	for(i=0; i<nPlotNum; i++)
	{
//		NPlotDll_InitialData(i);
		NPlotDll_SetMaxDataNum(i, PLOT_MAX_DATA_NUM);
		NPlotDll_Invalidate(i);
	}
}

void CDlgPlot::InitPlotData()
{
	long  i, j, k, nPlotMaxNum;
	long  nCamID, nPlotNum, nNum;
	float fRate;
	CString strT, str;
	INSP_RESULT_DATA *pRes;

	nPlotMaxNum = PLOT_PAGE_MAX_NUM * PLOT_MAX_NUM;
	for(i=0; i<nPlotMaxNum; i++)
	{
		m_PL[i].m_nNum = 0;
		m_PL[i].m_bLink = FALSE;
		m_PL[i].m_nCamID = -1;
		for(j=0; j<PLOT_ONE_MAX_NUM; j++)
		{
			m_PL[i].m_nIndexR[j] = -1;
			_tcscpy_s(m_PL[i].m_strTitle[j], _T(""));
		}
    }

	for(i=0; i<VIEW_MAX_NUM; i++)
	{
	    nCamID = i+1;
		pRes = (INSP_RESULT_DATA *)NAppDll_GetResultData(i);

		for(j=0; j<GOM_MAX_NUM; j++)
		{
			nPlotNum = pRes->m_nPlotNum[j]-1;

			if (!_tcscmp(pRes->m_strTitle[j], _T(""))) continue;
			if (pRes->m_nInspType[j] == INSP_MEASURE_SIDE_ONE) continue; //shsong

			if (nPlotNum<0 || nPlotNum>=nPlotMaxNum) continue;
			if (pRes->m_bLocROI[j]) continue;
			
				
			nNum = m_PL[nPlotNum].m_nNum;	
			if (nNum>=PLOT_ONE_MAX_NUM) continue;

			_tcscpy_s(m_PL[nPlotNum].m_strTitle[nNum], pRes->m_strTitle[j]);
			m_PL[nPlotNum].m_fLowSpec = pRes->m_dLowLimit[j];
			m_PL[nPlotNum].m_fHighSpec = pRes->m_dHighLimit[j];
			fRate = (pRes->m_dHighLimit[j]-pRes->m_dLowLimit[j])*0.4f;
			m_PL[nPlotNum].m_fUpperL = pRes->m_dHighLimit[j]+fRate;
			m_PL[nPlotNum].m_fLowerL = pRes->m_dLowLimit[j]-fRate;
			m_PL[nPlotNum].m_nCamID = nCamID;
			m_PL[nPlotNum].m_nIndexF[nNum] = j;
  	
	#if _DUO_CELL
			m_PL[nPlotNum].m_nIndexR[nNum] = -1;
			for(k=0; k<GOM_MAX_NUM; k++)
			{
				if (!_tcscmp(pRes->m_strTitle[k], _T(""))) continue;
				if (!pRes->m_bLocROI[k]) continue;
				if (!_tcscmp(pRes->m_strTitle[j], pRes->m_strTitle[k]))
				{
					m_PL[nPlotNum].m_nIndexR[nNum] = k;
					break;
				}
			}
	#endif
			m_PL[nPlotNum].m_nNum++;
		}

		for(j=0; j<pRes->m_nLinkNum; j++)
		{
			nPlotNum = pRes->m_nLinkPlotNum[j]-1;

			if (!_tcscmp(pRes->m_strLinkTitle[j], _T(""))) continue;
			if (nPlotNum<0 || nPlotNum>=nPlotMaxNum) continue;
			if (pRes->m_bLinkLocROI[j]) continue;
							
			nNum = m_PL[nPlotNum].m_nNum;	
			if (nNum>=PLOT_ONE_MAX_NUM) continue;

			_tcscpy_s(m_PL[nPlotNum].m_strTitle[nNum], pRes->m_strLinkTitle[j]);
			m_PL[nPlotNum].m_fLowSpec = pRes->m_dLinkLowLimit[j];
			m_PL[nPlotNum].m_fHighSpec = pRes->m_dLinkHighLimit[j];
			fRate = (pRes->m_dLinkHighLimit[j]-pRes->m_dLinkLowLimit[j])*0.4f;
			m_PL[nPlotNum].m_fUpperL = pRes->m_dLinkHighLimit[j]+fRate;
			m_PL[nPlotNum].m_fLowerL = pRes->m_dLinkLowLimit[j]-fRate;
			m_PL[nPlotNum].m_nCamID = nCamID;
			m_PL[nPlotNum].m_nIndexF[nNum] = j;
			m_PL[nPlotNum].m_bLink = TRUE;

	#if _DUO_CELL
			m_PL[nPlotNum].m_nIndexR[nNum] = -1;
			for(k=0; k<GOM_MAX_NUM; k++)
			{
				if (!_tcscmp(pRes->m_strLinkTitle[k], _T(""))) continue;
				if (!pRes->m_bLinkLocROI[k]) continue;
				if (!_tcscmp(pRes->m_strLinkTitle[j], pRes->m_strLinkTitle[k]))
				{
					m_PL[nPlotNum].m_nIndexR[nNum] = k;
					break;
				}
			}
	#endif

			m_PL[nPlotNum].m_nNum++;
		}
	}

	for(i=0; i<nPlotMaxNum; i++)
	{
		nCamID = m_PL[i].m_nCamID-1;

		if (nCamID<0) 
		{
			NPlotDll_SetEnableHoriRefLine(i, FALSE);
			NPlotDll_SetRemarkNum(i, 0);
			NPlotDll_SetRangeLY(i, 0.0f, 3.0f);		
		}
		else
		{
			NPlotDll_SetRemarkNum(i, m_PL[i].m_nNum);
			NPlotDll_SetEnableHoriRefLine(i, TRUE);

			NPlotDll_SetPosHoriRefLine(i, TRUE, m_PL[i].m_fLowSpec, m_PL[i].m_fHighSpec);
			NPlotDll_SetRangeLY(i, m_PL[i].m_fLowerL, m_PL[i].m_fUpperL);

			for(j=0; j<m_PL[i].m_nNum; j++)
				NPlotDll_SetCaptionRemark(i, j, (LPCTSTR)m_PL[i].m_strTitle[j]);
		}

		NPlotDll_Invalidate(i);
	}
}

void CDlgPlot::UpdatePlotData(long nPageID)
{
	long i, j, nIndex, nS, nE;
	long nCamID, nNum;

	nS = nPageID*PLOT_MAX_NUM;
	nE = nS+PLOT_MAX_NUM;

	for(i=nS; i<nE; i++)
	{
		nNum = m_PL[i].m_nNum;
		nCamID = m_PL[i].m_nCamID-1;
		if (nCamID<0) continue;

		for(j=0; j<nNum; j++)
		{
			nIndex = m_PL[i].m_nIndexF[j];
			if (nIndex>=0 && !m_PL[i].m_bLink)
	    		NPlotDll_AppendStickData(i, j, 0, m_pMainFrm->m_pRes[nCamID]->m_dValue[nIndex]);

			if (nIndex>=0 && m_PL[i].m_bLink)
				NPlotDll_AppendStickData(i, j, 0,  m_pMainFrm->m_pRes[nCamID]->m_dLinkValue[nIndex]);

#if _DUO_CELL
			nIndex = m_PL[i].m_nIndexR[j];
			if (nIndex>=0 && !m_PL[i].m_bLink)
		    	NPlotDll_AppendStickData(i, j, 0, m_pMainFrm->m_pRes[nCamID]->m_dValue[nIndex]);

			if (nIndex>=0 && m_PL[i].m_bLink)
				NPlotDll_AppendStickData(i, j, 0, m_pMainFrm->m_pRes[nCamID]->m_dLinkValue[nIndex]);
#endif
		}

		NPlotDll_Invalidate(i);
	}
}

long CDlgPlot::GetSelectedTab(long nSel)
{
	long i, nLngSel, nMaxPlot, nS, nE;
    HICON hIcon[5];
	CString str;

	nLngSel = m_pMainFrm->m_Setting.m_nSelLanguage;
	nSel = m_tabMulti.GetCurSel();
	if (nSel<0) nSel=0;

	m_nCurSel = nSel;

	for(i=0; i<PLOT_PAGE_MAX_NUM; i++)
		hIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
	hIcon[nSel] = AfxGetApp()->LoadIcon(IDI_ICON_LED_ON);

	m_imgListSel.DeleteImageList();
	m_imgListSel.Create(16, 16, TRUE, 3, 3);

	for(i=0; i<PLOT_PAGE_MAX_NUM; i++)
		m_imgListSel.Add(hIcon[i]);

	m_tabMulti.DeleteAllItems();
	m_tabMulti.SetImageList(&m_imgListSel);
	m_tabMulti.SetMinTabWidth(120);

	TC_ITEM item;
    item.mask = TCIF_IMAGE|TCIF_TEXT;

	for(i=0; i<PLOT_PAGE_MAX_NUM; i++)
	{
		item.iImage = i;
		item.pszText = new TCHAR[10];

		str.Format(_T(" PAGE %d "), i+1);
		_tcscpy(item.pszText, str);

		m_tabMulti.InsertItem(i, &item);
		delete item.pszText;
	}

	nMaxPlot = PLOT_MAX_NUM*PLOT_PAGE_MAX_NUM;
	for(i=0; i<nMaxPlot; i++)
		EnablePlotWindow(i, false);

	nS = PLOT_MAX_NUM*nSel;
	nE = nS + PLOT_MAX_NUM;
	for(i=nS; i<nE; i++)
		EnablePlotWindow(i, true);

	m_tabMulti.SetCurSel(nSel);
	return nSel;
}

void CDlgPlot::OnTcnSelchangeMultiTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	long nSel;
	double dTime = m_pMainFrm->m_ctrlTimer.GetClockTimer(TIMER_ON_PROCESS);

	if (m_pMainFrm->m_bCriticalF && dTime<DEF_PROC_TIMEOUT) 
	{
		if (m_nCurSel>=0)
		    m_tabMulti.SetCurSel(m_nCurSel);
		return;
	}

	nSel = m_tabMulti.GetCurSel();
	if (nSel<0) nSel=0;

	EnableFullSizePlot(m_nFullSizeID, false);
	GetSelectedTab(nSel);

	*pResult = 0;
}

void CDlgPlot::EnablePlotWindow(long nID, BOOL bFlag)
{
	if (bFlag)
		GetDlgItem(IDC_STATIC_UP_PLOT1+nID)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_STATIC_UP_PLOT1+nID)->ShowWindow(SW_HIDE);
}

void CDlgPlot::EnableFullSizePlot(long nID, bool bFlag)
{
	long i, nSel, nMaxNum, nS, nE;
	CString str;
	WINDOWPLACEMENT wp;
	RECT rc;

	nSel = m_tabMulti.GetCurSel();
	if (nSel<0)
		nSel = 0;

	m_nFullSizeID = nID;
	m_bToggleSize = bFlag;
		
	nMaxNum = PLOT_MAX_NUM*PLOT_PAGE_MAX_NUM;
	nS = PLOT_MAX_NUM*nSel;
	nE = nS + PLOT_MAX_NUM;

	if (bFlag)
	{
		GetDlgItem(IDC_STATIC_UP_PLOT1+nID)->GetWindowPlacement(&wp);
	    rc = wp.rcNormalPosition;

		m_rcPlotRect.left = rc.left;
		m_rcPlotRect.top = rc.top;
		m_rcPlotRect.right = rc.right;
		m_rcPlotRect.bottom = rc.bottom;

		for(i=0; i<nMaxNum; i++)
		    EnablePlotWindow(i, false);

	    GetDlgItem(IDC_STATIC_UP_PLOT1+nID)->SetWindowPos(NULL, 10, 40, PLOT_MAX_WIDTH, PLOT_MAX_HEIGHT, SWP_SHOWWINDOW);
        NPlotDll_SetWindowPtr(nID, (long)GetDlgItem(IDC_STATIC_UP_PLOT1+nID));
		NPlotDll_LoadSetupData(nID, DEF_MAXPLOT_PATHNAME);
	
	    InitPlotData();
	}
	else
	{
	    GetDlgItem(IDC_STATIC_UP_PLOT1+nID)->SetWindowPos(NULL, m_rcPlotRect.left, m_rcPlotRect.top, m_rcPlotRect.Width(), m_rcPlotRect.Height(), SWP_HIDEWINDOW);
        NPlotDll_SetWindowPtr(nID, (long)GetDlgItem(IDC_STATIC_UP_PLOT1+nID));
		NPlotDll_LoadSetupData(nID, DEF_MINPLOT_PATHNAME);

	    InitPlotData();

		for(i=0; i<nMaxNum; i++)
			EnablePlotWindow(i, false);

		for(i=nS; i<nE; i++)
		    EnablePlotWindow(i, true);
	}

	str.Format(_T("No. %d"), nID+1);
	NPlotDll_SetCaptionX(nID, str);
}

void CALLBACK NPlotDll_CallBackEvent()
{
	long nID;
	BOOL bToggle;

	nID = m_pMainFrm->m_pView->m_pDlgPlot->NPlotDll_Data.m_nPlotID;

	bToggle = m_pMainFrm->m_pView->m_pDlgPlot->m_bToggleSize;
	m_pMainFrm->m_pView->m_pDlgPlot->EnableFullSizePlot(nID, !bToggle);
}

void CDlgPlot::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	    InitPlotData();
}
