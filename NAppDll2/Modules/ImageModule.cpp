// ImageModule.cpp : implementation file
//

#include "stdafx.h"
#include "ImageModule.h"
#include "..\..\NVisionDll\NVisionDll.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageModule
CImageModule::CImageModule()
{
	m_pData = NULL;
	m_strImageFileName = "";
	m_strGrayLevel = "";

	m_nImageWidth = IMAGE_WIDTH; 			  // BMP file의 폭
	m_nImageHeight= IMAGE_HEIGHT;    		  // BMP file의 높이
	m_nImagePitch = IMAGE_WIDTH * IMAGE_BAND; // BMP file의 Pitch
	m_nImageColor = 8;                        // 8bit Default     
	m_nImageSize  = m_nImageWidth * m_nImageHeight * IMAGE_BAND;
	m_nMarginFillColor = 200;

	m_pRawImage = NULL;
//	m_pDispImage = NULL;
	m_fileDib = NULL;
	m_dispDib = NULL;
	m_bAllocFrame = FALSE;
	m_bDispFrame = FALSE;

	SetImageSize(IMAGE_WIDTH, IMAGE_HEIGHT);
	SetFrameSize(FRAME_WIDTH, FRAME_HEIGHT, IMAGE_BAND, IMAGE_DEPTH);

	InitializeCriticalSection(&m_csSync);
}

CImageModule::~CImageModule()
{
	DeleteCriticalSection(&m_csSync);

    if (m_fileDib)       delete m_fileDib;
    if (m_dispDib)       delete m_dispDib;

	if (m_pRawImage) {
		delete m_pRawImage;
		m_pRawImage = NULL;
	}
	/*
	if (m_pDispImage) {
		delete m_pDispImage;
		m_pDispImage = NULL;
	}
	*/
}

void CImageModule::SetModelData(SETUP_MODEL_DATA *pData)
{
    m_pData = pData;
}

//////////////////////////////////////////////////////////
//                   Image Infomation
////////////////////////////////////////////////////////// 
BOOL CImageModule::GetCurrentImage()
{
	long i, j;
	LPBYTE fm;

	fm = m_fileDib->m_lpImage;
	if (m_nImageColor == 8) 
	{
		for(i=0; i<m_nImageHeight; i++) 
		for(j=0; j<m_nImageWidth; j++) 
			*(m_pRawImage + m_nImageWidth*(m_nImageHeight-i-1)+j) = *(fm + m_nImagePitch*i + j);
	}

	return m_nImageColor;
}

