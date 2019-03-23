// cdib.cpp
// new version for WIN32
#include "stdafx.h"
#include "cdib.h"
#include "math.h"

IMPLEMENT_SERIAL(CDib, CObject, 0);

CDib::CDib()
{
	m_hFile = NULL;
	m_hBitmap = NULL;
	m_hPalette = NULL;
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	Empty();
}

//BMP를 하나 임의로 만들때 사용 
CDib::CDib(long nWidth, long nHeight, int nBitCount)
{
	m_hFile = NULL;
	m_hBitmap = NULL;
	m_hPalette = NULL;
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	Empty();

	switch(nBitCount) 
	{
			case 1:
				m_nColorTableEntries = 2;
				break;
			case 4:
				m_nColorTableEntries = 16;
				break;
			case 8:
				m_nColorTableEntries = 256;
				break;
			case 16:
			case 24:
			case 32:
				m_nColorTableEntries = 0;
				break;
			default:
				ASSERT(FALSE);
	}

	m_lpBMIH = (LPBITMAPINFOHEADER) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries];

	m_nBmihAlloc = crtAlloc;
	m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth = nWidth;
	m_lpBMIH->biHeight = nHeight;
	m_lpBMIH->biPlanes = 1;
	m_lpBMIH->biBitCount = nBitCount;
	m_lpBMIH->biCompression = BI_RGB;
	m_lpBMIH->biSizeImage = 0;
	m_lpBMIH->biXPelsPerMeter = 0;
	m_lpBMIH->biYPelsPerMeter = 0;
	m_lpBMIH->biClrUsed = m_nColorTableEntries;
	m_lpBMIH->biClrImportant = m_nColorTableEntries;
	ComputeMetrics();
	memset(m_lpvColorTable, 0, sizeof(RGBQUAD) * m_nColorTableEntries);

	LPBYTE  tmp=(LPBYTE)m_lpvColorTable;

	for(int i=0;i<m_nColorTableEntries;i++)
	{
		*(tmp++)=i;
		*(tmp++)=i;
		*(tmp++)=i;
		*(tmp++)=0;
	}

	m_nImageAlloc = crtAlloc;
	m_lpImage = new BYTE[m_dwSizeImage];  
}

CDib::~CDib()
{
	Empty();
}

CSize CDib::GetDimensions()
{	
	if(m_lpBMIH == NULL) return CSize(0, 0);
	return CSize((int) m_lpBMIH->biWidth, (int) m_lpBMIH->biHeight);
}

BOOL CDib::AttachMapFile(TCHAR* strPathname, BOOL bShare) // for reading
{
	// if we open the same file twice, Windows treats it as 2 separate files
	HANDLE hFile = ::CreateFile(strPathname, GENERIC_READ, bShare ? FILE_SHARE_READ : 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(hFile);
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	DWORD dwErr = ::GetLastError();
	if(hMap == NULL) {
		AfxMessageBox(_T("Empty bitmap file"));
		return FALSE;
	}
	LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0); // map whole file
	if(((LPBITMAPFILEHEADER) lpvFile)->bfType != 0x4d42) {
		AfxMessageBox(_T("Invalid bitmap file"));
		return FALSE;
	}
	AttachMemory((LPBYTE) lpvFile + sizeof(BITMAPFILEHEADER));
	m_lpvFile = lpvFile;
	m_hFile = hFile;
	m_hMap = hMap;
	return TRUE;
}

BOOL CDib::CopyToMapFile(TCHAR* strPathname)
{
	// copies DIB to a new file, releases prior pointers
	// if you previously used CreateSection, the HBITMAP will be NULL (and unusable)
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	bmfh.bfSize = m_dwSizeImage + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries + sizeof(BITMAPFILEHEADER);
	// meaning of bfSize open to interpretation
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	
	HANDLE hFile = ::CreateFile(strPathname, GENERIC_WRITE | GENERIC_READ, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(hFile);
	int nSize =  sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * m_nColorTableEntries +  m_dwSizeImage;
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, nSize, NULL);
	DWORD dwErr = ::GetLastError();
	ASSERT(hMap);
	LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0); // map whole file
	ASSERT(lpvFile);
	LPBYTE lpbCurrent = (LPBYTE) lpvFile;
	memcpy(lpbCurrent, &bmfh, sizeof(BITMAPFILEHEADER)); // file header
	lpbCurrent += sizeof(BITMAPFILEHEADER);
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) lpbCurrent;
	memcpy(lpbCurrent, m_lpBMIH,
		sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries); // info
	lpbCurrent += sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
	memcpy(lpbCurrent, m_lpImage, m_dwSizeImage); // bit image
	DWORD dwSizeImage = m_dwSizeImage;
	Empty();
	m_dwSizeImage = dwSizeImage;
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	m_lpBMIH = lpBMIH;
	m_lpImage = lpbCurrent;
	m_hFile = hFile;
	m_hMap = hMap;
	m_lpvFile = lpvFile;
	ComputePaletteSize(m_lpBMIH->biBitCount);
	ComputeMetrics();
	MakePalette();
	return TRUE;
}

