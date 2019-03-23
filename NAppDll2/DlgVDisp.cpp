// DlgVDisp.cpp : implementation file
//

#include "stdafx.h"
#include "nappdll.h"
#include "DlgVDisp.h"
#include "NPioControlDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVDisp dialog
CDlgVDisp::CDlgVDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVDisp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVDisp)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pParent->GetClientRect(&m_rcBound);

	m_pModelData = new CModelData();
	m_pData = m_pModelData->GetModelDataPtr();
	m_pBackData = m_pModelData->GetModelBackDataPtr();

	m_pModuleLink = new CModuleLink();
    m_pModuleLink->SetModelData(m_pData);
	m_pModuleLink->SetModelBackData(m_pBackData);
	m_pModuleLink->InitializeInspData();

	m_pModuleLink->m_pCAL = &m_pCAL;
	m_pModuleLink->m_MutualLink.m_pCAL = &m_pCAL;

	m_pImageModule = new CImageModule();
	m_pImageModule->SetModelData(m_pData);

	m_memDC.m_hDC = NULL;
	m_pDlgVisionSetup = NULL;
	m_strFullPathName = "";
	m_strSaveFileName = "";
	m_nLngSel = 2;

	m_bModifySetup = FALSE;
    m_bLeftBtnDown = FALSE;
    m_bRightBtnDown = FALSE;
	m_bDLeftBtnDown = FALSE;
	m_bSaveImage = FALSE;

	m_ptLStartPnt.x = 0.0f;
	m_ptLStartPnt.y = 0.0f;

	m_ptRStartPnt = CPoint(0, 0);
	m_ptREndPnt = CPoint(0, 0);
	m_rcRect = CRect(0, 0, 0, 0);
}

CDlgVDisp::~CDlgVDisp()
{
	delete m_pModelData;
	delete m_pModuleLink;
	delete m_pImageModule;

	m_memDC.DeleteDC();
	m_bmpBuffer.DeleteObject();
	if (m_pDlgVisionSetup)
	{
		delete m_pDlgVisionSetup;
		m_pDlgVisionSetup=NULL;
	}
}

void CDlgVDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVDisp)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgVDisp, CDialog)
	//{{AFX_MSG_MAP(CDlgVDisp)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_LOAD_IMAGE_DATA, OnLoadImageData)
	ON_COMMAND(IDM_SAVE_IMAGE_DATA, OnSaveImageData)
	ON_COMMAND(IDM_ADD_RECT_ROI, OnAddRectRoi)
	ON_COMMAND(IDM_REMOVE_ROI, OnRemoveRoi)
	ON_COMMAND(IDM_SHOW_SETUP, OnShowSetup)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CDlgVDisp::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowPos(NULL, m_rcBound.left, m_rcBound.top, m_rcBound.Width(), m_rcBound.Height(), SWP_SHOWWINDOW);

	m_memDC.CreateCompatibleDC(NULL);
	m_bmpBuffer.CreateCompatibleBitmap(m_pParent->GetDC(), m_rcBound.Width(), m_rcBound.Height()); 
	(CBitmap*)m_memDC.SelectObject(&m_bmpBuffer); 

#if 0
	EnableScrollBar(SB_BOTH);
	ShowScrollBar(SB_BOTH);
	UpdateScrollPos();
	SetScrollPos(SB_HORZ, 0);
	SetScrollPos(SB_VERT, 0);  
#endif // 0


	InvalidateView();
    //SetClassLong(this->m_hWnd, GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(IDC_CURSOR));  //shosng	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/////////////////////////////////////////////////////////////////////////////
////////////////////    영상 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CDlgVDisp::SetRawImagePtr(long pImage)
{
	memcpy(m_pImageModule->m_pRawImage, (LPBYTE)pImage, m_pData->m_nImageWidth*m_pData->m_nImageHeight);
//	UpdateLUT((long)m_pImageModule->m_pRawImage);
}

