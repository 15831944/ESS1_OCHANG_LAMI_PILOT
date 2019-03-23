
#include "stdafx.h"
#include "DlgInspectSetup.h"
#include "DlgVDisp.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInspectSetup dialog
CDlgInspectSetup::CDlgInspectSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInspectSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInspectSetup)
	m_editLowLimit = _T("");
	m_editHighLimit = _T("");
	m_editOffsetV = _T("");
	m_dAngle = 0.0f;
	m_nToggle = FALSE;
	m_bHScrollFirst = FALSE;
	m_bHScrollSecond = FALSE;
	m_bVScrollFirst = FALSE;
	m_bVScrollSecond = FALSE;
	//}}AFX_DATA_INIT

	m_pParent = (CDlgVDisp*)pParent;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelDataPtr();
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelBackDataPtr();
	m_nSelTab = TAB_PROFILE;
}

CDlgInspectSetup::~CDlgInspectSetup()
{
	delete m_pImageBMP;
	delete m_fmImage;
    delete m_pPatternDlg;
	delete m_pEtcDlg;
	delete m_pBlobDlg;
	//delete m_pFoldingDlg;
	/*
	delete m_pMarkDlg;
	delete m_pDefectDlg;
	*/
    delete m_pProfileDlg;
//	delete m_pSLineDlg;
}

void CDlgInspectSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgInspectSetup)
	DDX_Control(pDX, IDC_TAB_MULTI, m_ctrlTab);
	DDX_Control(pDX, IDC_FRAME_PROFILE, m_fmProfile);
	DDX_Control(pDX, IDC_BTN_ITEM_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BTN_ITEM_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_ITEM_TEST, m_btnTest);
	DDX_Text(pDX, IDC_EDIT_LOW_LIMIT, m_editLowLimit);
	DDX_Text(pDX, IDC_EDIT_HIGH_LIMIT, m_editHighLimit);
	DDX_Text(pDX, IDC_EDIT_OFFSET_VALUE, m_editOffsetV);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_TITLE, m_comboTitle);
}

BEGIN_MESSAGE_MAP(CDlgInspectSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgInspectSetup)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ITEM_MODIFY, OnBtnItemModify)
	ON_BN_CLICKED(IDC_BTN_ITEM_APPLY, OnBtnItemApply)
	ON_BN_CLICKED(IDC_BTN_ITEM_TEST, OnBtnItemTest)
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MULTI, OnSelchangeMultiTab)
	ON_BN_CLICKED(IDC_CHECK_DISP_ROI, OnCheckDispRoi)
	ON_WM_CREATE()
//	ON_BN_CLICKED(IDC_BTN_ITEM_CW, OnBtnItemCw)
//	ON_BN_CLICKED(IDC_BTN_ITEM_CCW, OnBtnItemCcw)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO_TITLE, &CDlgInspectSetup::OnCbnSelchangeComboTitle)
END_MESSAGE_MAP()

int CDlgInspectSetup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgInspectSetup message handlers
BOOL CDlgInspectSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitImageBMP();
	InitControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgInspectSetup::InitImageBMP()
{
	CRect rcBounds;

	m_fmProfile.GetClientRect(&rcBounds);
    m_pParent->m_rcPlotBounds = rcBounds;

	m_nFrameWidth  = rcBounds.Width();
	m_nFrameHeight = rcBounds.Height();

	m_pImageBMP = new CImageBMP();
	m_pImageBMP->Create(m_nFrameWidth, m_nFrameHeight, IMAGE_BAND*IMAGE_DEPTH);

	m_fmImage = new unsigned char[m_nFrameWidth * m_nFrameHeight];
}