BOOL CDib::AttachMemory(LPVOID lpvMem, BOOL bMustDelete, HGLOBAL hGlobal)
{
	// assumes contiguous BITMAPINFOHEADER, color table, image
	// color table could be zero length
	Empty();
	m_hGlobal = hGlobal;
	if(bMustDelete == FALSE) {
		m_nBmihAlloc = noAlloc;
	}
	else {
		m_nBmihAlloc = ((hGlobal == NULL) ? crtAlloc : heapAlloc);
	}
	m_lpBMIH = (LPBITMAPINFOHEADER) lpvMem;
	ComputePaletteSize(m_lpBMIH->biBitCount);
	ComputeMetrics();
	m_lpImage = (LPBYTE) m_lpvColorTable + sizeof(RGBQUAD) * m_nColorTableEntries;
	MakePalette();
	return TRUE;
}

UINT CDib::UsePalette(CDC* pDC, BOOL bBackground /* = FALSE */)
{
	if(m_hPalette == NULL) return 0;
	HDC hdc = pDC->GetSafeHdc();
	HPALETTE hOldPalette = ::SelectPalette(hdc, m_hPalette, bBackground);
	return ::RealizePalette(hdc);
}

BOOL CDib::Draw(CDC* pDC, CPoint origin, CSize size)
{
    if (m_lpBMIH == NULL) return FALSE;
    if (m_hPalette != NULL) {
      ::SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
    }
    ::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
        0, 0, m_lpBMIH->biWidth, m_lpBMIH->biHeight,
        m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS, SRCCOPY);
    return TRUE;
}

HBITMAP CDib::CreateSection(CDC* pDC)
{
	if(m_lpBMIH == NULL) return NULL;
	if(m_lpImage != NULL) return NULL; // can only do this if image doesn't exist
	m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO) m_lpBMIH,
		DIB_RGB_COLORS,	(LPVOID*) &m_lpImage, NULL, 0);
	ASSERT(m_lpImage != NULL);
	return m_hBitmap;
}

void CDib::MemoryDC2BMP(HDC hDC, HBITMAP hbit, CString strFileName)
{
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    BITMAP bit;
    BITMAPINFO *pih;
    int PalSize;
    HANDLE hFile;
    DWORD dwWritten, Size;

    // 비트맵 정보로 부터 정보 구조체를 초기화한다.
    GetObject(hbit, sizeof(BITMAP), &bit);
    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = bit.bmWidth;
    ih.biHeight = bit.bmHeight;
    ih.biPlanes = 1;
    ih.biBitCount = bit.bmPlanes * bit.bmBitsPixel;
    if(ih.biBitCount > 8) ih.biBitCount = 24;
    ih.biCompression = BI_RGB;
    ih.biSizeImage = 0;
    ih.biXPelsPerMeter = 0;
    ih.biYPelsPerMeter = 0;
    ih.biClrUsed = 0;
    ih.biClrImportant = 0;
    
    // 정보 구조체 + 팔레트 크기만큼 메모리를 할당하고 이 버퍼에 정보 구조체를 복사한다.
    PalSize = (ih.biBitCount==24 ? 0:1 << ih.biBitCount) * sizeof(RGBQUAD);
    pih = (BITMAPINFO*)malloc(ih.biSize + PalSize);
    pih->bmiHeader = ih;

    // 비트맵의 크기를 구한다.
    GetDIBits(hDC, hbit, 0, bit.bmHeight, NULL, pih, DIB_RGB_COLORS);
    ih = pih->bmiHeader;

    // 비트맵의 크기가 구해지지 않았을 경우 수작업으로 직접 계산한다.
    if(ih.biSizeImage==0) {
        ih.biSizeImage = ((((ih.biWidth*ih.biBitCount)+31) & ~31) >> 3) * ih.biHeight;
    }

    // 래스터 데이터를 읽기위해 메모리를 재할당한다.
    Size = ih.biSize + PalSize + ih.biSizeImage;
    pih = (BITMAPINFO *) realloc(pih, Size);

    // 래스터 데이터를 읽어들인다.
    GetDIBits(hDC, hbit, 0, bit.bmHeight, (PBYTE)pih+ih.biSize+PalSize, pih, DIB_RGB_COLORS);

    // 파일 헤더를 만든다.
    fh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PalSize;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfSize = Size+sizeof(BITMAPFILEHEADER);
    fh.bfType = 0x4d42;

    // 파일을 생성하고 파일 헤더와 정보 구조체, 팔레트, 래스터 데이터를 출력한다.
    hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &fh, sizeof(fh), &dwWritten, NULL);
    WriteFile(hFile, pih, Size, &dwWritten, NULL);

    CloseHandle(hFile);
	free(pih);
}


