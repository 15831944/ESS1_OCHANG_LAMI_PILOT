// DlgBadImage.cpp : implementation file
//

#include "stdafx.h"
#include "NLaminateApp.h"
#include "DlgBadImage.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgBadImage dialog
extern CMainFrame *m_pMainFrm;

IMPLEMENT_DYNAMIC(CDlgBadImage, CDialogEx)

CDlgBadImage::CDlgBadImage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBadImage::IDD, pParent)
{
	long i;

	for(i=0; i<GOM_MAX_NUM; i++)
		 wcscpy_s(m_pSelRes.m_strTitle[i], _T(""));

	m_bSaveStart = FALSE;
	m_strImageFileName = _T("");	
}

CDlgBadImage::~CDlgBadImage()
{
	delete m_dispDib;

	m_memDC.DeleteDC();
	m_bmpBuffer.DeleteObject();
}

void CDlgBadImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgBadImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CDlgBadImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_dispDib = new CDib(SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, 8);
	m_memDC.CreateCompatibleDC(NULL);
	m_bmpBuffer.CreateCompatibleBitmap(GetDC(), SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT); 
	(CBitmap*)m_memDC.SelectObject(&m_bmpBuffer); 

	SetWindowPos(NULL, 0, 0, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgBadImage::OnPaint()
{
	CPaintDC dc(this); 

	InvalidateView();
}

void CDlgBadImage::InvalidateView()
{
	CDC *pDC = GetDC();

	if (m_memDC.m_hDC && m_dispDib)
	{
		//{BEGIN : 화면에 영상그리기 >>>>>>>>>>>>>>>>>>>>>>>>
		CSize sizeFileDib = m_dispDib->GetDimensions();
		m_dispDib->Draw(&m_memDC, CPoint(0,0), sizeFileDib);
		//}END : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		InvalidateDrawInspItem(&m_memDC);
    	pDC->BitBlt(0, 0, SMALL_IMAGE_WIDTH, SMALL_IMAGE_HEIGHT, &m_memDC, 0, 0,SRCCOPY); 	

		if (m_bSaveStart)
		{
			m_dispDib->MemoryDC2BMP(m_memDC.m_hDC, m_bmpBuffer, m_strImageFileName);
			m_bSaveStart = FALSE;
		}
	}

	ReleaseDC(pDC);
}

void CDlgBadImage::InvalidateDrawInspItem(CDC *pDC)
{
	long i, j;
	long  nNum, nFrontItemCnt, nRearItemCnt;
	float dLowLimit, dHighLimit, dValue;
	float dScale;
	CString szbuf, strTitle;
	CRect rcRect;
	CFont *pNewFont, *pOldFont;
	CPen   *pNewPen, *pOldPen;
	CPoint vtSt, vtEt;
	COLORREF nColor;
	
	pNewFont = new CFont;
	pNewFont->CreateFont(15, 0, 0, 5, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    								   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));
	pOldFont = pDC->SelectObject(pNewFont);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	
	rcRect = m_pMainFrm->m_fmRect;
	dScale = 1.0f/m_pMainFrm->m_fZoomScale;

	nFrontItemCnt = 0;
	nRearItemCnt = 0;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		strTitle.Format(_T("%s"), m_pSelRes.m_strTitle[i]);
		if (!_tcscmp(strTitle, _T(""))) continue;
		
		if (m_pSelRes.m_bDecisionROI[i])
			nColor = RGB(255, 0, 0);
		else 
			nColor = RGB(0, 255, 0);

		pNewPen = new CPen(PS_SOLID, 1, nColor);
    	pOldPen= pDC->SelectObject(pNewPen);

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		dLowLimit = m_pSelRes.m_dLowLimit[i];
		dHighLimit = m_pSelRes.m_dHighLimit[i];
		dValue = m_pSelRes.m_dValue[i];

		if ((m_pSelRes.m_nInspType[i] != INSP_MEASURE_SIDE_ONE && 
			 m_pSelRes.m_nInspType[i] != INSP_SLINE) && 
			 m_pSelRes.m_bExecROI[i])
		{
			szbuf.Format(_T("%s : [%3.2f %3.2f]=%3.2f"), strTitle, dLowLimit, dHighLimit, dValue);
			if (!m_pSelRes.m_bLocROI[i])
			{
				pDC->TextOut(150, 250 + nFrontItemCnt*20, szbuf);
   				nFrontItemCnt++;
			}
			else
			{
				pDC->TextOut(600, 250 + nRearItemCnt*20, szbuf);
   				nRearItemCnt++;
			}
		}

#if _NG_DISP
		for(j=0; j<m_pSelRes.m_nSampleNum[i]; j++)
		{
			vtSt.x = 0;
			vtSt.y = 0;
			vtEt.x = 0;
			vtEt.y = 0;

			if (m_pSelRes.m_nInspType[i] == INSP_MEASURE_SIDE_ONE)
			{
				vtEt.x = (long)((m_pSelRes.m_ptSecondEdge[i][j].x - rcRect.left-1)/dScale);
			    vtEt.y = (long)((m_pSelRes.m_ptSecondEdge[i][j].y - rcRect.top-1)/dScale);
			}

			if (m_pSelRes.m_nInspType[i] == INSP_MEASURE_SIDE_TWO)
			{
				vtSt.x = (long)((m_pSelRes.m_ptFirstEdge[i][j].x - rcRect.left-1)/dScale);
				vtSt.y = (long)((m_pSelRes.m_ptFirstEdge[i][j].y - rcRect.top-1)/dScale);
				vtEt.x = (long)((m_pSelRes.m_ptSecondEdge[i][j].x - rcRect.left-1)/dScale);
				vtEt.y = (long)((m_pSelRes.m_ptSecondEdge[i][j].y - rcRect.top-1)/dScale);
			}
			
			pDC->MoveTo((long)(vtEt.x - 3), (long)(vtEt.y-3));
			pDC->LineTo((long)(vtEt.x + 4), (long)(vtEt.y+4));
			pDC->MoveTo((long)(vtEt.x + 3), (long)(vtEt.y-3));
			pDC->LineTo((long)(vtEt.x - 4), (long)(vtEt.y+4));
			
			pDC->MoveTo((long)(vtSt.x - 3), (long)(vtSt.y-3));
			pDC->LineTo((long)(vtSt.x + 4), (long)(vtSt.y+4));
			pDC->MoveTo((long)(vtSt.x + 3), (long)(vtSt.y-3));
			pDC->LineTo((long)(vtSt.x - 4), (long)(vtSt.y+4));
		}
		
		//szbuf.Format(_T("%s : [%3.2f %3.2f]=%3.2f"), strTitle, dLowLimit, dHighLimit, dValue);
		//pDC->TextOut(400, 300 + nFrontItemCnt*24, szbuf);
		//nFrontItemCnt++;

		vtSt.x = (long)((m_pSelRes.m_dPosRoiX1[i] - rcRect.left-1)/dScale);
		vtSt.y = (long)((m_pSelRes.m_dPosRoiY1[i] - rcRect.top-1)/dScale);
		vtEt.x = (long)((m_pSelRes.m_dPosRoiX2[i] - rcRect.left-1)/dScale);
		vtEt.y = (long)((m_pSelRes.m_dPosRoiY2[i] - rcRect.top-1)/dScale);

		szbuf.Format(_T("%s"), strTitle);
		pDC->TextOut((long)(vtEt.x+2), (long)(vtEt.y), szbuf);

		pDC->MoveTo((long)(vtSt.x), (long)(vtSt.y));
		pDC->LineTo((long)(vtEt.x), (long)(vtSt.y));
		pDC->LineTo((long)(vtEt.x), (long)(vtEt.y));
		pDC->LineTo((long)(vtSt.x), (long)(vtEt.y));
		pDC->LineTo((long)(vtSt.x), (long)(vtSt.y));			
#endif

		pDC->SelectObject(pOldPen);
        delete pNewPen;
	}


	nNum = m_pSelRes.m_nLinkNum;
	for(i=0; i<nNum; i++)
	{
		strTitle.Format(_T("%s"), m_pSelRes.m_strLinkTitle[i]);
		if (!_tcscmp(strTitle, _T(""))) continue;
		
		if (m_pSelRes.m_bLinkDecROI[i]) 
			nColor = RGB(255, 0, 0);
		else 
			nColor = RGB(0, 255, 0);

		pDC->SetTextColor(nColor);
		pDC->SetBkMode(TRANSPARENT);

		dLowLimit = m_pSelRes.m_dLinkLowLimit[i];
		dHighLimit = m_pSelRes.m_dLinkHighLimit[i];
		dValue = m_pSelRes.m_dLinkValue[i];
		
		if (m_pSelRes.m_bLinkExecROI[i])
		{
			szbuf.Format(_T("%s : [%3.2f %3.2f]=%3.2f"), strTitle, dLowLimit, dHighLimit, dValue);
			if (!m_pSelRes.m_bLinkLocROI[i])
			{
				pDC->TextOut(150, 250 + nFrontItemCnt*20, szbuf);
   				nFrontItemCnt++;
			}
			else
			{
				pDC->TextOut(600, 250 + nRearItemCnt*20, szbuf);
   				nRearItemCnt++;
			}
		}
	}

	pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CDlgBadImage::SetSaveImageFile(CString strPathName)
{
	m_bSaveStart = TRUE;
	m_strImageFileName = strPathName;
}