void CDlgInspectSetup::InitControls()
{
	m_btnModify.SetIcon(IDI_ICON_MODIFY);
	m_btnModify.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnModify.DrawBorder(FALSE);
	m_btnModify.DrawTransparent();
	
	m_btnApply.SetIcon(IDI_ICON_APPLY);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.DrawBorder(FALSE);
	m_btnApply.DrawTransparent();

	/*
	m_btnTest.SetIcon(IDI_ICON_INSP);
	m_btnTest.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnTest.DrawBorder(FALSE);
	m_btnTest.DrawTransparent();
    */

	m_btnTest.SetIcon(IDI_ICON_SAVE);
	m_btnTest.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnTest.DrawTransparent();

	if (m_pParent->m_nLngSel==0) 
	{
		m_btnModify.SetWindowText(_T("변경"));
		m_btnApply.SetWindowText(_T("적용"));
		m_btnTest.SetWindowText(_T("저장"));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" 허용공차 "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("하한 - 상한"));
		GetDlgItem(IDC_STATIC_COMPENS)->SetWindowText(_T("보정치"));
		GetDlgItem(IDC_STATIC_ROINAME)->SetWindowText(_T("항목명칭"));	

		m_ctrlTab.InsertItem(0, _T(" 거리 측정 "));
//	    m_ctrlTab.InsertItem(1, _T(" 직선 측정 "));
		m_ctrlTab.InsertItem(1, _T(" 패턴 검사 "));
		m_ctrlTab.InsertItem(2, _T(" 면적 검사"));
		//m_ctrlTab.InsertItem(2, _T("TAB 접힘검사"));
		m_ctrlTab.InsertItem(3, _T(" 기타 검사 "));
	}
	else if (m_pParent->m_nLngSel==1) 
	{
		m_btnModify.SetWindowText(_T("MODIFY"));
		m_btnApply.SetWindowText(_T("APPLY"));
		m_btnTest.SetWindowText(_T("SAVE"));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" Tolerrance "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("LSL-USL"));
		GetDlgItem(IDC_STATIC_COMPENS)->SetWindowText(_T("Offset V"));
		GetDlgItem(IDC_STATIC_ROINAME)->SetWindowText(_T("ROI NAME"));

		m_ctrlTab.InsertItem(0, _T(" DIST. MEAS. "));
//	    m_ctrlTab.InsertItem(1, _T(" LINE MEAS. "));
		m_ctrlTab.InsertItem(1, _T("PATTERN INSP."));
		m_ctrlTab.InsertItem(2, _T("BLOB INSP."));
		//m_ctrlTab.InsertItem(2, _T("TAB FOLDING"));
		m_ctrlTab.InsertItem(3, _T("ETC. INSP."));
	}
	else
	{
		m_btnModify.SetWindowText(_T("变更"));
		m_btnApply.SetWindowText(_T("适用"));
		m_btnTest.SetWindowText(_T("保存"));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" 许可公差 "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("下限 - 上限"));
		GetDlgItem(IDC_STATIC_COMPENS)->SetWindowText(_T("补偿"));
		GetDlgItem(IDC_STATIC_ROINAME)->SetWindowText(_T("项目名称"));

		m_ctrlTab.InsertItem(0, _T(" 距離 測定 "));
//	    m_ctrlTab.InsertItem(1, _T(" 直線 測定 "));
		m_ctrlTab.InsertItem(1, _T("PATTERN MATCH"));
		m_ctrlTab.InsertItem(2, _T("BLOB INSP."));
	//	m_ctrlTab.InsertItem(2, _T("TAB FOLDING"));
		m_ctrlTab.InsertItem(3, _T("ETC. INSP."));
	}
		
    m_pProfileDlg = new CDlgSubProfile((CWnd*)m_pParent);
//	m_pSLineDlg = new CDlgSubSLine((CWnd*)m_pParent);
    m_pPatternDlg = new CDlgSubPattern((CWnd*)m_pParent);
	m_pBlobDlg = new CDlgSubBlob((CWnd*)m_pParent);
