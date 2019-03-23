// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "DlgVisionSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CDlgVisionSetup::CDlgVisionSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVisionSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVisionSetup)
	//}}AFX_DATA_INIT
	m_pParent = (CDlgVDisp*)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelDataPtr();
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelBackDataPtr();

	m_nSelTab = 0;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
}

CDlgVisionSetup::~CDlgVisionSetup()
{
    delete m_pMappingDlg;
    delete m_pInspectDlg;
    delete m_pTeachingDlg;
	delete m_pLightingDlg;
}

int CDlgVisionSetup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CDlgVisionSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVisionSetup)
	DDX_Control(pDX, IDC_MULTI_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgVisionSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgVisionSetup)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MULTI_TAB, OnSelchangeMultiTab)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVisionSetup message handlers
BOOL CDlgVisionSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(VISION_SETUP_VERSION);

	InitDialogs();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgVisionSetup::InitDialogs()
{
    m_pMappingDlg = new CDlgMappingSetup((CWnd*)m_pParent);
    m_pInspectDlg = new CDlgInspectSetup((CWnd*)m_pParent);
    m_pTeachingDlg = new CDlgTeachSetup((CWnd*)m_pParent);
	m_pLightingDlg = new CDlgLightingSetup((CWnd*)m_pParent);

	m_pMappingDlg->m_pWndParent = this;
    m_pInspectDlg->m_pWndParent = this;
	m_pTeachingDlg->m_pWndParent = this;
	m_pLightingDlg->m_pWndParent = this;

    m_pInspectDlg->Create(IDD_DLG_INSPECT_SETUP, GetDlgItem(IDC_MULTI_FRAME));
	m_pInspectDlg->ShowWindow(SW_SHOW);
	m_pTeachingDlg->Create(IDD_DLG_TEACH_SETUP, GetDlgItem(IDC_MULTI_FRAME));
	m_pMappingDlg->Create(IDD_DLG_MAPPING_SETUP, GetDlgItem(IDC_MULTI_FRAME));
	m_pLightingDlg->Create(IDD_DLG_LIGHT_SETUP, GetDlgItem(IDC_MULTI_FRAME));
 
	GetSelectedTab();
}

void CDlgVisionSetup::LButtonDown()
{
	if (m_nSelTab==TAB_INSPECT)
		m_pInspectDlg->UpdateSelectItemData();

	if (m_nSelTab==TAB_TEACH)
		m_pTeachingDlg->UpdateSelectItemData();

	if (m_nSelTab==TAB_MAPPING)
		m_pMappingDlg->UpdateControlsData();

	if (m_nSelTab==TAB_LIGHTING)
		m_pLightingDlg->UpdateControlsData();
}

void CDlgVisionSetup::MouseMove(UINT nFlags, CPoint point)
{
    UpdateFrameRect();
}

void CDlgVisionSetup::OnClose() 
{
	long i;

	m_pParent->m_bModifySetup = FALSE;
	m_pBackData->m_bSetupMode = FALSE;

	for(i=0; i<GOM_MAX_NUM; i++)
		m_pBackData->m_GOM[i].m_bSelectedROI = FALSE;

    memcpy(&m_pBackData->m_IMG, &m_pData->m_IMG, sizeof(IMAGE_DATA));
	m_pParent->m_pModelData->CopyModelData(m_pBackData, m_pData);

	m_pParent->LoadCalFromFile();
	m_pParent->Invalidate(false);
	m_pParent->SetEventFuncCall(DEF_CONT_EXIT_SETUP);

	CDialog::OnClose();
}

void CDlgVisionSetup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_pData->m_bSetupMode = TRUE;
		m_pBackData->m_bSetupMode = TRUE;

		CRect rcRect;
		GetWindowRect(&rcRect);
		//MoveWindow(FRAME_WIDTH-60, 60, rcRect.Width(), rcRect.Height());		  // 葛聪磐 2俺 , 谅快肺
		MoveWindow(rcRect.right, 60, rcRect.Width(), rcRect.Height());		  // 葛聪磐 2俺 , 谅快肺
	}
}

