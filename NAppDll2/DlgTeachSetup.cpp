// DlgTeachSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVDisp.h"
#include "math.h"
#include "DlgTeachSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgTeachSetup::CDlgTeachSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeachSetup::IDD, pParent)
	, m_editModifyName(_T(""))
	, m_strValueA(_T(""))
{
	//{{AFX_DATA_INIT(CDlgTeachSetup)
	m_staticLinkItem1 = _T("");
	m_staticLinkItem2 = _T("");
	m_strLowLimit = _T("");
	m_strHighLimit = _T("");
	m_strOffsetV = _T("");
	m_nLinkType = TEACH_NONE;
	m_nSelectedROI1 = -1;
	m_nSelectedROI2 = -1;
	//}}AFX_DATA_INIT

	m_strRelation = _T("");
	m_pParent = (CDlgVDisp *)pParent;
	m_pData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelDataPtr();
	m_pBackData = (SETUP_MODEL_DATA *)m_pParent->m_pModelData->GetModelBackDataPtr();
	m_strOffsetX = _T("");
	m_strOffsetY = _T("");
}

void CDlgTeachSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTeachSetup)
	DDX_Control(pDX, IDC_LIST_ITEM_LINK, m_listLinkItem);
	DDX_Control(pDX, IDC_LIST_LOC_ITEM, m_listLocItem);
	DDX_Control(pDX, IDC_BTN_ITEM_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BTN_ITEM_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);	
	DDX_Control(pDX, IDC_BTN_LINK, m_btnLink);
	DDX_Control(pDX, IDC_BTN_LOC, m_btnLoc);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_CHECK_DIST_PNT2LINE, m_btnToolDistPnt2Line);
	DDX_Control(pDX, IDC_CHECK_DIST_LINE2LINE, m_btnToolDistLine2Line);
	DDX_Control(pDX, IDC_CHECK_ANG_PNT2PNT, m_btnToolAngPnt2Pnt);
	DDX_Control(pDX, IDC_CHECK_ANG_LINE2LINE, m_btnToolAngLine2Line);
	DDX_Control(pDX, IDC_CHECK_ANG_LINE4PNT, m_btnToolAngLine4Pnt);
	DDX_Text(pDX, IDC_STATIC_LINK_ITEM1, m_staticLinkItem1);
	DDX_Text(pDX, IDC_STATIC_LINK_ITEM2, m_staticLinkItem2);
	DDX_Text(pDX, IDC_EDIT_LOW_LIMIT, m_strLowLimit);
	DDX_Text(pDX, IDC_EDIT_HIGH_LIMIT, m_strHighLimit);
	DDX_Text(pDX, IDC_EDIT_OFFSET_VALUE, m_strOffsetV);
	DDX_Text(pDX, IDC_EDIT_LINK_NAME, m_editModifyName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_OFFSET_X, m_strOffsetX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_Y, m_strOffsetY);
	DDX_Text(pDX, IDC_EDIT_A_VALUE, m_strValueA);
}

BEGIN_MESSAGE_MAP(CDlgTeachSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgTeachSetup)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ITEM_APPLY, OnBtnItemApply)
	ON_BN_CLICKED(IDC_BTN_LOC, OnBtnLoc)
	ON_BN_CLICKED(IDC_BTN_LINK, OnBtnLink)
	ON_BN_CLICKED(IDC_BTN_LINK_INSERT1, OnBtnLinkInsert1)
	ON_BN_CLICKED(IDC_BTN_LINK_INSERT2, OnBtnLinkInsert2)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_CHECK_NONE, OnCheckNone)
	ON_BN_CLICKED(IDC_CHECK_DIST_LINE2LINE, OnCheckDistLine2Line)
	ON_BN_CLICKED(IDC_CHECK_ANG_PNT2PNT, OnCheckAngPnt2Pnt)
	ON_BN_CLICKED(IDC_CHECK_DIST_PNT2LINE, OnCheckDistPnt2Line)
	ON_BN_CLICKED(IDC_CHECK_ANG_LINE2LINE, OnCheckAngLine2Line)
	ON_LBN_SELCHANGE(IDC_LIST_ITEM_LINK, OnSelchangeListItemLink)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgTeachSetup::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_ITEM_COPY, &CDlgTeachSetup::OnBnClickedBtnItemCopy)
	ON_BN_CLICKED(IDC_RADIO_FIRST_ROI, &CDlgTeachSetup::OnBnClickedRadioFirstRoi)
	ON_BN_CLICKED(IDC_RADIO_SECOND, &CDlgTeachSetup::OnBnClickedRadioSecond)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CDlgTeachSetup::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CDlgTeachSetup::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgTeachSetup::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DN, &CDlgTeachSetup::OnBnClickedButtonDn)
	//}}AFX_MSG_MAP

	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_DISP_ROI, &CDlgTeachSetup::OnBnClickedCheckDispRoi)
	ON_STN_CLICKED(IDC_BUTTON_LEFT, &CDlgTeachSetup::OnStnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BTN_COMPENS, &CDlgTeachSetup::OnBnClickedBtnCompens)
	ON_COMMAND(IDM_MENU_DIST2EDGE, &CDlgTeachSetup::OnMenuDist2edge)
	ON_COMMAND(IDM_MENU_PNT_LINE, &CDlgTeachSetup::OnMenuPntLine)
	ON_COMMAND(IDM_MENU_1stLine_Line, &CDlgTeachSetup::OnMenu1stLineToLine)
	ON_COMMAND(IDM_MENU_2stLine_Line, &CDlgTeachSetup::OnMenu2stLineToLine)
	ON_COMMAND(IDM_MENU_NADIST_LINETOLINE, &CDlgTeachSetup::OnMenuNALineToLine)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeachSetup message handlers