void CImageModule::DisplayImageToFrame(LPBYTE pImage)
{
	int i, j, nHighS, nHighD;
	float x1, y1, x2, y2, fSX, fSY, fScale;
	LPBYTE fm;
	CPoint ptMouse, ptOldMouse;

	if (m_bAllocFrame) return;

	EnterCriticalSection(&m_csSync);

	fm = m_dispDib->m_lpImage;
	fScale = 1.0f/m_pData->m_IMG.m_fZoomScale;
	ptMouse = m_pData->m_IMG.m_ptMouse;
	ptOldMouse.x = m_pData->m_IMG.m_nOldMouseX;
	ptOldMouse.y = m_pData->m_IMG.m_nOldMouseY;
	
	if (pImage && fm)
	{
		fSX = (float)(long)((m_nImageWidth - m_nFrameWidth*fScale)/2.0f + ptMouse.x);
		fSY = (float)(long)((m_nImageHeight - m_nFrameHeight*fScale)/2.0f + ptMouse.y);

		y1 = fSY;
		y2 = fSY + m_nFrameHeight*fScale;
		x1 = fSX;
		x2 = fSX + m_nFrameWidth*fScale;
		
		if (x1<0 && x2>=m_nImageWidth)
		{
			fSX = (m_nImageWidth-(x2 - x1)) / 2.0f;
			m_pData->m_IMG.m_ptMouse.x = 0;
		}
		else
		{
			if (x1<0)
			{
				fSX = m_nFrameWidth*fScale;
				if (fSX>=m_nImageWidth)
				{
					fSX = (m_nImageWidth-(x2 - x1)) / 2.0f;
					m_pData->m_IMG.m_ptMouse.x = 0;
				}
				else
				{
					fSX = 0;
					if (m_pData->m_IMG.m_ptMouse.x<ptOldMouse.x)
						m_pData->m_IMG.m_ptMouse.x = ptOldMouse.x;
				}
			}
	
			if (x2>=m_nImageWidth)
			{
				fSX = m_nImageWidth - m_nFrameWidth*fScale;
				if (fSX<0)
					fSX = (m_nImageWidth-(x2 - x1)) / 2.0f;

				if (m_pData->m_IMG.m_ptMouse.x>ptOldMouse.x)
					m_pData->m_IMG.m_ptMouse.x = ptOldMouse.x;
			}
		}

		if (y1<0 && y2>=m_nImageHeight)
		{
			fSY = (m_nImageHeight-(y2 - y1)) / 2.0f;
			m_pData->m_IMG.m_ptMouse.y = 0;
		}
		else
		{
			if (y1<0)
			{
				fSY = m_nFrameHeight*fScale;
				if (fSY>=m_nImageHeight)
				{
					fSY = (m_nImageHeight-(y2 - y1)) / 2.0f;
					m_pData->m_IMG.m_ptMouse.y = 0;
				}
				else
				{
					fSY = 0;
					if (m_pData->m_IMG.m_ptMouse.y<ptOldMouse.y)
						m_pData->m_IMG.m_ptMouse.y = ptOldMouse.y;
				}
			}

			if (y2>=m_nImageHeight)
			{
				fSY = m_nImageHeight - m_nFrameHeight*fScale;
				if (fSY<0)
				{
					fSY = (m_nImageHeight-(y2 - y1)) / 2.0f;
					m_pData->m_IMG.m_ptMouse.y = 0;
				}

				if (m_pData->m_IMG.m_ptMouse.y>ptOldMouse.y)
					m_pData->m_IMG.m_ptMouse.y = ptOldMouse.y;
			}
		}

		m_pData->m_IMG.m_fmRect.left = (long)fSX;
		m_pData->m_IMG.m_fmRect.top = (long)fSY;
		m_pData->m_IMG.m_nOldMouseX = m_pData->m_IMG.m_ptMouse.x;
		m_pData->m_IMG.m_nOldMouseY = m_pData->m_IMG.m_ptMouse.y;

		memset(fm, 0, m_nFrameHeight*m_nFrameWidth);

		y1 = fSY;
		for(i=0; i<m_nFrameHeight; i++)
		{
			y1 += fScale;
			x1 = fSX;

			if (y1<0 ||  y1>m_nImageHeight-2)
				continue;

			nHighS = (long)(y1+0.5)*m_nImageWidth;
			nHighD = (m_nFrameHeight-i-1) * m_nFrameWidth;
			
			for(j=0; j<m_nFrameWidth; j++)
			{
				x1 += fScale;
				if (x1<0 || x1>m_nImageWidth-2) 
					continue;

				fm[nHighD + j] = (unsigned char)pImage[nHighS + (long)(x1+0.5)];	
			}
		}
	}

	LeaveCriticalSection(&m_csSync);
}

void CImageModule::WriteGrayLevelPos(long x, long y, long *nPosX, long *nPosY, long *nLevel)
{
	float fScale;
	CRect  rcRect;

	fScale = 1.0f/m_pData->m_IMG.m_fZoomScale;
	rcRect = m_pData->m_IMG.m_fmRect;

	*nPosX = (long)(rcRect.left + x*fScale + 0.5);
	*nPosY = (long)(rcRect.top + y*fScale + 0.5);

	*nLevel = 0;
	if (0<*nPosX && *nPosX<m_nImageWidth && 0<*nPosY && *nPosY<m_nImageHeight)
	{
		*nLevel = m_pRawImage[(*nPosY)*m_nImageWidth + (*nPosX)];
		m_strGrayLevel.Format(_T("[%d  %d] = %d"), *nPosX, *nPosY, *nLevel);
	}
}