//	m_pFoldingDlg = new CDlgSubFolding((CWnd*)m_pParent);
	m_pEtcDlg = new CDlgSubEtc((CWnd*)m_pParent);

	/*
	m_pMarkDlg = new CDlgSubMark((CWnd*)m_pParent);
    m_pDefectDlg = new CDlgSubDefect((CWnd*)m_pParent);
	*/

	m_pProfileDlg->Create(IDD_DLG_SUB_PROFILE, GetDlgItem(IDC_FRAME_MULTI));
	m_pProfileDlg->ShowWindow(SW_HIDE);
	m_pProfileDlg->ShowWindow(SW_SHOW);

//	m_pSLineDlg->Create(IDD_DLG_SUB_SLINE, GetDlgItem(IDC_FRAME_MULTI));
	m_pPatternDlg->Create(IDD_DLG_SUB_PATTERN, GetDlgItem(IDC_FRAME_MULTI));
	m_pBlobDlg->Create(IDD_DLG_SUB_BLOB, GetDlgItem(IDC_FRAME_MULTI));
//	m_pFoldingDlg->Create(IDD_DLG_SUB_FOLDING, GetDlgItem(IDC_FRAME_MULTI));
	m_pEtcDlg->Create(IDD_DLG_SUB_ETC, GetDlgItem(IDC_FRAME_MULTI));
	/*
	m_pMarkDlg->Create(IDD_DLG_SUB_MARK, GetDlgItem(IDC_FRAME_MULTI));
	m_pDefectDlg->Create(IDD_DLG_SUB_DEFECT, GetDlgItem(IDC_FRAME_MULTI));
	m_pEtcDlg->Create(IDD_DLG_SUB_ETC, GetDlgItem(IDC_FRAME_MULTI));
	*/
}

void CDlgInspectSetup::OnBtnItemModify() 
{
#if 0
	long nSelectedROI;
	CString strTitle, strT;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		GetDlgItem(IDC_COMBO_TITLE)->GetWindowText(strTitle);
		strT = WithoutSpaceToString(strTitle);
		_tcscpy_s(m_pData->m_GOM[nSelectedROI].m_strTitle, strT);
		_tcscpy_s(m_pBackData->m_GOM[nSelectedROI].m_strTitle, strT);

		SetComboTitle(nSelectedROI);
	}

	m_pParent->Invalidate(FALSE);  
#endif // 0
	CString  strMessage;
	strMessage = _T(" 주의!, 모든 Min,Max Threshold 값이 기본값으로 바뀝니다. 실행 하시겠읍니까?, All Min,Max Thr Values will be as default value ! Apply ?  ");
	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		for(int i = 0 ; i < GOM_MAX_NUM; i++) {
			m_pData->m_INSP[i].m_nMaxThr = 255;
			m_pData->m_INSP[i].m_nMinThr = 0;
		}
	}  

}

