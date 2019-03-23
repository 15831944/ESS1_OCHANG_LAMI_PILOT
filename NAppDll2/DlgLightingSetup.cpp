// DlgLightingSetup.cpp : implementation file
//

#include "stdafx.h"
#include "NAppDll.h"
#include "DlgVDisp.h"
#include "DlgLightingSetup.h"
#include "afxdialogex.h"

// CDlgLightingSetup dialog

IMPLEMENT_DYNAMIC(CDlgLightingSetup, CDialog)

CDlgLightingSetup::CDlgLightingSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLightingSetup::IDD, pParent)
{
	m_pParent = (CDlgVDisp*)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelDataPtr();
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelBackDataPtr();
}

CDlgLightingSetup::~CDlgLightingSetup()
{
	long i;

	for(i=0; i<DEF_LIGHT_NUM; i++)
    	if (m_pLightSlide[i])  delete m_pLightSlide[i];

	if (m_pExposureT)  delete m_pExposureT;
}

void CDlgLightingSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgLightingSetup)
	DDX_Control(pDX, IDC_BTN_LIGHT_SAVE, m_btnSave);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgLightingSetup, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_LIGHT_SAVE, &CDlgLightingSetup::OnBnClickedBtnLightSave)
END_MESSAGE_MAP()

// CDlgLightingSetup message handlers
BOOL CDlgLightingSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLightingSetup::InitControls()
{
	long i;
	CString str;

	m_btnSave.SetIcon(IDI_ICON_SAVE);
	m_btnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnSave.DrawTransparent();

	m_pExposureT = new CDlgSliderBarCtrl(GetDlgItem(IDC_STATIC_EXPOSURE_T));
	m_pExposureT->Create(IDD_DLG_SLIDEBAR_CTRL, GetDlgItem(IDC_STATIC_EXPOSURE_T));
	m_pExposureT->m_pWndParent = (CWnd*)m_pParent;
	m_pExposureT->ShowWindow(SW_SHOW);
	m_pExposureT->SetScrollRange(1, 100, 1000);
	m_pExposureT->SetWarningRange(1, 700, 1000, RGB(255, 170, 170));

	for(i=0; i<DEF_LIGHT_NUM; i++)
	{
		m_pLightSlide[i] = new CDlgSliderBarCtrl(GetDlgItem(IDC_STATIC_LIGHT1+i));
		m_pLightSlide[i]->Create(IDD_DLG_SLIDEBAR_CTRL, GetDlgItem(IDC_STATIC_LIGHT1+i));
		m_pLightSlide[i]->m_pWndParent = (CWnd*)m_pParent;
		m_pLightSlide[i]->ShowWindow(SW_SHOW);
		m_pLightSlide[i]->SetScrollRange(1, 0, 999);
	}

	if (m_pParent->m_nLngSel==0) 
	{
		m_btnSave.SetWindowText(_T("저장"));
		for(i=0; i<DEF_LIGHT_NUM; i++)
		{
			str.Format(_T("조명 (CH%d)"), i+1);
		    GetDlgItem(IDC_STATIC_TEXT1+i)->SetWindowText(str);
		}
	}
	else if (m_pParent->m_nLngSel==1) 
	{
		m_btnSave.SetWindowText(_T("SAVE"));
		for(i=0; i<DEF_LIGHT_NUM; i++)
		{
			str.Format(_T("ILLU.(CH%d)"), i+1);
		    GetDlgItem(IDC_STATIC_TEXT1+i)->SetWindowText(str);
		}
	}
	else
	{
		m_btnSave.SetWindowText(_T("保存"));
		for(i=0; i<DEF_LIGHT_NUM; i++)
		{
			str.Format(_T("照明 (CH%d)"), i+1);
		    GetDlgItem(IDC_STATIC_TEXT1+i)->SetWindowText(str);
		}
	}

	for(i=0; i<DEF_LIGHT_NUM; i++)
	{
	    ((CButton *)GetDlgItem(IDC_STATIC_LIGHT1+i))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_STATIC_TEXT1+i))->EnableWindow(FALSE);
	}

	if (m_pParent->m_nCamID==0)
	{
		for(i=0; i<4; i++)
		{
		    ((CButton *)GetDlgItem(IDC_STATIC_LIGHT1+i))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_STATIC_TEXT1+i))->EnableWindow(TRUE);
		}
	}
	else
	{
		for(i=4; i<8; i++)
		{
		    ((CButton *)GetDlgItem(IDC_STATIC_LIGHT1+i))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_STATIC_TEXT1+i))->EnableWindow(TRUE);
		}
	}
}

void CDlgLightingSetup::UpdateControlsData()
{
    long i;

	if (m_pData->m_Enhence.m_nBrightness<100) 
		m_pData->m_Enhence.m_nBrightness = 150;
	if (m_pData->m_Enhence.m_nBrightness>1000) 
		m_pData->m_Enhence.m_nBrightness = 150;

	m_pExposureT->SetScrollPos(0, (float)m_pParent->m_pSYS.m_nExposureTime);
	for(i=0; i<DEF_LIGHT_NUM; i++)
		m_pLightSlide[i]->SetScrollPos(0, (float)m_pParent->m_pSYS.m_LightV[i]);
}

void CDlgLightingSetup::OnBnClickedBtnLightSave()
{
	long i;
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 변경된 데이타를 적용하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want apply the changed data ?  ");
	else strMessage = _T("  要保存变更的参数吗  ");

	if (MessageBox(strMessage, _T(" Message Box  "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pParent->m_pSYS.m_nExposureTime = (long)m_pExposureT->GetScrollPos(0);
		for(i=0; i<DEF_LIGHT_NUM; i++)
			m_pParent->m_pSYS.m_LightV[i] = (long)m_pLightSlide[i]->GetScrollPos(0);

		m_pParent->SaveCalToFile();
		m_pParent->SetEventFuncCall(DEF_CONT_LIGHT);
	}
}

void CDlgLightingSetup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		UpdateControlsData(); 
	}

	m_pParent->Invalidate(FALSE);
}
