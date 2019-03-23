#ifndef __BMPRGN_H__
#define __BMPRGN_H__

#define ALLOC_UNIT	100

class CBmpRgn
{
public:

    CBmpRgn(CWnd *pWnd)
	{m_pParentWnd = pWnd;}
    ~CBmpRgn()
	{
//		if(m_pParentWnd) delete m_pParentWnd;
		DeleteObject(hBmp);
	}

    inline HRGN CBmpRgn::BuildRegion(LPCTSTR strFilePath, COLORREF cTransparentColor)
	{
		hBmp = (HBITMAP)LoadImage( AfxGetInstanceHandle(), strFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if(!hBmp) return NULL;

		BITMAP bmp;
		int nCheck = GetObject(hBmp, sizeof(BITMAP), &bmp );
		if(!nCheck) return NULL;

		m_nWndWidth = bmp.bmWidth;
		m_nWndHeight = bmp.bmHeight;

		return BitmapToRegion(hBmp, cTransparentColor, 0x00000000);
	}

    inline HBITMAP CBmpRgn::GetBitmap()
	{return hBmp;}
    inline long CBmpRgn::GetBitmapWidth()
	{return m_nWndWidth;}
    inline long CBmpRgn::GetBitmapHeight()
	{return m_nWndHeight;}

protected:
	inline HRGN CBmpRgn::BitmapToRegion(HBITMAP bmp, COLORREF cTransparentColor, COLORREF cTolerance)
    {
		HRGN hRgn = NULL;

		if(bmp==NULL) return hRgn;

		// Create a memory DC inside which we will scan the bitmap content
		CDC *pDC = m_pParentWnd->GetDC();
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		if(!memDC) return hRgn = NULL;

		// Create a 32 bits depth bitmap and select it into the memory DC 
		BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
				sizeof(BITMAPINFOHEADER),	// biSize 
				m_nWndWidth,				// biWidth; 
				m_nWndHeight,				// biHeight; 
				1,							// biPlanes; 
				32,							// biBitCount 
				BI_RGB,						// biCompression; 
				0,							// biSizeImage; 
				0,							// biXPelsPerMeter; 
				0,							// biYPelsPerMeter; 
				0,							// biClrUsed; 
				0							// biClrImportant; 
		};

		VOID *pbits32 = NULL; 
		HBITMAP hbm32 = CreateDIBSection(memDC.GetSafeHdc(), (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
		if(hbm32)
		{
			HBITMAP holdBmp = (HBITMAP)SelectObject(memDC.GetSafeHdc(), hbm32);

			// Create a DC just to copy the bitmap into the memory DC
			CDC dcMem;
			dcMem.CreateCompatibleDC(&memDC);

			if(dcMem)
			{
				// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
				BITMAP bm32;
				GetObject(hbm32, sizeof(bm32), &bm32);
				while (bm32.bmWidthBytes % 4)
					bm32.bmWidthBytes++;

				// Copy the bitmap into the memory DC
				HBITMAP holdBmp = (HBITMAP)SelectObject(dcMem.GetSafeHdc(), bmp);
				memDC.BitBlt(0, 0, m_nWndWidth, m_nWndHeight, &dcMem, 0, 0, SRCCOPY);

				// For better performances, we will use the ExtCreateRegion() function to create the
				// region. This function take a RGNDATA structure on entry. We will add rectangles by
				// amount of ALLOC_UNIT number in this structure.
				DWORD maxRects = ALLOC_UNIT;
				HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
				RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
				pData->rdh.dwSize = sizeof(RGNDATAHEADER);
				pData->rdh.iType = RDH_RECTANGLES;
				pData->rdh.nCount = pData->rdh.nRgnSize = 0;
				SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

				// Keep on hand highest and lowest values for the "transparent" pixels
				BYTE lr = GetRValue(cTransparentColor);
				BYTE lg = GetGValue(cTransparentColor);
				BYTE lb = GetBValue(cTransparentColor);

				BYTE hr = min(0xff, lr + GetRValue(cTolerance));
				BYTE hg = min(0xff, lg + GetGValue(cTolerance));
				BYTE hb = min(0xff, lb + GetBValue(cTolerance));

				// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
				BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
				for(int y=0; y<m_nWndHeight; y++)
				{
					// Scan each bitmap pixel from left to right
					for(int x=0; x<m_nWndWidth; x++)
					{
						// Search for a continuous range of "non transparent pixels"
						int x0 = x;
						LONG *p = (LONG *)p32 + x;

						while(x<m_nWndWidth)
						{
							BYTE b = GetRValue(*p);
							if(b>=lr && b<=hr)
							{
								b = GetGValue(*p);
								if(b>=lg && b<=hg)
								{
									b = GetBValue(*p);
									if(b>=lb && b<=hb)// This pixel is "transparent"
										break;
								}
							}
							p++;
							x++;
						}

						if(x>x0)
						{
							// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
							if(pData->rdh.nCount >= maxRects)
							{
								GlobalUnlock(hData);
								maxRects += ALLOC_UNIT;
								hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
								pData = (RGNDATA *)GlobalLock(hData);
							}
							RECT *pr = (RECT *)&pData->Buffer;
							SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);

							if(x0 < pData->rdh.rcBound.left)
								pData->rdh.rcBound.left = x0;
							if(y < pData->rdh.rcBound.top)
								pData->rdh.rcBound.top = y;
							if(x > pData->rdh.rcBound.right)
								pData->rdh.rcBound.right = x;
							if(y+1 > pData->rdh.rcBound.bottom)
								pData->rdh.rcBound.bottom = y+1;

							pData->rdh.nCount++;
						}
					}

					// Go to next row (remember, the bitmap is inverted vertically)
					p32 -= bm32.bmWidthBytes;
				}

				// Create or extend the region with the remaining rectangles
				HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
				if(hRgn)
				{
					CombineRgn(hRgn, hRgn, h, RGN_OR);
					DeleteObject(h);
				}
				else
					hRgn = h;

				// Clean up
				GlobalFree(hData);

				SelectObject(dcMem.GetSafeHdc(), holdBmp);
				DeleteObject(holdBmp);
				DeleteDC(dcMem);
			}

			DeleteObject(SelectObject(memDC.GetSafeHdc(), holdBmp));
		}

		DeleteDC(memDC);

		return hRgn;
    }
    
private:
	CWnd *m_pParentWnd;
	HBITMAP hBmp;
	long m_nWndWidth;
	long m_nWndHeight;
};

#endif