void CImageModule::SetImageSize(long nWidth, long nHeight)
{
	if (m_fileDib) delete m_fileDib;
	m_fileDib = new CDib(nWidth, nHeight, IMAGE_BAND*IMAGE_DEPTH);

	m_nImageWidth = nWidth; 			  // BMP file의 폭
	m_nImageHeight= nHeight;    		  // BMP file의 높이
	m_nImagePitch = nWidth * IMAGE_BAND;       // BMP file의 Pitch
	m_nImageColor = IMAGE_DEPTH;            // 8bit Default     
	m_nImageSize  = nWidth * nHeight * IMAGE_BAND;

	InitImageMemory(nWidth, nHeight);
}

void CImageModule::GetImageSize(long *nWidth, long *nHeight)
{
	*nWidth = m_nImageWidth;
	*nHeight = m_nImageHeight;
}

void CImageModule::SetZoomScale(float fScale)
{
#if 0
	long nTempX, nTempY;
	float fTemp;

	fTemp = 1.0f/m_pData->m_IMG.m_fZoomScale;

	nTempX = (long)((m_nFrameWidth*fTemp)/2.0 + 0.5);
	nTempY = (long)((m_nFrameHeight*fTemp)/2.0 + 0.5);  
#endif // 0


//	m_pData->m_IMG.m_ptMouse.x += (nTempX-(long)((m_nFrameWidth*fTemp)/2.0 + 0.5));
//	m_pData->m_IMG.m_ptMouse.y += (nTempY-(long)((m_nFrameHeight*fTemp)/2.0 + 0.5));
	m_pData->m_IMG.m_fZoomScale = fScale;
}

float CImageModule::GetZoomScale()
{
	return m_pData->m_IMG.m_fZoomScale;
}

void CImageModule::InitImageMemory(long nWidth, long nHeight) 
{
	if (m_pRawImage) delete m_pRawImage;
    m_pRawImage = new unsigned char[m_nImageWidth*m_nImageHeight];

	/*
	if (m_pDispImage) delete m_pDispImage;
    m_pDispImage = new unsigned char[m_nImageWidth*m_nImageHeight];

	memset(m_pDispImage, 0, m_nImageWidth*m_nImageHeight);
*/

	memset(m_pRawImage, 0, m_nImageWidth*m_nImageHeight);
}

void CImageModule::SetFrameSize(long nWidth, long nHeight, long nBand, long nDepth)
{
//	EnterCriticalSection(&m_csSync);
	m_bAllocFrame = TRUE;

	if (m_dispDib) delete m_dispDib;
	m_dispDib = new CDib(nWidth, nHeight, nBand*nDepth);

	m_nFrameWidth = nWidth; 			  // BMP file의 폭
	m_nFrameHeight= nHeight;    		  // BMP file의 높이

	if (m_pData)
	{
		m_pData->m_IMG.m_fmRect.left = 0;
		m_pData->m_IMG.m_fmRect.top = 0;
		m_pData->m_IMG.m_fmRect.right = nWidth;
		m_pData->m_IMG.m_fmRect.bottom = nHeight;
	}

//  InitFrameMemory(nWidth, nHeight, nBand, nDepth);
	m_bAllocFrame = FALSE;
//	LeaveCriticalSection(&m_csSync);
}

void CImageModule::InitFrameMemory(long nWidth, long nHeight, long nBand, long nDepth)
{

}

long CImageModule::GetCurrImageColor()
{
	return m_nImageColor;
}

long CImageModule::GetCurrImageHeight()
{
	return m_nImageHeight;
}

long CImageModule::GetCurrImageWidth()
{
	return m_nImageWidth;
}
 
void CImageModule::SetCurrentImage(long nPos)
{
	/*
	long i, nType;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		nType = m_pData->m_INSP[i].m_nInspectType;
		if (_tcscmp(m_pData->m_GOM[i].m_strTitle, "") && (nType==INSP_DEFECT_NORMAL || nType==INSP_DEFECT_SIDE_NORMAL)) 
			m_pData->m_INSP[i].m_nCurrViewPos = nPos;
	}*/
}
//////////////////////////////////////////////////////////
//                   Image Infomation
////////////////////////////////////////////////////////// 