void CDlgInspectSetup::OnBtnItemApply()
{
	CString  strMessage, strTitle;
//	BOOL bCheck;
	long nSelectedROI, nTab;

 	UpdateData(true);
	nTab = m_ctrlTab.GetCurSel();

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 변경된 데이타를 적용하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want apply the changed data ?  ");
	else strMessage = _T("  要保存变更的参数吗 ? ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
		if (nSelectedROI>=0)
		{
			GetDlgItem(IDC_COMBO_TITLE)->GetWindowText(strTitle);
			strTitle = WithoutSpaceToString(strTitle);
			_tcscpy_s(m_pData->m_GOM[nSelectedROI].m_strTitle, strTitle);

			m_pData->m_INSP[nSelectedROI].m_dStdSpec = (float)_ttof(m_editLowLimit);
			m_pData->m_INSP[nSelectedROI].m_dToleSpec = (float)_ttof(m_editHighLimit);
//			m_pData->m_INSP[nSelectedROI].m_dStandLowLimit = m_pData->m_INSP[nSelectedROI].m_dStdSpec - m_pData->m_INSP[nSelectedROI].m_dToleSpec;
//	    	m_pData->m_INSP[nSelectedROI].m_dStandHighLimit = m_pData->m_INSP[nSelectedROI].m_dStdSpec + m_pData->m_INSP[nSelectedROI].m_dToleSpec;
//			m_pData->m_GOM[nSelectedROI].m_dAngle = m_dAngle;

			m_pData->m_INSP[nSelectedROI].m_dOffsetV = (float)_ttof(m_editOffsetV);
			m_pData->m_INSP[nSelectedROI].m_dStandLowLimit = (float)_ttof(m_editLowLimit);
	    	m_pData->m_INSP[nSelectedROI].m_dStandHighLimit = (float)_ttof(m_editHighLimit);
			m_pParent->m_pModelData->SetLimitData(strTitle, m_pData->m_INSP[nSelectedROI].m_dStandLowLimit, m_pData->m_INSP[nSelectedROI].m_dStandHighLimit);

//			bCheck = ((CButton *)GetDlgItem(IDC_CHECK_DISP_ROI))->GetCheck();
//	    	m_pData->m_GOM[nSelectedROI].m_bExecShowROI = bCheck;

			switch(nTab)
			{
				case TAB_PROFILE: 
					m_pProfileDlg->ApplyInspData(nSelectedROI);
					break;
//				case TAB_SLINE: 
//					m_pSLineDlg->ApplyInspData(nSelectedROI);
//					break;
				case TAB_PATTERN: 
					m_pPatternDlg->ApplyInspData(nSelectedROI);
					break;
//				case TAB_FOLDING: 
//					m_pFoldingDlg->ApplyInspData(nSelectedROI);
//					break;
				case TAB_ETC: 
					m_pEtcDlg->ApplyInspData(nSelectedROI);
					break;
				case TAB_BLOB: 
	    			m_pBlobDlg->ApplyInspData(nSelectedROI);
					break;
					/*
				case TAB_MARK: 
	    			m_pMarkDlg->ApplyInspData(nSelectedROI);
					break;
				case TAB_DEFECT: 
	    			m_pDefectDlg->ApplyInspData(nSelectedROI);
					break;
				case TAB_ETC:
					m_pEtcDlg->ApplyInspData(nSelectedROI);
					break;
					*/
			}

            m_pParent->m_pModelData->CopySelectedROI(nSelectedROI, m_pData, m_pBackData);
		}
		else
		{
			if (m_pParent->m_nLngSel==0) MessageBox(_T(" 검사항목(ROI)가 선택되지 않았거나 Title이 일치하지 않습니다. "));
			else if (m_pParent->m_nLngSel==1) MessageBox(_T(" ROI has not been selected, or the Title does not match. "));
			else MessageBox(_T(" 检查项目未被选定，或与Title不一致 "));
		}
	}

	UpdateSelectItemData();
	m_pParent->Invalidate(FALSE);
}