BOOL CDlgTeachSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControls();
	
	return TRUE;  
}

void CDlgTeachSetup::InitControls()
{
	short int shBtnColor=30;

	m_btnCopy.SetIcon(IDI_ICON_COPY);
	m_btnCopy.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnCopy.DrawTransparent();

	m_btnApply.SetIcon(IDI_ICON_APPLY);
	m_btnApply.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnApply.DrawTransparent();

	m_btnDelete.SetIcon(IDI_ICON_DELETE);
	m_btnDelete.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnDelete.DrawTransparent();

	m_btnSave.SetIcon(IDI_ICON_SAVE);
	m_btnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnSave.DrawTransparent();

	m_btnLink.SetIcon(IDI_ICON_RIGHT);
	m_btnLink.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnLink.SetFlat(FALSE);

	m_btnLoc.SetIcon(IDI_ICON_RIGHT);
	m_btnLoc.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnLoc.SetFlat(FALSE);

	m_bRearOption = FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_FIRST_ROI))->SetCheck(TRUE);

	GetDlgItem(IDC_BTN_LINK_INSERT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK_INSERT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LOC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_NONE))->SetCheck(TRUE);

	if (m_pParent->m_nLngSel==0) 
	{
		m_btnCopy.SetWindowText(_T("복사"));
		m_btnApply.SetWindowText(_T("적용"));
		m_btnDelete.SetWindowText(_T("삭제"));
		m_btnSave.SetWindowText(_T("저장"));
		GetDlgItem(IDC_STATIC_LOCCOMPENS)->SetWindowText(_T(" 위치보정 "));
		GetDlgItem(IDC_BTN_COMPENS)->SetWindowText(_T(" 위치보정 "));
		GetDlgItem(IDC_STATIC_LINK)->SetWindowText(_T(" 항목연결 "));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" 허용공차 "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("하한 - 상한"));
		GetDlgItem(IDC_STATIC_OFFSET)->SetWindowText(_T("보정치"));
		GetDlgItem(IDC_RADIO_FIRST_ROI)->SetWindowText(_T("항목위치(앞)"));
		GetDlgItem(IDC_RADIO_SECOND)->SetWindowText(_T("항목위치(뒤)"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T("검사형태"));	
	}
	else if (m_pParent->m_nLngSel==1) 
	{
		m_btnCopy.SetWindowText(_T("COPY"));
		m_btnApply.SetWindowText(_T("APPLY"));
		m_btnDelete.SetWindowText(_T("DELETE"));
		m_btnSave.SetWindowText(_T("SAVE"));
		GetDlgItem(IDC_STATIC_LOCCOMPENS)->SetWindowText(_T(" LOC. COMPENS. "));
		GetDlgItem(IDC_BTN_COMPENS)->SetWindowText(_T("LOC. COMPENS."));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" Tolerrance "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("LSL-USL"));
		GetDlgItem(IDC_STATIC_OFFSET)->SetWindowText(_T("Offset V"));
		GetDlgItem(IDC_RADIO_FIRST_ROI)->SetWindowText(_T("LOC. ROI(FRONT)"));
		GetDlgItem(IDC_RADIO_SECOND)->SetWindowText(_T("LOC. ROI(REAR)"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T("INSP. TYPE"));				
	}
	else
	{
		m_btnCopy.SetWindowText(_T("复制"));
		m_btnApply.SetWindowText(_T("适用"));
		m_btnDelete.SetWindowText(_T("删除"));
		m_btnSave.SetWindowText(_T("保存"));
		GetDlgItem(IDC_STATIC_LOCCOMPENS)->SetWindowText(_T(" 位置更正 "));
		GetDlgItem(IDC_BTN_COMPENS)->SetWindowText(_T(" 位置更正 "));
		GetDlgItem(IDC_STATIC_LINK)->SetWindowText(_T(" 项目连接 "));
		GetDlgItem(IDC_STATIC_TOLERR)->SetWindowText(_T(" 许可公差 "));
		GetDlgItem(IDC_STATIC_SPEC)->SetWindowText(_T("下限 - 上限"));
		GetDlgItem(IDC_STATIC_OFFSET)->SetWindowText(_T("更正値"));
		GetDlgItem(IDC_RADIO_FIRST_ROI)->SetWindowText(_T("项目位置(前)"));
		GetDlgItem(IDC_RADIO_SECOND)->SetWindowText(_T("项目位置(後)"));
		GetDlgItem(IDC_STATIC_INSPTYPE)->SetWindowText(_T("检查形態"));	
	}
}

void CDlgTeachSetup::OnBnClickedBtnItemCopy()
{
	BOOL bRet;
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T(" 선택 그룹의 항목을 복사하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want copy the selected group items ?  ");
	else strMessage = _T("  要复制参数集团吗 ?  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		bRet = m_pParent->m_pModelData->CopyFrontToRear();
		m_pParent->Invalidate(FALSE);
	}
}

