// DlgSubBlob.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSubBlob.h"
#include "DlgVDisp.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubBlob dialog
CDlgSubBlob::CDlgSubBlob(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubBlob::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubBlob)
	m_editThresh = 0;
	m_editMaxArea = _T("");
	m_editMinArea = _T("");
	//}}AFX_DATA_INIT

	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

	m_nPolarity = POL_BLACK;
}

void CDlgSubBlob::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubBlob)
	DDX_Control(pDX, IDC_HORIZON_THRESH, m_ctrlThresh);
	DDX_Text(pDX, IDC_EDIT_THRESH, m_editThresh);
	DDX_Text(pDX, IDC_EDIT_MAX_AREA, m_editMaxArea);
	DDX_Text(pDX, IDC_EDIT_MIN_AREA, m_editMinArea);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BTN_POLARITY, m_ctrlPolarity);
}

BEGIN_MESSAGE_MAP(CDlgSubBlob, CDialog)
	//{{AFX_MSG_MAP(CDlgSubBlob)
	ON_WM_HSCROLL()
	ON_COMMAND(IDM_BLACK, OnBlack)
	ON_COMMAND(IDM_WHITE, OnWhite)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_MIN_MAX_DIST, OnCheckMinMaxDist)
	ON_EN_SETFOCUS(IDC_EDIT_MIN_AREA, OnSetfocusEditMinArea)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_AREA, OnSetfocusEditMaxArea)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 100, On_MenuCallback)
	ON_BN_CLICKED(IDC_BTN_POLARITY, &CDlgSubBlob::OnBnClickedBtnPolarity)
	ON_BN_CLICKED(IDC_RADIO_LOC_RIGHT, &CDlgSubBlob::OnBnClickedRadioLocRight)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubBlob message handlers

BOOL CDlgSubBlob::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	UpdateControlsData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSubBlob::InitControls()
{
	short shBtnColor = 30;

	m_ctrlPolarity.SetBitmaps(IDB_BMP_BLACK, RGB(255, 0, 0));
	m_ctrlPolarity.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_ctrlPolarity.SetFlat(FALSE);
	m_ctrlPolarity.SetMenu(IDR_MENU_POLARITY, m_hWnd, TRUE, NULL);
	m_ctrlPolarity.SetMenuCallback(m_hWnd, WM_USER + 100);

	m_ctrlThresh.SetScrollRange(BLACK_LEVEL, WHITE_LEVEL, 1);
	m_ctrlThresh.SetScrollPos(0);

	((CButton *)GetDlgItem(IDC_RADIO_MAX_AREA))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_TOTAL_AREA))->SetCheck(FALSE);
}

void CDlgSubBlob::UpdateControlsData()
{
	LPBYTE pPAT;
	BOOL bOption, bFelet;
	long nSelectedROI, nLocROI;
	long nPatWidth, nPatHeight;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		bOption = (BOOL)m_pData->m_INSP[nSelectedROI].m_nBlobOption;
		bFelet = (BOOL)m_pData->m_INSP[nSelectedROI].m_bBlobFeret;
		nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;

		((CButton *)GetDlgItem(IDC_RADIO_MAX_AREA))->SetCheck(!bOption);
		((CButton *)GetDlgItem(IDC_RADIO_TOTAL_AREA))->SetCheck(bOption);
		((CButton *)GetDlgItem(IDC_CHECK_MIN_MAX_DIST))->SetCheck(bFelet);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);

		if (nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);
		
		m_editThresh = m_pData->m_INSP[nSelectedROI].m_nBlobThresh;
//		m_editMinArea.Format(_T("%3.1f"), m_pData->m_INSP[nSelectedROI].m_dBlobMinArea);
//		m_editMaxArea.Format(_T("%3.1f"), m_pData->m_INSP[nSelectedROI].m_dBlobMaxArea);
		m_nPolarity = m_pData->m_INSP[nSelectedROI].m_nBlobPolarity;

		m_ctrlThresh.SetScrollPos(m_editThresh);
		if (m_nPolarity == POL_BLACK)
			m_ctrlPolarity.SetBitmaps(IDB_BMP_BLACK, RGB(255, 0, 0));
		else
			m_ctrlPolarity.SetBitmaps(IDB_BMP_WHITE, RGB(255, 0, 0));

		pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[0];
		nPatWidth = m_pData->m_INSP[nSelectedROI].m_nPatWidth;
		nPatHeight = m_pData->m_INSP[nSelectedROI].m_nPatHeight;

	    m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nPatWidth, nPatHeight);
		UpdateData(false);
    }	
}