void CDlgVDisp::LoadImageData(LPCTSTR strFileName)
{
	BOOL bRet;
	long nHeight, nWidth;

	bRet = m_pImageModule->OpenImageFile(strFileName);
	if (bRet && m_pImageModule->GetCurrImageColor()==IMAGE_DEPTH) 
	{
	    nHeight = m_pImageModule->GetCurrImageHeight();
	    nWidth = m_pImageModule->GetCurrImageWidth();

		if (nHeight==m_pData->m_nImageHeight && nWidth==m_pData->m_nImageWidth)
		{
			m_pImageModule->InitImageMemory(nWidth, nHeight);
			m_pImageModule->GetCurrentImage();

//			m_pImageModule->DisplayImageToFrame(m_pImageModule->m_pRawImage);
		}
		else
		{
			SetImageSize(m_pData->m_nImageWidth, m_pData->m_nImageHeight);
			MessageBox(_T(" The image is inadequate in size "), _T(" Message Box "), MB_OK|MB_ICONEXCLAMATION);
		}
	}

//  InvalidateView();
}

void CDlgVDisp::SetImageSize(long nWidth, long nHeight)
{
	m_pData->m_nImageWidth = nWidth;
	m_pData->m_nImageHeight = nHeight;

    m_pImageModule->SetImageSize(nWidth, nHeight);
	m_pModuleLink->SetImageRawData(m_pImageModule->m_pRawImage, nWidth, nHeight);
//  m_pModuleLink->SetImageProcData(m_pImageModule->m_pDispImage, nWidth, nHeight);
}