void CDlgTeachSetup::OnBtnItemApply() 
{
	CString  strMessage, strTitle;
	long nPos, nSel1, nSel2;

 	UpdateData(true);

	if (m_pParent->m_nLngSel==0) strMessage = _T("  변경된 데이타를 적용하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want apply the changed data ?  ");
	else strMessage = _T("   要适用变更的密码吗 ?  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		m_pBackData->m_dSourX = m_pData->m_dSourX;
		m_pBackData->m_dSourY = m_pData->m_dSourY;
		m_pBackData->m_dSourT = m_pData->m_dSourT;

		m_pBackData->m_dSourX2 = m_pData->m_dSourX2;
		m_pBackData->m_dSourY2 = m_pData->m_dSourY2;
		m_pBackData->m_dSourT2 = m_pData->m_dSourT2;

        nPos = m_listLinkItem.GetCurSel();
		if (nPos>=0)
		{
			m_pData->m_Teach.m_LINK[nPos].m_dStdSpec = (float)_ttof(m_strLowLimit);
			m_pData->m_Teach.m_LINK[nPos].m_dToleSpec = (float)_ttof(m_strHighLimit);
//			m_pData->m_Teach.m_LINK[nPos].m_dStandLowLimit = m_pData->m_Teach.m_LINK[nPos].m_dStdSpec - m_pData->m_Teach.m_LINK[nPos].m_dToleSpec;
//			m_pData->m_Teach.m_LINK[nPos].m_dStandHighLimit = m_pData->m_Teach.m_LINK[nPos].m_dStdSpec + m_pData->m_Teach.m_LINK[nPos].m_dToleSpec;
			m_pData->m_Teach.m_LINK[nPos].m_dStandLowLimit = (float)_ttof(m_strLowLimit);
			m_pData->m_Teach.m_LINK[nPos].m_dStandHighLimit = (float)_ttof(m_strHighLimit);
			m_pData->m_Teach.m_LINK[nPos].m_dOffsetV = (float)_ttof(m_strOffsetV);
			m_pData->m_Teach.m_LINK[nPos].m_dValueA = (float)_ttof(m_strValueA);

			strTitle.Format(_T("%s"), m_pData->m_Teach.m_LINK[nPos].m_strTitle);
			m_pParent->m_pModelData->SetLinkLimitData(strTitle, m_pData->m_Teach.m_LINK[nPos].m_dStandLowLimit, m_pData->m_Teach.m_LINK[nPos].m_dStandHighLimit);

			nSel1 = m_pData->m_Teach.m_LINK[nPos].m_nIndex1;
			nSel2 = m_pData->m_Teach.m_LINK[nPos].m_nIndex2;

			/*
			bBaseL1 = m_pData->m_INSP[nSel1].m_bBaseLine;
	        bBaseL2 = m_pData->m_INSP[nSel2].m_bBaseLine;
			if ((!bBaseL1 && !bBaseL2) || (bBaseL1 && bBaseL2))
			{
				m_pData->m_INSP[nSel1].m_bBaseLine = TRUE;
				m_pData->m_INSP[nSel2].m_bBaseLine = FALSE;
			}
			*/
		}

		m_pData->m_Teach.m_nLinkNum = m_listLinkItem.GetCount();
	    m_pData->m_Teach.m_nLocNum = m_listLocItem.GetCount();
		m_pData->m_Teach.m_fOffsetX = (float)_ttof(m_strOffsetX);
		m_pData->m_Teach.m_fOffsetY = (float)_ttof(m_strOffsetY);

        m_pParent->m_pModelData->CopyModelData(m_pData, m_pBackData);
	}
}

void CDlgTeachSetup::UpdateSelectItemData()
{
	BOOL bLoc;
	CString str, strSelectedTitle;
	long i, j, nSel1, nSel2, nSelectedROI, nType;
	float fTempX, fTempY;

	GetDlgItem(IDC_BTN_LINK_INSERT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK_INSERT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LOC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);

	m_listLocItem.SetCurSel(-1);
	m_listLinkItem.SetCurSel(-1);
	m_listLocItem.ResetContent();
	m_listLinkItem.ResetContent();

	fTempX = m_pData->m_Teach.m_fOffsetX;
	fTempY = m_pData->m_Teach.m_fOffsetY;

	if (fabs(fTempX) < 0) fTempX = 0.0f;
	if (fabs(fTempY) < 0) fTempY = 0.0f;

	m_strOffsetX.Format(_T("%5.2f"), fTempX);
	m_strOffsetY.Format(_T("%5.2f"), fTempY);

	for(i=0; i<LOCATE_MAX_NUM; i++)
	{
		strSelectedTitle = m_pData->m_Teach.m_LOC[i].m_strTitle;
		if (_tcscmp(strSelectedTitle, _T("")))
		{
			nSel1 = m_pData->m_Teach.m_LOC[i].m_nIndex1;
			bLoc = m_pData->m_INSP[nSel1].m_bLocROI;

			if (!bLoc) str.Format(_T("%s(%s)"), strSelectedTitle, _T("FRONT"));
			else str.Format(_T("%s(%s)"), strSelectedTitle, _T("REAR"));

			m_listLocItem.AddString(str);
		}
    }

	for(i=0; i<LINK_MAX_NUM; i++)
	{
		strSelectedTitle = m_pData->m_Teach.m_LINK[i].m_strTitle;
		if (_tcscmp(strSelectedTitle, _T("")))
		{
			nSel1 = m_pData->m_Teach.m_LINK[i].m_nIndex1;
			nSel2 = m_pData->m_Teach.m_LINK[i].m_nIndex2;
			bLoc = m_pData->m_INSP[nSel1].m_bLocROI;

			if (!_tcscmp(m_pData->m_GOM[nSel1].m_strTitle, _T("")) || !_tcscmp(m_pData->m_GOM[nSel2].m_strTitle, _T("")))
			{
				for(j=i; j<LINK_MAX_NUM-1; j++)
					memcpy(&m_pData->m_Teach.m_LINK[j], &m_pData->m_Teach.m_LINK[j+1], sizeof(TEACH_ATTR));

				m_listLinkItem.DeleteString(i);
				m_pData->m_Teach.m_nLinkNum = m_listLinkItem.GetCount();
			}
			else
			{
				if (!bLoc) str.Format(_T("%s(%s)"), strSelectedTitle, _T("FRONT"));
				else str.Format(_T("%s(%s)"), strSelectedTitle, _T("REAR"));

				m_listLinkItem.AddString(str);
			}
		}
    }

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	strSelectedTitle = m_pParent->m_pModelData->GetSelectedTitle();
	if (nSelectedROI>=0)
	{
		nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		if (INSP_MEASURE_SIDE_ONE<=nType && nType<=INSP_DATUM_B || nType==INSP_PATTERN_MATCH)
		{
			GetDlgItem(IDC_BTN_LINK_INSERT1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_LINK_INSERT2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_LOC)->EnableWindow(TRUE);
        }
	}
	else
	{
		m_staticLinkItem1 = _T("");
		m_staticLinkItem2 = _T("");
	}

	UpdateData(false);
}