void CDlgInspectSetup::OnBtnItemTest() 
{
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 변경된 데이타를 저장하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want save the changed data ?  ");
	else strMessage = _T("  要保存变更的参数吗 ? ");

	if (MessageBox(strMessage, _T(" Message Box  "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		memcpy(&m_pBackData->m_IMG, &m_pData->m_IMG, sizeof(IMAGE_DATA));

		m_pParent->m_pModelData->CopyModelData(m_pBackData, m_pData);
		m_pParent->m_pModelData->SaveModelData(m_pParent->m_strFullPathName);
		m_pParent->SetEventFuncCall(DEF_CONT_SAVE_SETUP);

		m_pParent->Invalidate(false);
	}
}

void CDlgInspectSetup::OnSelchangeMultiTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ChangeControlTAB(m_ctrlTab.GetCurSel());

    InvalidateRect(m_pParent->m_rcPlotBounds, false);

	*pResult = 0;
}

void CDlgInspectSetup::ChangeControlTAB(long nTab)
{
	m_nSelTab = nTab;
    m_ctrlTab.SetCurSel(nTab);

	m_pProfileDlg->ShowWindow(SW_HIDE);
//	m_pSLineDlg->ShowWindow(SW_HIDE);
	m_pPatternDlg->ShowWindow(SW_HIDE);
//	m_pFoldingDlg->ShowWindow(SW_HIDE);
	m_pBlobDlg->ShowWindow(SW_HIDE);
	m_pEtcDlg->ShowWindow(SW_HIDE);
	/*
	m_pMarkDlg->ShowWindow(SW_HIDE);
	m_pDefectDlg->ShowWindow(SW_HIDE);
	m_pEtcDlg->ShowWindow(SW_HIDE);
	*/

	switch(nTab)
	{
		case TAB_PROFILE: 
				m_pProfileDlg->ShowWindow(SW_SHOW);
				break;
//		case TAB_SLINE: 
//				m_pSLineDlg->ShowWindow(SW_SHOW);
//				break;
		case TAB_PATTERN: 
				m_pPatternDlg->ShowWindow(SW_SHOW);
				break;
//		case TAB_FOLDING: 
//				m_pFoldingDlg->ShowWindow(SW_SHOW);
//				break;
		case TAB_ETC: 
				m_pEtcDlg->ShowWindow(SW_SHOW);
				break;
		case TAB_BLOB: 
				m_pBlobDlg->ShowWindow(SW_SHOW);
				break;
		/*
		case TAB_MARK: 
				m_pMarkDlg->ShowWindow(SW_SHOW);
				break;
		case TAB_DEFECT: 
				m_pDefectDlg->ShowWindow(SW_SHOW);
				break;
		case TAB_ETC: 
				m_pEtcDlg->ShowWindow(SW_SHOW);
				break;
				*/
	}

	if (nTab==TAB_PROFILE)
	{
//		GetDlgItem(IDC_VERTICAL_SLIDER)->EnableWindow(true);
//		GetDlgItem(IDC_HORIZON_SLIDER)->EnableWindow(true);
	}
	/*
	else
	{
		m_ctrlVertSlider.SetRange(BLACK_LEVEL, WHITE_LEVEL);
		m_ctrlHoriSlider.SetRange(BLACK_LEVEL, WHITE_LEVEL);
		GetDlgItem(IDC_VERTICAL_SLIDER)->EnableWindow(false);
		GetDlgItem(IDC_HORIZON_SLIDER)->EnableWindow(false);
	}
	*/
}

void CDlgInspectSetup::UpdateSelectItemData()
{
	CString strSelectedTitle;
	long nSelectedROI, nSelTab, nType;

	nSelTab = TAB_PROFILE;
	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	strSelectedTitle = m_pParent->m_pModelData->GetSelectedTitle();
	if (nSelectedROI>=0)
	{
		((CButton *)GetDlgItem(IDC_CHECK_DISP_ROI))->SetCheck(m_pData->m_GOM[nSelectedROI].m_bExecShowROI);
		nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
//		m_editLowLimit.Format(_T("%3.2f"), m_pData->m_INSP[nSelectedROI].m_dStdSpec);
//		m_editHighLimit.Format(_T("%3.2f"), m_pData->m_INSP[nSelectedROI].m_dToleSpec);
		m_editOffsetV.Format(_T("%3.2f"), m_pData->m_INSP[nSelectedROI].m_dOffsetV);
		m_editLowLimit.Format(_T("%3.2f"), m_pData->m_INSP[nSelectedROI].m_dStandLowLimit);
		m_editHighLimit.Format(_T("%3.2f"), m_pData->m_INSP[nSelectedROI].m_dStandHighLimit);
//		m_dAngle = m_pData->m_GOM[nSelectedROI].m_dAngle;

		switch(nType)
		{
			case INSP_MEASURE_SIDE_ONE :
			case INSP_MEASURE_SIDE_TWO :
			case INSP_SLINE :
			case INSP_MISMATCH :
    			nSelTab = TAB_PROFILE;
				break;
//			case INSP_SLINE :
//			case INSP_DATUM_B :
//			case INSP_DATUM_C :
//			case INSP_STRAIGHTNESS :
//    			nSelTab = TAB_SLINE;
//				break;
			case INSP_PATTERN_MATCH :
				nSelTab = TAB_PATTERN;
				break;
//			case INSP_TAB_FOLDING :
//				nSelTab = TAB_FOLDING;
//				break;
			case INSP_CAMERA_CAL :
			case INSP_GRAY_VALUE :
				nSelTab = TAB_ETC;
				break;
			case INSP_BLOB_CIRCLE :
			case INSP_BLOB_ANALYSIS :
				nSelTab = TAB_BLOB;
				break;
				/*
			case INSP_MARK :
				nSelTab = TAB_MARK;
				break;
			case INSP_DEFECT_NORMAL :
			case INSP_DEFECT_CIRCLE :
				nSelTab = TAB_DEFECT;
				break;
			case INSP_GRAY_VALUE :
			case INSP_SHARPNESS_VALUE :
			case INSP_IS_OBJECT : 
				nSelTab = TAB_ETC;
				break;
				*/
		}

		ChangeControlTAB(nSelTab);
	}

	SetComboTitle(nSelectedROI);

	UpdateData(false);
    InvalidateRect(m_pParent->m_rcPlotBounds, false);
}

void CDlgInspectSetup::SetComboTitle(long nROI)
{
	long i, nPos, nCnt;

	m_comboTitle.ResetContent();

	nPos = -1;
	nCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")))
		{
			m_comboTitle.AddString(m_pData->m_GOM[i].m_strTitle);

			if (i==nROI) nPos = nCnt;
			nCnt++;
		}
	}

	if (nPos>=0)
        m_comboTitle.SetCurSel(nPos);
}

