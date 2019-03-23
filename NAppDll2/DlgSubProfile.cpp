// DlgSubProfile.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSubProfile.h"
#include "DlgVDisp.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubProfile dialog
CDlgSubProfile::CDlgSubProfile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubProfile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubProfile)
	m_editSlope = 0;
	m_editMaxThr = 0;
	m_editMinThr = 0;

	//}}AFX_DATA_INIT
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pData;
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pBackData;
//	m_pImage = (LPBYTE)m_pParent->GetDisplayImagePtr();

	m_bCheckVert = TRUE; 
	m_dAngle = 0.0f;

	m_bCheckScrollX = TRUE;
	m_bCheckScrollY = TRUE;
	
	m_colorF = RGB(100, 100, 100);
	m_colorB = RGB(200, 200, 255);
}

void CDlgSubProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubProfile)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	//	DDX_Control(pDX, IDC_COMBO_METHOD, m_comboMethod);
	DDX_Text(pDX, IDC_EDIT_SLOPE, m_editSlope);
	DDX_Text(pDX, IDC_EDIT_SLOPE2, m_editMaxThr);
	DDX_Text(pDX, IDC_EDIT_SLOPE3, m_editMinThr);
	DDX_Control(pDX, IDC_BTN_POL1, m_btnPol1);
	DDX_Control(pDX, IDC_BTN_POL2, m_btnPol2);
	DDX_Control(pDX, IDC_COMBO_SAMPLE_NUM, m_comboSampleNum);
	DDX_Control(pDX, IDC_COMBO_RATE, m_comboRate);	
	DDX_Control(pDX, IDC_COMBO_OVERLAP_RATE, m_comboOverlapRate);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSubProfile, CDialog)
	//{{AFX_MSG_MAP(CDlgSubProfile)
	ON_BN_CLICKED(IDC_RADIO_TOP_1, OnRadioTop1)
	ON_BN_CLICKED(IDC_RADIO_BOT_1, OnRadioBot1)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_1, OnRadioRight1)
	ON_BN_CLICKED(IDC_RADIO_LEFT_1, OnRadioLeft1)
	ON_BN_CLICKED(IDC_RADIO_TOP_2, OnRadioTop2)
	ON_BN_CLICKED(IDC_RADIO_BOT_2, OnRadioBot2)
	ON_BN_CLICKED(IDC_RADIO_LEFT_2, OnRadioLeft2)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_2, OnRadioRight2)
	ON_WM_SHOWWINDOW()
//	ON_CBN_SELCHANGE(IDC_COMBO_METHOD, OnSelchangeComboMethod)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOPE, OnDeltaposSpinSlope)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOPE2, OnDeltaposSpinSlope2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOPE3, OnDeltaposSpinSlope3)

	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
//	ON_BN_CLICKED(IDC_BTN_CW, OnBtnCw)
//	ON_BN_CLICKED(IDC_BTN_CCW, OnBtnCcw)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_POL1, &CDlgSubProfile::OnBnClickedBtnPol1)
	ON_BN_CLICKED(IDC_BTN_POL2, &CDlgSubProfile::OnBnClickedBtnPol2)
	ON_COMMAND(IDM_BLACK_WHITE, &CDlgSubProfile::OnBlackWhite)
	ON_COMMAND(IDM_WHITE_BLACK, &CDlgSubProfile::OnWhiteBlack)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_RADIO_LOC_LEFT, &CDlgSubProfile::OnBnClickedRadioLocLeft)
	ON_BN_CLICKED(IDC_RADIO_LOC_RIGHT, &CDlgSubProfile::OnBnClickedRadioLocRight)
	ON_BN_CLICKED(IDC_RADIO_MULTIPLE, &CDlgSubProfile::OnBnClickedRadioMultiple)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubProfile message handlers

BOOL CDlgSubProfile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	InitialPlotRect();

	return TRUE;  
}

