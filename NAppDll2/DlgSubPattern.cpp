// DlgSubPattern.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "DlgSubPattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern dialog
CDlgSubPattern::CDlgSubPattern(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubPattern::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubPattern)
	m_editMatchScore = 0;
	m_editSearchRate = 0;
	m_editPatSizeX = 0;
	m_editPatSizeY = 0;
	//}}AFX_DATA_INIT

	m_nPage=0;
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
}

void CDlgSubPattern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubPattern)
	DDX_Control(pDX, IDC_COMBO_PAT_NUM, m_comboPatNum);
	DDX_Text(pDX, IDC_EDIT_MATCH_SCORE, m_editMatchScore);
	DDX_Text(pDX, IDC_EDIT_SEARCH_RATE, m_editSearchRate);
	DDX_Text(pDX, IDC_EDIT_PAT_SIZE_X, m_editPatSizeX);
	DDX_Text(pDX, IDC_EDIT_PAT_SIZE_Y, m_editPatSizeY);
	DDX_Control(pDX, IDC_BTN_REGIST, m_btnRegist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSubPattern, CDialog)
	//{{AFX_MSG_MAP(CDlgSubPattern)
	ON_BN_CLICKED(IDC_BTN_REGIST, OnBtnRegist)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MATCH_SCORE, OnDeltaposSpinMatchScore)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SEARCH_RATE, OnDeltaposSpinSearchRate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PAT_SIZE_X, OnDeltaposSpinPatSizeX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PAT_SIZE_Y, OnDeltaposSpinPatSizeY)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_LOC_LEFT, &CDlgSubPattern::OnBnClickedRadioLocLeft)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern message handlers
BOOL CDlgSubPattern::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSubPattern::InitControls()
{
	short shBtnColor = 30;
//	WINDOWPLACEMENT wp;
//	RECT rc;

	m_btnRegist.SetIcon(IDI_ICON_REGIST);
	m_btnRegist.SetTooltipText(_T("설정한 데이타를 저장합니다."));
	m_btnRegist.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnRegist.DrawBorder(TRUE);
	m_btnRegist.DrawTransparent(TRUE);

	/*
    GetDlgItem(IDC_SPIN_MATCH_SCORE)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_MATCH_SCORE)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_SEARCH_RATE)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_SEARCH_RATE)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);
	*/

	m_comboPatNum.ResetContent();
	m_comboPatNum.AddString(_T("1"));
	m_comboPatNum.AddString(_T("3"));
	m_comboPatNum.AddString(_T("5"));
}