BOOL CDib::MakePalette()
{
	// makes a logical palette (m_hPalette) from the DIB's color table
	// this palette will be selected and realized prior to drawing the DIB
	if(m_nColorTableEntries == 0) return FALSE;
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);

	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		m_nColorTableEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = m_nColorTableEntries;
	LPRGBQUAD pDibQuad = (LPRGBQUAD) m_lpvColorTable;
	for(int i = 0; i < m_nColorTableEntries; i++) {
		pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
		pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
		pDibQuad++;
	}
	m_hPalette = ::CreatePalette(pLogPal);
	delete pLogPal;
	return TRUE;
}	

BOOL CDib::SetSystemPalette(CDC* pDC)
{
	if(m_nColorTableEntries != 0) return FALSE;
	HDC hDC = pDC->GetSafeHdc();
//	if (!::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE) return FALSE;
	int nSysColors = ::GetDeviceCaps(hDC, NUMCOLORS);
	int nPalEntries = ::GetDeviceCaps(hDC, SIZEPALETTE);
	ASSERT(nPalEntries <= 256);
	int nEntries = (nPalEntries == 0) ? nSysColors : nPalEntries;
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		nEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nEntries;
	::GetSystemPaletteEntries(hDC, 0, nEntries,
		(LPPALETTEENTRY) ((LPBYTE) pLogPal + 2 * sizeof(WORD)));
	m_hPalette = ::CreatePalette(pLogPal);
	delete pLogPal;
	return TRUE;
}

HBITMAP CDib::CreateBitmap(CDC* pDC)
{
    if (m_dwSizeImage == 0) return NULL;
    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
            CBM_INIT, m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS);
    ASSERT(hBitmap);
    return hBitmap;
}

BOOL CDib::Compress(CDC* pDC, BOOL bCompress /* = TRUE */)
{
	// 1. makes GDI bitmap from existing DIB
	// 2. makes a new DIB from GDI bitmap with compression
	// 3. cleans up the original DIB
	// 4. puts the new DIB in the object
	if((m_lpBMIH->biBitCount != 4) && (m_lpBMIH->biBitCount != 8)) return FALSE;
		// compression supported only for 4 bpp and 8 bpp DIBs
	if(m_hBitmap) return FALSE; // can't compress a DIB Section!

	HDC hdc = pDC->GetSafeHdc();
	HPALETTE hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
	HBITMAP hBitmap;  // temporary
	if((hBitmap = CreateBitmap(pDC)) == NULL) return FALSE;
	int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
	LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
	memcpy(lpBMIH, m_lpBMIH, nSize);  // new header
	if(bCompress) {
		switch (lpBMIH->biBitCount) {
		case 4:
			lpBMIH->biCompression = BI_RLE4;
			break;
		case 8:
			lpBMIH->biCompression = BI_RLE8;
			break;
		default:
			ASSERT(FALSE);
		}
		// calls GetDIBits with null data pointer to get size of compressed DIB
		if(!::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
						NULL, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS)) {
			AfxMessageBox(_T("Unable to compress this DIB"));
			// probably a problem with the color table
	 		::DeleteObject(hBitmap);
			delete [] lpBMIH;
			::SelectPalette(hdc, hOldPalette, FALSE);
			return FALSE; 
		}
		if (lpBMIH->biSizeImage == 0) {
			AfxMessageBox(_T("Driver can't do compression"));
	 		::DeleteObject(hBitmap);
			delete [] lpBMIH;
			::SelectPalette(hdc, hOldPalette, FALSE);
			return FALSE; 
		}
		else {
			m_dwSizeImage = lpBMIH->biSizeImage;
		}
	}
	else {
		lpBMIH->biCompression = BI_RGB; // decompress
		// figure the image size from the bitmap width and height
		DWORD dwBytes = ((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) / 32;
		if(((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) % 32) {
			dwBytes++;
		}
		dwBytes *= 4;
		m_dwSizeImage = dwBytes * lpBMIH->biHeight; // no compression
		lpBMIH->biSizeImage = m_dwSizeImage;
	} 
	// second GetDIBits call to make DIB
	LPBYTE lpImage = (LPBYTE) new char[m_dwSizeImage];
	VERIFY(::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
    		lpImage, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS));
    
	::DeleteObject(hBitmap);
	Empty();
	m_nBmihAlloc = m_nImageAlloc = crtAlloc;
	m_lpBMIH = lpBMIH;
	m_lpImage = lpImage;
	ComputePaletteSize(m_lpBMIH->biBitCount);
	ComputeMetrics();
	MakePalette();
	::SelectPalette(hdc, hOldPalette, FALSE);

	return TRUE;
}