void CDlgInspectSetup::OnCbnSelchangeComboTitle()
{
	long i, nPos, nCnt;

	nPos = m_comboTitle.GetCurSel();
	GetDlgItem(IDC_BTN_ITEM_APPLY)->SetFocus();

	nCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pData->m_GOM[i].m_bSelectedROI = FALSE;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")))
		{
			if (nPos == nCnt)
				m_pData->m_GOM[i].m_bSelectedROI = TRUE;

			nCnt++;
		}
	}

	UpdateSelectItemData();
	m_pParent->InvalidateView();
}

void CDlgInspectSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcBounds;
	CDC *pDC, memDC;
    CBitmap  bmpBuffer;
	CBitmap *pOldBitmap;


    pDC = m_fmProfile.GetDC();
	m_fmProfile.GetClientRect(&rcBounds);
	m_pParent->m_rcPlotBounds = rcBounds;

	memDC.CreateCompatibleDC(pDC);
    bmpBuffer.CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height()); 
    pOldBitmap = (CBitmap*)memDC.SelectObject(&bmpBuffer); 

	
	if (m_nSelTab == TAB_PROFILE) 
		m_pProfileDlg->DrawProfile(&memDC);
//	else if (m_nSelTab == TAB_SLINE)
//		m_pSLineDlg->DrawProfile(&memDC);
	else 
	{
		//{BEGIN : 화면에 영상그리기 >>>>>>>>>>>>>>>>>>>>>>>>
		CRect rc(0, 0, rcBounds.Width(), rcBounds.Height());
		m_pImageBMP->Draw(memDC.m_hDC, &rc, &rc);
		//}END : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	

	pDC->BitBlt(0, 0, rcBounds.Width(), rcBounds.Height(), &memDC, 0, 0, SRCCOPY); 
    memDC.SelectObject(pOldBitmap); 

	m_fmProfile.ReleaseDC(pDC);
}