void CDlgSubPattern::UpdateControlsData()
{
	LPBYTE pPAT;
	long nSelectedROI, nSel, nPatNum, nPatType;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editMatchScore = m_pData->m_INSP[nSelectedROI].m_nPatScore;
		m_editSearchRate = m_pData->m_INSP[nSelectedROI].m_nPatSearchRate;
		m_editPatSizeX = m_pData->m_INSP[nSelectedROI].m_nPatWidth;
		m_editPatSizeY = m_pData->m_INSP[nSelectedROI].m_nPatHeight;
		nPatType = m_pData->m_INSP[nSelectedROI].m_nBlobOption;

		if (m_editPatSizeX<50) m_editPatSizeX = 50;
		if (m_editPatSizeY<50) m_editPatSizeY = 50;

		pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[0];
		nPatNum = m_pData->m_INSP[nSelectedROI].m_nPatNum;
		m_nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;

		((CButton *)GetDlgItem(IDC_RADIO_PAT0_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(FALSE);

		if (nPatType==0) ((CButton *)GetDlgItem(IDC_RADIO_PAT0_TYPE))->SetCheck(TRUE);
		if (nPatType==1) ((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(TRUE);
		if (nPatType==2) ((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(TRUE);
		if (nPatType==3) ((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(TRUE);
		if (nPatType==4) ((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(TRUE);
		
		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);
		if (m_nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);

		if (nPatNum<=1) nSel = 0;
		else if (nPatNum<=3) nSel = 1;
		else if (nPatNum<=5) nSel = 2;
		m_comboPatNum.SetCurSel(nSel);

	    m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, m_editPatSizeX, m_editPatSizeY);
		UpdateData(false);
    }
}

void CDlgSubPattern::ApplyInspData(long nSelectROI)
{
	long nPatWidth, nPatHeight, nSel;
	float fCx, fCy;
	UpdateData(true);

    m_pData->m_INSP[nSelectROI].m_nInspectType = INSP_PATTERN_MATCH;
	m_pData->m_INSP[nSelectROI].m_nPatScore = m_editMatchScore;
    m_pData->m_INSP[nSelectROI].m_nPatSearchRate = m_editSearchRate;
	m_pData->m_INSP[nSelectROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();

	nSel = 0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT0_TYPE))->GetCheck()) nSel=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->GetCheck()) nSel=1;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->GetCheck()) nSel=2;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->GetCheck()) nSel=3;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->GetCheck()) nSel=4;
	m_pData->m_INSP[nSelectROI].m_nBlobOption = nSel;

	nPatWidth = m_pData->m_INSP[nSelectROI].m_nPatWidth;
	nPatHeight = m_pData->m_INSP[nSelectROI].m_nPatHeight;

	fCx = (m_pData->m_GOM[nSelectROI].m_ptPoint1.x + m_pData->m_GOM[nSelectROI].m_ptPoint2.x) / 2.0f;
	fCy = (m_pData->m_GOM[nSelectROI].m_ptPoint1.y + m_pData->m_GOM[nSelectROI].m_ptPoint3.y) / 2.0f;

	m_pData->m_GOM[nSelectROI].m_ptPoint1.x = fCx - nPatWidth/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint2.x = fCx + nPatWidth/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint3.x = fCx - nPatWidth/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint4.x = fCx + nPatWidth/2;

	m_pData->m_GOM[nSelectROI].m_ptPoint1.y = fCy - nPatHeight/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint2.y = fCy - nPatHeight/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint3.y = fCy + nPatHeight/2;
	m_pData->m_GOM[nSelectROI].m_ptPoint4.y = fCy + nPatHeight/2;

	m_pParent->InvalidateView();
}

void CDlgSubPattern::OnBtnRegist()
{
	LPBYTE pPAT;
	CString strPatNum, strMessage;
	GOM_ATTR *pGOM;
	long nSelectedROI, nPatWidth, nPatHeight, nSel, nPatNum;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 패턴을 등록하시겠습니까 ?  ");
	else strMessage = _T("  Do you want to regist a pattern image ?  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		UpdateData(true);
		m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

		nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
		if (nSelectedROI>=0)
		{
			nPatNum = 1;
			nSel = m_comboPatNum.GetCurSel();
			if (nSel>=0)
			{
	    		m_comboPatNum.GetLBText(nSel, strPatNum);
	    		nPatNum = _ttoi(strPatNum);
			}

			pGOM = &m_pData->m_GOM[nSelectedROI];
			pPAT = (LPBYTE)m_pParent->m_pModelData->RegisterPattern(nSelectedROI, m_pImage, (long)(pGOM->m_ptPoint1.x+0.5), (long)(pGOM->m_ptPoint1.y+0.5),
																							(long)(pGOM->m_ptPoint4.x+0.5), (long)(pGOM->m_ptPoint4.y+0.5),
																							 m_pData->m_nImageWidth, m_pData->m_nImageHeight, nPatNum, &nPatWidth, &nPatHeight);
			pGOM->m_dAngle = 0.0f;
			pGOM->m_ptPoint2.x = pGOM->m_ptPoint4.x;
			pGOM->m_ptPoint2.y = pGOM->m_ptPoint1.y;
			pGOM->m_ptPoint3.x = pGOM->m_ptPoint1.x;
			pGOM->m_ptPoint3.y = pGOM->m_ptPoint4.y;

			m_pData->m_INSP[nSelectedROI].m_nInspectType = INSP_PATTERN_MATCH;
			m_pData->m_INSP[nSelectedROI].m_nPatWidth = nPatWidth;
			m_pData->m_INSP[nSelectedROI].m_nPatHeight = nPatHeight;
			m_pData->m_INSP[nSelectedROI].m_nPatNum = nPatNum;

			m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nPatWidth, nPatHeight);
			Invalidate();
		}
		else
		{
			if (m_pParent->m_nLngSel==0) MessageBox(_T(" 검사항목(ROI)가 선택되지 않았거나 Title이 일치하지 않습니다. "));
			else MessageBox(_T(" ROI has not been selected, or the Title does not match. "));
		}
	}
}

