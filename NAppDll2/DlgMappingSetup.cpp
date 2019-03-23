// DlgMappingSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMappingSetup.h"
#include "DlgVDisp.h"
#include "DlgPassWord.h"
#include "NPioControlDll.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMappingSetup dialog
CDlgMappingSetup::CDlgMappingSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMappingSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMappingSetup)
	m_chkEnhence = FALSE;
	//}}AFX_DATA_INIT
	m_pParent = (CDlgVDisp*)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelDataPtr();
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelBackDataPtr();
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

	m_bCalProc = FALSE;
	m_bCalProc = FALSE;
	m_nMaxCount = 0;
	for(long i=0; i<IMAGE_LEVEL; i++) 
		m_pProfileData[i] = 0;
}

CDlgMappingSetup::~CDlgMappingSetup()
{
	if (m_pContSlide)  delete m_pContSlide;
	if (m_pBrightSlide) delete m_pBrightSlide;
	if (m_pLightSlide) delete m_pLightSlide;
}

void CDlgMappingSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMappingSetup)
	DDX_Control(pDX, IDC_FRAME_PROFILE, m_fmProfile);
	DDX_Check(pDX, IDC_CHECK_ENHENCE, m_chkEnhence);
	DDX_Control(pDX, IDC_BTN_ITEM_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_MAPPING_SAVE, m_btnSave);	
	DDX_Control(pDX, IDC_EDIT_SCALE_FACTOR, m_editScaleFactor);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_editRealSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMappingSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgMappingSetup)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_ENHENCE, OnCheckEnhence)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ITEM_APPLY, OnBtnItemApply)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CAL, &CDlgMappingSetup::OnBnClickedBtnCal)
	ON_BN_CLICKED(IDC_BTN_EXE_CAL, &CDlgMappingSetup::OnBnClickedBtnExeCal)
	ON_BN_CLICKED(IDC_BTN_MAPPING_SAVE, &CDlgMappingSetup::OnBnClickedBtnMappingSave)
	ON_BN_CLICKED(IDC_BTN_EXE_CANCEL, &CDlgMappingSetup::OnBnClickedBtnExeCancel)
END_MESSAGE_MAP()

BOOL CDlgMappingSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitControls();
	InitialPlotRect();
		
	m_chkEnhence = m_pData->m_Enhence.m_bUseEnhence;
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgMappingSetup::InitControls()
{
	m_btnApply.SetIcon(IDI_ICON_APPLY);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0,0,0));
	m_btnApply.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0,0,0));
	m_btnApply.DrawBorder(FALSE);
	m_btnApply.DrawTransparent();

	m_btnSave.SetIcon(IDI_ICON_SAVE);
	m_btnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnSave.DrawTransparent();

	m_pContSlide = new CDlgSliderBarCtrl(GetDlgItem(IDC_STATIC_CONTRAST));
	m_pContSlide->Create(IDD_DLG_SLIDEBAR_CTRL, GetDlgItem(IDC_STATIC_CONTRAST));
	m_pContSlide->ShowWindow(SW_SHOW);
	m_pContSlide->SetScrollRange(2, 0, 255);

	m_pBrightSlide = new CDlgSliderBarCtrl(GetDlgItem(IDC_STATIC_BRIGHT));
	m_pBrightSlide->Create(IDD_DLG_SLIDEBAR_CTRL, GetDlgItem(IDC_STATIC_BRIGHT));
	m_pBrightSlide->ShowWindow(SW_SHOW);
	m_pBrightSlide->SetScrollRange(1, 0, 255);
	
	m_pLightSlide = new CDlgSliderBarCtrl(GetDlgItem(IDC_STATIC_LIGHT));
	m_pLightSlide->Create(IDD_DLG_SLIDEBAR_CTRL, GetDlgItem(IDC_STATIC_LIGHT));
	m_pLightSlide->ShowWindow(SW_SHOW);
	m_pLightSlide->SetScrollRange(1, 0, 999);

	m_editScaleFactor.SetBkColor(RGB(255, 255, 255));
	m_editScaleFactor.SetFontSize(15);

	m_editRealSize.SetBkColor(RGB(255, 255, 255));
	m_editRealSize.SetFontSize(15);

	GetDlgItem(IDC_BTN_CAL)->ShowWindow(TRUE);
	GetDlgItem(IDC_BTN_EXE_CAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_EXE_CANCEL)->ShowWindow(FALSE);

	if (m_pParent->m_nLngSel==0) 
	{
		m_btnApply.SetWindowText(_T("적용"));
		m_btnSave.SetWindowText(_T("저장"));
		GetDlgItem(IDC_STATIC_ILLU)->SetWindowText(_T("조명"));
	}
	else if (m_pParent->m_nLngSel==1) 
	{
		m_btnApply.SetWindowText(_T("APPLY"));
		m_btnSave.SetWindowText(_T("SAVE"));
		GetDlgItem(IDC_STATIC_ILLU)->SetWindowText(_T("ILLU."));
	}
	else
	{
		m_btnApply.SetWindowText(_T("适用"));
		m_btnSave.SetWindowText(_T("保存"));
		GetDlgItem(IDC_STATIC_ILLU)->SetWindowText(_T("照明"));
	}
}

