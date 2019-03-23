// DlgSplash.cpp : implementation file
//
#include "stdafx.h"
#include "NLaminateApp.h"
#include "MainFrm.h"
#include "DlgSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame    *m_pMainFrm;
/////////////////////////////////////////////////////////////////////////////
// CDlgSplash dialog
CDlgSplash::CDlgSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSplash)
	//}}AFX_DATA_INIT
	m_nFinish = 0;
}

void CDlgSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSplash)
	DDX_Control(pDX, IDC_LBL_INITIAL_MESSAGE, m_panMessage);
	DDX_Control(pDX, IDC_LBL_VERSION, m_panVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSplash, CDialog)
	//{{AFX_MSG_MAP(CDlgSplash)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_SPLASH_INIT, OnSplashInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash message handlers

BOOL CDlgSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSplash::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect client;
	dc.GetClipBox(client);
}

LRESULT CDlgSplash::OnSplashInit(WPARAM wParam, LPARAM lParam)
{
	CString str;

	m_panVersion.SetCaption(MAIN_WINDOW_TITLE);

	switch (wParam)
	{
		case ID_LOADING_FILE:
			m_panMessage.SetCaption(TEXT(" Loading system file ..."));
			((CNLaminateAppApp*) AfxGetApp())->ExecMessageHandleAll();
			Sleep(100);
			break;
		case ID_INITIAL_CONTROL:
			m_panMessage.SetCaption(TEXT(" Initialize system controls ..."));
			((CNLaminateAppApp*) AfxGetApp())->ExecMessageHandleAll();
			Sleep(100);
			break;
		case ID_INITIAL_FINISH:
			m_panMessage.SetCaption(TEXT(" Completing system paramiter ..."));
			((CNLaminateAppApp*) AfxGetApp())->SetInitFinished();
			Sleep(100);
			break;
	}

	return 1;
}