void CDlgVDisp::SetFrameSize(long nWidth, long nHeight, long nBand, long nDepth)
{
	m_pImageModule->SetFrameSize(nWidth, nHeight, nBand, nDepth);
	m_pData->m_IMG.m_ptMouse.x = 0; 
	m_pData->m_IMG.m_ptMouse.y = 0;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    영상 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////    모델 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CDlgVDisp::SetCameraNum(long nCamID)
{
	m_nCamID = nCamID; 
	//m_pImageModule->m_nCamID = nCamID; // shsong 만약 이부문이 문제가 되면 코멘트 풀자. 14호기는 코멘트됨.
	m_pModuleLink->m_nCamID = nCamID;

	wsprintf(m_pData->m_strTitle, _T("CAM%d"), nCamID);
	wsprintf(m_pBackData->m_strTitle, _T("CAM%d"), nCamID);

	LoadCalFromFile();
}

void CDlgVDisp::CreateModelData(LPCTSTR strFileName)
{
	m_strFullPathName = strFileName;
	m_pModelData->CreateModelData();
}

BOOL CDlgVDisp::LoadModelData(LPCTSTR strFileName)
{
	BOOL bRet;

	m_strFullPathName = strFileName;
	bRet = m_pModelData->LoadModelData(strFileName);
	return bRet;
}

void CDlgVDisp::SaveModelData(LPCTSTR strFileName)
{
//	m_pModelData->CopyModelData(m_pData, m_pBackData);
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    모델 데이타 입출력 메서드    ////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
////////////////////    티칭 화면 정보 관련 메서드   ////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CDlgVDisp::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bModifySetup) return;

    m_bLeftBtnDown = TRUE;
	m_ptLStartPnt.x = (float)point.x;
	m_ptLStartPnt.y = (float)point.y;

	if (m_pDlgVisionSetup != NULL)
	{
       	m_pImageModule->SetSelectedROI(m_ptLStartPnt);
//		m_pModelData->SetExecuteROI(FALSE);
		m_pDlgVisionSetup->LButtonDown();
	   	Invalidate(false);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgVDisp::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_bLeftBtnDown = FALSE;	
	SetFocus();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgVDisp::OnRButtonDown(UINT nFlags, CPoint point) 
{
    m_bRightBtnDown = TRUE;
	m_ptRStartPnt.x = point.x;
	m_ptRStartPnt.y = point.y;
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CDlgVDisp::OnRButtonUp(UINT nFlags, CPoint point) 
{
    m_bRightBtnDown = FALSE;
	SetFocus();

	CDialog::OnRButtonUp(nFlags, point);
}

void CDlgVDisp::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CMenu menu;
	CRect rc;

	if (m_pData->m_bRealTimeMode) return;
	this->GetWindowRect(&rc);

	if (menu.LoadMenu(IDR_MENU_POPUP))
	{
		CMenu* pSub = menu.GetSubMenu(0);
		if (pSub)
			pSub->TrackPopupMenu(TPM_LEFTALIGN, rc.left+point.x, rc.top+point.y, this);
	}
	
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CDlgVDisp::OnMouseMove(UINT nFlags, CPoint point) 
{
	long nPosX, nPosY, nLevel;
	float dScale;

   	if (point.x>=m_rcBound.left && point.y>=m_rcBound.top  &&
		point.x<m_rcBound.right && point.y<m_rcBound.bottom)
	{
		// 해당 좌표의 Gray Level를 읽어 이벤트 메세지를 보냄
		m_pImageModule->WriteGrayLevelPos(point.x, point.y, &nPosX, &nPosY, &nLevel);
//		FireOnMouseMove(nPosX, nPosY, nLevel);
    	// 해당 좌표의 Gray Level를 읽어 이벤트 메세지를 보냄

		// 해당 좌표의 ROI OBJECT를 움직임
		if (m_bLeftBtnDown)
		{
			m_ptLEndPnt.x = (float)point.x;
			m_ptLEndPnt.y = (float)point.y;

			m_ptLDelta.x = (float)(m_ptLEndPnt.x - m_ptLStartPnt.x);
			m_ptLDelta.y = (float)(m_ptLEndPnt.y - m_ptLStartPnt.y);

			m_pImageModule->SetMoveROI(m_ptLStartPnt, m_ptLDelta);
			m_ptLStartPnt = m_ptLEndPnt;
		}

		if (nFlags & MK_RBUTTON)
		{
			dScale = m_pData->m_IMG.m_fZoomScale;
           // if (!m_pData->m_bRealTimeMode) 
			{
				m_pData->m_IMG.m_ptMouse.x += (long)((m_ptRStartPnt.x - point.x)/dScale);
        		m_pData->m_IMG.m_ptMouse.y += (long)((m_ptRStartPnt.y - point.y)/dScale);
			}

			m_ptRStartPnt = point;
			m_pImageModule->DisplayImageToFrame(m_pImageModule->m_pRawImage);
		}
    	// 해당 좌표의 ROI OBJECT를 움직임

		Invalidate(false);
	}

	if (!m_pData->m_bRealTimeMode)
	{
		if (m_pDlgVisionSetup != NULL) 
			m_pDlgVisionSetup->MouseMove(nFlags , point);
	}	
	
	CDialog::OnMouseMove(nFlags, point);
}

#if 0
void CDlgVDisp::UpdateScrollPos()
{
	long nWidth, nHeight;
	long nScaleW=1, nScaleH=1;

	m_pImageModule->GetImageSize(&nWidth, &nHeight);

	SetScrollRange(SB_HORZ, 0, (int)((nWidth-FRAME_WIDTH) * m_pData->m_IMG.m_fZoomScale), TRUE);
	SetScrollRange(SB_VERT, 0, (int)((nHeight-FRAME_HEIGHT) * m_pData->m_IMG.m_fZoomScale), TRUE);
}

void CDlgVDisp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    long nScrollPos, nTemp;
	float dScale;

	switch(nSBCode)
	{
		case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우
     			SetScrollPos(SB_HORZ,(int)(GetScrollPos(SB_HORZ)-10)); break;
		case SB_LINERIGHT: // 오른쪽 끝을 누른경우
     			SetScrollPos(SB_HORZ,(int)(GetScrollPos(SB_HORZ)+10)); break;
		case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
     			SetScrollPos(SB_HORZ,GetScrollPos(SB_HORZ)-FRAME_HEIGHT); break;
		case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
     			SetScrollPos(SB_HORZ,GetScrollPos(SB_HORZ)+FRAME_HEIGHT); break;
		case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
     			SetScrollPos(SB_HORZ, nPos); break;
	}

	nScrollPos = GetScrollPos(SB_HORZ);

	dScale = m_pData->m_IMG.m_fZoomScale;
	nTemp = (long)(nScrollPos - (m_pData->m_nImageWidth/2 - FRAME_WIDTH/2) * dScale);

    if (!m_pData->m_bRealTimeMode) 
		m_pData->m_IMG.m_ptMouse.x = (long)(nTemp/dScale);

	InvalidateView();
}

void CDlgVDisp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	long nScrollPos, nTemp;
	float dScale;

	switch(nSBCode)
	{
		case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)-10)); break;
		case SB_LINERIGHT: // 오른쪽 끝을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)+10)); break;
		case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)-FRAME_WIDTH); break;
		case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)+FRAME_WIDTH); break;
		case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
     			SetScrollPos(SB_VERT, nPos); break;
	}

	nScrollPos = GetScrollPos(SB_VERT);

	dScale = m_pData->m_IMG.m_fZoomScale;
	nTemp = (long)(nScrollPos - (m_pData->m_nImageHeight/2 - FRAME_HEIGHT/2) * dScale);

    if (!m_pData->m_bRealTimeMode) 
		m_pData->m_IMG.m_ptMouse.y = (long)(nTemp/dScale);

	InvalidateView();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}  