void CDlgBadImage::UpdateFrameBadImage(long nCAM, long nIndexPnt)
{
	int i, j, nSelPnt, nHighS, nHighD;
	float x1, y1, x2, y2, fSX, fSY, fScale;
	LPBYTE fm, pImage;
	CPoint ptMouse, ptOldMouse;

	if (nCAM<0 || nIndexPnt<0)
	    return;

	fm = m_dispDib->m_lpImage;
	fScale = 1.0f/m_pMainFrm->m_fZoomScale;

	ptMouse = m_pMainFrm->m_ptMouse;
	ptOldMouse = m_pMainFrm->m_ptOldMouse;

	nSelPnt = g_DataRT.m_nBadIndex[nCAM] - nIndexPnt - 1;
	if (nSelPnt<0)
		nSelPnt += DEF_QUEUE_MAX_NUM;

	memcpy(&m_pSelRes, &g_DataRT.m_BadRes[nCAM][nSelPnt], sizeof(INSP_RESULT_DATA));
	pImage = g_pBadImage[nCAM][nSelPnt];

	if (pImage && fm)
	{
		fSX = (float)(long)((IMAGE_WIDTH - SMALL_IMAGE_WIDTH*fScale)/2.0f + ptMouse.x);
		fSY = (float)(long)((IMAGE_HEIGHT - SMALL_IMAGE_HEIGHT*fScale)/2.0f + ptMouse.y);

		y1 = fSY;
		y2 = fSY + SMALL_IMAGE_HEIGHT*fScale;
		x1 = fSX;
		x2 = fSX + SMALL_IMAGE_WIDTH*fScale;
		
		if (x1<0 && x2>=IMAGE_WIDTH)
		{
			fSX = (IMAGE_WIDTH-(x2 - x1)) / 2.0f;
			m_pMainFrm->m_ptMouse.x = 0;
		}
		else
		{
			if (x1<0)
			{
				fSX = SMALL_IMAGE_WIDTH*fScale;
				if (fSX>=IMAGE_WIDTH)
				{
					fSX = (IMAGE_WIDTH-(x2 - x1)) / 2.0f;
					m_pMainFrm->m_ptMouse.x = 0;
				}
				else
				{
					fSX = 0;
					if (m_pMainFrm->m_ptMouse.x<ptOldMouse.x)
						m_pMainFrm->m_ptMouse.x = ptOldMouse.x;
				}
			}
	
			if (x2>=IMAGE_WIDTH)
			{
				fSX = IMAGE_WIDTH - SMALL_IMAGE_WIDTH*fScale;
				if (fSX<0)
					fSX = (IMAGE_WIDTH-(x2 - x1)) / 2.0f;

				if (m_pMainFrm->m_ptMouse.x>ptOldMouse.x)
					m_pMainFrm->m_ptMouse.x = ptOldMouse.x;
			}
		}

		if (y1<0 && y2>=IMAGE_HEIGHT)
		{
			fSY = (IMAGE_HEIGHT-(y2 - y1)) / 2.0f;
			m_pMainFrm->m_ptMouse.y = 0;
		}
		else
		{
			if (y1<0)
			{
				fSY = SMALL_IMAGE_HEIGHT*fScale;
				if (fSY>=IMAGE_HEIGHT)
				{
					fSY = (IMAGE_HEIGHT-(y2 - y1)) / 2.0f;
					m_pMainFrm->m_ptMouse.y = 0;
				}
				else
				{
					fSY = 0;
					if (m_pMainFrm->m_ptMouse.y<ptOldMouse.y)
						m_pMainFrm->m_ptMouse.y = ptOldMouse.y;
				}
			}

			if (y2>=IMAGE_HEIGHT)
			{
				fSY = IMAGE_HEIGHT - SMALL_IMAGE_HEIGHT*fScale;
				if (fSY<0)
				{
					fSY = (IMAGE_HEIGHT-(y2 - y1)) / 2.0f;
					m_pMainFrm->m_ptMouse.y = 0;
				}

				if (m_pMainFrm->m_ptMouse.y>ptOldMouse.y)
					m_pMainFrm->m_ptMouse.y = ptOldMouse.y;
			}
		}

		m_pMainFrm->m_fmRect.left = (long)fSX;
		m_pMainFrm->m_fmRect.top = (long)fSY;
		m_pMainFrm->m_ptOldMouse = m_pMainFrm->m_ptMouse;

		memset(fm, 0, SMALL_IMAGE_HEIGHT*SMALL_IMAGE_WIDTH);

		y1 = fSY;
		for(i=0; i<SMALL_IMAGE_HEIGHT; i++)
		{
			y1 += fScale;
			x1 = fSX;

			if (y1<0 ||  y1>IMAGE_HEIGHT-2)
				continue;

			nHighS = (long)(y1+0.5)*IMAGE_WIDTH;
			nHighD = (SMALL_IMAGE_HEIGHT-i-1) * SMALL_IMAGE_WIDTH;
			
			for(j=0; j<SMALL_IMAGE_WIDTH; j++)
			{
				x1 += fScale;
				if (x1<0 || x1>IMAGE_WIDTH-2) 
					continue;

				fm[nHighD + j] = (unsigned char)pImage[nHighS + (long)(x1+0.5)];
			}
		}
	}

	InvalidateView();
}