BOOL CDlgInspectSetup::SetImageMapping(LPBYTE fm_R, LPBYTE fm_G, LPBYTE fm_B)
{
	long i, j;
	LPBYTE fm;

	fm = m_pImageBMP->GetImagePtr();

	for(i=0; i<m_nFrameHeight; i++) 
	for(j=0; j<m_nFrameWidth; j++) 
	{
		*(fm + m_nFrameWidth*(m_nFrameHeight-i-1)+j) = *(fm_R + m_nFrameWidth*i + j);
    }

	return TRUE;
}

void CDlgInspectSetup::DisplayImageToFrame(LPBYTE pImage, long nImageWidth, long nImageHeight)
{
	long   i, j, x, y;
	double rateX, rateY, dImageRate;
	LPBYTE fmG=NULL;
	LPBYTE fmB=NULL;

	rateX = (double)nImageWidth / (double)m_nFrameWidth;
	rateY = (double)nImageHeight/ (double)m_nFrameHeight;

	if (rateX>rateY) dImageRate=rateX;
	else             dImageRate=rateY;

	memset(m_fmImage, 0, m_nFrameWidth*m_nFrameHeight);
	if (pImage)
	{
		for(i=0; i<m_nFrameHeight; i++)
		for(j=0; j<m_nFrameWidth; j++)
		{
			y = (long)(i*dImageRate + 0.5);
			x = (long)(j*dImageRate + 0.5);

			if (y>=nImageHeight || x>=nImageWidth)
				m_fmImage[i*m_nFrameWidth + j] = MEDIUM_LEVEL;
			else
				m_fmImage[i*m_nFrameWidth + j] = pImage[y*nImageWidth + x];
		}
	}
	else
	{
		for(i=0; i<m_nFrameHeight; i++)
		for(j=0; j<m_nFrameWidth; j++)
		    m_fmImage[i*m_nFrameWidth + j] = MEDIUM_LEVEL;
	}

	SetImageMapping(m_fmImage, fmG, fmB);
	InvalidateRect(m_pParent->m_rcPlotBounds, false);
}

void CDlgInspectSetup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		UpdateSelectItemData();

	m_pParent->Invalidate(FALSE);
}

void CDlgInspectSetup::OnCheckDispRoi() 
{

}

void CDlgInspectSetup::OnBtnItemCw() 
{
	/*
	CString str;
	long nSelectedROI;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_dAngle += 0.5f;  
		m_pParent->m_pModelData->SetTransRectROI(nSelectedROI, m_dAngle);
	}

    m_pParent->Invalidate(false);
	*/
}

void CDlgInspectSetup::OnBtnItemCcw() 
{
	/*
	CString str;
	long nSelectedROI;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_dAngle -= 0.5f;
		m_pParent->m_pModelData->SetTransRectROI(nSelectedROI, m_dAngle);

//		m_dAngle = m_pData->m_GOM[nSelectedROI].m_dAngle;
//		str.Format("%d", (long)m_dAngle);
//		GetDlgItem(IDC_STATIC_ANGLE)->SetWindowText(str);
	}

    m_pParent->Invalidate(false);
	*/
}