#endif // 0

/////////////////////////////////////////////////////////////////////////////
////////////////////       화면 조작 관련 메서드     ////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CDlgVDisp::OnLoadImageData() 
{
	static TCHAR BASED_CODE szFilter[] = _T("Image Files (*.bmp)|*.bmp|");
	CFileDialog dlg(true, _T("*.bmp"), NULL , OFN_HIDEREADONLY, szFilter, NULL);
	if (dlg.DoModal() ==IDOK)
    	LoadImageData(dlg.GetPathName());
}

void CDlgVDisp::OnSaveImageData() 
{
	static TCHAR BASED_CODE szFilter[] = _T("Image Files (*.bmp)|*.bmp|");
	CFileDialog dlg(false, _T("*.bmp"), NULL , OFN_HIDEREADONLY, szFilter, NULL);
	if (dlg.DoModal() ==IDOK)
		m_pImageModule->SaveImageFile(dlg.GetPathName());
}

void CDlgVDisp::OnAddRectRoi() 
{
	long nIndex;
	CString str;
 
	if (m_bModifySetup)
	{
		nIndex = m_pModelData->GetNextROI();
		str.Format(_T("%d"), nIndex);
		m_pModelData->AddROI(str, GOM_RECT, INSP_MEASURE_SIDE_ONE, 1, RGB(0, 255, 0));
	}

	Invalidate(false);
}

void CDlgVDisp::OnRemoveRoi() 
{
	if(!m_bModifySetup) return;

	long nSelectedROI = m_pModelData->GetIndexSelectedROI();
	if (nSelectedROI >= 0)
	{
		CString strMessage = _T(" Do you want delete the selected item ?  ");
		if (MessageBox(strMessage, _T(" Message Box "), MB_YESNO|MB_ICONQUESTION) == IDYES) 
		{
			m_pModelData->DeleteSelectedROI();
			m_pModelData->CopyModelData(m_pData, m_pBackData);
//			m_pModelData->CopySelectedROI(nSelectedROI, m_pData, m_pBackData);
		}
	}

	Invalidate(false);		
}

void CDlgVDisp::OnShowSetup() 
{
	ShowSetupDialog();	
}

void CDlgVDisp::ShowSetupDialog()
{
	m_bModifySetup = TRUE;

	if (m_pDlgVisionSetup != NULL) 
	{
		if (m_pDlgVisionSetup->IsWindowVisible()) 
			return;
		else
		{
			delete m_pDlgVisionSetup;
            m_pDlgVisionSetup = NULL;
		}
	}

	m_pDlgVisionSetup = new CDlgVisionSetup(this);
	m_pDlgVisionSetup->Create(IDD_DLG_VISION_SETUP, NULL);
	m_pDlgVisionSetup->ShowWindow(SW_SHOW);
}

void CDlgVDisp::SetEventFuncCall(long nContNo)
{
	long i;

	m_pCallData->m_nCamID = m_nCamID;
	m_pCallData->m_bContNo = nContNo;

	switch (nContNo)
	{
	    case DEF_CONT_EXIT_SETUP :
			break;
		case DEF_CONT_LIGHT :
			m_pCallData->m_nExposureTime = m_pSYS.m_nExposureTime;
			for(i=0; i<DEF_LIGHT_NUM; i++)
				m_pCallData->m_LightV[i] = m_pSYS.m_LightV[i];
			break;
	}

	(*m_lpCallBack)();
}
/////////////////////////////////////////////////////////////////////////////
////////////////////    티칭 화면 정보 관련 메서드   ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////      화면 변경 및 영상개선      ////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CDlgVDisp::InvalidateView()
{
//	UpdateLUT((long)m_pImageModule->m_pRawImage);
	if (!m_pImageModule->m_bAllocFrame)
	{
		m_pImageModule->DisplayImageToFrame(m_pImageModule->m_pRawImage);
		Invalidate(false);
	}
}

