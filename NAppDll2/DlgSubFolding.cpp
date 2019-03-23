// DlgSubPattern.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "DlgSubFolding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern dialog
CDlgSubFolding::CDlgSubFolding(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubFolding::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubPattern)
	m_editArea = 0;
	m_editSlope = 0;
	m_editTabColor = _T("BLACK");
	m_editRoiSize = 50;
	//}}AFX_DATA_INIT

	m_nPage=0;
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
}

void CDlgSubFolding::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubPattern)
	DDX_Text(pDX, IDC_EDIT_AREA, m_editArea);
	DDX_Text(pDX, IDC_EDIT_SLOPE, m_editSlope);
	DDX_Text(pDX, IDC_EDIT_TAB_COLOR, m_editTabColor);
	DDX_Text(pDX, IDC_EDIT_ROI_SIZE, m_editRoiSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSubFolding, CDialog)
	//{{AFX_MSG_MAP(CDlgSubPattern)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AREA, OnDeltaposSpinArea)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOPE, OnDeltaposSpinSlope)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TAB_COLOR, OnDeltaposSpinTabColor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROI_SIZE, OnDeltaposSpinRoiSize)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_PAT4_TYPE, &CDlgSubFolding::OnBnClickedRadioPat4Type)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubPattern message handlers
BOOL CDlgSubFolding::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSubFolding::InitControls()
{
	short shBtnColor = 30;
	WINDOWPLACEMENT wp;
	RECT rc;

    GetDlgItem(IDC_SPIN_AREA)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_AREA)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);

	GetDlgItem(IDC_SPIN_SLOPE)->GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;
    GetDlgItem(IDC_SPIN_SLOPE)->SetWindowPos(NULL, rc.left, rc.top, 40, 25, SWP_SHOWWINDOW);
}

void CDlgSubFolding::UpdateControlsData()
{
	LPBYTE pPAT;
	long nSelectedROI, nPatType, nTabColor, nSizeX, nSizeY;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_editArea = (long)m_pData->m_INSP[nSelectedROI].m_dTeachValue;
		m_editSlope = (long)m_pData->m_INSP[nSelectedROI].m_dSensity;

		nTabColor = m_pData->m_INSP[nSelectedROI].m_nBlobPolarity;
		m_editRoiSize = (long)m_pData->m_INSP[nSelectedROI].m_bBlobFeret;
		nPatType = m_pData->m_INSP[nSelectedROI].m_nBlobOption;


		pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[0];
	//	nPatNum = m_pData->m_INSP[nSelectedROI].m_nPatNum;
		m_nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;
		nSizeX = m_pData->m_INSP[nSelectedROI].m_nPatWidth;
		nSizeY = m_pData->m_INSP[nSelectedROI].m_nPatHeight;

		((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(FALSE);

		if (nPatType==0) ((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->SetCheck(TRUE);
		if (nPatType==1) ((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->SetCheck(TRUE);
		if (nPatType==2) ((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->SetCheck(TRUE);
		if (nPatType==3) ((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->SetCheck(TRUE);

		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);
		if (m_nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);

		if (nTabColor==0) m_editTabColor = _T("BLACK");
		else m_editTabColor = _T("WHITE");

	  //  m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nSizeX, nSizeY);
		UpdateData(false);
    }
}

void CDlgSubFolding::ApplyInspData(long nSelectROI)
{
	long nSel;
	UpdateData(true);

    m_pData->m_INSP[nSelectROI].m_nInspectType = INSP_TAB_FOLDING;
	m_pData->m_INSP[nSelectROI].m_dTeachValue = (float)m_editArea;
	m_pData->m_INSP[nSelectROI].m_dSensity = (float)m_editSlope;
	m_pData->m_INSP[nSelectROI].m_bBlobFeret = (BOOL)m_editRoiSize;
	m_pData->m_INSP[nSelectROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();

	if (m_editTabColor==_T("BLACK")) m_pData->m_INSP[nSelectROI].m_nBlobPolarity = 0;
	else m_pData->m_INSP[nSelectROI].m_nBlobPolarity = 1;

	nSel = 0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT1_TYPE))->GetCheck()) nSel=0;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT2_TYPE))->GetCheck()) nSel=1;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT3_TYPE))->GetCheck()) nSel=2;
	if (((CButton *)GetDlgItem(IDC_RADIO_PAT4_TYPE))->GetCheck()) nSel=3;
	m_pData->m_INSP[nSelectROI].m_nBlobOption = nSel;

	m_pData->m_INSP[nSelectROI].m_nPatNum = 0;

	m_pParent->InvalidateView();
}

void CDlgSubFolding::OnDeltaposSpinSlope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editSlope<50) m_editSlope++;
	}
	else
	{
		if (m_editSlope>5)  m_editSlope--;
	}

	UpdateData(false);
	*pResult = 0;
}



void CDlgSubFolding::OnDeltaposSpinArea(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editArea<10000) m_editArea++;
	}
	else
	{
		if (m_editArea>10)     m_editArea--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnDeltaposSpinTabColor(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;
	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		if (pNMUpDown->iDelta<0)
		{
			if (m_editTabColor==_T("BLACK")) m_editTabColor = _T("WHITE");
		}
		else
		{
			if (m_editTabColor==_T("WHITE")) m_editTabColor = _T("BLACK");
		}
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnDeltaposSpinRoiSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	long nSelectedROI;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		if (pNMUpDown->iDelta<0)
		{
			if(m_editRoiSize<100) m_editRoiSize+=5;
		}
		else
		{
			if(m_editRoiSize>30)  m_editRoiSize-=5;
		}
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubFolding::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		UpdateControlsData();
}

void CDlgSubFolding::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

}	


void CDlgSubFolding::OnBnClickedRadioPat4Type()
{
	// TODO: Add your control notification handler code here
}