LRESULT CDlgSubBlob::On_MenuCallback(WPARAM wParam, LPARAM lParam)
{
	BCMenu*	pMenu = (BCMenu*)wParam;
	return 0;
} 

void CDlgSubBlob::OnBlack() 
{
	m_nPolarity = POL_BLACK;
	m_ctrlPolarity.SetBitmaps(IDB_BMP_BLACK, RGB(255, 0, 0));
}

void CDlgSubBlob::OnWhite() 
{
	m_nPolarity = POL_WHITE;
	m_ctrlPolarity.SetBitmaps(IDB_BMP_WHITE, RGB(255, 0, 0));
}

void CDlgSubBlob::ApplyInspData(long nSelectedROI)
{
	LPBYTE pPAT;
	BOOL bOption, bFelet;
	long nPatWidth, nPatHeight;
	GOM_ATTR *pGOM;

	UpdateData(true);
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		pGOM = &m_pData->m_GOM[nSelectedROI];
		pPAT = (LPBYTE)m_pParent->m_pModelData->RegisterBLOB(nSelectedROI, m_pImage, (long)(pGOM->m_ptPoint1.x+0.5), (long)(pGOM->m_ptPoint1.y+0.5),
			                                                                         (long)(pGOM->m_ptPoint4.x+0.5), (long)(pGOM->m_ptPoint4.y+0.5),
			                                                                         m_pData->m_nImageWidth, m_pData->m_nImageHeight, 
																					 m_nPolarity, m_editThresh, &nPatWidth, &nPatHeight);
		if (pPAT)
		{
			pGOM->m_dAngle = 0.0f;
			pGOM->m_ptPoint2.x = pGOM->m_ptPoint4.x;
			pGOM->m_ptPoint2.y = pGOM->m_ptPoint1.y;
			pGOM->m_ptPoint3.x = pGOM->m_ptPoint1.x;
			pGOM->m_ptPoint3.y = pGOM->m_ptPoint4.y;

			if (pGOM->m_nGOMType == GOM_CIRCLE2) 
				m_pData->m_INSP[nSelectedROI].m_nInspectType = INSP_BLOB_CIRCLE;
			else
				m_pData->m_INSP[nSelectedROI].m_nInspectType = INSP_BLOB_ANALYSIS;

 			bOption = ((CButton *)GetDlgItem(IDC_RADIO_MAX_AREA))->GetCheck();
 			bFelet = ((CButton *)GetDlgItem(IDC_CHECK_MIN_MAX_DIST))->GetCheck();

			m_pData->m_INSP[nSelectedROI].m_bBlobFeret = bFelet;
			m_pData->m_INSP[nSelectedROI].m_nBlobOption = (long)!bOption;
			m_pData->m_INSP[nSelectedROI].m_nBlobThresh = m_editThresh;
			m_pData->m_INSP[nSelectedROI].m_nBlobPolarity = m_nPolarity;
			m_pData->m_INSP[nSelectedROI].m_nPatWidth = nPatWidth;
			m_pData->m_INSP[nSelectedROI].m_nPatHeight = nPatHeight;
			m_pData->m_INSP[nSelectedROI].m_nPatNum = 1;
//			m_pData->m_INSP[nSelectedROI].m_dBlobMinArea = (float)_ttof(m_editMinArea);
//			m_pData->m_INSP[nSelectedROI].m_dBlobMaxArea = (float)_ttof(m_editMaxArea);
		}

		m_pData->m_INSP[nSelectedROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();
        m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nPatWidth, nPatHeight);
    }

	UpdateData(false);
}