void CDlgMappingSetup::UpdateControlsData()
{
	long nDir, nH, nIndex,  bRear, nMid;
	float fReal, fScale;
	BOOL bUseEnhence = m_pData->m_Enhence.m_bUseEnhence;         // Mapping 적용 유무
	long nContrastLow = m_pData->m_Enhence.m_nContrastLow;       // Low Position 지정
	long nContrastHigh = m_pData->m_Enhence.m_nContrastHigh;     // High Position 지정
//	long nBrightness = m_pData->m_Enhence.m_nBrightness;         // Brightness Value 지정
//	long nLight = m_pData->m_Enhence.m_nLight;                   // 조명값 
	CString str;

    ((CButton *)GetDlgItem(IDC_CHECK_ENHENCE))->SetCheck(m_pData->m_Enhence.m_bUseEnhence);

	m_pContSlide->SetScrollPos(0, (float)nContrastLow);
	m_pContSlide->SetScrollPos(1, (float)nContrastHigh);
//	m_pBrightSlide->SetScrollPos(0, (float)nBrightness);
//	m_pLightSlide->SetScrollPos(0, (float)nLight);

	nIndex = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nIndex>=0)
	{
		nH = m_pData->m_nImageHeight/2;
    	nDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
		bRear = m_pData->m_INSP[nIndex].m_bLocROI;

		if (nDir==DIR_LEFT_RIGHT || nDir==DIR_RIGHT_LEFT)
		{
			nMid = (long)(m_pData->m_GOM[nIndex].m_ptPoint1.y + m_pData->m_GOM[nIndex].m_ptPoint4.y)/2;

			if (bRear && nMid<nH)   fScale = m_pParent->m_pCAL.m_dScaleRT_X;
			if (bRear && nMid>=nH)  fScale = m_pParent->m_pCAL.m_dScaleRB_X;
			if (!bRear && nMid<nH)  fScale = m_pParent->m_pCAL.m_dScaleLT_X;
			if (!bRear && nMid>=nH) fScale = m_pParent->m_pCAL.m_dScaleLB_X;

			if (nMid<nH) fReal =  m_pParent->m_pCAL.m_dRealT_X; 
			else fReal =  m_pParent->m_pCAL.m_dRealB_X; 
		}
		else
		{
			if (bRear) fScale = m_pParent->m_pCAL.m_dScaleR_Y;
			else  fScale = m_pParent->m_pCAL.m_dScaleL_Y;

			fReal =  m_pParent->m_pCAL.m_dReal_Y; 
		}

		str.Format(_T("%6.5f"), fScale);
	    m_editScaleFactor.SetTextOut(str);
		m_editScaleFactor.UpdateData(false);

		str.Format(_T("%5.3f"), fReal);
		m_editRealSize.SetTextOut(str);
		m_editRealSize.UpdateData(false);
	}
}