BOOL CDib::Read(CFile* pFile)
{
	// 1. read file header to get size of info hdr + color table
	// 2. read info hdr (to get image size) and color table
	// 3. read image
	// can't use bfSize in file header
	Empty();
	int nCount, nSize;
	BITMAPFILEHEADER bmfh;
	TRY {
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			AfxMessageBox(_T("read error 1"));
			return FALSE;
		}
		if(bmfh.bfType != 0x4d42) {
			AfxMessageBox(_T("Invalid bitmap file"));
			return FALSE;
		}
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
		m_nBmihAlloc = m_nImageAlloc = crtAlloc;
		nCount = pFile->Read(m_lpBMIH, nSize); // info hdr & color table
		ComputeMetrics();
		m_lpImage = (LPBYTE) new char[m_dwSizeImage];
		nCount = pFile->Read(m_lpImage, m_dwSizeImage); // image only
	}
	CATCH (CException, e) {
		AfxMessageBox(_T("Read error 1"));
		return FALSE;
	}
	END_CATCH
	ComputePaletteSize(m_lpBMIH->biBitCount);
	MakePalette();

	return TRUE;
}

BOOL CDib::Read8BitRaw(CFile* pFile, int nWidth, int nHeight)
{
	// 1. read file header to get size of info hdr + color table
	// 2. read info hdr (to get image size) and color table
	// 3. read image
	// can't use bfSize in file header
	Empty();
	int nCount, nSize=1064;
	BITMAPFILEHEADER bmfh;
	TRY {
		nCount=MakeBMPFileHead(&bmfh,nWidth,nHeight);
		if(nCount != sizeof(BITMAPFILEHEADER)) {
			AfxMessageBox(_T("read error 1"));
			return FALSE;
		}
		if(bmfh.bfType != 0x4d42) {
			AfxMessageBox(_T("Invalid bitmap file"));
			return FALSE;
		}
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
		m_nBmihAlloc = m_nImageAlloc = crtAlloc;
		MakeBMPRead(m_lpBMIH,nWidth,nHeight); // info hdr & color table
		ComputeMetrics();
		MakePalette(TRUE);
		m_dwSizeImage=nWidth*nHeight;
		m_lpImage = (LPBYTE) new char[m_dwSizeImage];
		nCount = pFile->Read(m_lpImage, m_dwSizeImage); // image only
	}
	CATCH (CException, e) {
		AfxMessageBox(_T("Read error 1"));
		return FALSE;
	}
	END_CATCH
	ComputePaletteSize(m_lpBMIH->biBitCount);
	MakePalette();
	return TRUE;
}
int  CDib::MakeBMPFileHead(LPBITMAPFILEHEADER bmfh,int nWidth, int nHeight)
{
   char *bfType = (char*)&bmfh->bfType;
   *bfType++ = 'B';  // 이 두 바이트는 아스키 문자 'B'와 'M'을 넣어야 함
   *bfType++ = 'M';
   bmfh->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
	   + sizeof(RGBQUAD)*256 + nWidth*nHeight;
   bmfh->bfReserved1 = 0;
   bmfh->bfReserved2 = 0;
   bmfh->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
	   + sizeof(RGBQUAD)*256;
   return sizeof(BITMAPFILEHEADER);
}

BOOL CDib::MakeBMPRead(LPBITMAPINFOHEADER bmih, int nWidth, int nHeight)
{
   // BITMAPINFOHEADER bmih
   bmih->biSize = sizeof(BITMAPINFOHEADER);
   bmih->biWidth=nWidth;   // 이 값들은 앞에서 이미 ...
   bmih->biHeight=nHeight;
   bmih->biPlanes = 1;
   bmih->biBitCount = 8;
   bmih->biCompression = BI_RGB;
   bmih->biSizeImage = 0;
   bmih->biXPelsPerMeter = 0;
   bmih->biYPelsPerMeter = 0;
   bmih->biClrUsed = 0;
   bmih->biClrImportant = 0;
   
   return TRUE;
}

