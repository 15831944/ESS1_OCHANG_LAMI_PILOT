
// NLaminateAppView.cpp : CNLaminateAppView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "NLaminateApp.h"
#include "MainFrm.h"
#endif

#include "NLaminateAppDoc.h"
#include "NLaminateAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CMainFrame *m_pMainFrm;
// CNLaminateAppView

IMPLEMENT_DYNCREATE(CNLaminateAppView, CFormView)

BEGIN_MESSAGE_MAP(CNLaminateAppView, CFormView)
	ON_MESSAGE(WM_DELETE_DATA, OnDeleteDataPath)
END_MESSAGE_MAP()

// CNLaminateAppView 생성/소멸

CNLaminateAppView::CNLaminateAppView()
	: CFormView(CNLaminateAppView::IDD)
{
	m_pDlgMainBar = NULL;
	// TODO: 여기에 생성 코드를 추가합니다.
}

CNLaminateAppView::~CNLaminateAppView()
{
	delete m_pDlgExecAuto;
	delete m_pDlgResult;
    delete m_pDlgSetting;
    delete m_pDlgQuery;
    delete m_pDlgPlot;

	delete m_pDlgMainBar;
	delete m_pDlgSubBar;
#if _SHORTAGE_USE
	delete m_pDlgTestShort;
#endif
}

void CNLaminateAppView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DIALOG_SCREEN, m_fmDialog);
}

BOOL CNLaminateAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CNLaminateAppView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();

	m_pMainFrm = (CMainFrame *)AfxGetMainWnd();
	m_pMainFrm->m_pView = this;

    m_fmDialog.SetWindowPos(NULL, DEF_DIALOG_LEFT, DEF_DIALOG_TOP, DEF_DIALOG_WIDTH, DEF_DIALOG_HEIGHT, SWP_SHOWWINDOW);

	InitDialogs();
//  ::PostMessage(this->m_hWnd, WM_DELETE_DATA, 0, 0);
}

void CNLaminateAppView::InitDialogs()
{
    m_pDlgExecAuto = new CDlgExecAuto((CWnd*)this);
    m_pDlgSetting = new CDlgSetting((CWnd*)this);
	m_pDlgResult = new CDlgResult((CWnd*)this);
    m_pDlgPlot = new CDlgPlot((CWnd*)this);
    m_pDlgQuery = new CDlgQuery((CWnd*)this);

	m_pDlgExecAuto->Create(IDD_DIALOG_EXEC_AUTO, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgExecAuto->ShowWindow(SW_SHOW);
	m_pDlgPlot->Create(IDD_DIALOG_PLOT, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgPlot->ShowWindow(SW_SHOW);
	m_pDlgSetting->Create(IDD_DIALOG_SETUP, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgResult->Create(IDD_DIALOG_RESULT, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgQuery->Create(IDD_DIALOG_QUERY, GetDlgItem(IDC_STATIC_DIALOG_SCREEN));
	m_pDlgQuery->InitPlotControl(0);

	SelectDialog(DEF_BUTTON_AUTO);

    m_pDlgMainBar = new CDlgMainBar((CWnd*)this);
	GetDlgItem(IDC_STATIC_MAIN_BAR)->SetWindowPos(NULL, DEF_MAINBAR_LEFT, DEF_MAINBAR_TOP, DEF_MAINBAR_WIDTH, DEF_MAINBAR_HEIGHT, SWP_SHOWWINDOW);
	m_pDlgMainBar->Create(IDD_DIALOG_MAIN_BAR, GetDlgItem(IDC_STATIC_MAIN_BAR));
	m_pDlgMainBar->ShowWindow(SW_SHOW);

    m_pDlgSubBar = new CDlgSubBar((CWnd*)this);
	GetDlgItem(IDC_STATIC_SUB_BAR)->SetWindowPos(NULL, DEF_SUBBAR_LEFT, DEF_SUBBAR_TOP, DEF_SUBBAR_WIDTH, DEF_SUBBAR_HEIGHT, SWP_SHOWWINDOW);
    m_pDlgSubBar->Create(IDD_DIALOG_SUB_BAR, GetDlgItem(IDC_STATIC_SUB_BAR));
	m_pDlgSubBar->ShowWindow(SW_SHOW);

#if _SHORTAGE_USE
    m_pDlgTestShort = new CDlgTestShort((CWnd*)this);
	m_pDlgTestShort->Create(IDD_DIALOG_TEST_SHORT, this);
	m_pDlgTestShort->MoveWindow(DEF_TESTSHORT_LEFT, DEF_TESTSHORT_TOP, DEF_TESTSHORT_WIDTH, DEF_TESTSHORT_HEIGHT);
	m_pDlgTestShort->ShowWindow(SW_SHOW);
#endif
}

void CNLaminateAppView::SelectDialog(int nSel)
{
    CString str;

	m_pDlgExecAuto->ShowWindow(SW_HIDE);
	m_pDlgResult->ShowWindow(SW_HIDE);
	m_pDlgSetting->ShowWindow(SW_HIDE);
	//m_pDlgPlot->ShowWindow(SW_HIDE);
	m_pDlgQuery->ShowWindow(SW_HIDE);

	switch(nSel)
	{
		case DEF_BUTTON_AUTO:
        	m_pDlgExecAuto->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[AUTO SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_RESULT:
        	m_pDlgResult->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[RESULT SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_SEARCH:
        	m_pDlgQuery->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[SEARCH SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;

#if 0
		case DEF_BUTTON_PLOT:
			m_pDlgPlot->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[PLOT SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;    
#endif // 0



			/*
		case BUTTON_REPORT:
         	m_pDlgReport->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+"[REPORT 화면]";
			m_pMainFrm->SetWindowText(_T(str));
			break;
			*/
		case DEF_BUTTON_SETUP:
        	m_pDlgSetting->ShowWindow(SW_SHOW);
			str = (CString)MAIN_WINDOW_TITLE+(CString)"[SETTING SCREEN]";
			m_pMainFrm->SetWindowText(str);
			break;
		case DEF_BUTTON_EXIT:
			m_pMainFrm->ExitProgram();
			break;
	}
}

LONG CNLaminateAppView::OnDeleteDataPath(UINT wParam, LONG lParam)
{
	BeginWaitCursor();
//	m_pMainFrm->DeleteDataPath();

  	EndWaitCursor();
	return 0;
}

// CNLaminateAppView 진단
#ifdef _DEBUG
void CNLaminateAppView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNLaminateAppView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

/*
CNLaminateAppDoc* CNLaminateAppView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNLaminateAppDoc)));
	return (CNLaminateAppDoc*)m_pDocument;
}
*/

#endif //_DEBUG
// CNLaminateAppView 메시지 처리기