//////////////////////////////////////////////////////////
//                 File Handling Function
////////////////////////////////////////////////////////// 
CString CImageModule::GetImageFileName() 
{
	return m_strImageFileName;
}

void CImageModule::SetImageFileName(CString strImageFileName) 
{
    m_strImageFileName = strImageFileName;
}

BOOL CImageModule::OpenImageFile(LPCTSTR lpszPathName) 
{
	BOOL retValue=FALSE;
	CFile file;

	if (file.Open(lpszPathName, CFile::modeRead)) // Disk에서 BMP파일을 열면 
	{
		if (m_fileDib->Read(&file))
		{
			if (m_nImageColor!=8)
			{
				AfxMessageBox(_T("It isn't 8bit gray BMP file."));
				return FALSE;
			}

			// Pitch 구하기
			m_nImagePitch = m_fileDib->m_lpBMIH->biWidth;
			m_nImageWidth = m_fileDib->m_lpBMIH->biWidth;  // BMP file의 폭
			m_nImageHeight = m_fileDib->m_lpBMIH->biHeight;
			m_nImageColor = m_fileDib->m_lpBMIH->biBitCount;

			retValue = TRUE;
		}

		file.Close();
	}

	return retValue;
}

BOOL CImageModule::SaveImageFile(LPCTSTR lpszPathName) 
{
	BOOL bRet;

	bRet = m_fileDib->SaveImageBMP(lpszPathName, m_pRawImage, m_nImageWidth, m_nImageHeight);
	return bRet;
}

BOOL CImageModule::SaveAsImageFile(LPCTSTR lpszPathName) 
{
	long i, j, x, y, nDiv, nWidth, nHeight;
	LPBYTE pImage;
	char szbuf[256];
	BOOL bRet=FALSE;
	
	/*
	CFile file;
	CFileException fe;
	
	// 쓰기 모드로 파일 열기
	if (!file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &fe)) return FALSE;

	m_dispDib->Write(&file);
	file.Close();
	*/

	nDiv = 2;
#if _DUO_CELL
	nDiv = 4;
#endif

	nWidth = m_nImageWidth/nDiv;
	nHeight = m_nImageHeight/nDiv;

	pImage = new unsigned char[nWidth*nHeight];

	y = 0;
	for(i=0; i<m_nImageHeight; i+=nDiv)
	{
		x = 0;
		for(j=0; j<m_nImageWidth; j+=nDiv)
		{
			pImage[y*nWidth + x] = m_pRawImage[i*m_nImageWidth + j];
			x++;
		}

		y++;
	}

//	bRet = m_fileDib->SaveImageBMP(lpszPathName, pImage, nWidth, nHeight);
	StringToChar(lpszPathName, szbuf);
	NVisionDll_SaveImageToFile(szbuf, pImage, nWidth, nHeight);

	delete pImage;
	return bRet;
}

BOOL CImageModule::SaveAsRawImageData(LPCTSTR lpszPathName, unsigned char *fm)
{
	CFile f;
	CString fileName, pathName;

	if (f.Open(lpszPathName, CFile::modeCreate|CFile::modeWrite))
	{
        f.Write(fm, (m_nImageHeight * m_nImageWidth) * sizeof(unsigned char));
		f.Close();
	}

	return TRUE;
}
//////////////////////////////////////////////////////////
//                  File Handling Function
////////////////////////////////////////////////////////// 