/*
void CDlgVDisp::UpdateLUT(long pImagePtr) 
{
	long i, j;
	BOOL bEnhence;
	LPBYTE pImage = (LPBYTE)pImagePtr;

	if (pImage != NULL)
	{
		bEnhence = m_pData->m_Enhence.m_bUseEnhence;
		if (bEnhence)
		{
			for(i=0; i<m_pData->m_nImageHeight; i++)
			for(j=0; j<m_pData->m_nImageWidth; j++)
				m_pImageModule->m_pDispImage[i * m_pData->m_nImageWidth + j] = (unsigned char)m_pData->m_Enhence.m_nLUT[pImage[i * m_pData->m_nImageWidth + j]];
		}
		else
			memcpy(m_pImageModule->m_pDispImage, pImage, m_pData->m_nImageHeight*m_pData->m_nImageWidth);
	}
}
*/

void CDlgVDisp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_memDC.m_hDC && m_pImageModule->m_dispDib && !m_pImageModule->m_bAllocFrame)
	{
		//{BEGIN : 화면에 영상그리기 >>>>>>>>>>>>>>>>>>>>>>>>
		CSize sizeFileDib = m_pImageModule->m_dispDib->GetDimensions();
		m_pImageModule->m_dispDib->Draw(&m_memDC, CPoint(0,0), sizeFileDib);
		//}END : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		m_pImageModule->InvalidateView(&m_memDC);
	}

	dc.BitBlt(0, 0, m_rcBound.Width(), m_rcBound.Height(), &m_memDC, 0, 0,SRCCOPY); 	

	//SetFocus();
	
	// Do not call CDialog::OnPaint() for painting messages
}
/////////////////////////////////////////////////////////////////////////////
////////////////////      화면 변경 및 영상개선      ////////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote 조명 제어 함수     //////////////////////////
////////////////////////////////////////////////////////////////////////////
void CDlgVDisp::SetRemoteLight(BOOL bLight) 
{
	/*
	long nLightValue;

	if (bLight)	nLightValue = m_pData->m_Enhence.m_nLight;
	else nLightValue = 0;

	if (nLightValue<0) nLightValue=0;
	if (nLightValue>255) nLightValue=255;

	NPioControlDll_SetLight(m_nCamID, nLightValue);
	*/
}

