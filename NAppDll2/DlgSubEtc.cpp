// DlgSubEtc.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "DlgSubEtc.h"
#include "NBlobDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubEtc dialog
CDlgSubEtc::CDlgSubEtc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubEtc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubEtc)
	//}}AFX_DATA_INIT
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
}

void CDlgSubEtc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubEtc)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSubEtc, CDialog)
	//{{AFX_MSG_MAP(CDlgSubEtc)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_LOC_RIGHT, &CDlgSubEtc::OnBnClickedRadioLocRight)
	ON_BN_CLICKED(IDC_BTN_REGIST, &CDlgSubEtc::OnBnClickedBtnRegist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubEtc message handlers
BOOL CDlgSubEtc::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateControlsData();

	return TRUE;
}

void CDlgSubEtc::UpdateControlsData()
{
	long nType, nSelectedROI, nSelPos, nLocROI;
	long nPatWidth, nPatHeight;
	LPBYTE pPAT;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		pPAT = m_pData->m_INSP[nSelectedROI].m_pPat[0];
		nPatWidth = m_pData->m_INSP[nSelectedROI].m_nPatWidth;
		nPatHeight = m_pData->m_INSP[nSelectedROI].m_nPatHeight;

		nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		nSelPos = nType-INSP_GRAY_VALUE;
		m_comboType.SetCurSel(nSelPos);
		nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;

		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);

		if (nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);

		m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nPatWidth, nPatHeight);
		m_pParent->Invalidate(false);
	}
}

void CDlgSubEtc::ApplyInspData(long nSelectROI)
{
	GOM_ATTR *pGOM;
	long nSelectedROI, nType, nSel;

	UpdateData(true);
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		pGOM = &m_pData->m_GOM[nSelectedROI];
		nSel = m_comboType.GetCurSel();
		if (nSel<0) nSel = 0;

    	nType = nSel + INSP_GRAY_VALUE;
		m_pData->m_INSP[nSelectedROI].m_nInspectType = nType;
		m_pData->m_INSP[nSelectedROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();
	}
			
	m_pParent->m_pDlgVisionSetup->m_pInspectDlg->UpdateSelectItemData();
	m_pParent->Invalidate(false);
}

void CDlgSubEtc::OnSelchangeComboType() 
{
}

void CDlgSubEtc::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (bShow)
		UpdateControlsData();
}



void CDlgSubEtc::OnBnClickedRadioLocRight()
{
	// TODO: Add your control notification handler code here
}


void CDlgSubEtc::OnBnClickedBtnRegist()
{
	LPBYTE pPAT;
	CString strPatNum, strMessage;
	GOM_ATTR *pGOM;
	long nSelectedROI, nType, nPatWidth, nPatHeight, nSel, nPatNum;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 패턴을 등록하시겠습니까 ?  ");
	else strMessage = _T("  Do you want to regist a pattern image ?  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		UpdateData(true);
		m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

		nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
		if (nSelectedROI>=0)
		{
			pGOM = &m_pData->m_GOM[nSelectedROI];
			nSel = m_comboType.GetCurSel();
			if (nSel<0) nSel = 0;

    		nType = nSel + INSP_CAMERA_CAL;
			m_pData->m_INSP[nSelectedROI].m_nInspectType = nType;
			m_pData->m_INSP[nSelectedROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();

			if (nType == INSP_CAMERA_CAL)
			{
				nPatNum = 1;
				pGOM = &m_pData->m_GOM[nSelectedROI];
				pPAT = (LPBYTE)m_pParent->m_pModelData->RegisterPattern(nSelectedROI, m_pImage, (long)(pGOM->m_ptPoint1.x+0.5), (long)(pGOM->m_ptPoint1.y+0.5),
																								(long)(pGOM->m_ptPoint4.x+0.5), (long)(pGOM->m_ptPoint4.y+0.5),
																								 m_pData->m_nImageWidth, m_pData->m_nImageHeight, nPatNum, &nPatWidth, &nPatHeight);
				pGOM->m_dAngle = 0.0f;
				pGOM->m_ptPoint2.x = pGOM->m_ptPoint4.x;
				pGOM->m_ptPoint2.y = pGOM->m_ptPoint1.y;
				pGOM->m_ptPoint3.x = pGOM->m_ptPoint1.x;
				pGOM->m_ptPoint3.y = pGOM->m_ptPoint4.y;


				m_pData->m_INSP[nSelectedROI].m_nInspectType = INSP_CAMERA_CAL;
				m_pData->m_INSP[nSelectedROI].m_nPatWidth = nPatWidth;
				m_pData->m_INSP[nSelectedROI].m_nPatHeight = nPatHeight;
				m_pData->m_INSP[nSelectedROI].m_nPatNum = nPatNum;

				m_pParent->m_pModuleLink->ExecuteInspectionROI(nSelectedROI);
				pGOM->m_dCX = m_pData->m_INSP[nSelectedROI].m_dCX;
				pGOM->m_dCY = m_pData->m_INSP[nSelectedROI].m_dCY;

				m_pParent->m_pDlgVisionSetup->m_pInspectDlg->DisplayImageToFrame(pPAT, nPatWidth, nPatHeight);
			}
		}
		else
		{
			if (m_pParent->m_nLngSel==0) MessageBox(_T(" 검사항목(ROI)가 선택되지 않았거나 Title이 일치하지 않습니다. "));
			else MessageBox(_T(" ROI has not been selected, or the Title does not match. "));
		}
		
		m_pParent->m_pDlgVisionSetup->m_pInspectDlg->UpdateSelectItemData();
		m_pParent->Invalidate(false);
	}
}