//////////////////////////////////////////////////////////
//                   Overlay Function
////////////////////////////////////////////////////////// 
#define COODI_GRAYLEVEL_POSX  5
#define COODI_GRAYLEVEL_POSY  5
BOOL CImageModule::InvalidateView(CDC *pDC) 
{
	long  i, nType, nGOMType;
	CPoint ptStart, ptEnd, ptCircle;

	// 좌표 및 영상 밝기값 표시
	m_dispDib->DrawText(pDC, COODI_GRAYLEVEL_POSX, COODI_GRAYLEVEL_POSY, m_strGrayLevel, 1, RGB(0, 255, 0));
	if (!m_pData->m_bRealTimeMode)
	{
		m_pData->m_dDeltaX = 0.0f;
		m_pData->m_dDeltaY = 0.0f;
		m_pData->m_dDeltaT = 0.0f;
	}

	m_OverlayResultGOM.DrawOverlayResult(pDC, m_pData);

	for(i=0; i<GOM_MAX_NUM; i++)
	{
		if (!_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")) || !m_pData->m_GOM[i].m_bShowROI) continue;

		nType = m_pData->m_INSP[i].m_nInspectType;
		nGOMType = m_pData->m_GOM[i].m_nGOMType;
		switch(nGOMType)
		{
			case GOM_LINE :
			{
//				m_dispDib->DrawLine(pDC, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, 
//										   m_pData->m_GOM[i].m_nThick, m_pData->m_GOM[i].m_nNormalColor);
				break;
			}
			case GOM_RECT :
			case GOM_MASK_RECT :
			{
				m_pData->m_GOM[i].m_bSelectedROI;
				m_OverlayRectGOM.DrawOverlayRect(pDC, i, m_pData);
				break;
			}
			case GOM_CIRCLE :
			{
				m_OverlayCircleGOM.DrawOverlayCircle(pDC, i, m_pData);
				break;
			}
			case GOM_CROSS :
			{
//				m_dispDib->DrawCross(pDC, ptStart.x, ptStart.y, 
//					                        m_pData->m_GOM[i].m_nRadius,  m_pData->m_GOM[i].m_nThick, m_pData->m_GOM[i].m_nNormalColor);
				break;
			}
			case GOM_TEXT :
			{
//				m_dispDib->DrawText(pDC, ptStart.x, ptStart.y, 
//					                       m_pData->m_GOM[i].m_strTitle, m_pData->m_GOM[i].m_nThick, m_pData->m_GOM[i].m_nNormalColor);
				break;
			}
			case GOM_CIRCLE2:
			{
				m_OverlayCircleGOM.DrawOverlayCircle2(pDC, i, m_pData);
				break;
			}
		}
	}

	return TRUE;
}

BOOL CImageModule::SetSelectedROI(POINT2 m_ptMouse)
{
	BOOL bRet, bRetCode;
	long i, nIndex, nMinDist, nTempDist; 

	bRetCode = 0;
	nMinDist = 100000;
	nIndex = -1;
	for(i=0; i<GOM_MAX_NUM; i++)
	{
		m_pData->m_GOM[i].m_bSelectedROI = FALSE;
        if (_tcscmp(m_pData->m_GOM[i].m_strTitle, _T(""))) 
		{
			switch(m_pData->m_GOM[i].m_nGOMType)
			{
				case GOM_LINE :
					break;
				case GOM_RECT :
				case GOM_MASK_RECT :
				{
					bRet = m_OverlayRectGOM.SetSelectedRect(m_pData, i, m_ptMouse, &nTempDist);
					break;
				}
				case GOM_CIRCLE :
				{
					bRet = m_OverlayCircleGOM.SetSelectedCircle(m_pData, i, m_ptMouse, &nTempDist);
					break;
				}
				case GOM_CIRCLE2:
				{
					bRet = m_OverlayCircleGOM.SetSelectedCircle2(m_pData, i, m_ptMouse, &nTempDist);
					break;
				}
				case GOM_CROSS :
				case GOM_TEXT :
				{
					break;
				}
			}

			if (bRet && nTempDist<nMinDist) 
			{
				bRetCode = bRet;
				nMinDist = nTempDist;
				nIndex = i;
			}
		}
	}

	if (nIndex>=0)
		m_pData->m_GOM[nIndex].m_bSelectedROI = bRetCode;

	return TRUE;
}

void CImageModule::SetMoveROI(POINT2 ptMouse, POINT2 ptDelta)
{
	BOOL bRet;
   	long i;
	float dInRadius, dOutRadius;
	POINT2 ptTarg1, ptTarg2, ptTarg3, ptTarg4;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (m_pData->m_GOM[i].m_bSelectedROI && _tcscmp(m_pData->m_GOM[i].m_strTitle, _T(""))) 
		{
			switch(m_pData->m_GOM[i].m_nGOMType)
			{
				case GOM_LINE :
					break;
				case GOM_RECT :
				case GOM_MASK_RECT :
				{
					bRet = m_OverlayRectGOM.SetMoveRect(m_pData, i, ptMouse, ptDelta, &ptTarg1, &ptTarg2, &ptTarg3, &ptTarg4);
					if (bRet)
					{
						if (5<ptTarg1.x && 5<ptTarg1.y && ptTarg1.x<m_nImageWidth-5 && ptTarg1.y<m_nImageHeight-5 &&
							5<ptTarg2.x && 5<ptTarg2.y && ptTarg2.x<m_nImageWidth-5 && ptTarg2.y<m_nImageHeight-5 &&
							5<ptTarg3.x && 5<ptTarg3.y && ptTarg3.x<m_nImageWidth-5 && ptTarg3.y<m_nImageHeight-5 &&
							5<ptTarg4.x && 5<ptTarg4.y && ptTarg4.x<m_nImageWidth-5 && ptTarg4.y<m_nImageHeight-5)
						{
							m_pData->m_GOM[i].m_ptPoint1 = ptTarg1;
							m_pData->m_GOM[i].m_ptPoint2 = ptTarg2;
							m_pData->m_GOM[i].m_ptPoint3 = ptTarg3;
							m_pData->m_GOM[i].m_ptPoint4 = ptTarg4;
						}
					}

					break;
				}
				case GOM_CIRCLE :
				{
					bRet = m_OverlayCircleGOM.SetMoveCircle(m_pData, i, ptMouse, ptDelta, &ptTarg1, &dInRadius);
					if (bRet)
					{
						if (5<ptTarg1.x-dInRadius && ptTarg1.x+dInRadius<m_nImageWidth-5 && 
							5<ptTarg1.y-dInRadius && ptTarg1.y+dInRadius<m_nImageHeight-5) 
						{
							m_pData->m_GOM[i].m_dCX = (float)ptTarg1.x;
							m_pData->m_GOM[i].m_dCY = (float)ptTarg1.y;
							m_pData->m_GOM[i].m_dInRadius = dInRadius;
						}
					}

					break;
				}
				case GOM_CIRCLE2:
					bRet = m_OverlayCircleGOM.SetMoveCircle2(m_pData, i, ptMouse, ptDelta, &ptTarg1, &dInRadius, &dOutRadius);
					if (bRet)
					{
						if (5<ptTarg1.x-dOutRadius && ptTarg1.x+dOutRadius<m_nImageWidth-5 && 
							5<ptTarg1.y-dOutRadius && ptTarg1.y+dOutRadius<m_nImageHeight-5) 
						{
							m_pData->m_GOM[i].m_dCX = (float)ptTarg1.x;
							m_pData->m_GOM[i].m_dCY = (float)ptTarg1.y;
							m_pData->m_GOM[i].m_dInRadius = dInRadius;
							m_pData->m_GOM[i].m_dOutRadius = dOutRadius;
						}
					}

					break;
				case GOM_CROSS :
				case GOM_TEXT :
				{
					break;
				}
			}
		}
	}
}