void CDlgVisionSetup::OnSelchangeMultiTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nSelTab = GetSelectedTab();

	switch(m_nSelTab)
	{
	case TAB_MAPPING: 
			m_pMappingDlg->ShowWindow(SW_SHOW);
			m_pInspectDlg->ShowWindow(SW_HIDE);
			m_pTeachingDlg->ShowWindow(SW_HIDE);
			m_pLightingDlg->ShowWindow(SW_HIDE);
			break;
	case TAB_INSPECT: 
			m_pMappingDlg->ShowWindow(SW_HIDE);
			m_pInspectDlg->ShowWindow(SW_SHOW);
			m_pTeachingDlg->ShowWindow(SW_HIDE);
			m_pLightingDlg->ShowWindow(SW_HIDE);
			break;
	case TAB_TEACH: 
			m_pMappingDlg->ShowWindow(SW_HIDE);
			m_pInspectDlg->ShowWindow(SW_HIDE);
			m_pTeachingDlg->ShowWindow(SW_SHOW);
			m_pLightingDlg->ShowWindow(SW_HIDE);
			break;
	case TAB_LIGHTING: 
			m_pMappingDlg->ShowWindow(SW_HIDE);
			m_pInspectDlg->ShowWindow(SW_HIDE);
			m_pTeachingDlg->ShowWindow(SW_HIDE);
			m_pLightingDlg->ShowWindow(SW_SHOW);
			break;
	}

	*pResult = 0;
}

long CDlgVisionSetup::GetSelectedTab()
{
    HICON hIcon[4];
	long nSel;
	
	nSel = m_ctrlTab.GetCurSel();
	if (nSel<0) nSel=0;

    hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
    hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
    hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
	hIcon[3] = AfxGetApp()->LoadIcon(IDI_ICON_LED_OFF);
    hIcon[nSel] = AfxGetApp()->LoadIcon(IDI_ICON_LED_ON);

	m_imglistOnOff.DeleteImageList();
	m_imglistOnOff.Create(16, 16, TRUE, 4, 4);
	m_imglistOnOff.Add(hIcon[0]);
	m_imglistOnOff.Add(hIcon[1]);
	m_imglistOnOff.Add(hIcon[2]);
	m_imglistOnOff.Add(hIcon[3]);

	m_ctrlTab.DeleteAllItems();
	m_ctrlTab.SetImageList(&m_imglistOnOff);
	m_ctrlTab.SetMinTabWidth(80);

	TC_ITEM item;
    item.mask = TCIF_IMAGE|TCIF_TEXT;

	item.iImage = 0;
	if (m_pParent->m_nLngSel == 0) item.pszText = _T("검사설정");
	else if (m_pParent->m_nLngSel == 1) item.pszText = _T("INSP. SETUP");
	else item.pszText = _T("检查设定");
	m_ctrlTab.InsertItem(0, &item);

	item.iImage = 1;
	if (m_pParent->m_nLngSel == 0) item.pszText = _T("항목연결");
	else if (m_pParent->m_nLngSel == 1) item.pszText = _T("ITEM LINK");
	else item.pszText = _T("项目连接");
	m_ctrlTab.InsertItem(1, &item);

	item.iImage = 2;
	if (m_pParent->m_nLngSel == 0) item.pszText = _T("CAL.설정");
	else if (m_pParent->m_nLngSel == 1) item.pszText = _T("CAL. SETUP");
	else item.pszText = _T("CAL. 设定");
	m_ctrlTab.InsertItem(2, &item);

	item.iImage = 3;
	if (m_pParent->m_nLngSel == 0) item.pszText = _T("조명설정");
	else if (m_pParent->m_nLngSel == 1) item.pszText = _T("ILLU. SETUP");
	else item.pszText = _T("照明设定");
	m_ctrlTab.InsertItem(3, &item);

	m_ctrlTab.SetCurSel(nSel);
	return nSel;
}

void CDlgVisionSetup::UpdateFrameRect()
{
	if (m_nSelTab==TAB_INSPECT)
	    m_pInspectDlg->InvalidateRect(m_pParent->m_rcPlotBounds, false);
}