void CDlgMappingSetup::OnBtnItemApply()
{
	long i;
	CString strFlag;
	UpdateData(true);

	if (m_pParent->m_nLngSel==0) strFlag = _T(" 변경된 데이타를 적용하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strFlag = _T("  Do you want apply the changed data ?  ");
	else strFlag = _T("  要保存变更的参数吗  ");

	if (MessageBox(strFlag, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pData->m_Enhence.m_bUseEnhence = ((CButton *)GetDlgItem(IDC_CHECK_ENHENCE))->GetCheck();
//		m_pData->m_Enhence.m_nBrightness = (long)m_pBrightSlide->GetScrollPos(0);
		m_pData->m_Enhence.m_nContrastLow = (long)m_pContSlide->GetScrollPos(0);
		m_pData->m_Enhence.m_nContrastHigh = (long)m_pContSlide->GetScrollPos(1);
//		m_pData->m_Enhence.m_nLight = (long)m_pLightSlide->GetScrollPos(0);

		memcpy(&m_pBackData->m_Enhence, &m_pData->m_Enhence, sizeof(ENHENCE_DATA));
		memcpy(&m_pBackData->m_IMG, &m_pData->m_IMG, sizeof(IMAGE_DATA));
		memcpy(&m_pBackData->m_CAL, &m_pData->m_CAL, sizeof(CAL_DATA));

		for(i=0; i<GOM_MAX_NUM; i++)
			m_pBackData->m_INSP[i].m_dOffsetV = m_pData->m_INSP[i].m_dOffsetV;
		for(i=0; i< m_pData->m_Teach.m_nLinkNum; i++)
			m_pBackData->m_Teach.m_LINK[i].m_dOffsetV = m_pData->m_Teach.m_LINK[i].m_dOffsetV;
		
		m_pBackData->m_nImageWidth = m_pData->m_nImageWidth;
		m_pBackData->m_nImageHeight = m_pData->m_nImageHeight;

		m_pParent->SetEventFuncCall(DEF_CONT_EXIT_SETUP);
	}

	OnBnClickedBtnExeCancel();
}

void CDlgMappingSetup::OnBnClickedBtnMappingSave()
{
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 변경된 데이타를 저장하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want save the changed data ?  ");
	else strMessage = _T("  要保存变更的参数吗  ");

	if (MessageBox(strMessage, _T(" Message Box  "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pParent->m_pModelData->CopyModelData(m_pBackData, m_pData);
		m_pParent->m_pModelData->SaveModelData(m_pParent->m_strFullPathName);
		m_pParent->SaveCalToFile();
		m_pParent->SetEventFuncCall(DEF_CONT_SAVE_SETUP);
		m_pParent->Invalidate(false);
	}

	OnBnClickedBtnExeCancel();
}

void CDlgMappingSetup::OnCheckEnhence() 
{
	UpdateData(TRUE);
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

//	m_pData->m_Enhence.m_bUseEnhence = m_chkEnhence;
//	m_pParent->UpdateLUT((long)m_pImage);
	m_pParent->InvalidateView();
}

void CDlgMappingSetup::SetLUTable()
{
	/*
	long i;
	long nLeftCont, nRightcont, nBright, nRange;

	nLeftCont  = m_pData->m_Enhence.m_nContrastLow;
	nRightcont = m_pData->m_Enhence.m_nContrastHigh;
	nBright = m_pData->m_Enhence.m_nBrightness;
	nRange = nRightcont - nLeftCont + 1;
	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();

	for(i=0; i<IMAGE_LEVEL; i++)
	{
		if (nLeftCont > i) 
			m_pData->m_Enhence.m_nLUT[i]= BLACK_LEVEL;
		else if(nRightcont < i) 
			m_pData->m_Enhence.m_nLUT[i]= WHITE_LEVEL;
		else
			m_pData->m_Enhence.m_nLUT[i] = (long)((i - nLeftCont)*IMAGE_LEVEL)/nRange + nBright;

		if (m_pData->m_Enhence.m_nLUT[i]>WHITE_LEVEL) 
			m_pData->m_Enhence.m_nLUT[i] = WHITE_LEVEL;
	}
	
	m_pParent->UpdateLUT((long)m_pImage);
	m_pParent->InvalidateView();
	*/
}

void CDlgMappingSetup::InitialPlotRect()
{
	CRect    m_rcBounds;

	m_fmProfile.GetClientRect(&m_rcBounds);

	_tcscpy_s(m_Axis.m_sTitleX, _T(" X Profile "));
	_tcscpy_s(m_Axis.m_sTitleY, _T(" Y Profile "));

	// 영역설정 ---------------------------------------------
	m_pParent->m_rcPlotBounds = m_rcBounds;
    m_Axis.m_rClient = m_rcBounds;

	// Axis의 Setup에 관련된 파라미터 -------------------------
	m_Axis.m_nLeftMargin = 60;
	m_Axis.m_nRightMargin = 20;
	m_Axis.m_nBottomMargin = 40;
	m_Axis.m_nTopMargin = 25;
	
	m_Axis.m_nPlotStrSize = 10;
	m_Axis.m_nSlidePerPlot = 5;
	m_Axis.m_nPixelDistGrid = 5;

	m_Axis.m_dScaleX = 1; 
	m_Axis.m_dScaleY = 1;
	
	m_Axis.m_nRangeMin = 0;
	m_Axis.m_nRangeMax = 255;

	m_Axis.m_rPlot.left  = m_rcBounds.left  + m_Axis.m_nLeftMargin;
    m_Axis.m_rPlot.top   = m_rcBounds.top   + m_Axis.m_nTopMargin;
	m_Axis.m_rPlot.right = m_rcBounds.right - m_Axis.m_nRightMargin;
	m_Axis.m_rPlot.bottom= m_rcBounds.bottom- m_Axis.m_nBottomMargin;

	m_Axis.m_rTitleX.bottom = m_rcBounds.bottom-5;
    m_Axis.m_rTitleX.top = m_rcBounds.bottom-20;
    m_Axis.m_rTitleX.right =  m_rcBounds.right-15;
    m_Axis.m_rTitleX.left = 130;

	m_Axis.m_rTitleY.bottom = m_rcBounds.top+140;
    m_Axis.m_rTitleY.top = m_rcBounds.top+5;
    m_Axis.m_rTitleY.right =  m_rcBounds.left+50;
    m_Axis.m_rTitleY.left = m_rcBounds.left+5;

	m_Axis.m_rChart.left = m_Axis.m_rPlot.left + 10;
	m_Axis.m_rChart.top = m_Axis.m_rPlot.top+10;
	m_Axis.m_rChart.right = m_Axis.m_rPlot.left + 250;
    m_Axis.m_rChart.bottom = m_Axis.m_rPlot.Height();

	m_Axis.m_colorGridLine  = RGB(220, 220, 220);
	m_Axis.m_colorSliderLine = RGB(190, 190, 190);
	m_Axis.m_colorBkClient = RGB(255, 255, 255);
	m_Axis.m_colorBkPlot = RGB(255, 255, 255);
	m_Axis.m_colorPlotStr = RGB(0, 0, 0);
	
	m_Axis.m_colorUpLine = RGB(0, 255, 0);
	m_Axis.m_colorDownLine = RGB(0, 0, 255);
	m_Axis.m_colorDataLine = RGB(255, 0, 0);

	m_Axis.m_nPlotStrSize = 10;
	//--------------------------------------------------
}

void CDlgMappingSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC *pDC, memDC;
    CBitmap  bmpBuffer;
	CBitmap *pOldBitmap;

	m_fmProfile.GetClientRect(&m_rcBounds);
 	m_pParent->m_rcPlotBounds = m_rcBounds;

    pDC = m_fmProfile.GetDC();
	memDC.CreateCompatibleDC(pDC);
    bmpBuffer.CreateCompatibleBitmap(pDC, m_rcBounds.Width(), m_rcBounds.Height()); 
    pOldBitmap = (CBitmap*)memDC.SelectObject(&bmpBuffer); 

	DrawProfile(&memDC);
	
	pDC->BitBlt(0, 0, m_Axis.m_rClient.Width(), m_Axis.m_rClient.Height(), &memDC, 0, 0,SRCCOPY); 
    memDC.SelectObject(pOldBitmap); 
	m_fmProfile.ReleaseDC(pDC);
}

void CDlgMappingSetup::DrawProfile(CDC *pDC)
{
	DrawBasic(pDC);

	m_Axis.m_nRangeMin = BLACK_LEVEL;
	m_Axis.m_nRangeMax = WHITE_LEVEL;
	DrawXAxisGrid(pDC);

	m_Axis.m_nRangeMin = BLACK_LEVEL;
	m_Axis.m_nRangeMax = m_nMaxCount;
	DrawYAxisGrid(pDC);

	DrawRangerLine(pDC);
	DrawPoly(pDC);
}

void CDlgMappingSetup::DrawBasic(CDC *pDC)
{
	CBrush brushctlBkColor(m_Axis.m_colorBkClient);
	pDC->FillRect(m_Axis.m_rClient, &brushctlBkColor);

	CPen *old, stick(PS_SOLID,1,RGB(0,0,0));
	old = pDC->SelectObject(&stick); 

	pDC->Rectangle(m_Axis.m_rClient);
	pDC->Rectangle(m_Axis.m_rPlot);
	pDC->SelectObject(old);

	CFont *pNewFont, *pOldFont;
	pNewFont = new CFont;
    pNewFont->CreateFont(16, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                               DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));

	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(m_Axis.m_rTitleX.left, m_Axis.m_rTitleX.top, m_Axis.m_sTitleX); 
//	pDC->TextOut(m_Axis.m_rTitleY.left, m_Axis.m_rTitleY.top, m_Axis.m_sTitleY); 
	
	pDC->SelectObject(pOldFont);
	delete pNewFont;

	pNewFont = new CFont;
	pNewFont->CreateFont (17, 0, 900, 0, FALSE, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("Terminal"));

	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(m_Axis.m_rTitleY.left, m_Axis.m_rTitleY.bottom, m_Axis.m_sTitleY); 
	delete pNewFont;
}

void CDlgMappingSetup::DrawXAxisGrid(CDC *pDC)
{
	long  nSlide;
	float dSx, dRange, dPixelPerSlide, dLevelPerSlide;
	CString szbuf;

	// 스케일 산정 ----------------------------------------------------
	dRange = (float)(m_Axis.m_nRangeMax - m_Axis.m_nRangeMin);
    dLevelPerSlide = dRange / (float)m_Axis.m_nSlidePerPlot;
	dPixelPerSlide = (float)(m_Axis.m_rPlot.Width()-1) / (float)m_Axis.m_nSlidePerPlot;
	// 스케일 산정 ----------------------------------------------------

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorSliderLine);
	CPen stick(PS_SOLID, 0, RGB(0,0,0));
	CPen mline(PS_SOLID, 0, m_Axis.m_colorGridLine);
	CFont *pNewFont, *pOldFont;

	pNewFont = new CFont;
    pNewFont->CreateFont(14, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                               DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	szbuf.Format(_T("%d"), (long)m_Axis.m_nRangeMin);
	pDC->TextOut(m_Axis.m_rPlot.left-5, m_Axis.m_rPlot.bottom+3, szbuf);

	nSlide = 0;
	for(dSx=(float)(m_Axis.m_rPlot.left+dPixelPerSlide); dSx<=m_Axis.m_rPlot.right; dSx+=dPixelPerSlide) 
	{
		old = pDC->SelectObject(&stick); 
		pDC->MoveTo(CPoint((long)(dSx+0.5), m_Axis.m_rPlot.bottom));
		pDC->LineTo(CPoint((long)(dSx+0.5), m_Axis.m_rPlot.bottom+3));
		
		nSlide++;
		szbuf.Format(_T("%d"), (long)(m_Axis.m_nRangeMin+nSlide*dLevelPerSlide));
    	pDC->TextOut((long)(dSx-10), m_Axis.m_rPlot.bottom+3, szbuf);
		pDC->SelectObject(old);

		if ((long)(dSx+0.5) >= m_Axis.m_rPlot.right-1) continue;

		old = pDC->SelectObject(&pen); 
		pDC->MoveTo(CPoint((long)(dSx+0.5), m_Axis.m_rPlot.bottom-1));
		pDC->LineTo(CPoint((long)(dSx+0.5), m_Axis.m_rPlot.top));
    	pDC->SelectObject(old);
	}

	delete pNewFont;
}

void CDlgMappingSetup::DrawYAxisGrid(CDC *pDC)
{
	long  nSlide;
	float dSy, dRange, dPixelPerSlide, dLevelPerSlide;
	CString szbuf;

	// 스케일 산정 ----------------------------------------------------
	dRange = (float)(m_Axis.m_nRangeMax - m_Axis.m_nRangeMin);
    dLevelPerSlide = dRange / (float)m_Axis.m_nSlidePerPlot;
	dPixelPerSlide = (float)(m_Axis.m_rPlot.Height()-1) / (float)m_Axis.m_nSlidePerPlot;
	// 스케일 산정 ----------------------------------------------------

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorSliderLine);
	CPen stick(PS_SOLID, 0, RGB(0,0,0));
	CPen mline(PS_SOLID, 0, m_Axis.m_colorGridLine);
	CFont *pNewFont, *pOldFont;

	pNewFont = new CFont;
    pNewFont->CreateFont(14, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                               DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	szbuf.Format(_T("%dk"), (long)(m_Axis.m_nRangeMin/1000));
	pDC->TextOut(m_Axis.m_rPlot.left-35, m_Axis.m_rPlot.bottom-5, szbuf);

	nSlide = 0;
	for(dSy=(float)(m_Axis.m_rPlot.bottom-dPixelPerSlide); dSy>=(float)m_Axis.m_rPlot.top; dSy-=dPixelPerSlide) 
	{
		old = pDC->SelectObject(&stick); 
		pDC->MoveTo(CPoint(m_Axis.m_rPlot.left-3, (long)dSy));
		pDC->LineTo(CPoint(m_Axis.m_rPlot.left, (long)dSy));

		nSlide++;
		szbuf.Format(_T("%dk"), (long)((m_Axis.m_nRangeMin+nSlide*dLevelPerSlide)/1000));
		pDC->TextOut(m_Axis.m_rPlot.left-35, (long)(dSy-5), szbuf);
		pDC->SelectObject(old);

		if ((long)(dSy+0.5) <= m_Axis.m_rPlot.top+1) continue;

		old = pDC->SelectObject(&mline);
		pDC->MoveTo(CPoint(m_Axis.m_rPlot.left+1, (long)(dSy+0.5)));
		pDC->LineTo(CPoint(m_Axis.m_rPlot.right-1, (long)(dSy+0.5)));
		pDC->SelectObject(old);
	}

	pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CDlgMappingSetup::DrawRangerLine(CDC *pDC)
{
	/*
	int nValue;
	float dRangeY , dRangeX;
	CString szbuf;

	dRangeY = (float)fabs((double)(m_Axis.m_rPlot.bottom - m_Axis.m_rPlot.top));
	dRangeX = (float)fabs((double)(m_Axis.m_rPlot.right - m_Axis.m_rPlot.left));

	CPen *old, penUp(PS_DASH, 1, m_Axis.m_colorUpLine),penDown(PS_DASH, 1, m_Axis.m_colorDownLine);
	old = pDC->SelectObject(&penUp);

	nValue	=1;//	(int)(m_Axis.m_rPlot.left + (((double)m_ctrlContrast.GetLeft() * dRangeX) / IMAGE_LEVEL));
	pDC->MoveTo(CPoint((long)nValue+1, m_Axis.m_rPlot.top+1));
	pDC->LineTo(CPoint((long)nValue+1, m_Axis.m_rPlot.bottom-1));
	pDC->SelectObject(&penDown);

	nValue	=1;//	(int)(m_Axis.m_rPlot.left + (((double)m_ctrlContrast.GetRight() * dRangeX) / IMAGE_LEVEL));
	pDC->MoveTo(CPoint((long)nValue, m_Axis.m_rPlot.top+1));
	pDC->LineTo(CPoint((long)nValue, m_Axis.m_rPlot.bottom-1));
	pDC->SelectObject(old);
	*/
}

void CDlgMappingSetup::DrawPoly(CDC *pDC)
{
	long i;
	long nStartPosX, nEndPosX , nStartPosY, nEndPosY;
	long nLevel, nOffset, nRangeX, nRangeY, nGraRangeX, nGraRangeY;

	nStartPosX  =	BLACK_LEVEL;
	nEndPosX	=	WHITE_LEVEL;
	nStartPosY	=	BLACK_LEVEL;
	nEndPosY	=	m_nMaxCount;

	nRangeX = nEndPosX - nStartPosX;
	nRangeY = nEndPosY - nStartPosY;
	nGraRangeX	=	m_Axis.m_rPlot.right - m_Axis.m_rPlot.left + 1;
	nGraRangeY	=	m_Axis.m_rPlot.bottom - m_Axis.m_rPlot.top + 1;

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorDataLine);
	old = pDC->SelectObject(&pen); 

	nLevel = 0;
	if (m_nMaxCount>0)	
		nLevel = (m_pProfileData[nStartPosX] * nGraRangeY / m_nMaxCount);
	pDC->MoveTo(m_Axis.m_rPlot.left , m_Axis.m_rPlot.bottom);

	for(i=nStartPosX+1; i<nEndPosX; i++)
	{
		nLevel = 0;
		nOffset	= (i-nStartPosX) * nGraRangeX / nRangeX;

		if (m_nMaxCount>0)
			nLevel = (m_pProfileData[i] * nGraRangeY / m_nMaxCount);
		pDC->LineTo(m_Axis.m_rPlot.left + nOffset, m_Axis.m_rPlot.bottom-nLevel);
	}
	
	pDC->SelectObject(old);
}

void CDlgMappingSetup::UpdateProfile()
{
	long i, j, nGray;

	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
	m_nMaxCount	=	0;
	for(i=0; i<IMAGE_LEVEL; i++) 
		m_pProfileData[i] = 0;

	for(i=0; i<m_pData->m_nImageHeight; i++)
	for(j=0; j<m_pData->m_nImageWidth; j++)
	{
		nGray = m_pImage[i*m_pData->m_nImageWidth + j];
		m_pProfileData[nGray]++;
	}

	for(i=0; i<IMAGE_LEVEL; i++)	
	{
		if (m_nMaxCount < m_pProfileData[i]) 
			m_nMaxCount = m_pProfileData[i];
	}

	Invalidate(FALSE);
}

void CDlgMappingSetup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		UpdateProfile();
		UpdateControlsData(); 
	}

//    m_pParent->m_pModelData->DeleteROI(_T("CAL"));
	m_pParent->Invalidate(FALSE);
}