BOOL CImageModule::DrawLineROI(LPCTSTR strTitle, long x1, long y1, long x2, long y2, long nThick, COLORREF nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
    m_pData->m_GOM[nPos].m_nGOMType = GOM_LINE;

    m_pData->m_GOM[nPos].m_ptPoint1.x = (float)x1;
    m_pData->m_GOM[nPos].m_ptPoint1.y = (float)y1;
    m_pData->m_GOM[nPos].m_ptPoint4.x = (float)x2;
    m_pData->m_GOM[nPos].m_ptPoint4.y = (float)y2;

    m_pData->m_GOM[nPos].m_nThick = nThick;
    m_pData->m_GOM[nPos].m_nNormalColor = nColor;

    m_pData->m_GOM[nPos].m_bShowROI = TRUE;
	return TRUE;
}

BOOL CImageModule::DrawCrossROI(LPCTSTR strTitle, long x, long y, long nSize, long nThick, COLORREF nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
	m_pData->m_GOM[nPos].m_nGOMType = GOM_CROSS;

    m_pData->m_GOM[nPos].m_dCX = (float)x;
    m_pData->m_GOM[nPos].m_dCY = (float)y;

    m_pData->m_GOM[nPos].m_dInRadius = (float)nSize;
    m_pData->m_GOM[nPos].m_nThick = nThick;
    m_pData->m_GOM[nPos].m_nNormalColor = nColor;

    m_pData->m_GOM[nPos].m_bShowROI = TRUE;
	return TRUE;
}