void CDlgTeachSetup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		UpdateSelectItemData();
}

void CDlgTeachSetup::OnBtnLoc() 
{
	BOOL bLoc;
	long nSel, nPos, nType;
	long nSelectedROI;
	CString str, strTitle, strSelectedTitle;

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	strSelectedTitle = m_pParent->m_pModelData->GetSelectedTitle();

	nSel = m_listLocItem.GetCount();
	if (nSelectedROI>=0)
	{
	    nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;
		bLoc = m_pData->m_INSP[nSelectedROI].m_bLocROI;
//		if (nType==INSP_MEASURE_SIDE_ONE || nType==INSP_MEASURE_SIDE_TWO)
		if (nType==INSP_PATTERN_MATCH || nType==INSP_MEASURE_SIDE_ONE)
		{
			if (!bLoc) str.Format(_T("%s(%s)"), strSelectedTitle, _T("FRONT"));
			else str.Format(_T("%s(%s)"), strSelectedTitle, _T("REAR"));

			nPos = m_listLocItem.FindString(0, str);
			if (nPos<0)
			{
				m_listLocItem.AddString(str);
				m_pData->m_Teach.m_LOC[nSel].m_bSel1 = FALSE;
				_tcscpy_s(m_pData->m_Teach.m_LOC[nSel].m_strTitle, strSelectedTitle);
				m_pData->m_Teach.m_LOC[nSel].m_nIndex1 = nSelectedROI;
    			m_pData->m_Teach.m_LOC[nSel].m_nIndex2 = -1;
			}
		}
	}

    m_pData->m_Teach.m_nLocNum = m_listLocItem.GetCount();
   	GetDlgItem(IDC_BTN_LOC)->EnableWindow(FALSE);
}

void CDlgTeachSetup::OnBtnLinkInsert1() 
{
 	long nSelectedROI, nCnt, nType;
	CString strSelectedTitle;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	strSelectedTitle = m_pParent->m_pModelData->GetSelectedTitle();
	m_nSelectedROI1 = nSelectedROI;
	nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;

	nCnt = 0;
	if (nSelectedROI>=0 && (INSP_MEASURE_SIDE_ONE<=nType && nType<=INSP_DATUM_B) || nType==INSP_PATTERN_MATCH)
	{
		if (m_staticLinkItem2!=strSelectedTitle)
			m_staticLinkItem1 = strSelectedTitle;

		if (m_staticLinkItem1!=_T("")) nCnt++;
		if (m_staticLinkItem2!=_T("")) nCnt++;

		if (nCnt>1)
    		GetDlgItem(IDC_BTN_LINK)->EnableWindow(TRUE);
		else
    		GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
	}

	UpdateData(false);
}

void CDlgTeachSetup::OnBtnLinkInsert2() 
{
 	long nSelectedROI, nCnt, nType;
	CString strSelectedTitle;

	UpdateData(true);

	nSelectedROI = m_pParent->m_pModelData->GetIndexSelectedROI();
	strSelectedTitle = m_pParent->m_pModelData->GetSelectedTitle();
	m_nSelectedROI2 = nSelectedROI;
	nType = m_pData->m_INSP[nSelectedROI].m_nInspectType;

	nCnt = 0;
	if (nSelectedROI>=0 && (INSP_MEASURE_SIDE_ONE<=nType && nType<=INSP_DATUM_B) || nType==INSP_PATTERN_MATCH)
	{
		if (m_staticLinkItem1!=strSelectedTitle)
			m_staticLinkItem2 = strSelectedTitle;

		if (m_staticLinkItem1!=_T("")) nCnt++;
		if (m_staticLinkItem2!=_T("")) nCnt++;

		if (nCnt>1) GetDlgItem(IDC_BTN_LINK)->EnableWindow(TRUE);
		else   		GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
	}

	UpdateData(false);
}

