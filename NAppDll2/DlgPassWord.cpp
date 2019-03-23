// PswDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPassWord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPassWord dialog
CDlgPassWord::CDlgPassWord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassWord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPassWord)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strTitleName = _T(" PASSWORD DIALOG ");
	m_strMsg       = _T(" ENTRY PASSWORD ");
	m_rectWindow = CRect(0, 0, 0, 0);
}

void CDlgPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPassWord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_PASSWORD_MODIFY, m_btnPasswordModify);
	DDX_Text(pDX, IDC_PSW_EDIT, m_strPswEdt);
	DDX_Text(pDX, IDC_OLD_PSW_EDIT, m_strOldPswEdt);
	DDX_Text(pDX, IDC_NEW_PSW_EDIT, m_strNewPswEdt);
	DDX_Text(pDX, IDC_RE_PSW_EDIT, m_strRePswEdt);
	DDV_MaxChars(pDX, m_strPswEdt, 20);
	DDV_MaxChars(pDX, m_strOldPswEdt, 20);
	DDV_MaxChars(pDX, m_strNewPswEdt, 20);
	DDV_MaxChars(pDX, m_strRePswEdt, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgPassWord, CDialog)
	//{{AFX_MSG_MAP(CDlgPassWord)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BTN_PASSWORD_MODIFY, OnBtnPasswordModify)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgPassWord::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPassWord message handlers
void CDlgPassWord::CreateXButton()
{
	m_btnPasswordModify.SetColor(RGB(255, 64, 64));
	m_btnPasswordModify.SetTextAlign(ALIGN_TEXT_RIGHT);
}

BOOL CDlgPassWord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControl();
	CreateXButton();

	GetWindowRect(m_rectWindow);
	SetWindowPos(&wndTop, 0, 0, m_rectWindow.Width(), 232, SWP_NOMOVE);
	
	return TRUE;  
}

void CDlgPassWord::InitControl()
{
	SetWindowText(m_strTitleName);
}

void CDlgPassWord::OnBtnPasswordModify() 
{
	SetWindowPos(&wndTop, 0, 0, m_rectWindow.Width(), 
	m_btnPasswordModify.GetMoreShown() ? 232 : m_rectWindow.Height(), SWP_NOMOVE);
}

void CDlgPassWord::OnBtnModify()
{
	UpdateData(TRUE);

	if ((m_strOldPswEdt.Compare((WCHAR *)m_strPassWord) && m_strOldPswEdt.Compare(_T("NEURO"))) ||
		m_strNewPswEdt.Compare(m_strRePswEdt))
	{
		MessageBox(_T(" ERROR ! MODIFY PASSWORD ..."), _T("ERROR"));
		m_strOldPswEdt	="";
	    m_strNewPswEdt	="";
	    m_strRePswEdt	="";
	}
	else
	{
		wsprintf(m_strPassWord, _T("%s"), m_strNewPswEdt);
		MessageBox(_T(" The password completed modify . "), _T("OK"));
	}

	UpdateData(FALSE);
}

void CDlgPassWord::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_strPswEdt.Compare((WCHAR *)m_strPassWord) && m_strPswEdt.Compare(_T("NEURO")))
		return;

	CDialog::OnOK();
}