BOOL CDib::MakePalette(BOOL flag)
{
	// makes a logical palette (m_hPalette) from the DIB's color table
	// this palette will be selected and realized prior to drawing the DIB
//	if(m_nColorTableEntries == 0) return FALSE;
	m_nColorTableEntries=256;
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);

	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		m_nColorTableEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = m_nColorTableEntries;
	LPRGBQUAD pDibQuad = (LPRGBQUAD) m_lpvColorTable;
	for(int i = 0; i < m_nColorTableEntries; i++) {
		pDibQuad->rgbRed=i;
		pDibQuad->rgbGreen=i;
		pDibQuad->rgbBlue=i;
		pDibQuad->rgbReserved=0;
		pDibQuad++;
	}
	m_hPalette = ::CreatePalette(pLogPal);
	delete pLogPal;
	return TRUE;
}	

BOOL CDib::Write(CFile* pFile)
{
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSize =  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries +  m_dwSizeImage;
	int nSizeBITMAPFILEHEADER=sizeof(BITMAPFILEHEADER);
	bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	
	TRY {
		pFile->Write((LPVOID) &bmfh, nSizeBITMAPFILEHEADER);
		pFile->Write((LPVOID) m_lpBMIH, bmfh.bfOffBits - nSizeBITMAPFILEHEADER);
		pFile->Write((LPVOID) m_lpImage, nSize-bmfh.bfOffBits+nSizeBITMAPFILEHEADER);
	}
	CATCH (CException, e) {
		AfxMessageBox(_T("write error"));
		return FALSE;
	}
	END_CATCH
	return TRUE;
}

BOOL CDib::Write8BitRaw(CFile* pFile)
{
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSize =  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries +  m_dwSizeImage;
	int nSizeBITMAPFILEHEADER=sizeof(BITMAPFILEHEADER);
	bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	
	TRY {
	//	pFile->Write((LPVOID) &bmfh, nSizeBITMAPFILEHEADER);
	//	pFile->Write((LPVOID) m_lpBMIH, bmfh.bfOffBits - nSizeBITMAPFILEHEADER);
		pFile->Write((LPVOID) m_lpImage, nSize-bmfh.bfOffBits+nSizeBITMAPFILEHEADER);
	}
	CATCH (CException, e) {
		AfxMessageBox(_T("write error"));
		return FALSE;
	}
	END_CATCH
	return TRUE;
}

void CDib::Serialize(CArchive& ar)
{
	ar.Flush();
	if(ar.IsStoring()) {
		Write(ar.GetFile());
	}
	else {
		Read(ar.GetFile());
	}
}

// helper functions
void CDib::ComputePaletteSize(int nBitCount)
{
	if(m_lpBMIH->biClrUsed == 0) {
		switch(nBitCount) {
			case 1:
				m_nColorTableEntries = 2;
				break;
			case 4:
				m_nColorTableEntries = 16;
				break;
			case 8:
				m_nColorTableEntries = 256;
				break;
			case 16:
			case 24:
			case 32:
				m_nColorTableEntries = 0;
				break;
			default:
				ASSERT(FALSE);
		}
	}
	else {
		m_nColorTableEntries = m_lpBMIH->biClrUsed;
	}
	ASSERT(m_nColorTableEntries <= 256); 
}

void CDib::ComputeMetrics()
{
	m_dwSizeImage = m_lpBMIH->biSizeImage;
	if(m_dwSizeImage == 0) {
		DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) / 32;
		if(((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) % 32) {
			dwBytes++;
		}
		dwBytes *= 4;
		m_dwSizeImage = dwBytes * m_lpBMIH->biHeight; // no compression
	}

	m_lpvColorTable = (LPBYTE) m_lpBMIH + sizeof(BITMAPINFOHEADER);
}

void CDib::Empty()
{
	// this is supposed to clean up whatever is in the DIB
	DetachMapFile();
	if(m_nBmihAlloc == crtAlloc) {
		delete [] m_lpBMIH;
	}
	else if(m_nBmihAlloc == heapAlloc) {
		::GlobalUnlock(m_hGlobal);
		::GlobalFree(m_hGlobal);
	}
	if(m_nImageAlloc == crtAlloc) delete [] m_lpImage;
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);
	if(m_hBitmap != NULL) ::DeleteObject(m_hBitmap);
	m_nBmihAlloc = m_nImageAlloc = noAlloc;
	m_hGlobal = NULL;
	m_lpBMIH = NULL;
	m_lpImage = NULL;
	m_lpvColorTable = NULL;
	m_nColorTableEntries = 0;
	m_dwSizeImage = 0;
	m_lpvFile = NULL;
	m_hMap = NULL;
	m_hFile = NULL;
	m_hBitmap = NULL;
	m_hPalette = NULL;
}