void CDlgTeachSetup::OnBtnLink()
{
	CMenu hMenu, *hSubMenu;
	CString strMessage;
	long nType1, nType2;

	UpdateData(true);

	CPoint ptCur;
	GetCursorPos(&ptCur);

	if (m_editModifyName!=_T("") && m_nSelectedROI1>=0 && m_nSelectedROI2>=0)
	{
		nType1 = m_pData->m_INSP[m_nSelectedROI1].m_nInspectType;
	    nType2 = m_pData->m_INSP[m_nSelectedROI2].m_nInspectType;
		if ((INSP_MEASURE_SIDE_ONE==nType1 && INSP_MEASURE_SIDE_ONE==nType2) || 
			(INSP_SLINE<=nType1 && nType1<=INSP_DATUM_B && INSP_SLINE<=nType2 && nType2<=INSP_DATUM_B) ||
            (INSP_MEASURE_SIDE_ONE==nType1 && INSP_PATTERN_MATCH==nType2) || 
			(INSP_MEASURE_SIDE_ONE==nType2 && INSP_PATTERN_MATCH==nType1) ||
			(INSP_MEASURE_SIDE_ONE==nType1 && INSP_MEASURE_SIDE_TWO==nType2) ||
			(INSP_MEASURE_SIDE_ONE==nType2 && INSP_MEASURE_SIDE_TWO==nType1)) 
		{
			hMenu.LoadMenu(IDR_MENU_INSP_TYPE);
			hSubMenu = hMenu.GetSubMenu(0);

			hSubMenu->TrackPopupMenu(TPM_LEFTALIGN, ptCur.x, ptCur.y, this);
		}
		else
		{
			if (m_pParent->m_nLngSel==0) strMessage = _T(" 검사형태가 일치하지 않습니다.  ");
			else if (m_pParent->m_nLngSel==1) strMessage = _T("  Inspection type is mismatch ! ");
			else strMessage = _T("  检查TYPE不一致  ");

			MessageBox(strMessage, _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
		}
	}

	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
}

void CDlgTeachSetup::OnMenuPntLine()
{
	LinkRelationObject(TEACH_DIST_PNT2LINE);
}

void CDlgTeachSetup::OnMenuDist2edge()
{
    LinkRelationObject(TEACH_DIST_LINE2LINE);
}

void CDlgTeachSetup::OnMenu1stLineToLine()
{
	LinkRelationObject(TEACH_DIST_1STLINE2LINE);
}

void CDlgTeachSetup::OnMenu2stLineToLine()
{
	LinkRelationObject(TEACH_DIST_2STLINE2LINE);
}

void CDlgTeachSetup::OnMenuNALineToLine()
{
	LinkRelationObject(TEACH_NADIST_LINE2LINE);
}



void CDlgTeachSetup::LinkRelationObject(long nLinkType)
{
	BOOL bLoc, bRet=FALSE;
	long  nPos, nCnt;
	CString str, strT, strMessage;
	long nType1, nType2;

	UpdateData(true);

	nType1 = m_pData->m_INSP[m_nSelectedROI1].m_nInspectType;
	nType2 = m_pData->m_INSP[m_nSelectedROI2].m_nInspectType;

	if (nLinkType==TEACH_DIST_LINE2LINE || nLinkType==TEACH_NADIST_LINE2LINE)
	{
		if (nType1==INSP_MEASURE_SIDE_ONE || nType2==INSP_MEASURE_SIDE_ONE ||
			nType1==INSP_DATUM_B || nType2==INSP_DATUM_B)
			bRet = TRUE;
	}
	else if (nLinkType==TEACH_DIST_1STLINE2LINE)
	{
		if ((nType1==INSP_MEASURE_SIDE_ONE && nType2==INSP_MEASURE_SIDE_TWO) ||
		    (nType1==INSP_MEASURE_SIDE_TWO && nType2==INSP_MEASURE_SIDE_ONE)) 
		{ bRet = TRUE;  }
	}
	else if (nLinkType==TEACH_DIST_2STLINE2LINE)
	{
		if ((nType1==INSP_MEASURE_SIDE_ONE && nType2==INSP_MEASURE_SIDE_TWO) ||
		    (nType1==INSP_MEASURE_SIDE_TWO && nType2==INSP_MEASURE_SIDE_ONE))
		{ bRet = TRUE;  }
	}
	else if (nLinkType==TEACH_DIST_PNT2LINE)
	{
		if ((nType1==INSP_MEASURE_SIDE_ONE && nType2==INSP_PATTERN_MATCH) ||
		    (nType1==INSP_PATTERN_MATCH && nType2==INSP_MEASURE_SIDE_ONE))
			bRet = TRUE;
	}
	else
	{
		    bRet = FALSE;
	}

	if (bRet)
	{
		nCnt=0;

		nPos = m_listLinkItem.GetCount();
		bLoc = m_pData->m_INSP[m_nSelectedROI1].m_bLocROI;

		if (!bLoc) str.Format(_T("%s(%s)"), m_editModifyName, _T("FRONT"));
		else str.Format(_T("%s(%s)"), m_editModifyName, _T("REAR"));
		strT = WithoutSpaceToString(str);

		m_listLinkItem.AddString(strT);
		m_listLinkItem.SetCurSel(nPos);

		if (m_staticLinkItem1 != _T(""))
		{
			m_pData->m_Teach.m_LINK[nPos].m_bSel1 = TRUE;
			m_pData->m_Teach.m_LINK[nPos].m_nIndex1 = m_nSelectedROI1;
		}

		if (m_staticLinkItem2 != _T(""))
		{
			m_pData->m_Teach.m_LINK[nPos].m_bSel2 = TRUE;
			m_pData->m_Teach.m_LINK[nPos].m_nIndex2 = m_nSelectedROI2;
		}

		strT = WithoutSpaceToString(m_editModifyName);
		_tcscpy_s(m_pData->m_Teach.m_LINK[nPos].m_strTitle, strT);
		m_pData->m_Teach.m_LINK[nPos].m_dStandHighLimit = 0.0f;
		m_pData->m_Teach.m_LINK[nPos].m_dStandLowLimit = 0.0f;
		m_pData->m_Teach.m_LINK[nPos].m_dStdSpec = 0.0f;
		m_pData->m_Teach.m_LINK[nPos].m_dToleSpec = 0.0f;
		m_pData->m_Teach.m_LINK[nPos].m_nLinkType = nLinkType;
		m_pData->m_Teach.m_LINK[nPos].m_bLinkExec = TRUE;
		m_pData->m_Teach.m_LINK[nPos].m_bEjectROI = TRUE;
	    m_pData->m_Teach.m_nLinkNum = nPos+1;
		((CButton *)GetDlgItem(IDC_CHECK_DISP_ROI))->SetCheck(TRUE);

	    m_pParent->m_pModelData->CopyModelData(m_pData, m_pBackData);
//		LinkRelationObject(long nLinkType)
//		ToggleButtonControls(m_nLinkType);

		switch(nLinkType)
		{
			case TEACH_DIST_PNT2LINE :
				if (m_pParent->m_nLngSel==0) str = _T(" 측점검사 (교차점 - 라인) ");
				else if (m_pParent->m_nLngSel==1) str = _T(" DIST. (POINT - LINE) ");
				else str = _T(" DIST. (POINT - LINE) ");
				break;
			case TEACH_DIST_LINE2LINE :
				if (m_pParent->m_nLngSel==0) str = _T(" 라인과 라인의 거리 ");
				else if (m_pParent->m_nLngSel==1) str = _T(" DIST. (LINE - LINE) ");
				else str = _T(" 测量检查 ");
				break;

			case TEACH_DIST_1STLINE2LINE :
				if (m_pParent->m_nLngSel==0) str = _T(" 양방향에서 첫번째 라인과 라인의 거리 ");
				else if (m_pParent->m_nLngSel==1) str = _T(" DIST. (1st LINE - LINE) ");
				else str = _T(" 两个方向上第一行和第一行之间的距离 ");
				break;
			case TEACH_DIST_2STLINE2LINE :
				if (m_pParent->m_nLngSel==0) str = _T(" 양방향에서 두번째 라인과 라인의 거리 ");
				else if (m_pParent->m_nLngSel==1) str = _T(" DIST. (2nd LINE - LINE) ");
				else str = _T(" 两个方向上的线和线之间的距离 ");
				break;
			case TEACH_NADIST_LINE2LINE :
				if (m_pParent->m_nLngSel==0) str = _T(" 부호가 있는 라인과 라인의 거리 ");
				else if (m_pParent->m_nLngSel==1) str = _T(" DIST with PLUS, MINUS (LINE - LINE) ");
				else str = _T(" 计算有符号距离值 ");
				break;
		}

		GetDlgItem(IDC_STATIC_LINK_TYPE)->SetWindowText(str);
	}
	else
	{
		if (m_pParent->m_nLngSel==0) strMessage = _T(" 검사형태가 일치하지 않습니다.  ");
		else if (m_pParent->m_nLngSel==1) strMessage = _T("  Inspection type is mismatch ! ");
		else strMessage = _T("  检查TYPE不一致  ");

		MessageBox(strMessage, _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
	}

   	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK_INSERT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK_INSERT2)->EnableWindow(FALSE);

	UpdateData(false);
}

/*
void CDlgTeachSetup::LinkRelationObject(long nLinkType)
{
	long  nPos, nSel, nPos1, nPos2;
	CString strTitle;

	UpdateData(true);
	if (m_staticLinkItem1==_T("") || m_staticLinkItem2==_T("")) return;

	strTitle = m_staticLinkItem1 + _T("+") + m_staticLinkItem2;
	nPos = m_listLinkItem.FindString(0, strTitle);
	if (nPos<0)
	{
		nSel = m_listLinkItem.GetCount();
		nPos = m_pData->m_Teach.m_nLinkNum;

		m_listLinkItem.AddString(strTitle);
		_tcscpy_s(m_pData->m_Teach.m_LINK[nSel].m_strTitle, strTitle);

		nPos1 = m_pParent->m_pModelData->GetIndexROI(m_staticLinkItem1);
		nPos2 = m_pParent->m_pModelData->GetIndexROI(m_staticLinkItem2);
		if (nPos1>=0 && nPos2>=0)
		{
			m_pData->m_Teach.m_LINK[nPos].m_bSel1 = TRUE;
			m_pData->m_Teach.m_LINK[nPos].m_nIndex1 = nPos1;

			m_pData->m_Teach.m_LINK[nPos].m_bSel2 = TRUE;
			m_pData->m_Teach.m_LINK[nPos].m_nIndex2 = nPos2;

			m_pData->m_Teach.m_LINK[nPos].m_nLinkType = nLinkType;
		    m_pData->m_Teach.m_nLinkNum = nSel+1;
		}
	}

	m_staticLinkItem1 = _T("");
	m_staticLinkItem2 = _T("");
   	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);

	UpdateData(false);
}
*/

void CDlgTeachSetup::OnBtnDelete() 
{
	CString strMessage;
	long i, nPos;

	if (m_pParent->m_nLngSel==0) strMessage = _T("  선택한 검사항목을 제거하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Are you sure you want to delete the selected data ?  ");
	else strMessage = _T("  要删除选择的参数吗 ?  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		nPos = m_listLocItem.GetCurSel();
		if (nPos>=0)
		{
			for(i=nPos; i<LOCATE_MAX_NUM-1; i++)
				memcpy(&m_pData->m_Teach.m_LOC[i], &m_pData->m_Teach.m_LOC[i+1], sizeof(TEACH_ATTR));

			m_listLocItem.DeleteString(nPos);
			m_pData->m_Teach.m_nLocNum = m_listLocItem.GetCount();

            m_pParent->m_pModelData->CopyModelData(m_pData, m_pBackData);
		}

		nPos = m_listLinkItem.GetCurSel();
		if (nPos>=0)
		{
			for(i=nPos; i<LINK_MAX_NUM-1; i++)
				memcpy(&m_pData->m_Teach.m_LINK[i], &m_pData->m_Teach.m_LINK[i+1], sizeof(TEACH_ATTR));

			m_listLinkItem.DeleteString(nPos);
			m_pData->m_Teach.m_nLinkNum = m_listLinkItem.GetCount();

            m_pParent->m_pModelData->CopyModelData(m_pData, m_pBackData);
		}
	}

	UpdateData(false);
}

void CDlgTeachSetup::OnBnClickedBtnSave()
{
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T("  변경된 데이타를 저장하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want save the changed data ?  ");
	else strMessage = _T("  要保存变更的参数吗 ?  ");

	if (MessageBox(strMessage, _T(" Message Box  "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		memcpy(&m_pBackData->m_IMG, &m_pData->m_IMG, sizeof(IMAGE_DATA));

		m_pParent->m_pModelData->CopyModelData(m_pBackData, m_pData);
		m_pParent->m_pModelData->SaveModelData(m_pParent->m_strFullPathName);
		m_pParent->SetEventFuncCall(DEF_CONT_SAVE_SETUP);
		m_pParent->Invalidate(false);
	}
}

void CDlgTeachSetup::OnCheckNone() 
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_NONE)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_NONE;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckDistPnt2Pnt()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_DIST_PNT2PNT)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_DIST_PNT2PNT;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckDistLine2Line()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_DIST_LINE2LINE)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_DIST_LINE2LINE;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckDistPnt2Line()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_DIST_PNT2LINE)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_DIST_PNT2LINE;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckAngPnt2Pnt()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_ANG_PNT2PNT)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_ANG_PNT2PNT;

	    ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckAngLine2Line()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_ANG_LINE2LINE)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_ANG_LINE2LINE;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnCheckDiffPnt2Pnt()
{
	CString strMessage;
	long nPos=m_listLinkItem.GetCurSel();

	if (nPos>=0 && m_nLinkType!=TEACH_DIFF_PNT2PNT)
	{
		strMessage = _T(" 측정 형태를 변경하시겠습니까 ? ");
		if (MessageBox(strMessage, _T(" 메세지 화면 "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
			m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType = TEACH_DIFF_PNT2PNT;

		ToggleButtonControls(m_nLinkType);
	}
}

void CDlgTeachSetup::OnSelchangeListItemLink() 
{
	CString str;
	long nPos, nSel1, nSel2;

	nPos = m_listLinkItem.GetCurSel();	
	if (nPos>=0)
	{
		m_nLinkType = m_pData->m_Teach.m_LINK[nPos].m_nLinkType;
        ToggleButtonControls(m_nLinkType);

		nSel1 = m_pData->m_Teach.m_LINK[nPos].m_nIndex1;
		nSel2 = m_pData->m_Teach.m_LINK[nPos].m_nIndex2;
		if (nSel1>=0) m_staticLinkItem1.Format(_T("%s"), m_pData->m_GOM[nSel1].m_strTitle);
		if (nSel2>=0) m_staticLinkItem2.Format(_T("%s"), m_pData->m_GOM[nSel2].m_strTitle);

//		m_strLowLimit.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dStdSpec);
//		m_strHighLimit.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dToleSpec);
		m_strLowLimit.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dStandLowLimit);
		m_strHighLimit.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dStandHighLimit);
		m_strOffsetV.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dOffsetV);
		m_strValueA.Format(_T("%5.2f"), m_pData->m_Teach.m_LINK[nPos].m_dValueA);

		switch(m_nLinkType)
		{
			case TEACH_DIST_LINE2LINE :
			    str = _T(" DIST. EDGE ");
				break;
			case 	TEACH_DIST_1STLINE2LINE:
			    str = _T(" 1st EDGE to EDGE   ");
				break;
			case TEACH_DIST_2STLINE2LINE :
			    str = _T(" 2nd EDGE to EDGE ");
				break;
			case TEACH_NADIST_LINE2LINE :
				str = _T(" Not Abs DIST EDGE to EDGE ");
				break;
		}

		GetDlgItem(IDC_STATIC_LINK_TYPE)->SetWindowText(str);
	}

	UpdateData(false);
}