void CDlgInspectSetup::OnLButtonDown(UINT nFlags, CPoint point)
{
	long nTop, nBot, nL, nR;
	CRect rcBounds;

	WINDOWPLACEMENT wp;
	RECT rc;

	m_fmProfile.GetWindowPlacement(&wp);
	rc = wp.rcNormalPosition;

	nTop = m_pProfileDlg->m_rtScrollH.top + rc.top;
	nBot = m_pProfileDlg->m_rtScrollH.bottom + rc.top;
	nL = (long)m_pProfileDlg->m_fFirstH + rc.left;
	nR = (long)m_pProfileDlg->m_fSecondH + rc.left;

	if (nTop<point.y && point.y<nBot &&
		nL-5<point.x && point.x<nL+5)
		m_bHScrollFirst = TRUE;
	if (nTop<point.y && point.y<nBot &&
		nR-5<point.x && point.x<nR+5)
		m_bHScrollSecond = TRUE;

	nL = m_pProfileDlg->m_rtScrollV.left + rc.left;
	nR = m_pProfileDlg->m_rtScrollV.right + rc.left;
	nTop = (long)m_pProfileDlg->m_fFirstV + rc.top;
	nBot = (long)m_pProfileDlg->m_fSecondV + rc.top;

	if (nL<point.x && point.x<nR &&
		nTop-5<point.y && point.y<nTop+5)
		m_bVScrollFirst = TRUE;
	if (nL<point.x && point.x<nR &&
		nBot-5<point.y && point.y<nBot+5)
		m_bVScrollSecond = TRUE;

	m_ptLStartPnt = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgInspectSetup::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bHScrollFirst = FALSE;
	m_bHScrollSecond = FALSE;
	m_bVScrollFirst = FALSE;
	m_bVScrollSecond = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgInspectSetup::OnMouseMove(UINT nFlags, CPoint point)
{
	float fSwap;
	CRect rc;

	if (nFlags & MK_LBUTTON)
	{
		m_ptLDelta = point-m_ptLStartPnt;
		m_ptLStartPnt = point;

		//////////////////      SCROLL X 설정       //////////////////
		if (m_bHScrollFirst)  m_pProfileDlg->m_fFirstH += m_ptLDelta.x;
		else if (m_bHScrollSecond) m_pProfileDlg->m_fSecondH += m_ptLDelta.x;
		if (m_pProfileDlg->m_fSecondH<m_pProfileDlg->m_fFirstH)
		{
			fSwap = m_pProfileDlg->m_fFirstH;
			m_pProfileDlg->m_fFirstH = m_pProfileDlg->m_fSecondH;
			m_pProfileDlg->m_fSecondH = fSwap;
			if (m_bHScrollFirst) 
			{
				m_bHScrollFirst = FALSE;
				m_bHScrollSecond = TRUE;
			}
			else
			{
				m_bHScrollFirst = TRUE;
				m_bHScrollSecond = FALSE;
			}			
		}

		if (m_pProfileDlg->m_fFirstH<m_pProfileDlg->m_rtScrollH.left) 
			m_pProfileDlg->m_fFirstH = (float)m_pProfileDlg->m_rtScrollH.left;
		if (m_pProfileDlg->m_fSecondH>m_pProfileDlg->m_rtScrollH.right) 
			m_pProfileDlg->m_fSecondH = (float)m_pProfileDlg->m_rtScrollH.right;
		//////////////////      SCROLL X 설정       //////////////////

		//////////////////      SCROLL Y 설정       //////////////////
		if (m_bVScrollFirst)  m_pProfileDlg->m_fFirstV += m_ptLDelta.y;
		else if (m_bVScrollSecond) m_pProfileDlg->m_fSecondV += m_ptLDelta.y;
		if (m_pProfileDlg->m_fSecondV<m_pProfileDlg->m_fFirstV)
		{
			fSwap = m_pProfileDlg->m_fFirstV;
			m_pProfileDlg->m_fFirstV = m_pProfileDlg->m_fSecondV;
			m_pProfileDlg->m_fSecondV = fSwap;
			if (m_bVScrollFirst) 
			{
				m_bVScrollFirst = FALSE;
				m_bVScrollSecond = TRUE;
			}
			else
			{
				m_bVScrollFirst = TRUE;
				m_bVScrollSecond = FALSE;
			}			
		}

		if (m_pProfileDlg->m_fFirstV<m_pProfileDlg->m_rtScrollV.top) 
			m_pProfileDlg->m_fFirstV = (float)m_pProfileDlg->m_rtScrollV.top;
		if (m_pProfileDlg->m_fSecondV>m_pProfileDlg->m_rtScrollV.bottom) 
			m_pProfileDlg->m_fSecondV = (float)m_pProfileDlg->m_rtScrollV.bottom;
		//////////////////      SCROLL Y 설정       //////////////////

		InvalidateRect(m_pParent->m_rcPlotBounds, false);
	}

	CDialog::OnMouseMove(nFlags, point);
}