void CDib::DetachMapFile()
{
	if(m_hFile == NULL) return;
	::UnmapViewOfFile(m_lpvFile);
	::CloseHandle(m_hMap);
	::CloseHandle(m_hFile);
	m_hFile = NULL;
}


BOOL CDib::Write8BitGrayBMP(CFile* pFile, LPBYTE fm, int width, int height)
{
	int i, j;
	// Image가 4의 배수인가 아닌가결정해서 아니면 아래와 같이 함.
	DWORD dwBytes = ((DWORD) width * 8) / 32;
	if(((DWORD) width * 8) % 32) 	dwBytes++;
	dwBytes *= 4;
	int SizeImage = dwBytes * height; 
	
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSize =  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 +  SizeImage;
	int nSizeBITMAPFILEHEADER=sizeof(BITMAPFILEHEADER);
	bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
					 sizeof(RGBQUAD) * 256;	
	
	BITMAPINFOHEADER bmih;
	bmih.biSize=40;
    bmih.biWidth=width; 		// BMP  폭
    bmih.biHeight=height; 		// BMP  높이
    bmih.biPlanes=1; 			// =1
    bmih.biBitCount=8; 			// 픽셀당 Bit 수  8Bit Gray,  24Bit RGB
    bmih.biCompression=BI_RGB;// ( No Compression 대부분이 이에 해당 )
	if((int)dwBytes!=width)
	{
		bmih.biSizeImage=SizeImage;    	 
		bmih.biXPelsPerMeter=0;  
		bmih.biYPelsPerMeter=0; 	 
		bmih.biClrUsed=256; 		 
		bmih.biClrImportant=256; 	 
	}
	else
	{
		bmih.biSizeImage=0;    	 
		bmih.biXPelsPerMeter=0;  
		bmih.biYPelsPerMeter=0; 	 
		bmih.biClrUsed=0; 		 
		bmih.biClrImportant=0; 	 
	}

	RGBQUAD rgbq[256];
	for(i=0;i<256;i++)
	{
		rgbq[i].rgbBlue=i; 
		rgbq[i].rgbGreen=i; 
		rgbq[i].rgbRed=i; 
		rgbq[i].rgbReserved=0; 
	}

	LPBYTE fmTmp= new BYTE[SizeImage];

	// BMP 파일로 Format으로 만듦.
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			*(fmTmp+dwBytes*i+j)=*(fm+width*(height-i-1)+j);


	TRY {
		pFile->Write((LPVOID) &bmfh, nSizeBITMAPFILEHEADER);
		pFile->Write((LPVOID) &bmih, sizeof(BITMAPINFOHEADER));
		pFile->Write((LPVOID) rgbq, sizeof(RGBQUAD)*256);
		pFile->Write((LPVOID) fmTmp, SizeImage);
	}
	CATCH (CException, e) {
		AfxMessageBox(_T("write error"));
		return FALSE;
	}
	END_CATCH

	delete fmTmp;
	return TRUE;
}

