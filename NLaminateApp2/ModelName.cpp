// ModelName.cpp : implementation file
//

#include "stdafx.h"
#include "NAlignerApp.h"
#include "ModelName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelName dialog
 

CModelName::CModelName(CWnd* pParent /*=NULL*/)
	: CDialog(CModelName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelName)
	m_sModelName = _T("");
	//}}AFX_DATA_INIT
	m_sTitle=_T("신규 모델명 입력");
}


void CModelName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelName)
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_sModelName);
	//}}AFX_DATA_MAP
} 

BEGIN_MESSAGE_MAP(CModelName, CDialog)
	//{{AFX_MSG_MAP(CModelName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelName message handlers

BOOL CModelName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(m_sTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