void CDlgMappingSetup::OnBnClickedBtnCal()
{
	GetDlgItem(IDC_BTN_CAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_EXE_CAL)->ShowWindow(TRUE);
	GetDlgItem(IDC_BTN_EXE_CANCEL)->ShowWindow(TRUE);
}

void CDlgMappingSetup::OnBnClickedBtnExeCal()
{
	BOOL bRet = TRUE;
	CString str, strMessage;
	long i, nIndex, nDir, nMid, bRear, nH, nLoc;
	float fPixelSize, fScale, fRealSize;
	
	UpdateData(true);

	fRealSize = (float)m_editRealSize.GetTextValue();

	if (m_pParent->m_nLngSel==0) strMessage = _T("  CAL. PROC.를 수행하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want process calibration ?  ");
	else strMessage = _T("  确定要重新进行校正吗？  ");

	if (MessageBox(strMessage, _T(" Message Box  "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pData->m_Enhence.m_bUseEnhence = ((CButton *)GetDlgItem(IDC_CHECK_ENHENCE))->GetCheck();

		nIndex = m_pParent->m_pModelData->GetIndexSelectedROI();
		if (nIndex>=0 && fRealSize>20 && fRealSize<500)
		{
			nH = m_pData->m_nImageHeight/2;
			nDir = m_pData->m_Scan[nIndex].m_nSecondScanDirType;
			bRear = m_pData->m_INSP[nIndex].m_bLocROI;
//			m_pData->m_INSP[nIndex].m_nBlobOption = 2;
		
			if (m_pData->m_Enhence.m_bUseEnhence)
			{
				m_pParent->m_pModuleLink->InspectMeasurementROI(nIndex);

				fPixelSize = m_pParent->m_pModuleLink->m_fCalPixelDist;
				fScale = 0.0f;
				if (fPixelSize>0.0f)
					fScale = fRealSize / fPixelSize;
			}
			else
				fScale = (float)m_editScaleFactor.GetTextValue();

			if (fScale>0.03 && fScale<0.2) 
			{
				nMid = (long)(m_pData->m_GOM[nIndex].m_ptPoint1.y + m_pData->m_GOM[nIndex].m_ptPoint4.y)/2;

				if (nDir==DIR_LEFT_RIGHT || nDir==DIR_RIGHT_LEFT)
				{
					if (bRear && nMid<nH)   m_pParent->m_pCAL.m_dScaleRT_X=fScale;
					if (bRear && nMid>=nH)  m_pParent->m_pCAL.m_dScaleRB_X=fScale;
					if (!bRear && nMid<nH)  m_pParent->m_pCAL.m_dScaleLT_X=fScale;
					if (!bRear && nMid>=nH) m_pParent->m_pCAL.m_dScaleLB_X=fScale;

					if (nMid<nH) m_pParent->m_pCAL.m_dRealT_X = fRealSize; 
			        else m_pParent->m_pCAL.m_dRealB_X = fRealSize; 
				}
				else
				{
					if (bRear) m_pParent->m_pCAL.m_dScaleR_Y = fScale;
					else  m_pParent->m_pCAL.m_dScaleL_Y=fScale;

					m_pParent->m_pCAL.m_dReal_Y = fRealSize;
				}
				
				for(i=0; i<GOM_MAX_NUM; i++)
				{
					if (bRear == m_pData->m_INSP[i].m_bLocROI)
						m_pData->m_INSP[i].m_dOffsetV = 0.0f;
				}

				for(i=0; i<m_pData->m_Teach.m_nLinkNum; i++)
				{
					nLoc = m_pData->m_Teach.m_LINK[i].m_nIndex1;
					if (bRear == m_pData->m_INSP[nLoc].m_bLocROI)
						m_pData->m_Teach.m_LINK[i].m_dOffsetV = 0.0f;
				}

				str.Format(_T("%6.5f"), fScale);
				m_editScaleFactor.SetTextOut(str);

				m_pData->m_INSP[nIndex].m_nBlobOption = 0;
				bRet = FALSE;
			}
		}

		if (m_pData->m_Enhence.m_bUseEnhence)
		{
			if (!bRet)
				MessageBox(_T("  Success Cal. Process  "), _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
			else 
				MessageBox(_T("  Failure Cal. Process Or Master JIG not found !  "), _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
		}
	}
	
	UpdateData(false);
    m_pParent->Invalidate(false);
}

void CDlgMappingSetup::OnBnClickedBtnExeCancel()
{
	m_bCalProc = FALSE;
	m_pParent->m_pModelData->ShowAllROI(TRUE);
	m_pParent->m_pModelData->DeleteROI(_T("CAL"));

	GetDlgItem(IDC_BTN_CAL)->ShowWindow(TRUE);
	GetDlgItem(IDC_BTN_EXE_CAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_EXE_CANCEL)->ShowWindow(FALSE);

	m_pParent->Invalidate(false);
}