BOOL CDib::DrawPixel(CDC *pDC, long x, long y, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, 1, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	pDC->SetPixel(x, y, color);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawPolyRect(CDC *pDC, long x1, long y1, long x2, long y2, long width, long type, COLORREF color)
{
//	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	float angle, rev_angle;
	CPoint sp, ep;
	CPoint p1, p2, p3, p4;

	pNewPen = new CPen(type, 1, color);
//	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
//	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	if (x1==x2) return FALSE;
	angle = (float)(atan((double)((y1 - y2) / (x1 - x2))) * DEG);

	rev_angle = angle+90.0f;
 	p1.x = (long)(width * cos(rev_angle*RAD) + x1);
    p1.y = (long)(width * sin(rev_angle*RAD) + y1);

	rev_angle = angle-90.0f;
 	p2.x = (long)(width * cos(rev_angle*RAD) + x1);
    p2.y = (long)(width * sin(rev_angle*RAD) + y1);

	rev_angle = angle+90.0f;
 	p3.x = (long)(width * cos(rev_angle*RAD) + x2);
    p3.y = (long)(width * sin(rev_angle*RAD) + y2);

	rev_angle = angle-90.0f;
 	p4.x = (long)(width * cos(rev_angle*RAD) + x2);
    p4.y = (long)(width * sin(rev_angle*RAD) + y2);
     
	pDC->MoveTo(p1.x, p1.y);
	pDC->LineTo(p2.x, p2.y);
	pDC->LineTo(p4.x, p4.y);
	pDC->LineTo(p3.x, p3.y);
	pDC->LineTo(p1.x, p1.y);

//	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
//	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawLine(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawCross(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

    pDC->MoveTo(x-radius, y);
	pDC->LineTo(x+radius, y);
    pDC->MoveTo(x, y-radius);
	pDC->LineTo(x, y+radius);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawEdgeCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;
	CPoint cp;
	CRect  rect;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	cp.x = x;
	cp.y = y;

	rect.bottom = y+radius;
    rect.top = y-radius;
	rect.left = x-radius;
	rect.right = x+radius;

	pDC->Arc(rect, cp, cp);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawFillCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;
	CPoint cp;
	CRect  rect;

	pNewPen = new CPen(PS_SOLID, nThick, RGB(0, 0, 0));
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	cp.x = x;
	cp.y = y;

	rect.bottom = y+radius;
    rect.top = y-radius;
	rect.left = x-radius;
	rect.right = x+radius;

	pDC->Ellipse(rect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y1);
	pDC->LineTo(x2, y2);
	pDC->LineTo(x1, y2);
	pDC->LineTo(x1, y1);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawHeriProfile(CDC *pDC, long x1, long y1, long nNum, long *pData, long nScale, long nThick, COLORREF color)
{
	long i, j;
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(x1, pData[0]);
	for(i=0,j=0; i<nNum; i+=nScale,j++)
	    pDC->LineTo(x1+j, pData[i]);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawVertProfile(CDC *pDC, long x1, long y1, long nNum, long *pData, long nScale, long nThick, COLORREF color)
{
	long i, j;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);

	pOldPen= pDC->SelectObject(pNewPen);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(pData[0], y1);
	for(i=0,j=0; i<nNum; i+=nScale,j++)
	    pDC->LineTo(pData[i], y1+j);

	pDC->SelectObject(pOldPen);
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawPixel(CDC *pDC, long *pCX, long *pCY, long nNum, long nThick, COLORREF color)
{
	long i;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);

	pOldPen= pDC->SelectObject(pNewPen);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(pCX[0], pCY[0]);
	for(i=1; i<nNum; i++)
	    pDC->LineTo(pCX[i], pCY[i]);

	pDC->SelectObject(pOldPen);
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawLine4(CDC *pDC, POINT2 ptStart1, POINT2 ptStart2, POINT2 ptStart3, POINT2 ptStart4, long nThick, COLORREF color)
{
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);

	pOldPen= pDC->SelectObject(pNewPen);
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo((long)ptStart1.x, (long)ptStart1.y);
	pDC->LineTo((long)ptStart2.x, (long)ptStart2.y);
	pDC->LineTo((long)ptStart4.x, (long)ptStart4.y);
    pDC->LineTo((long)ptStart3.x, (long)ptStart3.y);
    pDC->LineTo((long)ptStart1.x, (long)ptStart1.y);

	pDC->SelectObject(pOldPen);
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawFillRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color)
{
	CRect rect;
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

	rect.bottom = y2;
    rect.top = y1;
	rect.left = x1;
	rect.right = x2;

	pDC->FillRect(rect, pNewBrush);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

BOOL CDib::DrawText(CDC *pDC, long x, long y, CString string, long nThick, COLORREF color)
{
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;
	CFont    *pNewFont, *pOldFont;

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);
	pNewFont = new CFont;

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pOldFont = pDC->SelectObject(pNewFont);

	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

    pNewFont->CreateFont(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                               DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Time New Roman"));

	pDC->TextOut(x, y, string, string.GetLength());
	    
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);

	delete pNewBrush;
	delete pNewPen;
	delete pNewFont;

	return TRUE;
}

BOOL CDib::DrawEllipse(CDC *pDC,long lCenterX, long lCenterY, float la, float lb, float angle, long nThick, COLORREF color)
{
	CRect rect;
	CBrush *pNewBrush, *pOldBrush;
	CPen   *pNewPen, *pOldPen;

	bool  flag = false;
    double theta, r1,r2;
    double x, y, rad;
    CPoint savePnt, nextPnt;
    int   ini=1;
    int   BoundCnt=0;

    while(1) {
        if (nThick==ini) break;
        ini+=2;
        BoundCnt++;
	}       

	pNewPen = new CPen(PS_SOLID, nThick, color);
	pNewBrush = new CBrush(color);

	pOldPen= pDC->SelectObject(pNewPen);
	pOldBrush = pDC->SelectObject(pNewBrush);
	pDC->SetBkMode(TRANSPARENT);

    for(theta=0; theta<360; theta+=1.0) {
     // r의 길이를 가변한다.
        rad = theta * RAD;
        float ccc = (float)((la*la+lb*lb)/2 - (la*la-lb*lb)/2*cos(2*rad));
        if(ccc<= 0) continue;
        r1=(la*lb)/(sqrt(ccc));
        for(long cnt=-BoundCnt;cnt<=BoundCnt;cnt++) {
			r2=r1+(double)cnt;
			x = r2 *cos(rad);
			y = r2 *sin(rad);

			//로테이트...!!
			nextPnt.x = (long)(x*cos(angle*RAD)-y*sin(angle*RAD));
			nextPnt.y = (long)(x*sin(angle*RAD)+y*cos(angle*RAD));

			if (!flag) {
				savePnt = nextPnt;
				flag = true;
			}
			pDC->MoveTo(savePnt.x+lCenterX, savePnt.y+lCenterY);  
			pDC->LineTo(nextPnt.x+lCenterX, nextPnt.y+lCenterY);  
			savePnt = nextPnt;
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	delete pNewBrush;
    delete pNewPen;

	return TRUE;
}

void CDib::DrawBitmap(CDC* pDC, HBITMAP hBitmap, CRect rcRect)
{
	if(!hBitmap) return;

	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(hBitmap);

	pDC->BitBlt(rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height(), &memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);
}

BOOL CDib::SaveImageBMP(LPCTSTR strFileName, UCHAR *pImage, long width, long height)
{
	FILE* fp;
	BITMAPFILEHEADER* bmpfile;
	BITMAPINFOHEADER* bmpinfo;
	UCHAR *pSrcImage;
	UCHAR *pDestImage;
	UCHAR *pBmpImage = (UCHAR *)calloc(width*height, sizeof(UCHAR));
	BOOL stat;

	RGBQUAD bmiColor[256];
	for(int i = 0; i < 256; i++){
		bmiColor[i].rgbBlue = i;
		bmiColor[i].rgbGreen = i;
		bmiColor[i].rgbRed = i;
		bmiColor[i].rgbReserved = 0;
	}

	bmpfile = (BITMAPFILEHEADER *)calloc(sizeof(BITMAPFILEHEADER), sizeof(UCHAR));
	bmpinfo = (BITMAPINFOHEADER *)calloc(sizeof(BITMAPINFOHEADER), sizeof(UCHAR));

	MakeBMPHeader(bmpinfo, bmpfile, width, height);

    _tfopen_s(&fp, strFileName, _T("wb"));
	if(fp == NULL) return FALSE;

	fwrite(bmpfile, sizeof(UCHAR), sizeof(BITMAPFILEHEADER), fp);
	fwrite(bmpinfo, sizeof(UCHAR), sizeof(BITMAPINFOHEADER), fp);
	fwrite(bmiColor, sizeof(UCHAR), sizeof(bmiColor), fp);

	for(int h = 0 ; h < height; h++)
	{
		pSrcImage = pImage + ( height - 1 - h )*width;
		pDestImage = pBmpImage + h*width;
		memcpy(pDestImage, pSrcImage, width);
	}

	stat=fwrite(pBmpImage, sizeof(UCHAR), width * height, fp)==width*height*sizeof(UCHAR);
	fclose(fp);

	free(bmpfile);
	free(bmpinfo);
	free(pBmpImage);

	return stat;
}

void CDib::MakeBMPHeader(BITMAPINFOHEADER *pbmiHdr, BITMAPFILEHEADER *pbmfHdr, long width, long height)
{
	memset(pbmfHdr, 0, sizeof(BITMAPFILEHEADER));
	pbmfHdr->bfType = ((WORD)'M'<<8 | 'B');
	pbmfHdr->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + (width*height);
	pbmfHdr->bfReserved1 = 0;
	pbmfHdr->bfReserved2 = 0;
	pbmfHdr->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);

	memset(pbmiHdr, 0, sizeof(BITMAPINFOHEADER));
	pbmiHdr->biSize = sizeof(BITMAPINFOHEADER);
	pbmiHdr->biWidth = width;
	pbmiHdr->biHeight = height;
	pbmiHdr->biPlanes = 1;
	pbmiHdr->biBitCount = 8;
	pbmiHdr->biCompression = BI_RGB;
	pbmiHdr->biSizeImage = width*height;
}