void CDlgTeachSetup::ToggleButtonControls(long nType) 
{
	((CButton *)GetDlgItem(IDC_CHECK_NONE))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_DIST_PNT2LINE))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_DIST_LINE2LINE))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_ANG_PNT2PNT))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_ANG_LINE4PNT))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_CHECK_ANG_LINE2LINE))->SetCheck(false);

	switch(nType)
	{
		case TEACH_NONE :
	        ((CButton *)GetDlgItem(IDC_CHECK_NONE))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 검사 형태가 지정되지 않았습니다. "));
			break;
		case TEACH_DIST_PNT2LINE :
	        ((CButton *)GetDlgItem(IDC_CHECK_DIST_PNT2LINE))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 항목 중심과 직선의 수직 교차점간 거리를 측정합니다. "));
			break;
		case TEACH_DIST_LINE2LINE :
	        ((CButton *)GetDlgItem(IDC_CHECK_DIST_LINE2LINE))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 두 항목의 직선간 평균 거리를 측정합니다. "));
			break;
		case TEACH_ANG_PNT2PNT :
	        ((CButton *)GetDlgItem(IDC_CHECK_ANG_PNT2PNT))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 두 항목의 중심간 각도를 측정합니다. "));
			break;
		case TEACH_ANG_LINE4PNT :
	        ((CButton *)GetDlgItem(IDC_CHECK_ANG_LINE4PNT))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 4점의 2라인 사이각을 측정합니다. "));
			break;
		case TEACH_ANG_LINE2LINE :
	        ((CButton *)GetDlgItem(IDC_CHECK_ANG_LINE2LINE))->SetCheck(true);
			GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T(" 두 항목의 직선 사이각을 측정합니다. "));
			break;
    }
}


