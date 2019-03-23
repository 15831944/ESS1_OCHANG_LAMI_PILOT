// cdib.h
// new version for WIN32
#ifndef CDIB_H
#define CDIB_H

class CDib : public CObject
{
    enum Alloc {noAlloc, crtAlloc,
                heapAlloc}; // applies to BITMAPINFOHEADER
    DECLARE_SERIAL(CDib)
public:
    LPVOID  m_lpvColorTable;
    HBITMAP m_hBitmap;
    LPBYTE  m_lpImage;  // starting address of DIB bits
    LPBITMAPINFOHEADER m_lpBMIH; // buffer containing the
                                 //  BITMAPINFOHEADER
private:
    HGLOBAL m_hGlobal; // for external windows we need to free
                       // could be allocated by this class or
                       //  allocated externally
    Alloc m_nBmihAlloc;
    Alloc m_nImageAlloc;
    DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER
                         //  or BITMAPFILEHEADER
    int m_nColorTableEntries;

    HANDLE m_hFile;
    HANDLE m_hMap;
    LPVOID m_lpvFile;
    HPALETTE m_hPalette;
public:
    CDib();
    CDib(long nWidth, long nHeight, int nBitCount);  // builds BITMAPINFOHEADER
    ~CDib();
    int GetSizeImage() {return m_dwSizeImage;}
    int GetSizeHeader()
        {return sizeof(BITMAPINFOHEADER) +
                sizeof(RGBQUAD) * m_nColorTableEntries;}
    CSize GetDimensions();
    BOOL AttachMapFile(TCHAR* strPathname, BOOL bShare = FALSE);
    BOOL CopyToMapFile(TCHAR* strPathname);
    BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE,
        HGLOBAL hGlobal = NULL);
    BOOL Draw(CDC* pDC, CPoint origin,
        CSize size); // until we implement CreateDibSection
    HBITMAP CreateSection(CDC* pDC = NULL);
    UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
    BOOL MakePalette();
    BOOL SetSystemPalette(CDC* pDC);
    BOOL Compress(CDC* pDC,
        BOOL bCompress = TRUE); // FALSE means decompress
    HBITMAP CreateBitmap(CDC* pDC);
    BOOL Read(CFile* pFile);
    BOOL Read8BitRaw(CFile* pFile, int nWidth, int nHeight);
    BOOL Write(CFile* pFile);

	//Made By Y.W KO
    BOOL Write8BitRaw(CFile* pFile);
    BOOL Write8BitGrayBMP(CFile* pFile, LPBYTE fm, int width, int height);
	BOOL SaveImageBMP(LPCTSTR strFileName, UCHAR *pImage, long width, long height);
	void MemoryDC2BMP(HDC hDC, HBITMAP hbit, TCHAR *Path);

    void Serialize(CArchive& ar);
    void Empty();

	BOOL     DrawEdgeCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
	BOOL     DrawFillCircle(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
    BOOL     DrawLine(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
    BOOL     DrawLine4(CDC *pDC, POINT2 ptStart1, POINT2 ptStart2, POINT2 ptStart3, POINT2 ptStart4, long nThick, COLORREF color);
    BOOL     DrawPoint(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
    BOOL     DrawText(CDC *pDC, long x, long y, CString string, long nThick, COLORREF color);
	BOOL     DrawCross(CDC *pDC, long x, long y, long radius, long nThick, COLORREF color);
	BOOL     DrawRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
    BOOL     DrawHeriProfile(CDC *pDC, long x1, long y1, long nNum, long *pData, long nScale, long nThick, COLORREF color);
    BOOL     DrawVertProfile(CDC *pDC, long x1, long y1, long nNum, long *pData, long nScale, long nThick, COLORREF color);
	BOOL     DrawFillRect(CDC *pDC, long x1, long y1, long x2, long y2, long nThick, COLORREF color);
	BOOL     DrawPolyRect(CDC *pDC, long x1, long y1, long x2, long y2, long width, long type, COLORREF color);
	BOOL     DrawPixel(CDC *pDC, long x, long y, long nThick, COLORREF color);
    BOOL     DrawPixel(CDC *pDC, long *pCX, long *pCY, long nNum, long nThick, COLORREF color);
    BOOL     DrawEllipse(CDC *pDC,long lCenterX, long lCenterY, float la, float lb, float angle, long nThick, COLORREF lpColor);
    void     DrawBitmap(CDC* pDC, HBITMAP hBitmap, CRect rcRect);
private:
    void DetachMapFile();
    void ComputePaletteSize(int nBitCount);
    void ComputeMetrics();
	int  MakeBMPFileHead(LPBITMAPFILEHEADER bmfh,int nWidth, int nHeight);
	BOOL MakeBMPRead(LPBITMAPINFOHEADER bmih, int nWidth, int nHeight);
    BOOL MakePalette(BOOL flag);
    void MakeBMPHeader(BITMAPINFOHEADER *pbmiHdr, BITMAPFILEHEADER *pbmfHdr, long width, long height);

};
#endif