long CDlgVDisp::GetBrightnessValue(long nCh)
{
	long nLightValue=0;
	/*
	nLightValue = m_pData->m_Enhence.m_nLight;

	if (nLightValue<0) nLightValue=0;
	if (nLightValue>255) nLightValue=255;
	*/
	return nLightValue;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////       Remote 조명 제어 함수     ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////        캘리브레이션 처리        ////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL CDlgVDisp::LoadCalFromFile()
{
	long i;
	CFile f;
	CString filename;

	filename = _T("");
	if (m_nCamID==0) filename.Format(_T("%s"), CAL_UP_FILE_PATH);
	else             filename.Format(_T("%s"), CAL_DN_FILE_PATH);

	if (f.Open(filename, CFile::modeRead)) 
	{
		f.Read(&m_pCAL, sizeof(CAL_DATA));
		f.Close();
	}
	else
	{
		m_pCAL.m_dScaleLT_X = 1.0f;
		m_pCAL.m_dScaleLB_X = 1.0f;
		m_pCAL.m_dScaleRT_X = 1.0f;
		m_pCAL.m_dScaleRB_X = 1.0f;
		m_pCAL.m_dScaleL_Y = 1.0f;
		m_pCAL.m_dScaleR_Y = 1.0f;

		m_pCAL.m_dRealT_X = 95.9f;
		m_pCAL.m_dRealB_X = 95.9f;
		m_pCAL.m_dReal_Y = 144.0f;
	}

	filename = _T("");
	if (m_nCamID==0) 
		filename.Format(_T("%s"), SYS_UP_FILE_PATH);
	else             
		filename.Format(_T("%s"), SYS_DN_FILE_PATH);

	if (f.Open(filename, CFile::modeRead)) 
	{
		f.Read(&m_pSYS, sizeof(SYSTEM_DATA));
		f.Close();
	}
	else
	{
		m_pSYS.m_nExposureTime = 150;
		for(i=0; i<DEF_LIGHT_NUM; i++)
			m_pSYS.m_LightV[i] = 0;
	}
		
	return TRUE;
}

BOOL CDlgVDisp::SaveCalToFile()
{
	CFile f;
	CString filename;

	filename = _T("");
	if (m_nCamID==0) filename.Format(_T("%s"), CAL_UP_FILE_PATH);
	else             filename.Format(_T("%s"), CAL_DN_FILE_PATH);

	if (f.Open(filename, CFile::modeCreate|CFile::modeWrite)) 
	{
		f.Write(&m_pCAL, sizeof(CAL_DATA));
		f.Close();
	}

	filename = _T("");
	if (m_nCamID==0) filename.Format(_T("%s"), SYS_UP_FILE_PATH);
	else             filename.Format(_T("%s"), SYS_DN_FILE_PATH);

	if (f.Open(filename, CFile::modeCreate|CFile::modeWrite))
	{
		f.Write(&m_pSYS, sizeof(SYSTEM_DATA));
		f.Close();
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////        캘리브레이션 처리        ////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
////////////////////         OFFSET DATA 처리         ////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL CDlgVDisp::LoadOffsetFile(CString strFileName)
{
	long i, nCnt;
	FILE *fp;
	TCHAR *pCh, szbuf[128], szFileName[128];
	CString strT, strL;

	m_pModuleLink->m_nItemMaxNum = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pModuleLink->m_fSpecV[i] = 0.0f;
		m_pModuleLink->m_fOffsetV[i] = 0.0f;
		m_pModuleLink->m_strItemTitle[i].Format(_T("%s"), _T(""));
	}

	strL = _T("");
	for(i=strFileName.GetLength()-1; i>0; i--)
	{
		strT = strFileName.Mid(i, 1);
		if (strT == _T("\\"))
		{
			strL = strFileName.Left(i);
			break;
		}
	}

	wsprintf(szFileName, _T("%s\\ConfigData.txt"), strL);
	_tfopen_s(&fp, szFileName, _T("r"));
	if (fp)
	{
		nCnt = 0;
		for(i=0; i<GOM_MAX_NUM; i++)
		{
			pCh = _fgetts(szbuf, 100, fp);
			if (pCh==NULL) 
				break;

			strL.Format(_T("%s"), szbuf);
			strT = WithoutSpaceToString(strL.Mid(0, 10));
			if (_tcscmp(strT, _T(""))) 
			{
				m_pModuleLink->m_strItemTitle[nCnt] = strT;

				strT = WithoutSpaceToString(strL.Mid(10, 10));
				m_pModuleLink->m_fSpecV[nCnt] = (float)_ttof(strT);

				strT = WithoutSpaceToString(strL.Mid(20, 10));
				m_pModuleLink->m_fOffsetV[nCnt] = (float)_ttof(strT);

				nCnt++;
			}
		}

		m_pModuleLink->m_nItemMaxNum = nCnt;
		fclose(fp);
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////         OFFSET DATA 처리         ////////////////////////
/////////////////////////////////////////////////////////////////////////////


BOOL CDlgVDisp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_pData->m_IMG.m_fZoomScale <= DEF_MINI_ZOOM_SCALE)
		return FALSE;

	if(zDelta > 0) // wheel down
	{		 
		m_pData->m_IMG.m_fZoomScale += 0.05f;
	}
	else 
	{
		m_pData->m_IMG.m_fZoomScale -= 0.05f;
	}

	if (m_pData->m_IMG.m_fZoomScale < DEF_INIT_ZOOM_SCALE) 
		m_pData->m_IMG.m_fZoomScale = DEF_INIT_ZOOM_SCALE;	

	if (m_pData->m_IMG.m_fZoomScale > 1.4f) 
		m_pData->m_IMG.m_fZoomScale = 1.4f;

	InvalidateView();

//	m_pMainFrm->m_fZoomScale = fScale;
//	m_pMainFrm->UpdateBadImage();

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}  