/*
void CDlgBadImage::UpdateFrameBadImage(long nCAM, long nIndexPnt)
{
	long i, j, nS, nT, nSelPnt;
	long nSWidth, nSHeight;
	float fScale;
	LPBYTE fm;
	LPBYTE pImage;

	fm = m_dispDib->m_lpImage;
    if (fm)
	{
		nSelPnt = g_DataRT.m_nBadIndex[nCAM] - nIndexPnt - 1;
		if (nSelPnt<0)
			nSelPnt += DEF_QUEUE_MAX_NUM;

		memcpy(&m_pSelRes, &g_DataRT.m_BadRes[nCAM][nSelPnt], sizeof(INSP_RESULT_DATA));
		m_pSelRes.m_nSampleNum[0];

		pImage = g_pImage[nCAM][nSelPnt];
		nSHeight = SMALL_IMAGE_HEIGHT-3;
		nSWidth = SMALL_IMAGE_WIDTH-3;

		for(i=0; i<nSHeight; i++)
		{
			nS = i*IMAGE_WIDTH;
			nT = (SMALL_IMAGE_HEIGHT-i-1)*SMALL_IMAGE_WIDTH;
			for(j=0; j<nSWidth; j++)
				fm[nT + j] = pImage[nS + j];
		}
	}

	InvalidateView();
}
*/


void CDlgBadImage::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_ptRStartPnt.x = point.x;
	m_ptRStartPnt.y = point.y;

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CDlgBadImage::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_RBUTTON)
	{
		m_pMainFrm->m_ptMouse.x += (long)((m_ptRStartPnt.x - point.x)/m_pMainFrm->m_fZoomScale);
       	m_pMainFrm->m_ptMouse.y += (long)((m_ptRStartPnt.y - point.y)/m_pMainFrm->m_fZoomScale);

		m_ptRStartPnt = point;
		m_pMainFrm->UpdateBadImage();
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}
