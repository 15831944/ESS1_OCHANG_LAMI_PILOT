// DlgModelName.cpp : 备泅 颇老涝聪促.
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgModelName.h"
#include "MainFrm.h"
#include "afxdialogex.h"

// CDlgModelName 措拳 惑磊涝聪促.
extern CMainFrame    *m_pMainFrm;

CDlgModelName::CDlgModelName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModelName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModelName)
	m_sModelName = _T("");
	//}}AFX_DATA_INIT

	if (m_pMainFrm->m_Setting.m_nSelLanguage==0)
	    m_sTitle=_T(" 모델명 입력 ");
	else if (m_pMainFrm->m_Setting.m_nSelLanguage==1)
		m_sTitle=_T(" Enter new model name ");
	else 
		m_sTitle=_T(" 脚痹 葛胆疙 涝仿 ");
}


void CDlgModelName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModelName)
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_sModelName);
	//}}AFX_DATA_MAP
} 

BEGIN_MESSAGE_MAP(CDlgModelName, CDialogEx)
	//{{AFX_MSG_MAP(CDlgModelName)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgModelName::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModelName message handlers

BOOL CDlgModelName::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(m_sTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModelName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