void CDlgSubProfile::InitControls()
{
	short i, shBtnColor = 30;
	CString str, strMessage;

	m_btnPol1.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
	m_btnPol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnPol1.SetFlat(FALSE);

	m_btnPol2.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
	m_btnPol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnPol2.SetFlat(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(TRUE);

	m_comboType.ResetContent();
	if (m_pParent->m_nLngSel==0) 
	{
		m_comboType.AddString(_T(" 일방향 검사 "));
	    m_comboType.AddString(_T(" 양방향 검사 "));
		m_comboType.AddString(_T(" 교차점 검사 "));
		m_comboType.AddString(_T("미스매칭 검사"));
		m_comboType.AddString(_T("DATUM"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T(" 검사형태 "));
		GetDlgItem(IDC_STATIC_SLOPE)->SetWindowText(_T(" 밝기차이 "));
		GetDlgItem(IDC_STATIC_LOCROI)->SetWindowText(_T(" 항목위치 "));
		GetDlgItem(IDC_RADIO_LOC_RIGHT)->SetWindowText(_T(" 전 "));
		GetDlgItem(IDC_RADIO_LOC_LEFT)->SetWindowText(_T(" 후 "));
	}
	else if (m_pParent->m_nLngSel==1) 
	{
		m_comboType.AddString(_T(" 1's DIR. SCAN INSP. "));
	    m_comboType.AddString(_T(" 2's DIR. SCAN INSP. "));
		m_comboType.AddString(_T(" CROSS POINT "));
		m_comboType.AddString(_T("Mis-Match INSP."));
		m_comboType.AddString(_T("DATUM"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T(" INSP. TYPE "));
		GetDlgItem(IDC_STATIC_SLOPE)->SetWindowText(_T(" SLOPE "));
		GetDlgItem(IDC_STATIC_LOCROI)->SetWindowText(_T(" LOC. ROI "));
		GetDlgItem(IDC_RADIO_LOC_RIGHT)->SetWindowText(_T(" FRONT "));
		GetDlgItem(IDC_RADIO_LOC_LEFT)->SetWindowText(_T(" REAR "));
	}
	else
	{
		m_comboType.AddString(_T(" 扉郯 塔捋 "));
		m_comboType.AddString(_T(" 寤郯 塔捋 "));
		m_comboType.AddString(_T(" CROSS POINT "));
		m_comboType.AddString(_T("Mis-Match INSP."));
		m_comboType.AddString(_T("DATUM"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T(" 检查形式 "));
		GetDlgItem(IDC_STATIC_SLOPE)->SetWindowText(_T(" 亮度差异 "));
		GetDlgItem(IDC_STATIC_LOCROI)->SetWindowText(_T(" 项目位置 "));
		GetDlgItem(IDC_RADIO_LOC_RIGHT)->SetWindowText(_T(" 前面 "));
		GetDlgItem(IDC_RADIO_LOC_LEFT)->SetWindowText(_T(" 后面 "));
	}

	m_comboSampleNum.ResetContent();
	for(i=1; i<=10; i++)
	{
		str.Format(_T(" %d "), i);
	    m_comboSampleNum.AddString(str);
	}
	
	m_comboRate.ResetContent();
	for(i=1; i<=5; i++)
	{
		str.Format(_T(" %d "), i*10);
	    m_comboRate.AddString(str);
	}
}

void CDlgSubProfile::OnCbnSelchangeComboPol1()
{
	long nSel;

	nSel = m_comboPol1.GetCurSel();
	if (nSel<=0)
		m_nFirstPolDirType = POL_BLACK_WHITE;
	else
		m_nFirstPolDirType = POL_WHITE_BLACK;
}

void CDlgSubProfile::OnCbnSelchangeComboPol2()
{
	long nSel;

	nSel = m_comboPol2.GetCurSel();
	if (nSel<=0)
		m_nSecondPolDirType = POL_BLACK_WHITE;
	else
		m_nSecondPolDirType = POL_WHITE_BLACK;
}

void CDlgSubProfile::UpdateControlsData()
{
	CString str;
	long i, nSelectedROI, nGOMType, nSelPos, nSelRate;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI>=0)
	{
		m_nFirstScanDirType = m_pData->m_Scan[nSelectedROI].m_nFirstScanDirType;
		m_nSecondScanDirType = m_pData->m_Scan[nSelectedROI].m_nSecondScanDirType;
		m_nFirstPolDirType = m_pData->m_Scan[nSelectedROI].m_nFirstPolDirType;
		m_nSecondPolDirType = m_pData->m_Scan[nSelectedROI].m_nSecondPolDirType;
		m_nFirstThresh = m_pData->m_Scan[nSelectedROI].m_nFirstThresh;
		m_nSecondThresh = m_pData->m_Scan[nSelectedROI].m_nSecondThresh;
		m_nInspectType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		m_nScanType = m_pData->m_Scan[nSelectedROI].m_nScanType;
		m_nScanDir = m_pData->m_Scan[nSelectedROI].m_nSecondScanDirType;
		m_nLocROI = m_pData->m_INSP[nSelectedROI].m_bLocROI;
		m_dAngle = m_pData->m_GOM[nSelectedROI].m_dAngle;

		m_comboOverlapRate.ResetContent();
		m_comboOverlapRate.AddString(_T("NONE"));
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			str.Format(_T("%s"), m_pData->m_GOM[i].m_strTitle);
			if (str == _T("A1L") || str == _T("A2L") || str == _T("A3L") || str == _T("A4L"))
			{
				if (m_pData->m_INSP[i].m_bLocROI)
					m_comboOverlapRate.AddString(_T("R_") + str);
				else
					m_comboOverlapRate.AddString(_T("F_") + str);
			}
		}

		if (m_nInspectType == INSP_MEASURE_CIRCLE) 
            nSelPos = 0;
		else
			nSelPos = m_nInspectType-INSP_MEASURE_SIDE_ONE;

		m_comboType.SetCurSel(nSelPos);

		nSelPos = m_pData->m_INSP[nSelectedROI].m_nSampleNum-1; 
		if (nSelPos<0) nSelPos = 0;
		m_comboSampleNum.SetCurSel(nSelPos);

		nSelPos = m_pData->m_INSP[nSelectedROI].m_bOverlapRate; 
		if (nSelPos<0) nSelPos = 0;
		m_comboOverlapRate.SetCurSel(nSelPos);

		nSelRate = m_pData->m_INSP[nSelectedROI].m_bBlobFeret;
		if (nSelRate<0) nSelRate = 0;
		m_comboRate.SetCurSel(nSelRate);

		str.Format(_T("%d"), (long)m_dAngle);
		m_editSlope = (long)m_pData->m_INSP[nSelectedROI].m_dSensity;
		m_editMaxThr = (long)m_pData->m_INSP[nSelectedROI].m_nMaxThr;
		m_editMinThr = (long)m_pData->m_INSP[nSelectedROI].m_nMinThr;  
//		GetDlgItem(IDC_STATIC_ANGLE)->SetWindowText(str);

		((CButton *)GetDlgItem(IDC_RADIO_MULTIPLE))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(FALSE);
		if (m_nScanType==SCAN_PROJECT_FT) ((CButton *)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_MULTIPLE))->SetCheck(TRUE);

		((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(FALSE);
		if (m_nLocROI) ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->SetCheck(TRUE);
		else  ((CButton *)GetDlgItem(IDC_RADIO_LOC_RIGHT))->SetCheck(TRUE);

		((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->EnableWindow(TRUE);

		((CButton *)GetDlgItem(IDC_BTN_POL1))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_BTN_POL2))->EnableWindow(TRUE);

		((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->SetCheck(FALSE);

		if (m_nInspectType==INSP_MEASURE_SIDE_ONE || 
			m_nInspectType==INSP_MEASURE_CIRCLE)
		{
			((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_BTN_POL1))->EnableWindow(FALSE);
		}

		m_btnPol1.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
		if (m_nFirstPolDirType == POL_WHITE_BLACK)
			m_btnPol1.SetBitmaps(IDB_BMP_WHITE_BLACK, RGB(255, 0, 0));

		m_btnPol2.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
		if (m_nSecondPolDirType == POL_WHITE_BLACK)
	    	m_btnPol2.SetBitmaps(IDB_BMP_WHITE_BLACK, RGB(255, 0, 0));

		if (m_nFirstScanDirType==DIR_LEFT_RIGHT || m_nFirstScanDirType==DIR_RIGHT_LEFT)
		{
			SetScrollPosV(m_nFirstThresh, m_nSecondThresh);
			SetScrollPosH(BLACK_LEVEL, WHITE_LEVEL);
        }
		else
		{
			SetScrollPosV(BLACK_LEVEL, WHITE_LEVEL);
			SetScrollPosH(m_nFirstThresh, m_nSecondThresh);
		}

		if (m_nFirstScanDirType == DIR_TOP_BOTTOM)
           ((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->SetCheck(TRUE);
		if (m_nFirstScanDirType == DIR_BOTTOM_TOP)
           ((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(TRUE);
		if (m_nFirstScanDirType == DIR_LEFT_RIGHT)
           ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->SetCheck(TRUE);
		if (m_nFirstScanDirType == DIR_RIGHT_LEFT)
           ((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->SetCheck(TRUE);
		if (m_nSecondScanDirType == DIR_TOP_BOTTOM)
           ((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->SetCheck(TRUE);
		if (m_nSecondScanDirType == DIR_BOTTOM_TOP)
           ((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(TRUE);
		if (m_nSecondScanDirType == DIR_LEFT_RIGHT)
           ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->SetCheck(TRUE);
		if (m_nSecondScanDirType == DIR_RIGHT_LEFT)
           ((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->SetCheck(TRUE);

		nGOMType = m_pData->m_GOM[nSelectedROI].m_nGOMType;
		if (nGOMType==GOM_RECT)
		{
			GetDlgItem(IDC_RADIO_TOP_1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_BOT_1)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_TOP_2)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_BOT_2)->ShowWindow(TRUE);
		}
		if (nGOMType==GOM_CIRCLE2 || nGOMType==GOM_CIRCLE)
		{
			GetDlgItem(IDC_RADIO_TOP_1)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_BOT_1)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_TOP_2)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_BOT_2)->ShowWindow(FALSE);
		}

		m_bCheckVert = TRUE;
		if (m_nFirstScanDirType == DIR_LEFT_RIGHT ||
			m_nFirstScanDirType == DIR_RIGHT_LEFT) m_bCheckVert = FALSE;
	}

	UpdateData(false);
	m_pParent->m_pDlgVisionSetup->UpdateFrameRect();
}

void CDlgSubProfile::ApplyInspData(long nSelectROI)
{
	long nSel, nSelSampleNum, nSelOverlapNum, nSelectedROI;
	long nValue, nSelRate;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	if (nSelectROI>=0)
	{
		nSelSampleNum = m_comboSampleNum.GetCurSel()+1;
		if (nSelSampleNum<1) nSelSampleNum = 1;

		nSelOverlapNum = m_comboOverlapRate.GetCurSel();
		if (nSelOverlapNum<0) nSelOverlapNum = 0;

		nSelRate = m_comboRate.GetCurSel();
		if (nSelRate<0) nSelRate = 0;

		nSel = m_comboType.GetCurSel();
		if (nSel<0) nSel = 0;
				
    	m_pData->m_INSP[nSelectROI].m_nInspectType = nSel + INSP_MEASURE_SIDE_ONE;
		m_pData->m_INSP[nSelectROI].m_nSampleNum = nSelSampleNum;
		m_pData->m_INSP[nSelectROI].m_bOverlapRate = nSelOverlapNum;
		m_pData->m_INSP[nSelectROI].m_bBlobFeret = nSelRate;

		nValue = m_pParent->m_pModuleLink->GetGrayValueItem(nSelectROI, 0);
		if (nValue>0) 
			m_pData->m_INSP[nSelectROI].m_nTGrayV = nValue;

		if (m_pData->m_GOM[nSelectROI].m_nGOMType == GOM_CIRCLE2) 
            m_pData->m_INSP[nSelectROI].m_nInspectType = INSP_MEASURE_CIRCLE;

		m_pData->m_INSP[nSelectedROI].m_bLocROI = ((CButton *)GetDlgItem(IDC_RADIO_LOC_LEFT))->GetCheck();

        m_nScanType = ((CButton *)GetDlgItem(IDC_RADIO_MULTIPLE))->GetCheck();
		m_nScanType += SCAN_PROJECT_FT;

		if (m_nSecondScanDirType==DIR_LEFT_RIGHT || m_nSecondScanDirType==DIR_RIGHT_LEFT)
		    GetScrollPosV();
		else
			GetScrollPosH();

		m_pData->m_Scan[nSelectROI].m_nScanType = m_nScanType;
		m_pData->m_Scan[nSelectROI].m_nFirstScanDirType = m_nFirstScanDirType;
		m_pData->m_Scan[nSelectROI].m_nSecondScanDirType = m_nSecondScanDirType;
		m_pData->m_Scan[nSelectROI].m_nFirstPolDirType = m_nFirstPolDirType;
		m_pData->m_Scan[nSelectROI].m_nSecondPolDirType = m_nSecondPolDirType;
		m_pData->m_Scan[nSelectROI].m_nFirstThresh = m_nFirstThresh;
		m_pData->m_Scan[nSelectROI].m_nSecondThresh = m_nSecondThresh;
		m_pData->m_INSP[nSelectROI].m_dSensity = (float)m_editSlope;
		m_pData->m_INSP[nSelectROI].m_nMaxThr = m_editMaxThr;
		m_pData->m_INSP[nSelectROI].m_nMinThr = m_editMinThr;
	}
}

/*
void CDlgSubProfile::OnSelchangeComboMethod() 
{
	long nInspectType, nScanType;

	nInspectType = m_comboType.GetCurSel();
	if (nInspectType>=0)
	{
		nInspectType += INSP_MEASURE_SIDE_ONE;
		if (nInspectType==INSP_LINERITY || nInspectType==INSP_DEFECT_VARI)
    	    m_comboMethod.SetCurSel(SCAN_PROFILE_FT-SCAN_PROJECT_FT);
    }

	nScanType = m_comboMethod.GetCurSel();
	if (nScanType>=0)
		m_nScanType = nScanType+SCAN_PROJECT_FT;

    m_pParent->Invalidate(false);
}
*/

void CDlgSubProfile::OnSelchangeComboType()
{
	long nPos;

	nPos = m_comboType.GetCurSel()+INSP_MEASURE_SIDE_ONE;
	switch(nPos)
	{
		case INSP_MEASURE_SIDE_ONE :
			m_nInspectType = nPos;//INSP_MEASURE_SIDE_ONE;

			((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_BTN_POL1))->EnableWindow(FALSE);
			break;
		case INSP_MEASURE_SIDE_TWO :
		case INSP_SLINE :
		case INSP_MISMATCH :
			m_nInspectType = nPos;

			((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BTN_POL2))->EnableWindow(TRUE);
			break;
	}

    m_pParent->Invalidate(false);
}

void CDlgSubProfile::OnDeltaposSpinSlope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if(m_editSlope<WHITE_LEVEL) m_editSlope++;
	}
	else
	{
		if(m_editSlope>0) m_editSlope--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubProfile::OnDeltaposSpinSlope2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editMaxThr<WHITE_LEVEL) m_editMaxThr++;
	}
	else
	{
		if (m_editMaxThr>0)  m_editMaxThr--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubProfile::OnDeltaposSpinSlope3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(true);

	if(pNMUpDown->iDelta<0)
	{
		if (m_editMinThr<WHITE_LEVEL) m_editMinThr++;
	}
	else
	{
		if (m_editMinThr>0)  m_editMinThr--;
	}

	UpdateData(false);
	*pResult = 0;
}

void CDlgSubProfile::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (bShow)
		UpdateControlsData(); // TODO: Add your message handler code here

	m_pParent->m_pDlgVisionSetup->UpdateFrameRect();
}

void CDlgSubProfile::InitialPlotRect()
{
	CRect    rcBounds;

	rcBounds = m_pParent->m_rcPlotBounds;
    m_Axis.m_rClient = rcBounds;

	_tcscpy_s(m_Axis.m_sTitleX, _T(" X Profile "));
	_tcscpy_s(m_Axis.m_sTitleY, _T(" Y Profile "));

	// 康开汲沥 ----------------------------------------

	// Axis狼 Setup俊 包访等 颇扼固磐 -------------------------
	m_Axis.m_nLeftMargin = 50;
	m_Axis.m_nRightMargin = 25;
	m_Axis.m_nBottomMargin = 40;
	m_Axis.m_nTopMargin = 25;
	
	m_Axis.m_nPlotStrSize = 10;
	m_Axis.m_nSlidePerPlot = 5;
	m_Axis.m_nPixelDistGrid = 5;

	m_Axis.m_dScaleX = 1; 
	m_Axis.m_dScaleY = 1;
	
	m_Axis.m_nRangeMin = 0;
	m_Axis.m_nRangeMax = IMAGE_LEVEL;

	m_Axis.m_rPlot.left  = rcBounds.left  + m_Axis.m_nLeftMargin;
    m_Axis.m_rPlot.top   = rcBounds.top   + m_Axis.m_nTopMargin;
	m_Axis.m_rPlot.right = rcBounds.right - m_Axis.m_nRightMargin;
	m_Axis.m_rPlot.bottom= rcBounds.bottom- m_Axis.m_nBottomMargin;

	m_Axis.m_rTitleX.bottom = rcBounds.top+25;
    m_Axis.m_rTitleX.top = rcBounds.top+3;
    m_Axis.m_rTitleX.right =  rcBounds.right-15;
    m_Axis.m_rTitleX.left = 140;

	m_Axis.m_rTitleY.bottom = rcBounds.top+140;
    m_Axis.m_rTitleY.top = rcBounds.top+5;
    m_Axis.m_rTitleY.right =  rcBounds.left+50;
    m_Axis.m_rTitleY.left = rcBounds.left+5;

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

	m_rtScrollH = m_Axis.m_rPlot;
	m_rtScrollH.top = rcBounds.bottom-15;
	m_rtScrollH.bottom = rcBounds.bottom-5;

	m_rtScrollV = m_Axis.m_rPlot;
	m_rtScrollV.left = rcBounds.right-15;
	m_rtScrollV.right = rcBounds.right-5;	

	SetScrollPosH(0, WHITE_LEVEL);
	SetScrollPosV(0, WHITE_LEVEL);
	//--------------------------------------------------
}

void CDlgSubProfile::DrawProfile(CDC *pDC)
{
	BOOL  bRet=TRUE;
	long  nType;
	GOM_ATTR *pGOM;

	m_nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	pGOM = &m_pData->m_GOM[m_nSelectedROI];
	m_nGOMType = pGOM->m_nGOMType;
    
	if (m_nFirstScanDirType==DIR_LEFT_RIGHT || m_nFirstScanDirType == DIR_RIGHT_LEFT)
	{
		SetScrollPosH(0, WHITE_LEVEL);
		if (m_nInspectType == INSP_MEASURE_SIDE_ONE ||
			m_nInspectType == INSP_MEASURE_CIRCLE)
			m_fFirstV = (float)m_rtScrollV.top;
	}
	else
	{
		SetScrollPosV(0, WHITE_LEVEL);
		if (m_nInspectType == INSP_MEASURE_SIDE_ONE ||
			m_nInspectType == INSP_MEASURE_CIRCLE)
			m_fFirstH = (float)m_rtScrollH.left;
	}

	m_Axis.m_nRangeMin	= 0;
	m_Axis.m_nRangeMax	= IMAGE_LEVEL;

	DrawBasic(pDC);
	DrawScrollBar(pDC);

    nType = m_pData->m_INSP[m_nSelectedROI].m_nInspectType;
	if (nType==INSP_MEASURE_SIDE_ONE || 
		nType==INSP_MEASURE_SIDE_TWO ||
		nType==INSP_SLINE || nType==INSP_MISMATCH)
	{
		if (m_nGOMType == GOM_RECT)
		{
			if (m_bCheckVert) 
			{
				m_Axis.m_nRangeMin	= (long)((pGOM->m_ptPoint3.y+pGOM->m_ptPoint4.y)/2);
				m_Axis.m_nRangeMax	= (long)((pGOM->m_ptPoint1.y+pGOM->m_ptPoint2.y)/2);
			}
			else
			{
				m_Axis.m_nRangeMin	= 0;
				m_Axis.m_nRangeMax	= IMAGE_LEVEL;
			}

     		DrawYAxisGrid(pDC);

			if (!m_bCheckVert)
			{
				m_Axis.m_nRangeMin	= (long)((pGOM->m_ptPoint1.x+pGOM->m_ptPoint3.x)/2);
				m_Axis.m_nRangeMax	= (long)((pGOM->m_ptPoint2.x+pGOM->m_ptPoint4.x)/2);
			}
			else
			{
				m_Axis.m_nRangeMin	= 0;
				m_Axis.m_nRangeMax	= IMAGE_LEVEL;
			}

     		DrawXAxisGrid(pDC);
		}
		if (m_nGOMType==GOM_CIRCLE || m_nGOMType==GOM_CIRCLE2)
		{
			m_Axis.m_nRangeMin	= 0;
			m_Axis.m_nRangeMax	= IMAGE_LEVEL;
       		DrawYAxisGrid(pDC);

			m_Axis.m_nRangeMin	= (long)(pGOM->m_dCX + pGOM->m_dInRadius);
			m_Axis.m_nRangeMax	= (long)(pGOM->m_dCX + pGOM->m_dOutRadius);
     		DrawXAxisGrid(pDC);
		}

		DrawRangerLine(pDC);
    	DrawPoly(pDC);
	}
	else
	{
		DrawXAxisGrid(pDC);
		DrawYAxisGrid(pDC);
		DrawRangerLine(pDC);
	}
}

void CDlgSubProfile::DrawRangerLine(CDC *pDC)
{
	CString szbuf;

	CPen *old, penUp(PS_DASH, 1, m_Axis.m_colorUpLine),penDown(PS_DASH, 1, m_Axis.m_colorDownLine);
	old = pDC->SelectObject(&penUp);

	if (m_fFirstH<=m_Axis.m_rPlot.left)   m_fFirstH = (float)m_Axis.m_rPlot.left+1;
	if (m_fFirstH>=m_Axis.m_rPlot.right)  m_fFirstH = (float)m_Axis.m_rPlot.right-2;
	if (m_fSecondH<=m_Axis.m_rPlot.left)  m_fSecondH = (float)m_Axis.m_rPlot.left+1;
	if (m_fSecondH>=m_Axis.m_rPlot.right) m_fSecondH = (float)m_Axis.m_rPlot.right-2;
	
	if (m_fFirstV<=m_Axis.m_rPlot.top)    m_fFirstV = (float)m_Axis.m_rPlot.top+1;
	if (m_fFirstV>=m_Axis.m_rPlot.bottom) m_fFirstV = (float)m_Axis.m_rPlot.bottom-2;
	if (m_fSecondV<=m_Axis.m_rPlot.top)   m_fSecondV = (float)m_Axis.m_rPlot.top+1;
	if (m_fSecondV>=m_Axis.m_rPlot.bottom)m_fSecondV = (float)m_Axis.m_rPlot.bottom-2;
	
	if(m_bCheckVert)
	{
		pDC->MoveTo(CPoint((long)m_fFirstH, m_Axis.m_rPlot.top+1));
		pDC->LineTo(CPoint((long)m_fFirstH, m_Axis.m_rPlot.bottom-1));
	}
	else
	{
		pDC->MoveTo(CPoint(m_Axis.m_rPlot.left+1, (long)m_fFirstV));
		pDC->LineTo(CPoint(m_Axis.m_rPlot.right-1, (long)m_fFirstV));
	}

	pDC->SelectObject(&penDown);
	
	if(m_bCheckVert)
	{
		pDC->MoveTo(CPoint((long)m_fSecondH, m_Axis.m_rPlot.top+1));
		pDC->LineTo(CPoint((long)m_fSecondH, m_Axis.m_rPlot.bottom-1));
	}
	else
	{
		pDC->MoveTo(CPoint(m_Axis.m_rPlot.left+1, (long)m_fSecondV));
		pDC->LineTo(CPoint(m_Axis.m_rPlot.right-1, (long)m_fSecondV));
	}

	pDC->SelectObject(old);
}

void CDlgSubProfile::DrawBasic(CDC *pDC)
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
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	if (m_bCheckVert)
    	pDC->TextOut(m_Axis.m_rTitleX.left, m_Axis.m_rTitleX.top, _T("Gray Level")); 
    else
		pDC->TextOut(m_Axis.m_rTitleX.left, m_Axis.m_rTitleX.top, m_Axis.m_sTitleX); 
	
	pDC->SelectObject(pOldFont);
	delete pNewFont;

	pNewFont = new CFont;
	pNewFont->CreateFont (17, 0, 900, 0, FALSE, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("Terminal"));

	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	if (!m_bCheckVert)
    	pDC->TextOut(m_Axis.m_rTitleY.left, m_Axis.m_rTitleY.bottom, _T("Gray Level")); 
    else
		pDC->TextOut(m_Axis.m_rTitleY.left, m_Axis.m_rTitleY.bottom, m_Axis.m_sTitleY); 

    pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CDlgSubProfile::DrawScrollBar(CDC *pDC)
{
	CRect rcRect, rcSlide;
	CBrush *brushBColor;

	if (m_bCheckScrollX)
	{
		brushBColor = new CBrush(m_colorB);
 		pDC->FillRect(m_rtScrollH, brushBColor);
	
		DrawFillCircle(pDC, (long)m_fFirstH, (m_rtScrollH.top+m_rtScrollH.bottom)/2, 8, m_colorF);
		DrawFillCircle(pDC, (long)m_fSecondH, (m_rtScrollH.top+m_rtScrollH.bottom)/2, 8, m_colorF);
		delete brushBColor;
	}
	
	if (m_bCheckScrollY)
	{
		brushBColor = new CBrush(m_colorB);
 		pDC->FillRect(m_rtScrollV, brushBColor);

		DrawFillCircle(pDC, (m_rtScrollV.left+m_rtScrollV.right)/2, (long)m_fFirstV, 8, m_colorF);
		DrawFillCircle(pDC, (m_rtScrollV.left+m_rtScrollV.right)/2, (long)m_fSecondV, 8, m_colorF);
		delete brushBColor;
	}
}

void CDlgSubProfile::GetScrollPosH()
{
	m_nFirstThresh = (long)(((m_fFirstH - m_rtScrollH.left)*WHITE_LEVEL)/(float)(m_rtScrollH.Width()-1));
	m_nSecondThresh = (long)(((m_fSecondH - m_rtScrollH.left)*WHITE_LEVEL)/(float)(m_rtScrollH.Width()-1));
}

void CDlgSubProfile::GetScrollPosV()
{
	m_nFirstThresh = (long)(((m_fFirstV - m_rtScrollV.top)*WHITE_LEVEL)/(float)(m_rtScrollV.Height()-1));
	m_nSecondThresh = (long)(((m_fSecondV - m_rtScrollV.top)*WHITE_LEVEL)/(float)(m_rtScrollV.Height()-1));
	m_nFirstThresh = WHITE_LEVEL - m_nFirstThresh;
	m_nSecondThresh = WHITE_LEVEL - m_nSecondThresh;
}

void CDlgSubProfile::SetScrollPosH(long nF, long nS)
{
	m_fFirstH = (m_rtScrollH.Width() * (float)nF)/WHITE_LEVEL + m_rtScrollH.left;
	m_fSecondH = (m_rtScrollH.Width() * (float)nS)/WHITE_LEVEL + m_rtScrollH.left;
}

void CDlgSubProfile::SetScrollPosV(long nF, long nS)
{
	m_fFirstV = (m_rtScrollV.Height() * (float)(WHITE_LEVEL-nF))/WHITE_LEVEL + m_rtScrollV.top;
	m_fSecondV = (m_rtScrollV.Height() * (float)(WHITE_LEVEL-nS))/WHITE_LEVEL + m_rtScrollV.top;
}

void CDlgSubProfile::DrawFillCircle(CDC *pDC, long x, long y, long nRadius, COLORREF nColor)
{
	CBrush *pNewBrush, *pOldBrush;
	CRect  rcRect;

	pNewBrush = new CBrush(nColor);
	pOldBrush = pDC->SelectObject(pNewBrush);

	rcRect.bottom = y+nRadius;
    rcRect.top = y-nRadius;
	rcRect.left = x-nRadius;
	rcRect.right = x+nRadius;

	pDC->Ellipse(rcRect);

	pDC->SelectObject(pOldBrush);
	delete pNewBrush;
}

void CDlgSubProfile::DrawYAxisGrid(CDC *pDC)
{
	long  nSlide;
	float dSy, dRange, dPixelPerSlide, dLevelPerSlide;
	CString szbuf;

	// 胶纳老 魂沥 ----------------------------------------------------
	dRange = (float)(m_Axis.m_nRangeMax - m_Axis.m_nRangeMin);
    dLevelPerSlide = dRange / (float)m_Axis.m_nSlidePerPlot;
	dPixelPerSlide = (float)(m_Axis.m_rPlot.Height()-1) / (float)m_Axis.m_nSlidePerPlot;
	// 胶纳老 魂沥 ----------------------------------------------------

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorSliderLine);
	CPen stick(PS_SOLID, 0, RGB(0, 0, 0));
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
	pDC->TextOut(m_Axis.m_rPlot.left-25, m_Axis.m_rPlot.bottom-5, szbuf);

	nSlide = 0;
	for(dSy=(float)(m_Axis.m_rPlot.bottom-dPixelPerSlide); dSy>=(float)m_Axis.m_rPlot.top; dSy-=dPixelPerSlide) 
	{
		old = pDC->SelectObject(&stick); 
		pDC->MoveTo(CPoint(m_Axis.m_rPlot.left-3, (long)dSy));
		pDC->LineTo(CPoint(m_Axis.m_rPlot.left, (long)dSy));

		nSlide++;
		szbuf.Format(_T("%d"), (long)(m_Axis.m_nRangeMin+nSlide*dLevelPerSlide));
		pDC->TextOut(m_Axis.m_rPlot.left-25, (long)(dSy-5), szbuf);
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

void CDlgSubProfile::DrawXAxisGrid(CDC * pDC)
{
	long  nSlide;
	float dSx, dRange, dPixelPerSlide, dLevelPerSlide;
	CString szbuf;

	// 胶纳老 魂沥 ----------------------------------------------------
	dRange = (float)(m_Axis.m_nRangeMax - m_Axis.m_nRangeMin);
    dLevelPerSlide = dRange / (float)m_Axis.m_nSlidePerPlot;
	dPixelPerSlide = (float)(m_Axis.m_rPlot.Width()) / (float)m_Axis.m_nSlidePerPlot;
	// 胶纳老 魂沥 ----------------------------------------------------

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

    pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CDlgSubProfile::DrawPoly(CDC *pDC)
{
	if (m_bCheckVert)
		DrawYProfile(pDC);
	else
		DrawXProfile(pDC);
}

void CDlgSubProfile::DrawXProfile(CDC *pDC)
{
	CPoint ptStart, ptEnd, ptMid;
	GOM_ATTR *pGOM;
	long  nGrayValue , nRangeX , nRangeY , nGrayRangeX , nGrayRangeY;
	long  i, nNum, nOffset, nWidth, nHeight;
    long  *pFt;
	float a, b;

	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
	pGOM = &m_pData->m_GOM[m_nSelectedROI];
	m_nGOMType = pGOM->m_nGOMType;

	nNum = 0;
	a = pGOM->m_ptPoint2.x-pGOM->m_ptPoint1.x;
	b = pGOM->m_ptPoint2.y-pGOM->m_ptPoint1.y;
	nWidth = (long)(sqrt(a*a + b*b)+0.5);

	a = pGOM->m_ptPoint3.x-pGOM->m_ptPoint1.x;
	b = pGOM->m_ptPoint3.y-pGOM->m_ptPoint1.y;
	nHeight = (long)(sqrt(a*a + b*b)+0.5);

	nNum = nWidth;
	pFt  = new long[nNum+5];

	ptStart.x = 0;
	ptStart.y = 0;
	ptEnd.x = nWidth-1;
	ptEnd.y = nHeight-1;

	if (m_nScanType==SCAN_PROFILE_FT)
	{
		ptMid.x = (ptStart.x + ptEnd.x) / 2;
		ptMid.y = (ptStart.y + ptEnd.y) / 2;
		if (m_nScanDir==DIR_LEFT_RIGHT || m_nScanDir==DIR_RIGHT_LEFT)
		{
			ptStart.x = 0;
			ptStart.y = ptMid.y-1;

			ptEnd.x = nWidth-1;
			ptEnd.y = ptMid.y+1;
		}
	}

    GetXSumGray(m_pImage, pFt, (long)pGOM->m_ptPoint1.x, (long)pGOM->m_ptPoint1.y, (long)pGOM->m_ptPoint4.x, (long)pGOM->m_ptPoint4.y, m_pData->m_nImageWidth);

	nRangeX = ptEnd.x - ptStart.x;
	nRangeY = ptEnd.y - ptStart.y;
	nGrayRangeX	= m_Axis.m_rPlot.right - m_Axis.m_rPlot.left + 1;
	nGrayRangeY	= m_Axis.m_rPlot.bottom- m_Axis.m_rPlot.top + 1;

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorDataLine);
	old = pDC->SelectObject(&pen); 

	if (m_nScanType==SCAN_PROFILE_FT || m_nScanType==SCAN_PROJECT_FT)
	{
		nGrayValue =  pFt[0] * nGrayRangeY / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left, m_Axis.m_rPlot.bottom - nGrayValue);

		for(i=1; i<nWidth; i++)
		{
			nGrayValue = pFt[i] * nGrayRangeY / IMAGE_LEVEL;
			nOffset	=	i * nGrayRangeX / nRangeX;

			pDC->LineTo(m_Axis.m_rPlot.left + nOffset, m_Axis.m_rPlot.bottom - nGrayValue);
			pDC->MoveTo(m_Axis.m_rPlot.left + nOffset, m_Axis.m_rPlot.bottom - nGrayValue);
		}
	}
	else
	{
		nGrayValue =  (MEDIUM_LEVEL+pFt[0])*nGrayRangeY / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left, m_Axis.m_rPlot.bottom - nGrayValue);

		for(i=1; i<nWidth; i++)
		{
			nGrayValue = (MEDIUM_LEVEL+pFt[i])*nGrayRangeY/IMAGE_LEVEL;
			nOffset	= i * nGrayRangeX / nRangeX;

			pDC->LineTo(m_Axis.m_rPlot.left + nOffset, m_Axis.m_rPlot.bottom - nGrayValue);
			pDC->MoveTo(m_Axis.m_rPlot.left + nOffset, m_Axis.m_rPlot.bottom - nGrayValue);
		}

		nOffset	=	MEDIUM_LEVEL * nGrayRangeY / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left, m_Axis.m_rPlot.bottom-nOffset);
		pDC->LineTo(m_Axis.m_rPlot.right, m_Axis.m_rPlot.bottom-nOffset);
    }

   	pDC->SelectObject(old);

	if (pFt) delete pFt;
}

void CDlgSubProfile::DrawYProfile(CDC *pDC)
{
	CPoint ptStart, ptEnd, ptMid;
	GOM_ATTR *pGOM;
	long  nGrayValue , nRangeX , nRangeY , nGrayRangeX , nGrayRangeY;
	long  i, nNum, nOffset, nWidth, nHeight;
    long  *pFt;
	float a, b;

	m_pImage = (LPBYTE)m_pParent->GetRawImagePtr();
	pGOM = &m_pData->m_GOM[m_nSelectedROI];
	m_nGOMType = pGOM->m_nGOMType;

	nNum = 0;
	a = pGOM->m_ptPoint2.x-pGOM->m_ptPoint1.x;
	b = pGOM->m_ptPoint2.y-pGOM->m_ptPoint1.y;
	nWidth = (long)(sqrt(a*a + b*b)+0.5);

	a = pGOM->m_ptPoint3.x-pGOM->m_ptPoint1.x;
	b = pGOM->m_ptPoint3.y-pGOM->m_ptPoint1.y;
	nHeight = (long)(sqrt(a*a + b*b)+0.5);

	nNum = nHeight;
	pFt  = new long[nNum+5];

	ptStart.x = 0;
	ptStart.y = 0;
	ptEnd.x = nWidth-1;
	ptEnd.y = nHeight-1;

	if (m_nScanType==SCAN_PROFILE_FT || m_nScanType==SCAN_PROFILE_DFDT)
	{
		ptMid.x = (ptStart.x + ptEnd.x) / 2;
		ptMid.y = (ptStart.y + ptEnd.y) / 2;
		if (m_nScanDir==DIR_TOP_BOTTOM || m_nScanDir==DIR_BOTTOM_TOP)
		{
			ptStart.x = ptMid.x-1;
			ptStart.y = 0;

			ptEnd.x = ptMid.x+1;
			ptEnd.y = nHeight-1;
		}
	}

	GetYSumGray(m_pImage, pFt, (long)pGOM->m_ptPoint1.x, (long)pGOM->m_ptPoint1.y, (long)pGOM->m_ptPoint4.x, (long)pGOM->m_ptPoint4.y, m_pData->m_nImageWidth);

	nRangeX = ptEnd.x - ptStart.x;
	nRangeY = ptEnd.y - ptStart.y;
	nGrayRangeX	=	m_Axis.m_rPlot.right - m_Axis.m_rPlot.left + 1;
	nGrayRangeY	=	m_Axis.m_rPlot.bottom - m_Axis.m_rPlot.top + 1;

	CPen *old, pen(PS_SOLID, 1, m_Axis.m_colorDataLine);
	old = pDC->SelectObject(&pen);

	if (m_nScanType==SCAN_PROFILE_FT || m_nScanType==SCAN_PROJECT_FT)
	{
		nGrayValue =  pFt[0] * nGrayRangeX / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left + nGrayValue, m_Axis.m_rPlot.top);
		for(i=1; i<nNum; i++)
		{
			nGrayValue = pFt[i] * nGrayRangeX / IMAGE_LEVEL;
			nOffset	=	i * nGrayRangeY / nRangeY;
			
			pDC->LineTo(m_Axis.m_rPlot.left + nGrayValue , m_Axis.m_rPlot.top + nOffset);
			pDC->MoveTo(m_Axis.m_rPlot.left + nGrayValue , m_Axis.m_rPlot.top + nOffset);
		}
	}
	else
	{
		nGrayValue =  (MEDIUM_LEVEL+pFt[0]) * nGrayRangeX / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left + nGrayValue, m_Axis.m_rPlot.top);
		for(i=1; i<nNum; i++)
		{
			nGrayValue = (MEDIUM_LEVEL+pFt[i]) * nGrayRangeX / IMAGE_LEVEL;
			nOffset	=	i * nGrayRangeY / nRangeY;
			
			pDC->LineTo(m_Axis.m_rPlot.left + nGrayValue , m_Axis.m_rPlot.top + nOffset);
			pDC->MoveTo(m_Axis.m_rPlot.left + nGrayValue , m_Axis.m_rPlot.top + nOffset);
		}

		nOffset	=	MEDIUM_LEVEL * nGrayRangeX / IMAGE_LEVEL;
		pDC->MoveTo(m_Axis.m_rPlot.left+nOffset, m_Axis.m_rPlot.top);
		pDC->LineTo(m_Axis.m_rPlot.left+nOffset, m_Axis.m_rPlot.bottom);
    }

	pDC->SelectObject(old);

	if (pFt) delete pFt;
}

void CDlgSubProfile::GetXSumGray(LPBYTE pImage, long *pFt, long x1, long y1, long x2, long y2, long nPitch)
{
	long i, j, nSum, nCnt;

    nCnt = abs(y2-y1)+1;
	if (nCnt<=0) return;

	for(i=x1; i<=x2; i++)
	{
    	nSum = 0;
		for(j=y1; j<=y2; j++)
			nSum += pImage[j*nPitch + i];
		
		pFt[i-x1] = nSum/nCnt; 	
	}
}

void CDlgSubProfile::GetYSumGray(LPBYTE pImage, long *pFt, long x1, long y1, long x2, long y2, long nPitch)
{
	long i, j, nSum, nCnt;

    nCnt = abs(x2-x1)+1;
	if (nCnt<=0) return;

	for(i=y1; i<=y2; i++)
	{
		nSum = 0;
		for(j=x1; j<=x2; j++)
			nSum += pImage[i*nPitch + j];
		
		pFt[i-y1] = nSum/nCnt; 	
	}
}

void CDlgSubProfile::OnRadioTop1() 
{
	m_nFirstScanDirType = DIR_BOTTOM_TOP;
	m_bCheckVert = TRUE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(0);
}

void CDlgSubProfile::OnRadioBot1() 
{
	m_nFirstScanDirType = DIR_TOP_BOTTOM;
	m_bCheckVert = TRUE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(0);
}

void CDlgSubProfile::OnRadioRight1() 
{
    m_nFirstScanDirType = DIR_LEFT_RIGHT;
	m_bCheckVert = FALSE;
	if (m_nInspectType<=INSP_MISMATCH) 
	    AdjustRadioControls(0);
}

void CDlgSubProfile::OnRadioLeft1() 
{
    m_nFirstScanDirType = DIR_RIGHT_LEFT;
	m_bCheckVert = FALSE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(0);
}

void CDlgSubProfile::OnRadioTop2() 
{
	m_nSecondScanDirType = DIR_BOTTOM_TOP;
	m_bCheckVert = TRUE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(1);
}

void CDlgSubProfile::OnRadioBot2() 
{
	m_nSecondScanDirType = DIR_TOP_BOTTOM;
	m_bCheckVert = TRUE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(1);
}

void CDlgSubProfile::OnRadioRight2() 
{
    m_nSecondScanDirType = DIR_LEFT_RIGHT;
	m_bCheckVert = FALSE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(1);
}

void CDlgSubProfile::OnRadioLeft2() 
{
    m_nSecondScanDirType = DIR_RIGHT_LEFT;
	m_bCheckVert = FALSE;
	if (m_nInspectType<=INSP_MISMATCH)
	    AdjustRadioControls(1);
}

void CDlgSubProfile::AdjustRadioControls(long nSel)
{
	UpdateData(true);

	if (nSel==0)
	{
		switch (m_nFirstScanDirType)
		{
			case DIR_LEFT_RIGHT : 
				if (((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->SetCheck(TRUE);
					m_nSecondScanDirType = DIR_LEFT_RIGHT;
				}
				break;
			case DIR_RIGHT_LEFT :
				if (((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->SetCheck(TRUE);
					m_nSecondScanDirType = DIR_RIGHT_LEFT;
				}
				break;
			case DIR_TOP_BOTTOM :
				if (((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_2))->SetCheck(TRUE);
					m_nSecondScanDirType = DIR_TOP_BOTTOM;
				}
			case DIR_BOTTOM_TOP:
				if (((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_2))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_TOP_2))->SetCheck(TRUE);
					m_nSecondScanDirType = DIR_BOTTOM_TOP;
				}
		}
	}
	else
	{
		switch (m_nSecondScanDirType)
		{
			case DIR_LEFT_RIGHT : 
				if (((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->SetCheck(TRUE);
					m_nFirstScanDirType = DIR_LEFT_RIGHT;
				}
				break;
			case DIR_RIGHT_LEFT :
				if (((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->SetCheck(TRUE);
					m_nFirstScanDirType = DIR_RIGHT_LEFT;
				}
				break;
			case DIR_TOP_BOTTOM :
				if (((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_BOT_1))->SetCheck(TRUE);
					m_nFirstScanDirType = DIR_TOP_BOTTOM;
				}
			case DIR_BOTTOM_TOP:
				if (((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->GetCheck() ||
				    ((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->GetCheck())
				{
					((CButton *)GetDlgItem(IDC_RADIO_LEFT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_RIGHT_1))->SetCheck(FALSE);
					((CButton *)GetDlgItem(IDC_RADIO_TOP_1))->SetCheck(TRUE);
					m_nFirstScanDirType = DIR_BOTTOM_TOP;
				}
		}
	}

	m_pParent->m_pDlgVisionSetup->UpdateFrameRect();
    m_pParent->InvalidateRect(m_pParent->m_rcPlotBounds, false);
}

void CDlgSubProfile::OnBnClickedBtnPol1()
{
	CMenu hMenu, *hSubMenu;

	CPoint ptCur;
	GetCursorPos(&ptCur);

	hMenu.LoadMenu(IDR_MENU_DIR_POLARITY);
	hSubMenu = hMenu.GetSubMenu(0);

	hSubMenu->TrackPopupMenu(TPM_LEFTALIGN, ptCur.x, ptCur.y, this);
	m_bSelLeftPol = TRUE;
	// TODO: 咯扁俊 皋矫瘤 贸府扁 内靛甫 眠啊钦聪促.
}

void CDlgSubProfile::OnBnClickedBtnPol2()
{
	CMenu hMenu, *hSubMenu;

	CPoint ptCur;
	GetCursorPos(&ptCur);

	hMenu.LoadMenu(IDR_MENU_DIR_POLARITY);
	hSubMenu = hMenu.GetSubMenu(0);

	hSubMenu->TrackPopupMenu(TPM_LEFTALIGN, ptCur.x, ptCur.y, this);
	m_bSelLeftPol = FALSE;
}

void CDlgSubProfile::OnBlackWhite()
{
	if (m_bSelLeftPol)
	{
		m_nFirstPolDirType = POL_BLACK_WHITE;
		m_btnPol1.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
	}
	else
	{
		m_nSecondPolDirType = POL_BLACK_WHITE;
		m_btnPol2.SetBitmaps(IDB_BMP_BLACK_WHITE, RGB(255, 0, 0));
	}			
	// TODO: 咯扁俊 疙飞 贸府扁 内靛甫 眠啊钦聪促.
}

void CDlgSubProfile::OnWhiteBlack()
{
	if (m_bSelLeftPol)
	{
		m_nFirstPolDirType = POL_WHITE_BLACK;
		m_btnPol1.SetBitmaps(IDB_BMP_WHITE_BLACK, RGB(255, 0, 0));
	}
	else
	{
		m_nSecondPolDirType = POL_WHITE_BLACK;
		m_btnPol2.SetBitmaps(IDB_BMP_WHITE_BLACK, RGB(255, 0, 0));
	}			
}

void CDlgSubProfile::OnBnClickedRadioLocLeft()
{
	// TODO: 咯扁俊 牧飘费 舅覆 贸府扁 内靛甫 眠啊钦聪促.
}


void CDlgSubProfile::OnBnClickedRadioLocRight()
{
	// TODO: Add your control notification handler code here
}


void CDlgSubProfile::OnBnClickedRadioMultiple()
{
	// TODO: Add your control notification handler code here
}