void CDlgTeachSetup::OnLButtonDown(UINT nFlags, CPoint point)
{
	WINDOWPLACEMENT wp;
	RECT lpRect;

	GetDlgItem(IDC_BUTTON_LEFT)->GetWindowPlacement(&wp);
	lpRect = wp.rcNormalPosition;
	if (lpRect.left<point.x && point.x<lpRect.right &&
		lpRect.top<point.y && point.y<lpRect.bottom)
		SetTimer(1000, 100, NULL);

	GetDlgItem(IDC_BUTTON_RIGHT)->GetWindowPlacement(&wp);
	lpRect = wp.rcNormalPosition;
	if (lpRect.left<point.x && point.x<lpRect.right &&
		lpRect.top<point.y && point.y<lpRect.bottom)
		SetTimer(1001, 100, NULL);

	GetDlgItem(IDC_BUTTON_UP)->GetWindowPlacement(&wp);
	lpRect = wp.rcNormalPosition;
	if (lpRect.left<point.x && point.x<lpRect.right &&
		lpRect.top<point.y && point.y<lpRect.bottom)
		SetTimer(1002, 100, NULL);

	GetDlgItem(IDC_BUTTON_DN)->GetWindowPlacement(&wp);
	lpRect = wp.rcNormalPosition;
	if (lpRect.left<point.x && point.x<lpRect.right &&
		lpRect.top<point.y && point.y<lpRect.bottom)
		SetTimer(1003, 100, NULL);

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTeachSetup::OnLButtonUp(UINT nFlags, CPoint point)
{
	KillTimer(1000);
	KillTimer(1001);
	KillTimer(1002);
	KillTimer(1003);

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgTeachSetup::OnBnClickedRadioFirstRoi()
{
	m_bRearOption = FALSE;
}

void CDlgTeachSetup::OnBnClickedRadioSecond()
{
	m_bRearOption = TRUE;
}

void CDlgTeachSetup::OnBnClickedButtonLeft()
{
	KillTimer(1000);
}

void CDlgTeachSetup::OnBnClickedButtonRight()
{
	KillTimer(1001);
}

void CDlgTeachSetup::OnBnClickedButtonUp()
{
	KillTimer(1002);
}

void CDlgTeachSetup::OnBnClickedButtonDn()
{
	KillTimer(1003);
}

void CDlgTeachSetup::OnTimer(UINT_PTR nIDEvent)
{
	m_pParent->m_pModelData->SetMoveROI(nIDEvent-1000, m_bRearOption);
	m_pParent->Invalidate(false);

	CDialog::OnTimer(nIDEvent);
}

void CDlgTeachSetup::OnBnClickedCheckDispRoi()
{
	
}


void CDlgTeachSetup::OnStnClickedButtonLeft()
{
	// TODO: Add your control notification handler code here
}


void CDlgTeachSetup::OnBnClickedBtnCompens()
{
	BOOL bRet1, bRet2;
	float dPx, dPy, dPt, dX, dY, dT;
	CString strMessage;

	if (m_pParent->m_nLngSel==0) strMessage = _T("  위치 보정을 적용하시겠습니까 ?  ");
	else if (m_pParent->m_nLngSel==1) strMessage = _T("  Do you want compensate Location ROI Data ?  ");
	else strMessage = _T("  位置补偿是否要使用？  ");

	if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		bRet1 = m_pParent->m_pModuleLink->CompensatePosition(0, &dPx, &dPy, &dPt, &dX, &dY, &dT);
		if (!bRet1)
		{
			m_pData->m_dSourX = dPx;
			m_pData->m_dSourY = dPy;
			m_pData->m_dSourT = dPt;

			m_pData->m_dDeltaX = 0;
			m_pData->m_dDeltaY = 0;
			m_pData->m_dDeltaT = 0;
		}

		bRet2 = m_pParent->m_pModuleLink->CompensatePosition(1, &dPx, &dPy, &dPt, &dX, &dY, &dT);
		if (!bRet2)
		{
			m_pData->m_dSourX2 = dPx;
			m_pData->m_dSourY2 = dPy;
			m_pData->m_dSourT2 = dPt;

			m_pData->m_dDeltaX2 = 0;
			m_pData->m_dDeltaY2 = 0;
			m_pData->m_dDeltaT2 = 0;
		}

		m_pParent->Invalidate(false);

		if (!bRet1 || !bRet2)
		{
			if (m_pParent->m_nLngSel==0) strMessage = _T("  위치 보정을 성공적으로 마쳤습니다. ");
			else if (m_pParent->m_nLngSel==1) strMessage = _T("  Success Location Compensation  ! ");
			else strMessage = _T("  位置补偿完成  ");

			MessageBox(strMessage , _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
		}
		else
			MessageBox(_T("  Failure Location Compensation !  ") , _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
	}

}