BOOL CImageModule::DrawCircleROI(LPCTSTR strTitle, long x, long y, long nRadius, long nThick, COLORREF nColor)
{
	long nPos;

	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
	m_pData->m_GOM[nPos].m_nGOMType = GOM_CIRCLE;

    m_pData->m_GOM[nPos].m_dCX = (float)x;
    m_pData->m_GOM[nPos].m_dCY = (float)y;

    m_pData->m_GOM[nPos].m_dInRadius = (float)nRadius;
    m_pData->m_GOM[nPos].m_nThick = nThick;
    m_pData->m_GOM[nPos].m_nNormalColor = nColor;

    m_pData->m_GOM[nPos].m_bShowROI = TRUE;
	return TRUE;
}

BOOL CImageModule::DrawRectROI(LPCTSTR strTitle, long x1, long y1, long x2, long y2, long nThick, COLORREF nColor)
{
	double dImageRate;
	long nPos;

	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
	m_pData->m_GOM[nPos].m_nGOMType = GOM_RECT;

	dImageRate = 1.0f/m_pData->m_IMG.m_fZoomScale;

	m_pData->m_GOM[nPos].m_ptPoint1.x = (float)(x1/dImageRate);
	m_pData->m_GOM[nPos].m_ptPoint1.y = (float)(y1/dImageRate);
	m_pData->m_GOM[nPos].m_ptPoint4.x = (float)(x2/dImageRate);
	m_pData->m_GOM[nPos].m_ptPoint4.y = (float)(y2/dImageRate);

    m_pData->m_GOM[nPos].m_nThick = nThick;
    m_pData->m_GOM[nPos].m_nNormalColor = nColor;
    m_pData->m_GOM[nPos].m_bShowROI = TRUE;
    m_pData->m_GOM[nPos].m_bShowTitle = TRUE;

	return TRUE;
}

BOOL CImageModule::DrawTextROI(LPCTSTR strTitle, long x, long y, COLORREF nColor)
{
	long nPos;

	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
	m_pData->m_GOM[nPos].m_nGOMType = GOM_TEXT;

    m_pData->m_GOM[nPos].m_ptPoint1.x = (float)x;
    m_pData->m_GOM[nPos].m_ptPoint1.y = (float)y;

    m_pData->m_GOM[nPos].m_nNormalColor = nColor;
  	m_pData->m_GOM[nPos].m_bShowROI = TRUE;
    m_pData->m_GOM[nPos].m_bShowTitle = TRUE;
	return TRUE;
}

/*
BOOL CImageModule::DrawEllipseROI(LPCTSTR strTitle, long x, long y, float la, float lb, float dAngle, int nThick, COLORREF nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    _tcscpy_s(m_pData->m_GOM[nPos].m_strTitle, strTitle);
	m_pData->m_GOM[nPos].m_nGOMType = GOM_ELLIPSE;

    m_pData->m_GOM[nPos].m_ptPoint1.x = x;
    m_pData->m_GOM[nPos].m_ptPoint1.y = y;
    m_pData->m_GOM[nPos].m_dFacA = la;
    m_pData->m_GOM[nPos].m_dFacB = lb;

	m_pData->m_GOM[nPos].m_dAngle = dAngle;
    m_pData->m_GOM[nPos].m_nThick = nThick;
    m_pData->m_GOM[nPos].m_nNormalColor = nColor;
    m_pData->m_GOM[nPos].m_bShowROI = TRUE;
	return TRUE;
}
*/
/////////////////////////////////////////////////
BOOL CImageModule::GetLineROI(LPCTSTR strTitle, long *x1, long *y1, long *x2, long *y2, long *nThick, COLORREF *nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    *x1 = (long)m_pData->m_GOM[nPos].m_ptPoint1.x;
    *y1 = (long)m_pData->m_GOM[nPos].m_ptPoint1.y;
    *x2 = (long)m_pData->m_GOM[nPos].m_ptPoint4.x;
    *y2 = (long)m_pData->m_GOM[nPos].m_ptPoint4.y;

    *nThick = m_pData->m_GOM[nPos].m_nThick;
    *nColor = m_pData->m_GOM[nPos].m_nNormalColor;
	return TRUE;
}