void CDlgSubBlob::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{ 
	LPBYTE pImage=NULL;
	long i, j, nPol, nColor, nType, nGray;
	long nSelectedROI, nWidth, nHeight, nImageWidth, nImageHeight;
	float a, b, dDist, dInR, dOutR, dCX, dCY;
	CPoint ptStart, ptEnd;

 	UpdateData(true);
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
	nImageWidth = m_pData->m_nImageWidth;
	nImageHeight = m_pData->m_nImageHeight;

	switch (nSBCode) 
	{ 
    	case SB_LINELEFT: 
        	pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()-1); 
         	break; 
    	case SB_LINERIGHT: 
        	pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()+1); 
            break; 
    	case SB_PAGELEFT: 
        	pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()-10); 
        	break; 
    	case SB_PAGERIGHT: 
        	pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()+10); 
        	break; 
    	case SB_THUMBTRACK: 
        	pScrollBar->SetScrollPos(nPos); 
         	break; 
	} 
	
	if (pScrollBar->m_hWnd == GetDlgItem(IDC_HORIZON_THRESH)->m_hWnd)
	{
		m_editThresh = pScrollBar->GetScrollPos();
		UpdateData(false);
	}

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		nPol = m_pData->m_INSP[nSelectedROI].m_nBlobPolarity;
        if (nType==INSP_BLOB_CIRCLE)
		{
			if (nPol==POL_WHITE) nColor = 0;
			else nColor = 255;

			dCX = m_pData->m_GOM[nSelectedROI].m_dCX;
			dCY = m_pData->m_GOM[nSelectedROI].m_dCY;
			dInR = m_pData->m_GOM[nSelectedROI].m_dInRadius;
			dOutR = m_pData->m_GOM[nSelectedROI].m_dOutRadius;

			ptStart.x = (long)(dCX-dOutR);
			ptStart.y = (long)(dCY-dOutR);
			ptEnd.x = (long)(dCX+dOutR);
			ptEnd.y = (long)(dCY+dOutR);

			nWidth = (ptEnd.x-ptStart.x + 1);
			nHeight = (ptEnd.y-ptStart.y + 1);

			pImage = new unsigned char[nHeight*nWidth];

			for(i=0; i<nHeight; i++)
			for(j=0; j<nWidth; j++)
			{
				a = (i+ptStart.y)-dCY;
				b = (j+ptStart.x)-dCX;
				dDist = (float)sqrt(a*a + b*b);

				nGray = m_pImage[(i+ptStart.y)*nImageWidth + (j+ptStart.x)];
				if (dDist>=dInR && dDist<=dOutR)
				{
					if (nGray>m_editThresh)
						pImage[i*nWidth + j] = WHITE_LEVEL;
					else 
						pImage[i*nWidth + j] = BLACK_LEVEL;
				}
				else
					pImage[i*nWidth + j] = (unsigned char)nColor;
			}
		}
		else
		{
			ptStart.x = (long)m_pData->m_GOM[nSelectedROI].m_ptPoint1.x;
			ptStart.y = (long)m_pData->m_GOM[nSelectedROI].m_ptPoint1.y;
			ptEnd.x = (long)m_pData->m_GOM[nSelectedROI].m_ptPoint4.x;
			ptEnd.y = (long)m_pData->m_GOM[nSelectedROI].m_ptPoint4.y;

			nWidth = (ptEnd.x-ptStart.x + 1);
			nHeight = (ptEnd.y-ptStart.y + 1);

			pImage = new unsigned char[nHeight*nWidth];

			for(i=0; i<nHeight; i++)
			for(j=0; j<nWidth; j++)
			{
				nGray = m_pImage[(i+ptStart.y)*nImageWidth + (j+ptStart.x)];
				if (nGray>m_editThresh)
					pImage[i*nWidth + j] = WHITE_LEVEL;
				else 
					pImage[i*nWidth + j] = BLACK_LEVEL;
			}
		}

        m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pImage, nWidth, nHeight);
		if (pImage) delete pImage;
	}

	InvalidateRect(m_pParent->m_rcPlotBounds, false);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar); 
} 

void CDlgSubBlob::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (bShow)
		UpdateControlsData();
}

void CDlgSubBlob::OnCheckMinMaxDist() 
{
	if (((CButton *)GetDlgItem(IDC_CHECK_MIN_MAX_DIST))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_RADIO_MAX_AREA))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RADIO_TOTAL_AREA))->SetCheck(FALSE);
	}
}

void CDlgSubBlob::OnSetfocusEditMinArea() 
{
}

void CDlgSubBlob::OnSetfocusEditMaxArea() 
{
}


void CDlgSubBlob::OnBnClickedBtnPolarity()
{
	// TODO: Add your control notification handler code here
}


void CDlgSubBlob::OnBnClickedRadioLocRight()
{
	// TODO: Add your control notification handler code here
}