void CDlgSubPattern::OnDeltaposSpinMatchScore(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if(m_editMatchScore<100) m_editMatchScore++;
	}
	else
	{
		if(m_editMatchScore>0) m_editMatchScore--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubPattern::OnDeltaposSpinPatSizeX(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;
	float fCx;
	GOM_ATTR *pGOM;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editPatSizeX /= 10;
		m_editPatSizeX *= 10;

		if (pNMUpDown->iDelta<0)
		{
			if(m_editPatSizeX<500) m_editPatSizeX+=10;
		}
		else
		{
			if(m_editPatSizeX>50)  m_editPatSizeX-=10;
		}

		pGOM = &m_pData->m_GOM[nSelectedROI];

		fCx = (pGOM->m_ptPoint1.x + pGOM->m_ptPoint2.x)/2;
		pGOM->m_ptPoint1.x = fCx - m_editPatSizeX/2;
		pGOM->m_ptPoint2.x = fCx + m_editPatSizeX/2;
		pGOM->m_ptPoint3.x = fCx - m_editPatSizeX/2;
		pGOM->m_ptPoint4.x = fCx + m_editPatSizeX/2;

		m_pParent->InvalidateView();
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubPattern::OnDeltaposSpinPatSizeY(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;
	float fCy;
	GOM_ATTR *pGOM;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editPatSizeY /= 10;
		m_editPatSizeY *= 10;

		if (pNMUpDown->iDelta<0)
		{
			if(m_editPatSizeY<500) m_editPatSizeY+=10;
		}
		else
		{
			if(m_editPatSizeY>50)  m_editPatSizeY-=10;
		}

		pGOM = &m_pData->m_GOM[nSelectedROI];

		fCy = (pGOM->m_ptPoint1.y + pGOM->m_ptPoint3.y)/2;
		pGOM->m_ptPoint1.y = fCy - m_editPatSizeY/2.0f;
		pGOM->m_ptPoint2.y = fCy - m_editPatSizeY/2.0f;
		pGOM->m_ptPoint3.y = fCy + m_editPatSizeY/2.0f;
		pGOM->m_ptPoint4.y = fCy + m_editPatSizeY/2.0f;

		m_pParent->InvalidateView();
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubPattern::OnDeltaposSpinSearchRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if(m_editSearchRate<700) m_editSearchRate+=10;
	}
	else
	{
		if(m_editSearchRate>100) m_editSearchRate-=10;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubPattern::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		UpdateControlsData();
}

void CDlgSubPattern::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	/*
	BYTE ttt;
	LPBYTE pPAT;
	long l, i, j, nSelectedROI, nType;
	long nPatNum, nWidth, nHeight;
	long disp_width, disp_height;
	CBrush brush(RGB(200,0,0));

	RECT rct;
	WINDOWPLACEMENT wp;
	long  nPage=m_nPage*4;
	double rateX, rateY, rate;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		if (nType != INSP_PATTERN_MATCH) return;

    	nPatNum = m_pData->m_INSP[nSelectedROI].m_nPatNum;
		if (nPatNum<=0) return;

		for(l=0; l<4; l++)
		{
			GetDlgItem(IDC_FRAME_0+l)->GetWindowPlacement(&wp);
			rct=wp.rcNormalPosition;

			pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[l];
			nWidth = m_pData->m_INSP[nSelectedROI].m_nPatWidth; 
			nHeight = m_pData->m_INSP[nSelectedROI].m_nPatHeight; 

			if (pPAT && nWidth>0 && nHeight>0 && l<nPatNum)
			{
				disp_width=rct.right-rct.left;
				disp_height=rct.bottom-rct.top;

				rateX=(double)nWidth/(double)disp_width;
				rateY=(double)nHeight/(double)disp_height;
				if(rateX>rateY) rate=rateX;
				else            rate=rateY;

				if(nHeight>disp_height && nWidth>disp_width)
				{
					for(i=0;i<nHeight/rate;i++)
					for(j=0;j<nWidth/rate;j++)
					{
						ttt=*(pPAT+nWidth*((int)(i*rate))+(int)(j*rate));
						dc.SetPixel(rct.left+j, rct.top+i, RGB(ttt,ttt,ttt));
					}	
				}
				else if(nWidth>disp_width)
				{
					for(i=0;i<nHeight/rate;i++)
					for(j=0;j<disp_width;j++)
					{
						ttt=*(pPAT+nWidth*((int)(i*rate))+(int)(j*rate));
						dc.SetPixel(rct.left+j, rct.top+i, RGB(ttt,ttt,ttt));
					}	
				}
				else if(nHeight>disp_height)
				{
					for(i=0;i<disp_height;i++)
					for(j=0;j<nWidth/rate;j++)
					{
						ttt=*(pPAT+nWidth*((int)(i*rate))+(int)(j*rate));
						dc.SetPixel(rct.left+j, rct.top+i, RGB(ttt,ttt,ttt));
					}	
				}
				else
				{
					for(i=0;i<nHeight;i++)
					for(j=0;j<nWidth;j++)
					{
						ttt=*(pPAT+nWidth*i+j);
						dc.SetPixel(rct.left+j, rct.top+i, RGB(ttt,ttt,ttt));
					}	
				}
			}
			else
			{
    			dc.FrameRect(&rct,&brush);
            }
		}

		if ((nPatNum<4 && m_nPage==0) || (nPatNum>=4 && m_nPage==1) )
		{
			if (nPatNum>=4) nPatNum-=4;
			GetDlgItem(IDC_FRAME_0+(nPatNum%4))->GetWindowPlacement(&wp);
			rct=wp.rcNormalPosition;
			rct.left-=1;
			rct.top-=1;
			rct.right+=1;
			rct.bottom+=1;
			dc.FrameRect(&rct,&brush);
		}
	}
	*/
}	

void CDlgSubPattern::OnBtnNext() 
{
	GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_PREV)->ShowWindow(SW_SHOW);
	m_nPage=1;
	Invalidate();	
}

void CDlgSubPattern::OnBtnPrev() 
{
	GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_PREV)->ShowWindow(SW_HIDE);
	m_nPage=0;
	Invalidate();
}


void CDlgSubPattern::OnBnClickedRadioLocLeft()
{
	// TODO: Add your control notification handler code here
}