BOOL CImageModule::GetCrossROI(LPCTSTR strTitle, long *x, long *y, long *nSize, long *nThick, COLORREF *nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    *x = (long)m_pData->m_GOM[nPos].m_dCX;
    *y = (long)m_pData->m_GOM[nPos].m_dCY;
    
	*nSize = (long)m_pData->m_GOM[nPos].m_dInRadius;
    *nThick = m_pData->m_GOM[nPos].m_nThick;
    *nColor = m_pData->m_GOM[nPos].m_nNormalColor;
	return TRUE;
}

BOOL CImageModule::GetCircleROI(LPCTSTR strTitle, long *x, long *y, long *nRadius, long *nThick, COLORREF *nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    *x = (long)m_pData->m_GOM[nPos].m_dCX;
    *y = (long)m_pData->m_GOM[nPos].m_dCY;
    
	*nRadius = (long)m_pData->m_GOM[nPos].m_dInRadius;
    *nThick = m_pData->m_GOM[nPos].m_nThick;
    *nColor = m_pData->m_GOM[nPos].m_nNormalColor;
	return TRUE;
}

/*
BOOL CImageModule::GetEllipseROI(LPCTSTR strTitle, long *cx, long *cy, float *la, float *lb, float *dAngle, int *nThick, COLORREF *nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    *cx = m_pData->m_GOM[nPos].m_ptPoint1.x;
    *cy = m_pData->m_GOM[nPos].m_ptPoint1.y;
    *la = m_pData->m_GOM[nPos].m_dFacA;
    *lb = m_pData->m_GOM[nPos].m_dFacB;

	*dAngle = m_pData->m_GOM[nPos].m_dAngle;
    *nThick = m_pData->m_GOM[nPos].m_nThick;
    *nColor = m_pData->m_GOM[nPos].m_nNormalColor;
	return TRUE;
}
*/

BOOL CImageModule::GetTextROI(LPCTSTR strTitle, long *x, long *y, COLORREF *nColor)
{
	long nPos;
	
	nPos = GetModifyInsertPos(strTitle);
	if (nPos<0) return FALSE;

    *x = (long)m_pData->m_GOM[nPos].m_ptPoint1.x;
    *y = (long)m_pData->m_GOM[nPos].m_ptPoint1.y;

    *nColor = m_pData->m_GOM[nPos].m_nNormalColor;
	return TRUE;
}

BOOL CImageModule::GetModifyInsertPos(LPCTSTR strTitle)
{
	bool bModifyFlag, bInsertFlag;
	long i, nIndex;
	long nModifyPos, nInsertPos;

	if (strTitle == _T("")) return FALSE;

	bModifyFlag = false;
	bInsertFlag = false;

	for(i=0; i<GOM_MAX_NUM; i++)
	{
    	if (!_tcscmp(m_pData->m_GOM[i].m_strTitle, strTitle))
		{
			bModifyFlag = true;
			nModifyPos = i;
			break;
		}

		if (!bInsertFlag)
		{
			if (!_tcscmp(m_pData->m_GOM[i].m_strTitle, _T("")))
			{
		    	nInsertPos = i;
				bInsertFlag = true;
			}			
		}
	}

	nIndex = -1;
	if (bModifyFlag) nIndex = nModifyPos;
	else if (bInsertFlag) nIndex = nInsertPos;

	return nIndex;
}
//////////////////////////////////////////////////////////
//                   Overlay Function
////////////////////////////////////////////////////////// 
